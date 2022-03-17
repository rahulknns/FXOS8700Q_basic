
#ifndef FXOS8700Q_REGISTERS_H
#define FXOS8700Q_REGISTERS_H

//Values for different configuration
#define HYBRID_MODE         0b00000011
#define MAGNETO_ONLY        0b00000001
#define ACCEL_ONLY          0b00000000
#define WAKE                0b00000001
#define SLEEP               0b00000010
#define STANDBY             0b00000000
#define ACTIVE              0b00000001
#define NORMAL              0b00000000
#define LOW_NOISE_LOW_POWER 0b00000001
#define HIGH_RESOLUTION     0b00000010
#define LOW_POWER           0b00000011
#define ENABLE              1
#define DISABLE             0

//Register Addresses
#define SYSMOD_REG  0x0B 
#define OUT_X_MSB_REG 0x01
#define OUT_Y_MSB_REG 0x03
#define OUT_Z_MSB_REG 0x05
#define M_OUT_X_MSB_REG 0x33
#define M_OUT_Y_MSB_REG 0x35
#define M_OUT_Z_MSB_REG 0x37
#define M_CTRL_REG1  0x5B
#define M_CTRL_REG2  0x5C
#define CTRL_REG1    0x2A
#define CTRL_REG2    0x2B
#define XYZ_DATA_CONFIG_REG 0x0E
#define OFF_X_REG    0X2F
#define OFF_Y_REG    0x30
#define OFF_Z_REG    0x31

//Bitmasks

#define SYSMOD_BITS          0b00000011
#define OPERATING_MODE_BITS  0b00000011
#define HYBAUTOINC_MODE_BITS 0b00100000     
#define ODR_BITS             0b00111000 
#define ACCEL_OSR_BITS       0b00000011
#define MAGNETO_OSR_BITS     0b00011100
#define POWER_MODE_BITS      0b00000001
#define NOISE_MODE_BITS      0b00000100
#define FSR_BITS             0b00000011
#define AUTO_CAL_BIT         0b10000000
#endif