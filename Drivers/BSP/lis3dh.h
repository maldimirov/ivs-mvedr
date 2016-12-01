
#ifndef __LIS3DH_H
#define __LIS3DH_H

// Read/Write command
#define LIS3DH_READWRITE_CMD          ((uint8_t)0x80)

// Multiple byte read/write command
#define LIS3DH_MULTIPLEBYTE_CMD       ((uint8_t)0x40)

// Dummy Byte Send by the SPI Master device in order to generate the Clock to the Slave device
#define LIS3DH_DUMMY_BYTE             ((uint8_t)0x00)

// @breif Accelerometer register mapping
#define LIS3DH_WHO_AM_I_ADDR          0x0F  // device identification register
#define LIS3DH_CTRL_REG1_ADDR         0x20  // Control register 1 acceleration
#define LIS3DH_CTRL_REG2_ADDR         0x21  // Control register 2 acceleration
#define LIS3DH_CTRL_REG3_ADDR         0x22  // Control register 3 acceleration
#define LIS3DH_CTRL_REG4_ADDR         0x23  // Control register 4 acceleration
#define LIS3DH_CTRL_REG5_ADDR         0x24  // Control register 5 acceleration
#define LIS3DH_CTRL_REG6_ADDR         0x25  // Control register 6 acceleration
#define LIS3DH_REFERENCE_ADDR         0x26  // Reference register acceleration
#define LIS3DH_STATUS_REG_ADDR        0x27  // Status register acceleration
#define LIS3DH_OUT_X_L_ADDR           0x28  // Output Register X acceleration
#define LIS3DH_OUT_X_H_ADDR           0x29  // Output Register X acceleration
#define LIS3DH_OUT_Y_L_ADDR           0x2A  // Output Register Y acceleration
#define LIS3DH_OUT_Y_H_ADDR           0x2B  // Output Register Y acceleration
#define LIS3DH_OUT_Z_L_ADDR           0x2C  // Output Register Z acceleration
#define LIS3DH_OUT_Z_H_ADDR           0x2D  // Output Register Z acceleration
#define LIS3DH_FIFO_CTRL_REG_ADDR     0x2E  // Fifo control Register acceleration
#define LIS3DH_FIFO_SRC_REG_ADDR      0x2F  // Fifo src Register acceleration

#define LIS3DH_INT1_CFG_ADDR          0x30  // Interrupt 1 configuration Register acceleration
#define LIS3DH_INT1_SOURCE_ADDR       0x31  // Interrupt 1 source Register acceleration
#define LIS3DH_INT1_THS_ADDR          0x32  // Interrupt 1 Threshold register acceleration
#define LIS3DH_INT1_DURATION_ADDR     0x33  // Interrupt 1 DURATION register acceleration

#define LIS3DH_CLICK_CFG_ADDR         0x38  // Click configuration Register acceleration
#define LIS3DH_CLICK_SOURCE_ADDR      0x39  // Click 2 source Register acceleration
#define LIS3DH_CLICK_THS_ADDR         0x3A  // Click 2 Threshold register acceleration

#define LIS3DH_TIME_LIMIT_ADDR        0x3B  // Time Limit Register acceleration
#define LIS3DH_TIME_LATENCY_ADDR      0x3C  // Time Latency Register acceleration
#define LIS3DH_TIME_WINDOW_ADDR       0x3D  // Time window register acceleration

#define I_AM_LIS3DH                   ((uint8_t)0x33)

// @brief Power mode selection
#define LIS3DH_MODE_NORMAL            ((uint8_t)0x00)
#define LIS3DH_MODE_LOWPOWER          ((uint8_t)0x08)

// @brief Output data rate selection
#define LIS3DH_ODR_1_HZ               ((uint8_t)0x10)   // Output Data Rate = 1 Hz
#define LIS3DH_ODR_10_HZ              ((uint8_t)0x20)   // Output Data Rate = 10 Hz
#define LIS3DH_ODR_25_HZ              ((uint8_t)0x30)   // Output Data Rate = 25 Hz
#define LIS3DH_ODR_50_HZ              ((uint8_t)0x40)   // Output Data Rate = 50 Hz
#define LIS3DH_ODR_100_HZ             ((uint8_t)0x50)   // Output Data Rate = 100 Hz
#define LIS3DH_ODR_200_HZ             ((uint8_t)0x60)   // Output Data Rate = 200 Hz
#define LIS3DH_ODR_400_HZ             ((uint8_t)0x70)   // Output Data Rate = 400 Hz
#define LIS3DH_ODR_1600_HZ_LP         ((uint8_t)0x80)   // Output Data Rate = 1600 Hz only in Low Power Mode
#define LIS3DH_ODR_1250_HZ            ((uint8_t)0x90)   // Output Data Rate = 1250 Hz in Normal mode and 5000 Hz in Low Power Mode

// @brief Accelerometer axes selection
#define LIS3DH_X_ENABLE               ((uint8_t)0x01)
#define LIS3DH_Y_ENABLE               ((uint8_t)0x02)
#define LIS3DH_Z_ENABLE               ((uint8_t)0x04)
#define LIS3DH_AXES_ENABLE            ((uint8_t)0x07)
#define LIS3DH_AXES_DISABLE           ((uint8_t)0x00)


// @brief Single/continuous block data update
#define LIS3DH_BlockUpdate_Continuous ((uint8_t)0x00)   // Continuous Update
#define LIS3DH_BlockUpdate_Single     ((uint8_t)0x80)   // Single Update: output registers not updated until MSB and LSB reading

// @brief Big/little endian data selection
#define LIS3DH_BLE_LSB                ((uint8_t)0x00)   // Little endian: data LSB @ lower address
#define LIS3DH_BLE_MSB                ((uint8_t)0x40)   // Big endian: data MSB @ lower address

// @brief Full scale selection
#define LIS3DH_FULLSCALE_2G           ((uint8_t)0x00)   // ±2 g
#define LIS3DH_FULLSCALE_4G           ((uint8_t)0x10)   // ±4 g
#define LIS3DH_FULLSCALE_8G           ((uint8_t)0x20)   // ±8 g
#define LIS3DH_FULLSCALE_16G          ((uint8_t)0x30)   // ±16 g
#define LIS3DH_FULLSCALE_SELECTION    ((uint8_t)0x30)

// @brief High resolution
#define LIS3DH_HR_ENABLE              ((uint8_t)0x08)
#define LIS3DH_HR_DISABLE             ((uint8_t)0x00)

// @brief Acc_Boot_Mode_selection
#define LIS3DH_BOOT_NORMALMODE        ((uint8_t)0x00)
#define LIS3DH_BOOT_REBOOTMEMORY      ((uint8_t)0x80)

// @brief High pass filter mode
#define LIS3DH_HPM_NORMAL_MODE_RES    ((uint8_t)0x00)
#define LIS3DH_HPM_REF_SIGNAL         ((uint8_t)0x40)
#define LIS3DH_HPM_NORMAL_MODE        ((uint8_t)0x80)
#define LIS3DH_HPM_AUTORESET_INT      ((uint8_t)0xC0)

#define LIS3DH_HPFCF_0                ((uint8_t)0x00) // Highest cut-off frequency
#define LIS3DH_HPFCF_1                ((uint8_t)0x10)
#define LIS3DH_HPFCF_2                ((uint8_t)0x20)
#define LIS3DH_HPFCF_3                ((uint8_t)0x30) // Lowest cut-off frequency

#define LIS3DH_SENSITIVITY_2G         (uint8_t)1  // sensitivity with 2 g full scale [mg/LSB]
#define LIS3DH_SENSITIVITY_4G         (uint8_t)2  // sensitivity with 4 g full scale [mg/LSB]
#define LIS3DH_SENSITIVITY_8G         (uint8_t)4  // sensitivity with 8 g full scale [mg/LSB]
#define LIS3DH_SENSITIVITY_16G        (uint8_t)12 // sensitivity with 12 g full scale [mg/LSB]

// @brief High pass filter status
#define LIS3DH_HIGHPASSFILTER_DISABLE   ((uint8_t)0x00)
#define LIS3DH_HIGHPASSFILTER_ENABLE    ((uint8_t)0x08)

// @brief High pass filter click status
#define LIS3DH_HPF_CLICK_DISABLE        ((uint8_t)0x00)
#define LIS3DH_HPF_CLICK_ENABLE         ((uint8_t)0x04)

// @brief High pass filter AOI1 status
#define LIS3DH_HPF_AOI1_DISABLE         ((uint8_t)0x00)
#define LIS3DH_HPF_AOI1_ENABLE          ((uint8_t)0x01)

// @brief  High pass filter AOI2 status
#define LIS3DH_HPF_AOI2_DISABLE         ((uint8_t)0x00)
#define LIS3DH_HPF_AOI2_ENABLE          ((uint8_t)0x02)

// @brief Interrupt1 configuration definition
#define LIS3DH_IT1_CLICK                ((uint8_t)0x80)
#define LIS3DH_IT1_AOI1                 ((uint8_t)0x40)
#define LIS3DH_IT1_AOI2                 ((uint8_t)0x20)
#define LIS3DH_IT1_DRY1                 ((uint8_t)0x10)
#define LIS3DH_IT1_DRY2                 ((uint8_t)0x08)
#define LIS3DH_IT1_WTM                  ((uint8_t)0x04)
#define LIS3DH_IT1_OVERRUN              ((uint8_t)0x02)

// @brief  status
#define LIS3DH_FIFO_DISABLE             ((uint8_t)0x00)   // Disable FIFO buffer
#define LIS3DH_FIFO_ENABLE              ((uint8_t)0x40)   // Enable FIFO buffer

// @brief SPI wire mode
#define LIS3DH_SIM_4_WIRE               ((uint8_t)0x00) //SPI 4-wire mode
#define LIS3DH_SIM_3_WIRE               ((uint8_t)0x01) //SPI 3-wire mode

typedef enum {
  ACC_OK = 0,
  ACC_ERROR,
} LIS3DH_StatusTypedef;

// LIS3DH ACC struct
typedef struct {
  uint8_t PowerMode;          // Power-down/Normal Mode
  uint8_t OutputDataRate;     // Output data rate
  uint8_t AxesEnable;         // Axes enable
  uint8_t HighResolution;     // High resolution enabling/disabling
  uint8_t BlockDataUpdate;    // Block data update
  uint8_t Endianness;         // Endian data selection
  uint8_t FullScale;          // Full scale selection
  uint8_t SpiWireMode;        // SPI wire mode selection
} LIS3DH_InitTypedef;

// High pass filter struct
typedef struct {
  uint8_t HPF_ModeSelection;    // Internal filter mode
  uint8_t HPF_CutOffFrequency;  // High pass filter cut-off frequency
  uint8_t HPF_AOI1;             // HPF_enabling/disabling for AOI function on interrupt 1
  uint8_t HPF_AOI2;             // HPF_enabling/disabling for AOI function on interrupt 2
} LIS3DH_FilterConfigTypedef;

extern LIS3DH_StatusTypedef AccStatus;
extern int32_t Acc[3];

void      LIS3DH_Handler      (void);
uint8_t   LIS3DH_Configure    (void);
void      LIS3DH_Init         (LIS3DH_InitTypedef *LIS3DH_InitStruct);

void      LIS3DH_RebootCmd    (void);
uint8_t   LIS3DH_ReadID       (void);
uint8_t   LIS3DH_GetDataStatus(void);
void      LIS3DH_ReadXYZAcc   (int32_t* piData);

void      LIS3DH_FilterConfig (LIS3DH_FilterConfigTypedef *LIS3DH_FilterStruct);
void      LIS3DH_FilterCmd    (uint8_t HighPassFilterState);

void      LIS3DH_AccIT1Enable (uint8_t interrupts);
void      LIS3DH_AccIT1Disable(uint8_t interrupts);

void      LIS3DH_Write  (uint8_t* pBuffer, uint8_t WriteAddr, uint16_t NumByteToWrite);
void      LIS3DH_Read   (uint8_t* pBuffer, uint8_t ReadAddr, uint16_t NumByteToRead);

#endif