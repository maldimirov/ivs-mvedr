
#include "stm32f4xx_hal.h"

void HAL_UART_MspInit(UART_HandleTypeDef* huart)
{
  GPIO_InitTypeDef GPIO_InitStruct;

  if(huart->Instance == USART1) {
    // Peripheral clock enable
    __USART1_CLK_ENABLE();
  
    // GPIO Ports Clock Enable
    __GPIOA_CLK_ENABLE();

    // USART1 GPIO Configuration
    // PA9  ------> USART1_TX
    // PA10 ------> USART1_RX     

    GPIO_InitStruct.Pin = GPIO_PIN_9 | GPIO_PIN_10;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_LOW;
    GPIO_InitStruct.Alternate = GPIO_AF7_USART1;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
  }
}

void HAL_UART_MspDeInit(UART_HandleTypeDef* huart)
{
  if(huart->Instance==USART1) {
    // Peripheral clock disable
    __USART1_CLK_DISABLE();
  
    // USART1 GPIO Configuration  
    // PA9  ------> USART1_TX
    // PA10 ------> USART1_RX 

    HAL_GPIO_DeInit(GPIOA, GPIO_PIN_9 | GPIO_PIN_10);
  }
}

void HAL_SD_MspInit(SD_HandleTypeDef* hsd)
{
  GPIO_InitTypeDef GPIO_InitStruct;

  if(hsd->Instance == SDIO) {
    // Peripheral clock enable
    __SDIO_CLK_ENABLE();
  
    // GPIO Ports Clock Enable
    __GPIOC_CLK_ENABLE();
    __GPIOD_CLK_ENABLE();

    // SDIO GPIO Configuration  
    // PC8  ------> SDIO_D0
    // PC9  ------> SDIO_D1
    // PC10 ------> SDIO_D2
    // PC11 ------> SDIO_D3
    // PC12 ------> SDIO_CK
    // PD2  ------> SDIO_CMD 

    GPIO_InitStruct.Pin = GPIO_PIN_8 | GPIO_PIN_9 | GPIO_PIN_10 | GPIO_PIN_11 | GPIO_PIN_12;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_LOW;
    GPIO_InitStruct.Alternate = GPIO_AF12_SDIO;
    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = GPIO_PIN_2;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_LOW;
    GPIO_InitStruct.Alternate = GPIO_AF12_SDIO;
    HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);
  }
}

void HAL_SD_MspDeInit(SD_HandleTypeDef* hsd)
{
  if(hsd->Instance==SDIO) {
    // Peripheral clock disable
    __SDIO_CLK_DISABLE();
  
    // SDIO GPIO Configuration  
    // PC8  ------> SDIO_D0
    // PC9  ------> SDIO_D1
    // PC10 ------> SDIO_D2
    // PC11 ------> SDIO_D3
    // PC12 ------> SDIO_CK
    // PD2  ------> SDIO_CMD

    HAL_GPIO_DeInit(GPIOC, GPIO_PIN_8 | GPIO_PIN_9 | GPIO_PIN_10 | GPIO_PIN_11 | GPIO_PIN_12);

    HAL_GPIO_DeInit(GPIOD, GPIO_PIN_2);
  }
}

