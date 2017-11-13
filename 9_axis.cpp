
#include "9_axis.h"
#include <Arduino.h>
#include <Wire.h>

NineAxis::NineAxis() : MPU9250Address(0x68), MPU9250CompassAddress(0x0C) {}

void NineAxis::begin() {
  Wire.begin();
  NineAxis::writeRegisterData(MPU9250Address, 0x6B, 0x00);
  NineAxis::writeRegisterData(MPU9250Address, 0x37, 0x02);
  NineAxis::writeRegisterData(MPU9250CompassAddress, 0x0A, 0x12);
}

void NineAxis::update() {
  NineAxis::readAcceleration(&acceleration);
  NineAxis::readAngularAcceleration(&angularAcceleration);
  NineAxis::readMagneticFluxDensity(&magneticFluxDensity);
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

void NineAxis::readAcceleration(struct rawAcceleration *acceleration) {
  const double unitConversion = 0.061;
  int          top, bottom;

  top             = NineAxis::readRegisterData(MPU9250Address, 0x3b);
  bottom          = NineAxis::readRegisterData(MPU9250Address, 0x3c);
  acceleration->x = ((top << 8) | bottom) * unitConversion;

  top             = NineAxis::readRegisterData(MPU9250Address, 0x3d);
  bottom          = NineAxis::readRegisterData(MPU9250Address, 0x3e);
  acceleration->y = ((top << 8) | bottom) * unitConversion;

  top             = NineAxis::readRegisterData(MPU9250Address, 0x3f);
  bottom          = NineAxis::readRegisterData(MPU9250Address, 0x40);
  acceleration->z = ((top << 8) | bottom) * unitConversion;
}

void NineAxis::readAngularAcceleration(struct rawAngularAcceleration *angular) {
  const double unitConversion = 0.00763;
  int          top, bottom;

  top        = NineAxis::readRegisterData(MPU9250Address, 0x43);
  bottom     = NineAxis::readRegisterData(MPU9250Address, 0x44);
  angular->x = ((top << 8) | bottom) * unitConversion;

  top        = NineAxis::readRegisterData(MPU9250Address, 0x45);
  bottom     = NineAxis::readRegisterData(MPU9250Address, 0x46);
  angular->y = ((top << 8) | bottom) * unitConversion;

  top        = NineAxis::readRegisterData(MPU9250Address, 0x47);
  bottom     = NineAxis::readRegisterData(MPU9250Address, 0x48);
  angular->z = ((top << 8) | bottom) * unitConversion;
}

// 注意：これのみリトルエンディアンである
void NineAxis::readMagneticFluxDensity(
    struct rawMagneticFluxDensity *magnetic) {
  const double unitConversion = 0.15;
  const double offsetX        = -39.588;
  const double offsetY        = -9.9535;
  int          top, bottom;

  bottom      = NineAxis::readRegisterData(MPU9250CompassAddress, 0x03);
  top         = NineAxis::readRegisterData(MPU9250CompassAddress, 0x04);
  magnetic->x = ((top << 8) | bottom) * unitConversion + offsetX;

  bottom      = NineAxis::readRegisterData(MPU9250CompassAddress, 0x05);
  top         = NineAxis::readRegisterData(MPU9250CompassAddress, 0x06);
  magnetic->y = ((top << 8) | bottom) * unitConversion + offsetY;

  bottom      = NineAxis::readRegisterData(MPU9250CompassAddress, 0x07);
  top         = NineAxis::readRegisterData(MPU9250CompassAddress, 0x08);
  magnetic->z = ((top << 8) | bottom) * unitConversion;

  // データをリフレッシュするには0x09を読む必要がある
  NineAxis::readRegisterData(MPU9250CompassAddress, 0x09);
}

int NineAxis::getRawAccelerationX() { return acceleration.x; }
int NineAxis::getRawAccelerationY() { return acceleration.y; }
int NineAxis::getRawAccelerationZ() { return acceleration.z; }
int NineAxis::getRawAngularAccelerationX() { return angularAcceleration.x; }
int NineAxis::getRawAngularAccelerationY() { return angularAcceleration.y; }
int NineAxis::getRawAngularAccelerationZ() { return angularAcceleration.z; }
int NineAxis::getRawMagneticFluxDensityX() { return magneticFluxDensity.x; }
int NineAxis::getRawMagneticFluxDensityY() { return magneticFluxDensity.y; }
int NineAxis::getRawMagneticFluxDensityZ() { return magneticFluxDensity.z; }
