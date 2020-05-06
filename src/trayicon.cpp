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

#include "trayicon.h"

#include <QAction>
#include <QIcon>


static const int TIMEOUT_SUCCESS = 1500;
static const int TIMEOUT_ERROR   = 3000;

TrayIcon::TrayIcon(QObject *parent) : QSystemTrayIcon(parent)
{
    setIcon(QIcon("://icons/logo"));
    setContextMenu(&menu);

    QAction *saveAction = menu.addAction("Save settings");
    connect(saveAction, &QAction::triggered, this, &TrayIcon::saveEeprom);

    QAction *loadAction = menu.addAction("Load settings");
    connect(loadAction, &QAction::triggered, this, &TrayIcon::loadEeprom);

    menu.addSeparator();

    QAction *aboutAction = menu.addAction("About");
    connect(aboutAction, &QAction::triggered, this, &TrayIcon::showAboutDialog);

    QAction *quitAction = menu.addAction("Quit");
    connect(quitAction, &QAction::triggered, this, &TrayIcon::quitApplication);

    connect(this, &QSystemTrayIcon::activated, this, &TrayIcon::clicked);
}

void TrayIcon::clicked(QSystemTrayIcon::ActivationReason reason)
{
    if (reason == QSystemTrayIcon::Trigger)
        emit toggleMainWindow();
}

void TrayIcon::settingsSaved()
{
    showMessage("Settings saved", "Current settings have been saved to EEPROM.",
                QSystemTrayIcon::Information, TIMEOUT_SUCCESS);
}

void TrayIcon::settingsLoaded()
{
    showMessage("Settings loaded", "Settings have been loaded from EEPROM.",
                QSystemTrayIcon::Information, TIMEOUT_SUCCESS);
}

void TrayIcon::showError(QString message)
{
    showMessage("Error", message, QSystemTrayIcon::Critical, TIMEOUT_ERROR);
}
