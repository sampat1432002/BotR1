#include "QuadBase.h"

// constructor for holonomic x-drive / Quad-base
QuadBase::QuadBase(Motor* motorA, Motor* motorB, Motor* motorC, Motor* motorD) {
  this->motorA = motorA; this->motorB = motorB; this->motorC = motorC; this->motorD = motorD;
}

// configuring baudrate for cytron-mdds30a
void QuadBase::config(long baudrate) {
  this->motorA->config(baudrate);
}

// directions for holonomic x-drive / Quad-base
void QuadBase::forward(byte speedA, byte speedB, byte speedC, byte speedD) {
  this->motorA->anti_clockwise(speedA);
  this->motorB->clockwise(speedB);
  this->motorC->clockwise(speedC);
  this->motorD->anti_clockwise(speedD);
}

void QuadBase::backward(byte speedA, byte speedB, byte speedC, byte speedD) {
  this->motorA->clockwise(speedA);
  this->motorB->anti_clockwise(speedB);
  this->motorC->anti_clockwise(speedC);
  this->motorD->clockwise(speedD);
}

void QuadBase::left(byte speedA, byte speedB, byte speedC, byte speedD) {
  this->motorA->clockwise(speedA);
  this->motorB->clockwise(speedB);
  this->motorC->anti_clockwise(speedC);
  this->motorD->anti_clockwise(speedD);
}

void QuadBase::right(byte speedA, byte speedB, byte speedC, byte speedD) {
  this->motorA->anti_clockwise(speedA);
  this->motorB->anti_clockwise(speedB);
  this->motorC->clockwise(speedC);
  this->motorD->clockwise(speedD);
}

void QuadBase::up_left(byte speedB, byte speedD) {
  this->motorA->brake();
  this->motorB->clockwise(speedB);
  this->motorC->brake();
  this->motorD->anti_clockwise(speedD);
}

void QuadBase::up_right(byte speedA, byte speedC) {
  this->motorA->anti_clockwise(speedA);
  this->motorB->brake();
  this->motorC->clockwise(speedC);
  this->motorD->brake();
}

void QuadBase::down_left(byte speedA, byte speedC) {
  this->motorA->clockwise(speedA);
  this->motorB->brake();
  this->motorC->anti_clockwise(speedC);
  this->motorD->brake();
}

void QuadBase::down_right(byte speedB, byte speedD) {
  this->motorA->brake();
  this->motorB->anti_clockwise(speedB);
  this->motorC->brake();
  this->motorD->clockwise(speedD);
}

void QuadBase::clockwise(byte speedA, byte speedB, byte speedC, byte speedD) {
  this->motorA->anti_clockwise(speedA);
  this->motorB->anti_clockwise(speedB);
  this->motorC->anti_clockwise(speedC);
  this->motorD->anti_clockwise(speedD);
}

void QuadBase::anti_clockwise(byte speedA, byte speedB, byte speedC, byte speedD) {
  this->motorA->clockwise(speedA);
  this->motorB->clockwise(speedB);
  this->motorC->clockwise(speedC);
  this->motorD->clockwise(speedD);
}

void QuadBase::brake() {
  this->motorA->brake();
  this->motorB->brake();
  this->motorC->brake();
  this->motorD->brake();
}
