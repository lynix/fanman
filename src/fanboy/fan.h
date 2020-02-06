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

#include "fanboy/port.h"


class Fan : public Port
{
        Q_OBJECT

        friend class FanBoy;

    public:

        enum Mode
        {
            MODE_MANUAL = 0,
            MODE_LINEAR = 1
        };

        struct State
        {
            quint8   duty;
            quint16  rpm;
        };

        struct Param
        {
            quint8  lowDuty;
            qreal   lowTemp;
            quint8  highDuty;
            qreal   highTemp;

            bool operator!=(const Param& ref) const
            {
                return lowDuty != ref.lowDuty ||
                       lowTemp != ref.lowTemp ||
                       highDuty != ref.highDuty ||
                       highTemp != ref.highTemp;
            }
        };

    private:

        Mode     mMode;
        quint8   mSensor;
        State    mState;
        Param    mParam;

        void injectState(State state);
        void injectMode(Mode mode);
        void injectMapping(quint8 sensor);
        void injectParam(const Param &param);

    public:

        Fan(FanBoy *device, quint8 index);

        virtual Type type() const override { return FAN; }
        Mode mode() const { return mMode; };
        quint8 sensor() const { return mSensor; };
        State state() const { return mState; }
        Param param() const { return mParam; }

    public slots:

        void setMode(Mode mode);
        void setSensor(quint8 index);
        void setParam(const Param &param);
        void setFixedDuty(quint8 duty);

    signals:

        void modeChanged(Mode mode);
        void sensorChanged(quint8 index);
        void stateUpdated(State state);
        void paramChanged(const Param &param);
        void fixedDutyChanged(quint8 duty);
};
