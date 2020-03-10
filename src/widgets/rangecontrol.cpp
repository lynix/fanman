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

#include "rangecontrol.h"

#include <QHBoxLayout>


RangeControl::RangeControl(const ScrollingTimeChart *chart, int rangeIndex) :
    chart(chart),
    box(new QComboBox())
{
    setTitle("Plot");
    setStyleSheet("QGroupBox { font-weight: normal; } ");

    box->addItem("10 s", 10);
    box->addItem("60 s", 60);
    box->addItem("5 min", 5 * 60);
    box->addItem("10 min", 10 * 60);
    box->addItem("30 min", 30 * 60);
    box->addItem("60 min", 60 * 60);
    box->setCurrentIndex(rangeIndex);

    setLayout(new QHBoxLayout());
    layout()->addWidget(box);

    connect(box, QOverload<int>::of(&QComboBox::currentIndexChanged), this,
            &RangeControl::rangeControlIndexChanged);
    connect(this, &RangeControl::rangeChanged, chart,
            &ScrollingTimeChart::setTimeSpanSec);
}

void RangeControl::rangeControlIndexChanged(int index)
{
    emit rangeIndexChanged(index);
    emit rangeChanged(box->itemData(index).toUInt());
}
