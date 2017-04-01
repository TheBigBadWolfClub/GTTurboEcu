#ifndef GTTURBOECU_OBDSerialComm_h
#define GTTURBOECU_OBDSerialComm_h

#include <Arduino.h>
#include <SoftwareSerial.h>
#include "definitions.h"


class OBDSerialComm {
public:
    // ODB parameters
    enum STATUS {
        IDLE = 0, READY = 1
    };

    OBDSerialComm(uint32_t baudRate, uint8_t rxPin, uint8_t txPin);

    ~OBDSerialComm();

    void writeEndOK();

    void writeEndERROR();

	/**
	* example no response in time from command send from elm to ECU, respond no data to user
	*/
    void writeEndNoData();

    /**
     * incomplete command, input not understand and no action taken
     * respond to:  incomplete messages, incorrect AT or invalid HEX digit
     */
    void writeEndUnknown();


    void setToDefaults();

    String readData();

    void writeTo(uint8_t cChar);

    void writeTo(char const *string);

    void setEcho(bool echo);

    void writeEnd();

    bool isEchoEnable();

    void setLineFeeds(bool status);

    void setMemory(bool status);

    void setWhiteSpaces(bool status);

    void setHeaders(bool status);

    void setStatus(STATUS status);

    void writeEndPidTo(char const *string);

private:

    // Serial parameters
    SoftwareSerial *serial; // lib to communicate with bluetooth
    uint32_t boudRate; // Serial Boud Rate



    STATUS status; // Operation status
    bool echoEnable; // echoEnable command after received
    bool lineFeedEnable;
    bool memoryEnabled;
    bool whiteSpacesEnabled;
    bool headersEnabled;

    void setBaudRate(uint32_t rate);

    long getBaudRate();

    void addSpacesToResponse(const char *response, char string[]);
};


#endif