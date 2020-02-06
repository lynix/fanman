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

#include "valuegroup.h"

#include <float.h>
#include <QGridLayout>


ValueGroup::ValueGroup(const QtCharts::QXYSeries &series, const char *format,
                       QWidget* parent) : QFrame(parent),
    series(series),
    format(format),
    min(DBL_MAX),
    max(-DBL_MAX),
    minLabel(new QLabel(QString::asprintf(format, "0"))),
    maxLabel(new QLabel(QString::asprintf(format, "0"))),
    currentLabel(new QLabel(QString::asprintf(format, "0")))
{
    setStyleSheet("QGroupBox { font-weight: normal; } ");
    setMinimumWidth(100);

    QGridLayout *l = new QGridLayout();

    QFont f = currentLabel->font();
    f.setPixelSize(17);
    currentLabel->setFont(f);
    QPalette p = currentLabel->palette();
    p.setColor(QPalette::WindowText, series.color());
    currentLabel->setPalette(p);

    l->addWidget(currentLabel, 0, 0, 1, 2, Qt::AlignRight);
    l->addWidget(new QLabel("Min"), 1, 0, Qt::AlignLeft);
    l->addWidget(minLabel, 1, 1, Qt::AlignRight);
    l->addWidget(new QLabel("Max"), 2, 0, Qt::AlignLeft);
    l->addWidget(maxLabel, 2, 1, Qt::AlignRight);

    setFrameStyle(QFrame::StyledPanel|QFrame::Plain);
    setLayout(l);

    connect(&series, &QtCharts::QXYSeries::pointAdded, this,
            &ValueGroup::valueAdded);
}

void ValueGroup::valueAdded(int index)
{
    qreal value = series.at(index).y();
    QString valueStr = QString::asprintf(format, value);

    if (value < min) {
        min = value;
        minLabel->setText(valueStr);
    }
    if (value > max) {
        max = value;
        maxLabel->setText(valueStr);
    }

    currentLabel->setText(valueStr);
}
