/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef USER_DATABASE_H
#define USER_DATABASE_H
/* Includes ------------------------------------------------------------------*/
#include "main.h"
/* Exported define -----------------------------------------------------------*/
//#define tunerstatus.LastPitchStatus.high  1
//#define tunerstatus.LastPitchStatus.low   2
#define MusicalInstrumentDataTotalNum       16    // TODO size of (MusicalInstrumentData [])?
//#define MusicalInstrumentDataTotalNum       (sizeof MusicalInstrumentData /sizeof MusicalInstrumentData[0] )
#define MusicalInstrumentDataInsNum         2+1   // TODO 多少种乐器 

/* Exported types ------------------------------------------------------------*/
struct InstrumentData{
  const char      *Instrument;
  const char      *Tunings;           //Tunings
  const uint8_t   mStringTotalNum;
  const uint8_t   *mStringNameNumArray;
  const float     *RailsbackCurveDataArray;  
};   //size of MusicalInstrumentData[0] == 20 

extern const struct InstrumentData MusicalInstrumentData[];
/* Exported constants --------------------------------------------------------*/
extern const char *all_string_name[];
/* Exported macro ------------------------------------------------------------*/
/* Exported variables --------------------------------------------------------*/

/* Exported functions ------------------------------------------------------- */


#endif