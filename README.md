# Nova Fitness SDS dust sensors arduino library
Supports Nova Fitness SDS011 however should also work for other Nova Fitness SDS sensors as well.
This library attempts to provide easy-to-use abstraction over [Laser Dust Sensor Control Protocol V1.3](https://cdn.sparkfun.com/assets/parts/1/2/2/7/5/Laser_Dust_Sensor_Control_Protocol_V1.3.pdf).
Each response coming from sensor is validated whether it has correct head, command id, checksum and tail.

## Quickstart
```
#include "SdsDustSensor.h"

int rxPin = D1;
int txPin = D2;
SdsDustSensor sds(rxPin, txPin);

void setup() {
  Serial.begin(9600);
  sds.begin();

  Serial.println(sds.queryFirmwareVersion().toString()); // prints firmware version
  Serial.println(sds.setActiveReportingMode().toString()); // ensures sensor is in 'active' reporting mode
  Serial.println(sds.setContinuousWorkingPeriod().toString()); // ensures sensor has continuous working period - default but not recommended
}

void loop() {
  PmResult pm = sds.readPm();
  if (pm.isOk()) {
    Serial.print("PM2.5 = ");
    Serial.print(pm.pm25);
    Serial.print(", PM10 = ");
    Serial.println(pm.pm10);

    // if you want to just print the measured values, you can use toString() method as well
    Serial.println(pm.toString());
  } else {
    Serial.print("Could not read values from sensor, reason: ");
    Serial.println(pm.statusToString());
  }

  delay(500);
}
```
For more examples see [examples](examples/) folder.

## Initilization
Communication with sensor can be handled by SoftwareSerial or HardwareSerial. You can pass SoftwareSerial or HardwareSerial directly to the constructor or provide rx & tx pins (library will use SoftwareSerial then).

### Internal SoftwareSerial
```
int rxPin = D1;
int txPin = D2;
SdsDustSensor sds(rxPin, txPin);
sds.begin(); // you can pass custom baud rate as parameter (9600 by default)
```

### Explicit SoftwareSerial
```
int rxPin = D1;
int txPin = D2;
SoftwareSerial softwareSerial(rxPin, txPin);
SdsDustSensor sds(softwareSerial);
sds.begin(); // you can pass custom baud rate as parameter (9600 by default)
```

### Explicit HardwareSerial
```
SdsDustSensor sds(Serial1); // passing HardwareSerial as parameter
sds.begin(); // you can pass custom baud rate as parameter (9600 by default)
```

## Supported operations
All operations listed in [Laser Dust Sensor Control Protocol V1.3](https://cdn.sparkfun.com/assets/parts/1/2/2/7/5/Laser_Dust_Sensor_Control_Protocol_V1.3.pdf) are fully supported. They are listed below:
* read PM2.5, PM10 values (reads data from software serial, does not write anything to the serial, all other operations write date to the sensor and expect response - sensor should be set to "active" reporting mode),
* query PM2.5, PM10 values (in opposite to above, this one sends command to sensor and it responds with PM2.5 and PM10 values - sensor should be set to "query" reporting mode),
* query reporting mode,
* set reporting mode to "active",
* set reporting mode to "query",
* set new device id,
* query working state,
* set working state to "sleeping",
* set working state to "working",
* query working period,
* set "continuous" working period (factory default - sensor sends pm values every 1 second),
* set "custom" working period (recommended setting - 1-30min cycles),
* check firmware version (year, month, day).

Additionally you can read device id from every sensor response.

### Reading PM2.5 and PM10 values
The following function (readPm()) checks whether there is available data sent from sensor, it does not send any request to sensor so it has to be in 'active' reporting mode.
```
PmResult result = sds.readPm();
result.pm25; // float
result.pm10; // float
```

### Querying PM2.5 and PM10 values
In opposite to above function, this one sends request to sensor and expects the response. Sensor should be in 'query' reporting mode.
```
PmResult result = sds.queryPm();
result.pm25; // float
result.pm10; // float
```

### Setting custom working period - recommended over continuous
In order to set custom working period you need to specify single argument - duration (minutes) of the cycle. One cycle means working 30 sec, doing measurement and sleeping for ```duration-30 [sec]```. This setting is recommended when using 'active' reporting mode.
```
int cycleMinutes = 4;
WorkingPeriodResult result = sds.setCustomWorkingPeriod(cycleMinutes);
result.period; // 4
result.isContinuous(); // false
result.toString();
```

### Setting reporting mode to 'query'
When 'query' mode is active the sensor will not send data automatically, you need to send `sds.queryPm()` command on order to measure PM values.
```
ReportingModeResult result = sds.setQueryReportingMode();
result.mode; // MODE::QUERY
```

### Setting sensor state to 'sleeping'
```
WorkingStateResult result = sds.sleep();
result.isWorking(); // false
```

### Waking up
```
WorkingStateResult result = sds.wakeup();
result.isWorking(); // true
```

### Other functions
Responses format of other functions can be found in [src](src/) folder.

### Helpful methods
Additionally with every sensor result you can:
* access result status ```result.status```, which can be one of {Ok, NotAvailable, InvalidChecksum, InvalidResponseId, InvalidHead, InvalidTail},
* easily check whether response is correct with ```result.isOk()``` method,
* convert status to string with ```result.statusToString()``` method.

You can also access ```result.deviceId()``` (pointer to the 1st (of 2) device id byte) and ```result.rawBytes``` (pointer to raw sensor response - byte array).

### References
* [Laser Dust Sensor Control Protocol V1.3](https://cdn.sparkfun.com/assets/parts/1/2/2/7/5/Laser_Dust_Sensor_Control_Protocol_V1.3.pdf)
* http://www.inovafitness.com/en/a/chanpinzhongxin/95.html
