# STM32F103 IIC 驱动示例

本工程通过 IIC 读取 ICM45686 的三轴加速度、三轴角速度和温度，不包含姿态解算。

接线：SCL=`PB6`，SDA=`PB7`；USART1 为 115200 8N1。使用 Keil MDK 打开 `USER` 目录中的工程文件，编译并烧录即可观察 `acc(mg)`、`gyro(dps)`、`temp(C)` 输出。

驱动结构、接口和默认传感器配置见 [ICM45686_example 总说明](../../README.md)。
