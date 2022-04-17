#include "MotorsManager.h"

MotorsManager::MotorsManager(int x_motor_pins[4]) {
  _speedX = 0.0;

  // NOTE: The sequence 1-3-2-4 is required for proper sequencing of 28BYJ-48
  _stepperX = AccelStepper (HALFSTEP, x_motor_pins[0], x_motor_pins[2], x_motor_pins[1], x_motor_pins[3]);
}

void MotorsManager::setup() {
  _stepperX.setMaxSpeed(1000.0);
  _stepperX.setSpeed(_speedX);
}

void MotorsManager::moveRight(int step) {
  _autoStop = true;
  if (_speedX < 0.0) {
    // It was moving left, so stop it.
    _speedX = 0.0;
  } else {
    // Increase speed.
    _speedX += step;
  }

  Serial.print("New speed r: ");
  Serial.println(_speedX);
  
  // Update last time.
  _lastTimeX = millis();
}

void MotorsManager::moveLeft(int step) {
  _autoStop = true;
  if (_speedX > 0.0) {
    // It was moving left, so stop it.
    _speedX = 0.0;
  } else {
    // Decrease speed.
    _speedX -= step;
  }

  Serial.print("New speed l: ");
  Serial.println(_speedX);
  
  // Update last time.
  _lastTimeX = millis();
}

void MotorsManager::setSpeedX(int speed) {
  _autoStop = false;
  _speedX = speed;
  _stepperX.setSpeed(_speedX);
}

void MotorsManager::loop() {
  if (_autoStop && _speedX != 0.0) {
     unsigned int time = millis() - _lastTimeX;
    if (time > DEBOUNCE_MILLIS) {
      _speedX = 0.0;
    }
  }
  _stepperX.setSpeed(_speedX);
  _stepperX.runSpeed();
  
}