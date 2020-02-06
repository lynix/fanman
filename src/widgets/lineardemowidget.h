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


class LinearDemoWidget : public QChartView
{
        Q_OBJECT

        qreal   highTemp;
        qreal   lowTemp;
        quint8  highDuty;
        quint8  lowDuty;

        void updateGraph();

    public:

        LinearDemoWidget(QWidget *parent = nullptr);
        void setEnabled(bool enabled);

    public slots:

        void setHighTemp(qreal temp);
        void setLowTemp(qreal temp);
        void setHighDuty(quint8 duty);
        void setLowDuty(quint8 duty);
};
