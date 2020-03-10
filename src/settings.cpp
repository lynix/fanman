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

#include "settings.h"


static const char *KEY_WINDOW_VISIBLE   = "MainWindow/state";
static const char *KEY_WINDOW_GEO       = "MainWindow/geometry";
static const char *KEY_FAN              = "Fan%1";
static const char *KEY_SENSOR           = "Sensor%1";
static const char *KEY_PROP_NAME        = "name";
static const char *KEY_PROP_RANGE       = "range";
static const char *DEFAULT_FAN_NAME     = "Fan %1";
static const char *DEFAULT_SENSOR_NAME  = "Sensor %1";
static const int  DEFAULT_RANGE         = 1;

QString Settings::portKey(Port::Type type, quint8 index, const char *property)
{
    QString key(type == Port::Type::FAN ? KEY_FAN : KEY_SENSOR);
    key = key.arg(index).append("/").append(property);

    return key;
}

bool Settings::windowVisible()
{
    return s.value(KEY_WINDOW_VISIBLE, true).toBool();
}

QByteArray Settings::windowGeometry()
{
    return s.value(KEY_WINDOW_GEO).toByteArray();
}

QString Settings::portName(Port::Type type, quint8 index)
{
    QString defName(type == Port::Type::FAN ? DEFAULT_FAN_NAME :
                                              DEFAULT_SENSOR_NAME);
    defName = defName.arg(index+1);

    return s.value(portKey(type, index, KEY_PROP_NAME), defName).toString();
}

int Settings::plotRange(Port::Type type, quint8 index)
{
    return s.value(portKey(type, index, KEY_PROP_RANGE), DEFAULT_RANGE).toInt();
}

void Settings::setWindowVisible(bool value)
{
    s.setValue(KEY_WINDOW_VISIBLE, value);
}

void Settings::setWindowGeometry(QByteArray geometry)
{
    s.setValue(KEY_WINDOW_GEO, geometry);
}

void Settings::setPortName(Port::Type type, quint8 index, const QString &name)
{
    s.setValue(portKey(type, index, KEY_PROP_NAME), name);
}

void Settings::setPlotRange(Port::Type type, quint8 index, int rangeIndex)
{
    s.setValue(portKey(type, index, KEY_PROP_RANGE), rangeIndex);
}
