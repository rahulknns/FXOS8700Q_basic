
#ifndef FXOS8700Q_BASIC_H
#define FXOS8700Q_BASIC_H
#include "I2C_device.h"

#define DEFAULT_ODR 100
#define DEFAULT_FSR 4
class FXOS8700QBasic: public I2CDevice
{
protected:
   float g_ = 9.8;
   float accel_sensitivity_ = 0.0023, magneto_sensitivity_ = 0.1;
   

public:
   //Constructor Functions
   FXOS8700QBasic() =default;
   FXOS8700QBasic(float g = 9.8 , byte address,TwoWire* preferred_wire);
   
   //Functions to change output parameters
   void changeODR(unsigned int odr,bool activate_sensor = 1);
   void changeAccelRange(unsigned int fsr,bool activate_sensor = 1);


   //functions to get output
   void updateAccelMagData(float* accel_data, float* mag_data);
   

protected:
   //Functions called internally before changing output parameters
   void changePowerMode(bool mode);
   void waitTill(bool mode);
   void changeOperatingMode(byte mode);
   bool readPowerMode();
};
#endif


