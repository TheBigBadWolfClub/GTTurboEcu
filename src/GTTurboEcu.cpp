#include "GTTurboEcu.h"


GTTurboEcu::GTTurboEcu(uint32_t baudRate, uint8_t rxPin, uint8_t txPin) {
    _connection = new OBDSerialComm(baudRate, rxPin, txPin);
    _atProcessor = new ATCommands(_connection);
    _pidProcessor = new PidProcessor(_connection);

    _lastCommand = "";
}

GTTurboEcu::~GTTurboEcu() {
}


String GTTurboEcu::readPidRequest() {
    String rxData;
    do {
        rxData = _connection->readData();
        rxData.toUpperCase();
        // TODO ignore spaces, and all control chars (tab, etc)
        // TODO accept single carriage return as repeat last command at or pid
        // ignore null i.e 00
    } while (processResponse(rxData));
    return rxData;
}



bool GTTurboEcu::registerMode01Pid(uint32_t pid) {
    _pidProcessor->registerMode01Pid(pid);
}


void GTTurboEcu::writePidNotSupported() {
    _connection->writeEndNoData();
}


void GTTurboEcu::writePidResponse(String requestPid, uint8_t numberOfBytes, uint32_t value) {
    _pidProcessor->writePidResponse(requestPid, numberOfBytes, value);
}

bool GTTurboEcu::processResponse(String command) {

    //TODO: check for no 0X0D char in command, return ? and >
    //TODO: receive only <cr>, repeat last command

    //DEBUG("request: " + command);

    // carriage return, means repeat last command
    if (command == "\n") {
        command = _lastCommand;
    }

    // empty command, do not send to user
    if (command.length() == 0) {
        return true;
    }

    // TODO: check if this is repeated
    if(_atProcessor->process(command)) {
        _lastCommand = command;
        return true;
    }

    // check if is valid hex command
    if(!isValidHex(command.c_str())) {
        _connection->writeEndUnknown();
        DEBUG("Invalid HEX command: " + command);
        return true;
    }

    // TODO: check if this is repeated
    if(_pidProcessor->process(command)) {
        _lastCommand = command;
        return true;
    }

    return false;
}


bool GTTurboEcu::isValidHex(const char *pid) {
    return (pid[strspn(pid, "0123456789abcdefABCDEF")] == 0);
}


/*
int GTTurboEcu::freeRam() {
    extern int __heap_start, *__brkval;
    int v;
    return (int) &v - (__brkval == 0 ? (int) &__heap_start : (int) __brkval);
}
 */