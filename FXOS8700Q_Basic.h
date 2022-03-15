
#ifndef FXOS8700Q_BASIC_H
#define FXOS8700Q_BASIC_H
#include "I2C_device.h"

class FXOS8700QBasic: public I2CDevice
{
protected:
   float accel_sensitivity = 0.0023, magneto_sensitivity = 0.1,g;
   
   

public:
   //Constructor Functions
   FXOS8700QBasic() =default;
   FXOS8700QBasic(byte address,int port_no, float g);
   
   //Functions to change output parameters
   void changeODR(int odr);
   void changeAccelRange(int fsr);
   void enableOrDisableLowNoise(byte low_noise_en);
    
   void calibrateMag();
   void calibrateAccel();

   //functions to get output
   float getAccelX();
   float getAccelY();
   float getAccelZ();
   float getMagnetoX();
   float getMagnetoY();
   float getMagnetoZ();
protected:
   //Functions called internally before changing output parameters
   void changePowerMode(byte mode);
   void waitTill(byte mode);
   void changeOperatingMode(byte mode);
   byte readPowerMode();
};
#endif


