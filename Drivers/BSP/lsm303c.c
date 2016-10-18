
#include "main.h"
#include "lsm303c.h"
#include "bsp.h"

LSM303C_StatusTypedef MagStatus = MAG_ERROR;
int32_t MagInt[3] = {0, 0, 0};

void LSM303C_Handler(void)
{
  if (LSM303C_GetDataStatus() & 0x08) {
      LSM303C_ReadXYZMag();
  }
}

uint8_t LSM303C_Configure(void)
{
  //Configure only the Magnetometer
  uint8_t retVal = MAG_ERROR;

  LSM303C_InitTypedef LSM303C_InitStruct;

  // Configure Mems LSM303C initialisation structure
  //Confgiure values for LSM303C_CTRL_REG1_M
  LSM303C_InitStruct.TempMode = LSM303C_M_TEMP_DISABLE; //Reg1
  LSM303C_InitStruct.XYOperativeMode = LSM303C_M_XY_ULTRAHIGH_PERF; //Reg1
  LSM303C_InitStruct.OutputDataRate = LSM303C_M_ODR_80_HZ; //Reg1

  //Confgiure values for LSM303C_CTRL_REG2_M
  LSM303C_InitStruct.FullScale = LSM303C_M_FULL_SCALE; //Reg2

  //Confgiure values for LSM303C_CTRL_REG3_M
  LSM303C_InitStruct.PowerMode = LSM303C_M_LOW_POWER_DISABLE; //Reg3
  LSM303C_InitStruct.InterfaceModeI2C = LSM303C_M_I2C_DISABLE; //Reg3
  LSM303C_InitStruct.InterfaceModeSPI = LSM303C_M_SPI_READ_WRITE; //Reg3
  LSM303C_InitStruct.SystemOperatingMode = LSM303C_M_SOM_CONT_CONV; //Reg3

  //Confgiure values for LSM303C_CTRL_REG4_M
  LSM303C_InitStruct.ZOperativeMode = LSM303C_M_Z_ULTRAHIGH_PERF; //Reg4
  LSM303C_InitStruct.Endianness = LSM303C_M_BLE_LSB; //Reg4

  //Confgiure values for LSM303C_CTRL_REG5_M
  LSM303C_InitStruct.BlockDataUpdate = LSM303C_M_BDU_CONTINUOUS; //Reg5

  //Init before readID, beacuse the init operation sets the proper I2C and SPI
  //communication paramters
  LSM303C_Init(&LSM303C_InitStruct);

  if(LSM303C_ReadID() == I_AM_LSM303C_M) {
    retVal = MAG_OK;
  }

  return retVal;
}

void LSM303C_Init(LSM303C_InitTypedef *LSM303C_InitStruct)
{
    uint8_t ctrl1 = 0x00, ctrl2 = 0x00, ctrl3 = 0x00, ctrl4 = 0x00, ctrl5 = 0x00;

    BSP_MagInit();

    // Configure MEMS: Temp sensor mode, xy operative mode, output data rate
    ctrl1 |= (uint8_t) (LSM303C_InitStruct->TempMode |
                        LSM303C_InitStruct->XYOperativeMode |
                        LSM303C_InitStruct->OutputDataRate);

    // Configure MEMS: Full scale +-16 Gauss
    ctrl2 |= (uint8_t) (LSM303C_InitStruct->FullScale);

    // Configure MEMS: low power mode, I2C, SPI modes, System operating mode
    ctrl3 |= (uint8_t) (LSM303C_InitStruct->PowerMode |
                        LSM303C_InitStruct->InterfaceModeI2C |
                        LSM303C_InitStruct->InterfaceModeSPI |
                        LSM303C_InitStruct->SystemOperatingMode);

    // Configure MEMS: Z axis operative mode, Output endianness
    ctrl4 |= (uint8_t) (LSM303C_InitStruct->ZOperativeMode |
                        LSM303C_InitStruct->Endianness);

    // Configure MEMS: Block data update
    ctrl5 |= (uint8_t) (LSM303C_InitStruct->BlockDataUpdate);

    // Write value to MEMS CTRL_REG1 register
    LSM303C_Write(&ctrl1, LSM303C_CTRL_REG1_M, 1);
    // Write value to MEMS CTRL_REG1 register
    LSM303C_Write(&ctrl2, LSM303C_CTRL_REG2_M, 1);
    // Write value to MEMS CTRL_REG1 register
    LSM303C_Write(&ctrl3, LSM303C_CTRL_REG3_M, 1);
    // Write value to MEMS CTRL_REG4 register
    LSM303C_Write(&ctrl4, LSM303C_CTRL_REG4_M, 1);
    // Write value to MEMS CTRL_REG4 register
    LSM303C_Write(&ctrl5, LSM303C_CTRL_REG5_M, 1);
}

void LSM303C_ReadXYZMag()
{
    MagInt[0] += 1;
    MagInt[1] += 1;
    MagInt[2] += 1;
}

uint8_t LSM303C_GetDataStatus(void)
{
  uint8_t tmpreg;

  // Read STATUS_REG register
  LSM303C_Read(&tmpreg, LSM303C_STATUS_REG_M, 1);

  return tmpreg;
}

uint8_t LSM303C_Configure_old(void)
{
  uint8_t retVal = MAG_ERROR;
  uint8_t reg;

  BSP_MagInit();

  // Enable SPI read/write
  reg = 0x84; // I2C disabled , SPI -R/W, Cont.conv
  LSM303C_Write(&reg, LSM303C_CTRL_REG3_M, 1);

  if(LSM303C_ReadID() == I_AM_LSM303C_M) {
    // Configure Mems LSM303C
    // Write value to MAG CTRL_REG1 regsister
    reg = 0x50;
    LSM303C_Write(&reg, LSM303C_CTRL_REG1_M, 1);

    // Write value to MAG CTRL_REG2 regsister
    reg = 0x60;
    LSM303C_Write(&reg, LSM303C_CTRL_REG2_M, 1);

     // Write value to MAG CTRL_REG4 regsister
    reg = 0x08;
    LSM303C_Write(&reg, LSM303C_CTRL_REG4_M, 1);

    // Write value to MAG CTRL_REG5 regsister
    reg = 0x00;
    LSM303C_Write(&reg, LSM303C_CTRL_REG5_M, 1);

    retVal = MAG_OK;
  }

  return retVal;
}


uint8_t LSM303C_ReadID(void)
{
  uint8_t tmp = 0;

  // Read WHO I AM register
  LSM303C_Read(&tmp, LSM303C_WHO_AM_I_M, 1);

  // Return the ID
  return (uint8_t)tmp;
}


void LSM303C_Write(uint8_t* pBuffer, uint8_t WriteAddr, uint16_t NumByteToWrite)
{
  // Configure the MS bit:
  //  - When 0, the address will remain unchanged in multiple read/write commands.
  //  - When 1, the address will be auto incremented in multiple read/write commands.

  if(NumByteToWrite > 0x01) {
    WriteAddr |= (uint8_t)LSM303C_MULTIPLEBYTE_CMD;
  }

  //Set the uC in 1 line half duplex SPI mode
  hspi1.Init.Direction = SPI_DIRECTION_1LINE;
  HAL_SPI_Init(&hspi1);

  // Set chip select Low at the start of the transmission
  __HAL_SPI_1LINE_TX(&hspi1);
  BSP_MAG_CS_LOW();

  // Send the Address of the indexed register
  HAL_SPI_Transmit(&hspi1, &WriteAddr, 1, SpiTimeout);

  // Send the data that will be written into the device (MSB First)
  HAL_SPI_Transmit(&hspi1, pBuffer, NumByteToWrite, SpiTimeout);

  // Set chip select High at the end of the transmission
  BSP_MAG_CS_HIGH();

  //Reset the uC in 2 line SPI mode
  hspi1.Init.Direction = SPI_DIRECTION_2LINES;
  HAL_SPI_Init(&hspi1);
}

void LSM303C_Read(uint8_t* pBuffer, uint8_t ReadAddr, uint16_t NumByteToRead)
{
  if(NumByteToRead > 0x01) {
    ReadAddr |= (uint8_t)(LSM303C_READWRITE_CMD | LSM303C_MULTIPLEBYTE_CMD);
  } else {
    ReadAddr |= (uint8_t)LSM303C_READWRITE_CMD;
  }

  //Set the uC in 1 line half duplex SPI mode
  hspi1.Init.Direction = SPI_DIRECTION_1LINE;
  HAL_SPI_Init(&hspi1);

  // Set chip select Low at the start of the transmission
  __HAL_SPI_1LINE_TX(&hspi1);
  BSP_MAG_CS_LOW();

  // Send the Address of the indexed register
  HAL_SPI_Transmit(&hspi1, &ReadAddr, 1, SpiTimeout);

  // Receive the data that will be read from the device (MSB First)
  __HAL_SPI_1LINE_RX(&hspi1);
  HAL_SPI_Receive(&hspi1, pBuffer, NumByteToRead, SpiTimeout);

  // Set chip select High at the end of the transmission
  BSP_MAG_CS_HIGH();

  //Reset the uC in 2 line SPI mode
  hspi1.Init.Direction = SPI_DIRECTION_2LINES;
  HAL_SPI_Init(&hspi1);
}