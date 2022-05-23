VL6180X_ sensor;
VL6180X_Init(&sensor, &hi2c2);
configureDefault_VL6180X(&sensor);
uint16_t wynik = readRangeSingleMillimeters_VL6180X(&sensor);