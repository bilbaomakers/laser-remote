#include <Arduino.h>

#include "motor/MotorsManager.h"
#include "wifi/WifiManager.h"

#define SPEED_STEP 10

int X_MOTOR_PINS[4] = {13, 12, 14, 27};
int Y_MOTOR_PINS[4] = {26, 25, 33, 32};

MotorsManager motorsManager(X_MOTOR_PINS, Y_MOTOR_PINS);
WifiManager wifiManager("LASER_REMOTE", "123456789");

void setup()
{
  Serial.begin(9600);
  delay(5000);

  wifiManager.setup([](char* speedX, char* speedY) {
    if (strcmp(speedX, "+") == 0) {
      Serial.println("RIGHT");
      motorsManager.moveHorizontal(SPEED_STEP, false);
    } else if (strcmp(speedX, "-") == 0) {
      Serial.println("LEFT");
      motorsManager.moveHorizontal(-SPEED_STEP, false);
    } else {
      motorsManager.setSpeedX(atoi(speedX) * 10);
    }

    if (strcmp(speedY, "+") == 0) {
      Serial.println("UP");
      motorsManager.moveVertical(SPEED_STEP, false);
    } else if (strcmp(speedY, "-") == 0) {
      Serial.println("DOWN");
      motorsManager.moveVertical(-SPEED_STEP, false);
    } else {
      motorsManager.setSpeedY(atoi(speedY) * 10);
    }
  });
  motorsManager.setup();
}

unsigned int lastTime = 0;

void loop()
{
  if (Serial.available()) {
    byte ch = Serial.read();
    unsigned int time = millis() - lastTime;
    Serial.print("MILLIS: ");
    Serial.println(time);
    lastTime = millis();
    if (ch == '+') {
      motorsManager.moveHorizontal(SPEED_STEP, true);
    } else if (ch == '-') {
      motorsManager.moveHorizontal(-SPEED_STEP, true);
    }
  }

  wifiManager.loop();
  motorsManager.loop();
}