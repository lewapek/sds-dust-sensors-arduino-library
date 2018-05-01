#include <Arduino.h>

struct Command {
  const static int length = 19;
  const static int headIndex = 0;
  const static int commandIdIndex = 1;
  const static int dataStartIndex = 2, dataEndIndex = 14;
  const static int devideIdByte1Index = 15;
  const static int devideIdByte2Index = 16;
  const static int checksumIndex = 17;
  const static int tailIndex = 18;

  const static byte head = 0xAA;
  const static byte tail = 0xAB;

  const static int responseLenght = 10;

  byte *bytes = NULL;
  byte responseId;

  Command(byte commandId,
          const byte *commandData,
          int commandDataLength,
          byte responseId,
          byte deviceIdByte1 = 0xFF,
          byte deviceIdByte2 = 0xFF): responseId(responseId) {
    bytes = new byte[length];

    bytes[headIndex] = head;
    bytes[commandIdIndex] = commandId;
    for (int i = 0; i < commandDataLength; ++i) {
      bytes[dataStartIndex + i] = commandData[i]; // data bytes
    }
    for (int i = dataStartIndex + commandDataLength; i <= dataEndIndex; ++i) {
      bytes[i] = 0x00; // other data bytes
    }
    bytes[devideIdByte1Index] = deviceIdByte1;
    bytes[devideIdByte2Index] = deviceIdByte2;
    bytes[checksumIndex] = calculateChecksum();
    bytes[tailIndex] = tail;
  }

  ~Command() {
    if (bytes != NULL) {
      delete bytes;
    }
  }

  byte getChecksum() {
    return bytes[checksumIndex];
  }

  int calculateChecksum() {
    int sum = 0;
    for (int i = dataStartIndex; i <= dataEndIndex; ++i) {
      sum += bytes[i];
    }
    sum += bytes[devideIdByte1Index] + bytes[devideIdByte2Index];
    return (sum % 256);
  }
};

namespace Commands {
  // reporting mode
  const static byte queryReportingModeData[1] = {0x02};
  const static Command queryReportingMode(0xB4, queryReportingModeData, sizeof(queryReportingModeData), 0xC5);

  static const byte setActiveReportingModeData[3] = {0x02, 0x01, 0x01};
  static const Command setActiveReportingMode(0xB4, setActiveReportingModeData, sizeof(setActiveReportingModeData), 0xC5);

  static const byte setQueryReportingModeData[2] = {0x02, 0x01};
  static const Command setQueryReportingMode(0xB4, setQueryReportingModeData, sizeof(setQueryReportingModeData), 0xC5);

  // query data
  static const byte queryPmData[1] = {0x04};
  static const Command queryPm(0xB4, queryPmData, sizeof(queryPmData), 0xC0);

  // set device id
  static const byte setDeviceIdData[1] = {0x05};
  static const Command setDeviceId(0xB4, setDeviceIdData, sizeof(setDeviceIdData), 0xC5);

  // sleep and work
  static const byte queryWorkingStateData[1] = {0x06};
  static const Command queryWorkingState(0xB4, queryWorkingStateData, sizeof(queryWorkingStateData), 0xC5);

  static const byte sleepData[2] = {0x06, 0x01};
  static const Command sleep(0xB4, sleepData, sizeof(sleepData), 0xC5);

  static const byte wakeupData[3] = {0x06, 0x01, 0x01};
  static const Command wakeup(0xB4, wakeupData, sizeof(wakeupData), 0xC5);

  // working period
  static const byte queryWorkingPeriodData[1] = {0x08};
  static const Command queryWorkingPeriod(0xB4, queryWorkingPeriodData, sizeof(queryWorkingPeriodData), 0xC5);

  static const byte setContinuousWorkingPeriodData[2] = {0x08, 0x01};
  static const Command setContinuousWorkingPeriod(0xB4, setContinuousWorkingPeriodData, sizeof(setContinuousWorkingPeriodData), 0xC5);

  static const byte setCustomWorkingPeriodData[2] = {0x08, 0x01};
  static const Command setCoustomWorkingPeriod(0xB4, setCustomWorkingPeriodData, sizeof(setCustomWorkingPeriodData), 0xC5);

  // firmware version
  static const byte queryFirmwareVersionData[1] = {0x07};
  static const Command queryFirmwareVersion(0xB4, queryFirmwareVersionData, sizeof(queryFirmwareVersionData), 0xC5);
};



//
// // sleep and work
// static const byte sleepData[2] = {0x06, 0x01};
// static const Command sleepCommand(0xB4, sleepData, sizeof(sleepData), 0xC5);
//
// static const byte workData[3] = {0x06, 0x01, 0x01};
// static const Command workCommand(0xB4, workData, sizeof(workData), 0xC5);
//
// static const byte queryWorkingStateData[1] = {0x06};
// static const Command workQueryCommand(0xB4, queryWorkingStateData, sizeof(queryWorkingStateData), 0xC5);
//
//
// static const byte workPeriodData[2] = {0x08, 0x01};
// static const Command workPeriodCommand(0xB4, workPeriodData, sizeof(workPeriodData), 0xC5);
