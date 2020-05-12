/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef BSP_UI_H
#define BSP_UI_H
/* Includes ------------------------------------------------------------------*/
#include "tim.h"
#include "gpio.h"
#include "stdlib.h"
#include "multi_button.h"

/* Exported define -----------------------------------------------------------*/
typedef enum
{
  UI_INPUT_NULL = (uint8_t)0x00,
  UI_INPUT_B1_SC = (uint8_t)0x01,
  UI_INPUT_B2_SC = (uint8_t)0x02,
  UI_INPUT_B3_SC = (uint8_t)0x03,
  UI_INPUT_B4_SC = (uint8_t)0x04,
  UI_INPUT_B2_LPS = (uint8_t)0x05,
  UI_INPUT_B1_LPS = (uint8_t)0x06,
  UI_INPUT_B3_LPS = (uint8_t)0x07,
  UI_INPUT_B4_LPS = (uint8_t)0x08,
} UI_INPUT_ENUM;

#define UI_BUTTON_MAX_SCAN_TIME 1400 // Maximum time of one scan of the button (ms)

/* Exported types ------------------------------------------------------------*/
extern struct Button button_1;
extern struct Button button_2;
extern struct Button button_3;
extern struct Button button_4;

/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions --------------------------------------------------------*/
extern void USER_BSP_Button_Init(void);
extern void USER_BSP_UI_ButtonScan(void);
extern void USER_BSP_Encoder_IQRHanlder(void);
/* Exported variables --------------------------------------------------------*/
#endif
