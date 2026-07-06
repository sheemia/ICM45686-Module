# STM32F407 HAL 驱动示例

本工程读取 ICM45686 的三轴加速度、三轴角速度和温度，不包含姿态解算。默认总线模式以 `USER/icm45686_port.c` 中的接口宏为准。

SPI 接线：CS=`PA4`，SCLK=`PA5`，MISO=`PA6`，MOSI=`PA7`。IIC 接线：SCL=`PC1`，SDA=`PC2`。串口为 115200 8N1。

使用 Keil MDK 打开 `USER` 目录中的工程文件，编译并烧录即可观察 `acc(mg)`、`gyro(dps)`、`temp(C)` 输出。驱动结构、接口和默认传感器配置见 [ICM45686_example 总说明](../../README.md)。
