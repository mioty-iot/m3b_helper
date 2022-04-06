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

#include <Arduino.h>
#include "m3bDemoHelper.h"

M3BDemoHelper::M3BDemoHelper() {
}


void M3BDemoHelper::begin(void) {
    pinMode(STATUS_LED, OUTPUT);
    pinMode(BLUE_LED, OUTPUT);
    pinMode(RED_LED, OUTPUT);
    pinMode(GREEN_LED, OUTPUT);
    digitalWrite(STATUS_LED, LOW);
    digitalWrite(RED_LED, HIGH);
    digitalWrite(GREEN_LED, HIGH);
    digitalWrite(BLUE_LED, HIGH);
}


miotyAtClient_returnCode M3BDemoHelper::transmitAcceleration(float temp, int16_t acc_x, int16_t acc_y, int16_t acc_z) {
    uint8_t msg[9] = {1, 0};
    uint16_t temp_int = temp * 10 + 2731;
    msg[1] = (temp_int)>>8;
    msg[2] = temp_int;
    msg[3] = acc_x>>8;
    msg[4] = acc_x;
    msg[5] = acc_y>>8;
    msg[6] = acc_y;
    msg[7] = acc_z>>8;
    msg[8] = acc_z;
    uint32_t cnt = 0;
    
    digitalWrite(STATUS_LED, LOW);
    miotyAtClient_returnCode ret = miotyAtClient_sendMessageUniMPF(msg, 9, &cnt);
    digitalWrite(STATUS_LED, HIGH);
    return ret;
}


miotyAtClient_returnCode M3BDemoHelper::transmitWeather(float pres, float temp, float hum, uint16_t lum) {
    uint8_t msg[8] = {0};
    uint16_t temp_int = temp * 10 + 2731;
    uint16_t pres_int = pres * 10;
    msg[1] = (temp_int)>>8;
    msg[2] = temp_int;
    msg[3] = (uint8_t)hum;
    msg[4] = pres_int>>8;
    msg[5] = pres_int;
    msg[6] = lum>>8;
    msg[7] = lum;
    uint32_t cnt = 0;
    
    digitalWrite(STATUS_LED, LOW);
    miotyAtClient_returnCode ret = miotyAtClient_sendMessageUniMPF(msg, 8, &cnt);
    digitalWrite(STATUS_LED, HIGH);
    return ret;
}


void miotyAtClientWrite(uint8_t * msg, uint16_t len){
  SerialMioty.write(msg, len);
  SerialM3B.println(" ");
  SerialM3B.write(msg, len);
  SerialM3B.println(" ");
}


bool miotyAtClientRead(uint8_t * buf, uint8_t * p_len){
  int i=0;
  while(SerialMioty.available() > 0){
    buf[i] = SerialMioty.read();
    i++;
  }
  // write for debugging purposes
  SerialM3B.write(buf, i);
  *p_len = i;
  return true;
}
