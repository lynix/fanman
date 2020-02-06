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

#include "fanwidget.h"

#include "widgets/valuegroup.h"
#include "windows/fandialog.h"


static quint16  DEFAULT_RPM_MAX = 2500;
static quint16  RPM_BORDER = 100;

FanWidget::FanWidget(Fan* fan, QWidget *parent) : PortWidget(fan, parent)
{
    dutyAxis = new QValueAxis();
    dutyAxis->setRange(0, 100);
    dutyAxis->setTickCount(5);
    dutyAxis->setMinorTickCount(4);
    dutyAxis->setLabelFormat("%d%%");
    dutyAxis->setLabelsColor(Qt::blue);
    chart->addAxis(dutyAxis, Qt::AlignLeft);

    dutyValues = new QLineSeries();
    dutyValues->setName("Duty");
    dutyValues->setPen(QPen(Qt::blue, 2, Qt::SolidLine, Qt::RoundCap,
                            Qt::RoundJoin));
    chart->addSeries(dutyValues);
    dutyValues->attachAxis(dutyAxis);

    rpmAxis = new QValueAxis();
    rpmAxis->setRange(0, DEFAULT_RPM_MAX);
    rpmAxis->setTickAnchor(0);
    rpmAxis->setTickInterval(10);
    rpmAxis->setLabelFormat("%d");
    rpmAxis->setLabelsColor(Qt::darkGreen);
    chart->addAxis(rpmAxis, Qt::AlignRight);

    rpmValues = new QLineSeries();
    rpmValues->setName("RPM");
    rpmValues->setPen(QPen(Qt::darkGreen, 2, Qt::SolidLine, Qt::RoundCap,
                           Qt::RoundJoin));
    chart->addSeries(rpmValues);
    rpmValues->attachAxis(rpmAxis);

    vBar->insertWidget(0, new ValueGroup(*rpmValues, "%.0f rpm"));
    vBar->insertWidget(1, new ValueGroup(*dutyValues, "%.0f%%"));

    connect(fan, &Fan::stateUpdated, this, &FanWidget::newFanState);
}

void FanWidget::newFanState(const Fan::State &state)
{
    QDateTime now = QDateTime::currentDateTime();

    static quint16 rpmMax = DEFAULT_RPM_MAX;
    if (state.rpm > rpmMax) {
        rpmMax = state.rpm;
        rpmAxis->setMax(rpmMax + RPM_BORDER);
    }

    rpmValues->append(now.toMSecsSinceEpoch(), state.rpm);
    dutyValues->append(now.toMSecsSinceEpoch(), state.duty);
}
