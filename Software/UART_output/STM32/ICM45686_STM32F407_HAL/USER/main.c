#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "IMU.h"


// VCC--------5V或者3.3V都可以
// SPI 模式接线
// PA4 ------------------------CS
// PA5 ------------------------SCLK
// PA6------------------------MISO
// PA7------------------------MOSI

// IIC 接线
// PC1 ------------------------SCL
// PC2 ------------------------SDA
// 模式选择需要在icm45686_port.c里选择宏定义ICM_USE_HARD_SPI/ICM_USE_I2C
float accel_mg[3];
float gyro_dps[3];
float temperature_degc;

int main(void)
{
	
    HAL_Init();                   	//初始化HAL库    
    Stm32_Clock_Init(336,16,2,7);  	//设置时钟,168Mhz(内部HSI 16MHz,无需外部晶振)
	delay_init(168);               	//初始化延时函数
	uart_init(115200);             	//初始化USART

	if (IMU_Init() != 0)
	{
		printf("ICM45686 init failed\r\n");
		while (1) {}
	}

	delay_ms(100);

	printf("\r\n=========================================\r\n");
	printf("  ICM45686 raw data driver demo\r\n");
	printf("  Output: accel / gyro / temperature\r\n");
	printf("-----------------------------------------\r\n");
	printf("=========================================\r\n");

	while(1)
	{
		IMU_ReadData(accel_mg, gyro_dps, &temperature_degc);
		printf("acc(mg):%.2f\t%.2f\t%.2f\tgyro(dps):%.2f\t%.2f\t%.2f\ttemp(C):%.2f\r\n",
		       accel_mg[0], accel_mg[1], accel_mg[2],
		       gyro_dps[0], gyro_dps[1], gyro_dps[2], temperature_degc);
		delay_ms(20);
	}
}

