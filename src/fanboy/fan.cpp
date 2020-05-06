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

#include "fan.h"

#include "fanboy/fanboy.h"


Fan::Fan(FanBoy* device, quint8 index, Mode mode, quint8 sensor, const Param &param) : Port(device, FAN, index),
    mMode(mode),
    mSensor(sensor),
    mState({ .duty = 0, .rpm = 0 }),
    mParam(param)
{
}

void Fan::injectState(Fan::State state)
{
    mState = state;
    emit stateUpdated(mState);
}

void Fan::setMode(Mode mode)
{
    if (mMode != mode) {
        if (device->setFanMode(mIndex, mode)) {
            mMode = mode;
            emit modeChanged(mode);
        }
    }
}

void Fan::setSensor(quint8 index)
{
    if (mSensor != index) {
        if (device->setFanSensor(mIndex, index)) {
            mSensor = index;
            emit sensorChanged(index);
        }
    }
}

void Fan::setParam(const Param &param)
{
    if (mParam != param) {
        if (device->setFanParam(mIndex, param)) {
            mParam = param;
            emit paramChanged(param);
        }
    }
}

void Fan::setFixedDuty(quint8 duty)
{
    if (device->setFanFixedDuty(mIndex, duty)) {
        if (mMode != MODE_MANUAL) {
            mMode = MODE_MANUAL;
            emit modeChanged(mMode);
        }
        emit fixedDutyChanged(duty);
    }
}
