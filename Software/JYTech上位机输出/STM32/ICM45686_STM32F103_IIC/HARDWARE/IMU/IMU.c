#include "IMU.h"
#include "icm45686_port.h"

int IMU_Init(void)
{
    return setup_imu(1, 1, 1);
}

int IMU_ReadData(float accel_mg[3], float gyro_dps[3], float *temperature_degc)
{
    return bsp_IcmGetRawData(accel_mg, gyro_dps, temperature_degc);
}
