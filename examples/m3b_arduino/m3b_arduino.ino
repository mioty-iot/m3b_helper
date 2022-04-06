/**
 * \copyright    Copyright 2022 Fraunhofer Institute for Integrated Circuits IIS, Erlangen Germany
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
 * \brief        Example Code for M3Bv2 Board
 *               Initialise board and sensors (light, tph, accelerometer)
 *               sendWeatherData (Blue LED active) and sendAccelerationData (red LED active) every minute
 */

/**
 *  Hardware components on M3Bv2: 
 *    SerialM3B   - PinHeader                     - PA9, PA10, 9600baud 8N1
 *    SerialMioty - mioty module                  - PC10, PC11, 9600baud 8N1
 *    adxl362     - 3 axis accelerometer          - SPI, (CS-PA8, MISO-PA6, MOSI-PA7, SCLK-PA5, INT1-PA11, INT2-PA12)
 *    ms5637      - pressure sensor               - I2C Wire2(PB9, PB8)
 *    sht31       - temperature & humidity sensor - I2C Wire2(PB9, PB8)
 *    si1141      - light sensor                  - I2C Wire2(PB9, PB8)
 *    RGB LED     - low active                    - BLUE-PC6, LED-PC7, RED-PC8
 *    Status LED  - high active                   - PC13
 */
 
#include <SHT31.h>
#include <SparkFun_MS5637_Arduino_Library.h>
#include <miotyAtClient.h>
#include "m3bDemoHelper.h"
#include "m3b_sensors/si1141.h"

#include <ADXL362.h>
#include <SPI.h>

#include <Wire.h>
#include "SoftwareSerial.h"


// Debug Serial
SoftwareSerial SerialM3B(PA10, PA9);
// Mioty Bidi Stamp Serial
SoftwareSerial SerialMioty(PC11, PC10);

TwoWire Wire2(PB9, PB8);

ADXL362        adxl362;
MS5637         ms5637;
SHT31          sht31;
SI1141         si1141;

M3BDemoHelper m3bDemo;

/**
 * @brief Setup the hardware, called once
 * LEDs, serial, sensors, twoWire
 */
void setup() {
  // put your setup code here, to run once:
  m3bDemo.begin();

  SerialM3B.begin(9600);
  SerialMioty.begin(9600);
  SerialM3B.println("Starting Demo ");
  
  adxl362.begin(PA8);
  adxl362.beginMeasure();
  
  Wire2.begin();
  if (ms5637.begin(Wire2) == false)
  {
    SerialM3B.println("MS5637 sensor did not respond. Please check wiring.");
    while(1);
  }
  sht31.begin(0x44, &Wire2);
  si1141.begin(&Wire2);
}

/**
 * @brief Super Loop, called periodically
 * sendWeatherData (Blue LED active) and sendAccelerationData (red LED active) every minute
 */
void loop() {
  digitalWrite(BLUE_LED, LOW);
  float temperature = sendWeatherData();
  digitalWrite(BLUE_LED, HIGH);

  digitalWrite(RED_LED, LOW);
  sendAccelerationData(temperature);
  digitalWrite(RED_LED, HIGH);

  delay(60000);
}

/**
 * @brief send weather data over mioty
 * read temperatur, pressure, humidity, luminosity
 * tramsmit sensor values over mioty with blueprint id=0
 */
float sendWeatherData() {
  // pressure in hPa, temperature in °C
  float pres=0., temp=0., hum=0.;
  temp = ms5637.getTemperature();
  pres = ms5637.getPressure();
  sht31.read();
  hum = sht31.getHumidity();
  uint16_t lux;
  si1141.readLuminosity(&lux);

  SerialM3B.print("Pressure [hPa] ");
  SerialM3B.println(pres);
  SerialM3B.print("Temperature [°C] ");
  SerialM3B.println(temp);
  SerialM3B.print("Humidity ");
  SerialM3B.println(hum);
  SerialM3B.print("Luminosity (rawData) ");
  SerialM3B.print(lux);
  m3bDemo.transmitWeather(pres, temp, hum, lux);
  delay(4000);
  SerialM3B.println("");
  return temp;
}

/**
 * @brief send acceleration data over mioty
 * read acceleration sensor
 * tramsmit sensor values over mioty with blueprint id=1
 */
void sendAccelerationData(float temp) {
  int16_t temperature, acc_x, acc_y, acc_z;
  adxl362.readXYZTData(acc_x, acc_y, acc_z, temperature);
  char buff[50];
  sprintf(buff, "Acc. x,y,z: %i, %i, %i", acc_x, acc_y, acc_z);
  SerialM3B.print(buff);
  
  m3bDemo.transmitAcceleration(temp, acc_x, acc_y, acc_z);
  delay(4000);
  SerialM3B.println("");
}
