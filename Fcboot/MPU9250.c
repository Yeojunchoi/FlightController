/*
 * MPU9250.c
 *
 *  Created on: Jul 14, 2020
 *      Author: cjb88
 */


#include "MPU9250.h"
#include "stdlib.h" // malloc, free

const uint16_t MPU9250_gyrosensitivity  = 131;   // = 131 LSB/degrees/sec
const uint16_t MPU9250_accelsensitivity = 16384;  // = 16384 LSB/g
const float MPU9250_DEG2RAD = 0.017453292519f;

const uint16_t DEFAULT_TIMEOUT = 500;
//const int16_t attachAngle[3] = {-1, 1, -1};


uint16_t MPU9250_hzcnt = 0;

// mpu global instance
struct MPU9250 iMPU9250 = {0, };


uint16_t MPU9250_getHz(){ // call this func 1hz
	uint16_t temp = MPU9250_hzcnt;
	MPU9250_hzcnt = 0;
	return temp;
}

void MPU9250_init(struct MPU9250* obj, I2C_HandleTypeDef *phi2c){
//	obj->gyroBias[3] = {0, 0, 0}
//	obj->accelBias[3] = {0, 0, 0}; // Bias corrections for gyro and accelerometer

	obj->phi2c = phi2c;

	obj->Ascale = AFS_2G;
	obj->Gscale = GFS_250DPS;
	obj->Mscale = MFS_14BITS;
	obj->Mmode = 0x06;

	obj->initQ[0] = 1;//initial value
	obj->initQ[1] = 0;
	obj->initQ[2] = 0;
	obj->initQ[3] = 0;

	obj->status = MPU9250_disable;

	MPU9250_resetMPU9250(obj);
//	MPU9250_calibrateMPU9250(obj);

	MPU9250_initMPU9250(obj); // calculate ay, ac bias
	MPU9250_initAK8963(obj);

	MPU9250_getMres(obj);
	MPU9250_getGres(obj);
	MPU9250_getAres(obj);
}

uint16_t MPU9250_writeByte(struct MPU9250* obj, uint8_t address, uint8_t subAddress, uint8_t data)
{
	// Creating dynamic array to store regAddr + data in one buffer
	#define SIZE 1
	uint8_t * dynBuffer;
	dynBuffer = (uint8_t *) malloc(sizeof(uint8_t) * (SIZE+1));
	dynBuffer[0] = subAddress;

	// copy array
	memcpy(dynBuffer+1, &data, sizeof(uint8_t) * SIZE);

	HAL_StatusTypeDef status = HAL_I2C_Master_Transmit(obj->phi2c, address, dynBuffer, SIZE+1, DEFAULT_TIMEOUT);
	free(dynBuffer);
	return status == HAL_OK;
}



void MPU9250_readBytes(struct MPU9250* obj, uint8_t address, uint8_t subAddress, uint8_t count, uint8_t * dest)
{
	char data[14];
	char data_write[1];
	data_write[0] = subAddress;

	//    uint16_t tout = timeout > 0 ? timeout : DEFAULT_TIMEOUT;
	HAL_I2C_Master_Transmit(obj->phi2c, address, (uint8_t*)data_write, 1, DEFAULT_TIMEOUT);
	HAL_I2C_Master_Receive(obj->phi2c, address, (uint8_t*)data, count, DEFAULT_TIMEOUT);
	for(int ii = 0; ii < count; ii++) {
		dest[ii] = data[ii];
	}
}

char MPU9250_readByte(struct MPU9250* obj, uint8_t address, uint8_t subAddress)
{
	char data[1]; // `data` will store the register data
	MPU9250_readBytes(obj, address, subAddress, 1, (uint8_t*)data);
	return data[1];
}

void MPU9250_readMPU9250IT(struct MPU9250* obj){
	HAL_I2C_Mem_Read_IT(obj->phi2c, MPU9250_ADDRESS, ACCEL_XOUT_H, 1, obj->agtBuffer, 14);
}

uint8_t MPU9250_callbackMPU9250IT(struct MPU9250* obj){
	obj->accelCount[0] = (((int16_t)obj->agtBuffer[0]) << 8) | obj->agtBuffer[1];
	obj->accelCount[1] = (((int16_t)obj->agtBuffer[2]) << 8) | obj->agtBuffer[3];
	obj->accelCount[2] = (((int16_t)obj->agtBuffer[4]) << 8) | obj->agtBuffer[5];
	obj->tmpCount = (int16_t) (obj->agtBuffer[6] << 8 | obj->agtBuffer[7]);
	obj->gyroCount[0] = (((int16_t)obj->agtBuffer[8]) << 8) | obj->agtBuffer[9];
	obj->gyroCount[1] = (((int16_t)obj->agtBuffer[10]) << 8) | obj->agtBuffer[11];
	obj->gyroCount[2] = (((int16_t)obj->agtBuffer[12]) << 8) | obj->agtBuffer[13];

	if((!(obj->accelCount[0])&!(obj->accelCount[1])&!(obj->accelCount[2]))
	   |(!(obj->gyroCount[0])&!(obj->gyroCount[1])&!(obj->gyroCount[2]))){
		return 0;
	}
	//
//	obj->ax = rawAx-baseAcX;
//	Ay = rawAy-baseAcY;
//	Az = rawAz-(15384-baseAcZ);
//	Gx = (rawGx-baseGyX) / 131.0;
//	Gy = (rawGy-baseGyY) / 131.0;
//	Gz = (rawGz-baseGyZ) / 131.0;

	// Now we'll calculate the accleration value into actual g's
	obj->ax = ((float)obj->accelCount[0]*obj->aRes - obj->accelBias[0]);  // get actual g value, this depends on scale being set
	obj->ay = ((float)obj->accelCount[1]*obj->aRes - obj->accelBias[1]);
	obj->az = ((float)obj->accelCount[2]*obj->aRes - obj->accelBias[2]);

	obj->tmp = (float) ((int16_t) obj->tmpCount / (float) 340.0 + (float) 36.53);

	// Calculate the gyro value into actual degrees per second
	obj->gx = MPU9250_DEG2RAD*((float)obj->gyroCount[0]*obj->gRes - obj->gyroBias[0]);  // get actual gyro value, this depends on scale being set
	obj->gy = MPU9250_DEG2RAD*((float)obj->gyroCount[1]*obj->gRes - obj->gyroBias[1]);
	obj->gz = MPU9250_DEG2RAD*((float)obj->gyroCount[2]*obj->gRes - obj->gyroBias[2]);

	MPU9250_hzcnt++;
	return 1;
}

void MPU9250_getMres(struct MPU9250* obj) {
	switch (obj->Mscale)
	{
	// Possible magnetometer scales (and their register bit settings) are:
	// 14 bit resolution (0) and 16 bit resolution (1)
	case MFS_14BITS:
		  obj->mRes = 10.0*4912.0/8190.0; // Proper scale to return milliGauss
		  break;
	case MFS_16BITS:
		  obj->mRes = 10.0*4912.0/32760.0; // Proper scale to return milliGauss
		  break;
	}
}

void MPU9250_getGres(struct MPU9250* obj) {
  switch (obj->Gscale)
  {
    // Possible gyro scales (and their register bit settings) are:
    // 250 DPS (00), 500 DPS (01), 1000 DPS (10), and 2000 DPS  (11).
        // Here's a bit of an algorith to calculate DPS/(ADC tick) based on that 2-bit value:
    case GFS_250DPS:
    	  obj->gRes = 250.0/32768.0;
          break;
    case GFS_500DPS:
    	  obj->gRes = 500.0/32768.0;
          break;
    case GFS_1000DPS:
    	  obj->gRes = 1000.0/32768.0;
          break;
    case GFS_2000DPS:
    	  obj->gRes = 2000.0/32768.0;
          break;
  }
}


void MPU9250_getAres(struct MPU9250* obj) {
  switch (obj->Ascale)
  {
    // Possible accelerometer scales (and their register bit settings) are:
    // 2 Gs (00), 4 Gs (01), 8 Gs (10), and 16 Gs  (11).
        // Here's a bit of an algorith to calculate DPS/(ADC tick) based on that 2-bit value:
    case AFS_2G:
    	  obj->aRes = 2.0/32768.0;
          break;
    case AFS_4G:
    	  obj->aRes = 4.0/32768.0;
          break;
    case AFS_8G:
    	  obj->aRes = 8.0/32768.0;
          break;
    case AFS_16G:
    	  obj->aRes = 16.0/32768.0;
          break;
  }
}


void MPU9250_readAccelData(struct MPU9250* obj)
{
    uint8_t rawData[6];  // x/y/z accel register data stored here
    MPU9250_readBytes(obj, MPU9250_ADDRESS, ACCEL_XOUT_H, 6, &rawData[0]);  // Read the six raw data registers into data array
    obj->accelCount[0] = (int16_t)(((int16_t)rawData[0] << 8) | rawData[1]) ;  // Turn the MSB and LSB into a signed 16-bit value
    obj->accelCount[1] = (int16_t)(((int16_t)rawData[2] << 8) | rawData[3]) ;
    obj->accelCount[2] = (int16_t)(((int16_t)rawData[4] << 8) | rawData[5]) ;
}

void MPU9250_readGyroData(struct MPU9250* obj)
{
    uint8_t rawData[6];  // x/y/z gyro register data stored here
    MPU9250_readBytes(obj, MPU9250_ADDRESS, GYRO_XOUT_H, 6, &rawData[0]);  // Read the six raw data registers sequentially into data array
    obj->gyroCount[0] = (int16_t)(((int16_t)rawData[0] << 8) | rawData[1]) ;  // Turn the MSB and LSB into a signed 16-bit value
    obj->gyroCount[1] = (int16_t)(((int16_t)rawData[2] << 8) | rawData[3]) ;
    obj->gyroCount[2] = (int16_t)(((int16_t)rawData[4] << 8) | rawData[5]) ;
}

void MPU9250_readMagData(struct MPU9250* obj)
{
    uint8_t rawData[7];  // x/y/z gyro register data, ST2 register stored here, must read ST2 at end of data acquisition
    if(MPU9250_readByte(obj, AK8963_ADDRESS, AK8963_ST1) & 0x01) { // HAL_Delay for magnetometer data ready bit to be set
		MPU9250_readBytes(obj, AK8963_ADDRESS, AK8963_XOUT_L, 7, &rawData[0]);  // Read the six raw data and ST2 registers sequentially into data array
		uint8_t c = rawData[6]; // End data read by reading ST2 register
		if(!(c & 0x08)) { // Check if magnetic sensor overflow set, if not then report data
			obj->magCount[0] = (int16_t)(((int16_t)rawData[1] << 8) | rawData[0]);  // Turn the MSB and LSB into a signed 16-bit value
			obj->magCount[1] = (int16_t)(((int16_t)rawData[3] << 8) | rawData[2]) ;  // Data stored as little Endian
			obj->magCount[2] = (int16_t)(((int16_t)rawData[5] << 8) | rawData[4]) ;
        }
    }
}

uint16_t MPU9250_readTempData(struct MPU9250* obj)
{
  uint8_t rawData[2];  // x/y/z gyro register data stored here
  MPU9250_readBytes(obj, MPU9250_ADDRESS, TEMP_OUT_H, 2, &rawData[0]);  // Read the two raw data registers sequentially into data array
  obj->tmpCount = (int16_t)(((int16_t)rawData[0]) << 8 | rawData[1]) ;  // Turn the MSB and LSB into a 16-bit value
  return obj->tmpCount;
}


void MPU9250_resetMPU9250(struct MPU9250* obj) {
    // reset device
    MPU9250_writeByte(obj, MPU9250_ADDRESS, PWR_MGMT_1, 0x80); // Write a one to bit 7 reset bit; toggle reset device
    HAL_Delay(10);
}

void MPU9250_initAK8963(struct MPU9250* obj)
{
	// First extract the factory calibration for each magnetometer axis
	uint8_t rawData[3];  // x/y/z gyro calibration data stored here
	MPU9250_writeByte(obj, AK8963_ADDRESS, AK8963_CNTL, 0x00); // Power down magnetometer
	HAL_Delay(10);
	MPU9250_writeByte(obj, AK8963_ADDRESS, AK8963_CNTL, 0x0F); // Enter Fuse ROM access mode
	HAL_Delay(10);
	MPU9250_readBytes(obj, AK8963_ADDRESS, AK8963_ASAX, 3, &rawData[0]);  // Read the x-, y-, and z-axis calibration values
	obj->magCalibration[0] =  (float)(rawData[0] - 128)/256.0f + 1.0f;   // Return x-axis sensitivity adjustment values, etc.
	obj->magCalibration[1] =  (float)(rawData[1] - 128)/256.0f + 1.0f;
	obj->magCalibration[2] =  (float)(rawData[2] - 128)/256.0f + 1.0f;
	MPU9250_writeByte(obj, AK8963_ADDRESS, AK8963_CNTL, 0x00); // Power down magnetometer
	HAL_Delay(10);
	// Configure the magnetometer for continuous read and highest resolution
	// set Mscale bit 4 to 1 (0) to enable 16 (14) bit resolution in CNTL register,
	// and enable continuous mode data acquisition Mmode (bits [3:0]), 0010 for 8 Hz and 0110 for 100 Hz sample rates
	MPU9250_writeByte(obj, AK8963_ADDRESS, AK8963_CNTL, obj->Mscale << 4 | obj->Mmode); // Set magnetometer data resolution and sample ODR
	HAL_Delay(10);
}


void MPU9250_initMPU9250(struct MPU9250* obj)
{
 // Initialize MPU9250 device
 // wake up device
  MPU9250_writeByte(obj, MPU9250_ADDRESS, PWR_MGMT_1, 0x00); // Clear sleep mode bit (6), enable all sensors
  HAL_Delay(100); // Delay 100 ms for PLL to get established on x-axis gyro; should check for PLL ready interrupt

 // get stable time source
  MPU9250_writeByte(obj, MPU9250_ADDRESS, PWR_MGMT_1, 0x01);  // Set clock source to be PLL with x-axis gyroscope reference, bits 2:0 = 001

 // Configure Gyro and Accelerometer
 // Disable FSYNC and set accelerometer and gyro bandwidth to 44 and 42 Hz, respectively;
 // DLPF_CFG = bits 2:0 = 010; this sets the sample rate at 1 kHz for both
 // Maximum delay is 4.9 ms which is just over a 200 Hz maximum rate
  MPU9250_writeByte(obj, MPU9250_ADDRESS, CONFIG, 0x03);

 // Set sample rate = gyroscope output rate/(1 + SMPLRT_DIV)
  MPU9250_writeByte(obj, MPU9250_ADDRESS, SMPLRT_DIV, 0x04);  // Use a 200 Hz rate; the same rate set in CONFIG above

 // Set gyroscope full scale range
 // Range selects FS_SEL and AFS_SEL are 0 - 3, so 2-bit values are left-shifted into positions 4:3
  uint8_t c = MPU9250_readByte(obj, MPU9250_ADDRESS, GYRO_CONFIG); // get current GYRO_CONFIG register value
 // c = c & ~0xE0; // Clear self-test bits [7:5]
  c = c & ~0x02; // Clear Fchoice bits [1:0]
  c = c & ~0x18; // Clear AFS bits [4:3]
  c = c | obj->Gscale << 3; // Set full scale range for the gyro
 // c =| 0x00; // Set Fchoice for the gyro to 11 by writing its inverse to bits 1:0 of GYRO_CONFIG
  MPU9250_writeByte(obj, MPU9250_ADDRESS, GYRO_CONFIG, c ); // Write new GYRO_CONFIG value to register

 // Set accelerometer full-scale range configuration
  c = MPU9250_readByte(obj, MPU9250_ADDRESS, ACCEL_CONFIG); // get current ACCEL_CONFIG register value
 // c = c & ~0xE0; // Clear self-test bits [7:5]
  c = c & ~0x18;  // Clear AFS bits [4:3]
  c = c | obj->Ascale << 3; // Set full scale range for the accelerometer
  MPU9250_writeByte(obj, MPU9250_ADDRESS, ACCEL_CONFIG, c); // Write new ACCEL_CONFIG register value

 // Set accelerometer sample rate configuration
 // It is possible to get a 4 kHz sample rate from the accelerometer by choosing 1 for
 // accel_fchoice_b bit [3]; in this case the bandwidth is 1.13 kHz
  c = MPU9250_readByte(obj, MPU9250_ADDRESS, ACCEL_CONFIG2); // get current ACCEL_CONFIG2 register value
  c = c & ~0x0F; // Clear accel_fchoice_b (bit 3) and A_DLPFG (bits [2:0])
  c = c | 0x03;  // Set accelerometer rate to 1 kHz and bandwidth to 41 Hz
  MPU9250_writeByte(obj, MPU9250_ADDRESS, ACCEL_CONFIG2, c); // Write new ACCEL_CONFIG2 register value

 // The accelerometer, gyro, and thermometer are set to 1 kHz sample rates,
 // but all these rates are further reduced by a factor of 5 to 200 Hz because of the SMPLRT_DIV setting

  // Configure Interrupts and Bypass Enable
  // Set interrupt pin active high, push-pull, and clear on read of INT_STATUS, enable I2C_BYPASS_EN so additional chips
  // can join the I2C bus and all can be controlled by the Arduino as master
  MPU9250_writeByte(obj, MPU9250_ADDRESS, INT_PIN_CFG, 0x22);
  MPU9250_writeByte(obj, MPU9250_ADDRESS, INT_ENABLE, 0x01);  // Enable data ready (bit 0) interrupt
}


// Function which accumulates gyro and accelerometer data after device initialization. It calculates the average
// of the at-rest readings and then loads the resulting offsets into accelerometer and gyro bias registers.
void MPU9250_calibrateMPU9250(struct MPU9250* obj)
{
  uint8_t data[12]; // data array to hold accelerometer and gyro x, y, z, data
  uint16_t ii, packet_count, fifo_count;
  int32_t gyro_bias[3] = {0, 0, 0}, accel_bias[3] = {0, 0, 0};

// reset device, reset all registers, clear gyro and accelerometer bias registers
  MPU9250_writeByte(obj, MPU9250_ADDRESS, PWR_MGMT_1, 0x80); // Write a one to bit 7 reset bit; toggle reset device
  HAL_Delay(100);

// get stable time source
// Set clock source to be PLL with x-axis gyroscope reference, bits 2:0 = 001
  MPU9250_writeByte(obj, MPU9250_ADDRESS, PWR_MGMT_1, 0x01);
  MPU9250_writeByte(obj, MPU9250_ADDRESS, PWR_MGMT_2, 0x00);
  HAL_Delay(200);

// Configure device for bias calculation
  MPU9250_writeByte(obj, MPU9250_ADDRESS, INT_ENABLE, 0x00);   // Disable all interrupts
  MPU9250_writeByte(obj, MPU9250_ADDRESS, FIFO_EN, 0x00);      // Disable FIFO
  MPU9250_writeByte(obj, MPU9250_ADDRESS, PWR_MGMT_1, 0x00);   // Turn on internal clock source
  MPU9250_writeByte(obj, MPU9250_ADDRESS, I2C_MST_CTRL, 0x00); // Disable I2C master
  MPU9250_writeByte(obj, MPU9250_ADDRESS, USER_CTRL, 0x00);    // Disable FIFO and I2C master modes
  MPU9250_writeByte(obj, MPU9250_ADDRESS, USER_CTRL, 0x0C);    // Reset FIFO and DMP
  HAL_Delay(15);

// Configure MPU9250 gyro and accelerometer for bias calculation
  MPU9250_writeByte(obj, MPU9250_ADDRESS, CONFIG, 0x01);      // Set low-pass filter to 188 Hz
  MPU9250_writeByte(obj, MPU9250_ADDRESS, SMPLRT_DIV, 0x00);  // Set sample rate to 1 kHz
  MPU9250_writeByte(obj, MPU9250_ADDRESS, GYRO_CONFIG, 0x00);  // Set gyro full-scale to 250 degrees per second, maximum sensitivity
  MPU9250_writeByte(obj, MPU9250_ADDRESS, ACCEL_CONFIG, 0x00); // Set accelerometer full-scale to 2 g, maximum sensitivity

// Configure FIFO to capture accelerometer and gyro data for bias calculation
  MPU9250_writeByte(obj, MPU9250_ADDRESS, USER_CTRL, 0x40);   // Enable FIFO
  MPU9250_writeByte(obj, MPU9250_ADDRESS, FIFO_EN, 0x78);     // Enable gyro and accelerometer sensors for FIFO (max size 512 bytes in MPU-9250)
  HAL_Delay(40); // accumulate 40 samples in 80 milliseconds = 480 bytes

// At end of sample accumulation, turn off FIFO sensor read
  MPU9250_writeByte(obj, MPU9250_ADDRESS, FIFO_EN, 0x00);        // Disable gyro and accelerometer sensors for FIFO
  MPU9250_readBytes(obj, MPU9250_ADDRESS, FIFO_COUNTH, 2, &data[0]); // read FIFO sample count
  fifo_count = ((uint16_t)data[0] << 8) | data[1];
  packet_count = fifo_count/12;// How many sets of full gyro and accelerometer data for averaging

  for (ii = 0; ii < packet_count; ii++) {
    int16_t accel_temp[3] = {0, 0, 0}, gyro_temp[3] = {0, 0, 0};
    MPU9250_readBytes(obj, MPU9250_ADDRESS, FIFO_R_W, 12, &data[0]); // read data for averaging
    accel_temp[0] = (int16_t) (((int16_t)data[0] << 8) | data[1]  ) ;  // Form signed 16-bit integer for each sample in FIFO
    accel_temp[1] = (int16_t) (((int16_t)data[2] << 8) | data[3]  ) ;
    accel_temp[2] = (int16_t) (((int16_t)data[4] << 8) | data[5]  ) ;
    gyro_temp[0]  = (int16_t) (((int16_t)data[6] << 8) | data[7]  ) ;
    gyro_temp[1]  = (int16_t) (((int16_t)data[8] << 8) | data[9]  ) ;
    gyro_temp[2]  = (int16_t) (((int16_t)data[10] << 8) | data[11]) ;

    accel_bias[0] += (int32_t) accel_temp[0]; // Sum individual signed 16-bit biases to get accumulated signed 32-bit biases
    accel_bias[1] += (int32_t) accel_temp[1];
    accel_bias[2] += (int32_t) accel_temp[2];
    gyro_bias[0]  += (int32_t) gyro_temp[0];
    gyro_bias[1]  += (int32_t) gyro_temp[1];
    gyro_bias[2]  += (int32_t) gyro_temp[2];

  }
    accel_bias[0] /= (int32_t) packet_count; // Normalize sums to get average count biases
    accel_bias[1] /= (int32_t) packet_count;
    accel_bias[2] /= (int32_t) packet_count;
    gyro_bias[0]  /= (int32_t) packet_count;
    gyro_bias[1]  /= (int32_t) packet_count;
    gyro_bias[2]  /= (int32_t) packet_count;

  if(accel_bias[2] > 0L) {accel_bias[2] -= (int32_t) MPU9250_accelsensitivity;}  // Remove gravity from the z-axis accelerometer bias calculation
  else {accel_bias[2] += (int32_t) MPU9250_accelsensitivity;}

// Construct the gyro biases for push to the hardware gyro bias registers, which are reset to zero upon device startup
  data[0] = (-gyro_bias[0]/4  >> 8) & 0xFF; // Divide by 4 to get 32.9 LSB per deg/s to conform to expected bias input format
  data[1] = (-gyro_bias[0]/4)       & 0xFF; // Biases are additive, so change sign on calculated average gyro biases
  data[2] = (-gyro_bias[1]/4  >> 8) & 0xFF;
  data[3] = (-gyro_bias[1]/4)       & 0xFF;
  data[4] = (-gyro_bias[2]/4  >> 8) & 0xFF;
  data[5] = (-gyro_bias[2]/4)       & 0xFF;

/// Push gyro biases to hardware registers
/*  writeByte(MPU9250_ADDRESS, XG_OFFSET_H, data[0]);
  writeByte(MPU9250_ADDRESS, XG_OFFSET_L, data[1]);
  writeByte(MPU9250_ADDRESS, YG_OFFSET_H, data[2]);
  writeByte(MPU9250_ADDRESS, YG_OFFSET_L, data[3]);
  writeByte(MPU9250_ADDRESS, ZG_OFFSET_H, data[4]);
  writeByte(MPU9250_ADDRESS, ZG_OFFSET_L, data[5]);
*/
  obj->gyroBias[0] = (float) gyro_bias[0]/(float) MPU9250_gyrosensitivity; // construct gyro bias in deg/s for later manual subtraction
  obj->gyroBias[1] = (float) gyro_bias[1]/(float) MPU9250_gyrosensitivity;
  obj->gyroBias[2] = (float) gyro_bias[2]/(float) MPU9250_gyrosensitivity;

// Construct the accelerometer biases for push to the hardware accelerometer bias registers. These registers contain
// factory trim values which must be added to the calculated accelerometer biases; on boot up these registers will hold
// non-zero values. In addition, bit 0 of the lower byte must be preserved since it is used for temperature
// compensation calculations. Accelerometer bias registers expect bias input as 2048 LSB per g, so that
// the accelerometer biases calculated above must be divided by 8.

  int32_t accel_bias_reg[3] = {0, 0, 0}; // A place to hold the factory accelerometer trim biases
  MPU9250_readBytes(obj, MPU9250_ADDRESS, XA_OFFSET_H, 2, &data[0]); // Read factory accelerometer trim values
  accel_bias_reg[0] = (int16_t) ((int16_t)data[0] << 8) | data[1];
  MPU9250_readBytes(obj, MPU9250_ADDRESS, YA_OFFSET_H, 2, &data[0]);
  accel_bias_reg[1] = (int16_t) ((int16_t)data[0] << 8) | data[1];
  MPU9250_readBytes(obj, MPU9250_ADDRESS, ZA_OFFSET_H, 2, &data[0]);
  accel_bias_reg[2] = (int16_t) ((int16_t)data[0] << 8) | data[1];

  uint32_t mask = 1uL; // Define mask for temperature compensation bit 0 of lower byte of accelerometer bias registers
  uint8_t mask_bit[3] = {0, 0, 0}; // Define array to hold mask bit for each accelerometer bias axis

  for(ii = 0; ii < 3; ii++) {
    if(accel_bias_reg[ii] & mask) mask_bit[ii] = 0x01; // If temperature compensation bit is set, record that fact in mask_bit
  }

  // Construct total accelerometer bias, including calculated average accelerometer bias from above
  accel_bias_reg[0] -= (accel_bias[0]/8); // Subtract calculated averaged accelerometer bias scaled to 2048 LSB/g (16 g full scale)
  accel_bias_reg[1] -= (accel_bias[1]/8);
  accel_bias_reg[2] -= (accel_bias[2]/8);

  data[0] = (accel_bias_reg[0] >> 8) & 0xFF;
  data[1] = (accel_bias_reg[0])      & 0xFF;
  data[1] = data[1] | mask_bit[0]; // preserve temperature compensation bit when writing back to accelerometer bias registers
  data[2] = (accel_bias_reg[1] >> 8) & 0xFF;
  data[3] = (accel_bias_reg[1])      & 0xFF;
  data[3] = data[3] | mask_bit[1]; // preserve temperature compensation bit when writing back to accelerometer bias registers
  data[4] = (accel_bias_reg[2] >> 8) & 0xFF;
  data[5] = (accel_bias_reg[2])      & 0xFF;
  data[5] = data[5] | mask_bit[2]; // preserve temperature compensation bit when writing back to accelerometer bias registers

// Apparently this is not working for the acceleration biases in the MPU-9250
// Are we handling the temperature correction bit properly?
// Push accelerometer biases to hardware registers
/*  writeByte(MPU9250_ADDRESS, XA_OFFSET_H, data[0]);
  writeByte(MPU9250_ADDRESS, XA_OFFSET_L, data[1]);
  writeByte(MPU9250_ADDRESS, YA_OFFSET_H, data[2]);
  writeByte(MPU9250_ADDRESS, YA_OFFSET_L, data[3]);
  writeByte(MPU9250_ADDRESS, ZA_OFFSET_H, data[4]);
  writeByte(MPU9250_ADDRESS, ZA_OFFSET_L, data[5]);
*/
// Output scaled accelerometer biases for manual subtraction in the main program
    obj->accelBias[0] = (float)accel_bias[0]/(float)MPU9250_accelsensitivity;
    obj->accelBias[1] = (float)accel_bias[1]/(float)MPU9250_accelsensitivity;
    obj->accelBias[2] = (float)accel_bias[2]/(float)MPU9250_accelsensitivity;
}

// Accelerometer and gyroscope self test; check calibration wrt factory settings
void MPU9250_SelfTest(struct MPU9250* obj, float * destination) // Should return percent deviation from factory trim values, +/- 14 or less deviation is a pass
{
   uint8_t rawData[6] = {0, 0, 0, 0, 0, 0};
   uint8_t selfTest[6];
   int32_t gAvg[3] = {0}, aAvg[3] = {0}, aSTAvg[3] = {0}, gSTAvg[3] = {0};
   float factoryTrim[6];
   uint8_t FS = 0;

  MPU9250_writeByte(obj, MPU9250_ADDRESS, SMPLRT_DIV, 0x00); // Set gyro sample rate to 1 kHz
  MPU9250_writeByte(obj, MPU9250_ADDRESS, CONFIG, 0x02); // Set gyro sample rate to 1 kHz and DLPF to 92 Hz
  MPU9250_writeByte(obj, MPU9250_ADDRESS, GYRO_CONFIG, FS<<3); // Set full scale range for the gyro to 250 dps
  MPU9250_writeByte(obj, MPU9250_ADDRESS, ACCEL_CONFIG2, 0x02); // Set accelerometer rate to 1 kHz and bandwidth to 92 Hz
  MPU9250_writeByte(obj, MPU9250_ADDRESS, ACCEL_CONFIG, FS<<3); // Set full scale range for the accelerometer to 2 g

  for( int ii = 0; ii < 200; ii++) { // get average current values of gyro and acclerometer

	  MPU9250_readBytes(obj, MPU9250_ADDRESS, ACCEL_XOUT_H, 6, &rawData[0]); // Read the six raw data registers into data array
	  aAvg[0] += (int16_t)(((int16_t)rawData[0] << 8) | rawData[1]) ; // Turn the MSB and LSB into a signed 16-bit value
	  aAvg[1] += (int16_t)(((int16_t)rawData[2] << 8) | rawData[3]) ;
	  aAvg[2] += (int16_t)(((int16_t)rawData[4] << 8) | rawData[5]) ;

	  MPU9250_readBytes(obj, MPU9250_ADDRESS, GYRO_XOUT_H, 6, &rawData[0]); // Read the six raw data registers sequentially into data array
	  gAvg[0] += (int16_t)(((int16_t)rawData[0] << 8) | rawData[1]) ; // Turn the MSB and LSB into a signed 16-bit value
	  gAvg[1] += (int16_t)(((int16_t)rawData[2] << 8) | rawData[3]) ;
	  gAvg[2] += (int16_t)(((int16_t)rawData[4] << 8) | rawData[5]) ;
  }

  for (int ii =0; ii < 3; ii++) { // Get average of 200 values and store as average current readings
	  aAvg[ii] /= 200;
	  gAvg[ii] /= 200;
  }

// Configure the accelerometer for self-test
   MPU9250_writeByte(obj, MPU9250_ADDRESS, ACCEL_CONFIG, 0xE0); // Enable self test on all three axes and set accelerometer range to +/- 2 g
   MPU9250_writeByte(obj, MPU9250_ADDRESS, GYRO_CONFIG, 0xE0); // Enable self test on all three axes and set gyro range to +/- 250 degrees/s
   HAL_Delay(25); // Delay a while to let the device stabilize

  for( int ii = 0; ii < 200; ii++) { // get average self-test values of gyro and acclerometer

	  MPU9250_readBytes(obj, MPU9250_ADDRESS, ACCEL_XOUT_H, 6, &rawData[0]); // Read the six raw data registers into data array
	  aSTAvg[0] += (int16_t)(((int16_t)rawData[0] << 8) | rawData[1]) ; // Turn the MSB and LSB into a signed 16-bit value
	  aSTAvg[1] += (int16_t)(((int16_t)rawData[2] << 8) | rawData[3]) ;
	  aSTAvg[2] += (int16_t)(((int16_t)rawData[4] << 8) | rawData[5]) ;

	  MPU9250_readBytes(obj, MPU9250_ADDRESS, GYRO_XOUT_H, 6, &rawData[0]); // Read the six raw data registers sequentially into data array
	  gSTAvg[0] += (int16_t)(((int16_t)rawData[0] << 8) | rawData[1]) ; // Turn the MSB and LSB into a signed 16-bit value
	  gSTAvg[1] += (int16_t)(((int16_t)rawData[2] << 8) | rawData[3]) ;
	  gSTAvg[2] += (int16_t)(((int16_t)rawData[4] << 8) | rawData[5]) ;
  }

  for (int ii =0; ii < 3; ii++) { // Get average of 200 values and store as average self-test readings
	  aSTAvg[ii] /= 200;
	  gSTAvg[ii] /= 200;
  }

 // Configure the gyro and accelerometer for normal operation
   MPU9250_writeByte(obj, MPU9250_ADDRESS, ACCEL_CONFIG, 0x00);
   MPU9250_writeByte(obj, MPU9250_ADDRESS, GYRO_CONFIG, 0x00);
   HAL_Delay(25); // Delay a while to let the device stabilize

   // Retrieve accelerometer and gyro factory Self-Test Code from USR_Reg
   selfTest[0] = MPU9250_readByte(obj, MPU9250_ADDRESS, SELF_TEST_X_ACCEL); // X-axis accel self-test results
   selfTest[1] = MPU9250_readByte(obj, MPU9250_ADDRESS, SELF_TEST_Y_ACCEL); // Y-axis accel self-test results
   selfTest[2] = MPU9250_readByte(obj, MPU9250_ADDRESS, SELF_TEST_Z_ACCEL); // Z-axis accel self-test results
   selfTest[3] = MPU9250_readByte(obj, MPU9250_ADDRESS, SELF_TEST_X_GYRO); // X-axis gyro self-test results
   selfTest[4] = MPU9250_readByte(obj, MPU9250_ADDRESS, SELF_TEST_Y_GYRO); // Y-axis gyro self-test results
   selfTest[5] = MPU9250_readByte(obj, MPU9250_ADDRESS, SELF_TEST_Z_GYRO); // Z-axis gyro self-test results

  // Retrieve factory self-test value from self-test code reads
   factoryTrim[0] = (float)(2620/1<<FS)*(pow( 1.01 , ((float)selfTest[0] - 1.0) )); // FT[Xa] factory trim calculation
   factoryTrim[1] = (float)(2620/1<<FS)*(pow( 1.01 , ((float)selfTest[1] - 1.0) )); // FT[Ya] factory trim calculation
   factoryTrim[2] = (float)(2620/1<<FS)*(pow( 1.01 , ((float)selfTest[2] - 1.0) )); // FT[Za] factory trim calculation
   factoryTrim[3] = (float)(2620/1<<FS)*(pow( 1.01 , ((float)selfTest[3] - 1.0) )); // FT[Xg] factory trim calculation
   factoryTrim[4] = (float)(2620/1<<FS)*(pow( 1.01 , ((float)selfTest[4] - 1.0) )); // FT[Yg] factory trim calculation
   factoryTrim[5] = (float)(2620/1<<FS)*(pow( 1.01 , ((float)selfTest[5] - 1.0) )); // FT[Zg] factory trim calculation

 // Report results as a ratio of (STR - FT)/FT; the change from Factory Trim of the Self-Test Response
 // To get percent, must multiply by 100
   for (int i = 0; i < 3; i++) {
     destination[i] = 100.0*((float)(aSTAvg[i] - aAvg[i]))/factoryTrim[i] - 100.; // Report percent differences
     destination[i+3] = 100.0*((float)(gSTAvg[i] - gAvg[i]))/factoryTrim[i+3] - 100.; // Report percent differences
   }
}
void MPU9250_print(struct MPU9250* obj, uint8_t type){//type 0: raw, 1:after calli value
	if(type == 0){
		for(int i=0; i<3; i++){
			printf("%d ", obj->accelCount[i]);
		}
		for(int i=0; i<3; i++){
			printf("%d ", obj->gyroCount[i]);
		}
		printf("\r\n");
	}
	else if(type == 1){
		printf("%f %f %f, %f %f %f\r\n", obj->ax, obj->ay, obj->az, obj->gx, obj->gy, obj->gz);
	}
}
