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

#include <QApplication>
#include <QDebug>

#include "fanman.h"

#ifndef GIT_VERSION
#define GIT_VERSION "unknown"
#endif


int main(int argc, char *argv[])
{
    QApplication::setApplicationName("FanMan");
    QApplication::setApplicationVersion(GIT_VERSION);
    QApplication::setOrganizationName("fanboy");
    QApplication::setOrganizationDomain("lynix.github.io");

    try {
        FanMan f(argc, argv);
        return f.exec();
    } catch (QString &e) {
        qCritical() << e;
        return 1;
    }
}
