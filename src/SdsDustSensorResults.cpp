#include "SdsDustSensorResults.h"

String Result::statusToString() {
  switch (status) {
    case Status::Ok: return "Ok";
    case Status::NotAvailable: return "Not available";
    case Status::InvalidChecksum: return "Invalid checksum";
    case Status::InvalidResponseId: return "Invalid response id";
    case Status::InvalidHead: return "Invalid head";
    case Status::InvalidTail: return "Invalid tail";
    case Status::InvalidFirmwareVersion: return "Invalid firmware version";
    default: return "undefined status";
  }
}
