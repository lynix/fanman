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

#include <QObject>


class FanBoy;

class Port : public QObject
{
        Q_OBJECT

        friend class FanBoy;

    public:

        enum Type { FAN, SENSOR };

    private:

        void unplug();

    protected:

        FanBoy   *device;
        quint8   mIndex;
        QString  mName;
        bool     mPlugged;

        Port(FanBoy *device, Type type, quint8 index);

    public:

        virtual Type type() const = 0;
        quint8 index() const { return mIndex; };
        const QString &name() const { return mName; };
        bool plugged() const { return mPlugged; };

    public slots:

        void setName(const QString &name);

    signals:

        void renamed(const QString &name);
        void unplugged();
};
