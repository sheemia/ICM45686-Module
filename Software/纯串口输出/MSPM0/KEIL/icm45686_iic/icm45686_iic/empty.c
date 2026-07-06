#include "board.h"
#include <stdio.h>
#include "bsp_spi.h"
#include "IMU.h"

// VCC--------5V或者3.3V都可以
//IIC 模式接线
// PA0------------------------SDA
// PA1------------------------SCL
//SPI 模式接线
// PB6------------------------CS
// PB7------------------------MISO
// PB8------------------------MOSI
// PB9------------------------SCLK
void TimeA1_Init(void);
extern uint32_t nowtime;

float accel_mg[3];
float gyro_dps[3];
float temperature_degc;
int main(void)
{
    unsigned char buff[10] = {0};

    //开发板初始化
    board_init();
    printf("OK");
    delay_ms(100);//等待部署
	if (IMU_Init() != 0)
	{
		printf("ICM45686 init failed\r\n");
		while (1) {}
	}
    TimeA1_Init();
    delay_ms(20);

    printf("\r\n=========================================\r\n");
    printf("  ICM45686 raw data driver demo\r\n");
    printf("  Output: accel / gyro / temperature\r\n");
    printf("-----------------------------------------\r\n");
    printf("=========================================\r\n");

    while (1)
    {

		IMU_ReadData(accel_mg, gyro_dps, &temperature_degc);
        printf("acc(mg):%.2f,%.2f,%.2f gyro(dps):%.2f,%.2f,%.2f temp(C):%.2f\r\n",
               accel_mg[0], accel_mg[1], accel_mg[2],
               gyro_dps[0], gyro_dps[1], gyro_dps[2], temperature_degc);
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
