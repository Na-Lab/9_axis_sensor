#ifndef NINE_AXIS_H
#define NINE_AXIS_H
#include <Arduino.h>

/** 加速度センサデータ **/
struct rawAcceleration {
  int x; /**< 加速度X軸方向 */
  int y; /**< 加速度Y軸方向 */
  int z; /**< 加速度Z軸方向 */
};

/** 角加速度センサデータ **/
struct rawAngularAcceleration {
  int x; /**< 角加速度X軸方向 */
  int y; /**< 角加速度Y軸方向 */
  int z; /**< 角加速度Z軸方向 */
};

/** 磁束密度センサデータ **/
struct rawMagneticFluxDensity {
  int x; /**< 磁束密度X軸方向 */
  int y; /**< 磁束密度Y軸方向 */
  int z; /**< 磁束密度Z軸方向 */
};

/**
 * @brief 9軸センサMPU-9250制御クラス
 *
 */
class NineAxis {
 public:
  NineAxis();

  void begin();
  /**
   * @brief センサ情報更新関数
   *
   * MPU-9250から加速度、角加速度、磁束密度を受信して緯度経度を取得します。
   */
  void update();
  int getRawAccelerationX();
  int getRawAccelerationY();
  int getRawAccelerationZ();
  int getRawAngularAccelerationX();
  int getRawAngularAccelerationY();
  int getRawAngularAccelerationZ();
  int getRawMagneticFluxDensityX();
  int getRawMagneticFluxDensityY();
  int getRawMagneticFluxDensityZ();

 private:
  const byte MPU9250Address;
  const byte MPU9250CompassAddress;
  struct rawAcceleration        acceleration;
  struct rawAngularAcceleration angularAcceleration;
  struct rawMagneticFluxDensity magneticFluxDensity;

  /**
   * @brief レジスタデータ書き込み関数
   *
   * MPU-9250内のレジスタにデータを1バイト書き込みます。
   * @param[in] slaveAddress    通信対象アドレス
   * @param[in] registerAddress 書き込みレジスタアドレス
   * @param[in] data            書き込みデータ
   */
  void writeRegisterData(byte slaveAddress, byte registerAddress, byte data);

  /**
   * @brief レジスタデータ読み取り関数
   *
   * MPU-9250内のレジスタからデータを1バイト読み取ります。
   * @param[in] slaveAddress    通信対象アドレス
   * @param[in] registerAddress 読み取りレジスタアドレス
   * @return                    受信データ
   */
  byte readRegisterData(byte slaveAddress, byte registerAddress);

  /**
   * @brief 加速度読み取り関数
   *
   * MPU-9250から加速度を読み取ります。
   * @param[out] acceleration    加速度データ格納構造体
   */
  void readAcceleration(struct rawAcceleration *acceleration);

  /**
   * @brief 角加速度読み取り関数
   *
   * MPU-9250から角加速度を読み取ります。
   * @param[out] angular    角加速度データ格納構造体
   */
  void readAngularAcceleration(struct rawAngularAcceleration *angular);

  /**
   * @brief 磁束密度読み取り関数
   *
   * MPU-9250から磁束密度を読み取ります。
   * @param[out] Magnetic    磁束密度データ格納構造体
   */
  void readMagneticFluxDensity(struct rawMagneticFluxDensity *magnetic);
};

#endif
