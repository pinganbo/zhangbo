/**
  ******************************************************************************
  * File Name          :  USER_app_core.c
  * Description        :  
  ******************************************************************************
  * 
  *        
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "USER_app_core.h"
#include "USER_database.h"
#include "FreeRTOS.h"
#include "math.h"
#include "stdlib.h"
/* Private defines -----------------------------------------------------------*/
/* Private macros ------------------------------------------------------------*/
/* Private typedef -----------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
float calculate_final_frequency[10];
/* Private function prototypes -----------------------------------------------*/
/* External variables --------------------------------------------------------*/
/* External typedef ----------------------------------------------------------*/
struct tuner_status setting_tuner_status;
struct music_status setting_music_status;
/* Exported types ------------------------------------------------------------*/

/* CODE BEGIN ----------------------------------------------------------------*/

/**
  * @brief  Aplly default setting after booting
  *         
  * @note   default setting / saved last setting
  *         
  * @param  None
  * @retval None
  */
void USER_App_Core_Init(void)
{
  /*TODO 换成last_time_saved_status*/
  setting_tuner_status.tMode = 0;
  setting_tuner_status.Windows = WindowsHOMEPage;
  setting_tuner_status.mStringNum = 1;
  setting_tuner_status.ReferencePitch = 440.00;
  setting_tuner_status.F0Last = 440.00;
  setting_tuner_status.F0LastStatus = 1;
  setting_tuner_status.PosInstrumentList = 1;
  setting_tuner_status.PosInstrumentList = 1;
  setting_tuner_status.PosTuningsList = 1;
  setting_tuner_status.InstrumentDataArraySub = 0;

  setting_music_status.Instrument = (char *)MusicalInstrumentData[setting_tuner_status.InstrumentDataArraySub].Instrument;
  setting_music_status.Tunings = (char *)MusicalInstrumentData[setting_tuner_status.InstrumentDataArraySub].Tunings;
  setting_music_status.mStringTotalNum = MusicalInstrumentData[setting_tuner_status.InstrumentDataArraySub].mStringTotalNum;
  setting_music_status.mStringNameNumArray = (uint8_t *)MusicalInstrumentData[setting_tuner_status.InstrumentDataArraySub].mStringNameNumArray;

  /* Malloc calculate_final_frequency,array should always exist, setting_music_status.FreqDataArray is a pointer to calculate_final_frequency */
  //float *calculate_final_frequency = (float *)pvPortMalloc(7 * sizeof(float));
//float *calculate_final_frequency = calloc((MusicalInstrumentData[setting_tuner_status.InstrumentDataArraySub].mStringTotalNum + 1), sizeof(float)); //calloc realloc


  Calculate_FreqDataArray();

  /* if pinao , setting_tuner_status.mStringNum = 49, defualt A4 440Hz*/
  if (setting_music_status.mStringTotalNum > 50)
  {
    setting_tuner_status.mStringNum = 49;
  }
  else
  {
    setting_tuner_status.mStringNum = 1;
  }
}

/**
  * @brief  Wait for EventGroups (osSetting_EventGroupHandle) to apply setting
  *         
  * @note   EventGroup flags base on struct tuner_status and struct music_status
  *         EventGroup (which key was press) - tuner_status.Windows 
  * @param  None
  * @retval None
  */
void USER_App_Core(void)
{
}

/**
  * @brief  calculate the frequency data array , send to setting_music_status.FreqDataArray              
  * @note   get (uint8_t)key_number from MusicalInstrumentData[].mStringNameNumArray (StringNameNum == key_number)
  *         calculate base frequency base on key_number
  *         base frequency + RailsbackCurveData == final frequency
  *         WARNING! after setting_tuner_status.InstrumentDataArraySub and setting_tuner_status.mStringNameNumArray were set
  * @param  None
  * @retval None
  */
void Calculate_FreqDataArray(void)
{
  /* realloc array calculate_final_frequency. array should always exist, setting_music_status.FreqDataArray is a pointer to calculate_final_frequency */
  //TODO  check, if Memory was leak!!
  //realloc(calculate_final_frequency, (MusicalInstrumentData[setting_tuner_status.InstrumentDataArraySub].mStringTotalNum + 1) * sizeof(float));

  calculate_final_frequency[0] = 0.0;

  for (uint8_t DataNum = 1; DataNum <= MusicalInstrumentData[setting_tuner_status.InstrumentDataArraySub].mStringTotalNum; DataNum++)
  {
    uint8_t string_name_number = MusicalInstrumentData[setting_tuner_status.InstrumentDataArraySub].mStringNameNumArray[DataNum];

    /* calculate the frequency from key_number */
    //TODO 不光使用十二平均律，还有纯律和五度相生律
    double calculate_base_frequency = setting_tuner_status.ReferencePitch * pow(pow(2.0, (1.0 / 12.0)), (double)(string_name_number - 49));

    /* RailsbackCurveData */
    //TODO
    calculate_final_frequency[DataNum] = (float)calculate_base_frequency;

    //printf("\r\nnummbe=%f", calculate_final_frequency[DataNum]);
  }

  /* Fill in the array */
  setting_music_status.FreqDataArray = calculate_final_frequency;
}

/**
  * @brief  找到最接近的目标频率 F0Target
  * @note   binary search
  * @param  频率F0_present
  * @retval 返回MusicalInstrumentData[X].mStringNameNumArray 数组的下标 
  */
uint8_t Find_Closest_FreqDataArray_SubNum(float F0_present)
{
  uint8_t low_index = 0;
  uint8_t high_index = setting_music_status.mStringTotalNum;
  static uint8_t mid_index;

  float low_abs;
  float high_abs;
  uint8_t ret_index;


  while (low_index != high_index)
  {
    mid_index = ((low_index + high_index) / 2);

    if (F0_present <= setting_music_status.FreqDataArray[mid_index])
    {
      
      high_index = mid_index;
    }
    if (F0_present > setting_music_status.FreqDataArray[mid_index])
    {
      low_index = mid_index;
    }
    if (high_index - low_index < 2)
    {
      break;
    }
  }

  low_abs = fabsf(F0_present - setting_music_status.FreqDataArray[low_index]);
  high_abs = fabsf(F0_present - setting_music_status.FreqDataArray[high_index]);
  ret_index = high_abs <= low_abs ? high_index : low_index;

  setting_tuner_status.mStringNum = ret_index;
  return ret_index;
}
/* CODE END ------------------------------------------------------------------*/
/********************************END OF FILE***********************************/