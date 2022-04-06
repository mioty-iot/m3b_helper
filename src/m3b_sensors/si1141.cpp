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
 * \date         Dez 15, 2021
 * \brief        library for SI1141 ambient light sensor
 */

#include <Arduino.h>
#include "SI1141.h"


#define SI1141_I2C_ADRESS 0x5A

SI1141::SI1141() {
}


void SI1141::begin(TwoWire *twoWire) {
  _wire = twoWire;
  _wire->begin();
  // use forced measurement mode
  // Set MEAS_RATE 0
  _wire->beginTransmission(SI1141_I2C_ADRESS);
  _wire->write(0x08);
  _wire->write(0x00);
  _wire->endTransmission();
  // Set CHLIST params (enable EN_ALS_VIS)
  _wire->beginTransmission(SI1141_I2C_ADRESS);
  _wire->write(0x18); // write COMMAND register
  _wire->write(0x00); // Clear RESPONSE register
  _wire->endTransmission();
  _wire->beginTransmission(SI1141_I2C_ADRESS);
  // write value to PARAM_WR
  _wire->write(0x17);
  _wire->write(0x10);
  _wire->endTransmission();
  _wire->beginTransmission(SI1141_I2C_ADRESS);
  _wire->write(0x18); // write COMMAND register
  _wire->write(0xa1); // Param Set 0xa
  _wire->endTransmission();
}



bool SI1141::readLuminosity(uint16_t *lum) {
  // Clear RESPONSE register 
  _wire->beginTransmission(SI1141_I2C_ADRESS);
  _wire->write(0x18); // write COMMAND register
  _wire->write(0x00); // Clear RESPONSE register
  _wire->endTransmission();
  // Force ALS read
  _wire->beginTransmission(SI1141_I2C_ADRESS);
  _wire->write(0x18);
  _wire->write(0x06);
  _wire->endTransmission();
  byte response = I2CreadRegister(0x20);
  if (response&0x80)
      return false;
  delay(50);
  *lum = I2CreadRegister(0x22);
  *lum += (I2CreadRegister(0x23)<<8);
  return true;
}


byte SI1141::I2CreadRegister(byte reg) {
  _wire->beginTransmission(SI1141_I2C_ADRESS);
  _wire->write(reg);
  _wire->endTransmission();
  _wire->requestFrom(SI1141_I2C_ADRESS, 1);
  return _wire->read();
}
