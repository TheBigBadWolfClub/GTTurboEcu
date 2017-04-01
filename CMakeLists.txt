cmake_minimum_required(VERSION 2.8.4)
project(GTTurboEcu)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES
        src/elm327/OBDSerialComm.h
        src/elm327/OBDSerialComm.cpp
        src/elm327/ATCommands.h
        src/elm327/ATCommands.cpp
        src/GTTurboEcu.h
        src/GTTurboEcu.cpp
        src/elm327/PidProcessor.cpp
        src/elm327/PidProcessor.h
        src/elm327/definitions.h
        )

add_executable(GTTurboEcu ${SOURCE_FILES})

include_directories(/usr/share/arduino/hardware/archlinux-arduino/avr/cores/arduino)
include_directories(/usr/share/arduino/hardware/tools/avr/lib/avr/include)
include_directories(/usr/share/arduino/hardware/archlinux-arduino/avr/libraries/SoftwareSerial)