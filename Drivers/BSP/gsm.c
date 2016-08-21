
#include "main.h"

#include "gsm.h"
#include "bsp.h"
#include "at.h"
#include "gps.h"
#include "msd.h"
#include "config.h"

GsmTypedef Gsm = {0};
char IMEI[128];

void GSM_Init(void)
{
    BSP_GSM_Init();         // Configure the MCU peripherals
    BSP_GSM_RST_LOW();      // Hold GSM in reset state
    BSP_GSM_PWR_HIGH();     // Set power pin in order to make the power cycle
    osDelay(20);
    BSP_GSM_RST_HIGH();     // Release GSM reset
    osDelay(20);
    BSP_GSM_PWR_LOW();      // Force a power-on holding PWR_ON signal low for at least 5ms
    osDelay(10);
    BSP_GSM_PWR_HIGH();     // Set power pin in order to make the power cycle
    osDelay(20);

    // Start UART reception: provide the initial buffer pointer the buffer size
    HAL_UART_Receive_IT(&huart2, (uint8_t *)AtRxBuffer, 1);
    osDelay(1000);
    rxBuffIdx = 0;

    GSM_Configure();

    if (Gsm.Cfg && Gsm.Net) {
        if(!Tunnel) dputc('G');
    }
    else {
        if(!Tunnel) dputc('g');
    }
}

void GSM_Configure(void)
{
    *(char*)&Gsm = 0;
    *(char*)&GpsStat = 0;
    if (AT_Attention() != AT_CMD_OK) return;
    Gsm.Pwr = true;
    if (AT_DisableEcho() != AT_CMD_OK) return;
    if (AT_GetIMEI() != AT_CMD_OK) return;
    //if (AT_EnterPin() != AT_CMD_OK) return;
    osDelay(500);
    //if (GSM_ChangeSimSettings() != AT_CMD_OK) return;
    //if (GSM_NetRegistration() != AT_CMD_OK) return;
    //if (AT_EnNetworkInd() != AT_CMD_OK) return;
    //if (At_ClipEnable() != AT_CMD_OK) return;
    //if (At_FtpSetup() != AT_CMD_OK) return;
    //if (At_PsdEnable() == AT_CMD_OK) Gsm.Psd = true;

    if (AT_GpsEnable() != AT_CMD_OK) return;
    GpsStat.Pwr = true;
    if (AT_EnableGRMC() != AT_CMD_OK) return;
    if (AT_EnableGGGA() != AT_CMD_OK) return;
    Gsm.Cfg = true;
}

bool GSM_AudioSetup()
{
    int i;
    for(i = 0; i < 3; i++) {
        AT_ClearBuffer();
        if(AT_CMD_OK != AT_SetAudioPath(CFG_GlobVarsStruct.audioPath)) continue; //return false;
        if(AT_CMD_OK != AT_SetAudioGain(CFG_GlobVarsStruct.audioGain)) continue; //return false;
        if(AT_CMD_OK != AT_SetMicroGain(CFG_GlobVarsStruct.microGain)) continue; //return false;
        if(AT_CMD_OK != AT_SetAudioEcho(CFG_GlobVarsStruct.audioEcho)) continue; //return false;
        return true;
    }
    return false;
}

void GSM_Handler(void)
{
    if(GetPhrase(rxline, '\r')) {
        if(strstr(rxline, "+UGRMC:")||strstr(rxline, "+UGGGA:")){
            char *begin = NULL;
            begin = strstr(rxline, "$");
            if(begin != NULL) {
                if(strlen(begin) <= NMEA_BUFFER_SIZE-2) {
                    strcpy(nmea, begin);
                    GpsStat.Ack = true;
                }
            }
        }
    }
}


char GSM_ChangeSimSettings(void)
{
    if (AT_COPS(AT_COPS_DEREGISTER) != AT_CMD_OK)
    return AT_CMD_ERROR;
    if (AT_UECALLSTAT(AT_UECALLSTAT_NO_RESTRICTION) != AT_CMD_OK)
    return AT_CMD_ERROR;
    return AT_CMD_OK;
}

char GSM_NetRegistration(void)
{
    if (AT_COPS(AT_COPS_AUTOMATIC) != AT_CMD_OK)
    return AT_CMD_ERROR;
    for(uint8_t tmt = 0; tmt < 100; tmt++) {
        osDelay(100);
        if (AT_NetworkStatus() == AT_CMD_OK) return AT_CMD_OK;
    }
    return AT_CMD_TIMEOUT;
}