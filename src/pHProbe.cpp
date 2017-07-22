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
   \file pHProbe.cpp
   \brief pH Probe Class Implementation
 */

#include "pHProbe.h"

/*!
   \brief Class constructor
 */
pH_Probe::pH_Probe()
{
  Wire.begin();
}

/*!
   \brief Class destructor
 */
pH_Probe::~pH_Probe()
{}

/*!
   \code
   pH.measurepH();
   \endcode
   \brief Starts a pH measurement.
   \return pH
 */
float pH_Probe::measurepH()
{
  _send_command(PH_MEASURE_PH);
  delay(PH_PH_MEASURE_TIME);
  pH = _read_register(PH_PH_REGISTER);

  double places = pow(10.0, 2);
  pH = round(pH * places) / places;
  return pH;
}

/*!
   \code
   pH.measureTemp();
   \endcode
   \brief Starts a temperature measurement
   \post #tempC and #tempF are updated
   \note A value of -127 means the device is not connected.
   \return temperature in C
 */
float pH_Probe::measureTemp()
{
  _send_command(PH_MEASURE_TEMP);
  delay(PH_TEMP_MEASURE_TIME);
  tempC = _read_register(PH_TEMP_REGISTER);
  tempF = ((tempC * 9) / 5) + 32;
  return tempC;
}

/*!
   \code
   pH.calibrateZero();
   \endcode
   \brief Determines zero-point of the pH probe.
   \post result will be saved in the device's EEPROM and used
   automatically thereafter
 */
void pH_Probe::calibrateZero()
{
  _send_command(PH_CALIBRATE_ZERO);
  delay(PH_PH_MEASURE_TIME);
}

/*!
   \code
   pH.calibrateProbeLow(4.0);
   \endcode
   \brief Calibrates the dual-point values for the low reading and saves them
   in the devices's EEPROM.
   \param solutionpH          the pH of the calibration solution
 */
void pH_Probe::calibrateProbeLow(float solutionpH)
{
  _write_register(PH_SOLUTION_REGISTER, solutionpH);
  _send_command(PH_CALIBRATE_LOW);
  delay(PH_PH_MEASURE_TIME);
}

/*!
   \code
   pH.calibrateProbeHigh(7.0);
   \endcode
   \brief Calibrates the dual-point values for the high reading and saves them
   in the devices's EEPROM.
   \param solutionpH          the pH of the calibration solution
 */
void pH_Probe::calibrateProbeHigh(float solutionpH)
{
  _write_register(PH_SOLUTION_REGISTER, solutionpH);
  _send_command(PH_CALIBRATE_HIGH);
  delay(PH_PH_MEASURE_TIME);
}

/*!
   \code
   pH.setDualPointCalibration(4.0, 7.0, 3.8, 7.2);
   \endcode
   \brief Sets all the values for dual point calibration and saves them
   in the devices's EEPROM.
   \param refLow            the reference low point
   \param refHigh           the reference high point
   \param readLow           the measured low point
   \param readHigh          the measured high point
 */
void pH_Probe::setDualPointCalibration(float refLow,
                                       float refHigh,
                                       float readLow,
                                       float readHigh)
{
  _write_register(PH_CALIBRATE_REFLOW_REGISTER,   refLow);
  _write_register(PH_CALIBRATE_REFHIGH_REGISTER,  refHigh);
  _write_register(PH_CALIBRATE_READLOW_REGISTER,  readLow);
  _write_register(PH_CALIBRATE_READHIGH_REGISTER, readHigh);
}

/*!
   \code
   float z = pH.getZero();
   \endcode
   \brief Retrieves the zero-point offset from the device.
   \return   the probe's offset
 */
float pH_Probe::getZero()
{
  return _read_register(PH_CALIBRATE_ZERO_REGISTER);
}

/*!
   \code
   float calHigh = pH.getCalibrateHigh();
   \endcode
   \brief Retrieves the dual-point calibration high value
   \return   the dual-point calibration high value
 */
float pH_Probe::getCalibrateHigh()
{
  return _read_register(PH_CALIBRATE_REFHIGH_REGISTER);
}

/*!
   \code
   float calLow = pH.getCalibrateLow();
   \endcode
   \brief Retrieves the dual-point calibration low value
   \return   the dual-point calibration low value
 */
float pH_Probe::getCalibrateLow()
{
  return _read_register(PH_CALIBRATE_REFLOW_REGISTER);
}

/*!
   \code
   pH.useTemperatureCompensation(true);
   \endcode
   \brief Configures device to use temperature compensation or not
   \param b   true for false
 */
void pH_Probe::useTemperatureCompensation(bool b)
{
  byte retval;

  retval = _read_byte(PH_CONFIG_REGISTER);

  if (b)
  {
    retval = bitSet(retval, PH_TEMP_COMPENSATION_CONFIG_BIT);
  }
  else
  {
    retval = bitClear(retval, PH_TEMP_COMPENSATION_CONFIG_BIT);
  }
  _write_byte(PH_CONFIG_REGISTER, retval);
}

/*!
   \code
   pH.useDualPoint(true);
   \endcode
   \brief Configures device to use dual-point calibration
   \param b   true or false
 */
void pH_Probe::useDualPoint(bool b)
{
  byte retval;

  retval = _read_byte(PH_CONFIG_REGISTER);

  if (b)
  {
    retval = bitSet(retval, PH_DUALPOINT_CONFIG_BIT);
  }
  else
  {
    retval = bitClear(retval, PH_DUALPOINT_CONFIG_BIT);
  }
  _write_byte(PH_CONFIG_REGISTER, retval);
  Serial.println(retval);
}

/*!
   \code
   byte version = pH.getVersion();
   \endcode
   \brief Retrieves the firmware version of the device
   \return   version of firmware
 */
byte pH_Probe::getVersion()
{
  return _read_byte(PH_VERSION_REGISTER);
}

/*!
   \code
   pH.setTempConstant(25);
   \endcode
   \brief Configures device to use the provided temperature constant

   \param b   the temperature to use for compensation
 */
void pH_Probe::setTempConstant(byte b)
{
  _write_byte(PH_TEMP_COMPENSATION_REGISTER, b);
}

/*!
   \code
   byte temp = pH.getTempConstant();
   \endcode
   \brief Retrieves the temperature constant
   \return   the temperature to used for compensation
 */
byte pH_Probe::getTempConstant()
{
  return _read_byte(PH_TEMP_COMPENSATION_REGISTER);
}

/*!
   \code
   bool usingTemp = pH.usingTemperatureCompensation();
   \endcode
   \brief Determines if temperature compensation is being used
   \return   true if using compensation, false otherwise
 */
bool pH_Probe::usingTemperatureCompensation()
{
  byte retval = _read_byte(PH_CONFIG_REGISTER);

  return (retval >> PH_TEMP_COMPENSATION_CONFIG_BIT)  & 0x01;
}

/*!
   \code
   bool usingTemp = pH.usingDualPoint();
   \endcode
   \brief Determines if dual point calibration is being used
   \return   true if using compensation, false otherwise
 */
bool pH_Probe::usingDualPoint()
{
  byte retval = _read_byte(PH_CONFIG_REGISTER);

  return (retval >> PH_DUALPOINT_CONFIG_BIT)  & 0x01;
}

void pH_Probe::_change_register(byte r)
{
  Wire.beginTransmission(PH_PROBE);
  Wire.write(r);
  Wire.endTransmission();
  delay(10);
}

void pH_Probe::_send_command(byte command)
{
  Wire.beginTransmission(PH_PROBE);
  Wire.write(PH_TASK_REGISTER);
  Wire.write(command);
  Wire.endTransmission();
  delay(10);
}

void pH_Probe::_write_register(byte reg, float f)
{
  byte  b[5];
  float f_val = f;

  b[0] = reg;
  b[1] = *((uint8_t *)&f_val);
  b[2] = *((uint8_t *)&f_val + 1);
  b[3] = *((uint8_t *)&f_val + 2);
  b[4] = *((uint8_t *)&f_val + 3);
  Wire.beginTransmission(PH_PROBE);
  Wire.write(b, 5);
  Wire.endTransmission();
  delay(10);
}

float pH_Probe::_read_register(byte reg)
{
  float retval;

  _change_register(reg);
  Wire.requestFrom(PH_PROBE, 1);
  *((uint8_t *)&retval) = Wire.read();
  Wire.requestFrom(PH_PROBE, 1);
  *((uint8_t *)&retval + 1) = Wire.read();
  Wire.requestFrom(PH_PROBE, 1);
  *((uint8_t *)&retval + 2) = Wire.read();
  Wire.requestFrom(PH_PROBE, 1);
  *((uint8_t *)&retval + 3) = Wire.read();
  delay(10);
  return retval;
}

void pH_Probe::_write_byte(byte reg, byte val)
{
  byte b[5];

  b[0] = reg;
  b[1] = val;
  Wire.beginTransmission(PH_PROBE);
  Wire.write(b, 2);
  Wire.endTransmission();
  delay(10);
}

byte pH_Probe::_read_byte(byte reg)
{
  byte retval;

  _change_register(reg);
  Wire.requestFrom(PH_PROBE, 1);
  retval = Wire.read();
  delay(10);
  return retval;
}
