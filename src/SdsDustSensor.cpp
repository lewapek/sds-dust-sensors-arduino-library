#include "SdsDustSensor.h"

void SdsDustSensor::write(const Command &command) {
  sdsSerial->flush();
  for (int i = 0; i < Command::length; ++i) {
    sdsSerial->write(command.bytes[i]);
    #ifdef __DEBUG_SDS_DUST_SENSOR__
    Serial.print("|");
    Serial.print(command.bytes[i], HEX);
    #endif
  }
  #ifdef __DEBUG_SDS_DUST_SENSOR__
  Serial.println("| <- written bytes");
  #endif
  delay(500);
}

Status SdsDustSensor::readIntoBytes(byte responseId) {
  int checksum = 0;
  int readBytesQuantity = 0;

  while ((sdsSerial->available() > 0) &&
        (sdsSerial->available() >= Result::lenght - readBytesQuantity)) {
    byte readByte = sdsSerial->read();
    response[readBytesQuantity] = readByte;

    #ifdef __DEBUG_SDS_DUST_SENSOR__
    Serial.print("|");
    Serial.print(readByte, HEX);
    #endif

    ++readBytesQuantity;
    switch (readBytesQuantity) {
      case 1:
        if (readByte != 0xAA) {
          #ifdef __DEBUG_SDS_DUST_SENSOR__
          Serial.println("| <- read bytes with invalid head error");
          #endif
          return Status::InvalidHead;
        }
        break;
      case 2:
        if (readByte != responseId) {
          #ifdef __DEBUG_SDS_DUST_SENSOR__
          Serial.println("| <- read bytes with invalid response id");
          #endif
          return Status::InvalidResponseId;
        }
        break;
      case 3 ... 8:
        checksum += readByte;
        break;
      case 9:
        if (readByte != checksum % 256) {
          #ifdef __DEBUG_SDS_DUST_SENSOR__
          Serial.println("| <- read bytes with invalid checksum");
          #endif
          return Status::InvalidChecksum;
        }
        break;
      case 10:
        if (readByte != 0xAB) {
          #ifdef __DEBUG_SDS_DUST_SENSOR__
          Serial.println("| <- read bytes with invalid tail");
          #endif
          return Status::InvalidTail;
        }
        break;
    }
    if (readBytesQuantity == Result::lenght) {
      #ifdef __DEBUG_SDS_DUST_SENSOR__
      Serial.println("| <- read bytes with success");
      #endif
      return Status::Ok;
    }
    yield();
  }

  #ifdef __DEBUG_SDS_DUST_SENSOR__
  Serial.println("| <- read bytes with no more available");
  #endif

  return Status::NotAvailable;
}
