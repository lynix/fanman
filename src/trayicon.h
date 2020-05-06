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

#include <QMenu>
#include <QSystemTrayIcon>

#include "settings.h"


class TrayIcon : public QSystemTrayIcon
{
        Q_OBJECT

        QMenu    menu;

    public:

        explicit TrayIcon(QObject *parent = nullptr);

    private slots:

        void clicked(QSystemTrayIcon::ActivationReason reason);

    public slots:

        void settingsSaved();
        void settingsLoaded();
        void showError(QString message);

    signals:

        void quitApplication();
        void toggleMainWindow();
        void showAboutDialog();
        void saveEeprom();
        void loadEeprom();
};
