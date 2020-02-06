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

#include "widgets/lineardemowidget.h"


static qreal   DEFAULT_TEMP_LOW  = 25.0;
static qreal   DEFAULT_TEMP_HIGH = 35.0;
static quint8  DEFAULT_DUTY_LOW  = 25;
static quint8  DEFAULT_DUTY_HIGH = 75;
static qreal   BORDER_TEMP       = 10;
static int     CHART_MIN_WIDTH   = 300;
static int     CHART_MIN_HEIGHT  = 230;
static int     CHART_FONT_SIZE   = 8;

LinearDemoWidget::LinearDemoWidget(QWidget *parent) :
    QChartView(new QChart(), parent),
    highTemp(DEFAULT_TEMP_HIGH),
    lowTemp(DEFAULT_TEMP_LOW),
    highDuty(DEFAULT_DUTY_HIGH),
    lowDuty(DEFAULT_DUTY_LOW)
{
    setMinimumSize(CHART_MIN_WIDTH, CHART_MIN_HEIGHT);
    setRenderHint(QPainter::Antialiasing);
    chart()->legend()->hide();
    chart()->setMargins(QMargins(5, 5, 5, 0));
    chart()->setBackgroundRoundness(0);
    chart()->layout()->setContentsMargins(0, 0, 0, 0);

    QValueAxis *tempAxis = new QValueAxis();
    QFont f(tempAxis->labelsFont());
    f.setPointSize(CHART_FONT_SIZE);
    tempAxis->setLabelsFont(f);
    tempAxis->setTickType(QValueAxis::TicksDynamic);
    tempAxis->setTickAnchor(20);
    tempAxis->setTickInterval(5);
    tempAxis->setLabelFormat("%.1f");
    chart()->addAxis(tempAxis, Qt::AlignBottom);

    QValueAxis *dutyAxis = new QValueAxis();
    f = QFont(dutyAxis->labelsFont());
    f.setPointSize(CHART_FONT_SIZE);
    dutyAxis->setLabelsFont(f);
    dutyAxis->setTickCount(5);
    dutyAxis->setLabelFormat("%.0f%%");
    dutyAxis->setRange(0, 100);
    chart()->addAxis(dutyAxis, Qt::AlignLeft);

    QLineSeries *dutyValues = new QLineSeries();
    dutyValues->append(QPoint(lowTemp-BORDER_TEMP, lowDuty));
    dutyValues->append(QPoint(lowTemp, lowDuty));
    dutyValues->append(QPoint(highTemp, highDuty));
    dutyValues->append(QPoint(highTemp+BORDER_TEMP, highDuty));
    dutyValues->setPen(QPen(Qt::blue, 2, Qt::SolidLine, Qt::RoundCap,
                            Qt::RoundJoin));
    chart()->addSeries(dutyValues);
    dutyValues->attachAxis(tempAxis);
    dutyValues->attachAxis(dutyAxis);
}

void LinearDemoWidget::setHighTemp(qreal temp)
{
    highTemp = temp;
    updateGraph();
}

void LinearDemoWidget::setLowTemp(qreal temp)
{
    lowTemp = temp;
    updateGraph();
}

void LinearDemoWidget::setHighDuty(quint8 duty)
{
    highDuty = duty;
    updateGraph();
}

void LinearDemoWidget::setLowDuty(quint8 duty)
{
    lowDuty = duty;
    updateGraph();
}

void LinearDemoWidget::setEnabled(bool enabled)
{
    chart()->setOpacity(enabled ? 1.0 : 0.5);
    QChartView::setEnabled(enabled);
}

void LinearDemoWidget::updateGraph()
{
    QVector<QPointF> points;
    points.append(QPoint(lowTemp-BORDER_TEMP, lowDuty));
    points.append(QPoint(lowTemp, lowDuty));
    points.append(QPoint(highTemp, highDuty));
    points.append(QPoint(highTemp+BORDER_TEMP, highDuty));

    QLineSeries *series = static_cast<QLineSeries *>(chart()->series().first());
    series->replace(points);

    chart()->axes(Qt::Horizontal).first()->setRange(lowTemp-BORDER_TEMP,
                                                    highTemp+BORDER_TEMP);
}
