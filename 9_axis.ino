// 9軸センサ制御プログラム

#include <Wire.h>

// スレーブアドレス
#define MPU9250_ADDR          0x68    // 加速度、ジャイロ
#define MPU9250_COMPASS_ADDR  0x0C    // コンパス

// レジスタアドレス
#define WHO_AM_I              0x75    // 疎通確認用（正常稼働時0x71が格納されている）

// 定数
#define REVISE_ACC            0.061   // 加速度補正
#define REVISE_GYRO           0.00763 // ジャイロ補正

// 加速度センサの値
typedef struct acc{
  int x;
  int y;
  int z;
} acc_t;

// ジャイロセンサの値
typedef struct gyro{
  int x;
  int y;
  int z;
} gyro_t;

// 地磁気センサの値
typedef struct compass{
  int x;
  int y;
  int z;
} compass_t;

// 指定レジスタデータ書き込み関数
void write_register(uint8_t addr, uint8_t reg, uint8_t data){
  Wire.beginTransmission(addr);
  Wire.write(reg);
  Wire.write(data);
  Wire.endTransmission();
}


// 指定レジスタデータ読み取り関数
uint8_t read_register(uint8_t addr, uint8_t reg) {
  Wire.beginTransmission(addr);
  Wire.write(reg);
  Wire.endTransmission(false);
  Wire.requestFrom((int)addr, 1);

  if(!Wire.available()) return -1;
  return (Wire.read());
}


// 加速度データ取得関数
acc_t read_acc(){
  acc_t ac;
  int top, bottom;

   top = read_register(MPU9250_ADDR, 0x3b);
   bottom = read_register(MPU9250_ADDR, 0x3c);
   ac.x = ((top << 8) | bottom) * REVISE_ACC;

   top = read_register(MPU9250_ADDR, 0x3d);
   bottom = read_register(MPU9250_ADDR, 0x3e);
   ac.y = ((top << 8) | bottom) * REVISE_ACC;

   top = read_register(MPU9250_ADDR, 0x3f);
   bottom = read_register(MPU9250_ADDR, 0x40);
   ac.z = ((top << 8) | bottom) * REVISE_ACC;

   return ac;
}

// ジャイロデータ取得関数
gyro_t read_gyro(){
  gyro_t gy;
  int top, bottom;

   top = read_register(MPU9250_ADDR, 0x43);
   bottom = read_register(MPU9250_ADDR, 0x44);
   gy.x = ((top << 8) | bottom) * REVISE_GYRO;

   top = read_register(MPU9250_ADDR, 0x45);
   bottom = read_register(MPU9250_ADDR, 0x46);
   gy.y = ((top << 8) | bottom) * REVISE_GYRO;

   top = read_register(MPU9250_ADDR, 0x47);
   bottom = read_register(MPU9250_ADDR, 0x48);
   gy.z = ((top << 8) | bottom) * REVISE_GYRO;

   return gy;
}
// コンパスデータ取得関数
// メモ：これだけリトルエンディアン
compass_t read_compass(){
  compass_t com;
  int top, bottom;

  bottom = read_register(MPU9250_COMPASS_ADDR, 0x03);
  top = read_register(MPU9250_COMPASS_ADDR, 0x04);
  com.x = (top << 8) | bottom;

  bottom = read_register(MPU9250_COMPASS_ADDR, 0x05);
  top = read_register(MPU9250_COMPASS_ADDR, 0x06);
  com.y = (top << 8) | bottom;

  bottom = read_register(MPU9250_COMPASS_ADDR, 0x07);
  top = read_register(MPU9250_COMPASS_ADDR, 0x08);
  com.z = (top << 8) | bottom;

  // データをリフレッシュするには0x09を読む必要があり
  read_register(MPU9250_COMPASS_ADDR, 0x09);
  return com;
}
void setup() {
   //コンパス起動に必要な処理
  Wire.begin();
  write_register(MPU9250_ADDR, 0x6B, 0x00);
  write_register(MPU9250_ADDR, 0x37, 0x02);
  write_register(MPU9250_COMPASS_ADDR, 0x0A, 0x12);
  Serial.begin(9600);   // シリアルモニタ用
}


void loop() {
  compass_t com;
  acc_t ac;
  gyro_t gy;

  while(true) {
    ac = read_acc();
    Serial.println("Acceleration");
    Serial.print("X = ");
    Serial.print(ac.x);
    Serial.print(", Y = ");
    Serial.print(ac.y);
    Serial.print(", Z = ");
    Serial.println(ac.z);
    Serial.println("==============================");

    gy = read_gyro();
    Serial.println("Gyro");
    Serial.print("X = ");
    Serial.print(gy.x);
    Serial.print(", Y = ");
    Serial.print(gy.y);
    Serial.print(", Z = ");
    Serial.println(gy.z);
    Serial.println("==============================");

    com = read_compass();
    Serial.println("Geomagnetism");
    Serial.print("X = ");
    Serial.print(com.x);
    Serial.print(", Y = ");
    Serial.print(com.y);
    Serial.print(", Z = ");
    Serial.println(com.z);
    Serial.println("==============================");
    Serial.println("");
    delay(2000);
  }
}
