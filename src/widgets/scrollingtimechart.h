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

#include <QtCharts>


class ScrollingTimeChart : public QChart
{
        Q_OBJECT

        QTimer          scrollTimer;
        QDateTimeAxis   *timeAxis;
        bool            scrollOnNewValue;
        bool            timedScroll;
        quint32         timespan;

    public:

        ScrollingTimeChart(QGraphicsItem *parent = nullptr,
                           Qt::WindowFlags wFlags = Qt::WindowFlags());

        void setEnabled(bool enabled);
        void addSeries(QAbstractSeries *series);
        void removeSeries(QAbstractSeries *series);
        void setScrollIntervalMs(quint32 value);
        void setTimedScroll(bool enable);
        void setScrollOnNewValue(bool enable);

    private slots:

        void scrollChart();
        void seriesHasNewData();

    public slots:

        void setTimeSpanSec(quint32 value);
};
