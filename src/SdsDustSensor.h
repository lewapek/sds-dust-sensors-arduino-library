#ifndef __SDS_DUST_SENSOR_H__
#define __SDS_DUST_SENSOR_H__

#include "SdsDustSensorCommands.h"
#include "SdsDustSensorResults.h"
#include <SoftwareSerial.h>

// #define __DEBUG_SDS_DUST_SENSOR__

class SdsDustSensor {
public:
  SdsDustSensor(int pinRx, int pinTx, int baudRate = 9600):
    sdsSerial(new SoftwareSerial(pinRx, pinTx)),
    baudRate(baudRate) {}

  ~SdsDustSensor() {
    if (sdsSerial != NULL) {
      delete sdsSerial;
    }
  }

  void begin() {
    sdsSerial->begin(baudRate);
  }

  byte *getLastResponse() {
    return response;
  }

  ReportingModeResult queryReportingMode() {
    Status status = execute(Commands::queryReportingMode);
    return ReportingModeResult(status, response);
  }

  ReportingModeResult setQueryReportingMode() {
    Status status = execute(Commands::setQueryReportingMode);
    return ReportingModeResult(status, response);
  }

  ReportingModeResult setActiveReportingMode() {
    Status status = execute(Commands::setActiveReportingMode);
    return ReportingModeResult(status, response);
  }

  PmResult queryPm() {
    Status status = execute(Commands::queryPm);
    return PmResult(status, response);
  }

  // warning: this method doesn't write anything to the sensor, it just reads incoming bytes
  PmResult readPm() {
    Status status = readIntoBytes(Commands::queryPm.responseId);
    return PmResult(status, response);
  }

  Result setDeviceId(byte newDeviceIdByte1, byte newDeviceIdByte2) {
    (Commands::setDeviceId).setDeviceId(newDeviceIdByte1, newDeviceIdByte2);
    Status status = execute(Commands::setDeviceId);
    return Result(status, response);
  }

  WorkingStateResult queryWorkingState() {
    Status status = execute(Commands::queryWorkingState);
    return WorkingStateResult(status, response);
  }

  WorkingStateResult sleep() {
    Status status = execute(Commands::sleep);
    return WorkingStateResult(status, response);
  }

  WorkingStateResult wakeup() {
    Status status = execute(Commands::wakeup);
    return WorkingStateResult(status, response);
  }

  WorkingPeriodResult queryWorkingPeriod() {
    Status status = execute(Commands::queryWorkingPeriod);
    return WorkingPeriodResult(status, response);
  }

  WorkingPeriodResult setContinuousWorkingPeriod() {
    (Commands::setWorkingPeriod).setContinuousWorkingPeriod();
    Status status = execute(Commands::setWorkingPeriod);
    return WorkingPeriodResult(status, response);
  }

  WorkingPeriodResult setCustomWorkingPeriod(byte workingPeriod) {
    (Commands::setWorkingPeriod).setCustomWorkingPeriod(workingPeriod);
    Status status = execute(Commands::setWorkingPeriod);
    return WorkingPeriodResult(status, response);
  }

  FirmwareVersionResult queryFirmwareVersion() {
    Status status = execute(Commands::queryFirmwareVersion);
    return FirmwareVersionResult(status, response);
  }

  Status execute(const Command &command) {
    write(command);
    return readIntoBytes(command.responseId);
  }

  void write(const Command &command);
  Status readIntoBytes(byte responseId);

private:
  byte response[Result::lenght];
  int baudRate;
  SoftwareSerial *sdsSerial = NULL;
};

#endif // __SDS_DUST_SENSOR_H__
