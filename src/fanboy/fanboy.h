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

#pragma once

#include <QList>
#include <QObject>
#include <QSerialPort>

#include "fanboy/fan.h"
#include "fanboy/sensor.h"


class FanBoy : public QObject
{
        Q_OBJECT

        friend class Fan;

        QSerialPort      port;
        QString          fwVersion;
        QString          fwBuild;
        QList<Fan *>     fans;
        QList<Sensor *>  sensors;

        void sendCommand(const QString &cmd);
        void queryFans();

        void handleFanStatus(const QRegularExpressionMatch &match);
        void handleSensorStatus(const QRegularExpressionMatch &match);
        void handleFanMode(const QRegularExpressionMatch &match);
        void handleFanMap(const QRegularExpressionMatch &match);
        void handleFanPara(const QRegularExpressionMatch &match);
        void handleVersion(const QRegularExpressionMatch &match);
        void handleTimestamp(const QRegularExpressionMatch &match);
        void handleFixedDuty(const QRegularExpressionMatch &match);
        void handleStatus(const QRegularExpressionMatch &match);

    public:

        static quint16 USB_VENDOR_ID;
        static quint16 USB_PRODUCT_ID;
        static quint8  NUM_FANS;
        static quint8  NUM_SENSORS;

        FanBoy(const char *path = nullptr, QObject *parent = nullptr);
        ~FanBoy();

        QString  version() const;
        Fan      *fan(quint8 index) const;
        Sensor   *sensor(quint8 index) const;

    private slots:

        void handleSerialData();
        void serialError(QSerialPort::SerialPortError serialPortError);
        void setFanFixedDuty(quint8 index, quint8 duty);
        void setFanMode(quint8 index, Fan::Mode mode);
        void setFanSensor(quint8 index, quint8 sensor);
        void setFanParam(quint8 index, const Fan::Param &param);

    public slots:

        void saveSettings();
        void loadSettings();
        void setUpdateInterval(quint8 sec);

    signals:

        void settingsSaved();
        void settingsLoaded(bool success);
};
