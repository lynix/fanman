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

#include "scrollingtimechart.h"

#include <QDateTimeAxis>


static const quint32 DEFAULT_INTERVAL = 1000;
static const quint32 DEFAULT_TIMESPAN = 60;
static const qreal   DISABLED_OPACITY = 0.5;

ScrollingTimeChart::ScrollingTimeChart(QGraphicsItem *parent,
                                       Qt::WindowFlags wFlags) :
    QChart(parent, wFlags),
    scrollOnNewValue(true),
    timedScroll(false),
    timespan(DEFAULT_TIMESPAN)
{
    legend()->hide();
    setMargins(QMargins(3, 3, 3, 0));

    timeAxis = new QDateTimeAxis();
    timeAxis->setFormat("hh:mm:ss");
    timeAxis->setLabelsVisible(false);
    addAxis(timeAxis, Qt::AlignBottom);

    scrollTimer.setInterval(DEFAULT_INTERVAL);
    connect(&scrollTimer, &QTimer::timeout, this,
            &ScrollingTimeChart::scrollChart);
}

void ScrollingTimeChart::setEnabled(bool enabled)
{
    if (timedScroll)
        enabled ? scrollTimer.start() : scrollTimer.stop();

    setOpacity(enabled ? 1.0 : DISABLED_OPACITY);

    QChart::setEnabled(enabled);
}

void ScrollingTimeChart::setTimedScroll(bool enable)
{
   timedScroll = enable;

    if (enable)
        scrollTimer.start();
    else
        scrollTimer.stop();
}

void ScrollingTimeChart::setScrollIntervalMs(quint32 value)
{
    scrollTimer.setInterval(value);
}

void ScrollingTimeChart::setScrollOnNewValue(bool enable)
{
    scrollOnNewValue = enable;
}

void ScrollingTimeChart::setTimeSpanSec(quint32 value)
{
    timespan = value;
    scrollChart();
}

void ScrollingTimeChart::addSeries(QAbstractSeries *series)
{
    QChart::addSeries(series);

    series->attachAxis(timeAxis);

    QLineSeries *s = dynamic_cast<QLineSeries *>(series);
    if (s)
        connect(s, &QLineSeries::pointAdded, this,
                &ScrollingTimeChart::seriesHasNewData);
}

void ScrollingTimeChart::removeSeries(QAbstractSeries *series)
{
    series->detachAxis(timeAxis);

    QLineSeries *s = dynamic_cast<QLineSeries *>(series);
    if (s)
        disconnect(s, &QLineSeries::pointAdded, this,
                   &ScrollingTimeChart::seriesHasNewData);

    QChart::removeSeries(series);
}

void ScrollingTimeChart::scrollChart()
{
    QDateTime now = QDateTime::currentDateTime();

    timeAxis->setRange(now.addSecs(-(qint64)timespan), now);
}

void ScrollingTimeChart::seriesHasNewData()
{
    if (scrollOnNewValue)
        scrollChart();
}
