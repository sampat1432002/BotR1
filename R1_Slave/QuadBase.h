/*
	@file-name	: QuadBase.h
	@Author	  	:	GTU-Robotics-Club
	@Version	  :	v1
	@Date		    :	17-03-2022
	> Holonomic X-drive / Quad-base should be wired to the motor-drivers in the given order.
					A   B
				 	/	  \
					\	  /
					D   C
*/


#ifndef QUADBASE_H
#define QUADBASE_H

#include <Arduino.h>
#include "Motor.h"

class QuadBase {
  protected:
    Motor *motorA, *motorB, *motorC, *motorD;
  public:
    // constructor for base library
    QuadBase(Motor* motorA, Motor* motorB, Motor* motorC, Motor* motorD);
    // configuring baudrate for cytron-mdds30a
    void config(long baudrate);
    // direction functions for holonomic x-drive
    void forward(byte speedA, byte speedB, byte speedC, byte speedD);
    void backward(byte speedA, byte speedB, byte speedC, byte speedD);
    void left(byte speedA, byte speedB, byte speedC, byte speedD);
    void right(byte speedA, byte speedB, byte speedC, byte speedD);
    void up_left(byte speedB, byte speedD);
    void up_right(byte speedA, byte speedC);
    void down_left(byte speedA, byte speedC);
    void down_right(byte speedB, byte speedD);
    void anti_clockwise(byte speedA, byte speedB, byte speedC, byte speedD);
    void clockwise(byte speedA, byte speedB, byte speedC, byte speedD);
    void brake();
};

#endif
