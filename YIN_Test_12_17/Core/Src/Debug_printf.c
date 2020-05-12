/**
  ******************************************************************************
  * File Name          : Debug_printf.c
  * Description        : Code for printf FreeRTOS RunTimeStats
  ******************************************************************************
  * ----------------------------FreeRTOSConfig.h--------------------------------
  * 
  * #define configGENERATE_RUN_TIME_STATS                   1
  * #define configUSE_TRACE_FACILITY                        1
  * #define configUSE_STATS_FORMATTING_FUNCTIONS            1                     
  * 
  * #define portCONFIGURE_TIMER_FOR_RUN_TIME_STATS      configureTimerForRunTimeStats
  * #define portGET_RUN_TIME_COUNTER_VALUE              getRunTimeCounterValue 
  *
  * 
  *--------------------------------stm32xxxx_it.c-------------------------------
  * The run time statistics time base needs to have a higher resolution than the tick interrupt.
  * recommended to make the time base between 10 and 100 times faster than the tick interrupt.
  *-----------------------------------------------------------------------------
  * extern uint64_t ulHighFrequencyTimerTicks;
  * 
  * TIMx_IRQHandler()
  * {
  *   ulHighFrequencyTimerTicks++ 
  * }
  * 
  *----------------------------------freertos.c---------------------------------
  * STM32CubeMX
  *-----------------------------------------------------------------------------
  * uint64_t ulHighFrequencyTimerTicks = 0ul;
  * 
  * __weak void configureTimerForRunTimeStats(void)
  * {
  *   ulHighFrequencyTimerTicks = 0ul;
  * }
  * 
  * __weak unsigned long getRunTimeCounterValue(void)
  * {
  *   return ulHighFrequencyTimerTicks;
  * }
  * 
  * extern void StartTaskRunTimeStats(void);
  * 
  * StartTaskRunTimeStats();
  * 
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include <stdarg.h>
#include <stdio.h>
#include <string.h>
//#include "Debug_printf.h"
#include "usart.h"

/* External variables --------------------------------------------------------*/
extern TIM_HandleTypeDef htim11;
extern UART_HandleTypeDef huart2;

/* Function prototypes -------------------------------------------------------*/


/* Exported functions ------------------------------------------------------- */
void StartTaskRunTimeStats(void);

/* CODE BEGIN ----------------------------------------------------------------*/
void StartTaskRunTimeStats(void)
{
  /* HAL */
  HAL_TIM_Base_Start_IT(&htim11);

  /* LL */
  //LL_TIM_EnableIT_UPDATE(TIM21);
  //LL_TIM_EnableCounter(TIM21);
}


#ifdef __GNUC__
/* With GCC, small printf (option LD Linker->Libraries->Small printf
   set to 'Yes') calls __io_putchar() */
#define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
#define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif /* __GNUC__ */
/**
  * @brief  Retargets the C library printf function to the USART.
  * @param  None
  * @retval None
  */
int fputc(int ch, FILE *f)
{
  /* SWD */
  ITM_SendChar(ch);    //Cortex M3 +

  /* HAL UART2 for Nucleo VCP */
  //HAL_UART_Transmit(&huart2, (uint8_t *)&ch, 1, 0x00ff);    //Cortex M0

  /* LL UART2 for Nucleo VCP */
  //while (!LL_USART_IsActiveFlag_TXE(USART2));
  //LL_USART_TransmitData8(USART2, ch);
  //while (!LL_USART_IsActiveFlag_TC(USART2));

  return ch;
}
