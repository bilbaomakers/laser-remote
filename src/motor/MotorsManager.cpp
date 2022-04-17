#include "MotorsManager.h"

MotorsManager::MotorsManager(int x_motor_pins[4], int y_motor_pins[4]) {
  _speedX = 0.0;
  _speedY = 0.0;

  // NOTE: The sequence 1-3-2-4 is required for proper sequencing of 28BYJ-48
  _stepperX = AccelStepper (HALFSTEP, x_motor_pins[0], x_motor_pins[2], x_motor_pins[1], x_motor_pins[3]);
  _stepperY = AccelStepper (HALFSTEP, y_motor_pins[0], y_motor_pins[2], y_motor_pins[1], y_motor_pins[3]);
}

void MotorsManager::setup() {
  _stepperX.setMaxSpeed(MAX_SPEED);
  _stepperX.setSpeed(_speedX);

  _stepperY.setMaxSpeed(MAX_SPEED);
  _stepperY.setSpeed(_speedY);
}

void MotorsManager::moveHorizontal(int step, bool autoStop) {
  _autoStopX = autoStop;
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
  _autoStopX = false;
  _stepX = 100;
  _targetSpeedX = speed;
}

void MotorsManager::moveVertical(int step, bool autoStop) {
  _autoStopY = autoStop;
  _stepY = step;
  
  _targetSpeedY += step;
  if (_targetSpeedY > MAX_SPEED) {
    _targetSpeedY = MAX_SPEED;
  } else if (_targetSpeedY < -MAX_SPEED) {
    _targetSpeedY = -MAX_SPEED;
  }
  
  // Update last time.
  _lastTimeY = millis();
}

void MotorsManager::setSpeedY(int speed) {
  _autoStopY = false;
  _stepY = 100;
  _targetSpeedY = speed;
}

void MotorsManager::loop() {
  if (_autoStopX && _targetSpeedX != 0.0) {
     unsigned int time = millis() - _lastTimeX;
    if (time > DEBOUNCE_MILLIS) {
      _targetSpeedX = 0.0;
      Serial.println("TIMEOUT X!!!");
    }
  }
  if (_autoStopY && _targetSpeedY != 0.0) {
     unsigned int time = millis() - _lastTimeY;
    if (time > DEBOUNCE_MILLIS) {
      _targetSpeedY = 0.0;
      Serial.println("TIMEOUT Y!!!");
    }
  }
  
  _index += 1;
  if (_index > 8) {
    _index = 0;
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
      _stepperX.setSpeed(_speedX);
    }
    if (_speedY != _targetSpeedY) {
      if (_targetSpeedY == 0.0) {
        _speedY = 0.0;  
      } else {
        _speedY += _stepY;
        if ((_speedY < 0 && _speedY < _targetSpeedY) || (_speedY > 0 && _speedY > _targetSpeedY)) {
          _speedY = _targetSpeedY;
        }
      }

      Serial.print("Target: ");
      Serial.print(_targetSpeedY);
      Serial.print(" - Y: ");
      Serial.println(_speedY);
      _stepperY.setSpeed(_speedY);
    }
  }
  
  _stepperX.runSpeed();
  _stepperY.runSpeed();
}