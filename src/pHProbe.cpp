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
    pH_Probe::measurepH();
   \endcode
   \brief Starts a pH measurement.
   \return pH
 */
float pH_Probe::measurepH()
{
  _send_command(PH_MEASURE_PH);
  delay(PH_PH_MEASURE_TIME);
  pH = _read_register(PH_PH_REGISTER);

  mV = _read_register(PH_MV_REGISTER) * 1000;

  pH  = round(pH * 100.0) / 100.0;
  pOH = abs(pH - 14);
  return pH;
}

/*!
   \code
    pH_Probe::measureTemp();
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
    pH_Probe::calibrateSingle(7.0);
   \endcode
   \brief Calibrates the probe using a single point.
   \post result will be saved in the device's EEPROM and used
   automatically thereafter
 */
void pH_Probe::calibrateSingle(float solutionpH)
{
  bool dualpoint = usingDualPoint();

  useDualPoint(false);
  _write_register(PH_SOLUTION_REGISTER, solutionpH);
  _send_command(PH_CALIBRATE_SINGLE);
  delay(PH_PH_MEASURE_TIME);
  useDualPoint(dualpoint);
}

/*!
   \code
    pH_Probe::calibrateProbeLow(4.0);
   \endcode
   \brief Calibrates the dual-point values for the low reading and saves them
   in the devices's EEPROM.
   \param solutionpH          the pH of the calibration solution
 */
void pH_Probe::calibrateProbeLow(float solutionpH)
{
  bool dualpoint = usingDualPoint();

  useDualPoint(false);
  _write_register(PH_SOLUTION_REGISTER, solutionpH);
  _send_command(PH_CALIBRATE_LOW);
  delay(PH_PH_MEASURE_TIME);
  useDualPoint(dualpoint);
}

/*!
   \code
   pH_Probe::calibrateProbeHigh(7.0);
   \endcode
   \brief Calibrates the dual-point values for the high reading and saves them
   in the devices's EEPROM.
   \param solutionpH          the pH of the calibration solution
 */
void pH_Probe::calibrateProbeHigh(float solutionpH)
{
  bool dualpoint = usingDualPoint();

  useDualPoint(false);
  _write_register(PH_SOLUTION_REGISTER, solutionpH);
  _send_command(PH_CALIBRATE_HIGH);
  delay(PH_PH_MEASURE_TIME);
  useDualPoint(dualpoint);
}

/*!
   \code
   pH_Probe::setDualPointCalibration(4.0, 7.0, 3.8, 7.2);
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
   float z = pH_Probe::getCalibrateOffset();
   \endcode
   \brief Retrieves the single-point offset from the device.
   \return   the probe's offset
 */
float pH_Probe::getCalibrateOffset()
{
  return _read_register(PH_CALIBRATE_SINGLE_REGISTER);
}

/*!
   \code
   float calHigh = pH_Probe::getCalibrateHigh();
   \endcode
   \brief Retrieves the reference-high calibration data
   \return   the reference-high
 */
float pH_Probe::getCalibrateHigh()
{
  return _read_register(PH_CALIBRATE_REFHIGH_REGISTER);
}

/*!
   \code
   float calHigh = pH_Probe::getCalibrateHighReading();
   \endcode
   \brief Retrieves the reading-high calibration data
   \return   the reading-high
 */
float pH_Probe::getCalibrateHighReading()
{
  return _read_register(PH_CALIBRATE_READHIGH_REGISTER);
}

/*!
   \code
   float calLow = pH_Probe::getCalibrateLow();
   \endcode
   \brief Retrieves the reference-low calibration data
   \return   the reference-low
 */
float pH_Probe::getCalibrateLow()
{
  return _read_register(PH_CALIBRATE_REFLOW_REGISTER);
}

/*!
   \code
   float calLow = pH_Probe::getCalibrateLowReading();
   \endcode
   \brief Retrieves the reference-low calibration data
   \return   the reading-low
 */
float pH_Probe::getCalibrateLowReading()
{
  return _read_register(PH_CALIBRATE_READLOW_REGISTER);
}

/*!
   \code
   pH_Probe::useTemperatureCompensation(true);
   \endcode
   \brief Configures device to use temperature compensation or not
   \param b   true for false
 */
void pH_Probe::useTemperatureCompensation(bool b)
{
  byte retval;
  byte config = _read_byte(PH_CONFIG_REGISTER);

  if (b)
  {
    retval = bitSet(config, PH_TEMP_COMPENSATION_CONFIG_BIT);
  }
  else
  {
    retval = bitClear(config, PH_TEMP_COMPENSATION_CONFIG_BIT);
  }
  _write_byte(PH_CONFIG_REGISTER, retval);
}

/*!
   \code
   pH_Probe::useDualPoint(true);
   \endcode
   \brief Configures device to use dual-point calibration
   \param b   true or false
 */
void pH_Probe::useDualPoint(bool b)
{
  byte retval;
  byte config = _read_byte(PH_CONFIG_REGISTER);

  if (b)
  {
    retval = bitSet(config, PH_DUALPOINT_CONFIG_BIT);
  }
  else
  {
    retval = bitClear(config, PH_DUALPOINT_CONFIG_BIT);
  }

  _write_byte(PH_CONFIG_REGISTER, retval);
  Serial.println(retval);
}

/*!
   \code
   byte version = pH_Probe::getVersion();
   \endcode
   \brief Retrieves the firmware version of the device
   \return   version of firmware
 */
byte pH_Probe::getVersion()
{
  return _read_byte(PH_VERSION_REGISTER);
}

/*!
   \brief Resets all the stored calibration information.
 */
void pH_Probe::reset()
{
  _write_register(PH_CALIBRATE_SINGLE_REGISTER,   NAN);
  _write_register(PH_CALIBRATE_REFHIGH_REGISTER,  NAN);
  _write_register(PH_CALIBRATE_REFLOW_REGISTER,   NAN);
  _write_register(PH_CALIBRATE_READHIGH_REGISTER, NAN);
  _write_register(PH_CALIBRATE_READLOW_REGISTER,  NAN);
}

/*!
   \code
   pH_Probe::setTempConstant(25);
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
   byte temp = pH_Probe::getTempConstant();
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
   bool usingTemp = pH_Probe::usingTemperatureCompensation();
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
   bool usingTemp = pH_Probe::usingDualPoint();
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
