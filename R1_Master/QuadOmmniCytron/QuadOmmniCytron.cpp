/*
  @File Name : QuadOmmniCytron
  @Author : Shreyash Patel
  @Version : 1.0
*/

/*
  #define Motorfl 0b00000001
  #define Motorfr 0b00000000
  #define Motorbr 0b00001001
  #define Motorbl 0b00001000
*/

#include "QuadOmmniCytron.h"

Quadbase::Quadbase(HardwareSerial *_serial, byte _addressfl, byte _addressfr, byte _addressbr, byte _addressbl)
  : CMotorfl(_serial, _addressfl), CMotorfr(_serial, _addressfr), CMotorbr(_serial, _addressbr), CMotorbl(_serial, _addressbl)
{
  serial = _serial;
  addressfl = _addressfl;
  addressfr = _addressfr;
  addressbr = _addressbr;
  addressbl = _addressbl;
}
void Quadbase::initiate()
{
  CMotorfl.initiate();
}
void Quadbase::setSpeeed(short _speedfl, short _speedfr, short _speedbr, short _speedbl)
{
  speedfl = _speedfl;
  speedfr = _speedfr;
  speedbr = _speedbr;
  speedbl = _speedbl;
}
void Quadbase::forward()
{
  CMotorfl.antiClockwise(speedfl);
  CMotorfr.clockwise(speedfr);
  CMotorbr.clockwise(speedbr);
  CMotorbl.antiClockwise(speedbl);
}
void Quadbase::reverse()
{
  CMotorfl.clockwise(speedfl);
  CMotorfr.antiClockwise(speedfr);
  CMotorbr.antiClockwise(speedbr);
  CMotorbl.clockwise(speedbl);
}
void Quadbase::left()
{
  CMotorfl.clockwise(speedfl);
  CMotorfr.clockwise(speedfr);
  CMotorbr.antiClockwise(speedbr);
  CMotorbl.antiClockwise(speedbl);
}
void Quadbase::right()
{
  CMotorfl.antiClockwise(speedfl);
  CMotorfr.antiClockwise(speedfr);
  CMotorbr.clockwise(speedbr);
  CMotorbl.clockwise(speedbl);
}
void Quadbase::forwardleft()
{
  CMotorfl.brake();
  CMotorfr.clockwise(speedfr);
  CMotorbr.brake();
  CMotorbl.antiClockwise(speedbl);
}
void Quadbase::forwardright()
{
  CMotorfl.antiClockwise(speedfl);
  CMotorfr.brake();
  CMotorbr.clockwise(speedbr);
  CMotorbl.brake();
}
void Quadbase::reverseright()
{
  CMotorfl.brake();
  CMotorfr.antiClockwise(speedfr);
  CMotorbr.brake();
  CMotorbl.clockwise(speedbl);
}
void Quadbase::reverseleft()
{
  CMotorfl.clockwise(speedfl);
  CMotorfr.brake();
  CMotorbr.antiClockwise(speedbr);
  CMotorbl.brake();
}
void Quadbase::clockwise()
{
  CMotorfl.antiClockwise(speedfl);
  CMotorfr.antiClockwise(speedfr);
  CMotorbr.antiClockwise(speedbr);
  CMotorbl.antiClockwise(speedbl);
}
void Quadbase::antiClockwise()
{
  CMotorfl.clockwise(speedfl);
  CMotorfr.clockwise(speedfr);
  CMotorbr.clockwise(speedbr);
  CMotorbl.clockwise(speedbl);
}
void Quadbase::brake()
{
  CMotorfl.brake();
  CMotorfr.brake();
  CMotorbr.brake();
  CMotorbl.brake();
}
