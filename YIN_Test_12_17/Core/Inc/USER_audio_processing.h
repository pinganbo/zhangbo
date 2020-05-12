/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __USER_AUDIO_PROESSING_H
#define __USER_AUDIO_PROESSING_H
#ifdef __cplusplus
 extern "C" {
#endif
/* Includes ------------------------------------------------------------------*/
#include <stdint.h>
/* Exported define -----------------------------------------------------------*/
#define ADC_CONVERTED_DATA_BUFFER_SIZE   1024
#define ADC_CONVERTED_DATA_DOUBLEBUFFER_SIZE   (ADC_CONVERTED_DATA_BUFFER_SIZE*2)
/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported variables --------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */

float middle_of_3_float(float a, float b, float c);
void CopyADCData_MedianFilter(uint16_t *buffer, uint32_t dma_clpt_flag);

#ifdef __cplusplus
}
#endif
#endif