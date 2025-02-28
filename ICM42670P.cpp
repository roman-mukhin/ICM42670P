
#include "ICM42670P.h"

ICM42670P::ICM42670P() {
  _i2c = NULL;
}

bool ICM42670P::begin(TwoWire* wire, uint8_t accel_config, uint8_t gyro_config) {

  // Setup I2C
  if (wire) _i2c = wire;
  else _i2c = &Wire;

  _i2c->begin();
  _i2c->setClock(400000);
  delay(100);

  // - checking IMU sensor connection
  uint8_t err = 0;
  uint32_t timeout = millis();
  while ((millis() - timeout < 500) && (err == 0)) {
    Wire.beginTransmission(ICM42670P_I2C_ADDRESS);  // check the IMU address
    if (Wire.endTransmission() == 0) err = 1;       // IMU is responding
    delay(100);
  }
  if (err == 0) return 0;

  // - setup imu for I2C-only mode
  i2c_write(INTF_CONFIG1, INTF_CONFIG1_I2C_ONLY);
  delay(10);

  // Reset all configs to preset values
  // - enable SOFT_RESET_DEVICE_CONFIG
  i2c_write(SIGNAL_PATH_RESET, SOFT_RESET_DEVICE_CONFIG_EN);
  delay(10);

  // - setup imu for I2C-only mode
  i2c_write(INTF_CONFIG1, INTF_CONFIG1_I2C_ONLY);
  delay(10);

  // - reading RESET_DONE_INT
  err = 0;
  i2c_read(INT_STATUS, err);
  if (err != RESET_DONE_INT) return 0;

  // - ACCEL_MODE to LN mode, GYRO_MODE to LN mode, IDLE enable
  i2c_write(PWR_MGMT0, PWR_LN_IDDLE);
  err = 0;
  i2c_read(PWR_MGMT0, err);
  if (err != PWR_LN_IDDLE) return 0;

  // - setup ACCEL_ODR and ACCEL_UI_FS_SEL to preset values
  i2c_write(ACCEL_CONFIG0, accel_config);
  err = 0;
  i2c_read(ACCEL_CONFIG0, err);
  if (err != accel_config) return 0;

  // - setup GYRO_ODR and GYRO_UI_FS_SEL to preset values
  i2c_write(GYRO_CONFIG0, gyro_config);
  err = 0;
  i2c_read(GYRO_CONFIG0, err);
  if (err != gyro_config) return 0;

  return 1;
}

void ICM42670P::getRawData(int16_t& accel_x, int16_t& accel_y, int16_t& accel_z,
                           int16_t& gyro_x, int16_t& gyro_y, int16_t& gyro_z) {

  uint8_t buff;

  // Read accel_x
  accel_x = 0;
  buff = 0;
  i2c_read(ACCEL_DATA_X1, buff);
  accel_x = (buff << 8);
  buff = 0;
  i2c_read(ACCEL_DATA_X0, buff);
  accel_x |= buff;

  // Read accel_y
  accel_y = 0;
  buff = 0;
  i2c_read(ACCEL_DATA_Y1, buff);
  accel_y = (buff << 8);
  buff = 0;
  i2c_read(ACCEL_DATA_Y0, buff);
  accel_y |= buff;

  // Read accel_z
  accel_z = 0;
  buff = 0;
  i2c_read(ACCEL_DATA_Z1, buff);
  accel_z = (buff << 8);
  buff = 0;
  i2c_read(ACCEL_DATA_Z0, buff);
  accel_z |= buff;

  // Read gyro_x
  gyro_x = 0;
  buff = 0;
  i2c_read(GYRO_DATA_X1, buff);
  gyro_x = (buff << 8);
  buff = 0;
  i2c_read(GYRO_DATA_X0, buff);
  gyro_x |= buff;

  // Read gyro_y
  gyro_y = 0;
  buff = 0;
  i2c_read(GYRO_DATA_Y1, buff);
  gyro_y = (buff << 8);
  buff = 0;
  i2c_read(GYRO_DATA_Y0, buff);
  gyro_y |= buff;

  // Read gyro_z
  gyro_z = 0;
  buff = 0;
  i2c_read(GYRO_DATA_Z1, buff);
  gyro_z = (buff << 8);
  buff = 0;
  i2c_read(GYRO_DATA_Z0, buff);
  gyro_z |= buff;
}

void ICM42670P::i2c_read(uint8_t reg, uint8_t& buf) {
  _i2c->beginTransmission(ICM42670P_I2C_ADDRESS);
  _i2c->write(reg);
  _i2c->endTransmission(0);

  _i2c->requestFrom(ICM42670P_I2C_ADDRESS, 1);
  while (_i2c->available()) buf = _i2c->read();
}

void ICM42670P::i2c_write(uint8_t reg, uint8_t value) {
  _i2c->beginTransmission(ICM42670P_I2C_ADDRESS);
  _i2c->write(reg);
  _i2c->write(value);
  _i2c->endTransmission();
}