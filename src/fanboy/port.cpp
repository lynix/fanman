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

#include "port.h"

#include "fanboy/fanboy.h"
#include "settings.h"


Port::Port(FanBoy* device, Type type, quint8 index) : QObject(device),
    device(device),
    mIndex(index),
    mName(Settings::instance().portName(type, index)),
    mPlugged(true)
{
}

void Port::setName(const QString& name)
{
    if (name != mName) {
        mName = name;
        Settings::instance().setPortName(type(), mIndex, name);
        emit renamed(name);
    }
}

void Port::unplug()
{
    if (mPlugged) {
        mPlugged = false;
        emit unplugged();
    }
}
