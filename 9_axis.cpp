#include <Arduino.h>
#include <Wire.h>

#include "9_axis.h"

NineAxis::NineAxis() : MPU9250Address(0x68), MPU9250CompassAddress(0x0C) {
  // いい初期化の方法ないかな
  rawAcceleration.x        = 0.0;
  rawAcceleration.y        = 0.0;
  rawAcceleration.z        = 0.0;
  rawAngularAcceleration.x = 0.0;
  rawAngularAcceleration.y = 0.0;
  rawAngularAcceleration.z = 0.0;
  rawMagneticFluxDensity.x = 0.0;
  rawMagneticFluxDensity.y = 0.0;
  rawMagneticFluxDensity.z = 0.0;
}

void NineAxis::begin() {
  Wire.begin();
  NineAxis::writeRegisterData(MPU9250Address, 0x6B, 0x00);
  NineAxis::writeRegisterData(MPU9250Address, 0x37, 0x02);
  NineAxis::writeRegisterData(MPU9250CompassAddress, 0x0A, 0x12);
}

void NineAxis::update() {
  NineAxis::readAcceleration();
  NineAxis::readAngularAcceleration();
  NineAxis::readMagneticFluxDensity();
}

void NineAxis::writeRegisterData(byte slaveAddress, byte registerAddress,
                                 byte data) {
  Wire.beginTransmission(slaveAddress);
  Wire.write(registerAddress);
  Wire.write(data);
  Wire.endTransmission();
}

byte NineAxis::readRegisterData(byte slaveAddress, byte registerAddress) {
  Wire.beginTransmission(slaveAddress);
  Wire.write(registerAddress);
  Wire.endTransmission(false);
  Wire.requestFrom((int)slaveAddress, 1);

  if (!Wire.available()) return -1;
  return (Wire.read());
}

void NineAxis::readAcceleration() {
  const double unitConversion = 0.061;
  int          top, bottom;

  top               = NineAxis::readRegisterData(MPU9250Address, 0x3b);
  bottom            = NineAxis::readRegisterData(MPU9250Address, 0x3c);
  rawAcceleration.x = ((top << 8) | bottom) * unitConversion;

  top               = NineAxis::readRegisterData(MPU9250Address, 0x3d);
  bottom            = NineAxis::readRegisterData(MPU9250Address, 0x3e);
  rawAcceleration.y = ((top << 8) | bottom) * unitConversion;

  top               = NineAxis::readRegisterData(MPU9250Address, 0x3f);
  bottom            = NineAxis::readRegisterData(MPU9250Address, 0x40);
  rawAcceleration.z = ((top << 8) | bottom) * unitConversion;
}

void NineAxis::readAngularAcceleration() {
  const double unitConversion = 0.00763;
  int          top, bottom;

  top                      = NineAxis::readRegisterData(MPU9250Address, 0x43);
  bottom                   = NineAxis::readRegisterData(MPU9250Address, 0x44);
  rawAngularAcceleration.x = ((top << 8) | bottom) * unitConversion;

  top                      = NineAxis::readRegisterData(MPU9250Address, 0x45);
  bottom                   = NineAxis::readRegisterData(MPU9250Address, 0x46);
  rawAngularAcceleration.y = ((top << 8) | bottom) * unitConversion;

  top                      = NineAxis::readRegisterData(MPU9250Address, 0x47);
  bottom                   = NineAxis::readRegisterData(MPU9250Address, 0x48);
  rawAngularAcceleration.z = ((top << 8) | bottom) * unitConversion;
}

// 注意：これのみリトルエンディアンである
void NineAxis::readMagneticFluxDensity() {
  const double unitConversion = 0.15;
  const double offsetX        = -39.588;
  const double offsetY        = -9.9535;
  int          top, bottom;

  bottom = NineAxis::readRegisterData(MPU9250CompassAddress, 0x03);
  top    = NineAxis::readRegisterData(MPU9250CompassAddress, 0x04);
  rawMagneticFluxDensity.x = ((top << 8) | bottom) * unitConversion + offsetX;

  bottom = NineAxis::readRegisterData(MPU9250CompassAddress, 0x05);
  top    = NineAxis::readRegisterData(MPU9250CompassAddress, 0x06);
  rawMagneticFluxDensity.y = ((top << 8) | bottom) * unitConversion + offsetY;

  bottom = NineAxis::readRegisterData(MPU9250CompassAddress, 0x07);
  top    = NineAxis::readRegisterData(MPU9250CompassAddress, 0x08);
  rawMagneticFluxDensity.z = ((top << 8) | bottom) * unitConversion;

  // データをリフレッシュするには0x09を読む必要がある
  NineAxis::readRegisterData(MPU9250CompassAddress, 0x09);
}

double NineAxis::getRawAccelerationX() { return rawAcceleration.x; }
double NineAxis::getRawAccelerationY() { return rawAcceleration.y; }
double NineAxis::getRawAccelerationZ() { return rawAcceleration.z; }
double NineAxis::getRawAngularAccelerationX() {
  return rawAngularAcceleration.x;
}
double NineAxis::getRawAngularAccelerationY() {
  return rawAngularAcceleration.y;
}
double NineAxis::getRawAngularAccelerationZ() {
  return rawAngularAcceleration.z;
}
double NineAxis::getRawMagneticFluxDensityX() {
  return rawMagneticFluxDensity.x;
}
double NineAxis::getRawMagneticFluxDensityY() {
  return rawMagneticFluxDensity.y;
}
double NineAxis::getRawMagneticFluxDensityZ() {
  return rawMagneticFluxDensity.z;
}
