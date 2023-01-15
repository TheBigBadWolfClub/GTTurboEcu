//
// Created by dragon on 12-05-2016.
//


#include "PidProcessor.h"

PidProcessor::PidProcessor(OBDSerialComm *connection) {
    _connection = connection;
    resetPidMode01Array();
};

bool PidProcessor::process(String command) {
    bool processed = false;

    if (!isMode01(command))
        return false;

    uint16_t hexCommand = strtoul(command.c_str(), NULL, HEX);
    uint8_t pid = getPidCodeOnly(hexCommand);
    if (isSupportedPidRequest(pid)) {
        processed = true;
        uint32_t supportedPids = getSupportedPids(pid);
        writePidResponse(command, 4, supportedPids);
    }
    return processed;
}

void PidProcessor::writePidResponse(String requestPid, uint8_t numberOfBytes, uint32_t value) {
    uint8_t  nHexChars = PID_N_BYTES * N_CHARS_IN_BYTE +  numberOfBytes * N_CHARS_IN_BYTE ;
    char responseArray[nHexChars + 1]; // one more for termination char
    getFormattedResponse(responseArray, nHexChars, requestPid, value);
    _connection->writeEndPidTo(responseArray);
}

/**
 * adds a supported pid, so it can answer to pid support request, ex 0100, 0120, ...
 */
bool PidProcessor::registerMode01Pid(uint32_t pid) {
    // pid must be off type 01
    if (pid > 0x00 && pid < 0x0200) {
        // remove PidMode, only use pid code
        pid = getPidCodeOnly(pid);
        setPidBit(pid);

        char buffer[4];
        sprintf(buffer, "%02X", pid);
        DEBUG("Registered PID: " + String(buffer ));
        return true;
    }
    return false;
}

bool PidProcessor::isMode01(String command) {
    return command.startsWith("01") ? true : false;
}

/**
 *  return true if:
 *  - 1000
 *  - 1020
 *  - 1040
 *  - 1060
 *  ....
 */
bool PidProcessor::isSupportedPidRequest(uint8_t pid) {
    return pid == getPidIntervalId(pid);
}

/**
 * returns the interval RANGEID for the given pid (ex: 00, 20, 40, ...)
 */
uint8_t PidProcessor::getPidIntervalId(uint8_t pidcode) {
    return getPidIntervalIndex(pidcode) * PID_INTERVAL_OFFSET;
}

/**
 * returns the interval INDEX for the given pid (0,1,2,3,4,...)
 */
uint8_t PidProcessor::getPidIntervalIndex(uint8_t pidcode) {
    return pidcode / PID_INTERVAL_OFFSET;
}

/**
 * Receives a mode pid ex 010C ,
 * returns only the pid ex 0C
 */
uint8_t PidProcessor::getPidCodeOnly(uint16_t hexCommand) {
    if (hexCommand >= 0x0100) {
        hexCommand -= 0x100;
    }
    return hexCommand;
}

void PidProcessor::setPidBit(uint8_t pid) {
    uint8_t arrayIndex = getPidIntervalIndex(pid);
    if(isSupportedPidRequest(pid)) {
        arrayIndex -= 1;
    }

    uint8_t bitPosition = getPidBitPosition(pid);
    bitWrite(pidMode01Supported[arrayIndex], bitPosition, 1);

    // mark available intervals
    for (int i = 0; i < arrayIndex; i++) {
        bitWrite(pidMode01Supported[i], 0, 1);
    }
}

uint8_t PidProcessor::getPidBitPosition(uint8_t pid) {
    uint8_t intervalId = getPidIntervalId(pid) ;
    if(isSupportedPidRequest(pid)) {
        intervalId -= PID_INTERVAL_OFFSET;
    }

    uint16_t pos = pid - intervalId;
    pos = PID_INTERVAL_OFFSET - pos;
    return pos;
}


uint32_t PidProcessor:: getSupportedPids(uint8_t pid) {
    uint8_t index = getPidIntervalIndex(pid);
    return pidMode01Supported[index];
}

void PidProcessor::getFormattedResponse(char *response, uint8_t totalNumberOfChars, String pid, uint32_t value) {
    uint8_t nValueChars = totalNumberOfChars - PID_N_BYTES * N_CHARS_IN_BYTE;
    char cValue[2 + 1];
    itoa(nValueChars, cValue, DEC);
    String mask = "%s%0";
    mask.concat(cValue);
    mask.concat("lX\0");
    String pidResponse = convertToPidResponse(pid);
    sprintf(response, mask.c_str(), pidResponse.c_str(), value);

    DEBUG("RX PID: " + pid + " - TX: " + String(response));
}

String PidProcessor::convertToPidResponse(String pid) {
    String x = "4";
    x.concat(pid.substring(1, pid.length()));
    return x;
}

void PidProcessor::resetPidMode01Array() {
    for(uint8_t i = 0; i < N_MODE01_INTERVALS; i++ ) {
        pidMode01Supported[i] = 0x0;
    }
}






