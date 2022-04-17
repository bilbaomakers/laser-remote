#include <Arduino.h>

#include "motor/MotorsManager.h"
#include "wifi/WifiManager.h"

#define SPEED_STEP 10

int X_MOTOR_PINS[4] = {13, 12, 14, 27};

MotorsManager motorsManager(X_MOTOR_PINS);
WifiManager wifiManager("LASER_REMOTE", "123456789");

void setup()
{
  Serial.begin(9600);
  delay(5000);

  wifiManager.setup([](int speedX, int speedY) {
    Serial.print("X: ");
    Serial.println(speedX);
    Serial.print("Y: ");
    Serial.println(speedY);
    motorsManager.setSpeedX(speedX);
  });
  motorsManager.setup();
}

void loop()
{
  if (Serial.available()) {
    byte ch = Serial.read();
    if (ch == '+') {
      motorsManager.moveRight(SPEED_STEP);
    } else if (ch == '-') {
      motorsManager.moveLeft(SPEED_STEP);
    }
  }

  wifiManager.loop();
  motorsManager.loop();
}