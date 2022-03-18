#include "FXOS8700Q_Basic.h"
#include "FXOS8700Q_Registers.h"
#include "I2C_device.h"
#include "Arduino.h"
#include "EEPROM.h"
#include <stdlib.h>


//Constructor
FXOS8700QBasic::FXOS8700QBasic(float g ,byte address ,unsigned int port_no = 0, unsigned int frequency = 400000)
{
    g_ = g;
    setupDevice(address,port_no,frequency);
    
    checkConnection();
    changeOperatingMode(HYBRID_MODE);
    enableOrDisableLowNoise(ENABLE);
    enableOrDisableAutoInc(ENABLE);
    changeAccelRange(4);
}


//Change device between active and Standby Mode
void FXOS8700QBasic::changePowerMode(bool mode)
{
    writeBitsToReg(CTRL_REG1,POWER_MODE_BITS,mode);
}

//Change operating mode between accel only, magneto only, hybrid
void FXOS8700QBasic::changeOperatingMode(byte mode)
{
    if ((mode >= 0) && (mode <= 3))
    {
        writeBitsToReg(M_CTRL_REG1,OPERATING_MODE_BITS,mode);
    }
    else 
    {
        Serial.println("Invalid  operating mode selected");
    }
}
//Wait till power mode changes
void FXOS8700QBasic::waitTill(bool mode)
{ 
    while ((bool) readPowerMode() != mode)
        {
        }

}

//Return the current power mode
bool FXOS8700QBasic::readPowerMode()
{
    return (bool) readBitsFromReg(CTRL_REG1,POWER_MODE_BITS);
}


//Change the device output data rate
void FXOS8700QBasic::changeODR(unsigned int odr,bool activate_sensor = 1)
{

     if (readPowerMode() != STANDBY)
    {
        changePowerMode(STANDBY);
        waitTill(STANDBY);
    }

    switch (odr)
    {
    case 800:
        writeBitsToReg(CTRL_REG1,ODR_BITS,0 << 3);
        break;
    case 400:
        writeBitsToReg(CTRL_REG1,ODR_BITS,1 << 3);
        break;
    case 200:
        writeBitsToReg(CTRL_REG1,ODR_BITS,2 << 3);
        break;
    case 100:
        writeBitsToReg(CTRL_REG1,ODR_BITS,3<< 3);
        break;
    case  50:
        writeBitsToReg(CTRL_REG1,ODR_BITS,4 << 3);
        break;
    case  12:
        writeBitsToReg(CTRL_REG1,ODR_BITS,5 << 3);
        break;
    case  6:
        writeBitsToReg(CTRL_REG1,ODR_BITS,6 << 3);
        break;
    case  1:
        writeBitsToReg(CTRL_REG1,ODR_BITS,7 << 3);
        break;
    
    default:
        Serial.println("Invalid odr!");
        break;
    }
 
    if (activate_sensor)
    {
        //Entering Active Mode
        changePowerMode(ACTIVE);
        
        waitTill(ACTIVE);
    }
    
    
}

void FXOS8700QBasic::changeAccelOSR(unsigned int osr,bool activate_sensor = 1)
{
    if( (osr>= 0) && (osr <= 4)) {

        if (readPowerMode() != STANDBY)
        {
            changePowerMode(STANDBY);
            waitTill(STANDBY);
        }
    
        writeBitsToReg(CTRL_REG2,ACCEL_OSR_BITS,osr);

        if (activate_sensor)
        {
            //Entering Active Mode
            changePowerMode(ACTIVE);
            
            waitTill(ACTIVE);
        }
    }
    else{

        Serial.println("Invalid osr selection");
    }
    

}

void FXOS8700QBasic::changeMagOSR(unsigned int osr,bool activate_sensor = 1){

    if( (osr>= 0) && (osr <= 7)) {

        if (readPowerMode() != STANDBY)
        {
            changePowerMode(STANDBY);
            waitTill(STANDBY);
        }
    
        writeBitsToReg(M_CTRL_REG1,MAGNETO_OSR_BITS,osr<<2);

        if (activate_sensor)
        {
            //Entering Active Mode
            changePowerMode(ACTIVE);
            
            waitTill(ACTIVE);
        }

    }
    else{

        Serial.println("Invalid osr selection");
    }

    
}
//hanges Accelerometer range
void FXOS8700QBasic::changeAccelRange( unsigned int fsr,bool activate_sensor = 1)
{
     if (readPowerMode() != STANDBY)
    {
        changePowerMode(STANDBY);
        waitTill(STANDBY);
    }

    switch (fsr)
    {
    case 2:
        writeBitsToReg(XYZ_DATA_CONFIG_REG,FSR_BITS,0);
        accel_sensitivity_ = (0.244)*(0.001)*g_;
        break;
    case 4:
        writeBitsToReg(XYZ_DATA_CONFIG_REG,FSR_BITS,1);
        accel_sensitivity_ = (0.244)*2*(0.001)*g_;
        break;
    case 8:
        
        writeBitsToReg(XYZ_DATA_CONFIG_REG,FSR_BITS,2);
        accel_sensitivity_ = (0.244)*4*(0.001)*g_;
        break;
    
    default:
        Serial.println("invalid odr value !");
        break;
    
   
    }

        if (activate_sensor)
        {
            //Entering Active Mode
            changePowerMode(ACTIVE);
            
            waitTill(ACTIVE);
        }

}
void FXOS8700QBasic::enableOrDisableLowNoise(bool low_noise_en,bool activate_sensor = 1)
{
    if (readPowerMode() != STANDBY)
    {
        changePowerMode(STANDBY);
        waitTill(STANDBY);
    }

    writeBitsToReg(CTRL_REG1,NOISE_MODE_BITS,( (byte) low_noise_en) << 2);

     if (activate_sensor)
       {
            //Entering Active Mode
            changePowerMode(ACTIVE);
                
             waitTill(ACTIVE);
       }
}
void FXOS8700QBasic::enableOrDisableAutoInc(bool enable,bool activate_sensor = 1){
    if (readPowerMode() != STANDBY)
    {
        changePowerMode(STANDBY);
        waitTill(STANDBY);
        writeBitsToReg(M_CTRL_REG2,HYBAUTOINC_MODE_BITS, ((byte) enable)<<5);
        if (activate_sensor)
        {
            //Entering Active Mode
            changePowerMode(ACTIVE);
            
            waitTill(ACTIVE);
        }
    }
}




void FXOS8700QBasic::loadCalibrationData(byte eep_address)
{
    byte temp[4*3*3];
    for (int i = 0; i < 4*3; i++)
    {
        temp[i] = EEPROM.read(eep_address + i);
    }
    memcpy(accel_offset_,temp,4*3);

    for (int i = 0; i < 4*3; i++)
    {
        temp[i] = EEPROM.read(eep_address + 12 + i);
    }
    memcpy(mag_offset_,temp,4*3);

    for (int i = 0; i < 4*3*3; i++)
    {
        temp[i] = EEPROM.read(eep_address + 24 +  i);
    }
    for (int i = 0; i < 3; i++)
    {
         memcpy(hard_calib_matrix_[i],temp + (4*3*i),4*3);
    }
    
   
    
    

}


void FXOS8700QBasic::updateAccelMagData(float* accel_data,float* mag_data)
{
    short int sensor_data[6] ;
    readShortIntsFromReg(OUT_X_MSB_REG,3,sensor_data);
    readShortIntsFromReg(M_OUT_X_MSB_REG,3,sensor_data + 3);

    for (int  i = 0; i < 3; i++)
    {
        accel_data[i]  = ((sensor_data[i] >>2) * accel_sensitivity_);
        accel_data[i] -= accel_offset_[i];
  
    }

    for (int i = 0; i < 3; i++)
    {
        mag_data[i]  = (( sensor_data[3] * hard_calib_matrix_[0][i] + sensor_data[4] * hard_calib_matrix_[1][i] + sensor_data[5] * hard_calib_matrix_[2][i])*magneto_sensitivity_); 
        mag_data[i] -= mag_offset_[i];    
    }
    
}

