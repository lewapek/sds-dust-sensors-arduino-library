# Nova Fitness SDS011 dust sensor arduino library
Supports SDS011 however code should work for other SDS sensors as well.
Communication with sensor is handled via SoftwareSerial.

## Supported operations
Each data coming from sensor is checked whether it has correct head, command id, checksum and tail.
All operations listed in [Laser Dust Sensor Control Protocol V1.3](https://cdn.sparkfun.com/assets/parts/1/2/2/7/5/Laser_Dust_Sensor_Control_Protocol_V1.3.pdf) are fully supported. They are listed below:
* read PM2.5, PM10 values (reads data from software serial, does not write anuthing to the serial, all other operations write date to the sensor and expect response - sensor should be set to "active" reporting mode),
* query PM2.5, PM10 values (in opposite to above, this one sends command to sensor and it responds with PM2.5 and PM10 values - sensor should be set to "query" reporting mode),
* query reporting mode,
* set reporting mode to "active",
* set reporting mode to "query",
* set new device id,
* query working state,
* set working state to "sleeping",
* set working state to "working",
* query working period,
* set "countinuous" working period (factory default - sensor sends pm values every 1 second),
* set "custom" working period (recommended setting - 1-30min cycles),
* check firmware version (year, month, day).

Additionally you can read device id from every sensor response.

### Reading PM2.5 and PM10 values
```
PmResult result = sensor.readPm();
float pm25 = result.pm25;
float pm10 = result.pm10;
```
