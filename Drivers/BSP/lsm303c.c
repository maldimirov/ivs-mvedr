
#include "main.h"
#include "lsm303c.h"
#include "bsp.h"

LSM303C_StatusTypedef GyroStatus = MAG_ERROR;
int32_t AngRate[3] = {0, 0, 0};

void L3GD20_Handler(void)
{
  if (L3GD20_GetDataStatus() & 0x08) {
    L3GD20_ReadXYZAngRate();
  }
}