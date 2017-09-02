### pH Probe Interface

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
