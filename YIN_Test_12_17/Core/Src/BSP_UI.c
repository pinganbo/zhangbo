/**
  ******************************************************************************
  * File Name          : BSP_UI.c
  * Description        : This file provides code for processing UI input 
  *                      signal from MCU peripheral, output logic signal
  *                     
  ******************************************************************************
  *
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "BSP_UI.h"
#include "cmsis_os.h"
#include "FreeRTOS.h"
#include "task.h"
#include "stdbool.h"

/* Private typedef -----------------------------------------------------------*/
/* Private defines -----------------------------------------------------------*/
/* Private macros ------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/

uint8_t MultiButton_Read_Button_1(void);
uint8_t MultiButton_Read_Button_2(void);
uint8_t MultiButton_Read_Button_3(void);
uint8_t MultiButton_Read_Button_4(void);

void button_1_SINGLE_CLICK_Handler(void *btn);
void button_2_SINGLE_CLICK_Handler(void *btn);
void button_3_SINGLE_CLICK_Handler(void *btn);
void button_4_SINGLE_CLICK_Handler(void *btn);
void button_1_LONG_RRESS_START_Handler(void *btn);

/* External variables --------------------------------------------------------*/
/* External typedef ----------------------------------------------------------*/
extern TaskHandle_t osUI_EncoderHandle;
extern TaskHandle_t osUI_ButtonHandle;
//extern QueueHandle_t Queue_App_UI_InputHandle;
/* Exported types ------------------------------------------------------------*/
struct Button button_1;
struct Button button_2;
struct Button button_3;
struct Button button_4;
/* CODE BEGIN ----------------------------------------------------------------*/

/**
  * @brief  BSP Button Init
  * @note   MultiButton Lib
  * @param  None
  * @retval None
  */
void USER_BSP_Button_Init(void)
{
  button_init(&button_1, MultiButton_Read_Button_1, 0);
  button_init(&button_2, MultiButton_Read_Button_2, 0);
  button_init(&button_3, MultiButton_Read_Button_3, 0);
  button_init(&button_4, MultiButton_Read_Button_4, 0);

  button_attach(&button_1, SINGLE_CLICK, button_1_SINGLE_CLICK_Handler);
  button_attach(&button_2, SINGLE_CLICK, button_2_SINGLE_CLICK_Handler);
  button_attach(&button_3, SINGLE_CLICK, button_3_SINGLE_CLICK_Handler);
  button_attach(&button_4, SINGLE_CLICK, button_4_SINGLE_CLICK_Handler);
  button_attach(&button_1, LONG_RRESS_START, button_1_LONG_RRESS_START_Handler);

  button_start(&button_1);
  button_start(&button_2);
  button_start(&button_3);
  button_start(&button_4);
}

/**
  * @brief  BSP Button MultiButton Lib read pin
  * @note   MultiButton Lib
  * @param  None
  * @retval None
  */
uint8_t MultiButton_Read_Button_1(void)
{
  return HAL_GPIO_ReadPin(BSP_UI_B1_GPIO_Port, BSP_UI_B1_Pin);
}

uint8_t MultiButton_Read_Button_2(void)
{
  return HAL_GPIO_ReadPin(BSP_UI_B2_GPIO_Port, BSP_UI_B2_Pin);
}

uint8_t MultiButton_Read_Button_3(void)
{
  return HAL_GPIO_ReadPin(BSP_UI_B3_GPIO_Port, BSP_UI_B3_Pin);
}

uint8_t MultiButton_Read_Button_4(void)
{
  return HAL_GPIO_ReadPin(BSP_UI_B4_GPIO_Port, BSP_UI_B4_Pin);
}

/**
  * @brief  BSP Button MultiButton Lib callback
  * @note   MultiButton Lib
  * @param  None
  * @retval None
  */
void button_1_SINGLE_CLICK_Handler(void *btn)
{
  printf("\r\nbutton_1_SINGLE_CLICK");
  UI_INPUT_ENUM app_ui_input = UI_INPUT_B1_SC;
  //xQueueSendToBack(Queue_App_UI_InputHandle, &app_ui_input, 0);
}

void button_2_SINGLE_CLICK_Handler(void *btn)
{
  printf("\r\nbutton_2_SINGLE_CLICK");
  UI_INPUT_ENUM app_ui_input = UI_INPUT_B2_SC;
  //xQueueSendToBack(Queue_App_UI_InputHandle, &app_ui_input, 0);

  HAL_GPIO_TogglePin(LD1_GPIO_Port, LD1_Pin);
}

void button_3_SINGLE_CLICK_Handler(void *btn)
{
  printf("\r\nbutton_3_SINGLE_CLICK");
  UI_INPUT_ENUM app_ui_input = UI_INPUT_B3_SC;
  //xQueueSendToBack(Queue_App_UI_InputHandle, &app_ui_input, 0);
}

void button_4_SINGLE_CLICK_Handler(void *btn)
{
  printf("\r\nbutton_4_SINGLE_CLICK");
  UI_INPUT_ENUM app_ui_input = UI_INPUT_B4_SC;
  //xQueueSendToBack(Queue_App_UI_InputHandle, &app_ui_input, 0);
}

void button_1_LONG_RRESS_START_Handler(void *btn)
{
  printf("\r\nbutton_1_LONG_RRESS");
  UI_INPUT_ENUM app_ui_input = UI_INPUT_B1_LPS;
  //xQueueSendToBack(Queue_App_UI_InputHandle, &app_ui_input, 0);

  //HAL_GPIO_TogglePin(BSP_UI_PWRON_GPIO_Port, BSP_UI_PWRON_Pin);
}

/**
* @brief   start scan button status
* @note    None
* @param   button to scan enmu UI_INPUT_ENUM
* @retval  None
*/
void USER_BSP_UI_ButtonScan(void)
{
  //uint16_t button_scan_count = UI_BUTTON_MAX_SCAN_TIME / 20;

  //for (uint16_t i = 0; i < button_scan_count; i++)
  //{
  button_ticks();
  //  osDelay(20);
  //}
  //TODO  for循环结束后 还在长按 会卡死 清状态机?
}

/* CODE END ------------------------------------------------------------------*/
/********************************END OF FILE***********************************/
