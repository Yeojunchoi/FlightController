#ifndef MPU9250_H
#define MPU9250_H
 

#ifdef __cplusplus
extern "C" {
#endif
#include "main.h"
#include "stm32f4xx_hal_i2c.h"
#include "string.h"


// See also MPU-9250 Register Map and Descriptions, Revision 4.0, RM-MPU-9250A-00, Rev. 1.4, 9/9/2013 for registers not listed in 
// above document; the MPU9250 and MPU9150 are virtually identical but the latter has a different register map
//
//Magnetometer Registers
#define AK8963_ADDRESS   0x0C<<1
#define AK8963_WHO_AM_I  0x00 // should return 0x48
#define AK8963_INFO      0x01
#define AK8963_ST1       0x02  // data ready status bit 0
#define AK8963_XOUT_L    0x03  // data
#define AK8963_XOUT_H    0x04
#define AK8963_YOUT_L    0x05
#define AK8963_YOUT_H    0x06
#define AK8963_ZOUT_L    0x07
#define AK8963_ZOUT_H    0x08
#define AK8963_ST2       0x09  // Data overflow bit 3 and data read error status bit 2
#define AK8963_CNTL      0x0A  // Power down (0000), single-measurement (0001), self-test (1000) and Fuse ROM (1111) modes on bits 3:0
#define AK8963_ASTC      0x0C  // Self test control
#define AK8963_I2CDIS    0x0F  // I2C disable
#define AK8963_ASAX      0x10  // Fuse ROM x-axis sensitivity adjustment value
#define AK8963_ASAY      0x11  // Fuse ROM y-axis sensitivity adjustment value
#define AK8963_ASAZ      0x12  // Fuse ROM z-axis sensitivity adjustment value

#define SELF_TEST_X_GYRO 0x00                  
#define SELF_TEST_Y_GYRO 0x01                                                                          
#define SELF_TEST_Z_GYRO 0x02

/*#define X_FINE_GAIN      0x03 // [7:0] fine gain
#define Y_FINE_GAIN      0x04
#define Z_FINE_GAIN      0x05
#define XA_OFFSET_H      0x06 // User-defined trim values for accelerometer
#define XA_OFFSET_L_TC   0x07
#define YA_OFFSET_H      0x08
#define YA_OFFSET_L_TC   0x09
#define ZA_OFFSET_H      0x0A
#define ZA_OFFSET_L_TC   0x0B */

#define SELF_TEST_X_ACCEL 0x0D
#define SELF_TEST_Y_ACCEL 0x0E    
#define SELF_TEST_Z_ACCEL 0x0F

#define SELF_TEST_A      0x10

#define XG_OFFSET_H      0x13  // User-defined trim values for gyroscope
#define XG_OFFSET_L      0x14
#define YG_OFFSET_H      0x15
#define YG_OFFSET_L      0x16
#define ZG_OFFSET_H      0x17
#define ZG_OFFSET_L      0x18
#define SMPLRT_DIV       0x19
#define CONFIG           0x1A
#define GYRO_CONFIG      0x1B
#define ACCEL_CONFIG     0x1C
#define ACCEL_CONFIG2    0x1D
#define LP_ACCEL_ODR     0x1E   
#define WOM_THR          0x1F   

#define MOT_DUR          0x20  // Duration counter threshold for motion interrupt generation, 1 kHz rate, LSB = 1 ms
#define ZMOT_THR         0x21  // Zero-motion detection threshold bits [7:0]
#define ZRMOT_DUR        0x22  // Duration counter threshold for zero motion interrupt generation, 16 Hz rate, LSB = 64 ms

#define FIFO_EN          0x23
#define I2C_MST_CTRL     0x24   
#define I2C_SLV0_ADDR    0x25
#define I2C_SLV0_REG     0x26
#define I2C_SLV0_CTRL    0x27
#define I2C_SLV1_ADDR    0x28
#define I2C_SLV1_REG     0x29
#define I2C_SLV1_CTRL    0x2A
#define I2C_SLV2_ADDR    0x2B
#define I2C_SLV2_REG     0x2C
#define I2C_SLV2_CTRL    0x2D
#define I2C_SLV3_ADDR    0x2E
#define I2C_SLV3_REG     0x2F
#define I2C_SLV3_CTRL    0x30
#define I2C_SLV4_ADDR    0x31
#define I2C_SLV4_REG     0x32
#define I2C_SLV4_DO      0x33
#define I2C_SLV4_CTRL    0x34
#define I2C_SLV4_DI      0x35
#define I2C_MST_STATUS   0x36
#define INT_PIN_CFG      0x37
#define INT_ENABLE       0x38
#define DMP_INT_STATUS   0x39  // Check DMP interrupt
#define INT_STATUS       0x3A
#define ACCEL_XOUT_H     0x3B
#define ACCEL_XOUT_L     0x3C
#define ACCEL_YOUT_H     0x3D
#define ACCEL_YOUT_L     0x3E
#define ACCEL_ZOUT_H     0x3F
#define ACCEL_ZOUT_L     0x40
#define TEMP_OUT_H       0x41
#define TEMP_OUT_L       0x42
#define GYRO_XOUT_H      0x43
#define GYRO_XOUT_L      0x44
#define GYRO_YOUT_H      0x45
#define GYRO_YOUT_L      0x46
#define GYRO_ZOUT_H      0x47
#define GYRO_ZOUT_L      0x48
#define EXT_SENS_DATA_00 0x49
#define EXT_SENS_DATA_01 0x4A
#define EXT_SENS_DATA_02 0x4B
#define EXT_SENS_DATA_03 0x4C
#define EXT_SENS_DATA_04 0x4D
#define EXT_SENS_DATA_05 0x4E
#define EXT_SENS_DATA_06 0x4F
#define EXT_SENS_DATA_07 0x50
#define EXT_SENS_DATA_08 0x51
#define EXT_SENS_DATA_09 0x52
#define EXT_SENS_DATA_10 0x53
#define EXT_SENS_DATA_11 0x54
#define EXT_SENS_DATA_12 0x55
#define EXT_SENS_DATA_13 0x56
#define EXT_SENS_DATA_14 0x57
#define EXT_SENS_DATA_15 0x58
#define EXT_SENS_DATA_16 0x59
#define EXT_SENS_DATA_17 0x5A
#define EXT_SENS_DATA_18 0x5B
#define EXT_SENS_DATA_19 0x5C
#define EXT_SENS_DATA_20 0x5D
#define EXT_SENS_DATA_21 0x5E
#define EXT_SENS_DATA_22 0x5F
#define EXT_SENS_DATA_23 0x60
#define MOT_DETECT_STATUS 0x61
#define I2C_SLV0_DO      0x63
#define I2C_SLV1_DO      0x64
#define I2C_SLV2_DO      0x65
#define I2C_SLV3_DO      0x66
#define I2C_MST_DELAY_CTRL 0x67
#define SIGNAL_PATH_RESET  0x68
#define MOT_DETECT_CTRL  0x69
#define USER_CTRL        0x6A  // Bit 7 enable DMP, bit 3 reset DMP
#define PWR_MGMT_1       0x6B // Device defaults to the SLEEP mode
#define PWR_MGMT_2       0x6C
#define DMP_BANK         0x6D  // Activates a specific bank in the DMP
#define DMP_RW_PNT       0x6E  // Set read/write pointer to a specific start address in specified DMP bank
#define DMP_REG          0x6F  // Register in DMP from which to read or to which to write
#define DMP_REG_1        0x70
#define DMP_REG_2        0x71 
#define FIFO_COUNTH      0x72
#define FIFO_COUNTL      0x73
#define FIFO_R_W         0x74
#define WHO_AM_I_MPU9250 0x75 // Should return 0x71
#define XA_OFFSET_H      0x77
#define XA_OFFSET_L      0x78
#define YA_OFFSET_H      0x7A
#define YA_OFFSET_L      0x7B
#define ZA_OFFSET_H      0x7D
#define ZA_OFFSET_L      0x7E

// Using the MSENSR-9250 breakout board, ADO is set to 0 
// Seven-bit device address is 110100 for ADO = 0 and 110101 for ADO = 1
//mbed uses the eight-bit device address, so shift seven-bit addresses left by one!
#define ADO 0
#if ADO
#define MPU9250_ADDRESS 0x69<<1  // Device address when ADO = 1
#else
#define MPU9250_ADDRESS 0x68<<1  // Device address when ADO = 0
#endif  

#define MPU9250_DEFAULT_TIMEOUT 100
#define MPU9250_DEG2RAD 0.017453292519f

#define MPU9250_HZ_CHECK_TICK 1000	//1hz
#define AK8963_UPDATE_TICK 10		//100hz

// Set initial input parameters
typedef enum {
	MPU9250_AFS_2G = 0,
	MPU9250_AFS_4G,
	MPU9250_AFS_8G,
	MPU9250_AFS_16G
} MPU9250_Ascale_t;

typedef enum {
	MPU9250_GFS_250DPS = 0,
	MPU9250_GFS_500DPS,
	MPU9250_GFS_1000DPS,
	MPU9250_GFS_2000DPS
} MPU9250_Gscale_t;

typedef enum {
	MPU9250_MFS_14BITS = 0, // 0.6 mG per LSB
	MPU9250_MFS_16BITS      // 0.15 mG per LSB
} MPU9250_Mscale_t;

typedef enum {
	MPU9250_M_8HZ = 0x02,  // 8 Hz update
	MPU9250_M_100HZ = 0x06 // 100 Hz continuous magnetometer
} MPU9250_M_MODE_t;

typedef enum {
	MPU9250_dmaIdle = 0,
	MPU9250_dmaMPU9250 = 1,
	MPU9250_dmaAK8963 = 2
} MPU9250_DmaFlag_t;

typedef enum {
	MPU9250_Fail = 0,
	MPU9250_Success = 1
} MPU9250_Result_t;

typedef struct {
	I2C_HandleTypeDef *hi2c;
	MPU9250_Ascale_t Ascale;
	MPU9250_Gscale_t Gscale;
	MPU9250_Mscale_t Mscale;
	MPU9250_M_MODE_t Mmode;

	float aRes, gRes, mRes;
	int16_t accelCount[3];
	int16_t gyroCount[3];
	int16_t magCount[3];
	int16_t tmpCount;

	float magBias[3];	//accel, gyro bias are written on mpu9250 register
	float magCalibration[3];

	float accel[3], gyro[3], mag[3], tmp;	//norm accel vector, gyro(rad/s), mag, tmp

	uint8_t MPU9250_buffer[14];
	uint8_t AK8963_buffer[7];

	uint32_t AK8963_lastUpdate;

	MPU9250_DmaFlag_t dmaFlag;
}MPU9250_t;

MPU9250_t mpu9250;


/*
 *  initialize mpu9250 object
 */
void MPU9250(I2C_HandleTypeDef *phi2c);

/*
 *  periodically call this function
 *  accel, gyro update rate equal to rate of calling this function
 *
 *  this function calc hz
 *  this function check whether dma is used
 *
 */
void MPU9250_updateDMA();
/*
 *  called by rxCpltCallback
 *  this function check
 *  \return imu(1), mag(2)
 */
uint8_t MPU9250_i2cRxCpltCallback();

/*
 *  copy mpu9250 dma buffer to mpu9250 accel, gyro raw buffer
 */
MPU9250_Result_t MPU9250_calRawData();

/*
 *  calculate calibrated value of accel, gyro
 */
void MPU9250_calCalibValue();

/*
 *  copy ak8963 dma buffer to mpu9250 mag raw buffer
 *  axis change to axis of accel sensor
 */
MPU9250_Result_t AK8963_calRawData();

/*
 *  calculate calib value of magnetometer
 */
void AK8963_calCalibValue();

/*
 *  private function
 */

/*
 * reset MPU9250
 */
void MPU9250_resetMPU9250();

/*
 *  calibrate accel, gyro sensor
 */
void MPU9250_calibrateMPU9250();

/*
 *  initialize MPU9250, AK8963
 *  set sample rate, DLPF bandwidth
 *  we use
 *  - MPU9250 : accel 1khz, gyro 1khz,
 *  			DLPF bandwidth : 44, 42hz
 *  			sample rate : 200hz
 *  - AK8963 :  sample rate : 100hz
 */
void MPU9250_initMPU9250();
void MPU9250_initAK8963();

/*
 *  set mpu9250.aRes, gRes, mRes from Ascale, Gscale, Mscale
 */
void MPU9250_getMres();
void MPU9250_getGres();
void MPU9250_getAres();

// Accelerometer and gyroscope self test; check calibration wrt factory settings
void MPU9250_SelfTest(float * destination);	// not used in this project

/*
 *  i2c communication with MPU9250
 */
void MPU9250_writeByte(uint8_t address, uint8_t subAddress, uint8_t data);
void MPU9250_readBytes(uint8_t address, uint8_t subAddress, uint8_t count, uint8_t * dest);
char MPU9250_readByte(uint8_t address, uint8_t subAddress);

/*
 *  lock and read data (accel, gyro, mag, temp)
 */
void MPU9250_readAccelData();
void MPU9250_readGyroData();
void MPU9250_readMagData();
uint16_t MPU9250_readTempData();

float invSqrt(float x);

#ifdef __cplusplus
}
#endif
#endif
