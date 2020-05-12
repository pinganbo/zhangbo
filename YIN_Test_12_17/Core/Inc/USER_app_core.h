/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef USER_APP_CORE_H
#define USER_APP_CORE_H
/* Includes ------------------------------------------------------------------*/
#include "USER_database.h"
#include "FreeRTOS.h"
#include "event_groups.h"
/* Exported define -----------------------------------------------------------*/
/* Windows Page define */
#define WindowsHOMEPage                0
#define WindowsInstrumentPage          1
#define WindowsTuningsPage             2
/* Change setting_XX_status flag define */
#define EventBit_tMode                    (1UL<<0UL)
#define EventBit_Windows                  (1UL<<1UL)   
#define EventBit_Instrument               (1UL<<2UL) 
//#define EventBit_mMode                  (1UL<<3UL) 
#define EventBit_Tunings                  (1UL<<4UL) 
#define EventBit_FreqData                 (1UL<<5UL) 
#define EventBit_mStringNum               (1UL<<6UL)

#define EventBit_InsSelectUp              (1UL<<7UL)
#define EventBit_InsSelectDown            (1UL<<8UL)

#define EventBit_SyncCore                 (1UL<<22UL)
#define EventBit_SyncGUI                  (1UL<<23UL)
#define EventBit_Setting_All              (0xFFFFFF)
/* Exported types ------------------------------------------------------------*/
/** 
  * @brief  Tuner status flag State structure definition  
  */ 
struct tuner_status{
  float      F0Target;           /* 现在要测量的频率 */
  float      F0Last;         /* 上一个测试到的音高频率  */
  float      ReferencePitch;  /* defualt 440HZ */
  uint16_t   AudioPeak;       /* 音频输入峰值 */
  uint8_t    tMode;             /* 音准仪运行模式  */
  uint8_t    Windows;           /* 正在使用的窗口/界面  */
  uint8_t    mStringNum;        /* 要测量的键（弦）的数字 0-88/1-X弦  */
  uint8_t    F0LastStatus;   /* 上一个测试的音高状态，高/低  */
  uint8_t    PosInstrumentList; /* 保存的GUI选择列表中的位置 1-X */
  uint8_t    PosTuningsList;
  uint8_t    InstrumentDataArraySub;  /* 使用的MusicalInstrumentData[]的下标 */
};

/** 
  * @brief  Music status flag State structure definition  
  */ 
struct music_status{
  char       *Instrument;               /* 乐器种类  */
  char       *Tunings;                  /* 使用的调式或音准曲线  */
  uint8_t    mStringTotalNum;           /* 乐器总弦数  */
  float      *FreqDataArray;              /* 指向(struct InstrumentData)FreqData的指针 */
  uint8_t    *mStringNameNumArray;      /* 指向 数字弦名 数组的指针*/
};


extern struct tuner_status setting_tuner_status;
extern struct music_status setting_music_status;


/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported variables --------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
void USER_App_Core(void);
void USER_App_Core_Init(void);
void Calculate_FreqDataArray(void);
uint8_t Find_Closest_FreqDataArray_SubNum(float F0_present);
#endif