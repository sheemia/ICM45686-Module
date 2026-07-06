#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "IMU.h"
#include <string.h>


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

static void JYTech_SendFrame(void)
{
	uint8_t frame[56] = {0}, checksum = 0, i;
	int16_t accel_raw[3], gyro_raw[3];
	int16_t temp_raw = (int16_t)(temperature_degc * 100.0f);
	for (i = 0; i < 3; i++) {
		float accel_g = accel_mg[i] / 1000.0f;
		accel_raw[i] = (int16_t)(accel_mg[i] * 2.048f);
		gyro_raw[i] = (int16_t)(gyro_dps[i] * 16.4f);
		memcpy(&frame[30 + i * 4], &accel_g, 4);
		memcpy(&frame[42 + i * 4], &gyro_dps[i], 4);
	}
	frame[0] = 0xAA; frame[1] = 0x55; frame[2] = 0x02; frame[3] = 0x36;
	memcpy(&frame[4], accel_raw, 6);
	memcpy(&frame[10], gyro_raw, 6);
	memcpy(&frame[28], &temp_raw, 2);
	for (i = 2; i <= 53; i++) checksum += frame[i];
	frame[54] = checksum; frame[55] = 0x5A;
	for (i = 0; i < 56; i++) fputc(frame[i], stdout);
}

int main(void)
{
	
    HAL_Init();                   	//初始化HAL库    
    Stm32_Clock_Init(336,16,2,7);  	//设置时钟,168Mhz(内部HSI 16MHz,无需外部晶振)
	delay_init(168);               	//初始化延时函数
	uart_init(115200);             	//初始化USART

	if (IMU_Init() != 0)
	{
		while (1) {}
	}

	delay_ms(100);

	while(1)
	{
		IMU_ReadData(accel_mg, gyro_dps, &temperature_degc);
		JYTech_SendFrame();
		delay_ms(20);
	}
}

