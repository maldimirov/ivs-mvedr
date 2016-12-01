
#ifndef __L3GD20_H
#define __L3GD20_H

// Read/Write command
#define L3GD20_READWRITE_CMD          ((uint8_t)0x80)

// Multiple byte read/write command
#define L3GD20_MULTIPLEBYTE_CMD       ((uint8_t)0x40)

// Dummy Byte Send by the SPI Master device in order to generate the Clock to the Slave device
#define L3GD20_DUMMY_BYTE             ((uint8_t)0x00)

// Register Mapping
#define L3GD20_WHO_AM_I_ADDR          0x0F  // device identification register
#define L3GD20_CTRL_REG1_ADDR         0x20  // Control register 1
#define L3GD20_CTRL_REG2_ADDR         0x21  // Control register 2
#define L3GD20_CTRL_REG3_ADDR         0x22  // Control register 3
#define L3GD20_CTRL_REG4_ADDR         0x23  // Control register 4
#define L3GD20_CTRL_REG5_ADDR         0x24  // Control register 5
#define L3GD20_REFERENCE_REG_ADDR     0x25  // Reference register
#define L3GD20_OUT_TEMP_ADDR          0x26  // Out temp register
#define L3GD20_STATUS_REG_ADDR        0x27  // Status register
#define L3GD20_OUT_X_L_ADDR           0x28  // Output Register X
#define L3GD20_OUT_X_H_ADDR           0x29  // Output Register X
#define L3GD20_OUT_Y_L_ADDR           0x2A  // Output Register Y
#define L3GD20_OUT_Y_H_ADDR           0x2B  // Output Register Y
#define L3GD20_OUT_Z_L_ADDR           0x2C  // Output Register Z
#define L3GD20_OUT_Z_H_ADDR           0x2D  // Output Register Z
#define L3GD20_FIFO_CTRL_REG_ADDR     0x2E  // Fifo control Register
#define L3GD20_FIFO_SRC_REG_ADDR      0x2F  // Fifo src Register

#define L3GD20_INT1_CFG_ADDR          0x30  // Interrupt 1 configuration Register
#define L3GD20_INT1_SRC_ADDR          0x31  // Interrupt 1 source Register
#define L3GD20_INT1_TSH_XH_ADDR       0x32  // Interrupt 1 Threshold X register
#define L3GD20_INT1_TSH_XL_ADDR       0x33  // Interrupt 1 Threshold X register
#define L3GD20_INT1_TSH_YH_ADDR       0x34  // Interrupt 1 Threshold Y register
#define L3GD20_INT1_TSH_YL_ADDR       0x35  // Interrupt 1 Threshold Y register
#define L3GD20_INT1_TSH_ZH_ADDR       0x36  // Interrupt 1 Threshold Z register
#define L3GD20_INT1_TSH_ZL_ADDR       0x37  // Interrupt 1 Threshold Z register
#define L3GD20_INT1_DURATION_ADDR     0x38  // Interrupt 1 DURATION register

// WHO_AM_I register value
#define I_AM_L3GD20                 ((uint8_t)0xD4)

// @brief Power_Mode_selection
#define L3GD20_MODE_POWERDOWN       ((uint8_t)0x00)
#define L3GD20_MODE_ACTIVE          ((uint8_t)0x08)

// @brief Output data rate selection
#define L3GD20_OUTPUT_DATARATE_1    ((uint8_t)0x00) //95Hz
#define L3GD20_OUTPUT_DATARATE_2    ((uint8_t)0x40) //190Hz
#define L3GD20_OUTPUT_DATARATE_3    ((uint8_t)0x80) //380Hz
#define L3GD20_OUTPUT_DATARATE_4    ((uint8_t)0xC0) //760Hz

// @brief Axes selection
#define L3GD20_X_ENABLE             ((uint8_t)0x02)
#define L3GD20_Y_ENABLE             ((uint8_t)0x01)
#define L3GD20_Z_ENABLE             ((uint8_t)0x04)
#define L3GD20_AXES_ENABLE          ((uint8_t)0x07)
#define L3GD20_AXES_DISABLE         ((uint8_t)0x00)

// @brief Band width selection
#define L3GD20_BANDWIDTH_1          ((uint8_t)0x00) //12.5  12.5  20   30
#define L3GD20_BANDWIDTH_2          ((uint8_t)0x10) // 25    25   25   35
#define L3GD20_BANDWIDTH_3          ((uint8_t)0x20) // 25    50   50   50
#define L3GD20_BANDWIDTH_4          ((uint8_t)0x30) // 25    70   100  100

// @brief Full scale selection
#define L3GD20_FULLSCALE_250        ((uint8_t)0x00) //+-250 dps
#define L3GD20_FULLSCALE_500        ((uint8_t)0x10) //+-500 dps
#define L3GD20_FULLSCALE_2000       ((uint8_t)0x20) //+-2000 dps
#define L3GD20_FULLSCALE_SELECTION  ((uint8_t)0x30) //

// @brief Full scale sensitivity
#define L3GD20_SENSITIVITY_250DPS   875    // gyroscope sensitivity with 250 dps full scale [mDPS/LSB]
#define L3GD20_SENSITIVITY_500DPS   1750   // gyroscope sensitivity with 500 dps full scale [mDPS/LSB]
#define L3GD20_SENSITIVITY_2000DPS  7000   // gyroscope sensitivity with 2000 dps full scale [mDPS/LSB]

// @brief Block data update
#define L3GD20_BlockDataUpdate_Continous  ((uint8_t)0x00)
#define L3GD20_BlockDataUpdate_Single     ((uint8_t)0x80)

// @brief Endian data selection
#define L3GD20_BLE_LSB              ((uint8_t)0x00)
#define L3GD20_BLE_MSB              ((uint8_t)0x40)

// @brief High pass filter status
#define L3GD20_HIGHPASSFILTER_DISABLE     ((uint8_t)0x00)
#define L3GD20_HIGHPASSFILTER_ENABLE      ((uint8_t)0x10)

// @brief INT1 interrupt status
#define L3GD20_INT1INTERRUPT_DISABLE      ((uint8_t)0x00)
#define L3GD20_INT1INTERRUPT_ENABLE       ((uint8_t)0x80)

// @brief INT2 interrupt status
#define L3GD20_INT2INTERRUPT_DISABLE      ((uint8_t)0x00)
#define L3GD20_INT2INTERRUPT_ENABLE       ((uint8_t)0x08)

// @brief INT1 interrupt active edge
#define L3GD20_INT1INTERRUPT_LOW_EDGE     ((uint8_t)0x20)
#define L3GD20_INT1INTERRUPT_HIGH_EDGE    ((uint8_t)0x00)

// @brief Boot mode selection
#define L3GD20_BOOT_NORMALMODE            ((uint8_t)0x00)
#define L3GD20_BOOT_REBOOTMEMORY          ((uint8_t)0x80)

// @brief High pass filter mode
#define L3GD20_HPM_NORMAL_MODE_RES        ((uint8_t)0x00)
#define L3GD20_HPM_REF_SIGNAL             ((uint8_t)0x10)
#define L3GD20_HPM_NORMAL_MODE            ((uint8_t)0x20)
#define L3GD20_HPM_AUTORESET_INT          ((uint8_t)0x30)

// @brief High pass cutoff frequency
#define L3GD20_HPFCF_0    0x00
#define L3GD20_HPFCF_1    0x01
#define L3GD20_HPFCF_2    0x02
#define L3GD20_HPFCF_3    0x03
#define L3GD20_HPFCF_4    0x04
#define L3GD20_HPFCF_5    0x05
#define L3GD20_HPFCF_6    0x06
#define L3GD20_HPFCF_7    0x07
#define L3GD20_HPFCF_8    0x08
#define L3GD20_HPFCF_9    0x09

// @brief SPI wire mode
#define L3GD20_SIM_4_WIRE               ((uint8_t)0x00) //SPI 4-wire mode
#define L3GD20_SIM_3_WIRE               ((uint8_t)0x01) //SPI 3-wire mode

typedef enum {
  GYRO_OK = 0,
  GYRO_ERROR,
} L3GD20_StatusTypedef;

typedef struct {
  uint8_t Power_Mode;         // Power-down/Sleep/Normal Mode
  uint8_t Output_DataRate;    // OUT data rate
  uint8_t Axes_Enable;        // Axes enable
  uint8_t Band_Width;         // Bandwidth selection
  uint8_t BlockData_Update;   // Block Data Update
  uint8_t Endianness;         // Endian Data selection
  uint8_t Full_Scale;         // Full Scale selection
  uint8_t SpiWireMode;        // SPI wire mode selection
} L3GD20_InitTypedef;

// GYRO High Pass Filter struct
typedef struct {
  uint8_t HighPassFilter_Mode_Selection;      // Internal filter mode
  uint8_t HighPassFilter_CutOff_Frequency;    // High pass filter cut-off frequency
} L3GD20_FilterConfigTypedef;

// L3GD20 Interrupt struct
typedef struct {
  uint8_t Latch_Request;                      // Latch interrupt request into CLICK_SRC register
  uint8_t Interrupt_Axes;                     // X, Y, Z Axes Interrupts
  uint8_t Interrupt_ActiveEdge;               //  Interrupt Active edge
} L3GD20_InterruptConfigTypedef;

extern L3GD20_StatusTypedef GyroStatus;
extern int32_t AngRate[3];

void      L3GD20_Handler    (void);
uint8_t   L3GD20_Configure  (void);
void      L3GD20_Init       (L3GD20_InitTypedef *L3GD20_InitStruct);

uint8_t   L3GD20_ReadID         (void);
uint8_t   L3GD20_GetDataStatus  (void);
void      L3GD20_ReadXYZAngRate (void);

void      L3GD20_FilterConfig   (L3GD20_FilterConfigTypedef *L3GD20_FilterStruct);
void      L3GD20_FilterCmd      (uint8_t HighPassFilterState);

void      L3GD20_INT1InterruptConfig(L3GD20_InterruptConfigTypedef *L3GD20_IntConfigStruct);

void      L3GD20_INT1InterruptCmd   (uint8_t InterruptState);
void      L3GD20_INT2InterruptCmd   (uint8_t InterruptState);

void      L3GD20_Write      (uint8_t* pBuffer, uint8_t WriteAddr, uint16_t NumByteToWrite);
void      L3GD20_Read       (uint8_t* pBuffer, uint8_t ReadAddr, uint16_t NumByteToRead);

#endif
