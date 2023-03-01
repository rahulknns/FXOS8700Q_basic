#include "FXOS8700Q_Basic.h"
#include "FXOS8700Q_Registers.h"
#include "I2C_device.h"
#include "Arduino.h"


//Constructor
FXOS8700QBasic::FXOS8700QBasic(float g ,byte address ,TwoWire* preferred_wire = &Wire)
{
    g_ = g;
    setupDevice(address,preferred_wire);
    
    checkConnection();
    changeOperatingMode(HYBRID_MODE);
    changeODR(DEFAULT_ODR);
    changeAccelRange(DEFAULT_OSR);
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




void FXOS8700QBasic::updateAccelMagData(float* accel_data,float* mag_data)
{
    short int sensor_data[6] ;
    readShortIntsFromReg(OUT_X_MSB_REG,3,sensor_data);
    readShortIntsFromReg(M_OUT_X_MSB_REG,3,sensor_data + 3);

    for (int  i = 0; i < 3; i++)
    {
        accel_data[i]  = ((sensor_data[i] >>2) * accel_sensitivity_);
  
    }


    
    
    for (int i = 0; i < 3; i++)
    {
       
        mag_data[i]  = (sensor_data[3 + i]*magneto_sensitivity_); 
        
    }
    
}

