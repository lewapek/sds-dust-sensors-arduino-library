#include "SdsDustSensor.h"

int rxPin = D1;
int txPin = D2;
SdsDustSensor sds(rxPin, txPin);

void setup() {
  Serial.begin(9600);
  sds.begin();

  Serial.println(sds.queryFirmwareVersion().toString()); // prints firmware version
  Serial.println(sds.setActiveReportingMode().toString()); // ensures sensor is in 'active' reporting mode
  Serial.println(sds.setCustomWorkingPeriod(3).toString()); // sensor sends data every 3 minutes
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
    // notice that loop delay is set to 5s (sensor sends data every 3 minutes) and some reads are not available
    Serial.print("Could not read values from sensor, reason: ");
    Serial.println(pm.statusToString());
  }

  delay(5000);
}
