#include "QuadOmmniCytron.h"
#include"PlusbaseCytron.h"
#include<Ps3Controller.h>
#include<CytronDriver.h>
#include<analogWrite.h>
#include <MPU6050_tockn.h>
#include <Wire.h>

//Cytron Motor
#define Motorfl 0b00000001
#define Motorfr 0b00000000
#define Motorbr 0b00001001
#define Motorbl 0b00001000
#define speeeeed 80
#define slow_start_time 30

//Gripper UpDown
#define gripperA 27 
#define gripperB 26
#define gripperpwm 14
#define up_speed 180
#define down_speed 150

//Gripper
#define gripA 33
#define gripB 25
#define grippwm 32
#define tight_speed 100
#define loose_speed 100
MPU6050 mpu6050(Wire);
//#define sense 1

//Ball Gripper
#define g1 2 // Down
#define g2 4 // Up
#define g3 22
boolean flag2 = true;

CytronDriver CMotorfl(&Serial2, Motorfl);
CytronDriver CMotorfr(&Serial2, Motorfr);
CytronDriver CMotorbr(&Serial2, Motorbr);
CytronDriver CMotorbl(&Serial2, Motorbl);

Quadbase base(&Serial2, Motorfl, Motorfr, Motorbr, Motorbl);
byte speedf = speeeeed, speedb = speeeeed, speedl = speeeeed, speedr = speeeeed, i = 20, status, old_status;
unsigned long int previousMillis = 0;

//For MPU
float angle = 0, last_angle = 0;
byte changef = 0, changeb = 0, changel = 0, changer = 0;
boolean flag = true;

void notify()
{
  //--------------- Base Driving Events --------------
  if ( Ps3.event.button_down.up )
  {
    status = 1;
    Serial.println("Forward");
  }
  if ( Ps3.event.button_up.up )
  {
    status = 5;
    flag = false;
    Serial.println("Brake");
  }

  if ( Ps3.event.button_down.down )
  {
    status = 2;
    Serial.println("Down");
  }
  if ( Ps3.event.button_up.down )
  {
    status = 5;
    Serial.println("Brake");
  }
  if ( Ps3.event.button_down.left )
  {
    status = 3;
    Serial.println("Left");
  }
  if ( Ps3.event.button_up.left )
  {
    status = 5;
    Serial.println("Brake");
  }
  if ( Ps3.event.button_down.right )
  {
    status = 4;
    Serial.println("Right");
  }
  if ( Ps3.event.button_up.right )
  {
    status = 5;
    Serial.println("Brake");
  }

  if ( Ps3.event.button_down.l1)
  {
    status = 6;
    Serial.println("Anti-Clockwise");
  }
  if ( Ps3.event.button_up.l1 )
  {
    status = 5;
    Serial.println("Brake");
  }

  if ( Ps3.event.button_down.r1 )
  {
    status = 7;
    Serial.println("Clockwise");
  }
  if ( Ps3.event.button_up.r1 )
  {
    status = 5;
    Serial.println("Brake");
  }
  if ( Ps3.data.button.up && Ps3.data.button.left )
  {
    status = 8;
    Serial.println("Forward-Left");
  }
  if ( Ps3.data.button.up && Ps3.data.button.right)
  {
    status = 9;
    Serial.println("Forward-Right");
  }
  if ( Ps3.data.button.down && Ps3.data.button.right)
  {
    status = 10;
    Serial.println("Reverse-Right");
  }
  if ( Ps3.data.button.down && Ps3.data.button.left)
  {
    status = 11;
    Serial.println("Reverse-Left");
  }

  if ( Ps3.event.button_down.cross )
  {
    status = 14;
    Serial.println("Started pressing the cross button");
  }
  if ( Ps3.event.button_up.cross )
  {
    status = 5;
    Serial.println("Released the cross button");
  }
  if ( Ps3.event.button_down.square )
  {
    status = 13;
    Serial.println("Started pressing the square button");
  }
  if ( Ps3.event.button_up.square )
  {
    status = 5;
    Serial.println("Released the square button");
  }
  if ( Ps3.event.button_down.triangle )
  {
    status = 12;
    Serial.println("Started pressing the triangle button");
  }
  if ( Ps3.event.button_up.triangle )
  {
    status = 5;
    Serial.println("Released the triangle button");
  }
  if ( Ps3.event.button_down.circle )
  {
    status = 15;
    Serial.println("Started pressing the circle button");
  }
  if ( Ps3.event.button_up.circle )
  {
    status = 5;
    Serial.println("Released the circle button");
  }
  if ( Ps3.event.button_down.select)
  {
    if (flag2 == true)
      flag2 = false;
    else
      flag2 = true;
  }
}

void onConnect() {
  Serial.println("Connected.");
}

void _close() {
  digitalWrite(g2, HIGH);
}

void _open() {
  digitalWrite(g2, LOW);
}

void up() {
  digitalWrite(g1, HIGH);
  delay(1500);
  digitalWrite(g1, LOW);
}
void down() {
  digitalWrite(g3, HIGH);
  delay(1500);
  digitalWrite(g3, LOW);
}

void gripper_up() {
  digitalWrite(gripperA, LOW);
  digitalWrite(gripperB, HIGH);
  analogWrite(gripperpwm, up_speed);
}
void gripper_down() {
  digitalWrite(gripperA, HIGH);
  digitalWrite(gripperB, LOW);
  analogWrite(gripperpwm, down_speed);
}

void gripper_brake() {
  digitalWrite(gripperA, HIGH);
  digitalWrite(gripperB, HIGH);
  analogWrite(gripperpwm, 255);
}
void tight() {
  digitalWrite(gripA, LOW);
  digitalWrite(gripB, HIGH);
  analogWrite(grippwm, tight_speed);
}
void loose() {
  digitalWrite(gripA, HIGH);
  digitalWrite(gripB, LOW);
  analogWrite(grippwm, loose_speed);
}

void grip_brake() {
  digitalWrite(gripA, LOW);
  digitalWrite(gripB, LOW);
  analogWrite(grippwm, 0);
}

void slowStart() {
  if ((millis() - previousMillis) > slow_start_time) {
    i += 2;
    previousMillis = millis();
  }
  if (i == speeeeed)
    flag = false;

}

void forward() {
  if (flag == true && i <= speeeeed)
  {
    base.setSpeeed(i, i, i, i);
    base.forward();
    slowStart();
  }
  if (flag == false) {
    base.setSpeeed(speeeeed, speeeeed, speeeeed, speeeeed);
    base.forward();
  }
}

void reverse() {
  if (flag == true && i <= speeeeed)
  {
    base.setSpeeed(i, i, i, i);
    base.reverse();
    slowStart();
  }
  if (flag == false) {
    base.setSpeeed(speeeeed, speeeeed, speeeeed, speeeeed);
    base.reverse();
  }
}

void left() {
  if (flag == true && i <= speeeeed)
  {
    base.setSpeeed(i, i, i, i);
    base.left();
    slowStart();
    if (flag == false) {
      base.setSpeeed(speeeeed, speeeeed, speeeeed, speeeeed);
      base.left();
    }
  }
}

void right() {
  if (flag == true && i <= speeeeed)
  {
    base.setSpeeed(i, i, i, i);
    base.right();
    slowStart();
    if (flag == false) {
      base.setSpeeed(speeeeed, speeeeed, speeeeed, speeeeed);
      base.right();
    }
  }
}

void forwardleft() {
  if (flag == true && i <= speeeeed)
  {
    base.setSpeeed(i, i, i, i);
    base.forwardleft();
    slowStart();
    if (flag == false) {
      base.setSpeeed(speeeeed, speeeeed, speeeeed, speeeeed);
      base.forwardleft();
    }
  }
}

void forwardright() {
  if (flag == true && i <= speeeeed)
  {
    base.setSpeeed(i, i, i, i);
    base.forwardright();
    slowStart();
    if (flag == false) {
      base.setSpeeed(speeeeed, speeeeed, speeeeed, speeeeed);
      base.forwardright();
    }
  }
}

void reverseright() {
  if (flag == true && i <= speeeeed)
  {
    base.setSpeeed(i, i, i, i);
    base.reverseright();
    slowStart();
    if (flag == false) {
      base.setSpeeed(speeeeed, speeeeed, speeeeed, speeeeed);
      base.reverseright();
    }
  }
}

void reverseleft() {
  if (flag == true && i <= speeeeed)
  {
    base.setSpeeed(i, i, i, i);
    base.reverseleft();
    slowStart();
    if (flag == false) {
      base.setSpeeed(speeeeed, speeeeed, speeeeed, speeeeed);
      base.reverseleft();
    }
  }
}

void clockwise() {
  if (flag == true && i <= speeeeed)
  {
    base.setSpeeed(i, i, i, i);
    base.clockwise();
    slowStart();
    if (flag == false) {
      base.setSpeeed(speeeeed, speeeeed, speeeeed, speeeeed);
      base.clockwise();
    }
  }
}

void antiClockwise() {
  if (flag == true && i <= speeeeed)
  {
    base.setSpeeed(i, i, i, i);
    base.antiClockwise();
    slowStart();
    if (flag == false) {
      base.setSpeeed(speeeeed, speeeeed, speeeeed, speeeeed);
      base.antiClockwise();
    }
  }
}

// For Smooth Brake
void setSlowBrake() {
  if ((millis() - previousMillis) > 20) {
    i -= 10;
    previousMillis = millis();
  }
  base.setSpeeed(i, i, i, i);
}

void checkLim() {
  if (i <= 10) {
    base.brake();
    flag = true;
  }
}
void brake() {
  if (flag == false) {
    if (old_status == 1) {
      setSlowBrake();
      base.forward();
      checkLim();
    } else if (old_status == 2) {
      setSlowBrake();
      base.reverse();
      checkLim();
    } else if (old_status == 3) {
      setSlowBrake();
      base.left();
      checkLim();
    } else if (old_status == 4) {
      setSlowBrake();
      base.right();
      checkLim();
    } else if (old_status == 6) {
      setSlowBrake();
      base.antiClockwise();
      checkLim();
    } else if (old_status == 7) {
      setSlowBrake();
      base.clockwise();
      checkLim();
    } else if (old_status == 8) {
      setSlowBrake();
      base.forwardleft();
      checkLim();
    } else if (old_status == 9) {
      setSlowBrake();
      base.forwardright();
      checkLim();
    } else if (old_status == 10) {
      setSlowBrake();
      base.reverseright();
      checkLim();
    } else if (old_status == 11) {
      setSlowBrake();
      base.reverseleft();
      checkLim();
    }
  }
  if (flag == true) {
    base.brake();
    i = 20;
  }
  //  grip_brake();
  gripper_brake();
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial2.begin(115200);

  base.initiate();
  base.setSpeeed(speedf, speedb, speedl, speedr);

  Wire.begin();
  mpu6050.begin();
  mpu6050.calcGyroOffsets(true);

  pinMode(gripperA, OUTPUT);
  pinMode(gripperB, OUTPUT);
  pinMode(gripA, OUTPUT);
  pinMode(gripB, OUTPUT);
  pinMode(g1, OUTPUT);
  pinMode(g2, OUTPUT);
  pinMode(g3, OUTPUT);

  Ps3.attach(notify);
  Ps3.attachOnConnect(onConnect);
  Ps3.begin("11:11:11:11:11:11");

  Serial.println("Ready.");
}

void loop() {
  if (!Ps3.isConnected())
    return;

  mpu6050.update();
  angle = mpu6050.getAngleZ();
  Serial.print("\nAngle Z3  6: ");
  Serial.println(angle);

  if (status == 1) {
    if (flag2 == true)
    {
      old_status = 1;
      forward();
    }
    else
    {
      old_status = 2;
      reverse();
    }
  } else if (status == 2) {
    if (flag2 == true)
    {
      old_status = 2;
      reverse();
    }
    else
    {
      old_status = 1;
      forward();
    }
  } else if (status == 3) {
    if (flag2 == true)
    {
      old_status = 3;
      left();
    }
    else
    {
      old_status = 4;
      right();
    }
  } else if (status == 4) {
    if (flag2 == true)
    {
      old_status = 4;
      right();
    }
    else
    {
      old_status = 3;
      left();
    }
  } else if (status == 6) {
    old_status = 6;
    antiClockwise();
  } else if (status == 7) {
    old_status = 7;
    clockwise();
  } else if (status == 8) {
    if (flag2 == true)
    {
      old_status = 8;
      forwardleft();
    }
    else
    {
      old_status = 10;
      reverseright();
    }
  } else if (status == 9) {
    if (flag2 == true)
    {
      old_status = 9;
      forwardright();
    }
    else
    {
      old_status = 11;
      reverseleft();
    }
  } else if (status == 10) {
    if (flag2 == true)
    {
      old_status = 10;
      reverseright();
    }
    else
    {
      old_status = 8;
      forwardleft();
    }
  } else if (status == 11) {
    if (flag2 == true)
    {
      old_status = 11;
      reverseleft();
    }
    else
    {
      old_status = 9;
      forwardright();
    }
  } else if (status == 12) {
    if (flag2 == true)
      gripper_down();
    else
      up();
  } else if (status == 13) {
    if (flag2 == true)
      tight();
    else
      _close();
  } else if (status == 14) {
    if (flag2 == true)
      gripper_up();
    else
      down();
  } else if (status == 15) {
    if (flag2 == true)
      loose();
    else
      _open();
  } else {
    brake();
  }
}
