
#include <stdlib.h>
#include <string.h>

#include "main.h"

#include "msd.h"
#include "gps.h"
#include "gsm.h"
#include "config.h"

unsigned char msd[MSD_DATA_SIZE];
char msdAscii[MSD_DATA_ASCII];

MSD_DataTypedef MSD_Data;
static MSD_GpsPosTypedef pos = {512, 512};

void MSD_InitData(char aGpsStatus)
{
  unsigned char ch = 0, i = 0;
  unsigned int tmp = 0;

  MSD_Data.msdId = atoi(CFG_GlobVarsStruct.msdId);    // MSD format version 

  MSD_Data.MSD_Control.extraData = 0x00;              // Additional information not included
  
  // aGpsStatus
  MSD_Data.MSD_Control.recentPos1 = 0x01;             // Recent vehicle position 1 available
  MSD_Data.MSD_Control.recentPos2 = 0x01;             // Recent vehicle position 2 available

  // Check passangers number
  tmp = atoi(CFG_GlobVarsStruct.passengers);

  if (tmp >= 1) {
    MSD_Data.MSD_Control.passengers = 0x01;           // Passengers number available
    MSD_Data.passengers = tmp;
  } else {
    MSD_Data.MSD_Control.passengers = 0x00;           // Passengers number not available
    MSD_Data.passengers = 0x00;
  }
  
  tmp = atoi(CFG_GlobVarsStruct.msgId);    // Message ID
  if(tmp) 
    MSD_Data.msgId = tmp;
  else 
    MSD_Data.msgId++;
  //MSD_Data.msgId = atoi(CFG_GlobVarsStruct.msgId);    // Message ID

  //MSD_Data.MSD_Control.activation = 0x01;             // Automatic or manual eCall activation
  MSD_Data.MSD_Control.activation = eCall;            // Automatic or manual eCall activation

  // Test eCall
  MSD_Data.MSD_Control.testCall = atoi(CFG_GlobVarsStruct.testCall);

  if (aGpsStatus) {
    MSD_Data.MSD_Control.posStatus = 0x01;              // True if position is valid
  } else {
    MSD_Data.MSD_Control.posStatus = 0x00;              // False if position is not valid
  }

  MSD_Data.MSD_Control.vehicleType = atoi(CFG_GlobVarsStruct.vehicleType);

  // Set VIN
  for (i = 0; i < 17; i++)
  {
    ch = CFG_GlobVarsStruct.vin[i];
    if (ch >= '0' && ch <= '9') {     
      ch -= 0x30;     // Numbers are encoded with valuese 0x00 to 0x09
    } else if (ch >= 'A' && ch <= 'H') {
      ch -= 0x37;
    } else if (ch >= 'J' && ch <= 'N') {
      ch -= 0x38;
    } else if (ch == 'P') {
      ch = 0x17;
    } else if (ch >= 'R' && ch <= 'Z') {
      ch -= 0x3A;
    }

    MSD_Data.vin[i] = ch;
  }
  
  MSD_Data.MSD_Control.includedTanks = 0x3F;          // Enable all tank type to be transmitted
  // Set propulsion system storage
  MSD_Data.propulsion = atoi(CFG_GlobVarsStruct.propulsion);

  if (aGpsStatus) {
    ConvertTimeToEpoch(rmc.utc, rmc.dat, &MSD_Data.timestamp);
    ConvertDecToMilliArcSec(rmc.lat, &MSD_Data.pos.lat, NULL);
    ConvertDecToMilliArcSec(rmc.lon, &MSD_Data.pos.lon, NULL);

    MSD_Data.pos.lat += 0x80000000;                   // Make offset
    MSD_Data.pos.lon += 0x80000000;

    if (rmc.cog[0] != '\0') {
      MSD_Data.direction = (uint8_t)(atoi(rmc.cog) / 2);
    } else {
      MSD_Data.direction = 0xFF;
    }

    MSD_CalcPosDelta();
  } 
  else { // In case we don't have valid GPS data set defaults as described in EN15722
    
    MSD_Data.timestamp = 0x00;
    MSD_Data.pos.lat = 0x7FFFFFFF;
    MSD_Data.pos.lon = 0x7FFFFFFF;
    MSD_Data.direction = 0xFF;

    MSD_Data.MSD_PosDelta[0].lat = 512;
    MSD_Data.MSD_PosDelta[0].lon = 512;

    MSD_Data.MSD_PosDelta[1].lat = 512;
    MSD_Data.MSD_PosDelta[1].lon = 512;
  }
}

void MSD_PrepareData(void)
{
  if (GpsStat.Fix) {
    MSD_InitData(1);
  } else {
    MSD_InitData(0);
  }

  memset(msd, '\0', MSD_DATA_SIZE);     // Clear NMEA message buffer

  msd[0] = MSD_Data.msdId;                              // 8 bit size, pos xxxx xxxx

  msd[1] |= (MSD_Data.MSD_Control.extraData << 5);      // 3 bit size, pos xxx0 0000
  msd[1] |= (MSD_Data.MSD_Control.recentPos1 << 4);     // 1 bit size, pos 000x 0000
  msd[1] |= (MSD_Data.MSD_Control.recentPos2 << 3);     // 1 bit size, pos 0000 x000
  msd[1] |= (MSD_Data.MSD_Control.passengers << 2);     // 1 bit size, pos 0000 0x00
  msd[1] |= ((MSD_Data.msgId >> 6) & 0x03);             // 8 bit size, pos 0000 00xx

  msd[2] |= (MSD_Data.msgId << 2);                      // 8 bit size, pos xxxx xx00    
  msd[2] |= MSD_Data.MSD_Control.activation << 1;       // 1 bit size, pos 0000 00x0
  msd[2] |= MSD_Data.MSD_Control.testCall;              // 1 bit size, pos 0000 000x

  msd[3] |= MSD_Data.MSD_Control.posStatus << 7;        // 1 bit size, pos x000 0000
  msd[3] |= MSD_Data.MSD_Control.vehicleType << 2;      // 5 bit size, pos 0xxx xx00
  msd[3] |= ((MSD_Data.vin[0] >> 4) & 0x03);            // 6 bit size, pos 0000 00xx
  
  msd[4] |= (MSD_Data.vin[0] << 4);                     // 6 bit size, pos xxxx 0000
  msd[4] |= ((MSD_Data.vin[1] >> 2) & 0x0F);            // 6 bit size, pos 0000 xxxx
  
  msd[5] |= MSD_Data.vin[1] << 6;                       // 6 bit size, pos xx00 0000
  msd[5] |= MSD_Data.vin[2];                            // 6 bit size, pos 00xx xxxx
  
  msd[6] |= MSD_Data.vin[3] << 2;                       // 6 bit size, pos xxxx xx00
  msd[6] |= ((MSD_Data.vin[4] >> 4) & 0x03);            // 6 bit size, pos 0000 00xx
  
  msd[7] |= MSD_Data.vin[4] << 4;                       // 6 bit size, pos xxxx 0000
  msd[7] |= ((MSD_Data.vin[5] >> 2) & 0x0F);            // 6 bit size, pos 0000 xxxx
  
  msd[8] |= MSD_Data.vin[5] << 6;                       // 6 bit size, pos xx00 0000
  msd[8] |= MSD_Data.vin[6];                            // 6 bit size, pos 00xx xxxx
  
  msd[9] |= MSD_Data.vin[7] << 2;                       // 6 bit size, pos xxxx xx00
  msd[9] |= ((MSD_Data.vin[8] >> 4) & 0x03);            // 6 bit size, pos 0000 00xx
  
  msd[10] |= MSD_Data.vin[8] << 4;                      // 6 bit size, pos xxxx 0000
  msd[10] |= ((MSD_Data.vin[9] >> 2) & 0x0F);           // 6 bit size, pos 0000 xxxx
  
  msd[11] |= MSD_Data.vin[9] << 6;                      // 6 bit size, pos xx00 0000
  msd[11] |= MSD_Data.vin[10];                          // 6 bit size, pos 00xx xxxx
  
  msd[12] |= MSD_Data.vin[11] << 2;                     // 6 bit size, pos xxxx xx00
  msd[12] |= ((MSD_Data.vin[12] >> 4) & 0x03);          // 6 bit size, pos 0000 00xx
  
  msd[13] |= MSD_Data.vin[12] << 4;                     // 6 bit size, pos xxxx 0000
  msd[13] |= ((MSD_Data.vin[13] >> 2) & 0x0F);          // 6 bit size, pos 0000 xxxx
  
  msd[14] |= MSD_Data.vin[13] << 6;                     // 6 bit size, pos xx00 0000
  msd[14] |= MSD_Data.vin[14];                          // 6 bit size, pos 00xx xxxx
  
  msd[15] |= MSD_Data.vin[15] << 2;                     // 6 bit size, pos xxxx xx00
  msd[15] |= ((MSD_Data.vin[16] >> 4) & 0x03);          // 6 bit size, pos 0000 00xx
  
  msd[16] |= MSD_Data.vin[16] << 4;                     // 6 bit size, pos xxxx 0000
  msd[16] |= ((MSD_Data.MSD_Control.includedTanks >> 3) & 0x0F);  // 7 bit size, pos 0000 xxxx
  
  msd[17] |= MSD_Data.MSD_Control.includedTanks << 5;   // 7 bit size, pos xxx0 0000
  msd[17] |= (MSD_Data.propulsion >> 1) & 0x1F;         // 6 bit size, pos 000x xxxx
  
  msd[18] |= MSD_Data.propulsion << 7;                  // 6 bit size, pos x000 0000
  msd[18] |= ((MSD_Data.timestamp >> 25) & 0x7F);       // 32 bit size, pos 0xxx xxxx

  msd[19] |= ((MSD_Data.timestamp >> 17) & 0xFF);       // 32 bit size, pos xxxx xxxx

  msd[20] |= ((MSD_Data.timestamp >> 9) & 0xFF);        // 32 bit size, pos xxxx xxxx

  msd[21] |= ((MSD_Data.timestamp >> 1) & 0xFF);        // 32 bit size, pos xxxx xxxx

  msd[22] |= ((MSD_Data.timestamp << 7) & 0x80);        // 32 bit size, pos x000 0000

  // Encode gegraphical latitude
  msd[22] |= ((MSD_Data.pos.lat >> 25) & 0x7F);         // 32 bit size, pos 0xxx xxxx
  msd[23] |= ((MSD_Data.pos.lat >> 17) & 0xFF);         // 32 bit size, pos xxxx xxxx
  msd[24] |= ((MSD_Data.pos.lat >> 9) & 0xFF);          // 32 bit size, pos xxxx xxxx
  msd[25] |= ((MSD_Data.pos.lat >> 1) & 0xFF);          // 32 bit size, pos xxxx xxxx

  msd[26] |= ((MSD_Data.pos.lat << 7) & 0x80);          // 32 bit size, pos x000 0000

  // Encode gegraphical longitute
  msd[26] |= ((MSD_Data.pos.lon >> 25) & 0x7F);         // 32 bit size, pos 0xxx xxxx
  msd[27] |= ((MSD_Data.pos.lon >> 17) & 0xFF);         // 32 bit size, pos xxxx xxxx
  msd[28] |= ((MSD_Data.pos.lon >> 9) & 0xFF);          // 32 bit size, pos xxxx xxxx
  msd[29] |= ((MSD_Data.pos.lon >> 1) & 0xFF);          // 32 bit size, pos xxxx xxxx

  msd[30] |= ((MSD_Data.pos.lon << 7) & 0x80);          // 32 bit size, pos x000 0000
  msd[30] |= ((MSD_Data.direction >> 1) & 0x7F);        // 8 bit size, pos 0xxx xxxx

  msd[31] |= ((MSD_Data.direction << 7) & 0x80);        // 8 bit size, pos x000 0000
  msd[31] |= ((MSD_Data.MSD_PosDelta[0].lat >> 3) & 0x7F);    // 10 bit size, pos 0xxx xxxx

  msd[32] |= MSD_Data.MSD_PosDelta[0].lat << 5;         // 10 bit size, pos xxx0 0000
  msd[32] |= ((MSD_Data.MSD_PosDelta[0].lon >> 5) & 0x1F);    // 10 bit size, pos 000x xxxx

  msd[33] |= MSD_Data.MSD_PosDelta[0].lon << 3;         // 10 bit size, pos xxxx x000
  msd[33] |= ((MSD_Data.MSD_PosDelta[1].lat >> 7) & 0x07);    // 10 bit size, pos 0000 0xxx

  msd[34] |= MSD_Data.MSD_PosDelta[1].lat << 1;         // 10 bit size, pos xxxx xxx0
  msd[34] |= ((MSD_Data.MSD_PosDelta[1].lon >> 9) & 0x01);    // 10 bit size, pos 0000 000x

  msd[35] |= MSD_Data.MSD_PosDelta[1].lon >> 1;         // 10 bit size, pos xxxx xxxx
  msd[36] |= MSD_Data.MSD_PosDelta[1].lon << 7;         // 10 bit size, pos x000 0000

  msd[36] |= ((MSD_Data.passengers >> 1) & 0x7F);       // 8 bit size, pos 0xxx xxxx
  
  msd[37] |= MSD_Data.passengers << 7;                  // 8 bit size, pos x000 0000

  MSD_ConvertToAscii();
}

void MSD_ConvertToAscii(void)
{
  uint16_t i = 0;

  for (i = 0; i < 40; i++)
  {
    msdAscii[i << 1] = TO_HEX((msd[i] >> 4) & 0x0F);      // 2i
    msdAscii[(i << 1) + 1] = TO_HEX(msd[i] & 0x0F);       // 2i + 1
  }
}

void MSD_CalcPosDelta(void)
{
  int32_t latitude = 0, longitude = 0;
  int16_t deltaLat = 0, deltaLon = 0;

  // Get current geographical latitude and longitude
  ConvertDecToMilliArcSec(rmc.lat, &latitude, NULL);
  ConvertDecToMilliArcSec(rmc.lon, &longitude, NULL);

  deltaLat = (pos.lat - latitude) / 100;
  deltaLon = (pos.lon - longitude) / 100;

  if (deltaLat <= -512) {
    deltaLat = -512;
  } else if (deltaLat >= 511) {
    deltaLat = 511;
  }
  deltaLat += 512;                    // Make offset

  if (deltaLon <= -512) {
    deltaLon = -512;
  } else if (deltaLon >= 511) {
    deltaLon = 511;
  }
  deltaLon += 512;

  MSD_Data.MSD_PosDelta[1].lat = MSD_Data.MSD_PosDelta[0].lat;
  MSD_Data.MSD_PosDelta[1].lon = MSD_Data.MSD_PosDelta[0].lon;
  
  MSD_Data.MSD_PosDelta[0].lat = deltaLat;
  MSD_Data.MSD_PosDelta[0].lon = deltaLon;
  
  //Update to the latest position in order to calculate
  //correctly the deltas on the next iteration
  pos.lat = latitude;
  pos.lon = longitude;
}

