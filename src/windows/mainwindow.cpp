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

#include "mainwindow.h"

#include <QGridLayout>
#include <QPushButton>

#include "settings.h"
#include "windows/fandialog.h"
#include "windows/sensordialog.h"


MainWindow::MainWindow(const FanBoy& fanBoy, QWidget *parent) :
    QMainWindow(parent),
    fanBoy(fanBoy)
{
    setWindowIcon(QIcon("://icons/logo"));
    setMinimumSize(1050, 900);

    QGridLayout *mlayout = new QGridLayout();
    mlayout->setSpacing(20);
    mlayout->setMargin(20);

    for (quint8 i=0; i<FanBoy::numSensors(); i++) {
        SensorWidget *w = new SensorWidget(fanBoy.sensor(i), this);
        connect(w, &PortWidget::configClicked, this,
                &MainWindow::portConfigClicked);
        sensors.append(w);
        mlayout->addWidget(w, !(i%2) + mlayout->rowCount() - 1, i%2);
    }

    for (quint8 i=0; i<FanBoy::numFans(); i++) {
        FanWidget *w = new FanWidget(fanBoy.fan(i), this);
        connect(w, &PortWidget::configClicked, this,
                &MainWindow::portConfigClicked);
        fans.append(w);
        mlayout->addWidget(w, !(i%2) + mlayout->rowCount() - 1, i%2);
    }

    QWidget *widget = new QWidget(this);
    widget->setLayout(mlayout);
    setCentralWidget(widget);

    if (!Settings::instance().windowGeometry().isNull())
        restoreGeometry(Settings::instance().windowGeometry());
    if (Settings::instance().windowVisible())
        show();
}

void MainWindow::portConfigClicked()
{
    PortWidget *widget = static_cast<PortWidget *>(sender());
    PortDialog *dialog;
    int index = fans.indexOf(static_cast<FanWidget *>(widget));
    if (index >= 0) {
        dialog = new FanDialog(fanBoy.fan(index), this);
    } else {
        index = sensors.indexOf(static_cast<SensorWidget *>(widget));
        dialog = new SensorDialog(fanBoy.sensor(index), this);
    }

    connect(dialog, &PortDialog::accepted, widget, &PortWidget::enableConfig);
    connect(dialog, &PortDialog::rejected, widget, &PortWidget::enableConfig);

    dialog->show();
}

void MainWindow::setVisible(bool visible)
{
    Settings::instance().setWindowVisible(visible);
    if (!visible)
        Settings::instance().setWindowGeometry(saveGeometry());

    QWidget::setVisible(visible);
}
