
#ifndef __LSM303C_H
#define __LSM303C_H

// Read/Write command
#define LSM303C_READWRITE_CMD          ((uint8_t)0x80)

// Multiple byte read/write command
#define LSM303C_MULTIPLEBYTE_CMD       ((uint8_t)0x40)

// Dummy Byte Send by the SPI Master device in order to generate the Clock to the Slave device
#define LSM303C_DUMMY_BYTE             ((uint8_t)0x00)

// ACC Register Mapping
#define LSM303C_WHO_AM_I_A              ((uint8_t)0x0F)  // ACC device identification register
#define LSM303C_ACT_THS_A               ((uint8_t)0x1E)  // ACC Activity threshold register
#define LSM303C_ACT_DUR_A               ((uint8_t)0x1F)  // ACC Activity duration register
#define LSM303C_CTRL_REG1_A             ((uint8_t)0x20)  // ACC Control register 1
#define LSM303C_CTRL_REG2_A             ((uint8_t)0x21)  // ACC Control register 2
#define LSM303C_CTRL_REG3_A             ((uint8_t)0x22)  // ACC Control register 3
#define LSM303C_CTRL_REG4_A             ((uint8_t)0x23)  // ACC Control register 4
#define LSM303C_CTRL_REG5_A             ((uint8_t)0x24)  // ACC Control register 5
#define LSM303C_CTRL_REG6_A             ((uint8_t)0x25)  // ACC Control register 6
#define LSM303C_CTRL_REG7_A             ((uint8_t)0x26)  // ACC Control register 7
#define LSM303C_STATUS_REG_A            ((uint8_t)0x27)  // ACC Accelerometer status data register
#define LSM303C_OUT_X_L_A               ((uint8_t)0x28)  // ACC output X axis low byte
#define LSM303C_OUT_X_H_A               ((uint8_t)0x29)  // ACC output X axis high byte
#define LSM303C_OUT_Y_L_A               ((uint8_t)0x2A)  // ACC output Y axis low byte
#define LSM303C_OUT_Y_H_A               ((uint8_t)0x2B)  // ACC output Y axis high byte
#define LSM303C_OUT_Z_L_A               ((uint8_t)0x2C)  // ACC output Z axis low byte
#define LSM303C_OUT_Z_H_A               ((uint8_t)0x2D)  // ACC output Z axis high byte
#define LSM303C_FIFO_CTRL               ((uint8_t)0x2E)  // FIFO control register
#define LSM303C_FIFO_SRC                ((uint8_t)0x2F)  // FIFO status control register
#define LSM303C_IG_CFG1_A               ((uint8_t)0x30)  // ACC interrupt generator 1 configuration
#define LSM303C_IG_SRC1_A               ((uint8_t)0x31)  // ACC interrupt generator 1 status register
#define LSM303C_IG_THS_X1_A             ((uint8_t)0x32)  // ACC interrupt generator threshold X
#define LSM303C_IG_THS_Y1_A             ((uint8_t)0x33)  // ACC interrupt generator threshold Y
#define LSM303C_IG_THS_Z1_A             ((uint8_t)0x34)  // ACC interrupt generator threshold Z
#define LSM303C_IG_DUR1_A               ((uint8_t)0x35)  // ACC interrupt generator duration
#define LSM303C_IG_CFG2_A               ((uint8_t)0x36)  // ACC interrupt generator 2 configuration
#define LSM303C_IG_SRC2_A               ((uint8_t)0x37)  // ACC interrupt generator 2 status register
#define LSM303C_IG_THS2_A               ((uint8_t)0x38)  // ACC interrupt generator 2 threshold
#define LSM303C_IG_DUR2_A               ((uint8_t)0x39)  // ACC interrupt generator 2 duration
#define LSM303C_XL_REFERENCE            ((uint8_t)0x3A)  // ACC reference X low
#define LSM303C_XH_REFERENCE            ((uint8_t)0x3B)  // ACC reference X high
#define LSM303C_YL_REFERENCE            ((uint8_t)0x3C)  // ACC reference Y low
#define LSM303C_YH_REFERENCE            ((uint8_t)0x3D)  // ACC reference Y high
#define LSM303C_ZL_REFERENCE            ((uint8_t)0x3E)  // ACC reference Z low
#define LSM303C_ZH_REFERENCE            ((uint8_t)0x3F)  // ACC reference Z high

// MAG Register Mapping
#define LSM303C_WHO_AM_I_M              ((uint8_t)0x0F)  // MAG who I am ID
#define LSM303C_CTRL_REG1_M             ((uint8_t)0x20)  // MAG Control register 1
#define LSM303C_CTRL_REG2_M             ((uint8_t)0x21)  // MAG Control register 2
#define LSM303C_CTRL_REG3_M             ((uint8_t)0x22)  // MAG Control register 3
#define LSM303C_CTRL_REG4_M             ((uint8_t)0x23)  // MAG Control register 4
#define LSM303C_CTRL_REG5_M             ((uint8_t)0x24)  // MAG Control register 5
#define LSM303C_STATUS_REG_M            ((uint8_t)0x27)  // MAG status data register
#define LSM303C_OUT_X_L_M               ((uint8_t)0x28)  // MAG output X axis low byte
#define LSM303C_OUT_X_H_M               ((uint8_t)0x29)  // MAG output X axis high byte
#define LSM303C_OUT_Y_L_M               ((uint8_t)0x2A)  // MAG output Y axis low byte
#define LSM303C_OUT_Y_H_M               ((uint8_t)0x2B)  // MAG output Y axis high byte
#define LSM303C_OUT_Z_L_M               ((uint8_t)0x2C)  // MAG output Z axis low byte
#define LSM303C_OUT_Z_H_M               ((uint8_t)0x2D)  // MAG output Z axis high byte
#define LSM303C_TEMP_L_M                ((uint8_t)0x2E)  // MAG Temperature sensor data low
#define LSM303C_TEMP_H_M                ((uint8_t)0x2F)  // MAG Temperature sensor data high
#define LSM303C_INT_CFG_M               ((uint8_t)0x30)  // MAG interrupt configuration register
#define LSM303C_INT_SRC_M               ((uint8_t)0x31)  // MAG interrupt generator status
#define LSM303C_THS_L_M                 ((uint8_t)0x32)  // MAG interrupt generator threshold low
#define LSM303C_THS_H_M                 ((uint8_t)0x33)  // MAG interrupt generator threshold high

// WHO_AM_I register values
#define I_AM_LSM303C_A                  ((uint8_t)0x41)
#define I_AM_LSM303C_M                  ((uint8_t)0x3D)

//////////////////////////////////
// CTRL_REG1_M register values map
//////////////////////////////////
// Temperature sensor mode
#define LSM303C_M_TEMP_DISABLE            ((uint8_t)0x00)   //Disable temp sensor
#define LSM303C_M_TEMP_ENABLE             ((uint8_t)0x80)   //Enable temp sensor

// X and Y axes operative mode selection
#define LSM303C_M_XY_LOW_POWER            ((uint8_t)0x00)   //Low power mode
#define LSM303C_M_XY_MEDIUM_PERF          ((uint8_t)0x20)   //Medium-performance mode
#define LSM303C_M_XY_High_PERF            ((uint8_t)0x40)   //High-performance mode
#define LSM303C_M_XY_ULTRAHIGH_PERF       ((uint8_t)0x60)   //Ultra-high performance mode

// ODR Selection
#define LSM303C_M_ODR_0_625_HZ            ((uint8_t)0x00)   //ODR 0.625 Hz
#define LSM303C_M_ODR_1_25_HZ             ((uint8_t)0x04)   //ODR 1.25  Hz
#define LSM303C_M_ODR_2_5_HZ              ((uint8_t)0x08)   //ODR 2.5   Hz
#define LSM303C_M_ODR_5_HZ                ((uint8_t)0x0C)   //ODR 5  Hz
#define LSM303C_M_ODR_10_HZ               ((uint8_t)0x10)   //ODR 10 Hz
#define LSM303C_M_ODR_20_HZ               ((uint8_t)0x14)   //ODR 20 Hz
#define LSM303C_M_ODR_40_HZ               ((uint8_t)0x18)   //ODR 40 Hz
#define LSM303C_M_ODR_80_HZ               ((uint8_t)0x1C)   //ODR 80 Hz

//////////////////////////////////
// CTRL_REG2_M register values map
//////////////////////////////////
// Full scale configuration
#define LSM303C_M_FULL_SCALE              ((uint8_t)0xC0)   //Full Scale +-16 Gauss

//////////////////////////////////
// CTRL_REG3_M register values map
//////////////////////////////////
// I2C mode
#define LSM303C_M_I2C_ENABLE              ((uint8_t)0x00) //Enable communication over I2C
#define LSM303C_M_I2C_DISABLE             ((uint8_t)0x80) //Disable communication over I2C

// System power mode
#define LSM303C_M_LOW_POWER_ENABLE        ((uint8_t)0x20) //Enable low power mode  => ODR = 0.625Hz
#define LSM303C_M_LOW_POWER_DISABLE       ((uint8_t)0x00) //Disable low power mode => ODR = ODR Selection

// Serial Interface Mode
#define LSM303C_M_SPI_WRITE_ONLY          ((uint8_t)0x00) //SPI only Write operations enabled
#define LSM303C_M_SPI_READ_WRITE          ((uint8_t)0x04) //SPI Read and Write operations enabled

// System operating mode selection
#define LSM303C_M_SOM_CONT_CONV           ((uint8_t)0x00) //Continuous Conversion Mode
#define LSM303C_M_SOM_SINGLE_CONV         ((uint8_t)0x01) //Single-conversion mode => measurement is made only when asked for
#define LSM303C_M_SOM_POWER_DOWN1         ((uint8_t)0x02) //Power Down Mode
#define LSM303C_M_SOM_POWER_DOWN2         ((uint8_t)0x03) //Power Down Mode

//////////////////////////////////
// CTRL_REG4_M register values map
//////////////////////////////////
// Z-axis operative mode
#define LSM303C_M_Z_LOW_POWER             ((uint8_t)0x00) //Low power mode
#define LSM303C_M_Z_MEDIUM_PERF           ((uint8_t)0x04) //Medium-performance mode
#define LSM303C_M_Z_HIGH_PERF             ((uint8_t)0x08) //High-performance mode
#define LSM303C_M_Z_ULTRAHIGH_PERF        ((uint8_t)0x0C) //Ultra-high performance mode

// Endianness selection
#define LSM303C_M_BLE_LSB                 ((uint8_t)0x00) //data LSb at lower address
#define LSM303C_M_BLE_MSB                 ((uint8_t)0x02) //data MSb at lower address

//////////////////////////////////
// CTRL_REG5_M register values map
//////////////////////////////////
// Block data update for magnetic data
#define LSM303C_M_BDU_CONTINUOUS          ((uint8_t)0x00) //Continuous update
#define LSM303C_M_BDU_SINGLE              ((uint8_t)0x40) //Output registers not updated until MSB and LSB have been read


typedef enum {
  MAG_OK = 0,
  MAG_ERROR,
} LSM303C_StatusTypedef;

// LSM303C MAG struct
typedef struct {
  uint8_t PowerMode;            // Low power enable/disable
  uint8_t TempMode;           // Temperature sensor enable/disable
  uint8_t XYOperativeMode;      // Operative mode for X and Y axes
  uint8_t ZOperativeMode;       // Operative mode for Z axis
  uint8_t OutputDataRate;       // Output data rate
  uint8_t InterfaceModeI2C;     // Enable/disable I2C interface
  uint8_t InterfaceModeSPI;     // Select SPI interface mode (Write only / Read & Write)
  uint8_t SystemOperatingMode;  // System operating mode selection
  uint8_t BlockDataUpdate;      // Block data update
  uint8_t Endianness;           // Endian data selection
  uint8_t FullScale;            // Full scale selection
} LSM303C_InitTypedef;

extern LSM303C_StatusTypedef MagStatus;
extern int32_t MagInt[3];

void LSM303C_Handler(void);
uint8_t LSM303C_ReadID(void);
void LSM303C_Write(uint8_t* pBuffer, uint8_t WriteAddr, uint16_t NumByteToWrite);
void LSM303C_Read(uint8_t* pBuffer, uint8_t ReadAddr, uint16_t NumByteToRead);
uint8_t LSM303C_GetDataStatus(void);
void LSM303C_ReadXYZMag(void);
uint8_t LSM303C_Configure(void);
void LSM303C_Init(LSM303C_InitTypedef *LSM303C_InitStruct);

#endif