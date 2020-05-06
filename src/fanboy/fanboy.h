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
#include <QTimer>

#include "fanboy/fan.h"
#include "fanboy/sensor.h"


class FanBoy : public QObject
{
        Q_OBJECT

        friend class Fan;

        QTimer           updateTimer;
        QString          fwVersion;
        QString          fwBuild;
        QList<Fan *>     fans;
        QList<Sensor *>  sensors;

        bool readConfig();

        bool setFanFixedDuty(quint8 index, quint8 duty);
        bool setFanMode(quint8 index, Fan::Mode mode);
        bool setFanSensor(quint8 index, quint8 sensor);
        bool setFanParam(quint8 index, const Fan::Param &param);

    public:

        static quint8  numFans();
        static quint8  numSensors();

        FanBoy(const char *path = nullptr, QObject *parent = nullptr);
        ~FanBoy();

        QString  version() const;
        Fan      *fan(quint8 index) const;
        Sensor   *sensor(quint8 index) const;

    public slots:

        void saveSettings();
        void loadSettings();
        void updateValues();
        void setUpdateInterval(quint8 sec);

    signals:

        void settingsSaved();
        void settingsLoaded();
        void error(QString message);
};
