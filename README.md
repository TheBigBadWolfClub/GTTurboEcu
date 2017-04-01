# GTTurboEcu
Arduino ODBII & ELM327 Emulator - <br> 
Allows Arduino to act as a ECU and a OBD2 ELM327 device and response to PID requests, from any given ODBII app (ex: android::torque).

This library can be used to read values from real sensor's with arduino and visualized them in any OBDII client App, 
or it can be used as an emulator to help developing OBDII client App's over ELM327 chip's.

GTTurboEcu implements the specification's of the ELM327 chip.</br>

It was only tested with Torque, but in theory it should work OK,  with any OBDII client App that can communicate with a ELM327 chip.

ELM327 specifications: </br>
https://www.elmelectronics.com/wp-content/uploads/2016/07/ELM327DS.pdf

Before start using this library, it's necessary to understand what a PID is and how it is define (mode, pid, number of bytes, conversion formulas ...), read the Wikipage </br> 
https://en.wikipedia.org/wiki/OBD-II_PIDs

# Example: Creating a ino File


``` C++

/**
* Start by including GTTurboEcu Library
*/
#include <GTTurboEcu.h>

/**
 * Define and configure GTTurboEcu Library
 *
 * This values are related to the bluetooth configuration.
 * library SoftwareSerial.h is use to connect the bluetooth device
 * by identifying:
 * 1) baudRate
 * 2) rxPin
 * 3) txPin
 */
GTTurboEcu gtTurboEcu(9600L, 11, 10);


void setup() {

    // init serial communication, 
    Serial.begin(9600);


    /**
     * https://en.wikipedia.org/wiki/OBD-II_PIDs
     *
     * When the OBD client (ex: torque)
     * init's connection, it will ask what sensors the "ecu" supports
     *
     * Register the PID(sensors) that will be reported to ODBII client app,
     * only insert the PID in Hex, do not send the mode01 (see the wikipedia page for more info)
     *
     * ex PID(010C - Engine RPM) PID(0170 - Boost pressure control)
     */
    gtTurboEcu.registerMode01Pid(0x0C);
    gtTurboEcu.registerMode01Pid(0x70);

}

void loop() {

     /**
     * The OBD client App (ex: torque),
     * will frequently request values for the PID(sensors),
     * that where registered in setup()
     * 
     * use readPidRequest to listen for OBDII client app, pid request's
     */
     String pidRequest = gtTurboEcu.readPidRequest();
     
     /**
     * Process the pid request 010C - Engine RPM
     */
      if (pidRequest.equalsIgnoreCase("010C")) {
      
            // read RPM value from RPM sensor
            uint32_t rpmSensorValue = <readYourRpmSensorValue()>;
            
            // CONVERT FORMULAS ON PID (more on wikipage)
            // find the formula that is used to convert the RPM value on the client side
            // revert the formula and apply to the value that was readed from the RPM sensor
            // EX on the client side the RPM value is divide by 4, 
            // this means that the client is expecting an RPM = (rpmSensorValue * 4)
            
            uint32_t rpmSensorValueConverted = rpmSensorValue * 4;
            
            // NUMBER OF EXPECTED BYTES
            // The OBDII client is expecting a response with n number of bytes for a given PID (check the wikipage)
            // EX the RPM PID (010C), is expecting 2 bytes in the response
            
            uint8_t numberOfBytes = 2;
            
            // finally send the response back
            
            /**
             * pidRequest - we re-send the pid we received to identify it
             * numberOfBytes - the number of bytes this PID value has, see OBDII PID specifications
             * rpmSensorValueConverted - the value of the sensor
             */
            gtTurboEcu.writePidResponse(pidRequest, numberOfBytes, rpmSensorValueConverted);    
            
            
      } else if (pidRequest.equalsIgnoreCase("0170")) {
              // read  value from Turbo Boost sensor
              uint32_t turboBoostSensorValue = <readYourTurboBoostSensorValue()>;
              
              // CONVERT FORMULAS ON PID
              // no need to convert
              
              
              // NUMBER OF EXPECTED BYTES
              uint8_t numberOfBytes = 9;
              
              // finally send the response back
              gtTurboEcu.writePidResponse(pidRequest, numberOfBytes, turboBoostSensorValue);    
              
       } else {
      
           /**
            * If pid not implemented, report it as not implemented
            */
           gtTurboEcu.writePidNotSupported();
       }
       
       
       
     

} // loop end


```

# Example ino file with dummy values for sensors 
For a quick test, there's a ino file in examples/DataSimulator directory. This is a running example with some PID's and some fake data values.

Youtube video:
https://youtu.be/ACYquj39CmI

    # Requisites:
    - Arduino Uno
    - Bluetooth Shield
    - GTTurboEcu library
    
# Motivation
Has a fan of classic car's and owner of some, I need to read some performance values without changing the interior dash by setting gauges or arduino displays.</br>
     Since I was unable to find an ELM327 arduino implementation, decided to make my own and share it with all classic cars enthusiast.
     

# License


This library is free software; you can redistribute it and/or modify it under the terms of the GNU Lesser General Public License as published by the Free Software Foundation; either version 2.1 of the License, or (at your option) any later version.

This library is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License along with this library; if not, write to the Free Software Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301 USA

Copyright (c) 2017 . All right reserved.
