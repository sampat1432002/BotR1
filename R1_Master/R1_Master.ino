#include <QuadOmmniCytron.h>
#include<Ps3Controller.h>
#include<CytronDriver.h>
#include<analogWrite.h>
#define DEBUG
#define endl "\n"

template<class T> inline Print& operator<<(Print& serial, T args) {
  serial.print(args);
  return serial;
}
//Cytron Motor
#define Motorfl 0b00000001
#define Motorfr 0b00001000
#define Motorbr 0b00000000
#define Motorbl 0b00001001

#define speeeeed 80
#define slow_start_time 30

#define TX1 27
#define RX1 32

CytronDriver CMotorfl(&Serial1, Motorfl);
CytronDriver CMotorfr(&Serial1, Motorfr);
CytronDriver CMotorbr(&Serial1, Motorbr);
CytronDriver CMotorbl(&Serial1, Motorbl);

Quadbase base(&Serial1, Motorfl, Motorfr, Motorbr, Motorbl);
byte speedf = speeeeed, speedb = speeeeed, speedl = speeeeed, speedr = speeeeed, i = 20, status, old_status;
unsigned long int previousMillis = 0;

//For MPU
float angle = 0, last_angle = 0;
byte changef = 0, changeb = 0, changel = 0, changer = 0;
boolean flag = true, flag2 = true, initiate = false;

void notify()
{
  //--------------- Base Driving Events --------------
  //Base-Left
  if ( Ps3.event.button_down.up && initiate) {
    status = 1;
#ifdef DEBUG
    Serial << "Forward" << endl;
#endif
  } if ( Ps3.event.button_up.up && initiate) {
    status = 5;
#ifdef DEBUG
    Serial << "Brake" << endl;
#endif
  } 

  //Base-Down
  if ( Ps3.event.button_down.down && initiate) {
    status = 2;
#ifdef DEBUG
    Serial << "Down" << endl;
#endif
  } if ( Ps3.event.button_up.down && initiate) {
    status = 5;
#ifdef DEBUG
    Serial << "Brake" << endl;
#endif
  } 
  
  //Base-Left
  if ( Ps3.event.button_down.left && initiate) {
    status = 3;
#ifdef DEBUG
    Serial << "Left" << endl;
#endif
  } if ( Ps3.event.button_up.left && initiate) {
    status = 5;
#ifdef DEBUG
    Serial << "Brake" << endl;
#endif
  } 

  //Base-Right
  if ( Ps3.event.button_down.right && initiate) {
    status = 4;
#ifdef DEBUG
    Serial << "Right" << endl;
#endif
  } if ( Ps3.event.button_up.right && initiate) {
    status = 5;
#ifdef DEBUG
    Serial << "brake." << endl;
#endif
  }

  //Turret Anti-Clockwise
  if ( Ps3.data.button.l1 && initiate) {
    //        status = 6;
    Serial2.write(6);
#ifdef DEBUG
    Serial << "Turret Anti-Clockwise.." << endl;
#endif
  }

  //Turret Clockwise
  if ( Ps3.data.button.r1 && initiate) {
    //    status = 7;
    Serial2.write(7);
#ifdef DEBUG
    Serial << "Turret Clockwise..." << endl;
#endif
  }

  //Forward-Left
  if ( Ps3.data.button.up && Ps3.data.button.left && initiate) {
    //    status = 8;
#ifdef DEBUG
    Serial << "Forward-Left" << endl;
#endif
  }

  //Forward-Right
  if ( Ps3.data.button.up && Ps3.data.button.right && initiate) {
    //    status = 9;
#ifdef DEBUG
    Serial << "Forward-Right" << endl;
#endif
  }

  //Reverse-Right
  if ( Ps3.data.button.down && Ps3.data.button.right && initiate) {
    //    status = 10;
#ifdef DEBUG
    Serial << "Reverse-Right" << endl;
#endif
  }

  //Reverse-Left
  if ( Ps3.data.button.down && Ps3.data.button.left && initiate) {
    //    status = 11;
#ifdef DEBUG
    Serial << "Reverse-Left" << endl;
#endif
  }

  //Hercules Control
  if ( Ps3.event.button_down.square && initiate) {
    //    status = 12;
#ifdef DEBUG
    Serial << "Hercules Control..." << endl;
#endif
    Serial2.write(2);
  }

  //CIM speed Increment
  if ( Ps3.event.button_down.l3 && initiate) {
    status = 5;
#ifdef DEBUG
    Serial << "CIM Incre..." << endl;
#endif
    Serial2.write(11);
  }

  //CIM speed decrement
  if ( Ps3.event.button_down.r3 && initiate) {
    status = 5;
#ifdef DEBUG
    Serial << "CIM Decre.." << endl;
#endif
    Serial2.write(12);
  }

  //Turret Up
  if ( Ps3.data.button.triangle && initiate) {
    status = 5;
#ifdef DEBUG
    Serial << "Turret Up..." << endl;
#endif
    Serial2.write(4);
  }

  //Starting CIM
  if ( Ps3.event.button_down.circle && initiate)  {
    status = 5;
#ifdef DEBUG
    Serial << "CIM Started..." << endl;
#endif
    Serial2.write(3);
  }

  // Turret Down with angle
  if ( Ps3.data.button.cross && initiate) {
    status = 5;
#ifdef DEBUG
    Serial << "Turret Down..." << endl;
#endif
    Serial2.write(5);
  }

  // For Using PS
  if ( Ps3.event.button_down.ps && !initiate) {
    initiate = true;
    status = 5;
#ifdef DEBUG
    Serial << "Initiate..." << endl;
#endif
  }

  //Loading Ball
  if (Ps3.event.button_down.select && initiate) {
    status = 5;
#ifdef DEBUG
    Serial << "Loading Ball..." << endl;
#endif
    Serial2.write(1);
  }
}

void onConnect()  {
  Serial.println("Connected.");
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
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial2.begin(115200);
  Serial1.begin(115200, SERIAL_8N1, RX1, TX1);
  delay(1000);

  base.initiate();
  base.setSpeeed(speedf, speedb, speedl, speedr);

  Ps3.attach(notify);
  Ps3.attachOnConnect(onConnect);
  Ps3.begin("11:11:11:11:11:11");

  Serial.println("Ready.");
}

void loop() {
  if (!Ps3.isConnected()) {
    Serial2.write(10);
    Serial << "In if:" << Ps3.isConnected() << endl;
    base.brake();
    return;
  }
  Serial << "Out of if:" << Ps3.isConnected() << endl;

  if (status == 1) {
    if (flag2 == true)
    {
      old_status = 1;
      forward();
    } else {
      old_status = 2;
      reverse();
    }
  } else if (status == 2) {
    if (flag2 == true) {
      old_status = 2;
      reverse();
    } else {
      old_status = 1;
      forward();
    }
  } else if (status == 3) {
    if (flag2 == true) {
      old_status = 3;
      left();
    } else {
      old_status = 4;
      right();
    }
  } else if (status == 4) {
    if (flag2 == true) {
      old_status = 4;
      right();
    } else {
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
    if (flag2 == true) {
      old_status = 8;
      forwardleft();
    } else {
      old_status = 10;
      reverseright();
    }
  } else if (status == 9) {
    if (flag2 == true) {
      old_status = 9;
      forwardright();
    } else {
      old_status = 11;
      reverseleft();
    }
  } else if (status == 10) {
    if (flag2 == true) {
      old_status = 10;
      reverseright();
    } else {
      old_status = 8;
      forwardleft();
    }
  } else if (status == 11) {
    if (flag2 == true) {
      old_status = 11;
      reverseleft();
    } else {
      old_status = 9;
      forwardright();
    }
  } else {
    brake();
  }
}
