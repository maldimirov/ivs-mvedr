
#ifndef __GSM_H
#define __GSM_H

#include <stdbool.h>

typedef enum {
  GSM_NOT_PRESENT = 0,        // GSM not present Attention command timeout occurred
  GSM_PRESENT,                // GSM present and answered Ok to attention command
  GSM_CONFIGURED,
  GSM_NOT_CONFIGURED,
  GSM_SIM_PRESENT,            // SIM card present
  GSM_SIM_NOT_PRESENT,        // SIM card not present
  GSM_PIN_ENABLED,            // PIN on SIM card enabled
  GSM_PIN_NOT_ENABLED,        // PIN on SIM card not enabled
  GSM_NETWORK_REGISTRED,      // GSM network registred
  GSM_NETWORK_NOT_REGISTRED,  // GSM network not registred
} GSM_StatusTypedef;

typedef enum {
  ECALL_ERROR_OK = 0, 
  ECALL_NOT_POSSIBLE,
  ECALL_ERROR_SET_TYPE,
  ECALL_ERROR_STAT,
  ECALL_ERROR_DATA,
  ECALL_ERROR_DIAL,
} ECall_ErrorTypedef;

typedef struct {
  unsigned char Pwr     : 1;
  unsigned char Net     : 1;
  unsigned char Cfg     : 1;
  unsigned char mCall   : 1;
  unsigned char aCall   : 1;
  unsigned char eCall   : 1;
  unsigned char Psd     : 1;
  unsigned char Ftp     : 1;
} GsmTypedef;

extern GsmTypedef Gsm;
extern char IMEI[128];

void    GSM_Init      (void);
void    GSM_Configure (void);
void    GSM_Handler   (void);
char    GSM_eCall     (void);
char    GSM_eCall_785 (void);
char    GSM_ChangeSimSettings(void);
char    GSM_NetRegistration(void);
bool    GSM_AudioSetup();

#endif
