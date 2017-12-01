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
  Serial.print(nineAxisSensor.getRawAccelX());
  Serial.print(", Y = ");
  Serial.print(nineAxisSensor.getRawAccelY());
  Serial.print(", Z = ");
  Serial.print(nineAxisSensor.getRawAccelZ());
  Serial.println("");
  Serial.println("==============================");

  Serial.println("Gyro");
  Serial.print("X = ");
  Serial.print(nineAxisSensor.getRawGyroX());
  Serial.print(", Y = ");
  Serial.print(nineAxisSensor.getRawGyroY());
  Serial.print(", Z = ");
  Serial.print(nineAxisSensor.getRawGyroZ());
  Serial.println("");
  Serial.println("==============================");

  Serial.println("Geomagnetism");
  Serial.print("X = ");
  Serial.print(nineAxisSensor.getRawMagX());
  Serial.print(", Y = ");
  Serial.print(nineAxisSensor.getRawMagY());
  Serial.print(", Z = ");
  Serial.print(nineAxisSensor.getRawMagZ());
  Serial.println("");
  Serial.println("==============================");

  Serial.println("Azimuth");
  Serial.print(nineAxisSensor.getAzimuth());
  Serial.println("");
  Serial.println("==============================");
  Serial.println("");
  // delay(2000);
}
