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

#include "portwidget.h"

#include "widgets/rangecontrol.h"

#include <QHBoxLayout>
#include <QChartView>


PortWidget::PortWidget(const Port* port, QWidget *parent) : QGroupBox(parent)
{
    setTitle(port->name());
    setStyleSheet("QGroupBox { font-weight: bold; } ");

    chart = new ScrollingTimeChart();

    QChartView *chartView = new QChartView(chart, this);
    chartView->setRenderHint(QPainter::Antialiasing);
    chart->layout()->setContentsMargins(0, 0, 0, 0);
    chart->setBackgroundRoundness(0);

    vBar = new QVBoxLayout();
    vBar->addWidget(new RangeControl(chart));
    vBar->insertStretch(-1);

    configButton = new QPushButton("Configure");
    vBar->addWidget(configButton);

    QHBoxLayout *hlayout = new QHBoxLayout(this);
    hlayout->addWidget(chartView);
    hlayout->addLayout(vBar);
    setLayout(hlayout);

    connect(port, &Port::renamed, this, &PortWidget::sensorRenamed);
    connect(port, &Port::unplugged, this, &PortWidget::sensorDisconnected);

    connect(configButton, &QPushButton::clicked, this,
            &PortWidget::disableConfig);
    connect(configButton, &QPushButton::clicked, this,
            &PortWidget::configClicked);
}

void PortWidget::sensorRenamed(const QString &name)
{
    setTitle(name);
}

void PortWidget::sensorDisconnected()
{
   setEnabled(false);
   chart->setEnabled(false);
}

void PortWidget::enableConfig()
{
   configButton->setEnabled(true);
}

void PortWidget::disableConfig()
{
   configButton->setEnabled(false);
}
