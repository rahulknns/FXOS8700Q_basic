#include "FXOS8700Q_Basic.h"
#include "FXOS8700Q_Registers.h"
#include "I2C_device.h"
#include "Arduino.h"


//Constructor
FXOS8700QBasic::FXOS8700QBasic(float g = 9.8,byte address = 0x21,unsigned int port_no = 0, unsigned int frequency = 400000)
{
    g_ = g;
    setupDevice(address,port_no,frequency);
    
    checkConnection();
    changeOperatingMode(HYBRID_MODE);
    enableOrDisableLowNoise(LOW_NOISE_MODE_EN);
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
void FXOS8700QBasic::changeODR(unsigned int odr)
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

    //Entering Active Mode
    changePowerMode(ACTIVE);
    
    waitTill(ACTIVE);
}

void FXOS8700QBasic::changeAccelOSR(unsigned int osr)
{
    if( (osr>= 0) && (osr <= 4) {

        if (readPowerMode() != STANDBY)
        {
            changePowerMode(STANDBY);
            waitTill(STANDBY);
        }
    
        writeBitsToReg(CTRL_REG2,ACCEL_OSR_BITS,osr);

        //Entering Active Mode
        changePowerMode(ACTIVE);
        waitTill(ACTIVE);

    }
    else{

        Serial.println("Invalid osr selection");
    }
    

}

void FXOS8700QBasic::changeMagOSR(unsigned int osr){

    if( (osr>= 0) && (osr <= 7) {

        if (readPowerMode() != STANDBY)
        {
            changePowerMode(STANDBY);
            waitTill(STANDBY);
        }
    
        writeBitsToReg(M_CTRL_REG1,MAGNETO_OSR_BITS,osr<<2);

        //Entering Active Mode
        changePowerMode(ACTIVE);
        waitTill(ACTIVE);

    }
    else{

        Serial.println("Invalid osr selection");
    }

    
}
//hanges Accelerometer range
void FXOS8700QBasic::changeAccelRange( unsigned int fsr)
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
        accel_sensitivity = (0.244)*(0.001)*g;
        break;
    case 4:
        writeBitsToReg(XYZ_DATA_CONFIG_REG,FSR_BITS,1);
        accel_sensitivity = (0.244)*2*(0.001)*g;
        break;
    case 8:
        
        writeBitsToReg(XYZ_DATA_CONFIG_REG,FSR_BITS,2);
        accel_sensitivity = (0.244)*4*(0.001)*g;
        break;
    
    default:
        Serial.println("invalid odr value !");
        break;
    
   
    }

     //Entering Active Mode
    changePowerMode(ACTIVE);
    waitTill(ACTIVE);

}
void FXOS8700QBasic::enableOrDisableLowNoise(bool low_noise_en)
{
    if (readPowerMode() != STANDBY)
    {
        changePowerMode(STANDBY);
        waitTill(STANDBY);
    }

    writeBitsToReg(CTRL_REG1,NOISE_MODE_BITS,( (byte) low_noise_en) << 2);

    //Entering Active Mode
    changePowerMode(ACTIVE);
    waitTill(ACTIVE);
}




void FXOS8700QBasic::updateAccelMagData(float* accel_data,float* mag_data)
{
    float sensor_data[6];
    readShortIntsFromReg(OUT_X_MSB_REG,6,sensor_data);

    for (int  i = 0; i < 3; i++)
    {
        accel_data[i]  = sensordata[i];
        accel_data[i] -= accel_calib_offset[i];
    }

    for (int i = 0; i < 3; i++)
    {
        mag_data[i]  = sensordata[3]*hard_calib_matrix_[0][i] + sensordata[4]*hard_calib_matrix_[1][i] + sensordata[5]*hard_calib_matrix_[2][i];
        mag_data[i] -= soft_calib_offset_[i];
        
    }
    
}

