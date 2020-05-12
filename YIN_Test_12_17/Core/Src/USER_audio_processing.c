/**
  ******************************************************************************
  * File Name          : USER_audio_processing.c
  * Description        : Audio processing Code
  ******************************************************************************
  * 
  *        
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "USER_audio_processing.h"
#include "adc.h"
#include "FreeRTOS.h"
#include "task.h"
#include "tim.h"

/* Private defines -----------------------------------------------------------*/
/* Private macros ------------------------------------------------------------*/
/* Private typedef -----------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/

uint16_t middle_of_3(uint16_t a, uint16_t b, uint16_t c);
unsigned int medfilter(unsigned int datum);
uint16_t efficient_peak_detector(uint16_t value);
uint16_t efficient_reverse_peak_detector(uint16_t value);
/* External functions prototypes ---------------------------------------------*/
/* External variables --------------------------------------------------------*/
extern uint16_t aADC1ConvertedData[ADC_CONVERTED_DATA_DOUBLEBUFFER_SIZE];
extern uint16_t audio_input_buffer[ADC_CONVERTED_DATA_BUFFER_SIZE];
extern uint16_t peak_detector_value;
extern uint16_t reverse_peak_detector_value;

/* External typedef ----------------------------------------------------------*/

/* CODE BEGIN ----------------------------------------------------------------*/

/**
  * @brief  Copy ADC Data from DMA buffer to AudioProcessingBuffer,
  *         PreProcessing data with 1D Median filter
  *         https://embeddedgurus.com/stack-overflow/2010/10/median-filtering/
  * @note   Median filter for audio_input_buffer Windows size is 3
  *         DMA buffer is Ping Pong buffer.
  * @param  buffer: pointer to the input buffer
  *         dma_cplt_flag: DMA_IT status, 0x01==DMA HalfCplt, 0x02==DMA FullCplt
  * @retval None
  */
void CopyADCData_MedianFilter(uint16_t *buffer, uint32_t dma_clpt_flag)
{
  //uint_fast16_t temp_os_count= __HAL_TIM_GET_COUNTER(&htim11);
  //uint_fast32_t temp_os_count1= xTaskGetTickCount();

  if (dma_clpt_flag == 0x01)
  {
    //printf("\r\n0x%02x_DMA_HALF ",dma_clpt_flag);
    //printf(" %d ",temp_os_count1 );

    audio_input_buffer[0] = aADC1ConvertedData[0];

    //medfilter(aADC1ConvertedData[0]);
    //medfilter(aADC1ConvertedData[1]);

    for (uint16_t i = 1; i < ADC_CONVERTED_DATA_BUFFER_SIZE - 1; i++)
    {
      audio_input_buffer[i] = middle_of_3(aADC1ConvertedData[i - 1], aADC1ConvertedData[i], aADC1ConvertedData[i + 1]);
      //audio_input_buffer[i]=medfilter(aADC1ConvertedData[i+1]);

      //efficient_peak_detector(audio_input_buffer[i]);
      //efficient_reverse_peak_detector(audio_input_buffer[i]);
      //printf("%d\r\n",audio_input_buffer[i]);
    }

    audio_input_buffer[ADC_CONVERTED_DATA_BUFFER_SIZE - 1] = aADC1ConvertedData[ADC_CONVERTED_DATA_BUFFER_SIZE - 1];
  }

  if (dma_clpt_flag == 0x02)
  {
    //printf("\r\n0x%02x_DMA_FULL ",dma_clpt_flag);
    //printf(" %d ", temp_os_count1 );

    audio_input_buffer[0] = aADC1ConvertedData[ADC_CONVERTED_DATA_BUFFER_SIZE];

    for (uint16_t i = 1; i < ADC_CONVERTED_DATA_BUFFER_SIZE - 1; i++)
    {
      audio_input_buffer[i] = middle_of_3(aADC1ConvertedData[i + ADC_CONVERTED_DATA_BUFFER_SIZE - 1],
                                          aADC1ConvertedData[i + ADC_CONVERTED_DATA_BUFFER_SIZE],
                                          aADC1ConvertedData[i + ADC_CONVERTED_DATA_BUFFER_SIZE + 1]);

      //fficient_peak_detector(audio_input_buffer[i]);
      //efficient_reverse_peak_detector(audio_input_buffer[i]);
      //  //printf("%d\r\n",audio_input_buffer[i]);
    }

    audio_input_buffer[ADC_CONVERTED_DATA_BUFFER_SIZE - 1] = aADC1ConvertedData[ADC_CONVERTED_DATA_DOUBLEBUFFER_SIZE - 1];
  }

  //peak_detector_value = efficient_peak_detector(audio_input_buffer[ADC_CONVERTED_DATA_BUFFER_SIZE - 1]);
  //reverse_peak_detector_value = efficient_reverse_peak_detector(audio_input_buffer[ADC_CONVERTED_DATA_BUFFER_SIZE - 1]);

  //printf("\r\npeak_detector_value=%d\r\n", peak_detector_value);
  //printf("reverse_peak_detector_value=%d\r\n", reverse_peak_detector_value);
  //printf("差值=%d\r\n", peak_detector_value-reverse_peak_detector_value);
  //printf("中心r=%d\r\n", ((peak_detector_value-reverse_peak_detector_value)/2)+reverse_peak_detector_value);
  //printf("中心p=%d\r\n", peak_detector_value-((peak_detector_value-reverse_peak_detector_value)/2));

  //printf(" %d ", xTaskGetTickCount() );
  //printf("OS_TIME=%d\r\n", xTaskGetTickCount()-temp_os_count1);
  //printf("HAL_TIME=%d\r\n", __HAL_TIM_GET_COUNTER(&htim11)-temp_os_count);
}

/**
  * @brief  Median filter， size 3
  *         
  * @note   
  *         
  * @param  None
  * @retval None
  */
uint16_t middle_of_3(uint16_t a, uint16_t b, uint16_t c)
{
  uint16_t middle;

  if ((a <= b) && (a <= c))
  {
    middle = (b <= c) ? b : c;
  }
  else if ((b <= a) && (b <= c))
  {
    middle = (a <= c) ? a : c;
  }
  else
  {
    middle = (a <= b) ? a : b;
  }
  return middle;
}

float middle_of_3_float(float a, float b, float c)
{
  float middle;

  if ((a <= b) && (a <= c))
  {
    middle = (b <= c) ? b : c;
  }
  else if ((b <= a) && (b <= c))
  {
    middle = (a <= c) ? a : c;
  }
  else
  {
    middle = (a <= b) ? a : b;
  }
  return middle;
}

/**
  * @brief  medfilter 
  *         
  * @note   https://www.embedded.com/design/programming-languages-and-tools/4399504/Better-Than-Average
  *         
  * @param  None
  * @retval None
  */

/* Size of the data buffer; length of the sequence. */
#define NWIDTH 3
/* Smaller than any datum */
#define STOPPER 0

unsigned int medfilter(unsigned int datum)
{
  unsigned int i;
  struct pair
  {
    /* Pointers forming list linked in sorted order */
    struct pair *point;
    /* Values to sort */
    unsigned int value;
  };
  /* Buffer of nwidth pairs */
  static struct pair buffer[NWIDTH];
  /* pointer into circular buffer of data */
  static struct pair *datpoint = {buffer};
  /* chain stopper. */
  static struct pair small = {NULL, STOPPER};
  /* pointer to head (largest) of linked list.*/
  static struct pair big = {&small, 0};
  /* pointer to successor of replaced data item */
  struct pair *successor;
  /* pointer used to scan down the sorted list */
  struct pair *scan;
  /* previous value of scan */
  struct pair *scanold;
  /* pointer to median */
  struct pair *median;
  ;

  /* No stoppers allowed. */
  if (datum == STOPPER)
    datum = STOPPER + 1;
  /* increment and wrap data in pointer.*/
  if ((++datpoint - buffer) >= NWIDTH)
    datpoint = buffer;
  /* Copy in new datum */
  datpoint->value = datum;
  /* save pointer to old value's successor */
  successor = datpoint->point;
  /* median initially to first in chain */
  median = &big;
  /* scanold initially null. */
  scanold = NULL;
  /* points to pointer to first (largest) datum in chain */
  scan = &big;
  /* Handle chain-out of first item in chain as special case */
  if (scan->point == datpoint)
    scan->point = successor;
  /* Save this pointer and   */
  scanold = scan;
  /* step down chain */
  scan = scan->point;

  /* loop through the chain, normal loop exit via break. */
  for (i = 0; i < NWIDTH; i++)
  {
    /* Handle odd-numbered item in chain  */
    /* Chain out the old datum.*/
    if (scan->point == datpoint)
      scan->point = successor;
    /* If datum is larger than scanned value,*/
    if ((scan->value < datum))
    {
      /* chain it in here.  */
      datpoint->point = scanold->point;
      /* mark it chained in. */
      scanold->point = datpoint;
      datum = STOPPER;
    };
    /* Step median pointer down chain after doing odd-numbered element */
    /* Step median pointer.  */
    median = median->point;
    /* Break at end of chain  */
    if (scan == &small)
      break;
    /* Save this pointer and   */
    scanold = scan;
    /* step down chain */
    scan = scan->point;

    /* Handle even-numbered item in chain.  */
    if (scan->point == datpoint)
      scan->point = successor;
    if ((scan->value < datum))
    {
      datpoint->point = scanold->point;
      scanold->point = datpoint;
      datum = STOPPER;
    };
    if (scan == &small)
      break;
    scanold = scan;
    scan = scan->point;
  };
  return (median->value);
}

/**
  * @brief  peak_detector 
  *         
  * @note   https://embeddedgurus.com/stack-overflow/2015/09/peak-detection-of-a-time-series/
  *         
  * @param  None
  * @retval None
  */
#define WINDOW_SIZE 256

uint16_t efficient_peak_detector(uint16_t value)
{
  static uint16_t wbuffer[WINDOW_SIZE] = {0U};
  static uint16_t wr_idx = 0U;
  static uint16_t peak_value = 0U;
  static uint16_t peak_value_idx = 0U;

  if (value >= peak_value)
  {
    peak_value = value; /* New peak value, so record it */
    peak_value_idx = wr_idx;
    wbuffer[wr_idx] = value;
  }
  else
  {
    wbuffer[wr_idx] = value;      /* Not a new peak value, so just store it */
    if (wr_idx == peak_value_idx) /* Have we over written the peak value ? */
    {
      /*  Yes, so we need to do a brute force search to find the new
                maximum. Note that for efficiency reasons, if there are multiple
                values of the new peak value, then we want to chose the one
                whose index value is as far away as possible from the current index */
      uint16_t idx;
      uint16_t cnt;

      for (cnt = 0U, idx = wr_idx, peak_value = 0U; cnt < WINDOW_SIZE; ++cnt)
      {
        if (wbuffer[idx] >= peak_value)
        {
          peak_value = wbuffer[idx]; /* Record new peak */
          peak_value_idx = idx;
        }
        if (++idx >= WINDOW_SIZE)
        {
          idx = 0;
        }
      }
    }
  }
  if (++wr_idx >= WINDOW_SIZE)
  {
    wr_idx = 0;
  }

  return peak_value;
}

uint16_t efficient_reverse_peak_detector(uint16_t value)
{
  static uint16_t wbuffer[WINDOW_SIZE] = {0U};
  static uint16_t wr_idx = 0U;
  static uint16_t reverse_peak_value = 0U;
  static uint16_t reverse_peak_value_idx = 0U;

  if (value <= reverse_peak_value)
  {
    reverse_peak_value = value; /* New peak value, so record it */
    reverse_peak_value_idx = wr_idx;
    wbuffer[wr_idx] = value;
  }
  else
  {
    wbuffer[wr_idx] = value;              /* Not a new peak value, so just store it */
    if (wr_idx == reverse_peak_value_idx) /* Have we over written the peak value ? */
    {
      /*  Yes, so we need to do a brute force search to find the new
                maximum. Note that for efficiency reasons, if there are multiple
                values of the new peak value, then we want to chose the one
                whose index value is as far away as possible from the current index */
      uint16_t idx;
      uint16_t cnt;

      for (cnt = 0U, idx = wr_idx, reverse_peak_value = 0U; cnt < WINDOW_SIZE; ++cnt)
      {
        if (wbuffer[idx] >= reverse_peak_value)
        {
          reverse_peak_value = wbuffer[idx]; /* Record new peak */
          reverse_peak_value_idx = idx;
        }
        if (++idx >= WINDOW_SIZE)
        {
          idx = 0;
        }
      }
    }
  }
  if (++wr_idx >= WINDOW_SIZE)
  {
    wr_idx = 0;
  }

  return reverse_peak_value;
}
/* CODE END ------------------------------------------------------------------*/
/********************************END OF FILE***********************************/