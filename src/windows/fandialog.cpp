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

#include "fandialog.h"
#include "fanboy/fanboy.h"

#include <QHBoxLayout>
#include <QLabel>
#include <QRadioButton>

#include "widgets/lineardemowidget.h"


FanDialog::FanDialog(Fan *fan, QWidget *parent, Qt::WindowFlags f) :
    PortDialog(fan, parent, f),
    fan(fan)
{
    // sensor mapping
    gridLayout->addWidget(new QLabel("Sensor:"), gridLayout->rowCount(), 0);
    mappedSensor = new QComboBox();
    for (int i=0; i<FanBoy::numSensors(); i++)
        mappedSensor->addItem(QString("Sensor %1").arg(i+1), i);
    mappedSensor->setCurrentIndex(fan->sensor());
    gridLayout->addWidget(mappedSensor, gridLayout->rowCount()-1, 1);

    // mode selection
    gridLayout->addWidget(new QLabel("Mode:"), gridLayout->rowCount(), 0);
    QRadioButton *manual = new QRadioButton("manual");
    manual->setChecked(fan->mode() == Fan::MODE_MANUAL);
    QRadioButton *linear = new QRadioButton("linear");
    linear->setChecked(fan->mode() == Fan::MODE_LINEAR);
    modeGroup = new QButtonGroup();
    modeGroup->addButton(manual, static_cast<int>(Fan::MODE_MANUAL));
    modeGroup->addButton(linear, static_cast<int>(Fan::MODE_LINEAR));
    QHBoxLayout *modeLayout = new QHBoxLayout();
    modeLayout->addWidget(manual);
    modeLayout->addWidget(linear);
    modeLayout->insertStretch(-1);
    gridLayout->addLayout(modeLayout, gridLayout->rowCount()-1, 1);

    // fixed duty
    gridLayout->addWidget(new QLabel("Manual:"), gridLayout->rowCount(), 0);
    QHBoxLayout *dutyLayout = new QHBoxLayout();
    fixedDutySlider = new FanDutySlider();
    fixedDutySlider->setValue(fan->state().duty);
    QLabel *fixedDutyLabel = new QLabel(QString::number(
                                            fan->state().duty).append("%"));
    dutyLayout->addWidget(fixedDutySlider);
    dutyLayout->addWidget(fixedDutyLabel);
    QWidget *fixedWidgets = new QWidget();
    fixedWidgets->setLayout(dutyLayout);
    fixedWidgets->setEnabled(fan->mode() == Fan::Mode::MODE_MANUAL);
    gridLayout->addWidget(fixedWidgets, gridLayout->rowCount()-1, 1);

    // linear parameters
    gridLayout->addWidget(new QLabel("Linear:"), gridLayout->rowCount(), 0,
                          Qt::AlignLeft|Qt::AlignTop);
    QVBoxLayout *linearLayout = new QVBoxLayout();
    QHBoxLayout *linearLowDutyLayout = new QHBoxLayout();
    minDutySlider = new FanDutySlider();
    const Fan::Param params = fan->param();
    minDutySlider->setValue(params.lowDuty);
    QLabel *minDutyLabel = new QLabel(QString::number(
                                          params.lowDuty).append("%"));
    linearLowDutyLayout->addWidget(new QLabel("Low Duty:"));
    linearLowDutyLayout->addWidget(minDutySlider);
    linearLowDutyLayout->addWidget(minDutyLabel);
    linearLayout->addLayout(linearLowDutyLayout);

    QHBoxLayout *linearLowTempLayout = new QHBoxLayout();
    minTemp = new QDoubleSpinBox();
    minTemp->setSuffix(" °C");
    minTemp->setDecimals(1);
    minTemp->setValue(params.lowTemp);
    linearLowTempLayout->addWidget(new QLabel("Low Temp:"));
    linearLowTempLayout->addWidget(minTemp);
    linearLayout->addLayout(linearLowTempLayout);

    QHBoxLayout *linearHighDutyLayout = new QHBoxLayout();
    maxDutySlider = new FanDutySlider();
    maxDutySlider->setValue(params.highDuty);
    QLabel *maxDutyLabel = new QLabel(QString::number(
                                          params.highDuty).append("%"));
    linearHighDutyLayout->addWidget(new QLabel("High Duty:"));
    linearHighDutyLayout->addWidget(maxDutySlider);
    linearHighDutyLayout->addWidget(maxDutyLabel);
    linearLayout->addLayout(linearHighDutyLayout);

    QHBoxLayout *linearHighTempLayout = new QHBoxLayout();
    maxTemp = new QDoubleSpinBox();
    maxTemp->setValue(params.highTemp);
    maxTemp->setSuffix(" °C");
    maxTemp->setDecimals(1);
    linearHighTempLayout->addWidget(new QLabel("High Temp:"));
    linearHighTempLayout->addWidget(maxTemp);
    linearLayout->addLayout(linearHighTempLayout);

    LinearDemoWidget *demoWidget = new LinearDemoWidget();
    demoWidget->setHighDuty(params.highDuty);
    demoWidget->setLowDuty(params.lowDuty);
    demoWidget->setHighTemp(params.highTemp);
    demoWidget->setLowTemp(params.lowTemp);
    linearLayout->insertWidget(0, demoWidget);

    QWidget *linearWidgets = new QWidget();
    linearWidgets->setLayout(linearLayout);
    linearWidgets->setEnabled(fan->mode() == Fan::MODE_LINEAR);

    gridLayout->addWidget(linearWidgets, gridLayout->rowCount()-1, 1);

    connect(manual, &QRadioButton::toggled,
            fixedWidgets, &QWidget::setEnabled);
    connect(linear, &QRadioButton::toggled,
            linearWidgets, &QWidget::setEnabled);
    connect(fixedDutySlider, &FanDutySlider::dutyStrChanged,
            fixedDutyLabel, &QLabel::setText);
    connect(minDutySlider, &FanDutySlider::dutyStrChanged,
            minDutyLabel, &QLabel::setText);
    connect(minDutySlider, &QSlider::valueChanged,
            demoWidget, &LinearDemoWidget::setLowDuty);
    connect(maxDutySlider, &FanDutySlider::dutyStrChanged,
            maxDutyLabel, &QLabel::setText);
    connect(maxDutySlider, &QSlider::valueChanged,
            demoWidget, &LinearDemoWidget::setHighDuty);
    connect(minTemp, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
            demoWidget, &LinearDemoWidget::setLowTemp);
    connect(maxTemp, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
            demoWidget, &LinearDemoWidget::setHighTemp);
}

void FanDialog::apply()
{
    PortDialog::apply();

    Fan::Mode mode = static_cast<Fan::Mode>(modeGroup->checkedId());
    fan->setSensor(mappedSensor->currentIndex());
    fan->setMode(mode);
    if (mode == Fan::MODE_MANUAL) {
        fan->setFixedDuty(fixedDutySlider->value());
    } else if (mode == Fan::MODE_LINEAR) {
        Fan::Param linearParam;
        linearParam.lowDuty = minDutySlider->value();
        linearParam.highDuty = maxDutySlider->value();
        linearParam.lowTemp = minTemp->value();
        linearParam.highTemp = maxTemp->value();
        fan->setParam(linearParam);
    }
}
