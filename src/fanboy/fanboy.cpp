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

#include <libfanboy.h>

#include <QDebug>
#include <QSerialPortInfo>


static quint16 USB_VENDOR_ID      = 0x2341;
static quint16 USB_PRODUCT_ID     = 0x8036;
static quint16 DEFAULT_UPDATE_INT = 1000;


FanBoy::FanBoy(const char* path, QObject *parent) : QObject(parent)
{
    QString port(path);
    if (!path) {
        QList<QSerialPortInfo> ports = QSerialPortInfo::availablePorts();
        foreach (QSerialPortInfo portInfo, ports) {
            if (portInfo.vendorIdentifier() == USB_VENDOR_ID &&
                    portInfo.productIdentifier() == USB_PRODUCT_ID) {
                port = portInfo.systemLocation();
                break;
            }
        }
    }
    if (port.isEmpty()) {
        qCritical() << "No FanBoy found";
        return;
    }
    qDebug() << "Using port" << port;

    if (!fb_init(port.toLatin1().toStdString().c_str())) {
        qCritical() << "Failed to initialize FanBoy:"
                    << fb_error();
        return;
    }

    if (!readConfig())
        return;

    connect(&updateTimer, &QTimer::timeout, this, &FanBoy::updateValues);
    updateTimer.start(DEFAULT_UPDATE_INT);
}

FanBoy::~FanBoy()
{
    updateTimer.stop();
}

bool FanBoy::readConfig()
{
    fb_version_t vers;
    if (!fb_version(&vers)) {
        fb_exit();
        qCritical() << "Failed to read firmware version:"
                    << fb_error();
        return false;
    }
    fwVersion = vers.version;
    fwBuild = vers.build;

    fb_config_t config;
    if (!fb_config(&config)) {
        fb_exit();
        qCritical() << "Failed to read config:"
                    << fb_error();
        return false;
    }

    for (int i=0; i<NUM_FAN; i++) {
        Fan::Mode mode = config.fan[i].mode == MODE_MANUAL ?
                         Fan::MODE_MANUAL : Fan::MODE_LINEAR;
        Fan::Param param = { .lowDuty = config.fan[i].param.min_duty,
                             .lowTemp = (qreal)config.fan[i].param.min_temp / 100.0,
                             .highDuty = config.fan[i].param.max_duty,
                             .highTemp = (qreal)config.fan[i].param.max_temp / 100.0 };
        fans.append(new Fan(this, i, mode, config.fan[i].sensor, param));
    }
    for (int i=0; i<NUM_TEMP; i++) {
        sensors.append(new Sensor(this, i));
    }
    // TODO: reflect temperature unit selection

    return true;
}

void FanBoy::updateValues()
{
    fb_status_t status;
    if (!fb_status(&status)) {
        emit error(QString("Failed to query FanBoy status: %s").append(fb_error()));
        return;
    }

    for (int i=0; i<NUM_FAN; i++) {
        if (status.fan[i].rpm == NCONN)
            fans[i]->unplug();
        else
            fans[i]->injectState({ .duty = status.fan[i].duty, .rpm = status.fan[i].rpm });
    }

    for (int i=0; i<NUM_TEMP; i++) {
        if (status.temp[i] == NCONN)
            sensors[i]->unplug();
        else
            sensors[i]->injectValue((qreal)status.temp[i] / 100.0);
    }
}

bool FanBoy::setFanFixedDuty(quint8 index, quint8 duty)
{
    bool rc = fb_set_duty(index, duty);
    if (!rc)
        emit error(QString("Failed to set fan duty: %s").append(fb_error()));

    return rc;
}

bool FanBoy::setFanMode(quint8 index, Fan::Mode mode)
{
    bool rc = fb_set_mode(index, mode == Fan::MODE_MANUAL ?
                              MODE_MANUAL : MODE_LINEAR);
    if (!rc)
        emit error(QString("Failed to set fan mode: %s").append(fb_error()));

    return rc;
}

bool FanBoy::setFanSensor(quint8 index, quint8 sensor)
{
    bool rc = fb_set_map(index, sensor);
    if (!rc)
        emit error(QString("Failed to set fan mapping: %s").append(fb_error()));

    return rc;
}

bool FanBoy::setFanParam(quint8 index, const Fan::Param& param)
{
    linear_t params = { .min_temp = static_cast<uint16_t>(param.lowTemp * 100),
                        .min_duty = param.lowDuty,
                        .max_temp = static_cast<uint16_t>(param.highTemp * 100),
                        .max_duty = param.highDuty };

    bool rc = fb_set_linear(index, &params);
    if (!rc)
        emit error(QString("Failed to set params: %s").append(fb_error()));

    return rc;
}

QString FanBoy::version() const
{
    return fwVersion;
}

quint8 FanBoy::numFans()
{
    return NUM_FAN;
}

quint8 FanBoy::numSensors()
{
    return NUM_TEMP;
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
    if (fb_save())
        emit settingsSaved();
    else
        emit error(QString("Failed to save settings: ").append(fb_error()));
}

void FanBoy::loadSettings()
{
    if (fb_load())
        emit settingsLoaded();
    else
        emit error(QString("Failed to load settings: ").append(fb_error()));
}

void FanBoy::setUpdateInterval(quint8 sec)
{
    updateTimer.setInterval(sec * 1000);
}
