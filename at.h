
#ifndef __AT_H
#define __AT_H

#define AT_RX_BUFFER_SIZE    2048

#define AT_COPS_AUTOMATIC     0
#define AT_COPS_DEREGISTER    2

#define AT_UECALLSTAT_RESTRICTION       1
#define AT_UECALLSTAT_NO_RESTRICTION    2

typedef enum {
  AT_CMD_OK = 0,      // AT command response ends with OK
  AT_CMD_ERROR,       // AT command response ends with ERROR
  AT_CMD_WARNING,     // AT command response pending
  AT_CMD_TIMEOUT,     // AT command reponse timeout
} AT_CmdErrorTypedef;

typedef enum {
  AT_STATUS_IDLE = 0,           // Ready to accept data or command for transfer
  AT_STATUS_BUSY,               // Busy, transfer in progress
  AT_STATUS_PENDING,            // Data or command pending, waiting for response
  AT_STATUS_LINE_RECEIVED,      // Received line, so check if it last
  AT_STATUS_URC,                // Receiving unsolicited result code
  AT_STATUS_PARSE,              // Parse message
  AT_STATUS_TIMEOUT             // Communication timeout, no response received
} AT_StatusTypedef;

extern char AtRxBuffer[AT_RX_BUFFER_SIZE+2];

extern volatile uint32_t rxBuffIdx;
extern volatile AT_StatusTypedef AT_Status;

char      AT_CheckLastLine  (void);
void      AT_ClearBuffer    (void);
char      AT_GetIMEI   (void);
char      AT_Attention      (void);
char      AT_DisableEcho    (void);
char      AT_EnterPin       (void);
char      AT_NetworkStatus  (void);
char      AT_EnNetworkInd   (void);
char      AT_SetAudioPath   (char *settings);
char      AT_SetAudioGain   (char *settings);
char      AT_SetMicroGain   (char *settings);
char      AT_SetAudioEcho   (char *settings);

char      AT_GpsEnable      (void);
char      AT_EnableGRMC     (void);
char      AT_EnableGGGA     (void);
char      AT_GetGRMC        (char *nmea_dest);

char      AT_COPS           (char mode);
char      AT_UECALLVOICE    (char *cmd);
char      AT_UECALLSTAT     (char cmd);
char      AT_UECALLDATA     (char op, char param1, char *pData);

char      AT_InBandStat     (void);
char      AT_eCallType      (char eCallType);
char      AT_InBandData     (char *pData);
char      AT_Dial           (void);
char      AT_HangUp         (void);
char      At_ClipEnable     (void);
char      AT_PowerOff       (void);

char At_PsdEnable(void);
char At_FtpSetup(void);

#endif
