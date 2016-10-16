
#ifndef __MAIN_H
#define __MAIN_H

#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "stm32f4xx_hal.h"

// BSP includes component
#include "bsp.h"

// FatFs includes component
#include "ff_gen_drv.h"
#include "sd_diskio.h"

// USB related
#include "usbd_core.h"
#include "usbd_desc.h"
#include "usbd_msc.h"
#include "usbd_storage.h"

typedef struct _mot_ {
  int32_t AccX;
  int32_t AccY;
  int32_t AccZ;
  int32_t AngX;
  int32_t AngY;
  int32_t AngZ;
  int32_t MagX;
  int32_t MagY;
  int32_t MagZ;
} mot;

#define INERTIAL_SAMPLE_TIME_MS 10
#define IQ_LEN 1300

extern volatile char AccStat;
extern volatile char Tunnel;
extern volatile char UsbStat;
extern volatile char fLog;
extern uint16_t rmc_sog, rmc_cog;
extern volatile uint32_t log_period;
extern TIM_HandleTypeDef    t1;

#define RX_BUFFER_SIZE    2048
#define LINE_BUFFER_SIZE 1024
extern char txline[LINE_BUFFER_SIZE];
extern char rxline[LINE_BUFFER_SIZE];

void ADC_Handler();
void FTP_Handler();
void USB_Handler();
void dputc(char ch);
void      DebugPrint    (char *pMsg, uint16_t msgSize);
uint32_t GetAdcValue(uint32_t channel);
uint32_t GetPhrase(char *dst, char eor);
uint32_t GetBytes(char *dst, uint32_t size);
static void MainThread(void const *argument);
void InertialSampleTask(void);

#endif
