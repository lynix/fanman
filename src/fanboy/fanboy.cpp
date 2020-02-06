/*  FanMan: A GUI for FanBoy
 *
 *  Copyright (C) 2020  Alexander Koch
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#include "fanboy.h"

#include <QDateTime>
#include <QDebug>
#include <QRegularExpression>
#include <QSerialPortInfo>
#include <QThread>


quint16 FanBoy::USB_VENDOR_ID    = 0x2341;
quint16 FanBoy::USB_PRODUCT_ID   = 0x8036;

quint8  FanBoy::NUM_FANS    = 4;
quint8  FanBoy::NUM_SENSORS = 2;

static quint16 DEFAULT_UPDATE_INT = 1;

static const char *CMD_VERSION  = "version";
static const char *CMD_STATUS   = "status";
static const char *CMD_SAVE     = "save";
static const char *CMD_LOAD     = "load";
static const char *CMD_SET      = "set";
static const char *CMD_MODE     = "mode";
static const char *CMD_MAP      = "map";
static const char *CMD_LINEAR   = "linear";
static const char *STATE_DISC   = "disconnected";

static QRegularExpression RE_FLOAD("^Failed to load settings from EEPROM!$");
static QRegularExpression RE_FMAP("^Fan (\\d) mapped to sensor (\\d)$");
static QRegularExpression RE_FMODE("^Fan (\\d) set to mode '(.+)'$");
static QRegularExpression RE_FPARA("^Fan (\\d) linear params (\\d+\\.\\d\\d),(\\d+),(\\d+\\.\\d\\d),(\\d+)$");
static QRegularExpression RE_SAVE("^Settings saved to EEPROM$");
static QRegularExpression RE_SET("^Setting fan \\d duty \\d+%$");
static QRegularExpression RE_SFAN("^Fan (\\d): ((\\d+)% @ (\\d+) rpm|disconnected)$");
static QRegularExpression RE_SLOAD("^Settings loaded from EEPROM$");
static QRegularExpression RE_SMODE("^Setting fan (\\d) mode '(.+)'$");
static QRegularExpression RE_STATUS("^Setting status interval \\d+$");
static QRegularExpression RE_STEMP("^Temp (\\d): ((\\d+\\.\\d\\d) C|disconnected)$");
static QRegularExpression RE_TIME("^Built: (.+)$");
static QRegularExpression RE_VERS("^Version: (.+)$");

FanBoy::FanBoy(const char* path, QObject *parent) : QObject(parent)
{
    QString portName(path);
    if (!path) {
        QList<QSerialPortInfo> ports = QSerialPortInfo::availablePorts();
        foreach (QSerialPortInfo portInfo, ports) {
            if (portInfo.vendorIdentifier() == FanBoy::USB_VENDOR_ID &&
                    portInfo.productIdentifier() == FanBoy::USB_PRODUCT_ID) {
                portName = portInfo.portName();
                break;
            }
        }
    }
    if (portName.isEmpty())
        throw QString("No FanBoy found");
    qDebug() << "Using port" << portName;

    port.setPortName(portName);
    port.setBaudRate(QSerialPort::Baud57600);
    port.setDataBits(QSerialPort::Data8);
    port.setParity(QSerialPort::NoParity);
    port.setStopBits(QSerialPort::OneStop);
    port.setFlowControl(QSerialPort::SoftwareControl);

    if (!port.open(QIODevice::ReadWrite))
        throw port.errorString();

    port.setRequestToSend(true);

    for (int i=0; i<NUM_FANS; i++)
        fans.append(new Fan(this, i));
    for (int i=0; i<NUM_SENSORS; i++)
        sensors.append(new Sensor(this, i));

    connect(&port, &QSerialPort::readyRead, this, &FanBoy::handleSerialData);
    connect(&port, &QSerialPort::errorOccurred, this, &FanBoy::serialError);

    sendCommand(CMD_VERSION);
    setUpdateInterval(DEFAULT_UPDATE_INT);
}

FanBoy::~FanBoy()
{
    disconnect(&port, &QSerialPort::readyRead, this, &FanBoy::handleSerialData);

    setUpdateInterval(0);
    port.waitForBytesWritten(1000);
    port.close();
}

void FanBoy::sendCommand(const QString &cmd)
{
    port.write(QString(cmd).append("\n").toLatin1());
}

void FanBoy::handleSerialData()
{
    while (port.canReadLine()) {
        QString line = QString::fromLatin1(port.readLine()).trimmed();
        QRegularExpressionMatch match;

        if ((match = RE_SFAN.match(line)).hasMatch())
            handleFanStatus(match);
        else if ((match = RE_STEMP.match(line)).hasMatch())
            handleSensorStatus(match);
        else if ((match = RE_FMODE.match(line)).hasMatch() ||
                 (match = match = RE_SMODE.match(line)).hasMatch())
            handleFanMode(match);
        else if ((match = RE_FMAP.match(line)).hasMatch())
            handleFanMap(match);
        else if ((match = RE_FPARA.match(line)).hasMatch())
            handleFanPara(match);
        else if ((match = RE_VERS.match(line)).hasMatch())
            handleVersion(match);
        else if ((match = RE_TIME.match(line)).hasMatch())
            handleTimestamp(match);
        else if ((match = RE_SET.match(line)).hasMatch())
            handleFixedDuty(match);
        else if ((match = RE_STATUS.match(line)).hasMatch())
            handleStatus(match);
        else if ((match = RE_SAVE.match(line)).hasMatch())
            emit settingsSaved();
        else if ((match = RE_SLOAD.match(line)).hasMatch())
            emit settingsLoaded(true);
        else if ((match = RE_FLOAD.match(line)).hasMatch())
            emit settingsLoaded(false);
        else
            qDebug() << "Unhandled message:" << line;
    }
}

void FanBoy::serialError(QSerialPort::SerialPortError error)
{
    qCritical() << "Serial error:" << error;
}

void FanBoy::handleFanStatus(const QRegularExpressionMatch &match)
{
    quint8 fanNo = match.captured(1).toUInt()-1;
    QString state = match.captured(2);
    if (state == STATE_DISC)
        fans[fanNo]->unplug();
    else {
        quint8 duty = match.captured(3).toUInt();
        quint16 rpm = match.captured(4).toUInt();
        fans[fanNo]->injectState({duty, rpm});
    }
}

void FanBoy::handleSensorStatus(const QRegularExpressionMatch &match)
{
    quint8 sensorNo = match.captured(1).toUInt()-1;
    QString state = match.captured(2);
    if (state == STATE_DISC)
        sensors[sensorNo]->unplug();
    else
        sensors[sensorNo]->injectValue(match.captured(3).toDouble());
}

void FanBoy::handleFanMode(const QRegularExpressionMatch &match)
{
    quint8 fanNo = match.captured(1).toUInt()-1;
    QString modeStr = match.captured(2);
    fans[fanNo]->injectMode(modeStr == "manual" ? Fan::MODE_MANUAL :
                                                  Fan::MODE_LINEAR);
}

void FanBoy::handleFanMap(const QRegularExpressionMatch &match)
{
    quint8 fanNo = match.captured(1).toUInt()-1;
    quint8 sensor = match.captured(2).toUInt()-1;
    fans[fanNo]->injectMapping(sensor);
}

void FanBoy::handleFanPara(const QRegularExpressionMatch &match)
{
    quint8 fanNo = match.captured(1).toUInt()-1;
    qreal lowTemp = match.captured(2).toDouble();
    quint8 lowDuty = match.captured(3).toUInt();
    qreal highTemp = match.captured(4).toDouble();
    quint8 highDuty = match.captured(5).toUInt();
    fans[fanNo]->injectParam({lowDuty, lowTemp, highDuty, highTemp});
}

void FanBoy::handleVersion(const QRegularExpressionMatch &match)
{
    fwVersion = match.captured(1);
    queryFans(); // hijack version report to trigger fan discovery

    qDebug() << "FanBoy firmware version:" << fwVersion;
}

void FanBoy::handleTimestamp(const QRegularExpressionMatch &match)
{
    fwBuild = match.captured(1);
}

void FanBoy::handleFixedDuty(const QRegularExpressionMatch &)
{
    // ignore
}

void FanBoy::handleStatus(const QRegularExpressionMatch &)
{
    // ignore
}

void FanBoy::queryFans()
{
    for (int i=0; i<NUM_FANS; i++) {
        sendCommand(QString(CMD_MODE).append(" %1").arg(i+1));
        sendCommand(QString(CMD_MAP).append(" %1").arg(i+1));
        sendCommand(QString(CMD_LINEAR).append(" %1").arg(i+1));
    }
}

void FanBoy::setFanFixedDuty(quint8 index, quint8 duty)
{
    sendCommand(QString(CMD_SET).append(" %1 %2").arg(index+1).arg(duty));
}

void FanBoy::setFanMode(quint8 index, Fan::Mode mode)
{
    QString command = QString(CMD_MODE).append(" %1 %2").arg(index+1);
    command = command.arg(mode == Fan::MODE_MANUAL ? "manual" : "linear");

    sendCommand(command);
}

void FanBoy::setFanSensor(quint8 index, quint8 sensor)
{
    sendCommand(QString(CMD_MAP).append(" %1 %2").arg(index+1).arg(sensor+1));
}

void FanBoy::setFanParam(quint8 index, const Fan::Param& param)
{
    QString command = QString(CMD_LINEAR).append(" %1 %2,%3,%4,%5");
    command = command.arg(index+1);
    command = command.arg(param.lowTemp).arg(param.lowDuty);
    command = command.arg(param.highTemp).arg(param.highDuty);

    sendCommand(command);
}

QString FanBoy::version() const
{
    return fwVersion;
}

Fan* FanBoy::fan(quint8 index) const
{
    Q_ASSERT(index < fans.size());

    return fans.at(index);
}

Sensor* FanBoy::sensor(quint8 index) const
{
    Q_ASSERT(index < sensors.size());

    return sensors.at(index);
}

void FanBoy::saveSettings()
{
    sendCommand(CMD_SAVE);
}

void FanBoy::loadSettings()
{
    sendCommand(CMD_LOAD);
}

void FanBoy::setUpdateInterval(quint8 sec)
{
    sendCommand(QString(CMD_STATUS).append(" %1").arg(sec));
}
