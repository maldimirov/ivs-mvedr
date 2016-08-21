
#include <string.h>

#include "main.h"
#include "config.h"

FATFS SDFatFs;      // File system object for SD card logical drive
char SDPath[4];     // SD card logical drive path

CFG_GlobVarsTypedef CFG_GlobVarsStruct;
CFG_StatusTypedef CFG_Status;

static const char CFG_ConfigFilePath[] = "config.ini\0";

static const char CFG_KeyPin[] = "pin\0";
static const char CFG_KeyUserNum[] = "user number\0";
static const char CFG_KeyEmergencyNum[] = "emergency number\0";
static const char CFG_KeyMsdId[] = "msd version\0";
static const char CFG_KeyMsgId[] = "message identifier\0";
static const char CFG_KeyVehicleType[] = "vehicle type\0";
static const char CFG_KeyVin[] = "vin\0";
static const char CFG_KeyPropulsion[] = "propulsion system\0";
static const char CFG_KeyPassengers[] = "passengers\0";
static const char CFG_KeyTestCalls[] = "test ecalls\0";
static const char CFG_KeyAudioPath[] = "audio path\0";
static const char CFG_KeyAudioGain[] = "audio gain\0";
static const char CFG_KeyMicroGain[] = "micro gain\0";
static const char CFG_KeyAudioEcho[] = "audio echo\0";
static const char CFG_KeyPoiPeriod[] = "poi period\0";
static const char CFG_KeyGpsPeriod[] = "gps period\0";
static const char CFG_KeyGpsRadius[] = "gps radius\0";
static const char CFG_KeyGpsBeeps[] = "gps beeps\0";
static const char CFG_KeyLogSize[] = "log size\0";
static const char CFG_KeyLogSpeed[] = "log speed\0";
static const char CFG_KeyLogPeriod[] = "log period\0";
static const char CFG_KeyLogRepeat[] = "log repeat\0";
static const char CFG_KeyLogTimeout[] = "log timeout\0";
static const char CFG_KeyLowVoltage[] = "low voltage\0";
static const char CFG_KeyLowBattery[] = "low battery\0";
static const char CFG_KeyAccLevel[] = "acc level\0";

void CFG_LoadConfigFile(void)
{
    CFG_Status = CFG_ReadIniFile(CFG_ConfigFilePath);
    //if(CFG_Status == CONFIG_ERROR) dputc('r');
}

CFG_StatusTypedef CFG_ReadIniFile(const char *aPath)
{
  char lineBuffer[128], *ptr = NULL;          // File read buffer
  CFG_StatusTypedef retVal = CONFIG_ERROR;
  FIL CfgFile;                                // File object
  CFG_KeyValueTypedef mCFG_KeyValueStruct;

  CFG_SetDefaults(&CFG_GlobVarsStruct);
  if(FR_OK == f_open(&CfgFile, aPath, FA_READ)){
    while (!f_eof(&CfgFile)) {
      // Read data from the text file line by line
      ptr = f_gets((char *)lineBuffer, sizeof(lineBuffer), &CfgFile);
      if(ptr == NULL) {
        retVal = CONFIG_ERROR;
        break;
      }
      // Check for comment line
      if (!CFG_VerifyComment(lineBuffer, CFG_CommentPattern)) {
        CFG_StripLineEnd(lineBuffer); // Remove CR or LF at the end of the line
        if (CFG_GetKeyValue(lineBuffer, &mCFG_KeyValueStruct)) {
          CFG_AnalyzeKeyValue(&CFG_GlobVarsStruct, &mCFG_KeyValueStruct);
          retVal++; // Success parsing line with key value pair
        }
      }
    }
    f_close(&CfgFile);
  }
  return retVal;
}

char CFG_GetKeyValue(char *aSrc, CFG_KeyValueTypedef *aCFG_KeyValueStruct)
{
  char *ptr = NULL;
  uint8_t len = 0;

  ptr = strchr(aSrc, '=');

  if (ptr) {
    len = ptr - aSrc;
    strncpy(aCFG_KeyValueStruct->key, aSrc, len);
    aCFG_KeyValueStruct->key[len] = '\0';

    len = strlen(ptr);
    strncpy(aCFG_KeyValueStruct->value, (ptr + 1), len);
    aCFG_KeyValueStruct->value[len] = '\0';

    // Found key and value pair
    return 1;
  }

  // Key and value pair has not been found
  return 0;
}

void CFG_AnalyzeKeyValue(CFG_GlobVarsTypedef *aCFG_GlobVarsStruct, CFG_KeyValueTypedef *aCFG_KeyValueStruct)
{
  if (CFG_VerifyValue(aCFG_KeyValueStruct->key, CFG_KeyPin)) {
    // Found pin key so copy the value to the actual Global varialbe
    strcpy(aCFG_GlobVarsStruct->pin, aCFG_KeyValueStruct->value);
    return ;
  }

  if (CFG_VerifyValue(aCFG_KeyValueStruct->key, CFG_KeyUserNum)) {
    // Found user number key so copy the value to the actual Global varialbe
    strcpy(aCFG_GlobVarsStruct->userNum, aCFG_KeyValueStruct->value);
    return ;
  }

  if (CFG_VerifyValue(aCFG_KeyValueStruct->key, CFG_KeyEmergencyNum)) {
    // Found emergency number key so copy the value to the actual Global varialbe
    strcpy(aCFG_GlobVarsStruct->emergencyNum, aCFG_KeyValueStruct->value);
    return ;
  }

  if (CFG_VerifyValue(aCFG_KeyValueStruct->key, CFG_KeyMsdId)) {
    // Found msd ID key so copy the value to the actual Global varialbe
    strcpy(aCFG_GlobVarsStruct->msdId, aCFG_KeyValueStruct->value);
    return ;
  }

  if (CFG_VerifyValue(aCFG_KeyValueStruct->key, CFG_KeyMsgId)) {
    // Found msg ID key so copy the value to the actual Global varialbe
    strcpy(aCFG_GlobVarsStruct->msgId, aCFG_KeyValueStruct->value);
    return ;
  }

  if (CFG_VerifyValue(aCFG_KeyValueStruct->key, CFG_KeyVehicleType)) {
    // Found vehicle type key so copy the value to the actual Global varialbe
    strcpy(aCFG_GlobVarsStruct->vehicleType, aCFG_KeyValueStruct->value);
    return ;
  }

  if (CFG_VerifyValue(aCFG_KeyValueStruct->key, CFG_KeyVin)) {
    // Found VIN key so copy the value to the actual Global varialbe
    strcpy(aCFG_GlobVarsStruct->vin, aCFG_KeyValueStruct->value);
    return ;
  }

  if (CFG_VerifyValue(aCFG_KeyValueStruct->key, CFG_KeyPropulsion)) {
    // Found vehicle propulsion system key so copy the value to the actual Global varialbe
    strcpy(aCFG_GlobVarsStruct->propulsion, aCFG_KeyValueStruct->value);
    return ;
  }

  if (CFG_VerifyValue(aCFG_KeyValueStruct->key, CFG_KeyPassengers)) {
    // Found passangers number key so copy the value to the actual Global varialbe
    strcpy(aCFG_GlobVarsStruct->passengers, aCFG_KeyValueStruct->value);
    return ;
  }

  if (CFG_VerifyValue(aCFG_KeyValueStruct->key, CFG_KeyTestCalls)) {
    // Found test eCalls key so copy the value to the actual Global varialbe
    strcpy(aCFG_GlobVarsStruct->testCall, aCFG_KeyValueStruct->value);
    return ;
  }

  if (CFG_VerifyValue(aCFG_KeyValueStruct->key, CFG_KeyAudioPath)) {
    // Found test eCalls key so copy the value to the actual Global varialbe
    strcpy(aCFG_GlobVarsStruct->audioPath, aCFG_KeyValueStruct->value);
    return ;
  }
  //........
  if (CFG_VerifyValue(aCFG_KeyValueStruct->key, CFG_KeyAudioGain)) {
    // Found test eCalls key so copy the value to the actual Global varialbe
    strcpy(aCFG_GlobVarsStruct->audioGain, aCFG_KeyValueStruct->value);
    return ;
  }

  if (CFG_VerifyValue(aCFG_KeyValueStruct->key, CFG_KeyMicroGain)) {
    // Found test eCalls key so copy the value to the actual Global varialbe
    strcpy(aCFG_GlobVarsStruct->microGain, aCFG_KeyValueStruct->value);
    return ;
  }

  if (CFG_VerifyValue(aCFG_KeyValueStruct->key, CFG_KeyAudioEcho)) {
    // Found test eCalls key so copy the value to the actual Global varialbe
    strcpy(aCFG_GlobVarsStruct->audioEcho, aCFG_KeyValueStruct->value);
    return ;
  }

  if (CFG_VerifyValue(aCFG_KeyValueStruct->key, CFG_KeyPoiPeriod)) {
    // Found test eCalls key so copy the value to the actual Global varialbe
    strcpy(aCFG_GlobVarsStruct->poiPeriod, aCFG_KeyValueStruct->value);
    return ;
  }

  if (CFG_VerifyValue(aCFG_KeyValueStruct->key, CFG_KeyGpsPeriod)) {
    // Found test eCalls key so copy the value to the actual Global varialbe
    strcpy(aCFG_GlobVarsStruct->gpsPeriod, aCFG_KeyValueStruct->value);
    return ;
  }

  if (CFG_VerifyValue(aCFG_KeyValueStruct->key, CFG_KeyGpsRadius)) {
    // Found test eCalls key so copy the value to the actual Global varialbe
    strcpy(aCFG_GlobVarsStruct->gpsRadius, aCFG_KeyValueStruct->value);
    return ;
  }

  if (CFG_VerifyValue(aCFG_KeyValueStruct->key, CFG_KeyGpsBeeps)) {
    // Found test eCalls key so copy the value to the actual Global varialbe
    strcpy(aCFG_GlobVarsStruct->gpsBeeps, aCFG_KeyValueStruct->value);
    return ;
  }

  if (CFG_VerifyValue(aCFG_KeyValueStruct->key, CFG_KeyLogSize)) {
    // Found test eCalls key so copy the value to the actual Global varialbe
    strcpy(aCFG_GlobVarsStruct->logSize, aCFG_KeyValueStruct->value);
    return ;
  }

  if (CFG_VerifyValue(aCFG_KeyValueStruct->key, CFG_KeyLogSpeed)) {
    // Found test eCalls key so copy the value to the actual Global varialbe
    strcpy(aCFG_GlobVarsStruct->logSpeed, aCFG_KeyValueStruct->value);
    return ;
  }

  if (CFG_VerifyValue(aCFG_KeyValueStruct->key, CFG_KeyLogPeriod)) {
    // Found test eCalls key so copy the value to the actual Global varialbe
    strcpy(aCFG_GlobVarsStruct->logPeriod, aCFG_KeyValueStruct->value);
    return ;
  }
  
  if (CFG_VerifyValue(aCFG_KeyValueStruct->key, CFG_KeyLogRepeat)) {
    // Found test eCalls key so copy the value to the actual Global varialbe
    strcpy(aCFG_GlobVarsStruct->logRepeat, aCFG_KeyValueStruct->value);
    return ;
  }

  if (CFG_VerifyValue(aCFG_KeyValueStruct->key, CFG_KeyLogTimeout)) {
      // Found test eCalls key so copy the value to the actual Global varialbe
      strcpy(aCFG_GlobVarsStruct->logTimeout, aCFG_KeyValueStruct->value);
      return;
  }

  if (CFG_VerifyValue(aCFG_KeyValueStruct->key, CFG_KeyLowVoltage)) {
    // Found test eCalls key so copy the value to the actual Global varialbe
    strcpy(aCFG_GlobVarsStruct->lowVoltage, aCFG_KeyValueStruct->value);
    return ;
  }

  if (CFG_VerifyValue(aCFG_KeyValueStruct->key, CFG_KeyLowBattery)) {
    // Found test eCalls key so copy the value to the actual Global varialbe
    strcpy(aCFG_GlobVarsStruct->lowBattery, aCFG_KeyValueStruct->value);
    return ;
  }

  if (CFG_VerifyValue(aCFG_KeyValueStruct->key, CFG_KeyAccLevel)) {
    // Found test eCalls key so copy the value to the actual Global varialbe
    strcpy(aCFG_GlobVarsStruct->accLevel, aCFG_KeyValueStruct->value);
    return ;
  }
}

char CFG_VerifyValue(char *aStr, const char *aKey)
{
  if (strstr(aStr, aKey)) {
    // Key matched
    return 1;
  }

  // Key not matched
  return 0;
}

char CFG_VerifyComment(char *aStr, const char *aPattern)
{
  if (strstr(aStr, aPattern)) {
    // Found comment pattern
    return 1;
  }
  // Comment pattern has not been found
  return 0;
}

void CFG_StripLineEnd(char *aStr)
{
  uint8_t len = 0;

  len = strlen(aStr);

  if (aStr[len - 1] == '\n' || aStr[len - 1] == '\r') {
    aStr[len - 1] = '\0';
  }
}

void CFG_SetDefaults(CFG_GlobVarsTypedef *aCFG_GlobVarsStruct)
{
  strcpy(aCFG_GlobVarsStruct->pin, "0000\0");
  strcpy(aCFG_GlobVarsStruct->userNum, "0882795485\0");
  strcpy(aCFG_GlobVarsStruct->emergencyNum, "112\0");

  strcpy(aCFG_GlobVarsStruct->msdId, "1\0");
  strcpy(aCFG_GlobVarsStruct->msgId, "1\0");

  strcpy(aCFG_GlobVarsStruct->vehicleType, "0\0");
  strcpy(aCFG_GlobVarsStruct->vin, "WVWZZZ1HZPW463928\0");
  strcpy(aCFG_GlobVarsStruct->propulsion, "16\0");
  strcpy(aCFG_GlobVarsStruct->passengers, "1\0");
  strcpy(aCFG_GlobVarsStruct->testCall, "0\0");
  strcpy(aCFG_GlobVarsStruct->audioPath, "0,3,0,0,0\0");
  strcpy(aCFG_GlobVarsStruct->audioGain, "3,5,5,16384,16384,16384\0");
  strcpy(aCFG_GlobVarsStruct->microGain, "0,13,24000\0");
  strcpy(aCFG_GlobVarsStruct->audioEcho, "0,0x01F0,0x0160,30000,250,3,2,-300,0,512,512,24576,16384,16384\0");

  strcpy(aCFG_GlobVarsStruct->poiPeriod, "5\0");
  strcpy(aCFG_GlobVarsStruct->gpsPeriod, "5\0");
  strcpy(aCFG_GlobVarsStruct->gpsRadius, "50\0");
  strcpy(aCFG_GlobVarsStruct->gpsBeeps, "5\0");

  strcpy(aCFG_GlobVarsStruct->logSize, "1024\0");
  strcpy(aCFG_GlobVarsStruct->logSpeed, "5\0");
  strcpy(aCFG_GlobVarsStruct->logPeriod, "10\0");
  strcpy(aCFG_GlobVarsStruct->logTimeout, "2\0");

  strcpy(aCFG_GlobVarsStruct->lowVoltage, "10800\0");
  strcpy(aCFG_GlobVarsStruct->lowBattery, "3750\0");

  strcpy(aCFG_GlobVarsStruct->accLevel, "2000\0");
}

