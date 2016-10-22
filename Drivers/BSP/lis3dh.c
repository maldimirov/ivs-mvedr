
#include "main.h"
#include "bsp.h"
#include "lis3dh.h"
#include "config.h"

LIS3DH_StatusTypedef AccStatus = ACC_ERROR;
int32_t Acc[3], dAcc[3] = {0, 0, 0}, aAcc[3] = {0, 0, 0};
char adata[80];

void LIS3DH_Handler(void)
{
  int32_t AccLevel;
  uint8_t dataStatus = LIS3DH_GetDataStatus();

  if (dataStatus & 0x08) {
    LIS3DH_ReadXYZAcc(Acc);

    //Check if trigger level was reached and do something
    if(aAcc[0] == 0 && aAcc[1] == 0 && aAcc[2] == 0) {
      aAcc[0] = Acc[0]; aAcc[1] = Acc[1]; aAcc[2] = Acc[2];
    }
    dAcc[0] = Acc[0] - aAcc[0];
    dAcc[1] = Acc[1] - aAcc[1];
    dAcc[2] = Acc[2] - aAcc[2];
    aAcc[0] = (aAcc[0] + Acc[0]) / 2;
    aAcc[1] = (aAcc[1] + Acc[1]) / 2;
    aAcc[2] = (aAcc[2] + Acc[2]) / 2;
    AccLevel = atoi(CFG_GlobVarsStruct.accLevel);
    if(dAcc[0] > AccLevel || dAcc[1] > AccLevel || dAcc[2] > AccLevel) {
      //TRIGGER LEVEL WAS REACHED
      //SAVE YOURSELF
    }
  }
}

uint8_t LIS3DH_Configure(void)
{
  uint8_t retVal = ACC_ERROR;

  LIS3DH_InitTypedef LIS3DH_InitStruct;
  LIS3DH_FilterConfigTypedef LIS3DH_FilterStruct;

  if(LIS3DH_ReadID() == I_AM_LIS3DH) {
    // Configure Mems LIS3DH
    LIS3DH_InitStruct.PowerMode = LIS3DH_MODE_NORMAL;
    LIS3DH_InitStruct.OutputDataRate = LIS3DH_ODR_1250_HZ;
    LIS3DH_InitStruct.AxesEnable = LIS3DH_AXES_ENABLE;
    LIS3DH_InitStruct.HighResolution = LIS3DH_HR_ENABLE;
    LIS3DH_InitStruct.BlockDataUpdate = LIS3DH_BlockUpdate_Continuous;
    LIS3DH_InitStruct.Endianness = LIS3DH_BLE_LSB;
    LIS3DH_InitStruct.FullScale = LIS3DH_FULLSCALE_4G;
    LIS3DH_Init(&LIS3DH_InitStruct);

    LIS3DH_FilterStruct.HPF_ModeSelection = LIS3DH_HPM_NORMAL_MODE;
    LIS3DH_FilterStruct.HPF_CutOffFrequency = LIS3DH_HPFCF_0;
    LIS3DH_FilterStruct.HPF_AOI1 = LIS3DH_HPF_AOI1_DISABLE;
    LIS3DH_FilterStruct.HPF_AOI2 = LIS3DH_HPF_AOI2_DISABLE;
    LIS3DH_FilterConfig(&LIS3DH_FilterStruct);

    LIS3DH_FilterCmd(LIS3DH_HIGHPASSFILTER_DISABLE);

    //./LIS3DH_AccIT1Enable(LIS3DH_IT1_DRY1);

    retVal = ACC_OK;
  }

  return retVal;
}

/**
  * @brief  Set LIS3DH Initialization.
  * @param  LIS3DH_InitStruct: pointer to a LIS3DH_InitTypeDef structure
  *         that contains the configuration setting for the LIS3DH.
  * @retval None
  */
void LIS3DH_Init(LIS3DH_InitTypedef *LIS3DH_InitStruct)
{
  uint8_t ctrl1 = 0x00, ctrl4 = 0x00;

  BSP_AccInit();

  // Configure MEMS: data rate, power mode, full scale and axes
  ctrl1 |= (uint8_t) (LIS3DH_InitStruct->PowerMode | LIS3DH_InitStruct->OutputDataRate | \
                      LIS3DH_InitStruct->AxesEnable);

  ctrl4 |= (uint8_t) (LIS3DH_InitStruct->BlockDataUpdate | LIS3DH_InitStruct->Endianness | \
                      LIS3DH_InitStruct->FullScale | LIS3DH_InitStruct->HighResolution);

  // Write value to MEMS CTRL_REG1 regsister
  LIS3DH_Write(&ctrl1, LIS3DH_CTRL_REG1_ADDR, 1);

  // Write value to MEMS CTRL_REG4 regsister
  LIS3DH_Write(&ctrl4, LIS3DH_CTRL_REG4_ADDR, 1);
}

/**
  * @brief  Set High Pass Filter Modality
  * @param  FilterStruct: contains the configuration setting for the L3GD20.
  * @retval None
  */
void LIS3DH_FilterConfig(LIS3DH_FilterConfigTypedef *LIS3DH_FilterStruct)
{
  uint8_t tmpreg = 0x00;

  // Read CTRL_REG2 register
  LIS3DH_Read(&tmpreg, LIS3DH_CTRL_REG2_ADDR, 1);

  tmpreg &= 0x0C;

  // Configure MEMS: mode and cutoff frquency
  tmpreg |= (uint8_t)(LIS3DH_FilterStruct->HPF_ModeSelection | LIS3DH_FilterStruct->HPF_CutOffFrequency | \
                      LIS3DH_FilterStruct->HPF_AOI1 | LIS3DH_FilterStruct->HPF_AOI2);

  // Write value to MEMS CTRL_REG2 regsister
  LIS3DH_Write(&tmpreg, LIS3DH_CTRL_REG2_ADDR, 1);
}

/**
  * @brief  Enable or Disable High Pass Filter
  * @param  HighPassFilterState: new state of the High Pass Filter feature.
  *      This parameter can be:
  *         @arg: LIS3DH_HIGHPASSFILTER_DISABLE
  *         @arg: LIS3DH_HIGHPASSFILTER_ENABLE
  * @retval None
  */
void LIS3DH_FilterCmd(uint8_t HighPassFilterState)
{
  uint8_t tmpreg = 0x00;

  // Read CTRL_REG2 register
  LIS3DH_Read(&tmpreg, LIS3DH_CTRL_REG2_ADDR, 1);

  tmpreg &= 0xF7;

  tmpreg |= HighPassFilterState;

  // Write value to MEMS CTRL_REG2 regsister
  LIS3DH_Write(&tmpreg, LIS3DH_CTRL_REG2_ADDR, 1);
}

void LIS3DH_AccIT1Enable(uint8_t interrupts)
{
  uint8_t tmpreg = 0x00;

  // Read CTRL_REG3 register
  LIS3DH_Read(&tmpreg, LIS3DH_CTRL_REG3_ADDR, 1);

  // Enable IT1
  tmpreg |= interrupts;

  // Write value to MEMS CTRL_REG3 register
  LIS3DH_Write(&tmpreg, LIS3DH_CTRL_REG3_ADDR, 1);
}

/**
  * @brief Disable LSM303DLHC Interrupt1
  * @param  LSM303DLHC_IT: specifies the LSM303DLHC interrupt source to be enabled.
  *           This parameter can be any combination of the following values:
  *         @arg   LSM303DLHC_IT1_CLICK
  *         @arg   LSM303DLHC_IT1_AOI1
  *         @arg   LSM303DLHC_IT1_AOI2
  *         @arg   LSM303DLHC_IT1_DRY1
  *         @arg   LSM303DLHC_IT1_WTM
  *         @arg   LSM303DLHC_IT1_OVERRUN.
  * @retval None
  */
void LIS3DH_AccIT1Disable(uint8_t interrupts)
{
  uint8_t tmpreg = 0x00;

  // Read CTRL_REG3 register
  LIS3DH_Read(&tmpreg, LIS3DH_CTRL_REG3_ADDR, 1);

  // Disable IT1
  tmpreg &= ~interrupts;

  // Write value to MEMS CTRL_REG3 register
  LIS3DH_Write(&tmpreg, LIS3DH_CTRL_REG3_ADDR, 1);
}

/**
  * @brief  Reboot memory content of LIS3DH
  * @param  None
  * @retval None
  */
void LIS3DH_RebootCmd(void)
{
  uint8_t tmpreg;

  // Read CTRL_REG5 register
  LIS3DH_Read(&tmpreg, LIS3DH_CTRL_REG5_ADDR, 1);

  // Enable or Disable the reboot memory
  tmpreg |= LIS3DH_BOOT_REBOOTMEMORY;

  // Write value to MEMS CTRL_REG5 regsister
  LIS3DH_Read(&tmpreg, LIS3DH_CTRL_REG5_ADDR, 1);
}

uint8_t LIS3DH_ReadID(void)
{
  uint8_t tmp = 0;

  // Configure the low level interface
  //BSP_AccInit();

  // Read WHO I AM register
  LIS3DH_Read(&tmp, LIS3DH_WHO_AM_I_ADDR, 1);

  // Return the ID
  return (uint8_t)tmp;
}

/**
  * @brief  Get status for LIS3DH data
  * @param  None
  * @retval Data status in a LIS3DH Data
  */
uint8_t LIS3DH_GetDataStatus(void)
{
  uint8_t tmpreg;

  // Read STATUS_REG register
  LIS3DH_Read(&tmpreg, LIS3DH_STATUS_REG_ADDR, 1);

  return tmpreg;
}

void LIS3DH_ReadXYZAcc(int32_t * piData)
{
  uint8_t tmpbuffer[6] = {0};
  int16_t RawData[3] = {0};
  uint8_t tmpreg = 0, div = 1;
  uint8_t sensitivity = 0;
  int i =0;

  LIS3DH_Read(&tmpreg, LIS3DH_CTRL_REG4_ADDR, 1);

  LIS3DH_Read(tmpbuffer, LIS3DH_OUT_X_L_ADDR, 6);

  // check in the control register 4 the data alignment (Big Endian or Little Endian)
  if(!(tmpreg & LIS3DH_BLE_MSB)) {
    for(i = 0; i < 3; i++)
    {
      RawData[i]=(int16_t)(((uint16_t)tmpbuffer[2*i+1] << 8) + tmpbuffer[2*i]);
    }
  } else {
    for(i = 0; i < 3; i++)
    {
      RawData[i]=(int16_t)(((uint16_t)tmpbuffer[2*i] << 8) + tmpbuffer[2*i+1]);
    }
  }

  // Switch the sensitivity value set in the CRTL4
  switch(tmpreg & LIS3DH_FULLSCALE_SELECTION) {
    case LIS3DH_FULLSCALE_2G :
      sensitivity = LIS3DH_SENSITIVITY_2G;
      break;
    case LIS3DH_FULLSCALE_4G :
      sensitivity = LIS3DH_SENSITIVITY_4G;
      break;
    case LIS3DH_FULLSCALE_8G :
      sensitivity = LIS3DH_SENSITIVITY_8G;
      break;
    case LIS3DH_FULLSCALE_16G :
      sensitivity = LIS3DH_SENSITIVITY_16G;
      break;
  }

  LIS3DH_Read(&tmpreg, LIS3DH_CTRL_REG5_ADDR, 1);

  if (tmpreg & LIS3DH_FIFO_ENABLE) {
    div = 64;
  } else {
    div = 16;
  }

  // divide by sensitivity
  for(i = 0; i < 3; i++)
  {
    piData[i] = ((int32_t)RawData[i] * (int32_t)sensitivity) / (int32_t)div;
  }

}

void LIS3DH_Write(uint8_t* pBuffer, uint8_t WriteAddr, uint16_t NumByteToWrite)
{
  // Configure the MS bit:
  //  - When 0, the address will remain unchanged in multiple read/write commands.
  //  - When 1, the address will be auto incremented in multiple read/write commands.

  if(NumByteToWrite > 0x01) {
    WriteAddr |= (uint8_t)LIS3DH_MULTIPLEBYTE_CMD;
  }

  // Set chip select Low at the start of the transmission
  BSP_ACC_CS_LOW();

  // Send the Address of the indexed register
  BSP_SPI1_WriteRead(WriteAddr);

  // Send the data that will be written into the device (MSB First)
  while(NumByteToWrite >= 0x01)
  {
    BSP_SPI1_WriteRead(*pBuffer);
    NumByteToWrite--;
    pBuffer++;
  }

  // Set chip select High at the end of the transmission
  BSP_ACC_CS_HIGH();
}

void LIS3DH_Read(uint8_t* pBuffer, uint8_t ReadAddr, uint16_t NumByteToRead)
{
  if(NumByteToRead > 0x01) {
    ReadAddr |= (uint8_t)(LIS3DH_READWRITE_CMD | LIS3DH_MULTIPLEBYTE_CMD);
  } else {
    ReadAddr |= (uint8_t)LIS3DH_READWRITE_CMD;
  }

  // Set chip select Low at the start of the transmission
  BSP_ACC_CS_LOW();

  // Send the Address of the indexed register
  BSP_SPI1_WriteRead(ReadAddr);

  // Receive the data that will be read from the device (MSB First)
  while(NumByteToRead > 0x00)
  {
    // Send dummy byte (0x00) to generate the SPI clock to GYRO (Slave device)
    *pBuffer = BSP_SPI1_WriteRead(LIS3DH_DUMMY_BYTE);
    NumByteToRead--;
    pBuffer++;
  }

  // Set chip select High at the end of the transmission
  BSP_ACC_CS_HIGH();
}