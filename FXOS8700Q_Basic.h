
#ifndef FXOS8700Q_BASIC_H
#define FXOS8700Q_BASIC_H
#include "I2C_device.h"

class FXOS8700QBasic: public I2CDevice
{
protected:
   float g_ = 9.8;
   float accel_sensitivity_ = 0.0023, magneto_sensitivity_ = 0.1;
   float hard_calib_matrix_[3][3];
   

public:
   //Constructor Functions
   FXOS8700QBasic() =default;
   FXOS8700QBasic(float g = 9.8, byte address = 0x1F,int port_no = 0, unsigned int frequency = 400000);
   
   //Functions to change output parameters
   void changeODR(unsigned int odr);
   void changeAccelOSR( unsigned int osr);
   void changeMagOSR(unsigned int osr);
   void changeAccelRange(unsigned int fsr);
   void enableOrDisableLowNoise(byte low_noise_en);
   void loadCalibrationData(byte eep_address);


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


