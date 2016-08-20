
#include <string.h>
#include <stdlib.h>
//#include <stdbool.h>
#include <time.h>
#include <math.h>

#include "main.h"
#include "at.h"
#include "gps.h"
#include "gsm.h"
#include "config.h"
#include "msd.h"

char nmea[NMEA_BUFFER_SIZE];
bool ULOC_SENT = false;
GPS_POITypedef apoi[MAX_POI_NUM];
GPS_POITypedef cpoi;

GPS_StatusTypedef GpsStat = {0};

GPS_ErrorTypedef GPS_Error = GPS_ERROR_OK;

#define HALF_PI 1.5707963
#define RADIUS  6378160.0
unsigned int Distance(double lat0, double long0, double lat1, double long1)
{
	double a, b, c, u, v;

	a = HALF_PI * (1.0 - lat0 / 90.0);
	b = HALF_PI * (1.0 - lat1 / 90.0);
	u = a * a + b * b;
	v = 2 * a * b * cos(HALF_PI * (long1 - long0) / 90.0);
	if (u > v) c = sqrt(u - v);
	else c = sqrt(v - u);

	return (unsigned int)(RADIUS * c);
}

GPS_DataRMCTypedef rmc;
uint16_t rmc_sog = 0, rmc_cog = 0;

static void     ChecksumToHex     (char *aSum, char *aHex);
static char     ComputeChecksum   (char *aStr, int aLen);

bool LoadPOIFile()
{
	char fname[13];
	char fline[128], *prad = NULL, *plon = NULL;
	FIL  poifile;
	int  i = 0;

	sprintf(fname, "poi.txt\0");
	if(FR_OK != f_open(&poifile, fname, FA_READ)) return false;
	if(FR_OK != f_lseek(&poifile, 0)) {f_close(&poifile); return false;}
	memset(apoi, 0, sizeof(apoi));
	while (!f_eof(&poifile)) { // Read data from the text file line by line
		if(NULL == f_gets(fline, sizeof(fline), &poifile)) continue; //{f_close(&poifile); return false;}
		if (!CFG_VerifyComment(fline, CFG_CommentPattern)) {  // Check for comment line
			CFG_StripLineEnd(fline);      // Remove CR or LF at the end of the line
			if(NULL == (plon = strchr(fline, ' '))) continue; //{f_close(&poifile); return false;}
			*plon = 0;
			plon++;
			apoi[i].lat = atof(fline);
			if(NULL != (prad = strchr(plon, ' '))) {
				*prad = 0;
				prad++;
				apoi[i].lon = atof(plon);
				apoi[i].rad = atoi(prad);
			}
			else {
				apoi[i].lon = atof(plon);
				apoi[i].rad = atoi(CFG_GlobVarsStruct.gpsRadius);
			}
			i++;
			if(i >= MAX_POI_NUM) break;
		}
	}
	f_close(&poifile);
	return true;
}

void GPS_Handler(void)
{
	if(GpsStat.Req == true) {
		GpsStat.Req = false;
		if(GpsStat.Fix == false)
		printf("AT+UGRMC?\r");
		else
		{
			//printf("AT+UGRMC?\r");
			printf("AT+UGGGA?\r");
		}

	}

	if(GpsStat.Ack == true) {
		GpsStat.Ack = false;
		GpsStat.Rdy = true;
		if(GpsStat.Fix == false) {
			GPS_DecodeNMEA(nmea);
			if (rmc.sts[0] == 'A') {
				GpsStat.Fix = true;
				if(GpsStat.First == false){
					GpsStat.First = true;
				}
			}
			else {
				GpsStat.Fix = false;
			}
		}
	}
}

bool DetectPPS(void)
{
	GpsStat.PPS = READ_PPS();
	if (GpsStat.PPS && (GpsStat.PPS ^ GpsStat.PrevPPS))
	{
		GpsStat.PrevPPS = GpsStat.PPS;
		return true;
	}
	GpsStat.PrevPPS = GpsStat.PPS;
	return false;
}

// Decode NMEA messages
char GPS_DecodeNMEA(char *aMsg)
{
	// size and checksum of the NMEA message
	int msgSize = 0;
	char msgChecksum = 0;

	// pointer used in NMEA message tokenizer
	char *msgParser = NULL;

	// array of pointers to the NMEA message data fields
	char *msgField[NMEA_MESSAGE_DATA_FIELDS_MAX];

	int i = 0;

	msgSize = strlen(aMsg);

	// verify NMEA message buffer checksum
	ChecksumToHex(aMsg + (msgSize - 2), &msgChecksum);

	if (msgChecksum != ComputeChecksum(aMsg + 1, (msgSize - 4))) {
		return (GPS_ERROR_NMEA_MESSAGE_CHECKSUM_ERROR);
	}

	// initialize and run NMEA message buffer tokenizer
	msgParser = aMsg;

	i = 0;

	msgField[i] = msgParser;

	while(*msgParser != '\0')
	{
		if ((*msgParser == NMEA_MESSAGE_FIELD_SEPARATOR) || (*msgParser == NMEA_MESSAGE_EOM)) {
			// save position of the next field
			msgField[++i] = msgParser + 1;
		}

		if (i > NMEA_MESSAGE_DATA_FIELDS_MAX) {
			return (GPS_ERROR_NMEA_MESSAGE_FIELDS_EXCEEDED);
		}

		msgParser++;
	}

	// decode NMEA message type and data structures
	// RMC (field 10 and 11 are not outputed by the receiver)
	if (strncmp(msgField[0] + 3, "RMC", 3) == 0) {
		strncpy(rmc.utc, msgField[1], msgField[2] - msgField[1] - 1);
		strncpy(rmc.sts, msgField[2], msgField[3] - msgField[2] - 1);
		strncpy(rmc.lat, msgField[3], msgField[4] - msgField[3] - 1);
		strncpy(rmc.lat_ns, msgField[4], msgField[5] - msgField[4] - 1);
		strncpy(rmc.lon, msgField[5], msgField[6] - msgField[5] - 1);
		strncpy(rmc.lon_ew, msgField[6], msgField[7] - msgField[6] - 1);
		strncpy(rmc.sog, msgField[7], msgField[8] - msgField[7] - 1);
		strncpy(rmc.cog, msgField[8], msgField[9] - msgField[8] - 1);
		strncpy(rmc.dat, msgField[9], msgField[10] - msgField[9] - 1);
		strncpy(rmc.mav, msgField[10], msgField[11] - msgField[10] - 1);
		strncpy(rmc.mav_ew, msgField[11], msgField[12] - msgField[11] - 1);
		strncpy(rmc.mod, msgField[12], msgField[13] - msgField[12] - 1);
		rmc_cog = atoi(&rmc.cog[0]);
		rmc_sog = atoi(&rmc.sog[0]);
		ConvertDecToMilliArcSec(rmc.lat, NULL, &cpoi.lat);
		ConvertDecToMilliArcSec(rmc.lon, NULL, &cpoi.lon);
		if(rmc.lat_ns[0] != 'N') cpoi.lat = -cpoi.lat;
		if(rmc.lon_ew[0] != 'E') cpoi.lon = -cpoi.lon;
	}

	return GPS_ERROR_OK;
}

// latitude and longitude are in degrees, minutes and fraction of minutes
char ConvertDecimalToDMS(char *dec, int *deg, int *min, int *sec)
{
	double _dec = atof(dec);

	*deg = (int)(_dec / 100);
	*min = (int)(_dec) - (*deg * 100);

	*sec = (int)((_dec - *min - 100 * *deg) * 60);

	return 0;
}



// latitude and longitude are in degrees, minutes and fraction of minutes
char ConvertDecToMilliArcSec(char *dec, int *mArcs, double *gradus)
{
	int deg = 0, min = 0;
	double sec = 0.0, _dec = atof(dec);

	deg = (int)(_dec / 100);
	min = (int)(_dec) - (deg * 100);

	sec = (double)(_dec - min - 100 * deg) * 60.0;

	if(gradus != NULL) *gradus = deg + min/60.0 + sec/3600.0;

	if(mArcs != NULL) *mArcs = (int)(deg * 3600 + min * 60) * 1000 + (int)(sec * 1000);  // mArcseconds

	return 0;
}

// Convert UTC date and time in seconds since unix epoch
char ConvertTimeToEpoch(char *pTime, char *pDate, uint32_t *timestamp)
{
	struct tm t;
	long ddmmyy = 0, hhmmss = 0;

	ddmmyy = atol(pDate);

	t.tm_year = 100 + (ddmmyy - (ddmmyy / 100) * 100);
	t.tm_mon = ddmmyy / 100 - (ddmmyy / 10000) * 100 - 1;
	t.tm_mday = ddmmyy / 10000;

	hhmmss = atol(pTime);

	t.tm_hour = hhmmss / 10000;
	t.tm_min = (uint32_t)(hhmmss / 100) - (uint32_t)(hhmmss / 10000) * 100;
	t.tm_sec = hhmmss - (hhmmss / 100) * 100;

	t.tm_isdst = -1;      // Without information about daylight saving settings

	*timestamp = mktime(&t);

	return 0;
}

static void ChecksumToHex(char *aSum, char *aHex)
{
	// convert high nibble (hin) and low nibble (lon) to hex value
	char hin = CHAR_TO_HEX(aSum[0]);
	char lon = CHAR_TO_HEX(aSum[1]);

	*aHex = (hin << 4) | lon;
}

static char ComputeChecksum(char *aStr, int aLen)
{
	// reset the checksum hex value
	char sum = 0x00;

	// compute checksum by xoring each element in the str array
	while (aLen)
	{
		sum ^= aStr[--aLen];
	}

	return (sum);
}
