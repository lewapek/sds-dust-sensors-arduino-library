#include "Sds011Command.h"
#include <SoftwareSerial.h>

class Sds011Sensor {
public:
  enum class Status {
    Ok, NotAvailable, InvalidChecksum, InvalidCommandId, InvalidHead, InvalidTail
  };

  Sds011Sensor(int pinRx, int pinTx, int baudRate):
    sdsSerial(new SoftwareSerial(pinRx, pinTx)),
    baudRate(baudRate) {}

  ~Sds011Sensor() {
    if (sdsSerial != NULL) {
      delete sdsSerial;
    }
  }

  const char *toString(Status status) {
    switch (status) {
      case Status::Ok: return "Ok";
      case Status::NotAvailable: return "Not available";
      case Status::InvalidChecksum: return "Invalid checksum";
      case Status::InvalidCommandId: return "Invalid commandId";
      case Status::InvalidHead: return "Invalid head";
      case Status::InvalidTail: return "Invalid tail";
      default: return "undefined status";
    }
  }

  void begin() {
    sdsSerial->begin(baudRate);
  }

  byte *getResponse() {
    return response;
  }

  Status sleep() {
    const Command *command = &Commands::sleep;
    write(command);
    return readIntoBytes(command->responseId);
  }

  Status wakeup() {
    const Command *command = &Commands::wakeup;
    write(command);
    return readIntoBytes(command->responseId);
  }

  Status queryWorkingState() {
    const Command *command = &Commands::queryWorkingState;
    write(command);
    return readIntoBytes(command->responseId);
  }

  void write(const Command *command) {
    sdsSerial->flush();
    for (int i = 0; i < Command::length; ++i) {
      sdsSerial->write(command->bytes[i]);
      Serial.print("|");
      Serial.print(command->bytes[i], HEX);
    }
    Serial.println("|");
    delay(500);
  }

  Status readIntoBytes(byte responseId) {
    int checksum = 0;
    int readBytesQuantity = 0;
    while ((sdsSerial->available() > 0) &&
           (sdsSerial->available() >= Command::responseLenght - readBytesQuantity)) {
      byte readByte = sdsSerial->read();
      response[readBytesQuantity] = readByte;

      Serial.print("reading byte ");
      Serial.print(readBytesQuantity);
      Serial.print(": ");
      Serial.println(readByte, HEX);

      ++readBytesQuantity;
      switch (readBytesQuantity) {
        case 1:
          if (readByte != 0xAA) {
            return Status::InvalidHead;
          }
          break;
        case 2:
          if (readByte != responseId) {
            return Status::InvalidCommandId;
          }
          break;
        case 3 ... 8:
          checksum += readByte;
          break;
        case 9:
          if (readByte != checksum % 256) {
            return Status::InvalidChecksum;
          }
          break;
        case 10:
          if (readByte != 0xAB) {
            return Status::InvalidTail;
          }
          break;
      }
      if (readBytesQuantity == Command::responseLenght) {
        return Status::Ok;
      }
      yield();
    }
    return Status::NotAvailable;
  }

private:
  byte response[Command::responseLenght];
  int baudRate;
  SoftwareSerial *sdsSerial = NULL;
};
