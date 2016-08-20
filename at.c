
#include <stdio.h>
#include <string.h>

#include "main.h"
#include "at.h"
#include "config.h"
#include "gsm.h"
#include "gps.h"

char AtRxBuffer[AT_RX_BUFFER_SIZE+2];

// Increment this pointer or roll it back to start address when data are received over USART
volatile uint32_t rxBuffIdx = 0;

volatile AT_StatusTypedef AT_Status = AT_STATUS_IDLE;

char AT_Attention(void)
{
  char retVal;
  printf("AT\r");
  osDelay(500);
  retVal = AT_CheckLastLine();
  AT_ClearBuffer();
  return retVal;
}

char AT_DisableEcho(void)
{
  char retVal;
  printf("ATE0\r");
  osDelay(200);
  retVal = AT_CheckLastLine();
  AT_ClearBuffer();
  return retVal;
}

char AT_EnterPin(void)
{
  char retVal;
  printf("AT+CPIN=\"%s\"\r", CFG_GlobVarsStruct.pin);
  osDelay(300);
  retVal = AT_CheckLastLine();
  AT_ClearBuffer();
  return retVal;
}

char AT_GetIMEI(void)
{
  char retVal;
  printf("AT+CGSN\r");
  osDelay(200);
  if((retVal = AT_CheckLastLine()) == AT_CMD_OK) {
    if(GetPhrase(IMEI, '\r')) retVal = AT_CMD_OK;
  }
  AT_ClearBuffer();  
  return retVal;
}

char AT_NetworkStatus(void)
{
  char retVal = 0;

  printf("AT+CREG?\r");
  osDelay(200);
  retVal = AT_CheckLastLine();
  if (retVal == AT_CMD_OK) {
    if (strstr((char *)AtRxBuffer, "0,1")) Gsm.Net = true;
    else Gsm.Net = false;   
  }
   AT_ClearBuffer();

  return retVal;
}

char AT_GpsEnable(void)
{
  char retVal = 0;

  printf("AT+UGPS=1,1\r");

  osDelay(5000);

  retVal = AT_CheckLastLine();

  if (retVal == AT_CMD_OK) AT_ClearBuffer();

  return retVal;
}

char AT_EnableGRMC(void)
{
  char retVal = 0;

  printf("AT+UGRMC=1\r");

  osDelay(200);

  retVal = AT_CheckLastLine();

  if (retVal == AT_CMD_OK) {
    AT_ClearBuffer();
    return retVal;
  }

  return retVal;
}

char AT_EnableGGGA(void)
{
  char retVal = 0;

  printf("AT+UGGGA=1\r");

  osDelay(200);

  retVal = AT_CheckLastLine();

  if (retVal == AT_CMD_OK) {
    AT_ClearBuffer();
    return retVal;
  }

  return retVal;
}

//char AT_GetGRMC(char *nmea_dest)
//{
//  char *begin = NULL, *end = NULL;
//  char retVal = 0;
//
//  printf("AT+UGRMC?\r");
//
//  osDelay(200);
//
//  retVal = AT_CheckLastLine();
//
//  if (retVal == AT_CMD_OK) {
//    begin = strstr(AtRxBuffer, "$");          // Find begin of the NMEA RMC message
//    end = strstr(begin, "\r\n");              // Find end of the NMEA RMC message    
//    // Does the NMEA message exceed buffer size
//    if((end == NULL) || (begin == NULL))
//      return AT_CMD_ERROR;
//    if (end - begin > NMEA_BUFFER_SIZE-2) {
//      return AT_CMD_ERROR;
//    }
//    memset(nmea_dest, '\0', NMEA_BUFFER_SIZE);     // Clear NMEA message buffer
//    strncpy(nmea_dest, begin, end - begin);        // Copy NMEA message to the NMEA parser buffer
//    AT_ClearBuffer();                         // Delete AT+RMC? response
//  }
//
//  return retVal;
//}

char AT_InBandStat(void)
{
  char retVal = 0;

  printf("AT+UIBMSTAT=2,1\r");

  osDelay(200);

  retVal = AT_CheckLastLine();

  if (retVal == AT_CMD_OK) {
    AT_ClearBuffer();
    return retVal;
  }

  return retVal;
}

char AT_InBandData(char *pData)
{
  char retVal = 0;

  printf("AT+UIBMDATA=1,1,\"%s\"\r", pData);

  osDelay(200);

  retVal = AT_CheckLastLine();

  if (retVal == AT_CMD_OK) {
    AT_ClearBuffer();
    return retVal;
  }

  return retVal;
}

char AT_Dial()
{
  char retVal = 0;

  printf("ATD%s;\r", CFG_GlobVarsStruct.emergencyNum);

  osDelay(2000);

  retVal = AT_CheckLastLine();

  if (retVal == AT_CMD_OK) {
    AT_ClearBuffer();
    return retVal;
  }

  return retVal;
}

char AT_EnNetworkInd(void)
{
  char retVal = 0;

  printf("AT+UGPIOC=20,2\r");

  osDelay(200);

  retVal = AT_CheckLastLine();

  if (retVal == AT_CMD_OK) {
    AT_ClearBuffer();
    return retVal;
  }

  return retVal;
}


char AT_PowerOff(void)
{
  char retVal;  
  printf("AT+CPWROFF\r");
  osDelay(1000);
  retVal = AT_CheckLastLine();
  AT_ClearBuffer();
  return retVal;
}


#define AUDIO_TIMEOUT   100
char AT_SetAudioPath(char *settings)
{
  char retVal;
  printf("AT+USPM=%s\r", settings);      // Connector P4, SPK_P, SPK_N and MIC1
  osDelay(AUDIO_TIMEOUT);
  retVal = AT_CheckLastLine();
  AT_ClearBuffer();
  return retVal;
}

char AT_SetAudioGain(char *settings)
{
  char retVal;  
  printf("AT+USGC=%s\r", settings);
  osDelay(AUDIO_TIMEOUT);
  retVal = AT_CheckLastLine();
  AT_ClearBuffer();
  return retVal;
}

char AT_SetAudioEcho(char *settings)
{
  char retVal;
  printf("AT+UHFP=%s\r", settings);
  osDelay(AUDIO_TIMEOUT);
  retVal = AT_CheckLastLine();
  AT_ClearBuffer();
  return retVal;
}

char AT_SetMicroGain(char *settings)
{
  char retVal;
  printf("AT+UMGC=%s\r", settings);
  osDelay(AUDIO_TIMEOUT);
  retVal = AT_CheckLastLine();
  AT_ClearBuffer();
  return retVal;
}


char AT_COPS(char mode)
{
  char retVal = 0, timeout = 0;

  printf("AT+COPS=%d\r", mode);  

  do {
    osDelay(200);
    retVal = AT_CheckLastLine();
    timeout++;
  } while (retVal == AT_CMD_WARNING && timeout < 100);

  if (retVal == AT_CMD_OK) {
    AT_ClearBuffer();
    return retVal;
  }

  return retVal;
}


char AT_UECALLVOICE(char *cmd)
{
  char retVal = 0;

  osDelay(100);

  printf("AT+UECALLVOICE=%s\r", cmd);

  osDelay(200);

  retVal = AT_CheckLastLine();

  if (retVal == AT_CMD_OK) {
    AT_ClearBuffer();
    return retVal;
  }

  return retVal;
}


char AT_UECALLSTAT(char cmd)
{
  char retVal = 0;

  osDelay(100);

  printf("AT+UECALLSTAT=%d\r", cmd);

  osDelay(200);

  retVal = AT_CheckLastLine();

  if (retVal == AT_CMD_OK) {
    AT_ClearBuffer();
    return retVal;
  }

  return retVal;
}

char AT_UECALLDATA(char op, char param1, char *pData)
{
  char retVal = 0;

  if (pData == NULL) {
    //if(op == 0) 
    //  printf("AT+UECALLDATA=0\r");
    //else 
      printf("AT+UECALLDATA=%d,%d\r", op, param1);
  } else {
    printf("AT+UECALLDATA=%d,%d,\"%s\"\r", op, param1, pData);
  }

  osDelay(200);

  retVal = AT_CheckLastLine();

  if (retVal == AT_CMD_OK) {
    AT_ClearBuffer();
    return retVal;
  }

  return retVal;
}

char AT_HangUp(void)
{
  char retVal = 0;

  printf("AT+CHUP\r");                // Hang-up
  osDelay(500);
  retVal = AT_CheckLastLine();
  AT_ClearBuffer();
  
  return retVal;
}

char At_ClipEnable(void)
{
  char retVal;

  printf("AT+CLIP=1\r");
  osDelay(500);
  retVal = AT_CheckLastLine();  
  AT_ClearBuffer();
  return retVal;
}

char At_PsdEnable(void)
{
  char retVal;
  printf("AT+UPSDA=0,3\r");
  osDelay(2000);  
  retVal = AT_CheckLastLine();
  AT_ClearBuffer();
  return retVal;
}

char At_FtpSetup(void)
{
  char retVal = 0;

  printf("AT+UFTP=0,\"212.72.194.18\"\r");
  osDelay(100);
  if(AT_CMD_OK != (retVal = AT_CheckLastLine())){
    AT_ClearBuffer();
    return retVal;
  }
  AT_ClearBuffer();
  
  printf("AT+UFTP=2,\"ivsftp\"\r");
  osDelay(100);
  if(AT_CMD_OK != (retVal = AT_CheckLastLine())){
    AT_ClearBuffer();
    return retVal;
  }
  AT_ClearBuffer();
  
  printf("AT+UFTP=3,\"w@^&!J=x^8YWrwR\"\r");
  osDelay(100);
  if(AT_CMD_OK != (retVal = AT_CheckLastLine())){
    AT_ClearBuffer();
    return retVal;
  }
  AT_ClearBuffer();
  
  printf("AT+UFTP=5,90\r"); // No FTP session timeout termination
  osDelay(100);
  if(AT_CMD_OK != (retVal = AT_CheckLastLine())){
    AT_ClearBuffer();
    return retVal;
  }
  AT_ClearBuffer();
  
  printf("AT+UFTP=6,1\r"); // FTP passive mode
  osDelay(100);
  if(AT_CMD_OK != (retVal = AT_CheckLastLine())){
    AT_ClearBuffer();
    return retVal;
  }
  AT_ClearBuffer();
  
  return retVal;
}

char AT_CheckLastLine(void)
{
  __disable_interrupt();
  if (strstr(AtRxBuffer, "OK")) {
    __enable_interrupt();
    return AT_CMD_OK;
  } else if (strstr((char *)AtRxBuffer, "ERROR")) {
    __enable_interrupt();
    return AT_CMD_ERROR;
  }
  __enable_interrupt();
  return AT_CMD_WARNING;
}

void AT_ClearBuffer(void)
{  
  __disable_interrupt();
  memset(AtRxBuffer, '\0', AT_RX_BUFFER_SIZE+2);
  rxBuffIdx = 0;
  __enable_interrupt();
}
