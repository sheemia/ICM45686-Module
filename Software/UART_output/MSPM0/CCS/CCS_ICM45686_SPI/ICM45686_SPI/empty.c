/*
 * Copyright (c) 2021, Texas Instruments Incorporated
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * *  Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * *  Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * *  Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "ti_msp_dl_config.h"
#include "stdio.h"
#include "IMU.h"

void delay_ms(uint32_t __ms);
float accel_mg[3];
float gyro_dps[3];
float temperature_degc;
//重定向fputc函数
int fputc(int ch, FILE *stream)
{
    while( DL_UART_isBusy(UART_0_INST) == true );
    DL_UART_Main_transmitData(UART_0_INST, ch);
    return ch;
}

//重定向fputs函数
int fputs(const char* restrict s, FILE* restrict stream) {

    uint16_t char_len=0;
    while(*s!=0)
    {
        while( DL_UART_isBusy(UART_0_INST) == true );
        DL_UART_Main_transmitData(UART_0_INST, *s++);
        char_len++;
    }
    return char_len;
}
int puts(const char* _ptr)
{
 return 0;
}

void TimeA1_Init(void)
{
	NVIC_ClearPendingIRQ(TIMER_0_INST_INT_IRQN);
	NVIC_EnableIRQ(TIMER_0_INST_INT_IRQN);
}


void TIMER_0_INST_IRQHandler(void)
{
	switch(DL_TimerG_getPendingInterrupt(TIMER_0_INST))
	{
		case DL_TIMER_IIDX_ZERO:
		    IMU_ReadData(accel_mg, gyro_dps, &temperature_degc);
		break;
		default:
			
		break;
	}
}

//SPI 模式接线
// PB6------------------------CS
// PB7------------------------MISO
// PB8------------------------MOSI
// PB9------------------------SCLK
int main(void)
{
    SYSCFG_DL_init();
    printf("OK");
	if (IMU_Init() != 0)
	{
		printf("ICM45686 init failed\r\n");
		while (1) {}
	}
    delay_ms(100);
    TimeA1_Init();

    printf("\r\n=========================================\r\n");
    printf("  ICM45686 raw data driver demo\r\n");
    printf("  Output: accel / gyro / temperature\r\n");
    printf("-----------------------------------------\r\n");
    printf("=========================================\r\n");

    while (1)
    {
        printf("acc(mg):%.2f,%.2f,%.2f gyro(dps):%.2f,%.2f,%.2f temp(C):%.2f\r\n",
               accel_mg[0], accel_mg[1], accel_mg[2],
               gyro_dps[0], gyro_dps[1], gyro_dps[2], temperature_degc);
        delay_ms(50);
    }
}
