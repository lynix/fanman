# FanMan: A GUI for FanBoy
#
# Copyright (C) 2020  Alexander Koch
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <https://www.gnu.org/licenses/>.

cmake_minimum_required(VERSION 3.5)

project(FanMan LANGUAGES CXX)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

find_package(Qt5 COMPONENTS Widgets Charts SerialPort REQUIRED)

set(SOURCES
    src/fanboy/fan.cpp
    src/fanboy/fan.h
    src/fanboy/fanboy.cpp
    src/fanboy/fanboy.h
    src/fanboy/port.cpp
    src/fanboy/port.h
    src/fanboy/sensor.cpp
    src/fanboy/sensor.h
    src/fanman.cpp
    src/fanman.h
    src/main.cpp
    src/settings.cpp
    src/settings.h
    src/trayicon.cpp
    src/trayicon.h
    src/widgets/fandutyslider.cpp
    src/widgets/fandutyslider.h
    src/widgets/fanwidget.cpp
    src/widgets/fanwidget.h
    src/widgets/lineardemowidget.cpp
    src/widgets/lineardemowidget.h
    src/widgets/portwidget.cpp
    src/widgets/portwidget.h
    src/widgets/rangecontrol.cpp
    src/widgets/rangecontrol.h
    src/widgets/scrollingtimechart.cpp
    src/widgets/scrollingtimechart.h
    src/widgets/sensorwidget.cpp
    src/widgets/sensorwidget.h
    src/widgets/valuegroup.cpp
    src/widgets/valuegroup.h
    src/windows/fandialog.cpp
    src/windows/fandialog.h
    src/windows/mainwindow.cpp
    src/windows/mainwindow.h
    src/windows/portdialog.cpp
    src/windows/portdialog.h
    src/windows/sensordialog.cpp
    src/windows/sensordialog.h
)

list(APPEND CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake")
include(GitVersion)

string(TOLOWER ${PROJECT_NAME} BIN)
add_executable(${BIN} ${SOURCES} res/icons.qrc)

target_compile_options(${BIN} PRIVATE -Wall)
target_compile_options(${BIN} PRIVATE $<$<CONFIG:Debug>: -g>)
target_include_directories(${BIN} PRIVATE "${CMAKE_SOURCE_DIR}/src")

target_link_libraries(${BIN} PRIVATE
    Qt5::Widgets
    Qt5::Charts
    Qt5::SerialPort
)
