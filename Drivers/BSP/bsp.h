
#ifndef __BSP_H
#define __BSP_H

#include "main.h"

// @brief Definitios for GPIOs. Enable GPIO clock manually!
// ------------- Analog Inputs -------------
#define BSP_TAMPER_PIN          GPIO_PIN_1
#define BSP_TAMPER_GPIO_PORT    GPIOA
#define ADC_TAMPER_CHANNEL      ADC_CHANNEL_1
#define BSP_THS1_ADC_PIN        GPIO_PIN_0
#define BSP_THS1_ADC_GPIO_PORT  GPIOC
#define ADC_THS1_CHANNEL        ADC_CHANNEL_10
#define BSP_THS2_ADC_PIN        GPIO_PIN_1
#define BSP_THS2_ADC_GPIO_PORT  GPIOC
#define ADC_THS2_CHANNEL        ADC_CHANNEL_11
#define BSP_VBS1_ADC_PIN        GPIO_PIN_2
#define BSP_VBS1_ADC_GPIO_PORT  GPIOC
#define ADC_VBS1_CHANNEL        ADC_CHANNEL_12
#define BSP_VBS2_ADC_PIN        GPIO_PIN_3
#define BSP_VBS2_ADC_GPIO_PORT  GPIOC
#define ADC_VBS2_CHANNEL        ADC_CHANNEL_13

#define BSP_DIG2_PIN            GPIO_PIN_2
#define BSP_DIG2_GPIO_PORT      GPIOC

// ------------- GSM/GPS related -------------
#define BSP_EN_BCKP_PIN         GPIO_PIN_0
#define BSP_EN_BCKP_GPIO_PORT   GPIOC

//#define BSP_DSR_PIN             GPIO_PIN_1
//#define BSP_DSR_GPIO_PORT       GPIOC

#define BSP_DSR3_PIN            GPIO_PIN_11
#define BSP_DSR3_GPIO_PORT      GPIOA

//#define BSP_RI_PIN              GPIO_PIN_3
//#define BSP_RI_GPIO_PORT        GPIOC

#define BSP_RI3_PIN             GPIO_PIN_12
#define BSP_RI3_GPIO_PORT       GPIOA

#define BSP_INP1_PIN            GPIO_PIN_15
#define BSP_INP1_GPIO_PORT      GPIOA

#define BSP_INP2_PIN            GPIO_PIN_2
#define BSP_INP2_GPIO_PORT      GPIOC

#define BSP_RI3_PIN             GPIO_PIN_12
#define BSP_RI3_GPIO_PORT       GPIOA

#define BSP_PWR_PIN             GPIO_PIN_13
#define BSP_PWR_GPIO_PORT       GPIOC

#define BSP_RST_PIN             GPIO_PIN_0
#define BSP_RST_GPIO_PORT       GPIOB

#define BSP_RST_DET_PIN         GPIO_PIN_1
#define BSP_RST_DET_GPIO_PORT   GPIOB

#define BSP_PPS_PIN             GPIO_PIN_2
#define BSP_PPS_GPIO_PORT       GPIOB

#define BSP_CHARGE_PIN          GPIO_PIN_10
#define BSP_CHARGE_GPIO_PORT    GPIOB

#define BSP_VIN_ADC_PIN         GPIO_PIN_1
#define BSP_VIN_ADC_GPIO_PORT   GPIOC

#define BSP_VBAT_ADC_PIN        GPIO_PIN_3
#define BSP_VBAT_ADC_GPIO_PORT  GPIOC

// ------------- Gyroscope -------------
#define BSP_GYRO_CS_PIN         GPIO_PIN_4
#define BSP_GYRO_CS_GPIO_PORT   GPIOA

#define BSP_GYRO_INT1_PIN         GPIO_PIN_5
#define BSP_GYRO_INT1_GPIO_PORT   GPIOC
#define BSP_GYRO_INT1_EXTI_IRQn   EXTI9_5_IRQn

#define BSP_GYRO_INT2_PIN         GPIO_PIN_4
#define BSP_GYRO_INT2_GPIO_PORT   GPIOC
#define BSP_GYRO_INT2_EXTI_IRQn   EXTI4_IRQn

// ------------- Accelerometer -------------
#define BSP_ACC_CS_PIN          GPIO_PIN_8
#define BSP_ACC_CS_GPIO_PORT    GPIOA

#define BSP_ACC_INT1_PIN         GPIO_PIN_6
#define BSP_ACC_INT1_GPIO_PORT   GPIOC
#define BSP_ACC_INT1_EXTI_IRQn   EXTI9_5_IRQn

#define BSP_ACC_INT2_PIN         GPIO_PIN_7
#define BSP_ACC_INT2_GPIO_PORT   GPIOC
#define BSP_ACC_INT2_EXTI_IRQn   EXTI9_5_IRQn

// ------------- Magnetometer -------------
#define BSP_MAG_CS_PIN          GPIO_PIN_11
#define BSP_MAG_CS_GPIO_PORT    GPIOA


#define READ_INP1()             HAL_GPIO_ReadPin(BSP_INP1_GPIO_PORT, BSP_INP1_PIN)
#define READ_INP2()             HAL_GPIO_ReadPin(BSP_INP2_GPIO_PORT, BSP_INP2_PIN)
#define READ_PPS()              HAL_GPIO_ReadPin(BSP_PPS_GPIO_PORT, BSP_PPS_PIN)

// @brief GSM power pin definitions
#define BSP_LED_ON()           {}
#define BSP_LED_OFF()          {}
#define BSP_LED_TOGGLE()        {}


#define BSP_GSM_PWR_LOW()       HAL_GPIO_WritePin(BSP_PWR_GPIO_PORT, BSP_PWR_PIN, GPIO_PIN_RESET)
#define BSP_GSM_PWR_HIGH()      HAL_GPIO_WritePin(BSP_PWR_GPIO_PORT, BSP_PWR_PIN, GPIO_PIN_SET)

// @brief GSM charge pin definitions
#define BSP_CHARGE_OFF()        HAL_GPIO_WritePin(BSP_CHARGE_GPIO_PORT, BSP_CHARGE_PIN, GPIO_PIN_RESET)
#define BSP_CHARGE_ON()         HAL_GPIO_WritePin(BSP_CHARGE_GPIO_PORT, BSP_CHARGE_PIN, GPIO_PIN_SET)

// @brief GSM/GPS backup pin definitions
#define BSP_BACKUP_OFF()        HAL_GPIO_WritePin(BSP_EN_BCKP_GPIO_PORT, BSP_EN_BCKP_PIN, GPIO_PIN_RESET)
#define BSP_BACKUP_ON()         HAL_GPIO_WritePin(BSP_EN_BCKP_GPIO_PORT, BSP_EN_BCKP_PIN, GPIO_PIN_SET)

// @brief GSM reset pin definitions
#define BSP_GSM_RST_LOW()       HAL_GPIO_WritePin(BSP_RST_GPIO_PORT, BSP_RST_PIN, GPIO_PIN_RESET)
#define BSP_GSM_RST_HIGH()      HAL_GPIO_WritePin(BSP_RST_GPIO_PORT, BSP_RST_PIN, GPIO_PIN_SET)

// @brief GSM DTR pin definitions
#define BSP_GSM_DTR_LOW()       HAL_GPIO_WritePin(BSP_DTR_GPIO_PORT, BSP_DTR_PIN, GPIO_PIN_RESET)
#define BSP_GSM_DTR_HIGH()      HAL_GPIO_WritePin(BSP_DTR_GPIO_PORT, BSP_DTR_PIN, GPIO_PIN_SET)

// @brief Gyro Chip Select macro definition
#define BSP_GYRO_CS_LOW()       HAL_GPIO_WritePin(BSP_GYRO_CS_GPIO_PORT, BSP_GYRO_CS_PIN, GPIO_PIN_RESET)
#define BSP_GYRO_CS_HIGH()      HAL_GPIO_WritePin(BSP_GYRO_CS_GPIO_PORT, BSP_GYRO_CS_PIN, GPIO_PIN_SET)

// @brief Accelerometer Chip Select macro definition
#define BSP_ACC_CS_LOW()        HAL_GPIO_WritePin(BSP_ACC_CS_GPIO_PORT, BSP_ACC_CS_PIN, GPIO_PIN_RESET)
#define BSP_ACC_CS_HIGH()       HAL_GPIO_WritePin(BSP_ACC_CS_GPIO_PORT, BSP_ACC_CS_PIN, GPIO_PIN_SET)

// @brief Magnetometer Chip Select macro definition
#define BSP_MAG_CS_LOW()        HAL_GPIO_WritePin(BSP_MAG_CS_GPIO_PORT, BSP_MAG_CS_PIN, GPIO_PIN_RESET)
#define BSP_MAG_CS_HIGH()       HAL_GPIO_WritePin(BSP_MAG_CS_GPIO_PORT, BSP_MAG_CS_PIN, GPIO_PIN_SET)

/* Maximum Timeout values for flags waiting loops. These timeouts are not based
   on accurate values, they just guarantee that the application will not remain
   stuck if the SPI communication is corrupted.
   You may modify these timeout values depending on CPU frequency and application
   conditions (interrupts routines ...). */
#define SPI_TIMEOUT_MAX         ((uint32_t)0x1000)


typedef struct {
  char LowVin   : 1;
  char LowVbat  : 1;
  char res1     : 1;
  char res2     : 1;
  char res3     : 1;
  char res4     : 1;
  char res5     : 1;
  char res6     : 1;
} PWR_StateType;

extern UART_HandleTypeDef       huart1, huart2;
extern SPI_HandleTypeDef        hspi1;
extern ADC_HandleTypeDef        adc1;
extern PWR_StateType            Pwr;
extern IWDG_HandleTypeDef       iwdg1;

void      BSP_Config          (void);
void BSP_System_Init(void);

void      SystemClock_Config  (void);
void    BSP_Config      (void);
void    BSP_System_Init (void);
void      BSP_GSM_Init        (void);
void      BSP_GyroInit        (void);
void      BSP_AccInit         (void);
void      BSP_MagInit         (void);
void      BSP_USART1_Init     (void);
void      BSP_USART2_Init     (void);
void      BSP_SPI1_Init       (void);
void    BSP_TIM10_Init(void);

void      BSP_USART_SendData    (USART_TypeDef* USARTx, uint16_t Data);
uint16_t  BSP_USART_ReceiveData (USART_TypeDef* USARTx);

uint8_t   BSP_SPI1_WriteRead  (uint8_t Byte);

#endif
