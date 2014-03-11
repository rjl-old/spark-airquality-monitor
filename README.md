# Spark Core air quality monitor

## Project scope

This code is the firmware and python script for an internet connected air quality monitor built on a [Spark Core](https://www.spark.io).

The aim of the project is to build a Spark Core device which measures room temperature, humidity and carbon dioxide levels, and logs the data in a database for further analysis.

## Current Status (11 March 2014)

The core senses temperature from a [TMP36](http://www.analog.com/en/mems-sensors/digital-temperature-sensors/tmp36/products/product.html). It sets a status led 'green' if the temperature is within a pre-defined comfort zone, red otherwise. It also sets the angle of a servo to drive an analogue temperature display.

A python script reads the sensor and relays it to [TempoDB](https://tempo-db.com) ([Example plot](https://tempo-db.com/database/381560dfae8a4beda14ab015a944026d/series/ccfbc6915efd4730b4b5190366de36ec/?start=2014-03-10T00%3A00%3A00.000Z&end=2014-03-10T23%3A59%3A59.000Z&interval=&function=))

## Firmware

`TEMP_LOW` and `TEMP_HIGH` define the centigrade temperature range of the comfort zone and exposes them as Spark variables of the same name.

`TEMP_MIN` and `TEMP_MAX` define the centrigrade temperature range of the servo minimum (0 degrees) and maximum (180 degrees) angle and exposes them as Spark variables of the same name.

`TMP36` voltage is converted to centigrade temperature and exposed as Spark variable `temperature`.

The onboard multicolour LED is set to 'blue' at the lower end of the comfort zone and 'red' at the upper end. This can be disabled.

The device refreshes at interval `interval` milliseconds. The onboard `D7` flashes each loop.

Function `program` allows device parameters to be altered. 

## Python script

A base class `Core.py` provides generic Spark Core state information and connection functionality, and exposes variables and functions. API functionality is provided by the `Hammock` module.

A derived class `Temperature.py` exposes temperature readings and allows parameters to be set.

A utility script `monitor.py` establishes a `Temperature` class instance and a `TempoDB` client. It reads the temperature, prints it to the terminal, and uploads the data to TempoDB.

A configuration file (not included) provides authentication details for Spark and TempoDB APIs.