#ifndef ICM42670P_H
#define ICM42670P_H

#include "Arduino.h"
#include "Wire.h"

// Registers
#define SIGNAL_PATH_RESET 0x02
#define INT_CONFIG 0x06
#define ACCEL_DATA_X1 0x0b
#define ACCEL_DATA_X0 0x0c
#define ACCEL_DATA_Y1 0x0d
#define ACCEL_DATA_Y0 0x0e
#define ACCEL_DATA_Z1 0x0f
#define ACCEL_DATA_Z0 0x10
#define GYRO_DATA_X1 0x11
#define GYRO_DATA_X0 0x12
#define GYRO_DATA_Y1 0x13
#define GYRO_DATA_Y0 0x14
#define GYRO_DATA_Z1 0x15
#define GYRO_DATA_Z0 0x16
#define PWR_MGMT0 0x1f
#define GYRO_CONFIG0 0x20
#define ACCEL_CONFIG0 0x21
#define INTF_CONFIG1 0x36
#define INT_STATUS 0x3a

// Preset values for low-noise measurements: 12.5 Hz, ±2g, ±250 dps
#define ICM42670P_I2C_ADDRESS 0x68
#define INTF_CONFIG1_I2C_ONLY 0x00
#define SOFT_RESET_DEVICE_CONFIG_EN 0x10
#define RESET_DONE_INT 0x10
#define PWR_LN_IDDLE 0x1F
#define ACCEL_ODR_FS_DEFAULT 0x6C
#define GYRO_ODR_FS_DEFAULT 0x6C

class ICM42670P {
public:
  ICM42670P();
  bool begin(TwoWire* wire = NULL, uint8_t accel_config = (uint8_t)ACCEL_ODR_FS_DEFAULT, uint8_t gyro_config = (uint8_t)GYRO_ODR_FS_DEFAULT);
  void getRawData(int16_t& accel_x, int16_t& accel_y, int16_t& accel_z,
                  int16_t& gyro_x, int16_t& gyro_y, int16_t& gyro_z);

private:
  TwoWire* _i2c;
  void i2c_read(uint8_t reg, uint8_t& buf);
  void i2c_write(uint8_t reg, uint8_t value);
};

#endif