// Copyright (c) 2017 Justin Decker

//
// MIT License
//
// Permission is hereby granted, free of charge, to any person obtaining
// a copy of this software and associated documentation files (the
// "Software"), to deal in the Software without restriction, including
// without limitation the rights to use, copy, modify, merge, publish,
// distribute, sublicense, and/or sell copies of the Software, and to
// permit persons to whom the Software is furnished to do so, subject to
// the following conditions:
//
// The above copyright notice and this permission notice shall be
// included in all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
// EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
// MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
// NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
// LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
// OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
// WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

/*!
   \file pHProbe.h
   \brief phProbe Class Implementation
 */

#ifndef PHPROBE_H
#define PHPROBE_H
#include <Arduino.h>
#include <Wire.h>

#define PH_PROBE 0x47 /*!< pH probe I2C address */
#define PH_MEASURE_PH 80
#define PH_MEASURE_TEMP 40
#define PH_CALIBRATE_SINGLE 20
#define PH_CALIBRATE_LOW 10
#define PH_CALIBRATE_HIGH 8

#define PH_VERSION_REGISTER 0             /*!< version */
#define PH_PH_REGISTER 1                  /*!< pH */
#define PH_TEMP_REGISTER 5                /*!< temperature in C */
#define PH_CALIBRATE_SINGLE_REGISTER 9    /*!< calibration offset */
#define PH_CALIBRATE_REFHIGH_REGISTER 13  /*!< reference high calibration */
#define PH_CALIBRATE_REFLOW_REGISTER 17   /*!< reference low calibration */
#define PH_CALIBRATE_READHIGH_REGISTER 21 /*!< reading high calibration */
#define PH_CALIBRATE_READLOW_REGISTER 25  /*!< reading low calibration */
#define PH_SOLUTION_REGISTER  29          /*!< reference pH solution */
#define PH_TEMP_COMPENSATION_REGISTER 33  /*!< temperature compensation constant */
#define PH_MV 34                          /*!< measured mV from pH probe */
#define PH_CONFIG_REGISTER 38             /*!< config register */
#define PH_TASK_REGISTER 39               /*!< task register */

#define PH_TEMP_MEASURE_TIME 750
#define PH_PH_MEASURE_TIME 12000

#define PH_DUALPOINT_CONFIG_BIT 0         /*!< dual point config bit */
#define PH_TEMP_COMPENSATION_CONFIG_BIT 1 /*!< temperature compensation config bit */

class pH_Probe                            /*! pH Class */
{
public:

  float pH;    /*!< pH */
  float tempC; /*!< Temperature in C */
  float tempF; /*!< Temperature in F */
  float mV;    /*!< mV of probe */
  float pOH;   /*!< pOH */
  pH_Probe();
  ~pH_Probe();
  float measurepH();
  float measureTemp();
  void  calibrateSingle(float solutionpH);
  void  calibrateProbeLow(float solutionpH);
  void  calibrateProbeHigh(float solutionpH);
  void  setDualPointCalibration(float refLow,
                                float refHigh,
                                float readLow,
                                float readHigh);
  float getCalibrateOffset();
  void  setTempConstant(byte b);
  byte  getTempConstant();
  void  useTemperatureCompensation(bool b);
  bool  usingTemperatureCompensation();
  void  useDualPoint(bool b);
  bool  usingDualPoint();
  float getCalibrateHigh();
  float getCalibrateLow();
  float getCalibrateHighReading();
  float getCalibrateLowReading();
  byte  getVersion();
  void  reset();

private:

  void  _change_register(byte register);
  void  _send_command(byte command);
  void  _write_register(byte  reg,
                        float f);
  void  _write_byte(byte reg,
                    byte val);
  float _read_register(byte reg);
  byte  _read_byte(byte reg);
};

#endif // ifndef PHPROBE_H
