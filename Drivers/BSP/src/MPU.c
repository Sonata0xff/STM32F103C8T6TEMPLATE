#include "MPU.h"
#ifdef MPU_API_EN
//MPU addr: 0x68
#define SLAVE_ADDR 0x00d0

#define MAG_ADDR 0x003c


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
//accel config
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

//magnet config
#define MAGN_CONFIG_A 0x00
#define MAGN_CONFIG_B 0x01
#define MAGN_MODE 0x02
#define MAGN_STATUS 0x09
#define CalThreshold 0

//magnet output
#define MAGN_XOUT_H 0x03 
#define MAGN_XOUT_L 0x04
#define MAGN_YOUT_H 0x05
#define MAGN_YOUT_L 0x06
#define MAGN_ZOUT_H 0x07
#define MAGN_ZOUT_L 0x08


#define GYROSCOPE_SENSITIVITY 16.4f
#define ACCELEROMETER_SENSITIVITY 4096.0f
#define MAGNET_SENSITIVITY 1090.0f

Processed_Data cache1;
Data_Offset offset_cache1;
/*
Current remaining tasks:
1.Finish quaternion algorithm implementation and splitting for parallel calculation
2.MPU test case desgin and dev
*/

void MPU_Init()
{
	//orders
	char orders[1];
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
	// turn MPU into sleep
	orders[0] = 0x40;
	IIC1WriteSlaveReg(orders, 1, SLAVE_ADDR, PWR_MGMT_1);
	IIC1_Send_Block_Wait();
	
	//init magnet
	orders[0] = 0x78;
	IIC1WriteSlaveReg(orders, 1, MAG_ADDR, MAGN_CONFIG_A);
	IIC1_Send_Block_Wait();
	
	orders[0] = 0x20;
	IIC1WriteSlaveReg(orders, 1, MAG_ADDR, MAGN_CONFIG_B);
	IIC1_Send_Block_Wait();
	
	orders[0] = 0x02;
	IIC1WriteSlaveReg(orders, 1, MAG_ADDR, MAGN_MODE);
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
	
	//start magnet
	orders[0] = 0x00;
	IIC1WriteSlaveReg(orders, 1, MAG_ADDR, MAGN_MODE);
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
	
	//stop magnet
	orders[0] = 0x02;
	IIC1WriteSlaveReg(orders, 1, MAG_ADDR, MAGN_MODE);
	IIC1_Send_Block_Wait();
}

void MPU_System_Calibration()
{
	//tmp data
	float tmpVal = 0;
	float maxValue[3];
	float minValue[3];
	for (int i = 0; i < 3; ++i) {
		maxValue[i] = 0;
		minValue[i] = 0;
	}
	//data reset
	offset_cache1.accel_offset[0] = 0;
	offset_cache1.accel_offset[1] = 0;
	offset_cache1.accel_offset[2] = 0;
	offset_cache1.gyro_offset[0] = 0;
	offset_cache1.gyro_offset[1] = 0;
	offset_cache1.gyro_offset[2] = 0;
	offset_cache1.magnet_offset[0] = 0;
	offset_cache1.magnet_offset[1] = 0;
	offset_cache1.magnet_offset[2] = 0;
	//300 for average
	for (int i = 0; i < 300; ++i) {
		//read Accel
		MPU_Read_Accel();
		for (char j = 0; j < 3; ++j) {
			TransI16_2_float(cache1.accel_Data[j], &tmpVal, ACCELEROMETER_SENSITIVITY);
			if (j == 2) tmpVal -= 1.0f;
			offset_cache1.accel_offset[j] += tmpVal;
		}
		//read Gyro
		MPU_Read_Gyro();
		for (char j = 0; j < 3; ++j) {
			TransI16_2_float(cache1.gyro_Data[j], &tmpVal, GYROSCOPE_SENSITIVITY);
			offset_cache1.gyro_offset[j] += tmpVal;
		}
		//read Magnet
		MPU_Read_Magnet();
		for (char j = 0; j < 3; ++j) {
			TransI16_2_float(cache1.magnet_Data[j], &tmpVal, MAGNET_SENSITIVITY);
			maxValue[j] = tmpVal > maxValue[j] ? tmpVal : maxValue[j];
			minValue[j] = tmpVal < minValue[j] ? tmpVal : minValue[j];
		}
		HAL_Delay(20);
	}
	for (char i = 0; i < 3; ++i) {
		offset_cache1.accel_offset[i] /= 300.0f;
		offset_cache1.gyro_offset[i] /= 300.0f;
		offset_cache1.magnet_offset[i] = (maxValue[i] + minValue[i]) / 2;
	}
}


void MPU_Read_Accel()
{
	//datas
	char result[6];
	//read accel
	IIC1ReadSlaveReg(result, 6, SLAVE_ADDR, ACCEL_XOUT_H);
	IIC1_Send_Block_Wait();
	for (unsigned char i = 0; i < 6; i += 2) cache1.accel_Data[i / 2] = (int16_t)(uint16_t)( (((unsigned char)result[i]) << 8) | ((unsigned char)result[i + 1]) );
}

void MPU_Read_Gyro()
{
	//datas
	char result[6];
	//read Gyro
	IIC1ReadSlaveReg(result, 6, SLAVE_ADDR, GYRO_XOUT_H);
	IIC1_Send_Block_Wait();
	for (unsigned char i = 0; i < 6; i += 2) cache1.gyro_Data[i / 2] = ( (((unsigned char)result[i]) << 8) | ((unsigned char)result[i + 1]) );
}

void MPU_Read_Magnet()
{
	//datas
	char result[6];
	//read Magnet
	IIC1ReadSlaveReg(result, 6, MAG_ADDR, MAGN_XOUT_H);
	for (unsigned char i = 0; i < 6; i += 2) cache1.magnet_Data[i / 2] = ( (((unsigned char)result[i]) << 8) | ((unsigned char)result[i + 1]) );	
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

void MPU_Get_Magnet(float* res)
{
	for (char i = 0; i < 3; ++i) {
		TransI16_2_float(cache1.magnet_Data[i], &res[i], MAGNET_SENSITIVITY);
		res[i] -= offset_cache1.magnet_offset[i];
	}
	
}

#endif