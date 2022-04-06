/**
 * \copyright    Copyright 2021 - 2022 Fraunhofer Institute for Integrated Circuits IIS, Erlangen Germany
 *
 * SPDX-License-Identifier: MIT
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in the
 * Software without restriction, including without limitation the rights to use, copy,
 * modify, merge, publish, distribute, sublicense, and/or sell copies of the Software,
 * and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A
 * PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 * HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
 * OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
 * SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

/**
 * \file
 * \author       Fraunhofer Institute for Integrated Circuits IIS, Erlangen Germany
 * \author       gnt
 * \date         May 19, 2021
 * \brief        Utility functions for M3B
 *               board initalisation and communication with mioty modul
 */

#include "Arduino.h"
#include "SoftwareSerial.h"
#include "miotyAtClient.h"

#ifndef M3B_DEMO_HELPER_h
#define M3B_DEMO_HELPER_h


#define STATUS_LED PC13
#define BLUE_LED   PC6
#define GREEN_LED  PC7
#define RED_LED    PC8


extern SoftwareSerial SerialM3B;
extern SoftwareSerial SerialMioty;


class M3BDemoHelper
{
public:
	/**
	 * @brief constructor
	 */
    M3BDemoHelper();
	
	/**
	 * @brief Initialize onboard LEDs
	 */
    void begin();

	/**
	 * @brief Send acceleration sensor data with blueprint id=1 and catch status-response
	 *
	 * @param[in]   temp		temperatur value
	 * @param[in]   acc_x       acceleration in x-axes
	 * @param[in]   acc_y       acceleration in y-axes
	 * @param[in]   acc_z       acceleration in z-axes
	 *
	 * @return      miotyAtClient_returnCode    indicating success/error of AT_cmd execution
	 */
    miotyAtClient_returnCode transmitAcceleration(float temp, int16_t acc_x, int16_t acc_y, int16_t acc_z);

	/**
	 * @brief Send weather sensor data with blueprint id=0 and catch status-response
	 *
	 * @param[in]   pres		pressure value
	 * @param[in]   temp        temperatur value
	 * @param[in]   hum         humidity vaule
	 * @param[in]   lum         luminosity value
	 *
	 * @return      miotyAtClient_returnCode    indicating success/error of AT_cmd execution
	 */
    miotyAtClient_returnCode transmitWeather(float pres, float temp, float hum, uint16_t lum);
    
private:


};

/**
 * @brief Callback function from mioty_at_client_c
 * Write data to SerialMioty interface and for debug to SerialM3B
 *
 * @param[in]   msg         Pointer to a buffer containing message
 * @param[in]   len         length of message
 */
// void miotyAtClientWrite(uint8_t * msg, uint16_t len);

/**
 * @brief Send AT command to set the network key (AT-MNWK)
 *
 * @param[out]  buf         Pointer to a buffer for reception message
 * @param[out]  p_len       length of message
 *
 * @return      bool    	indicating data read
 */
// bool miotyAtClientRead(uint8_t * buf, uint8_t * p_len){


#endif
