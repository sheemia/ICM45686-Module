#ifndef ICM45686_DEMO_IMU_H
#define ICM45686_DEMO_IMU_H

int IMU_Init(void);
int IMU_ReadData(float accel_mg[3], float gyro_dps[3], float *temperature_degc);

#endif
