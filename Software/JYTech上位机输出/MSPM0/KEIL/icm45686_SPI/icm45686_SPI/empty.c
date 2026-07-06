#include "board.h"
#include <stdio.h>
#include "bsp_spi.h"
#include "IMU.h"
#include <string.h>

// VCC--------5V或者3.3V都可以
//IIC 模式接线
// PA0------------------------SDA
// PA1------------------------SCL
//SPI 模式接线
// PA2------------------------CS
// PA4------------------------MISO
// PB17------------------------MOSI
// PB18------------------------SCLK
void TimeA1_Init(void);
extern uint32_t nowtime;

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
    unsigned char buff[10] = {0};

    //开发板初始化
    board_init();
    delay_ms(100);//等待部署
	if (IMU_Init() != 0)
	{
		while (1) {}
	}
    TimeA1_Init();
    delay_ms(20);

    while (1)
    {

		IMU_ReadData(accel_mg, gyro_dps, &temperature_degc);
        JYTech_SendFrame();
        delay_ms(10);
    }
}

void TimeA1_Init(void)
{
	NVIC_ClearPendingIRQ(TIMER_1_INST_INT_IRQN);
	NVIC_EnableIRQ(TIMER_1_INST_INT_IRQN);
}


void TIMER_1_INST_IRQHandler(void)
{
	switch(DL_TimerA_getPendingInterrupt(TIMER_1_INST))
	{
		case DL_TIMER_IIDX_ZERO:
			nowtime++;
			DL_TimerG_clearInterruptStatus(TIMER_1_INST, DL_TIMER_IIDX_ZERO);
		break;
		default:
			
		break;
	}
}
