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
#include "string.h"
#include "IMU.h"

void delay_ms(uint32_t __ms);
int fputc(int ch, FILE *stream);
float accel_mg[3];
float gyro_dps[3];
float temperature_degc;

static void JYTech_SendFrame(void)
{
    uint8_t frame[56] = {0};
    int16_t accel_raw[3];
    int16_t gyro_raw[3];
    int16_t temp_raw;
    uint8_t checksum = 0;
    uint8_t i;

    for (i = 0; i < 3; i++) {
        accel_raw[i] = (int16_t)(accel_mg[i] * 2.048f);
        gyro_raw[i] = (int16_t)(gyro_dps[i] * 16.4f);
    }
    temp_raw = (int16_t)(temperature_degc * 100.0f);
    frame[0] = 0xAA; frame[1] = 0x55; frame[2] = 0x02; frame[3] = 0x36;
    memcpy(&frame[4], accel_raw, sizeof(accel_raw));
    memcpy(&frame[10], gyro_raw, sizeof(gyro_raw));
    memcpy(&frame[28], &temp_raw, sizeof(temp_raw));
    for (i = 0; i < 3; i++) {
        float accel_g = accel_mg[i] / 1000.0f;
        memcpy(&frame[30 + i * 4], &accel_g, sizeof(accel_g));
        memcpy(&frame[42 + i * 4], &gyro_dps[i], sizeof(gyro_dps[i]));
    }
    for (i = 2; i <= 53; i++) checksum += frame[i];
    frame[54] = checksum;
    frame[55] = 0x5A;
    for (i = 0; i < sizeof(frame); i++) fputc(frame[i], stdout);
}
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
	if (IMU_Init() != 0)
	{
		while (1) {}
	}
    delay_ms(100);
    TimeA1_Init();

    while (1)
    {
        JYTech_SendFrame();
        delay_ms(50);
    }
}
