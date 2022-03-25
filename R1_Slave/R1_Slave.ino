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

template<class T> inline Print& operator<<(Print& serial, T args) {
  serial.print(args);
  return serial;
}

//Hercules
Motor hercules(INA, INB, PWM);
Servo myservo;
RoboClaw roboclaw(&Serial1, 10000);

byte speed = 90;
byte ball = 0;
byte dataFromESP;
float angle = 0, pos = 0;
boolean status_Hercules = false, status_CIM = false;

void servo_rotate() {
  ball++;
  if (90 * ball < 265)
    myservo.write(90 * ball);
  else
    myservo.write(255);
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

//void setEncoderM1(float angle) {
//  pos = map(angle, 41, 75, 0, -500);
//  Serial.println(pos);
//  if (pos > -500)
//    roboclaw.SpeedAccelDeccelPositionM1(address, 0, 65000, 0, pos, 1);
//}

void homing() { // Turret Moving to home position
  //  roboclaw.SetPinFunctions(address, 0, 4, 0);
  //  roboclaw.WriteNVM(address);
  roboclaw.ForwardM1(address, 64);
  delay(1500);
  roboclaw.SetEncM1(address, 0);
  //  roboclaw.SetPinFunctions(address, 0, 0, 0);
  //  roboclaw.WriteNVM(address);
  //  roboclaw.BackwardM1(address, 64);
  //  delay(200);
  //  roboclaw.BackwardM1(address, 0);
  //  delay(100);
  //  roboclaw.SetPinFunctions(address, 0, 2, 0);
  //  roboclaw.WriteNVM(address);
  //  delay(500);
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial3.begin(115200);
  roboclaw.begin(115200);
  homing();
  //  roboclaw.SpeedAccelDeccelPositionM1(address, 3000, 65000, 3000, -500, 1);
  pinMode(DIR, OUTPUT);
  pinMode(CIMPWM, OUTPUT);
  myservo.attach(6);
  myservo.write(0);
}

void loop() {
  // put your main code h
ere, to run repeatedly:
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
        roboclaw.BackwardM1(address, 127);
        //        angle = angle < 75 ? angle + 0.3 : 75;
        //        setEncoderM1(angle);
#ifdef DEBUG
        Serial << "Turret Up" << endl;
#endif
        break; // turret up
      case 5:
        roboclaw.ForwardM1(address, 127);
        // angle = angle > 40 ? angle - 0.3 : 41;
        // setEncoderM1(angle);
#ifdef DEBUG
        Serial << "Turret Down" << endl;
#endif
        break; // turret down
      case 6:
        roboclaw.BackwardM2(address, 127);
#ifdef DEBUG
        Serial << "Turret Rotation Movement Anti - Clockwise.." << endl;
#endif
        break;
      case 7:
        roboclaw.ForwardM2(address, 127);
#ifdef DEBUG
        Serial << "Turret Rotation Movement Clockwise.." << endl;
#endif
        break;
      case 8:
        roboclaw.BackwardM2(address, 0);
#ifdef DEBUG
        Serial << "Turret Rotation Movement brake.." << endl;
#endif
        break;
      case 9:
        roboclaw.BackwardM1(address, 0);
#ifdef DEBUG
        Serial << "Turret Angle Movement Lock.." << endl;
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
#ifdef DEBUG
        Serial << "PS3 Disconnected.." << endl;
#endif
        break;
    }
  }
}
