
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


void LSM303C_ReadXYZMag(void)
{
  uint8_t tmpbuffer[6] = {0};
  int16_t RawData[3] = {0};
  uint8_t tmpreg = 0;
  int i =0;

  LSM303C_Read(&tmpreg, LSM303C_CTRL_REG4_M, 1);

  LSM303C_Read(tmpbuffer, LSM303C_OUT_X_L_M, 6);

  // check in the control register 4 the data alignment (Big Endian or Little Endian)
  if(!(tmpreg & LSM303C_BLE_MSB)) {
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

  for (i = 0; i < 3; i++) MagInt[i] = RawData[i];
}


uint8_t LSM303C_GetDataStatus(void)
{
  uint8_t tmpreg;

  // Read STATUS_REG register
  LSM303C_Read(&tmpreg, LSM303C_STATUS_REG_M, 1);

  return tmpreg;
}

uint8_t LSM303C_Configure(void)
{
  uint8_t retVal = MAG_ERROR;
  uint8_t reg;

  BSP_MagInit();
  if(LSM303C_ReadID() == I_AM_LSM303C_M) {
    // Configure Mems LSM303C
    // Write value to MAG CTRL_REG1 regsister
    reg = 0x50;
    LSM303C_Write(&reg, LSM303C_CTRL_REG1_M, 1);

    // Write value to MAG CTRL_REG2 regsister
    reg = 0x60;
    LSM303C_Write(&reg, LSM303C_CTRL_REG2_M, 1);

    // Write value to MAG CTRL_REG3 regsister
    reg = 0xA5;
    LSM303C_Write(&reg, LSM303C_CTRL_REG3_M, 1);

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

  // Set chip select Low at the start of the transmission
  BSP_MAG_CS_LOW();

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
  BSP_MAG_CS_HIGH();
}

void LSM303C_Read(uint8_t* pBuffer, uint8_t ReadAddr, uint16_t NumByteToRead)
{
  if(NumByteToRead > 0x01) {
    ReadAddr |= (uint8_t)(LSM303C_READWRITE_CMD | LSM303C_MULTIPLEBYTE_CMD);
  } else {
    ReadAddr |= (uint8_t)LSM303C_READWRITE_CMD;
  }

  // Set chip select Low at the start of the transmission
  BSP_MAG_CS_LOW();

  // Send the Address of the indexed register
  BSP_SPI1_WriteRead(ReadAddr);

  GPIO_InitTypeDef GPIO_InitStruct;
  uint8_t tmp;

    GPIO_InitStruct.Pin = GPIO_PIN_7;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_PULLDOWN;
    GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;
    GPIO_InitStruct.Alternate = 0;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);


  // Receive the data that will be read from the device (MSB First)
  while(NumByteToRead > 0x00)
  {
    // Send dummy byte (0x00) to generate the SPI clock to GYRO (Slave device)

    tmp = BSP_SPI1_WriteRead(LSM303C_DUMMY_BYTE);
    *pBuffer = tmp;
    NumByteToRead--;
    pBuffer++;
  }

    GPIO_InitStruct.Pin = GPIO_PIN_7;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF5_SPI1;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  // Set chip select High at the end of the transmission
  BSP_MAG_CS_HIGH();
}