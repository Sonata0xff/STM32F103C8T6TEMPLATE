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

Processed_Data cache1;
Processed_Data cache2;
AtomVarType cache_lock;//SET means lock on, RESET means unlock
/*
Current remaining tasks:
Complete function implementations in the MPU.h file
Implement uint16_t to float conversion in the utils file
Finish quaternion algorithm implementation and splitting for parallel calculation
Design the buffer structure
*/

void MPU_Init()
{
	//reset lock
	Atom_Write(&cache_lock, ATOM_VALUE_RESET);
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
	
}


void MPU_Read_Accel()
{
	
}

void MPU_Read_Gyro()
{
	
}

void MPU_Get_Accel(float* res)
{
	
}

void MPU_Get_Gyro(float* res)
{
	
}

#endif