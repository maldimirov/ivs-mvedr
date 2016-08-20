
#ifndef __GPS_H
#define __GPS_H

#include <stdbool.h>

// @brief NMEA message buffer size
#define NMEA_BUFFER_SIZE  256

// @brief NMEA message start-of-message (SOM) character
#define NMEA_MESSAGE_SOM  '$'

// @brief NMEA message end-of-message (EOM) character
#define NMEA_MESSAGE_EOM  '*'

// @brief NMEA message field separator
#define NMEA_MESSAGE_FIELD_SEPARATOR  ','

// @brief NMEA message data field number
#define NMEA_MESSAGE_DATA_FIELDS_MAX  20

// @brief Convert ASCII character to HEX value
#define CHAR_TO_HEX(__CHR__) ((__CHR__ - '0') > 9) ? (__CHR__ - 'A') + 10: (__CHR__ - '0')

typedef struct {
  unsigned char Pwr     : 1;
  unsigned char Rdy     : 1;
  unsigned char Req     : 1;
  unsigned char Fix     : 1;
  unsigned char Ack     : 1;
  unsigned char First   : 1;
  unsigned char PPS			: 1;
  unsigned char PrevPPS : 1; 
} GPS_StatusTypedef;

typedef enum {
  GPS_ERROR_OK = 0,                         // Success
  GPS_ERROR_GENERAL,                        // Common Error
  GPS_ERROR_NMEA_MESSAGE_CHECKSUM_ERROR,    // NMEA message checksum error
  GPS_ERROR_NMEA_MESSAGE_FIELDS_EXCEEDED,   // MMEA message fields exceeded
  GPS_ERROR_NMEA_MESSAGE_NOT_IMPLEMENTED,   // NMEA message not implemented
} GPS_ErrorTypedef;

// @brief Latitude data field, null-terminated string [ddmm.mmmmm]
typedef char LATITUDE[10 + 1];

// @brief Longitude data field, null-terminated string [dddmm.mmmmm]
typedef char LONGITUDE[11 + 1];

// @brief Date data field, null-terminated string [ddmmyy]
typedef char DATE[6 + 1];

// @brief Time data field, null-terminated string [hhmmss.sss]
typedef char TIME[10 + 1];

// @brief Numeric data field, null-terminated string
typedef char NUMERIC[8 + 1];

// @brief Digit data field, null-terminated string [d]
typedef char DIGIT[1 + 1];

// @brief Character data field, null-terminated string [a]
typedef char CHARACTER[1 + 1];

// @brief Latitude and longitude, with time of position fix and status (GLL) fields
typedef struct {
  TIME utc;               // See GPS_DATA_GGA.utc
  CHARACTER sts;          // Status: A = Navigation receiver warning, V = Valid
  LATITUDE lat;           // See GPS_DATA_GGA.lat
  CHARACTER lat_ns;       // See GPS_DATA_GGA.lat_ns
  LONGITUDE lon;          // See GPS_DATA_GGA.lon
  CHARACTER lon_ew;       // See GPS_DATA_GGA.lon_ew
  NUMERIC sog;            // Speed over ground [knot/s]
  NUMERIC cog;            // Course over ground [degrees] 
  DATE dat;               // Date
  NUMERIC mav;            // Magnetic variation [degrees]
  CHARACTER mav_ew;       // Magnetic variation E/W indicator: E = East, W = West
  CHARACTER mod;          // FAA mode indicator: N = No fix, A = Autonomous GNSS fix, 
                          // D = Differential GNSS fix, E = Estimated dead reckoning (DR) fix
} GPS_DataRMCTypedef;

#define MAX_POI_NUM     50
typedef struct {
  double        lat;
  double        lon;
  unsigned int  rad;
  unsigned int  beeps;
} GPS_POITypedef;

extern char nmea[NMEA_BUFFER_SIZE];
extern GPS_POITypedef apoi[MAX_POI_NUM];
extern GPS_POITypedef cpoi;
extern GPS_StatusTypedef GpsStat;
extern GPS_ErrorTypedef GPS_Error;

extern GPS_DataRMCTypedef rmc;
extern uint16_t rmc_sog, rmc_cog;

void    GPS_Handler(void);
void    GPS_Init();
char    GPS_DecodeNMEA(char *aMsg);
bool 		LoadPOIFile();
bool 		DetectPPS(void);

char      ConvertDecimalToDMS     (char *dec, int *deg, int *min, int *sec);
char      ConvertDecToMilliArcSec (char *dec, int *mArcs, double *gradus);
char      ConvertTimeToEpoch      (char *pTime, char *pDate, uint32_t *timestamp);
unsigned int Distance(double lat0, double long0, double lat1, double long1);

#endif
