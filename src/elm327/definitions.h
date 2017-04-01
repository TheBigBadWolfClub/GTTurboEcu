//
// Created by dragon on 12-05-2016.
//

#ifndef GTTURBOECU_DEFINITIONS_H
#define GTTURBOECU_DEFINITIONS_H

#include <stdint.h>


extern HardwareSerial Serial;


#define DO_DEBUG true
#define DEBUG(x) do {if (DO_DEBUG) { Serial.println(x); } } while (0)


#define xtoc(x) ((x < 10) ? ('0' + x) : ('A' - 10 + x))
#define getNumOfHexChars(nBytes) (nBytes * 2)

#define PID_N_BYTES 2
#define N_CHARS_IN_BYTE 2


// Device ID
#define ID  "ELM327 - GTTurboECU V0.0.1"
#define DESC  "GTTurbo OBD2 Arduino libs, based on ELM"
#define PROTOCOL "6" // canbus 500k 11 bit protocol id for elm.

// Char representing end of serial string
#define SERIAL_END_CHAR  0x0D
#define SERIAL_READ_TIMEOUT 20000L


const uint8_t maxPid = 0xFF;

const uint8_t N_MODE01_INTERVALS = 7;
const uint8_t PID_INTERVAL_OFFSET = 0x20;



#endif //GTTURBOECU_DEFINITIONS_H
