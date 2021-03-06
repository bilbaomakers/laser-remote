// MotorsManager.h

#ifndef MotorsManager_h
#define MotorsManager_h

#include <stdlib.h>
#include <Arduino.h>
#include <AccelStepper.h>

#define HALFSTEP 8
#define DEBOUNCE_MILLIS 300
#define MAX_SPEED 1000.0

/////////////////////////////////////////////////////////////////////
/// \class MotorsManager MotorsManager.h <MotorsManager.h>
/// \brief Support for moving 2 28BYJ stepper motors.
/////////////////////////////////////////////////////////////////////

// #define h_motor_pin_1  2     // IN1 on ULN2003 ==> Blue   on 28BYJ-48
// #define h_motor_pin_2  4     // IN2 on ULN2004 ==> Pink   on 28BYJ-48
// #define h_motor_pin_3  18    // IN3 on ULN2003 ==> Yellow on 28BYJ-48
// #define h_motor_pin_4  19    // IN4 on ULN2003 ==> Orange on 28BYJ-48

class MotorsManager {
  public:

    /// Constructor.
    /// \param[in] x_motor_pins 4 Pins for X motor.
    /// \param[in] y_motor_pins 4 Pins for Y motor.
    MotorsManager(int x_motor_pins[4], int y_motor_pins[4]);
    
    /// Function to say motor X to move.
    void moveHorizontal(int step, bool autoStop);

    /// Function to set specific speed to motor X.
    void setSpeedX(int speed); 
    
    /// Function to say motor Y to move.
    void moveVertical(int step, bool autoStop);

    /// Function to set specific speed to motor Y.
    void setSpeedY(int speed); 

    /// Function to be called on setup
    void setup(); 

    /// Function to be called in every loop to update motors
    void loop(); 
  
  private:
    /// Horizontal motor.
    unsigned int _index = 0;

    AccelStepper _stepperX;
    bool _autoStopX = false;
    int _stepX = 0.0;
    double _targetSpeedX = 0.0;
    double _speedX = 0.0;
    unsigned int _lastTimeX = 0;
    
    AccelStepper _stepperY;
    bool _autoStopY = false;
    int _stepY = 0.0;
    double _targetSpeedY = 0.0;
    double _speedY = 0.0;
    unsigned int _lastTimeY = 0;

};

#endif 