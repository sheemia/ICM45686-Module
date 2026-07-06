# MSPM0G3507 Keil SPI 驱动示例

本工程通过 SPI 读取 ICM45686 的三轴加速度、三轴角速度和温度，不包含姿态解算。

接线：CS=`PB6`，MISO=`PB7`，MOSI=`PB8`，SCLK=`PB9`；串口为 115200 8N1。使用 Keil MDK 打开工程，编译并烧录即可观察 `acc(mg)`、`gyro(dps)`、`temp(C)` 输出。

驱动结构、接口和默认传感器配置见 [ICM45686_example 总说明](../../../../README.md)。
