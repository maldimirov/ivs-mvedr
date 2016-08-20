
#ifndef __CONFIG_H
#define __CONFIG_H

typedef struct {
  char key[32];
  char value[32];                
} CFG_KeyValueTypedef;

typedef struct {
  char pin[5];
  char userNum[16];
  char emergencyNum[16];

  char msdId[4];
  char msgId[4];

  char vehicleType[3];
  char vin[18];
  char propulsion[3];
  char passengers[4];
  char testCall[2];
  char audioPath[12];
  char audioGain[40];
  char microGain[40];
  char audioEcho[80];
  char poiPeriod[12];
  char gpsPeriod[12];
  char gpsRadius[6];
  char gpsBeeps[6];
  char logSize[11];
  char logSpeed[6];
  char logPeriod[7];
  char logRepeat[7];
  char logTimeout[7];
  char lowVoltage[7];
  char lowBattery[7];
  char accLevel[12];
} CFG_GlobVarsTypedef;

typedef enum {
  CONFIG_ERROR = 0,
  CONFIG_OK
} CFG_StatusTypedef;

static const char CFG_CommentPattern[] = "//\0";
extern CFG_GlobVarsTypedef CFG_GlobVarsStruct;
extern CFG_StatusTypedef CFG_Status;

void      CFG_LoadConfigFile  (void);
void      CFG_SetDefaults     (CFG_GlobVarsTypedef *CFG_GlobVarsStruct);

CFG_StatusTypedef CFG_ReadIniFile     (const char *aPath);

char      CFG_GetKeyValue     (char *aSrc, CFG_KeyValueTypedef *CFG_KeyValueStruct);
char      CFG_VerifyComment   (char *aStr, const char *aPattern);
char      CFG_VerifyValue     (char *aStr, const char *aKey);
void      CFG_StripLineEnd    (char *aStr);

void      CFG_AnalyzeKeyValue (CFG_GlobVarsTypedef *aCFG_GlobVarsStruct, CFG_KeyValueTypedef *aCFG_KeyValueStruct);

void      CFG_ErrorHandler    (void);

#endif
