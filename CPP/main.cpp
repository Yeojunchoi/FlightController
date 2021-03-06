/*
 * cppmain.h
 *
 *  Created on: 2020. 8. 2.
 *      Author: junwoo
 */

//essential
#include "main.h"
#include "usart.h"
#include "i2c.h"
#include "tim.h"
#include "FreeRTOS.h"
#include "cmsis_os.h"
#include "timers.h"

//additional
#include "cstdio"
#include "cstdlib"

//selfmade
#include "Driver/MPU9250.h"
#include "Driver/sbus.h"
#include "Driver/tm_stm32_gps.h"
#include "Driver/IST8310.h"
#include "Driver/bme280.h"
#include "Driver/Lidar1D.h"
//
#include "PeripheralInterface/SensorAccel.hpp"
#include "PeripheralInterface/SensorGyro.hpp"
#include "PeripheralInterface/SensorMag.hpp"
#include "PeripheralInterface/SensorGPS.hpp"
#include "PeripheralInterface/SensorBaro.hpp"
#include "PeripheralInterface/RC.hpp"

#include "Module/ModuleCommander.h"
#include <Module/ModuleAHRS.h>
#include <Module/ModuleBuzzer.h>
#include "Module/ModuleHealth.h"
#include "Module/ModuleSD.h"
#include "Module/ModuleAttitudeController.h"

#include "Actuator/Motor.h"

using namespace FC;

SensorMag sensorMag;
SensorAccel sensorAccel;
SensorGyro sensorGyro;
SensorBaro sensorBaro;
SensorGPS sensorGPS;
RC rc;

float attitude;
uint16_t hzAccel, hzBaro, hzGyro, hzGPS, hzMag, hzAHRS, hzRC, hzAtti;
uint16_t pwm1, pwm2, pwm3, pwm4, pwm5, pwm6;
float att_roll, att_pitch, att_yaw;
uint16_t ctl_roll, ctl_pitch, ctl_yaw, ctl_throtle;
float att_p, att_q, att_r;

uint32_t channel;

uint16_t timeCheck;
int test = 0;

//we can use printf
int _write(int file, unsigned char* p, int len) // for debug through uart3
{
	HAL_UART_Transmit(&huart2, p, len, 10);
	return len;
}


void Health_StartTask(void *argument){
	ModuleHealth::main();
}
void Debug_StartTask(void *argument){
	struct Health health;
	struct MotorPWM pwm;
	struct Attitude att;
	struct Controller ctl;
	struct BodyAngularVelocity ang;
	while(1){
		msgBus.getHealth(&health);
		hzAccel = health.accel;
		hzBaro = health.baro;
		hzGyro = health.gyro;
		hzGPS = health.gps;
		hzMag = health.mag;
		hzAHRS = health.ahrs;
		hzRC = health.rc;
		hzAtti = health.attitudeController;
		test++;
//		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_SET);

		msgBus.getMotorPWM(&pwm);
		pwm1 = pwm.m1;
		pwm2 = pwm.m2;
		pwm3 = pwm.m3;
		pwm4 = pwm.m4;
		pwm5 = pwm.m5;
		pwm6 = pwm.m6;

		msgBus.getAttitude(&att);
		att_roll = att.roll;
		att_pitch = att.pitch;
		att_yaw = att.yaw;

		msgBus.getController(&ctl);
		ctl_roll = ctl.roll;
		ctl_pitch = ctl.pitch;
		ctl_yaw = ctl.yaw;
		ctl_throtle = ctl.throttle;

		msgBus.getBodyAngularVelocity(&ang);
		att_p = ang.xyz[0];
		att_q = ang.xyz[1];
		att_r = ang.xyz[2];

		channel = m1.Channel;

		osDelay(5);
	}
}

void MPU9250_StartTask(void *argument){
	uint32_t tick;
	tick = osKernelGetTickCount();
	while(1){
		tick += 5;
		osDelayUntil(tick);
		MPU9250_updateDMA();
//		osDelay(5);				/* 200hz */
	}
}
void BME280_StartTask(void *argument){
	while(1){
		BME280_readIT();
		osDelay(20); 			/* 50hz */
	}
}
void IST8310_StartTask(void *argument){
	while(1){
		IST8310_updataIT();
		osDelay(10); 			/* 100hz */
	}
}
void SD_StartTask(void *argument){
	ModuleSD::main();
}

void AHRS_StartTask(void *argument){
	ModuleAHRS::main();
}

void Commander_StartTask(void *argument){
	ModuleCommander::main();
}

void Buzzer_StartTask(void *argument){
	ModuleBuzzer::main();
}

void AC_StartTask(void *argument){
	ModuleAttitudeController::main();
}


/*
 *  Switch
 *  Click : High
 *  GPIO_PinState result = HAL_GPIO_ReadPin(GPIOD, GPIO_PIN_7);
 */
/*
 *  LED signal
 *  HAL_GPIO_WritePin(GPIOF, GPIO_PIN_12, GPIO_PIN_RESET);
 */

void cppMain(){
    setvbuf(stdout, NULL, _IONBF, 0);
    printf("test\r\n");


    /* micro second timer start */
	HAL_TIM_Base_Start_IT(&htim2);

	/*
	 * \setting		i2c1
	 * 				fastmode
	 * 				using global interrupt
	 * 				using dma_rx
	 */
	MPU9250(&hi2c1);

	/*
	 * \setting		i2c2
	 * 				fastmode
	 * 				using global interrupt
	 *
	 * recommended mode : gaming
	 * Sensor mode : normal mode, standby = 0.5ms
	 * oversampling : pressureX4, temperatureX1, humidityX0
	 * IIR filter coefficient : 16
	 * RMS Noise : 0.3Pa/2.5cm
	 * Data output rate : 83hz
	 * Filter bandwidth : 1.75 Hz
	 * response time : 0.3s
	 */
	BME280_init(&hi2c2, P_OSR_04, H_OSR_00, T_OSR_01, normal, BW0_021ODR,t_00_5ms);

	/*
	 * \setting		i2c2
	 * 				fastmode
	 * 				using global interrupt
	 * 	magnetometer in gps module
	 */
	IST8310(&hi2c2);

	/*
	 *  \setting		uart8
	 *  				baudrate		38400
	 *  				rx_dma(circular)
	 */
    TM_GPS_Init(&huart8);

    /*
     *  \setting		uart7
     *  				baudrate			100000
     *  				word length			9bits
     *  				parity				even
     *  				stop_bits			2
     *  				rx_dma(circular)
     */
	SBUS_init(&huart7);

	Lidar1D_init(&htim15, TIM_CHANNEL_1, TIM_CHANNEL_2);
	Lidar1D_run();

	m1.start();
	m2.start();
	m3.start();
	m4.start();
	m5.start();
	m6.start();

    std::printf("boot complete\r\n");
}

//callback
void HAL_I2C_MemRxCpltCallback(I2C_HandleTypeDef *hi2c){
//#ifdef USE_MPU9250
	if(hi2c->Instance == mpu9250.hi2c->Instance){
		switch(MPU9250_i2cRxCpltCallback()){
		case 1:
			sensorAccel.setAccel(mpu9250.accel[0], mpu9250.accel[1], mpu9250.accel[2]);
			sensorGyro.setGyro(mpu9250.gyro[0], mpu9250.gyro[1], mpu9250.gyro[2]);
			break;
		case 2:
	//		sensorMag.setMag(mpu9250.mag[0], mpu9250.mag[1], mpu9250.mag[2]);
			break;
		}
	}

	if(hi2c->Instance == ist8310.hi2c->Instance){
		if(IST8310_i2cRxCpltCallback())
			sensorMag.setMag(ist8310.raw[0], ist8310.raw[1], ist8310.raw[2]);
	}

	if(hi2c->Instance == bme280.hi2c->Instance){
		if(BME280_i2cRxCpltCallback())
			sensorBaro.setBaro(bme280.P, bme280.T);
	}
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	if(huart->Instance == sbus.huart->Instance){
		if(SBUS_uartRxCpltCallback() == SBUS_Result_NewData){
			rc.setRC(SBUS_getChannel(2),	/* roll */
					 SBUS_getChannel(3),	/* pitch */
					 SBUS_getChannel(4), 	/* yaw */
					 SBUS_getChannel(1),	/* throttle */
					 SBUS_getChannel(11));
		}
	}

	if(TM_GPS_Update(huart) == TM_GPS_Result_NewData && gpsUart.gpsData.Fix != 0 /* gps must fixed */){
		sensorGPS.setGPS(gpsUart.gpsData.Latitude, gpsUart.gpsData.Longitude, gpsUart.gpsData.Altitude,
						 TM_GPS_ConvertSpeed(gpsUart.gpsData.Speed, TM_GPS_Speed_MeterPerSecond), gpsUart.gpsData.Direction, gpsUart.gpsData.HDOP, gpsUart.gpsData.VDOP,
						 gpsUart.gpsData.Satellites, gpsUart.gpsData.FixMode, 0/* UTC in microsecond */);
	}
//	if(huart->Instance == UART8){
//		if(TM_GPS_Update() == TM_GPS_Result_NewData && gpsUart.gpsData.Fix != 0 /* gps must fixed */){
//			sensorGPS.setGPS(gpsUart.gpsData.Latitude, gpsUart.gpsData.Longitude, gpsUart.gpsData.Altitude,
//							 TM_GPS_ConvertSpeed(gpsUart.gpsData.Speed, TM_GPS_Speed_MeterPerSecond), gpsUart.gpsData.Direction, gpsUart.gpsData.HDOP, gpsUart.gpsData.VDOP,
//							 gpsUart.gpsData.Satellites, gpsUart.gpsData.FixMode, 0/* UTC in microsecond */);
//		}
//	}
}

void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim){
	Lidar1D_CaptureCallback(htim);
}
