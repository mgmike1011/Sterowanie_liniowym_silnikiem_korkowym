/*
 * VL6180X.c
 *
 *  Created on: Mar 2022
 *      Author: Miłosz Gajewski
 */
#include "VL6180X.h"
// Write
void writeReg(VL6180X_* VL6180x,uint16_t reg, uint8_t value){
	HAL_I2C_Mem_Write(VL6180x->i2cHandle, (VL6180x->address)<<1, reg, 1, &value, 1, 1000);
	VL6180x->last_status = busEndTransmission;
}
void writeReg16Bit(VL6180X_* VL6180x, uint16_t reg, uint16_t value){
	uint8_t partA = (uint8_t)((value & 0xFF00) >> 8);
	uint8_t partB = (uint8_t)(value & 0x00FF);
	uint8_t m[]= {partA,partB};
	HAL_I2C_Mem_Write(VL6180x->i2cHandle, (VL6180x->address)<<1, reg, 2, m, 2, 1000);
	VL6180x->last_status = busEndTransmission;
}
// Read
uint8_t readReg(VL6180X_* VL6180x, uint16_t reg){
	uint8_t value;
	HAL_I2C_Mem_Read(VL6180x->i2cHandle, (VL6180x->address)<<1, reg, 1, &value, 1, 1000);
	VL6180x->last_status = busEndTransmission;
	return value;
}
uint16_t readReg16Bit(VL6180X_* VL6180x, uint16_t reg){
	uint8_t value[2]={0};
	HAL_I2C_Mem_Read(VL6180x->i2cHandle, (VL6180x->address)<<1, reg, 2, value, 2, 1000);
	VL6180x->last_status = busEndTransmission;
	uint16_t value_ = ((uint16_t)value[1]<<8)|value[0];
	return value_;
}
void VL6180X_Init(VL6180X_* VL6180x, I2C_HandleTypeDef* i2cHandle){
	VL6180x->i2cHandle = i2cHandle;
	VL6180x->address = ADDRESS_DEFAULT;
	VL6180x->scaling = 0;
	VL6180x->ptp_offset = 0;
	VL6180x->io_timeout = 0;
	VL6180x->last_status = IDLE;
	//
	VL6180x->ptp_offset = readReg(VL6180x, SYSRANGE__PART_TO_PART_RANGE_OFFSET);
	if(readReg(VL6180x, SYSTEM__FRESH_OUT_OF_RESET) == 1){
		VL6180x->scaling = 1;
		writeReg(VL6180x,0x207, 0x01);
		    writeReg(VL6180x,0x208, 0x01);
		    writeReg(VL6180x,0x096, 0x00);
		    writeReg(VL6180x,0x097, 0xFD); // RANGE_SCALER = 253
		    writeReg(VL6180x,0x0E3, 0x01);
		    writeReg(VL6180x,0x0E4, 0x03);
		    writeReg(VL6180x,0x0E5, 0x02);
		    writeReg(VL6180x,0x0E6, 0x01);
		    writeReg(VL6180x,0x0E7, 0x03);
		    writeReg(VL6180x,0x0F5, 0x02);
		    writeReg(VL6180x,0x0D9, 0x05);
		    writeReg(VL6180x,0x0DB, 0xCE);
		    writeReg(VL6180x,0x0DC, 0x03);
		    writeReg(VL6180x,0x0DD, 0xF8);
		    writeReg(VL6180x,0x09F, 0x00);
		    writeReg(VL6180x,0x0A3, 0x3C);
		    writeReg(VL6180x,0x0B7, 0x00);
		    writeReg(VL6180x,0x0BB, 0x3C);
		    writeReg(VL6180x,0x0B2, 0x09);
		    writeReg(VL6180x,0x0CA, 0x09);
		    writeReg(VL6180x,0x198, 0x01);
		    writeReg(VL6180x,0x1B0, 0x17);
		    writeReg(VL6180x,0x1AD, 0x00);
		    writeReg(VL6180x,0x0FF, 0x05);
		    writeReg(VL6180x,0x100, 0x05);
		    writeReg(VL6180x,0x199, 0x05);
		    writeReg(VL6180x,0x1A6, 0x1B);
		    writeReg(VL6180x,0x1AC, 0x3E);
		    writeReg(VL6180x,0x1A7, 0x1F);
		    writeReg(VL6180x,0x030, 0x00);

		    writeReg(VL6180x,SYSTEM__FRESH_OUT_OF_RESET, 0);
	}else{
		uint16_t s = readReg16Bit(VL6180x, RANGE_SCALER);
		if(s == ScalerValues[3]){
			VL6180x->scaling = 3;
		}else if (s == ScalerValues[2]){
			VL6180x->scaling = 2;
		}else{
			VL6180x->scaling = 1;
		}
		VL6180x->ptp_offset *= VL6180x->scaling;
	}
}
void setI2C(VL6180X_* VL6180x, I2C_HandleTypeDef* i2cHandle){
	VL6180x->i2cHandle = i2cHandle;
}
I2C_HandleTypeDef* getBus(VL6180X_* VL6180x){
	return VL6180x->i2cHandle;
}
void setAddress(VL6180X_* VL6180x,uint8_t new_address){
	VL6180x->address = new_address;
}
uint8_t getAddress(VL6180X_* VL6180x){
	return VL6180x->address;
}
void setScaling(VL6180X_* VL6180x, uint8_t new_scaling){
	uint8_t const DefaultCrosstalkValidHeight = 20;
	if (new_scaling < 1 || new_scaling > 3) { return; }
	VL6180x->scaling = new_scaling;
	writeReg16Bit(VL6180x, RANGE_SCALER, ScalerValues[VL6180x->scaling]);
	writeReg(VL6180x, SYSRANGE__PART_TO_PART_RANGE_OFFSET, VL6180x->ptp_offset/VL6180x->scaling);
	writeReg(VL6180x, SYSRANGE__CROSSTALK_VALID_HEIGHT, DefaultCrosstalkValidHeight/VL6180x->scaling);
	uint8_t rce = readReg(VL6180x,SYSRANGE__RANGE_CHECK_ENABLES);
	writeReg(VL6180x,SYSRANGE__RANGE_CHECK_ENABLES, (rce & 0xFE) | (VL6180x->scaling == 1));
}
int16_t constrain(int16_t l1,int16_t l2, int16_t l3){
	if(l1 < l2){
		return l2;
	}else if(l1 > l3){
		return l3;
	}else{
		return l1;
	}
}
void startRangeContinuous(VL6180X_* VL6180x, uint16_t period)
{
  int16_t period_reg = (int16_t)(period / 10) - 1;
  period_reg = constrain(period_reg, 0, 254); 
  writeReg(VL6180x,SYSRANGE__INTERMEASUREMENT_PERIOD, period_reg);
  writeReg(VL6180x,SYSRANGE__START, 0x03);
}
void startAmbientContinuous(VL6180X_* VL6180x, uint16_t period)
{
  int16_t period_reg = (int16_t)(period / 10) - 1;
  period_reg = constrain(period_reg, 0, 254);

  writeReg(VL6180x,SYSALS__INTERMEASUREMENT_PERIOD, period_reg);
  writeReg(VL6180x,SYSALS__START, 0x03);
}
void startInterleavedContinuous(VL6180X_* VL6180x, uint16_t period)
{
  int16_t period_reg = (int16_t)(period / 10) - 1;
  period_reg = constrain(period_reg, 0, 254);

  writeReg(VL6180x,INTERLEAVED_MODE__ENABLE, 1);
  writeReg(VL6180x,SYSALS__INTERMEASUREMENT_PERIOD, period_reg);
  writeReg(VL6180x,SYSALS__START, 0x03);
}
void stopContinuous(VL6180X_* VL6180x)
{

  writeReg(VL6180x,SYSRANGE__START, 0x01);
  writeReg(VL6180x,SYSALS__START, 0x01);

  writeReg(VL6180x,INTERLEAVED_MODE__ENABLE, 0);
}
uint8_t readRangeContinuous(VL6180X_* VL6180x)
{
  uint16_t millis_start = HAL_GetTick();
  while ((readReg(VL6180x,RESULT__INTERRUPT_STATUS_GPIO) & 0x04) == 0)
  {
    if (VL6180x->io_timeout > 0 && ((uint16_t)HAL_GetTick() - millis_start) > VL6180x->io_timeout)
    {
      return 255;
    }
  }

  uint8_t range = readReg(VL6180x,RESULT__RANGE_VAL);
  writeReg(VL6180x,SYSTEM__INTERRUPT_CLEAR, 0x01);

  return range;
}
uint16_t readAmbientContinuous(VL6180X_* VL6180x)
{
  uint16_t millis_start = HAL_GetTick();
  while ((readReg(VL6180x,RESULT__INTERRUPT_STATUS_GPIO) & 0x20) == 0)
  {
    if (VL6180x->io_timeout > 0 && ((uint16_t)HAL_GetTick() - millis_start) > VL6180x->io_timeout)
    {
      return 0;
    }
  }

  uint16_t ambient = readReg16Bit(VL6180x,RESULT__ALS_VAL);
  writeReg(VL6180x,SYSTEM__INTERRUPT_CLEAR, 0x02);

  return ambient;
}
uint8_t readRangeSingle(VL6180X_* VL6180x)
{
  writeReg(VL6180x,SYSRANGE__START, 0x01);
  return readRangeContinuous(VL6180x);
}
uint16_t readAmbientSingle(VL6180X_* VL6180x)
{
  writeReg(VL6180x,SYSALS__START, 0x01);
  return readAmbientContinuous(VL6180x);
}
void configureDefault(VL6180X_* VL6180x){
	  writeReg(VL6180x,READOUT__AVERAGING_SAMPLE_PERIOD, 0x30);
	  writeReg(VL6180x,SYSALS__ANALOGUE_GAIN, 0x46);
	  writeReg(VL6180x,SYSRANGE__VHV_REPEAT_RATE, 0xFF);
	  writeReg16Bit(VL6180x,SYSALS__INTEGRATION_PERIOD, 0x0063);
	  writeReg(VL6180x,SYSRANGE__VHV_RECALIBRATE, 0x01);
	  writeReg(VL6180x,SYSRANGE__INTERMEASUREMENT_PERIOD, 0x09);
	  writeReg(VL6180x,SYSALS__INTERMEASUREMENT_PERIOD, 0x31);
	  writeReg(VL6180x,SYSTEM__INTERRUPT_CONFIG_GPIO, 0x24);
	  writeReg(VL6180x,SYSRANGE__MAX_CONVERGENCE_TIME, 0x31);
	  writeReg(VL6180x,INTERLEAVED_MODE__ENABLE, 0);
	  setScaling(VL6180x,1);
}
uint16_t readRangeContinuousMillimeters(VL6180X_* VL6180x){
	return (uint16_t)(VL6180x->scaling)*readRangeContinuous(VL6180x);
}
uint16_t readRangeSingleMillimeters(VL6180X_* VL6180x) {
	return (uint16_t)(VL6180x->scaling)* readRangeSingle(VL6180x);
}
