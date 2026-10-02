#ifndef __ICM45686_PORT_H
#define __ICM45686_PORT_H

/*
 * ICM45686驱动与本工程SPI/IIC底层的适配层。
 */

int setup_imu(int use_ln, int accel_en, int gyro_en);
int bsp_IcmGetRawData(float accel_mg[3], float gyro_dps[3], float *temp_degc);

#endif
