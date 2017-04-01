#include "ATCommands.h"


ATCommands::ATCommands(OBDSerialComm *connection) {
    this->connection = connection;
}

ATCommands::~ATCommands() {
    operator delete(this->connection);
}


bool ATCommands::process(String command) {
    bool processed = false;
    if (isATCommand(command)) {
        processed = true;
        processCommand(command);
        DEBUG("RX AT: " + command);
    }
    return processed;
}

void ATCommands::processCommand(String command) {

    // if space is enabled (ex: AT H[charPosition])
    uint8_t offset = 2;
    if (command.charAt(2) == 0x20) {
        offset = 3;
    }

    // refer to ELM327 specs
    String specificCommand = command.substring(offset, command.length());
    if (specificCommand.startsWith("D",offset)) {
        ATCommands::ATD();
    } else if (specificCommand.startsWith("Z")) {
        ATCommands::ATZ();
    } else if (specificCommand.startsWith("I")) {
        ATCommands::ATI();
    } else if (specificCommand.startsWith("E")) {
        ATCommands::ATEx(specificCommand);
    } else if (specificCommand.startsWith("L")) {
        ATCommands::ATLx(specificCommand);
    } else if (specificCommand.startsWith("M")) {
        ATCommands::ATMx(specificCommand);
    } else if (specificCommand.startsWith("S")) {
        if (specificCommand.startsWith("SP")) {
            ATCommands::ATSPx(specificCommand);
        } else {
            ATCommands::ATSx(specificCommand);
        }
    } else if (specificCommand.startsWith("H")) {
        ATCommands::ATHx(specificCommand);
    } else if (specificCommand.startsWith("AT")) {
        ATCommands::ATATx(specificCommand);
    } else if (specificCommand.startsWith("DPN")) {
        ATCommands::ATDPN();
    }  else if (specificCommand.startsWith("DESC") || specificCommand.startsWith("@1")) {
        ATCommands::ATDESC();
    } else if (specificCommand.startsWith("PC")) {
        ATCommands::ATPC();
    } else {

        // lets assume we process any at command
        connection->writeEndOK();
    }
}

// set all to defaults
void ATCommands::ATD() {
    connection->setToDefaults();
    connection->writeTo("BUS INIT: ...");
    connection->writeEndOK();

}


// reset all
void ATCommands::ATZ() {
    connection->setEcho(true);
    connection->setStatus(connection->IDLE);
    connection->writeTo(ID);
    connection->writeEndOK();
}

// Print the version ID
void ATCommands::ATI() {
    connection->setStatus(connection->READY);
    connection->writeTo(ID);
    connection->writeEndOK();
}

// send description
void ATCommands::ATDESC() {
    connection->writeTo(DESC);
    connection->writeEndOK();
}

// set echoEnable 0=off 1=on
void ATCommands::ATEx(String cmd) {
    connection->setEcho(cmd.equals("E0") ? false : true);
    connection->writeEndOK();
}

// set memory off=0 on=1
void ATCommands::ATMx(String cmd) {
    connection->setMemory(cmd.equals("M0") ? false : true);
    connection->writeEndOK();
}

// line feeds off=0 on=1
void ATCommands::ATLx(String cmd) {
    connection->setLineFeeds(cmd.equals("L0") ? false : true);
    connection->writeEndOK();
}

// ATSx printing spaces off=0 on=1
void ATCommands::ATSx(String cmd) {
    connection->setWhiteSpaces(cmd.equals("S0") ? false : true);
    connection->writeEndOK();
}

// Headers off=0 on=1
void ATCommands::ATHx(String cmd) {
    connection->setHeaders(cmd.equals("H0") ? false : true);
    connection->writeEndOK();
}

// ATSPx Define protocol 0=auto
void ATCommands::ATSPx(String cmd) {
    connection->writeEndOK();
}

// set protocol
void ATCommands::ATDPN() {
    connection->writeTo(PROTOCOL);
    connection->writeEndOK();
}

// AT AT2 adaptative time control
void ATCommands::ATATx(String cmd) {
    connection->writeEndOK();
}

// Terminates current diagnostic session. Protocol close
void ATCommands::ATPC() {
    connection->writeEndOK();
}

// return true ir connectionand is AT
bool ATCommands::isATCommand(String command) {
    toUpperCase(command.charAt(0));
    return command.startsWith("AT") ? true : false;
}

