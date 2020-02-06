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

#include "sensorwidget.h"

#include <QDateTime>
#include <QDebug>

#include "widgets/valuegroup.h"


static const qreal  DEFAULT_TEMP_MIN = 20;
static const qreal  DEFAULT_TEMP_MAX = 40;
static const qreal  BORDER_FACTOR    = 0.1;
static const int    TICKS_MAJOR      = 5;
static const int    TICKS_MINOR      = 4;

SensorWidget::SensorWidget(Sensor* sensor, QWidget *parent) :
    PortWidget(sensor, parent)
{
    axisY = new QValueAxis();
    axisY->setRange(DEFAULT_TEMP_MIN, DEFAULT_TEMP_MAX);
    axisY->setTickCount(TICKS_MAJOR);
    axisY->setMinorTickCount(TICKS_MINOR);
    axisY->setLabelFormat("%.0f");
    chart->addAxis(axisY, Qt::AlignLeft);

    values = new QLineSeries();
    values->setName("Temperature");
    values->setPen(QPen(Qt::red, 2, Qt::SolidLine, Qt::RoundCap,
                        Qt::RoundJoin));
    chart->addSeries(values);
    values->attachAxis(axisY);

    vBar->insertWidget(0, new ValueGroup(*values, "%.1f °C"));

    connect(sensor, &Sensor::newValue, this, &SensorWidget::newValue);
}

void SensorWidget::newValue(qreal value)
{
    QDateTime now = QDateTime::currentDateTime();

    static qreal min = DEFAULT_TEMP_MIN;
    if (value < min) {
        min = value;
        axisY->setMin(min - (BORDER_FACTOR * qAbs(min)));
    }
    static qreal max = DEFAULT_TEMP_MAX;
    if (value > max) {
        max = value;
        axisY->setMax(max + (BORDER_FACTOR * qAbs(max)));
    }

    values->append(now.toMSecsSinceEpoch(), value);
}
