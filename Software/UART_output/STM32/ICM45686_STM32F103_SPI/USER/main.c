#include "delay.h"
#include "sys.h"
#include "usart.h" 
#include "myiic.h"
#include "IMU.h"
#include "spi.h"
// 模式选择需要在icm45686_port.c里选择宏定义ICM_USE_HARD_SPI/ICM_USE_I2C

// VCC--------5V或者3.3V都可以
//SPI 模式接线
// PA2------------------------CS
// PB13------------------------SCLK
// PB14------------------------MISO
// PB15------------------------MOSI

//IIC 模式接线
// PB6------------------------SCL
// PB7------------------------SDA
// AD0默认上拉可以不接
float accel_mg[3];
float gyro_dps[3];
float temperature_degc;

int main(void)
{
	SystemInit();
	delay_init();	    	 //延时函数初始化	  
	uart_init(115200);	 	//串口初始化为115200
	printf("OK\r\n");
	IIC_Init();
	SPI2_Init();
	//SPI2_SetSpeed(SPI_BaudRatePrescaler_16);
	//delay_ms(100);
	
	//load_config();
	//delay_ms(50);
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
		delay_ms(10);
	}
}



