#include "MotorsManager.h"

MotorsManager::MotorsManager(int x_motor_pins[4]) {
  _speedX = 0.0;

  // NOTE: The sequence 1-3-2-4 is required for proper sequencing of 28BYJ-48
  _stepperX = AccelStepper (HALFSTEP, x_motor_pins[0], x_motor_pins[2], x_motor_pins[1], x_motor_pins[3]);
}

void MotorsManager::setup() {
  _stepperX.setMaxSpeed(MAX_SPEED);
  _stepperX.setSpeed(_speedX);
}

void MotorsManager::moveHorizontal(int step, bool autoStop) {
  _autoStop = autoStop;
  _stepX = step;
  
  _targetSpeedX += step;
  if (_targetSpeedX > MAX_SPEED) {
    _targetSpeedX = MAX_SPEED;
  } else if (_targetSpeedX < -MAX_SPEED) {
    _targetSpeedX = -MAX_SPEED;
  }
  
  // Update last time.
  _lastTimeX = millis();
}

void MotorsManager::setSpeedX(int speed) {
  _autoStop = false;
  _stepX = 100;
  _targetSpeedX = speed;
}

void MotorsManager::loop() {
  if (_autoStop && _speedX != 0.0) {
     unsigned int time = millis() - _lastTimeX;
    if (time > DEBOUNCE_MILLIS) {
      _targetSpeedX = 0.0;
    }
  }

  if (_speedX != _targetSpeedX) {
    if (_targetSpeedX == 0.0) {
      _speedX = 0.0;  
    } else {
      _speedX += _stepX;
      if ((_speedX < 0 && _speedX < _targetSpeedX) || (_speedX > 0 && _speedX > _targetSpeedX)) {
        _speedX = _targetSpeedX;
      }
    }

    Serial.print("Target: ");
    Serial.print(_targetSpeedX);
    Serial.print(" - X: ");
    Serial.println(_speedX);

   
  }
  
  _index += 1;
  if (_index > 8) {
     _stepperX.setSpeed(_speedX);
    _index = 0;
  }
  _stepperX.runSpeed();
}