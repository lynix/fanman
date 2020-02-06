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

#include <QGroupBox>
#include <QPushButton>
#include <QVBoxLayout>

#include "fanboy/port.h"
#include "widgets/scrollingtimechart.h"


class PortWidget : public QGroupBox
{
        Q_OBJECT

        QPushButton         *configButton;

    protected:

        ScrollingTimeChart  *chart;
        QVBoxLayout         *vBar;

        PortWidget(const Port *port, QWidget *parent = nullptr);

    public slots:

        void sensorRenamed(const QString &name);
        void sensorDisconnected();
        void enableConfig();
        void disableConfig();

    signals:

        void configClicked();

};