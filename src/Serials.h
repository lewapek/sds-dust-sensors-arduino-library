#ifndef __SDS_ABSTRACT_SERIAL_H__
#define __SDS_ABSTRACT_SERIAL_H__

#include <SoftwareSerial.h>
#include <HardwareSerial.h>

namespace Serials {

  struct AbstractSerial {
    virtual void begin(int baudRate) ;
    virtual Stream *getStream();
    virtual ~AbstractSerial() {}
  };

  struct Hardware: public AbstractSerial {
    Hardware(HardwareSerial &serial): serial(serial) {}

    void begin(int baudRate) {
      serial.begin(baudRate);
    }

    Stream *getStream() {
      return &serial;
    }

    HardwareSerial &serial;
  };

  struct Software: public AbstractSerial {
    Software(SoftwareSerial &serial): serial(serial) {}

    void begin(int baudRate) {
      serial.begin(baudRate);
    }

    Stream *getStream() {
      return &serial;
    }

    SoftwareSerial &serial;
  };

  struct InternalSoftware: public AbstractSerial {
    InternalSoftware(const int &pinRx, const int &pinTx):
      serial(SoftwareSerial(pinRx, pinTx)) {}

    void begin(int baudRate) {
      serial.begin(baudRate);
    }

    Stream *getStream() {
      return &serial;
    }

    SoftwareSerial serial;
  };

}

#endif // __SDS_ABSTRACT_SERIAL_H__
