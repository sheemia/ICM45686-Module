#include <Wire.h>

// ICM-45686 I²C 地址
#define ICM45686_ADDRESS 0x69  // 根据实际情况选择 0x68 或 0x69，ad0拉低0x68，拉高：0x69
#define WHO_AM_I_REG 0x72
#define EXPECTED_WHO_AM_I 0xE9



// 初始化配置寄存器



#define ACCEL_DATA_X1_UI 0x00
#define ACCEL_DATA_X0_UI 0x01
#define ACCEL_DATA_Y1_UI 0x02
#define ACCEL_DATA_Y0_UI 0x03
#define ACCEL_DATA_Z1_UI 0x04
#define ACCEL_DATA_Z0_UI 0x05
#define GYRO_DATA_X1_UI 0x06
#define GYRO_DATA_X0_UI 0x07
#define GYRO_DATA_Y1_UI 0x08
#define GYRO_DATA_Y0_UI 0x09
#define GYRO_DATA_Z1_UI 0x0a
#define GYRO_DATA_Z0_UI 0x0b
#define TEMP_DATA1_UI 0x0c
#define TEMP_DATA0_UI 0x0d
//#define TMST_FSYNCH                                                             0x0e
//#define TMST_FSYNCL                                                             0x0f


#define PWR_MGMT0 0x10           //low noise mode
#define ACCEL_CONFIG0 0x1b       //加速度计ODR和量程
#define GYRO_CONFIG0 0x1c        //陀螺仪ODR和量程
#define IPREG_SYS2_REG_131 0x83  //加速度计低通滤波带宽
#define IPREG_SYS1_REG_172 0xac  //陀螺仪低通滤波带宽

#define GYRO_SENSITIVITY 1000.0 / 32768.0  //1000°/s，
#define ACC_SENSITIVITY 4 / 32.768     //4g，
#define DEG_TO_RAD 0.017453292519943
#define RAD_TO_DEG 57.29577951308232


int16_t ax, ay, az, gx, gy, gz;
void setup() {
  Serial.begin(115200);   // 初始化串口
  Wire.begin();           // 初始化 I²C 总线
  Wire.setClock(400000);  // 400kbit/s I2C clock sda21 scl22

  Serial.println("ICM-45686 初始化...");
  uint8_t whoAmI = readReg(ICM45686_ADDRESS, WHO_AM_I_REG);
  if (whoAmI == EXPECTED_WHO_AM_I) {
    Serial.println("ICM-45686 检测成功!");
    //initializeICM45686();
  } else {
    Serial.print("检测失败! WHO_AM_I 返回: 0x");
    Serial.println(whoAmI, HEX);
    while (1)
      ;  // 停止程序，直到错误解决
  }
  writeReg(ICM45686_ADDRESS, PWR_MGMT0, 0x0F);  // 陀螺和加速度计LN mode，上电默认寄存器是0，陀螺和加速度计是关闭的

  writeReg(ICM45686_ADDRESS, ACCEL_CONFIG0, 0x38);       // 加速度计4g，200hzODR
  writeReg(ICM45686_ADDRESS, IPREG_SYS2_REG_131, 0x06);  // 加速度计低通滤波带宽 ODR/128=50hz

  writeReg(ICM45686_ADDRESS, GYRO_CONFIG0, 0x28);        // 陀螺仪1000dps，200hzODR
  writeReg(ICM45686_ADDRESS, IPREG_SYS1_REG_172, 0x06);  // 加速度计低通滤波带宽 ODR/128=50hz
}

void loop() {

  ICM45688_getacc(&ax, &ay, &az);

  ICM45688_getgyro(&gx, &gy, &gz);



  Serial.print(ax * ACC_SENSITIVITY);
  Serial.print("\t");
  Serial.print(ay * ACC_SENSITIVITY);
  Serial.print("\t");
  Serial.print(az * ACC_SENSITIVITY);
  Serial.print("\t");
  Serial.print(gx *GYRO_SENSITIVITY);
  Serial.print("\t");
  Serial.print(gy *GYRO_SENSITIVITY);
  Serial.print("\t");
  Serial.print(gz *GYRO_SENSITIVITY);
  Serial.println(" ");
  delay(20);  // 每隔500ms读取一次
}


void ICM45688_getacc(int16_t* ax, int16_t* ay, int16_t* az) {

  uint8_t buffer[6];                                // x/y/z accel register data stored here
  readRegs(ICM45686_ADDRESS, ACCEL_DATA_X1_UI, 6, buffer);  // Read the 12 raw data registers into data array
  *ax = buffer[1] << 8 | buffer[0];                 // Turn the MSB and LSB into a signed 16-bit value
  *ay = buffer[3] << 8 | buffer[2];
  *az = buffer[5] << 8 | buffer[4];
}

void ICM45688_getgyro(int16_t* gx, int16_t* gy, int16_t* gz) {

  uint8_t buffer[6];                                // x/y/z accel register data stored here
  readRegs(ICM45686_ADDRESS, GYRO_DATA_X1_UI, 6, buffer);  // Read the 12 raw data registers into data array
  *gx = buffer[1] << 8 | buffer[0];                 // Turn the MSB and LSB into a signed 16-bit value
  *gy = buffer[3] << 8 | buffer[2];
  *gz = buffer[5] << 8 | buffer[4];
}



void writeReg(uint8_t address, uint8_t subAddress, uint8_t data)  //写入寄存器一个字节
{
  Wire.beginTransmission(address);  // 从器件IIC地址
  Wire.write(subAddress);           //  访问寄存器地址
  Wire.write(data);                 // 写入的数据
  Wire.endTransmission();           //开始IIC通信
}


uint8_t readReg(uint8_t address, uint8_t subAddress)  //从寄存器读取一个字节
{
  uint8_t data = 0;
  Wire.beginTransmission(address);  // 从设备地址
  Wire.write(subAddress);           // 访问的寄存器地址
  Wire.endTransmission(false);      // false：不释放总线 继续通信
  Wire.requestFrom(address, 1);     // Read bytes from slave register address
  data = Wire.read();               // Put read results in the Rx buffer
  return data;
}


void readRegs(uint8_t address, uint8_t subAddress, uint8_t count, uint8_t data[])  //从寄存器读取多个字节
{
  uint8_t i = 0;
  Wire.beginTransmission(address);   // 从设备地址
  Wire.write(subAddress);            // 访问的寄存器地址
  Wire.endTransmission(false);       // false：不释放总线 继续通信
  Wire.requestFrom(address, count);  // Read bytes from slave register address
  while (Wire.available()) {
    data[i++] = Wire.read();  // Put read results in the Rx buffer
  }
}
