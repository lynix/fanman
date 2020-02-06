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

#include <QSettings>

#include "fanboy/port.h"


class Settings
{
        QSettings s;

        Settings() {};
        Settings(Settings const &other) = delete;
        Settings(Settings &&other) = delete;

        QString portNameKey(Port::Type type, quint8 index);

    public:

        static Settings &instance()
        {
           static Settings i;
           return i;
        }

        bool windowVisible();
        QByteArray windowGeometry();
        QString portName(Port::Type type, quint8 index);

        void setWindowVisible(bool value);
        void setWindowGeometry(QByteArray geometry);
        void setPortName(Port::Type type, quint8 index, const QString &name);
};
