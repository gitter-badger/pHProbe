[![GitHub release](https://img.shields.io/github/release/u-fire/pHProbe.svg)]()
[![Codacy grade](https://img.shields.io/codacy/grade/5a3eff9c5b7c49c5ba03cbe2b4435c3e.svg)]()

### pH Probe Interface

[![Join the chat at https://gitter.im/pHProbe/Lobby](https://badges.gitter.im/pHProbe/Lobby.svg)](https://gitter.im/pHProbe/Lobby?utm_source=badge&utm_medium=badge&utm_campaign=pr-badge&utm_content=badge)

> Measure pH easily
* pH
* pOH
* mV
* temperature in Celsius

Make one with the [source code](https://github.com/u-fire/ph-probe-interface) and [schematics](https://upverter.com/justind000/10bda5b8c95ab94b/pH-Meter/).  

#### What it is
An ATTiny85 programmed as an I2C slave, a DS18B20 waterproof temperature probe, and a pH probe. It measures pH in the full range of 0 - 14 and optionally compensates for temperature.

#### Using it
An Arduino-compatible [library](https://github.com/u-fire/pHProbe) is provided to make using the probe easy and there is extensive [documentation](http://ufire.co/pHProbe/) on the use and setup of the device.

~~~
#include <pHProbe.h>
pH_Probe _pH;

pH = _pH.measurepH();
~~~
