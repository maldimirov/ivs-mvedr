
#ifndef __LSM303C_H
#define __LSM303C_H

// Read/Write command
#define LSM303C_READWRITE_CMD          ((uint8_t)0x80)

// Multiple byte read/write command
#define LSM303C_MULTIPLEBYTE_CMD       ((uint8_t)0x40)

// Dummy Byte Send by the SPI Master device in order to generate the Clock to the Slave device
#define LSM303C_DUMMY_BYTE             ((uint8_t)0x00)

// ACC Register Mapping
#define LSM303C_WHO_AM_I_A              0x0F  // ACC device identification register
#define LSM303C_ACT_THS_A               0x1E  // ACC Activity threshold register
#define LSM303C_ACT_DUR_A               0x1F  // ACC Activity duration register
#define LSM303C_CTRL_REG1_A             0x20  // ACC Control register 1
#define LSM303C_CTRL_REG2_A             0x21  // ACC Control register 2
#define LSM303C_CTRL_REG3_A             0x22  // ACC Control register 3
#define LSM303C_CTRL_REG4_A             0x23  // ACC Control register 4
#define LSM303C_CTRL_REG5_A             0x24  // ACC Control register 5
#define LSM303C_CTRL_REG6_A             0x25  // ACC Control register 6
#define LSM303C_CTRL_REG7_A             0x26  // ACC Control register 7
#define LSM303C_STATUS_REG_A            0x27  // ACC Accelerometer status data register
#define LSM303C_OUT_X_L_A               0x28  // ACC output X axis low byte
#define LSM303C_OUT_X_H_A               0x29  // ACC output X axis high byte
#define LSM303C_OUT_Y_L_A               0x2A  // ACC output Y axis low byte
#define LSM303C_OUT_Y_H_A               0x2B  // ACC output Y axis high byte
#define LSM303C_OUT_Z_L_A               0x2C  // ACC output Z axis low byte
#define LSM303C_OUT_Z_H_A               0x2D  // ACC output Z axis high byte
#define LSM303C_FIFO_CTRL               0x2E  // FIFO control register
#define LSM303C_FIFO_SRC                0x2F  // FIFO status control register
#define LSM303C_IG_CFG1_A               0x30  // ACC interrupt generator 1 configuration
#define LSM303C_IG_SRC1_A               0x31  // ACC interrupt generator 1 status register
#define LSM303C_IG_THS_X1_A             0x32  // ACC interrupt generator threshold X
#define LSM303C_IG_THS_Y1_A             0x33  // ACC interrupt generator threshold Y
#define LSM303C_IG_THS_Z1_A             0x34  // ACC interrupt generator threshold Z
#define LSM303C_IG_DUR1_A               0x35  // ACC interrupt generator duration
#define LSM303C_IG_CFG2_A               0x36  // ACC interrupt generator 2 configuration
#define LSM303C_IG_SRC2_A               0x37  // ACC interrupt generator 2 status register
#define LSM303C_IG_THS2_A               0x38  // ACC interrupt generator 2 threshold
#define LSM303C_IG_DUR2_A               0x39  // ACC interrupt generator 2 duration
#define LSM303C_XL_REFERENCE            0x3A  // ACC reference X low
#define LSM303C_XH_REFERENCE            0x3B  // ACC reference X high
#define LSM303C_YL_REFERENCE            0x3C  // ACC reference Y low
#define LSM303C_YH_REFERENCE            0x3D  // ACC reference Y high
#define LSM303C_ZL_REFERENCE            0x3E  // ACC reference Z low
#define LSM303C_ZH_REFERENCE            0x3F  // ACC reference Z high

// MAG Register Mapping
#define LSM303C_WHO_AM_I_M              0x0F  // MAG who I am ID
#define LSM303C_CTRL_REG1_M             0x20  // MAG Control register 1
#define LSM303C_CTRL_REG2_M             0x21  // MAG Control register 2
#define LSM303C_CTRL_REG3_M             0x22  // MAG Control register 3
#define LSM303C_CTRL_REG4_M             0x23  // MAG Control register 4
#define LSM303C_CTRL_REG5_M             0x24  // MAG Control register 5
#define LSM303C_STATUS_REG_M            0x27  // MAG Accelerometer status data register
#define LSM303C_OUT_X_L_M               0x28  // MAG output X axis low byte
#define LSM303C_OUT_X_H_M               0x29  // MAG output X axis high byte
#define LSM303C_OUT_Y_L_M               0x2A  // MAG output Y axis low byte
#define LSM303C_OUT_Y_H_M               0x2B  // MAG output Y axis high byte
#define LSM303C_OUT_Z_L_M               0x2C  // MAG output Z axis low byte
#define LSM303C_OUT_Z_H_M               0x2D  // MAG output Z axis high byte
#define LSM303C_TEMP_L_M                0x2E  // MAG Temperature sensor data low
#define LSM303C_TEMP_H_M                0x2F  // MAG Temperature sensor data high
#define LSM303C_INT_CFG_M               0x30  // MAG interrupt configuration register
#define LSM303C_INT_SRC_M               0x31  // MAG interrupt generator status
#define LSM303C_THS_L_M                 0x32  // MAG interrupt generator threshold low
#define LSM303C_THS_H_M                 0x33  // MAG interrupt generator threshold high

// WHO_AM_I register values
#define I_AM_LSM303C_A                  ((uint8_t)0x41)
#define I_AM_LSM303C_M                  ((uint8_t)0x3D)

// @brief Big/little endian data selection
#define LSM303C_BLE_LSB                ((uint8_t)0x00)   // Little endian: data LSB @ lower address
#define LSM303C_BLE_MSB                ((uint8_t)0x02)   // Big endian: data MSB @ lower address

typedef enum {
  MAG_OK = 0,
  MAG_ERROR,
} LSM303C_StatusTypedef;

extern LSM303C_StatusTypedef MagStatus;
extern int32_t MagInt[3];

void LSM303C_Handler(void);
uint8_t LSM303C_ReadID(void);
void LSM303C_Write(uint8_t* pBuffer, uint8_t WriteAddr, uint16_t NumByteToWrite);
void LSM303C_Read(uint8_t* pBuffer, uint8_t ReadAddr, uint16_t NumByteToRead);
uint8_t LSM303C_GetDataStatus(void);
void LSM303C_ReadXYZMag(void);
uint8_t LSM303C_Configure(void);

#endif