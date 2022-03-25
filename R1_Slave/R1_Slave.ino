#include <Servo.h>
#include "Motor.h"
#include "RoboClaw.h"
#define INA 31
#define INB 33
#define PWM 7
#define DIR 37
#define CIMPWM 8
#define servo 6
#define address 0x80
#define DEBUG
#define endl "\n"
#define responsM2 23
#define responsM1 25

template<class T> inline Print& operator<<(Print& serial, T args) {
  serial.print(args);
  return serial;
}

//Hercules
Motor hercules(INA, INB, PWM);
Servo myservo;
RoboClaw roboclaw(&Serial1, 10000);

byte speed = 90;
byte ball = 3;
byte dataFromESP;
float angleM2 = 0, angleM1 = 0, posM2 = 0, posM1 = 0;
boolean status_Hercules = false, status_CIM = false;

void servo_rotate() {
  ball--;
  if (ball >= 0) {
    for (int i = 60 * (ball + 1); i <= 60 * ball; i -= 5)
    {
      myservo.write(i);
      delay(30);
    }
  } else
    myservo.write(180);
  ball = 3;
}

void destroyer() {
  digitalWrite(DIR, HIGH);
  analogWrite(CIMPWM, speed);
  //  for (int i = 90; i <= 210; i += 30) {
  //    analogWrite(CIMPWM, i);
  //    delay(100);
  //    if (i == 210) {
  //      return;
  //    }
  //  }
}

float mapf(float x, float in_min, float in_max, float out_min, float out_max)
{
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

//Setting encoder position according to angle for M1
void setEncoderM1(float angleM1) {
  posM1 = mapf(angleM1, 0, 90, 0, -14400);
  if (posM1 > -14400)
    roboclaw.SpeedAccelDeccelPositionM1(address, 0, 65000, 0, posM1, 1);
}

//Setting encoder position according to angle for M2
void setEncoderM2(float angleM2) {
  posM2 = mapf(angleM2, 0, 35, 0, 500);
  if (posM2 < 500)
    roboclaw.SpeedAccelDeccelPositionM2(address, 0, 65000, 0, posM2, 1);
}

void homing() { // Turret Moving to home position
  roboclaw.BackwardM2(address, 32);
  roboclaw.ForwardM1(address, 64);
  delay(3000);
  roboclaw.SetEncM1(address, 0);
  roboclaw.SetEncM2(address, 0);
  roboclaw.SpeedAccelDeccelPositionM1(address, 3000, 65000, 3000, -7200, 1);
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial3.begin(115200);
  roboclaw.begin(115200);
  homing();
  pinMode(DIR, OUTPUT);
  pinMode(CIMPWM, OUTPUT);
  myservo.attach(6);
  myservo.write(180);
}

void loop() {
  if (Serial3.available() > -1) {
    dataFromESP = Serial3.read();
    Serial.println(dataFromESP);
    switch (dataFromESP) {
      case 1:
        servo_rotate();
#ifdef DEBUG
        Serial << "Ball Loading..." << endl;
#endif
        break;
      case 2:
        if (!status_Hercules) {
          hercules.anti_clockwise(250);
#ifdef DEBUG
          Serial << "Hercules Started..." << endl;
#endif
          status_Hercules = true;
        } else {
          hercules.brake();
#ifdef DEBUG
          Serial << "Hercules Stopped..." << endl;
#endif
          status_Hercules = false;
        } break;
      case 3:
        if (!status_CIM) {
          destroyer();
#ifdef DEBUG
          Serial << "CIM Started..." << endl;
#endif
          status_CIM = true;
        } else {
          analogWrite(CIMPWM, 0);
#ifdef DEBUG
          Serial << "CIM Stopped..." << endl;
#endif
          status_CIM = false;
        } break;
      case 4:
        //        roboclaw.BackwardM1(address, 64);
        angleM2 = angleM2 < 36 ? angleM2 + 0.3 : 35;
        setEncoderM2(angleM2);
#ifdef DEBUG
        Serial << "Turret Up" << endl;
#endif
        break; // turret up
      case 5:
        angleM2 = angleM2 >= -1 ? angleM2 - 0.3 : 0;
        setEncoderM2(angleM2);
#ifdef DEBUG
        Serial << "Turret Down" << endl;
#endif
        break; // turret down
      case 6:
        angleM1 = angleM1 >= -1 ? angleM1 - 0.3 : 0;
        setEncoderM1(angleM1);
#ifdef DEBUG
        Serial << "Turret Rotation Movement Anti - Clockwise.." << endl;
#endif
        break;
      case 7:
        angleM1 = angleM1 < 91 ? angleM1 + 0.3 : 90;
        setEncoderM1(angleM1);
#ifdef DEBUG
        Serial << "Turret Rotation Movement Clockwise.." << endl;
#endif
        break;
      case 8:
        roboclaw.BackwardM1(address, 0);
#ifdef DEBUG
        Serial << "Turret Rotation Movement brake.." << endl;
#endif
        break;
      case 9:
        //        roboclaw.BackwardM2(address, 0);
#ifdef DEBUG
        Serial << "Turret angleM2 Movement Lock.." << endl;
#endif
        break;
      case 11:
        speed = speed > 89 ? speed - 10 : 90;
        destroyer();
#ifdef DEBUG
        Serial << "dspeed" << speed << endl;
#endif
        break;
      case 12:
        speed = speed < 256 ? speed + 10 : 255;
        destroyer();
#ifdef DEBUG
        Serial << "ispeed" << speed << endl;
#endif
        break;
      case 10:
        hercules.brake();
        roboclaw.BackwardM1(address, 0);
        roboclaw.BackwardM2(address, 0);
        analogWrite(CIMPWM, 0);
        myservo.write(180);
#ifdef DEBUG
        Serial << "PS3 Disconnected.." << endl;
#endif
        break;
    }
  }
}
