#ifndef QUADOMMNICYTRON_H
#define QUADOMMNICYTRON_H

#include<CytronDriver.h>

class Quadbase{
  private:
    HardwareSerial *serial;
    byte addressfl, addressfr,addressbr,addressbl;
    short speedfl,speedfr,speedbr,speedbl;
    CytronDriver CMotorfl;
    CytronDriver CMotorfr;
    CytronDriver CMotorbr;
    CytronDriver CMotorbl;
  
  public:
   Quadbase(HardwareSerial *_serial,byte _addressfl,byte _addressfr,byte _addressbr,byte _addressbl);
   void initiate();
   void setSpeeed(short _speedfl,short _speedfr,short _speedbr,short _speedbl);
   void forward();
   void reverse();
   void left();
   void right();
   void forwardleft();
   void forwardright();
   void reverseright();
   void reverseleft();
   void clockwise();
   void antiClockwise();
   void brake();
};

#endif
