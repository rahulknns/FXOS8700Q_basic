#include "FXOS8700Q_Basic.h"
#include "FXOS8700Q_Registers.h"
#include "I2C_device.h"
#include "Arduino.h"


//Constructor
FXOS8700QBasic::FXOS8700QBasic(byte address, float g,int port_no = 0)
{
    setupDevice(address,port_no);
    this->g = g;
    checkConnection();
    changeOperatingMode(HYBRID_MODE);
    enableOrDisableLowNoise(LOW_NOISE_MODE_EN);
    changeAccelRange(4);
    changePowerMode(ACTIVE);
    waitTill(ACTIVE);
}


//Changes device between active and Standby Mode
void FXOS8700QBasic::changePowerMode(byte mode)
{
    writeBitsToReg(CTRL_REG1,POWER_MODE_BITS,mode);
}

//Changes operating mode between accel only, magneto only, hybrid
void FXOS8700QBasic::changeOperatingMode(byte mode)
{
    
   
    writeBitsToReg(M_CTRL_REG1,OPERATING_MODE_BITS,mode);

}

void FXOS8700QBasic::waitTill(byte mode)
{ 
    while (readPowerMode() != mode)
        {
        }

}

//Returns the current system mode
byte FXOS8700QBasic::readPowerMode()
{
    return readBitsFromReg(CTRL_REG1,POWER_MODE_BITS);
}


//Changes the device output data rate
void FXOS8700QBasic::changeODR(int odr)
{

     if (readPowerMode() != STANDBY)
    {
        changePowerMode(STANDBY);
        waitTill(STANDBY);
    }

    switch (odr)
    {
    case 800:
        writeBitsToReg(CTRL_REG1,ODR_BITS,0b00000000);
        break;
    case 400:
        writeBitsToReg(CTRL_REG1,ODR_BITS,0b00001000);
        break;
    case 200:
        writeBitsToReg(CTRL_REG1,ODR_BITS,0b00010000);
        break;
    case 100:
        writeBitsToReg(CTRL_REG1,ODR_BITS,0b00011000);
        break;
    case  50:
        writeBitsToReg(CTRL_REG1,ODR_BITS,0b00100000);
        break;
    case  12:
        writeBitsToReg(CTRL_REG1,ODR_BITS,0b00101000);
        break;
    case  6:
        writeBitsToReg(CTRL_REG1,ODR_BITS,0b00110000);
        break;
    case  1:
        writeBitsToReg(CTRL_REG1,ODR_BITS,0b00111000);
        break;
    
    default:
        break;
    }

    //Entering Active Mode
    changePowerMode(ACTIVE);
    
    waitTill(ACTIVE);
}

//hanges Accelerometer range
void FXOS8700QBasic::changeAccelRange(int fsr)
{
     if (readPowerMode() != STANDBY)
    {
        changePowerMode(STANDBY);
        waitTill(STANDBY);
    }

    switch (fsr)
    {
    case 2:
        writeBitsToReg(XYZ_DATA_CONFIG_REG,FSR_BITS,0b00000000);
        accel_sensitivity = (0.244)*(0.001)*g;
        break;
    case 4:
        writeBitsToReg(XYZ_DATA_CONFIG_REG,FSR_BITS,0b00000001);
        accel_sensitivity = (0.244)*2*(0.001)*g;
        Serial.println("Hello world fsr");
        Serial.println(readByteFromReg(XYZ_DATA_CONFIG_REG));
        break;
    case 8:
        
        writeBitsToReg(XYZ_DATA_CONFIG_REG,FSR_BITS,0b00000010);
        accel_sensitivity = (0.244)*4*(0.001)*g;
        break;
    
    default:
        break;
    
   
    }

     //Entering Active Mode
    changePowerMode(ACTIVE);
    waitTill(ACTIVE);

}
void FXOS8700QBasic::enableOrDisableLowNoise(byte low_noise_en)
{
    if (readPowerMode() != STANDBY)
    {
        changePowerMode(STANDBY);
        waitTill(STANDBY);
    }

    writeBitsToReg(CTRL_REG1,NOISE_MODE_BITS,low_noise_en);

    //Entering Active Mode
    changePowerMode(ACTIVE);
    waitTill(ACTIVE);
}





float FXOS8700QBasic::getAccelX()
{
    return (readWordFromReg(OUT_X_MSB_REG)>>2)*accel_sensitivity;
}

float FXOS8700QBasic::getAccelY()
{
    return (readWordFromReg(OUT_Y_MSB_REG)>>2)*accel_sensitivity;
}

float FXOS8700QBasic::getAccelZ()
{
    return (readWordFromReg(OUT_Z_MSB_REG)>>2)*accel_sensitivity;
}




float FXOS8700QBasic::getMagnetoX()
{
    return (readWordFromReg(M_OUT_X_MSB_REG))*magneto_sensitivity;
}

float FXOS8700QBasic::getMagnetoY()
{
    return (readWordFromReg(M_OUT_Y_MSB_REG))*magneto_sensitivity;
}

float FXOS8700QBasic::getMagnetoZ()
{
    return (readWordFromReg(M_OUT_Z_MSB_REG))*magneto_sensitivity;
}

