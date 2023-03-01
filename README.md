# FXOS8700Q_basic 
* provides  basic functionality to access NXP FXOS8700Q accel + magneto sensor through cpp API.

# Usage :
* create an object of FXOS with address and optionally TwoWire object. Default value is Wire.
* Change ODR, FSR accordingly using the respective functions if needed. Default value is defined in Header file.
* Pass float pointers to updateData function as input to get real time values read from the sensor. Call them everytime you need updated value of the sensor.
