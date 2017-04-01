#ifndef GTTURBOECU_GTTurboEcu_h
#define GTTURBOECU_GTTurboEcu_h

#include <Arduino.h>

#include "elm327/OBDSerialComm.h"
#include "elm327/ATCommands.h"
#include "elm327/PidProcessor.h"

class GTTurboEcu {

public:

    /**
     * When creating a new instance of GTTurboEcu,
     * it needs to set the Bluetooth serial parameter's
     *
     * @param baudRate - rate for bluetooth serial port (normally 9600)
     * @param rxPin - the Arduino RX pin for the Bluetooth module (serial port)
     * @param txPin - the Arduino TX pin for the Bluetooth module (serial port)
     * @return
     */
    GTTurboEcu(uint32_t baudRate, uint8_t rxPin, uint8_t txPin);

    ~GTTurboEcu();

    /**
     * Waits and Reads a request from OBD client software (example: torque)
     *
     * It will only return if the OBD client software
     * request's one of the previous registred PID (see: registerMode01Pid())
     *
     * Any other request will be automaticly handle or it will be rejected.
     *
     * GTTurboEcu lib will automaticly handle:
     * - any AT command
     * - PID's support request like 0100, 0120, 0140, 0160, ...
     *
     * @return the request PID (Vehicle speed ex:  010D )
     */
    String readPidRequest();

    /**
     * Registry the PID's (sensors) your arduino will support.
     * Currently GTTurboEcu lib only supports MODE 01 PID's
     *
     * In OBDII MODE 01 PID's, are request
     * to Show current data for a given sensor identified by a PID
     *
     * List of MODE 01 PID's: https://en.wikipedia.org/wiki/OBD-II_PIDs#Mode_01
     *
     * Example, for Vehicle speed, where OBDII PID code is 0D:
     *
     * registerMode01Pid(0x0D)
     *
     * @param pidHexId
     * @return returns true if the entered ID is in the valide range [0x00, 0xFF]
     */
    bool registerMode01Pid(uint32_t pidHexId);

    /**
     * Used to informe the OBD client that the PID,
     * it has requested is not supported
     *
     * In normal conditions, you will only receive,
     * request for the PID you have registred
     * All other request is automaticly handle by GTTuboEcu lib
     */
    void writePidNotSupported();

    /**
     * Responde to a given PID request from OBDII client (ex: torque)
     *
     * @param requestPid - the received pid
     * @param numberOfBytes - number of bytes this PID response VALUE is supposed to have
     * @param value - sensor value for this PID,
     *                  lib will convert it to HEX
     *                  with the appropriated number of HEX chars (numberOfChars)
     */
    void writePidResponse(String requestPid, uint8_t numberOfBytes, uint32_t value);

private:

    OBDSerialComm *_connection;

    ATCommands *_atProcessor;

    PidProcessor *_pidProcessor;

    String _lastCommand;

    bool processResponse(String command);

    bool isValidHex(const char *pid);
};

#endif
