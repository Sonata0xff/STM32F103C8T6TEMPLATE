#include "MPU.h"
#ifdef MPU_API_EN
//MPU addr: 0x68
#define SLAVE_ADDR 0x00d0

//self test GYRO
#define SELF_TEST_X_GYRO 0x00
#define SELF_TEST_Y_GYRO 0x01
#define SELF_TEST_Z_GYRO 0x02

//self test ACCEL
#define SELF_TEST_X_ACCEL 0x0D
#define SELF_TEST_Y_ACCEL 0x0E
#define SELF_TEST_Z_ACCEL 0x0F
//Gyro config
#define XG_OFFSET_H 0x13
#define XG_OFFSET_L 0x14
#define YG_OFFSET_H 0x15
#define YG_OFFSET_L  0x16
#define ZG_OFFSET_H 0x17
#define ZG_OFFSET_L 0x18
//accel coonfig
#define XA_OFFSET_H 0x77
#define XA_OFFSET_L 0x78
#define YA_OFFSET_H 0x7A
#define YA_OFFSET_L 0x7B
#define ZA_OFFSET_H 0x7D
#define ZA_OFFSET_L 0x7E

//freq config
#define SMPLRT_DIV 0x19
#define CONFIG 0x1A
#define GYRO_CONFIG 0x1B
#define ACCEL_CONFIG 0x1C
#define ACCEL_CONFIG_2 0x1D
#define LP_ACCEL_ODR 0x1E

//others
#define USER_CTRL 0x6A
#define PWR_MGMT_1 0x6B
#define PWR_MGMT_2 0x6C

//output data read only
//accel output
#define ACCEL_XOUT_H 0x3B
#define ACCEL_XOUT_L 0x3C
#define ACCEL_YOUT_H 0x3D
#define ACCEL_YOUT_L 0x3E
#define ACCEL_ZOUT_H 0x3F
#define ACCEL_ZOUT_L 0x40
//Guro output
#define GYRO_XOUT_H 0x43
#define GYRO_XOUT_L 0x44
#define GYRO_YOUT_H 0x45
#define GYRO_YOUT_L 0x46
#define GYRO_ZOUT_H 0x47
#define GYRO_ZOUT_L 0x48

#define GYROSCOPE_SENSITIVITY 16.4f
#define ACCELEROMETER_SENSITIVITY 4096.0f

Processed_Data cache1;
Data_Offset offset_cache1;
/*
Current remaining tasks:
1.Finish quaternion algorithm implementation and splitting for parallel calculation
2.MPU test case desgin and dev
3.data read opt
*/

void MPU_Init()
{
	//orders
	char orders[1];
	// turn MPU into sleep
	orders[0] = 0x40;
	IIC1WriteSlaveReg(orders, 1, SLAVE_ADDR, PWR_MGMT_1);
	IIC1_Send_Block_Wait();
	//config the reg
	orders[0] = 0x00;
	IIC1WriteSlaveReg(orders, 1, SLAVE_ADDR, SMPLRT_DIV); //div = 1, means 1Khz
	IIC1_Send_Block_Wait();
	//config config_reg
	orders[0] = 0x02;
	IIC1WriteSlaveReg(orders, 1, SLAVE_ADDR, CONFIG); //thr is 98/92Hz
	IIC1_Send_Block_Wait();
	//config gyro
	orders[0] = 0x18;
	IIC1WriteSlaveReg(orders, 1, SLAVE_ADDR, GYRO_CONFIG); //+-2000 dps
	IIC1_Send_Block_Wait();
	//config Accel
	orders[0] = 0x10;
	IIC1WriteSlaveReg(orders, 1, SLAVE_ADDR, ACCEL_CONFIG); //+-8g
	IIC1_Send_Block_Wait();
	orders[0] = 0x02;
	IIC1WriteSlaveReg(orders, 1, SLAVE_ADDR, ACCEL_CONFIG_2); //98/92Hz
	IIC1_Send_Block_Wait();
	//config low power freq
	orders[0] = 0x02;
	IIC1WriteSlaveReg(orders, 1, SLAVE_ADDR, LP_ACCEL_ODR); //0.98Hz
	IIC1_Send_Block_Wait();
}

void MPU_Start()
{
	//orders
	char orders[1];
	//open mpu
	orders[0] = 0x01;
	IIC1WriteSlaveReg(orders, 1, SLAVE_ADDR, PWR_MGMT_1);
	IIC1_Send_Block_Wait();
	//ret system
	orders[0] = 0x01;
	IIC1WriteSlaveReg(orders, 1, SLAVE_ADDR, USER_CTRL);
	IIC1_Send_Block_Wait();
}

void MPU_Stop()
{
	//orders
	char orders[1];
	//close mpu
	orders[0] = 0x40;
	IIC1WriteSlaveReg(orders, 1, SLAVE_ADDR, PWR_MGMT_1);
	IIC1_Send_Block_Wait();
}

void MPU_System_Calibration()
{
	//tmp data
	float tmpVal = 0;
	//data reset
	offset_cache1.accel_offset[0] = 0;
	offset_cache1.accel_offset[1] = 0;
	offset_cache1.accel_offset[2] = 0;
	offset_cache1.gyro_offset[0] = 0;
	offset_cache1.gyro_offset[1] = 0;
	offset_cache1.gyro_offset[2] = 0;
	//10 for average
	for (int i = 0; i < 1000; ++i) {
		//read Accel
		MPU_Read_Accel();
		for (char j = 0; j < 3; ++j) {
			TransI16_2_float(cache1.accel_Data[j], &tmpVal, ACCELEROMETER_SENSITIVITY);
			if (j == 2) tmpVal -= -1.0f;
			offset_cache1.accel_offset[j] += tmpVal;
		}
		//read Gyro
		MPU_Read_Gyro();
		for (char j = 0; j < 3; ++j) {
			TransI16_2_float(cache1.gyro_Data[j], &tmpVal, GYROSCOPE_SENSITIVITY);
			offset_cache1.gyro_offset[j] += tmpVal;
		}
	}
	for (char i = 0; i < 3; ++i) {
		offset_cache1.accel_offset[i] /= 1000.0f;
		offset_cache1.gyro_offset[i] /= 1000.0f;
	}
}


void MPU_Read_Accel()
{
	//datas
	char result;
	//read accelX
	IIC1ReadSlaveReg(&result, 1, SLAVE_ADDR, ACCEL_XOUT_H);
	IIC1_Send_Block_Wait();
	cache1.accel_Data[0] = (unsigned char)result;
	cache1.accel_Data[0] <<= 8;
	IIC1ReadSlaveReg(&result, 1, SLAVE_ADDR, ACCEL_XOUT_L);
	IIC1_Send_Block_Wait();
	cache1.accel_Data[0] |= (unsigned char)result;
	//read accelY
	IIC1ReadSlaveReg(&result, 1, SLAVE_ADDR, ACCEL_YOUT_H);
	IIC1_Send_Block_Wait();
	cache1.accel_Data[1] = (unsigned char)result;
	cache1.accel_Data[1] <<= 8;
	IIC1ReadSlaveReg(&result, 1, SLAVE_ADDR, ACCEL_YOUT_L);
	IIC1_Send_Block_Wait();
	cache1.accel_Data[1] |= (unsigned char)result;
	//read  accelZ
	IIC1ReadSlaveReg(&result, 1, SLAVE_ADDR, ACCEL_ZOUT_H);
	IIC1_Send_Block_Wait();
	cache1.accel_Data[2] = (unsigned char)result;
	cache1.accel_Data[2] <<= 8;
	IIC1ReadSlaveReg(&result, 1, SLAVE_ADDR, ACCEL_ZOUT_L);
	IIC1_Send_Block_Wait();
	cache1.accel_Data[2] |= (unsigned char)result;
}

void MPU_Read_Gyro()
{
	//datas
	char result;
	//read GyroX
	IIC1ReadSlaveReg(&result, 1, SLAVE_ADDR, GYRO_XOUT_H);
	IIC1_Send_Block_Wait();
	cache1.gyro_Data[0] = (unsigned char)result;
	cache1.gyro_Data[0] <<= 8;
	IIC1ReadSlaveReg(&result, 1, SLAVE_ADDR, GYRO_XOUT_L);
	IIC1_Send_Block_Wait();
	cache1.gyro_Data[0] |= (unsigned char)result;
	//read GyroY
	IIC1ReadSlaveReg(&result, 1, SLAVE_ADDR, GYRO_YOUT_H);
	IIC1_Send_Block_Wait();
	cache1.gyro_Data[1] = (unsigned char)result;
	cache1.gyro_Data[1] <<= 8;
	IIC1ReadSlaveReg(&result, 1, SLAVE_ADDR, GYRO_YOUT_L);
	IIC1_Send_Block_Wait();
	cache1.gyro_Data[1] |= (unsigned char)result;
	//read  GyroZ
	IIC1ReadSlaveReg(&result, 1, SLAVE_ADDR, GYRO_ZOUT_H);
	IIC1_Send_Block_Wait();
	cache1.gyro_Data[2] = (unsigned char)result;
	cache1.gyro_Data[2] <<= 8;
	IIC1ReadSlaveReg(&result, 1, SLAVE_ADDR, GYRO_ZOUT_L);
	IIC1_Send_Block_Wait();
	cache1.gyro_Data[2] |= (unsigned char)result;
}

void MPU_Get_Accel(float* res)
{
	for (char i = 0; i < 3; ++i) {
		TransI16_2_float(cache1.accel_Data[i], &res[i], ACCELEROMETER_SENSITIVITY);
		res[i] -= offset_cache1.accel_offset[i];
	}
}

void MPU_Get_Gyro(float* res)
{
	for (char i = 0; i < 3; ++i) {
		TransI16_2_float(cache1.gyro_Data[i], &res[i], GYROSCOPE_SENSITIVITY);
		res[i] -= offset_cache1.gyro_offset[i];
	}
}

#endif