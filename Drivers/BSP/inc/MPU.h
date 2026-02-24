#include "bsp_config.h"

#ifdef MPU_API_EN
#ifndef MPU_H
#include "stm32f1xx.h"                  // Device header
#include "stm32f1xx_hal.h"
#include "IIC.h"
#include "GPIO_EXIT.h"
#include "AtomVariable.h"
#include "utils.h"

typedef struct {
	uint16_t accel_Data[3];//Accel value [x, y, z]
	uint16_t gyro_Data[3];//Gyro value [x, y, z]
	//Quaternion value
} Processed_Data;
typedef struct {
	float accel_offset[3];//Accel offset [x, y, z]
	float gyro_offset[3];//Gyro offsset [x, y, z]
} Data_Offset;

void MPU_Init();

void MPU_Start();

void MPU_Stop();

void MPU_System_Calibration();

void MPU_Read_Accel();//load Accel result into buff space

void MPU_Read_Gyro();//load Gyro result into buff space

void MPU_Get_Accel(float* res);//get Accel result

void MPU_Get_Gyro(float* res);//get Gyro result
#endif
#endif