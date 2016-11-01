
#include "main.h"
#include "bsp.h"

UART_HandleTypeDef      huart1, huart2;
SPI_HandleTypeDef       hspi1;
ADC_HandleTypeDef       adc1;
PWR_StateType           Pwr;
IWDG_HandleTypeDef      iwdg1;
TIM_HandleTypeDef    t1;

uint32_t SpiTimeout = SPI_TIMEOUT_MAX;      // Value of Timeout when SPI communication fails

void BSP_Config(void)
{
  // STM32F4xx HAL library initialization:
  //    - Configure the Flash prefetch, instruction and Data caches
  //    - Configure the Systick to generate an interrupt each 1 msec
  //    - Set NVIC Group Priority to 4
  //    - Global MSP (MCU Support Package) initialization
  HAL_Init();

  SystemClock_Config();

  BSP_System_Init();

  BSP_USART1_Init();

  BSP_AccInit();

  BSP_MagInit();

  BSP_GyroInit();

}

/**
  * @brief  System Clock Configuration
  *         The system Clock is configured as follow :
  *            System Clock source            = PLL (HSE)
  *            SYSCLK(Hz)                     = 168000000
  *            HCLK(Hz)                       = 168000000
  *            AHB Prescaler                  = 1
  *            APB1 Prescaler                 = 4
  *            APB2 Prescaler                 = 2
  *            HSE Frequency(Hz)              = 25000000
  *            PLL_M                          = 25
  *            PLL_N                          = 336
  *            PLL_P                          = 2
  *            PLL_Q                          = 7
  *            VDD(V)                         = 3.3
  *            Main regulator output voltage  = Scale1 mode
  *            Flash Latency(WS)              = 5
  * @param  None
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_ClkInitTypeDef RCC_ClkInitStruct;
  RCC_OscInitTypeDef RCC_OscInitStruct;

  // Enable Power Control clock
  __PWR_CLK_ENABLE();

  // The voltage scaling allows optimizing the power consumption when the device is
  // clocked below the maximum system frequency, to update the voltage scaling value
  // regarding system frequency refer to product datasheet.
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  // Enable HSE Oscillator and activate PLL with HSE as source
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 25;
  RCC_OscInitStruct.PLL.PLLN = 336;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 7;
  HAL_RCC_OscConfig(&RCC_OscInitStruct);

  // Select PLL as system clock source and configure the HCLK, PCLK1 and PCLK2
  // clocks dividers
  RCC_ClkInitStruct.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2);
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;
  HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5);
}

void BSP_System_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct;

  // GPIOx clock enable
  __GPIOA_CLK_ENABLE();
  __GPIOB_CLK_ENABLE();
  __GPIOC_CLK_ENABLE();
  __HAL_RCC_LSI_ENABLE();

    // Configure TIMER2 module as 1 microsecond counter
    t1.Instance = TIM2;
    t1.Channel = HAL_TIM_ACTIVE_CHANNEL_1;
    t1.Init.CounterMode = TIM_COUNTERMODE_UP;
    t1.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
    t1.Init.Period = 0; //MAX_TIME_PERIOD; // = 10 seconds
    t1.Init.Prescaler = 83; // F=84MHz / (Prescaler+1) = 1MHz
    t1.Init.RepetitionCounter = 0;
    HAL_TIM_Base_Init(&t1);
    //HAL_TIM_Base_Start_IT(&t1);

  // Configure GPIO pin : PB10 - CHARGE battery
  GPIO_InitStruct.Pin = BSP_CHARGE_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_LOW;
  HAL_GPIO_Init(BSP_CHARGE_GPIO_PORT, &GPIO_InitStruct);

  BSP_CHARGE_ON();

  __ADC1_CLK_ENABLE();

  // Configure GPIO pin : PC1 - VIN_ADC - ch11
  GPIO_InitStruct.Pin = BSP_VIN_ADC_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(BSP_VIN_ADC_GPIO_PORT, &GPIO_InitStruct);

  // Configure GPIO pin : PC3 - VBAT_ADC - ch13
  GPIO_InitStruct.Pin = BSP_VBAT_ADC_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(BSP_VBAT_ADC_GPIO_PORT, &GPIO_InitStruct);

  GPIO_InitStruct.Pin = BSP_INP1_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(BSP_INP1_GPIO_PORT, &GPIO_InitStruct);

  GPIO_InitStruct.Pin = BSP_INP2_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(BSP_INP2_GPIO_PORT, &GPIO_InitStruct);

  adc1.Instance = ADC1;
  adc1.Init.ClockPrescaler = ADC_CLOCKPRESCALER_PCLK_DIV8;
  adc1.Init.ScanConvMode = ENABLE;
  adc1.Init.Resolution = ADC_RESOLUTION10b;
  adc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
  adc1.Init.ExternalTrigConv = ADC_EXTERNALTRIGCONV_T1_CC1;
  adc1.Init.ContinuousConvMode = DISABLE;
  adc1.Init.EOCSelection = EOC_SINGLE_CONV;
  adc1.Init.ClockPrescaler = ADC_CLOCKPRESCALER_PCLK_DIV8;
  HAL_ADC_Init(&adc1);

  Pwr.LowVin = false;
  Pwr.LowVbat = false;

  iwdg1.Instance = IWDG;
  iwdg1.Init.Prescaler = IWDG_PRESCALER_64;
  iwdg1.Init.Reload = 0xFFF;
  HAL_IWDG_Init(&iwdg1);

}

void BSP_GSM_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct;

  // Configure GPIO pin : PC13/PWR
  GPIO_InitStruct.Pin = BSP_PWR_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_LOW;
  HAL_GPIO_Init(BSP_PWR_GPIO_PORT, &GPIO_InitStruct);

  // Configure GPIO pin : PA8/CS
  GPIO_InitStruct.Pin = BSP_EN_BCKP_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_MEDIUM;
  HAL_GPIO_Init(BSP_EN_BCKP_GPIO_PORT, &GPIO_InitStruct);

  BSP_BACKUP_ON();

  // Configure GPIO pin : PC1/DSR
  //GPIO_InitStruct.Pin = BSP_DSR_PIN;
  //HAL_GPIO_Init(BSP_DSR_GPIO_PORT, &GPIO_InitStruct);
  GPIO_InitStruct.Pin = BSP_DSR3_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(BSP_DSR3_GPIO_PORT, &GPIO_InitStruct);

  // Configure GPIO pin : PC3/RI
  //GPIO_InitStruct.Pin = BSP_RI_PIN;
  //HAL_GPIO_Init(BSP_RI_GPIO_PORT, &GPIO_InitStruct);
  GPIO_InitStruct.Pin = BSP_RI3_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(BSP_RI3_GPIO_PORT, &GPIO_InitStruct);

  // Configure GPIO pin : PB0/RST
  GPIO_InitStruct.Pin = BSP_RST_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_LOW;
  HAL_GPIO_Init(BSP_RST_GPIO_PORT, &GPIO_InitStruct);

  // Configure GPIO pin : PB1/RST_DETECT
  GPIO_InitStruct.Pin = BSP_RST_DET_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(BSP_RST_DET_GPIO_PORT, &GPIO_InitStruct);

  // Configure GPIO pin : PB2/PPS
  GPIO_InitStruct.Pin = BSP_PPS_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(BSP_PPS_GPIO_PORT, &GPIO_InitStruct);

  BSP_USART2_Init();
}

// USART1 init function
void BSP_USART1_Init(void)
{
  huart1.Instance = USART1;
  huart1.Init.BaudRate = 19200;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_8;

  HAL_UART_Init(&huart1);

  // Enable the UART Data Register not empty Interrupt
  __HAL_UART_ENABLE_IT(&huart1, UART_IT_RXNE);
  BSP_USART_ReceiveData(USART1);
}

// USART2 init function
void BSP_USART2_Init(void)
{
  huart2.Instance = USART2;
  huart2.Init.BaudRate = 115200;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_RTS_CTS;
  huart2.Init.OverSampling = UART_OVERSAMPLING_8;

  HAL_UART_Init(&huart2);
}

void HAL_UART_MspInit(UART_HandleTypeDef* huart)
{
  GPIO_InitTypeDef GPIO_InitStruct;

  if(huart->Instance == USART1) {
    __USART1_CLK_ENABLE();      // Peripheral clock enable

    __GPIOA_CLK_ENABLE();       // GPIOA clock enable

    // USART1 GPIO Configuration
    // PA9  ------> USART1_TX
    // PA10 ------> USART1_RX

    GPIO_InitStruct.Pin = GPIO_PIN_9 | GPIO_PIN_10;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_LOW;
    GPIO_InitStruct.Alternate = GPIO_AF7_USART1;

    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    // Peripheral interrupt init
    HAL_NVIC_SetPriority(USART1_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(USART1_IRQn);

  } else if(huart->Instance == USART2) {
    __USART2_CLK_ENABLE();      // Peripheral clock enable

    __GPIOA_CLK_ENABLE();       // GPIOA clock enable

    // USART2 GPIO Configuration
    // PA0-WKUP ------> USART2_CTS
    // PA1      ------> USART2_RTS
    // PA2      ------> USART2_TX
    // PA3      ------> USART2_RX

    GPIO_InitStruct.Pin = GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_LOW;
    GPIO_InitStruct.Alternate = GPIO_AF7_USART2;

    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    // Peripheral interrupt init
    HAL_NVIC_SetPriority(USART2_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(USART2_IRQn);
  }
}

void HAL_UART_MspDeInit(UART_HandleTypeDef* huart)
{
  if(huart->Instance == USART1) {
    // Peripheral clock disable
    __USART1_CLK_DISABLE();

    // USART1 GPIO Configuration
    // PA9  ------> USART1_TX
    // PA10 ------> USART1_RX

    HAL_GPIO_DeInit(GPIOA, GPIO_PIN_9 | GPIO_PIN_10);

    // Peripheral interrupt Deinit
    HAL_NVIC_DisableIRQ(USART1_IRQn);

  } else if(huart->Instance == USART2) {
    // Peripheral clock disable
    __USART2_CLK_DISABLE();

    // USART2 GPIO Configuration
    // PA0-WKUP ------> USART2_CTS
    // PA1      ------> USART2_RTS
    // PA2      ------> USART2_TX
    // PA3      ------> USART2_RX

    HAL_GPIO_DeInit(GPIOA, GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3);

    // Peripheral interrupt Deinit
    HAL_NVIC_DisableIRQ(USART2_IRQn);
  }
}

/**
  * @brief  Transmits single data through the USARTx peripheral.
  * @param  USARTx: where x can be 1, 2, 3, 4, 5 or 6 to select the USART or
  *         UART peripheral.
  * @param  Data: the data to transmit.
  * @retval None
  */
void BSP_USART_SendData(USART_TypeDef* USARTx, uint16_t Data)
{
  //while (__HAL_UART_GET_FLAG(&huart2, UART_FLAG_TC) == RESET)
  while ((USARTx->SR & UART_FLAG_TC) == RESET)
  {}
  // Transmit Data
  USARTx->DR = (Data & (uint16_t)0x01FF);
}

/**
  * @brief  Returns the most recent received data by the USARTx peripheral.
  * @param  USARTx: where x can be 1, 2, 3, 4, 5 or 6 to select the USART or
  *         UART peripheral.
  * @retval The received data.
  */
uint16_t BSP_USART_ReceiveData(USART_TypeDef* USARTx)
{
  // Receive Data
  return (uint16_t)(USARTx->DR & (uint16_t)0x01FF);
}

void HAL_SPI_MspInit(SPI_HandleTypeDef* hspi)
{
  GPIO_InitTypeDef GPIO_InitStruct;

  if(hspi->Instance == SPI1) {
    // Peripheral clock enable
    __SPI1_CLK_ENABLE();

    __GPIOA_CLK_ENABLE();       // GPIOA clock enable

    // SPI1 GPIO Configuration
    // PA4   ------> SPI1_NSS
    // PA5   ------> SPI1_SCK
    // PA6   ------> SPI1_MISO
    // PA7   ------> SPI1_MOSI

    GPIO_InitStruct.Pin = GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF5_SPI1;

    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
  }
}

void HAL_SPI_MspDeInit(SPI_HandleTypeDef* hspi)
{
  if(hspi->Instance == SPI1) {
    // Peripheral clock disable
    __SPI1_CLK_DISABLE();

    // SPI1 GPIO Configuration
    // PA4   ------> SPI1_NSS
    // PA5   ------> SPI1_SCK
    // PA6   ------> SPI1_MISO
    // PA7   ------> SPI1_MOSI

    HAL_GPIO_DeInit(GPIOA, GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7);
  }
}

void BSP_SPI1_Init_2_Lines(void)
{
  hspi1.Instance = SPI1;

  hspi1.Init.Mode = SPI_MODE_MASTER;
  hspi1.Init.Direction = SPI_DIRECTION_2LINES;
  hspi1.Init.DataSize = SPI_DATASIZE_8BIT;
  hspi1.Init.CLKPolarity = SPI_POLARITY_LOW;
  hspi1.Init.CLKPhase = SPI_PHASE_1EDGE;
  hspi1.Init.NSS = SPI_NSS_SOFT;
  hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_16;
  hspi1.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi1.Init.TIMode = SPI_TIMODE_DISABLED;
  hspi1.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLED;

  HAL_SPI_Init(&hspi1);
}

void BSP_SPI1_Init_1_Line(void)
{
  hspi1.Instance = SPI1;

  hspi1.Init.Mode = SPI_MODE_MASTER;
  hspi1.Init.Direction = SPI_DIRECTION_1LINE;
  hspi1.Init.DataSize = SPI_DATASIZE_8BIT;
  hspi1.Init.CLKPolarity = SPI_POLARITY_LOW;
  hspi1.Init.CLKPhase = SPI_PHASE_1EDGE;
  hspi1.Init.NSS = SPI_NSS_SOFT;
  hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_16;
  hspi1.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi1.Init.TIMode = SPI_TIMODE_DISABLED;
  hspi1.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLED;

  HAL_SPI_Init(&hspi1);
}

/**
  * @brief  Sends a Byte through the SPI interface and return the Byte received
  *         from the SPI bus.
  * @param  Byte : Byte send.
  * @retval The received byte value
  */
uint8_t BSP_SPI1_WriteRead(uint8_t Byte)
{
  uint8_t rcvByte = 0;

  // Send a Byte through the SPI peripheral
  // Read byte from the SPI bus
  HAL_SPI_TransmitReceive(&hspi1, (uint8_t*) &Byte, (uint8_t*) &rcvByte, 1, SpiTimeout);

  return rcvByte;
}

void BSP_AccInit(void)
{
  GPIO_InitTypeDef GPIO_InitStruct;

  // GPIOx clock enable
  __GPIOA_CLK_ENABLE();

  // Configure GPIO pin : PA8/CS
  GPIO_InitStruct.Pin = BSP_ACC_CS_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_MEDIUM;
  HAL_GPIO_Init(BSP_ACC_CS_GPIO_PORT, &GPIO_InitStruct);

  BSP_ACC_CS_HIGH();     // Deselect Accelerometer
}

void BSP_MagInit(void)
{
  GPIO_InitTypeDef GPIO_InitStruct;

  // GPIOx clock enable
  __GPIOA_CLK_ENABLE();

  // Configure GPIO pin : PA11/CS
  GPIO_InitStruct.Pin = BSP_MAG_CS_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_MEDIUM;
  HAL_GPIO_Init(BSP_MAG_CS_GPIO_PORT, &GPIO_InitStruct);

  BSP_MAG_CS_HIGH();     // Deselect Magnetometer
}

void BSP_GyroInit(void)
{
  GPIO_InitTypeDef GPIO_InitStruct;

  // GPIOx clock enable
  __GPIOA_CLK_ENABLE();

  // Configure GPIO pin : PA4/CS
  GPIO_InitStruct.Pin = BSP_GYRO_CS_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_MEDIUM;
  HAL_GPIO_Init(BSP_GYRO_CS_GPIO_PORT, &GPIO_InitStruct);

  BSP_GYRO_CS_HIGH();     // Deselect gyro


  // Enable INT2 GPIO clock and Configure GPIO pin to detect Interrupts

  __GPIOC_CLK_ENABLE();     // GPIOx clock enable

  GPIO_InitStruct.Pin = BSP_GYRO_INT2_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
  GPIO_InitStruct.Pull= GPIO_NOPULL;
  HAL_GPIO_Init(BSP_GYRO_INT1_GPIO_PORT, &GPIO_InitStruct);

  // Enable and set INT2/DRDY EXTI Interrupt to the lowest priority
  HAL_NVIC_SetPriority(BSP_GYRO_INT2_EXTI_IRQn, 0x07, 0);
  //HAL_NVIC_EnableIRQ(BSP_GYRO_INT2_EXTI_IRQn);
}