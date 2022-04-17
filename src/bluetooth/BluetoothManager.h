// BluetoothManager.h

#ifndef BluetoothManager_h
#define BluetoothManager_h

#include <stdlib.h>
#include <Arduino.h>

class BluetoothManager {
  public:

    BluetoothManager();

    /// Function to be called on setup
    void    setup(); 

    /// Function to be called in every loop to update motors
    void    loop(); 
  
  private:
  

};

#endif 