// 9軸センサ制御プログラム

#include <Wire.h>

// スレーブアドレス
#define MPU9250_ADDR          0x68  // 加速度、ジャイロ
#define MPU9250_COMPASS_ADDR  0x0C  // コンパス

// レジスタアドレス
#define WHO_AM_I              0x75  // 疎通確認用（正常稼働時0x71が格納されている）

// 指定レジスタデータ書き込み関数
void write_register(uint8_t reg, uint8_t data){
  Wire.beginTransmission(MPU9250_ADDR);
  Wire.write(reg);
  Wire.write(data);
  Wire.endTransmission();
}

// 指定レジスタデータ読み取り関数
uint8_t read_register(uint8_t reg) {
  Wire.beginTransmission(MPU9250_ADDR);
  Wire.write(reg);
  Wire.endTransmission(false);
  Wire.requestFrom(MPU9250_ADDR, 1);

  if(!Wire.available()) return -1;
  return (Wire.read());
}

// [未完]指定レジスタデータ読み取り関数（16ビット版）
int16_t read16_register(uint8_t reg) {
  Wire.beginTransmission(MPU9250_ADDR);
  Wire.write(reg);
  Wire.endTransmission(false);
  Wire.requestFrom(MPU9250_ADDR, 2);    // TODO: 2バイト読み取ってくれない
                                        // エンディアンの関係かもしれない
                                        // 例：0x3bにx軸加速度の上位8ビット
                                        //   0x3cにx軸加速度の下位8ビット

  if(!Wire.available()) return 0;
  return (Wire.read());
}
void setup() {
  // コンパス起動に必要な処理
  // TODO: これを有効化すると加速度センサの値が読み取れなくなる（要検証）
  //write_register(0x6b, 0x00);
  //write_register(0x37, 0x02);

  Serial.begin(9600);   // シリアルモニタ用
}

void loop() {
  //int c = read_register(WHO_AM_I);

  int acc_x_top = read_register(0x3b);
  int acc_x_bottom = read_register(0x3c);
  int acc_x = (acc_x_top << 8) | acc_x_bottom;
  Serial.println(acc_x);

  delay(500);

}
