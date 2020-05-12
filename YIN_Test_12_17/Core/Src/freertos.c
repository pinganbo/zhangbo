/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2019 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */     
#include "queue.h"
#include "USER_audio_processing.h"
#include "USER_app_core.h"
#include "BSP_UI.h"
#include "Yin.h"
#include "adc.h"
#include "gpio.h"
//#include "iwdg.h"
#include "stdbool.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"
/* Include ARM math */
#include "arm_math.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */

/* Debug Var*/
volatile uint16_t peak_detector_value;
volatile uint16_t reverse_peak_detector_value;
/* AudioProcessing Var*/
//uint16_t *audio_input_buffer;
uint16_t audio_input_buffer[ADC_CONVERTED_DATA_BUFFER_SIZE];
extern uint16_t aADC1ConvertedData[ADC_CONVERTED_DATA_DOUBLEBUFFER_SIZE];
extern volatile bool ubAnalogWatchdogStatus;
/* Pitch Var*/
float f0_target = 440.00;

/* USER CODE END Variables */
osThreadId osTask_DebugHandle;
osThreadId osTask_AudioP1Handle;
osThreadId osTask_AudioP2Handle;
osThreadId osTask_MoterHandle;
osThreadId osTask_UIHandle;
osThreadId osTask_GUIHandle;
osThreadId osTask_CoreHandle;
osMessageQId Queue_F0Handle;
uint8_t Queue_F0Buffer[ 4 * sizeof( float ) ];
osStaticMessageQDef_t Queue_F0ControlBlock;

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

extern void StartTaskRunTimeStats(void);

/* USER CODE END FunctionPrototypes */

void StartTask_Debug(void const * argument);
void StartTask_AudioPreProcessing(void const * argument);
void StartTask_AudioProcessing(void const * argument);
void StartTask_Moter(void const * argument);
void StartTask_UI(void const * argument);
void StartTask_GUI(void const * argument);
void StartTask_Core(void const * argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/* GetIdleTaskMemory prototype (linked to static allocation support) */
void vApplicationGetIdleTaskMemory( StaticTask_t **ppxIdleTaskTCBBuffer, StackType_t **ppxIdleTaskStackBuffer, uint32_t *pulIdleTaskStackSize );

/* Hook prototypes */
void configureTimerForRunTimeStats(void);
unsigned long getRunTimeCounterValue(void);
void vApplicationStackOverflowHook(TaskHandle_t xTask, signed char *pcTaskName);

/* USER CODE BEGIN 1 */
uint64_t ulHighFrequencyTimerTicks = 0ul;
/* Functions needed when configGENERATE_RUN_TIME_STATS is on */
__weak void configureTimerForRunTimeStats(void)
{
  uint64_t ulHighFrequencyTimerTicks = 0ul;
}

__weak unsigned long getRunTimeCounterValue(void)
{
  return ulHighFrequencyTimerTicks;
}
/* USER CODE END 1 */

/* USER CODE BEGIN 4 */
__weak void vApplicationStackOverflowHook(TaskHandle_t xTask, signed char *pcTaskName)
{
  /* Run time stack overflow checking is performed if
   configCHECK_FOR_STACK_OVERFLOW is defined to 1 or 2. This hook function is
   called if a stack overflow is detected. */

  printf("\r\nOverflow %s", pcTaskName);
}
/* USER CODE END 4 */

/* USER CODE BEGIN GET_IDLE_TASK_MEMORY */
static StaticTask_t xIdleTaskTCBBuffer;
static StackType_t xIdleStack[configMINIMAL_STACK_SIZE];

void vApplicationGetIdleTaskMemory(StaticTask_t **ppxIdleTaskTCBBuffer, StackType_t **ppxIdleTaskStackBuffer, uint32_t *pulIdleTaskStackSize)
{
  *ppxIdleTaskTCBBuffer = &xIdleTaskTCBBuffer;
  *ppxIdleTaskStackBuffer = &xIdleStack[0];
  *pulIdleTaskStackSize = configMINIMAL_STACK_SIZE;
  /* place for user code */
}
/* USER CODE END GET_IDLE_TASK_MEMORY */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */
  StartTaskRunTimeStats();
  USER_App_Core_Init();
  USER_BSP_Button_Init();

  //audio_input_buffer = (uint16_t *)pvPortMalloc(ADC_CONVERTED_DATA_BUFFER_SIZE * sizeof(uint16_t));

  /* USER CODE END Init */

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* Create the queue(s) */
  /* definition and creation of Queue_F0 */
  osMessageQStaticDef(Queue_F0, 4, float, Queue_F0Buffer, &Queue_F0ControlBlock);
  Queue_F0Handle = osMessageCreate(osMessageQ(Queue_F0), NULL);

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* definition and creation of osTask_Debug */
  osThreadDef(osTask_Debug, StartTask_Debug, osPriorityIdle, 0, 256);
  osTask_DebugHandle = osThreadCreate(osThread(osTask_Debug), NULL);

  /* definition and creation of osTask_AudioP1 */
  osThreadDef(osTask_AudioP1, StartTask_AudioPreProcessing, osPriorityAboveNormal, 0, 256);
  osTask_AudioP1Handle = osThreadCreate(osThread(osTask_AudioP1), NULL);

  /* definition and creation of osTask_AudioP2 */
  osThreadDef(osTask_AudioP2, StartTask_AudioProcessing, osPriorityHigh, 0, 256);
  osTask_AudioP2Handle = osThreadCreate(osThread(osTask_AudioP2), NULL);

  /* definition and creation of osTask_Moter */
  osThreadDef(osTask_Moter, StartTask_Moter, osPriorityHigh, 0, 320);
  osTask_MoterHandle = osThreadCreate(osThread(osTask_Moter), NULL);

  /* definition and creation of osTask_UI */
  osThreadDef(osTask_UI, StartTask_UI, osPriorityNormal, 0, 128);
  osTask_UIHandle = osThreadCreate(osThread(osTask_UI), NULL);

  /* definition and creation of osTask_GUI */
  osThreadDef(osTask_GUI, StartTask_GUI, osPriorityLow, 0, 256);
  osTask_GUIHandle = osThreadCreate(osThread(osTask_GUI), NULL);

  /* definition and creation of osTask_Core */
  osThreadDef(osTask_Core, StartTask_Core, osPriorityBelowNormal, 0, 128);
  osTask_CoreHandle = osThreadCreate(osThread(osTask_Core), NULL);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

}

/* USER CODE BEGIN Header_StartTask_Debug */
/**
  * @brief  Function implementing the osTask_Debug thread.
  * @param  argument: Not used 
  * @retval None
  */
/* USER CODE END Header_StartTask_Debug */
void StartTask_Debug(void const * argument)
{
  /* USER CODE BEGIN StartTask_Debug */
  //vTaskSuspend(osTask_DebugHandle);
  uint8_t pcWriteBuffer[256];
  /* Infinite loop */
  for (;;)
  {
    HAL_GPIO_TogglePin(LD2_GPIO_Port, LD2_Pin);

    vTaskList((char *)&pcWriteBuffer);
    printf("\r\n================================\r\n");
    printf("Task_name     State  Priority  R_Stack  Task_number\r\n");
    printf("%s\r\n", pcWriteBuffer);

    vTaskGetRunTimeStats((char *)&pcWriteBuffer);
    printf("Task_name       Running_count   CPU_usage \r\n");
    printf("%s\r\n", pcWriteBuffer);
    printf("Minimum memory: ");
    printf("%01d", xPortGetFreeHeapSize());
    printf("\r\nMinimum remaining memory: ");
    printf("%01d \r\n\r\n", xPortGetMinimumEverFreeHeapSize());

    osDelay(100);
    //HAL_IWDG_Refresh(&hiwdg);
  }
  /* USER CODE END StartTask_Debug */
}

/* USER CODE BEGIN Header_StartTask_AudioPreProcessing */
/**
* @brief Function implementing the osTask_AudioP1 thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartTask_AudioPreProcessing */
void StartTask_AudioPreProcessing(void const * argument)
{
  /* USER CODE BEGIN StartTask_AudioPreProcessing */
  uint_fast32_t dma_clpt_flag;
  /* Infinite loop */
  for (;;)
  {
    xTaskNotifyWait(0x00000000, 0x00000000, &dma_clpt_flag, portMAX_DELAY);

    /* TODO   AnalogWDGConfig.HighThreshold = 2000 ??
              AnalogWDGConfig.LowThreshold =  1000 ?? 
              ADC转换周期?ADC buffer长度?*/
    /* 测试时，删除下面adc Code*/
    if (__HAL_ADC_GET_FLAG(&hadc1, ADC_FLAG_AWD))
    {
      printf("\r\nAudioPreProcessing\r\n");

      __HAL_ADC_CLEAR_FLAG(&hadc1, ADC_FLAG_AWD);

      CopyADCData_MedianFilter(aADC1ConvertedData, dma_clpt_flag);
      //xTaskNotifyGive(osTask_AudioP2Handle);
      uint32_t now_syscount = xTaskGetTickCount();
      xTaskNotify(osTask_AudioP2Handle, now_syscount, eSetValueWithOverwrite);
    }

    //if (dma_clpt_flag == 0x01)  //dma HalfCplt
    //{
    //  memcpy(audio_input_buffer, &aADC1ConvertedData[0], ADC_CONVERTED_DATA_BUFFER_SIZE*sizeof(uint16_t));
    //}
    //if ((dma_clpt_flag & 0x02) != 0)  //dma full clpt
    //{
    //  memcpy(audio_input_buffer, &aADC1ConvertedData[ADC_CONVERTED_DATA_BUFFER_SIZE-1], ADC_CONVERTED_DATA_BUFFER_SIZE*sizeof(uint16_t));
    //}

    //osDelay(1000);
  }
  /* USER CODE END StartTask_AudioPreProcessing */
}

/* USER CODE BEGIN Header_StartTask_AudioProcessing */
/**
* @brief Function implementing the osTask_AudioP2 thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartTask_AudioProcessing */
void StartTask_AudioProcessing(void const * argument)
{
  /* USER CODE BEGIN StartTask_AudioProcessing */
  static uint32_t P1_count_present = 0;
  static uint32_t P1_count_last = 0;
  static Yin yin;
  static float f0_present = 0.0f;
  static float f0_last = 400.0f;
  static float f0_temp[3] = {0.0f, 0.0f, 0.0f};
  static uint8_t f0_temp_count = 0;

  Yin_init(&yin, ADC_CONVERTED_DATA_BUFFER_SIZE, 0.08);

  /* Infinite loop */
  for (;;)
  {
    xTaskNotifyWait(0, 0xffffff, &P1_count_present, portMAX_DELAY);

    f0_present = Yin_getPitch(&yin, audio_input_buffer);
    //printf("\r\nYIN_f0_present=%f", f0_present);

    //f0_temp[f0_temp_count] = Yin_getPitch(&yin, audio_input_buffer);
    //f0_temp_count++;
    //if (f0_temp_count >= 2)
    //{
    //  f0_temp_count = 0;
    //}

    Yin_freeBuffer_RTOS(&yin);

    //f0_present = middle_of_3_float(f0_temp[0], f0_temp[1], f0_temp[2]);
    //TODO replace middle_filter with kalman_filter

    if (f0_present > 25)
    {
      /* 除去 (F0/2) 的结�???? */
      if (((f0_present * 2 - f0_last) < 5.0) && (P1_count_present - P1_count_last < 200))
      {
        //printf("F0/2 del %f \r\n", f0_present);
        f0_present = f0_present * 2;
        //f0_present = 0.0;
      }
      else
      {
        f0_last = f0_present;
      }

      P1_count_last = P1_count_present;

      printf("\r\n\r\nF0 = %f", f0_present);
      //printf("\r\npeak_detector_value = %d", peak_detector_value);
      //printf("\r\nreverse_peak_detector_value = %d\r\n", reverse_peak_detector_value);
      //printf("中心p = %d\r\n", peak_detector_value-((peak_detector_value-reverse_peak_detector_value)/2));
      //printf("中心rp = %d\r\n", ((peak_detector_value-reverse_peak_detector_value)/2)+reverse_peak_detector_value);
      //printf("delta_high_low = %d\r\n", peak_detector_value-reverse_peak_detector_value);
      //printf("\r\ncloset_sub = %d \r\n", Find_Closest_FreqDataArray_SubNum(f0_present));
      f0_target = setting_music_status.FreqDataArray[Find_Closest_FreqDataArray_SubNum(f0_present)];
      printf("\r\nTarget_F0 = %f", f0_target);
      printf("\r\nTarget_Pitch= %s", all_string_name[setting_music_status.mStringNameNumArray[setting_tuner_status.mStringNum]]);

      if (xQueueSendToBack(Queue_F0Handle, &f0_present, 50) != pdPASS)
      {
        printf("Queue_F0 was full!");
      }

      //TODO 如果CPU富裕，换成TaskNotify
      //xTaskNotifyGive(osTask_MoterP1Handle);
    }
    //ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
  }
  /* USER CODE END StartTask_AudioProcessing */
}

/* USER CODE BEGIN Header_StartTask_Moter */
/**
* @brief Function implementing the osTask_Moter thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartTask_Moter */
void StartTask_Moter(void const * argument)
{
  /* USER CODE BEGIN StartTask_Moter */
  static float pid_input_f0[4];

  /* PID Var*/
  static arm_pid_instance_f32 PID;
  //TODO 调节PID参数
  PID.Kp = 30;   /**< Proporcional 比例 */
  PID.Ki = 0.05; /**< Integral     积分 */
  PID.Kd = 1;    /**< Derivative   微分 */

  /* 实测F0与目标F0 */
  static float f0_error;

  /* 步进电机参数 */
  static float motor_step;

  /* Initialize PID system, float32_t format */
  arm_pid_init_f32(&PID, 1);

  /* Infinite loop */
  for (;;)
  {
    //ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
    xQueueReceive(Queue_F0Handle, pid_input_f0, portMAX_DELAY);
    //printf("\r\nQueueSpacesAvailable=%u", (uint32_t)uxQueueSpacesAvailable(Queue_F0Handle));
    printf("\r\npid_input_f0=%f",pid_input_f0[0]);
    f0_error = f0_target - pid_input_f0[0];

    /* Calculate PID here, argument is f0_error */
    /* Output data will be returned, we will use it as step motor control parameter*/
    motor_step = arm_pid_f32(&PID, f0_error);

    printf("\r\nPID_output=%f\r\n", motor_step);
  }
  /* USER CODE END StartTask_Moter */
}

/* USER CODE BEGIN Header_StartTask_UI */
/**
* @brief Function implementing the osTask_UI thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartTask_UI */
void StartTask_UI(void const * argument)
{
  /* USER CODE BEGIN StartTask_UI */
  /* Infinite loop */
  for (;;)
  {
    /* TODO 这里处理按键，并设定目标频率(f0_target)*/
    USER_BSP_UI_ButtonScan();
    osDelay(40);
  }
  /* USER CODE END StartTask_UI */
}

/* USER CODE BEGIN Header_StartTask_GUI */
/**
* @brief Function implementing the osTask_GUI thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartTask_GUI */
void StartTask_GUI(void const * argument)
{
  /* USER CODE BEGIN StartTask_GUI */
  /* Infinite loop */
  for(;;)
  {
    osDelay(500);
  }
  /* USER CODE END StartTask_GUI */
}

/* USER CODE BEGIN Header_StartTask_Core */
/**
* @brief Function implementing the osTask_Core thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartTask_Core */
void StartTask_Core(void const * argument)
{
  /* USER CODE BEGIN StartTask_Core */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1000);
  }
  /* USER CODE END StartTask_Core */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
