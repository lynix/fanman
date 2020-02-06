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

#include "fandutyslider.h"


static const int STEP_SMALL = 1;
static const int STEP_LARGE = 10;

FanDutySlider::FanDutySlider(QWidget *parent) : QSlider(Qt::Horizontal, parent)
{
    setMinimum(0);
    setMaximum(100);
    setTickInterval(STEP_LARGE);
    setTickPosition(QSlider::TicksBelow);
    setSingleStep(STEP_SMALL);
    setPageStep(STEP_LARGE);

    connect(this, &QSlider::valueChanged, this, &FanDutySlider::newValue);
}

void FanDutySlider::newValue(int value)
{
    emit dutyStrChanged(QString::number(value).append("%"));
}
