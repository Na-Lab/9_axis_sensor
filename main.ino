// 9軸センサ制御プログラム
#include <Arduino.h>

#include "9_axis.h"

NineAxis nineAxisSensor;
void     setup() {
  nineAxisSensor.begin();
  Serial.begin(9600);  // シリアルモニタ用
}

void loop() {
  nineAxisSensor.update();
  Serial.println("Acceleration");
  Serial.print("X = ");
  Serial.print(nineAxisSensor.getRawAccelerationX());
  Serial.print(", Y = ");
  Serial.print(nineAxisSensor.getRawAccelerationY());
  Serial.print(", Z = ");
  Serial.print(nineAxisSensor.getRawAccelerationZ());
  Serial.println("");
  Serial.println("==============================");

  Serial.println("Gyro");
  Serial.print("X = ");
  Serial.print(nineAxisSensor.getRawAngularAccelerationX());
  Serial.print(", Y = ");
  Serial.print(nineAxisSensor.getRawAngularAccelerationY());
  Serial.print(", Z = ");
  Serial.print(nineAxisSensor.getRawAngularAccelerationZ());
  Serial.println("");
  Serial.println("==============================");

  Serial.println("Geomagnetism");
  Serial.print("X = ");
  Serial.print(nineAxisSensor.getRawMagneticFluxDensityX());
  Serial.print(", Y = ");
  Serial.print(nineAxisSensor.getRawMagneticFluxDensityY());
  Serial.print(", Z = ");
  Serial.print(nineAxisSensor.getRawMagneticFluxDensityZ());
  Serial.println("");
  Serial.println("==============================");
  Serial.println("");
  delay(2000);
}
