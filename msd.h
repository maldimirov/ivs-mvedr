
#ifndef __MSD_H
#define __MSD_H


#define MSD_DATA_SIZE   140
#define MSD_DATA_ASCII  81

#define ECALL_CLOSED    4
//#define ECALL_VOICE     3
#define ECALL_IDLE      2
#define ECALL_MANUAL    0
#define ECALL_AUTO      1

#define TO_HEX(NIBBLE)  ((NIBBLE) <= 9 ? '0' + (NIBBLE) : 'A' - 10 + (NIBBLE))

extern unsigned char msd[MSD_DATA_SIZE];
extern char msdAscii[MSD_DATA_ASCII];

// @brief Control and status data
typedef struct {
	unsigned char extraData;					// 000 - additional data excluded, 010 - additional data included
  unsigned char recentPos1;					// True if previous location is available
  unsigned char recentPos2;					// True if previous location is available
  unsigned char passengers;					// True if passangers' number is available
	unsigned char activation;					// True if Automatic, False if manual eCall activation
  unsigned char testCall;						// True if test eCall, Flase if not test eCall
  unsigned char posStatus;					// True if GPS position can be trusted
  unsigned char vehicleType;				// Vehicle identification according to 2007/46/EC, 2002/24/EC
  unsigned char includedTanks;			// if True include status data (present/not present) for the relevant tank
} MSD_ControlTypedef;

// @brief Vehicle location stored in milli-arc-second
typedef struct {
  int32_t lat;											// Position latitude
  int32_t lon;											// Position longitude
} MSD_GpsPosTypedef;

// @brief Vehicle location difference with the previous location stored in milli-arc-second
typedef struct {
  uint16_t lat;
  uint16_t lon;
} MSD_PosDeltaTypedef;

// @brief MSD data structure
typedef struct {
  unsigned char msdId;									// MSD format version set to 1
  unsigned char msgId;									// Message identifier, starting with 1 for each new eCall session
  MSD_ControlTypedef MSD_Control;				// Control flags and data
  unsigned char vin[18];								// VIN
  unsigned char propulsion;							// Vehicle propulsion storage type
  uint32_t timestamp;										// Timestamp
  MSD_GpsPosTypedef pos;								// Vehicle position
  uint8_t direction;										// Vehicle heading, course over ground, course over north
	MSD_PosDeltaTypedef MSD_PosDelta[2];	// Previous position delta
  uint8_t passengers;										// Passengers number
	unsigned char *pExtraData;						// Additional data
} MSD_DataTypedef;


void      MSD_InitData      (char aGpsStatus);
void      MSD_PrepareData   (void);
void      MSD_CalcPosDelta  (void);

void      MSD_ConvertToAscii(void);

#endif
