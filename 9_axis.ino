// 9軸センサ制御プログラム

#include <Wire.h>
#include<Servo.h>

// スレーブアドレス
#define MPU9250_ADDR          0x68  // 加速度、ジャイロ
#define MPU9250_COMPASS_ADDR  0x0C  // コンパス

// レジスタアドレス
#define WHO_AM_I              0x75  // 疎通確認用（正常稼働時0x71が格納されている）
Servo myservo;

float pulseMin,pulseMax,pulse_deg,pulse,deg;

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

  read_register(MPU9250_COMPASS_ADDR, 0x09);
  return com;
}
void setup() {
   //コンパス起動に必要な処理
  Wire.begin();
  Serial.begin(9600);
  
  write_register(MPU9250_ADDR, 0x6B, 0x00);
  write_register(MPU9250_ADDR, 0x37, 0x02);
  write_register(MPU9250_COMPASS_ADDR, 0x0A, 0x12);
  Serial.begin(9600);   // シリアルモニタ用
  myservo.attach(6,955,2000); /*(pin番号,最小パルス幅,最大パルス幅*/
  delay(3000);
}


void loop() {
  compass_t com;
  //int c = read_register(MPU9250_ADDR, WHO_AM_I);
  //加速度センサ

  //  int acc_x_top = read_register(MPU9250_ADDR, 0x3b);
  //  int acc_x_bottom = read_register(MPU9250_ADDR, 0x3c);
  //  int16_t acc_x = (acc_x_top << 8) | acc_x_bottom;
  //
  //  int acc_y_top = read_register(MPU9250_ADDR, 0x3d);
  //  int acc_y_bottom = read_register(MPU9250_ADDR, 0x3e);
  //  int16_t acc_y = (acc_y_top << 8) | acc_y_bottom;
  //
  //  int acc_z_top = read_register(MPU9250_ADDR, 0x3f);
  //  int acc_z_bottom = read_register(MPU9250_ADDR, 0x40);
  //  int16_t acc_z = (acc_z_top << 8) | acc_z_bottom;
  //
  ////こっからジャイロ
  //  int gyro_x_top = read_register(MPU9250_ADDR, 0x43);
  //  int gyro_x_bottom = read_register(MPU9250_ADDR, 0x44);
  //  int16_t gyro_x = (gyro_x_top << 8) | gyro_x_bottom;
  //
  //  int gyro_y_top = read_register(MPU9250_ADDR, 0x45);
  //  int gyro_y_bottom = read_register(MPU9250_ADDR, 0x46);
  //  int16_t gyro_y = (gyro_y_top << 8) | gyro_y_bottom;
  //
  //  int gyro_z_top = read_register(MPU9250_ADDR, 0x47);
  //  int gyro_z_bottom = read_register(MPU9250_ADDR, 0x48);
  //  int16_t gyro_z = (gyro_z_top << 8) | gyro_z_bottom;

  while(true) {
    com = read_compass();
    Serial.print("X = ");
    Serial.print(com.x);
    Serial.print(", Y = ");
    Serial.print(com.y);
    Serial.print(", Z = ");
    Serial.println(com.z);
    delay(2000);
  }
  //  Serial.print("x_axis = ");
  //  Serial.println(acc_x*0.061);
  //  Serial.print("y_axis = ");
  //  Serial.println(acc_y*0.061);
  //  Serial.print("z_axis = ");
  //  Serial.println(acc_z*0.061);

  //  Serial.print(i);
  //  Serial.print(" ");
  //  Serial.print("Gyro x = ") ;
  //  Serial.print(gyro_x*0.00763) ;
  //  Serial.print(" ");
  //  Serial.print("Gyro y = ") ;
  //  Serial.print(gyro_y*0.00763) ;
  //  Serial.print(" ");
  //  Serial.print("Gyro z = ") ;
  //  Serial.println(gyro_z*0.00763) ;

   pulseMin = 955;  /* パルス幅最小値を360で割る*/
   pulseMax = 2000; /* パルス幅最大値を360で割る*/

   pulse_deg = (pulseMax - pulseMin) / 360; /*1度あたり何パルス幅増えるか*/

   deg = 0;                            /*ここに何度回すか,deg表記で代入*/
   pulse = pulse_deg * deg + pulseMin; /*degは何パルス幅か*/

   myservo.writeMicroseconds(pulse);
   delay(2000);
}
