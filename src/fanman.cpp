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

#include "fanman.h"

#include <QMessageBox>


static const char *ABOUT_TITLE  = "About %1";
static const char *ABOUT_TEXT   = "<p><strong>%1</strong></p>" \
                                  "<p>Version: %2<br />" \
                                  "Built: %3 %4</p>";

FanMan::FanMan(int argc, char *argv[]) : QApplication(argc, argv),
    mainWindow(fanBoy)
{
    setQuitOnLastWindowClosed(false);

    connect(&trayIcon, &TrayIcon::toggleMainWindow,
            this, &FanMan::toggleMainWindow);
    connect(&trayIcon, &TrayIcon::quitApplication,
            this, &QApplication::quit);
    connect(&trayIcon, &TrayIcon::showAboutDialog,
            this, &FanMan::showAboutDialog);
    connect(&trayIcon, &TrayIcon::saveEeprom,
            &fanBoy, &FanBoy::saveSettings);
    connect(&trayIcon, &TrayIcon::loadEeprom,
            &fanBoy, &FanBoy::loadSettings);
    connect(&fanBoy, &FanBoy::settingsSaved,
            &trayIcon, &TrayIcon::settingsSaved);
    connect(&fanBoy, &FanBoy::settingsLoaded,
            &trayIcon, &TrayIcon::settingsLoaded);

    trayIcon.show();
}

void FanMan::toggleMainWindow()
{
    mainWindow.setVisible(!mainWindow.isVisible());
}

void FanMan::showAboutDialog()
{
    QString title = QString(ABOUT_TITLE).arg(QApplication::applicationName());
    QString text = QString(ABOUT_TEXT).arg(QApplication::applicationName());
    text = text.arg(QApplication::applicationVersion());
    text = text.arg(__DATE__).arg(__TIME__);

    QMessageBox::about(&mainWindow, title, text);
}
