/**
  ******************************************************************************
  * @file    FatFs/FatFs_uSD_RTOS/Src/stm32f4xx_it.c
  * @author  MCD Application Team
  * @version V1.0.1
  * @date    26-February-2014
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and
  *          peripherals interrupt service routine.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT(c) 2014 STMicroelectronics</center></h2>
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software
  * distributed under the License is distributed on an "AS IS" BASIS,
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_it.h"
#include "at.h"
#include "main.h"
#include "gsm.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/

extern PCD_HandleTypeDef hpcd;

extern void xPortSysTickHandler(void);

/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M4 Processor Exceptions Handlers                         */
/******************************************************************************/

/**
  * @brief  This function handles NMI exception.
  * @param  None
  * @retval None
  */
void NMI_Handler(void)
{
}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  BSP_USART_SendData(USART1, '<');
  BSP_USART_SendData(USART1, 'H');
  BSP_USART_SendData(USART1, 'F');
  BSP_USART_SendData(USART1, '>');
  while (1)
  {
  }
}

/**
  * @brief  This function handles Memory Manage exception.
  * @param  None
  * @retval None
  */
void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Bus Fault exception.
  * @param  None
  * @retval None
  */
void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Usage Fault exception.
  * @param  None
  * @retval None
  */
void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Debug Monitor exception.
  * @param  None
  * @retval None
  */
void DebugMon_Handler(void)
{
}

/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */
void SysTick_Handler(void)
{
  xPortSysTickHandler();
  HAL_IncTick();
}

/******************************************************************************/
/*                 STM32F4xx Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f4xx.s).                                               */
/******************************************************************************/

/**
  * @brief  This function handles DMA2 Stream 3 interrupt request.
  * @param  None
  * @retval None
  */
void DMA2_Stream3_IRQHandler(void)
{
  BSP_SD_DMA_Rx_IRQHandler();
}

/**
  * @brief  This function handles DMA2 Stream 6 interrupt request.
  * @param  None
  * @retval None
  */
void DMA2_Stream6_IRQHandler(void)
{
  BSP_SD_DMA_Tx_IRQHandler();
}

/**
  * @brief  This function handles SDIO interrupt request.
  * @param  None
  * @retval None
  */
void SDIO_IRQHandler(void)
{
  BSP_SD_IRQHandler();
}

/**
  * @brief  This function handles USB-On-The-Go FS global interrupt request.
  * @param  None
  * @retval None
  */
#ifdef USE_USB_FS
void OTG_FS_IRQHandler(void)
#else
void OTG_HS_IRQHandler(void)
#endif
{
  HAL_PCD_IRQHandler(&hpcd);
}

void USART1_IRQHandler(void)
{
  uint16_t data;
  static char cnt=0;

  if(__HAL_UART_GET_FLAG(&huart1, UART_FLAG_RXNE)) {
    data = huart1.Instance->DR;
    if(Tunnel == 1) {
      BSP_USART_SendData(USART2, data);
      if(data == '~') {
        cnt++;
        if(cnt == 3) {
          cnt = 0;
          BSP_USART_SendData(USART1, 'O');
          BSP_USART_SendData(USART1, 'K');
          BSP_USART_SendData(USART1, '\r');
          BSP_USART_SendData(USART1, '\n');
          Tunnel = 0;
          huart1.Init.BaudRate = 19200;
          HAL_UART_Init(&huart1);
        }
      }
      else cnt = 0;
    }
    else {
      if (data == 'p') {

      } else
      if (data == 'm') {

      } else if(data == '~') {
        cnt++;
        if(cnt == 3) {
          cnt = 0;
          BSP_USART_SendData(USART1, 'O');
          BSP_USART_SendData(USART1, 'K');
          BSP_USART_SendData(USART1, '\r');
          BSP_USART_SendData(USART1, '\n');
          Tunnel = 1;
          huart1.Init.BaudRate = 115200;
          HAL_UART_Init(&huart1);
        }
      }
      else cnt = 0;
    }
  }
}

void TIM2_IRQHandler(void)
{
    HAL_TIM_IRQHandler(&t1);
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  InertialSampleTask();
}
void USART2_IRQHandler(void)
{
  uint16_t data;
  if(__HAL_UART_GET_FLAG(&huart2, UART_FLAG_RXNE)) {
      data = huart2.Instance->DR;
      if(Tunnel) BSP_USART_SendData(USART1, data);
      if(rxBuffIdx < AT_RX_BUFFER_SIZE) {
        AtRxBuffer[rxBuffIdx] = data;
        rxBuffIdx++;
      }
  }
}

void EXTI4_IRQHandler(void)
{
  HAL_GPIO_EXTI_IRQHandler(BSP_GYRO_INT2_PIN);
}
/**
  * @brief  This function handles PPP interrupt request.
  * @param  None
  * @retval None
  */
/*void PPP_IRQHandler(void)
{
}*/

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
