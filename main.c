
#include "main.h"
#include "bsp.h"
#include "config.h"
#include "gsm.h"
#include "cmsis_os.h"
#include "at.h"
#include "gps.h"
#include "l3gd20.h"
#include "lis3dh.h"

#define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#define GPS_FLAG 0x3FFFFFFF;

USBD_HandleTypeDef USBD_Device;

volatile uint32_t poicnt = 0, poi_period = 0, acccnt = 0, logcnt = 0, log_speed = 0, log_period = 0, log_repeat = 0, log_timeout = 0;

char txline[LINE_BUFFER_SIZE];
char rxline[LINE_BUFFER_SIZE];

volatile char AccStat;
volatile char MyTick;
volatile char Tunnel;
volatile char UsbStat;

extern char SDPath[4];     // SD card logical drive path
extern FATFS SDFatFs;      // File system object for SD card logical drive
extern TIM_HandleTypeDef t1;

//static uint32_t mq_in;
static mot mq[IQ_LEN];
//static osTimerId        InertialTimerId;
static mot ms, tp;

void dputc(char ch)
{
    if (Tunnel) return;
    if (Pwr.LowVbat) return;
    BSP_USART_SendData(USART1, ch);
}

void InertialSampleTask()
{
    if ((GyroStatus != GYRO_OK) || (AccStatus != ACC_OK)) return;
    L3GD20_Handler();
    LIS3DH_Handler();
    ms.AccX = Acc[0]; ms.AccY = Acc[1]; ms.AccZ = Acc[2];
    ms.AngX = AngRate[0]; ms.AngY = AngRate[1]; ms.AngZ = AngRate[2];
    MyTick = 1;
}

//void AccLog()
//{
//  char LogFilePath[16];
//  uint32_t i, j, bw;
//  FIL accfile;
//
//  if (UsbStat) return;
//
//  sprintf(LogFilePath, "a%07X.txt\0", acccnt);
//
//    if (FR_OK != f_open(&accfile, LogFilePath, FA_WRITE))
//        f_open(&accfile, LogFilePath, FA_CREATE_NEW | FA_WRITE);
//    f_lseek(&accfile, f_size(&accfile));
//    if (osOK == osTimerStop(InertialTimerId)) {
//        for (i = mq_in, j = 0; j < IQ_LEN; j++) {
//          f_write(&accfile, &mq[i], sizeof(mot), &bw);
//          i = (i + 1) % IQ_LEN;
//        }
//        if (osOK == osTimerStart(InertialTimerId, log_period));
//    }
//    f_close(&accfile);
//}

void IncCntFile()
{
    FIL file;
    uint32_t bw;

    if (FR_OK != f_open(&file, "cnt.txt", FA_WRITE | FA_READ)) {
        acccnt = 0;
        f_open(&file, "cnt.txt", FA_CREATE_NEW | FA_WRITE);
        f_lseek(&file, 0);
        f_write(&file, (void*)&acccnt, sizeof(acccnt), &bw);
    }
    else {
        f_lseek(&file, 0);
        f_read(&file, (void *)&acccnt, sizeof(acccnt), &bw);
        acccnt++;
        f_lseek(&file, 0);
        f_write(&file, (void *)&acccnt, sizeof(acccnt), &bw);
    }
    f_close(&file);
}


int main()
{
    BSP_Config();

    poicnt = 0;
    acccnt = 0;
    AccStat = 0;
    UsbStat = 0;
    Tunnel = 0;
    //mq_in = 0;

    // Init file system on SD-card
    FATFS_LinkDriver(&SD_Driver, SDPath);
    f_mount(&SDFatFs, (TCHAR const*)SDPath, 0);

    // Start tasks
    osThreadDef(MainThread, MainThread, osPriorityNormal, 1, 1 * 16 * configMINIMAL_STACK_SIZE);
    osThreadCreate(osThread(MainThread), NULL);

    // Start scheduler
    osKernelStart(NULL, NULL);
    // We should never get here as control is now taken by the scheduler
    while (1);
}

#define ONE_SECOND_US    1000000
static void MainThread(void const *argument)
{
    uint32_t bw, dsec = 0, log_speed_file = 0;
    char LogFilePath[16], NmeaFilePath[16];
    FIL accfile, nmeafile;

    IncCntFile();
    sprintf(LogFilePath, "a%07u.ivs\0", acccnt);
    sprintf(NmeaFilePath, "n%07u.ivs\0", acccnt);

    // Init INS log file
    if (FR_OK != f_open(&accfile, LogFilePath, FA_WRITE))
    f_open(&accfile, LogFilePath, FA_CREATE_NEW | FA_WRITE);
    f_close(&accfile);

        // Init GPS log file
    if (FR_OK != f_open(&nmeafile, NmeaFilePath, FA_WRITE))
    f_open(&nmeafile, NmeaFilePath, FA_CREATE_NEW | FA_WRITE);
    f_close(&nmeafile);

        // Load configuration from SD card
    CFG_LoadConfigFile();
    log_repeat = atoi(CFG_GlobVarsStruct.logRepeat);
    log_period = atoi(CFG_GlobVarsStruct.logPeriod);
    log_speed = atoi(CFG_GlobVarsStruct.logSpeed) * 1000;
    log_timeout = atoi(CFG_GlobVarsStruct.logTimeout) * 1000;
    t1.Init.Period = log_speed;
    HAL_TIM_Base_Init(&t1);

    // Init MSC Application
    USBD_Init(&USBD_Device, &MSC_Desc, 0);
    USBD_RegisterClass(&USBD_Device, &USBD_MSC);
    USBD_MSC_RegisterStorage(&USBD_Device, &USBD_DISK_fops);
    USBD_Start(&USBD_Device);

    memset(mq, 0, sizeof(mq));
    AccStatus = (LIS3DH_StatusTypedef)LIS3DH_Configure();
    GyroStatus = (L3GD20_StatusTypedef)L3GD20_Configure();
    if ((GyroStatus == GYRO_OK) && (AccStatus == ACC_OK)) AccStat = 1;
    GSM_Init();
    USB_Handler();

    //HAL_Delay(log_timeout);

    tp.AccX = 0;
    tp.AngX = 0;
    tp.AccY = 0;
    tp.AngY = 0;
    tp.AccZ = 0;
    tp.AngZ = 0;
    dsec = 0;

        // Open INS log file
    f_open(&accfile, LogFilePath, FA_WRITE);
    f_lseek(&accfile, f_size(&accfile));
    log_speed_file = log_speed / 1000;
    f_write(&accfile, &log_speed_file, sizeof(uint32_t), &bw);

        // Open GPS log file
    f_open(&nmeafile, NmeaFilePath, FA_WRITE);
    f_lseek(&nmeafile, f_size(&nmeafile));

        // Wait for GPS to get fix
    while(!GpsStat.Fix)
    {
        if (DetectPPS())
        {
            GpsStat.Req = true;
        }
        GPS_Handler();
        GSM_Handler();
        USB_Handler();
    }

        nmea[0] = '\0';
    HAL_TIM_Base_Start_IT(&t1);

    while (1)
    {
        if (MyTick)
        {
            MyTick = 0;
            if ((UsbStat == 0) && (log_period*(ONE_SECOND_US / log_speed) > dsec))
            {
                dsec++;
                if (tp.AccX == 0x3FFFFFFF)
                {
                    f_write(&accfile, &tp, sizeof(mot), &bw);
                    tp.AccX = 0;
                    tp.AngX = 0;
                    tp.AccY = 0;
                    tp.AngY = 0;
                    tp.AccZ = 0;
                    tp.AngZ = 0;
                }
                f_write(&accfile, &ms, sizeof(mot), &bw);
            }
            else
            {
                f_close(&accfile);
                f_close(&nmeafile);
                HAL_TIM_Base_Stop(&t1);
            }
            USB_Handler();
        }

        if (DetectPPS())
        {
            GpsStat.Req = true;
            GpsStat.Rdy = false;
            tp.AccX = GPS_FLAG;
            tp.AngX = GPS_FLAG;
            tp.AccY = GPS_FLAG;
            tp.AngY = GPS_FLAG;
            tp.AccZ = GPS_FLAG;
            tp.AngZ = GPS_FLAG;
        }

        GPS_Handler();
        GSM_Handler();

        if (GpsStat.Rdy == true)
        {
            f_write(&nmeafile, &nmea, strlen(nmea), &bw);
            GpsStat.Rdy = false;
        }
    }
}

void USB_Handler()
{
    if (UsbStat == 0) {
        if ((USBD_Device.pClassData != NULL) &&
                ((USBD_Device.dev_state != USBD_STATE_DEFAULT) && (USBD_Device.dev_state != USBD_STATE_SUSPENDED))) {
            UsbStat = 1;
            HAL_Delay(100);
        }
    }
    else { // USB plugged out
        if ((USBD_Device.dev_state == USBD_STATE_DEFAULT) || (USBD_Device.dev_state == USBD_STATE_SUSPENDED)) {
            UsbStat = 0;
            HAL_Delay(100);
            log_repeat = atoi(CFG_GlobVarsStruct.logRepeat);
            log_period = atoi(CFG_GlobVarsStruct.logPeriod);
            log_speed = atoi(CFG_GlobVarsStruct.logSpeed) * 1000;
            log_timeout = atoi(CFG_GlobVarsStruct.logTimeout) * 1000;
            t1.Init.Period = log_speed;
            HAL_TIM_Base_Init(&t1);
            HAL_TIM_Base_Start_IT(&t1);
        }
    }
}


/**
* @brief  Retargets the C library printf function to the USART.
* @param  None
* @retval None
*/
PUTCHAR_PROTOTYPE
{
    // Place your implementation of fputc here
    // e.g. write a character to the GSM_COM1 and Loop until the end of transmission
    //osDelay(1);
    //HAL_UART_Transmit(&huart2, (uint8_t *)&ch, 1, 100);

    BSP_USART_SendData(USART2, (uint16_t)ch);                 // GSM port
    //./BSP_USART_SendData(USART1, (uint16_t)ch);                 // PC port

    return ch;
}

/**
* @brief  Debug print function to the USART.
* @param  None
* @retval None
*/
void DebugPrint(char *pMsg, uint16_t msgSize)
{
    uint16_t i = 0;

    if (!msgSize) {
        while (*pMsg) dputc((uint16_t)*pMsg++);
    }
    else {
        for (i = 0; i < msgSize; i++) dputc((uint16_t)pMsg[i]);
    }
}

uint32_t GetPhrase(char *dst, char eop) {
    uint32_t c, i, j, l;

    __disable_interrupt();
    if(rxBuffIdx) {
        for(i = 0; i < rxBuffIdx; i++) if(AtRxBuffer[i] >= ' ') break;
        for(l = 0; i < rxBuffIdx; l++) {
            dst[l] = AtRxBuffer[i];
            i++;
            if((dst[l] == eop) || (dst[l] == '\r')) break;
        }
        dst[l] = 0;
        c = rxBuffIdx - i;
        if(c != 0) {
            for(j = 0; i < rxBuffIdx; j++, i++) AtRxBuffer[j] = AtRxBuffer[i];
            rxBuffIdx = c;
        }
        else l = 0;
    }
    else l = 0;
    __enable_interrupt();

    return l;
}
