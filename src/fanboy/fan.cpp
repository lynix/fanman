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


Fan::Fan(FanBoy* device, quint8 index) : Port(device, FAN, index),
    mMode(MODE_MANUAL),
    mSensor(0),
    mState({0, 0}),
    mParam({0, 0, 0, 0})
{
}

void Fan::injectState(Fan::State state)
{
    mState = state;
    emit stateUpdated(mState);
}

void Fan::injectMode(Mode mode)
{
    if (mode != mMode) {
        mMode = mode;
        emit modeChanged(mode);
    }
}

void Fan::injectMapping(quint8 sensor)
{
    if (mSensor != sensor) {
        mSensor = sensor;
        emit sensorChanged(sensor);
    }
}

void Fan::injectParam(const Param &param)
{
    if (mParam != param) {
        mParam = param;
        emit paramChanged(param);
    }
}

void Fan::setMode(Mode mode)
{
    if (mMode != mode)
        device->setFanMode(mIndex, mode);
}

void Fan::setSensor(quint8 index)
{
    if (mSensor != index)
        device->setFanSensor(mIndex, index);
}

void Fan::setParam(const Param &param)
{
    if (mParam != param)
        device->setFanParam(mIndex, param);
}

void Fan::setFixedDuty(quint8 duty)
{
    setMode(MODE_MANUAL);
    device->setFanFixedDuty(mIndex, duty);
}
