/**
  ******************************************************************************
  * File Name          : USER_database.c
  * Description        : frequency database
  ******************************************************************************
  * struct InstrumentData{
  *  char      *Instrument               //乐器名
  *  char      *Tunings                  //调式名/曲线名
  *  uint8_t   mStringTotalNum           //总弦数
  * 
  *  uint_t8   *mStringNameNumArray      //琴弦名称对应的数组all_string_name的下标
  *                                      //例：1=A0 ；49=A4
  * 
  *  float     *RailsbackCurveDataArray  //音准曲线表， 
  *                                      //指向表格的指针？
  * };
  * 
  * string_name_xxxx[0]      = 1 //Calculate_FreqDataArray(); calculate_base_frequency 十二平均律
  *                          = 2 //为纯律
  *                          = 3 //为五度相生律 
  * xxxx_railsback_freq_data[]   //音准曲线数据
  ******************************************************************************
  */
 

/* Includes ------------------------------------------------------------------*/
#include "USER_database.h"
/* Private defines -----------------------------------------------------------*/
/* Private macros ------------------------------------------------------------*/
/* Private typedef -----------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* External variables --------------------------------------------------------*/
/* External typedef ----------------------------------------------------------*/


/* CODE BEGIN ----------------------------------------------------------------*/

/* TODO This is Scientific designation,add Helmholtz designation later */
const char *all_string_name[] = { 
  "0",                             //0   
  "A0", "A#0", "B0", "C1", "C#1",  //1-5
  "D1", "D#1", "E1", "F1", "F#1",  //6-10
  "G1", "G#1", "A1", "A#1", "B1",  //11-15
  "C2", "C#2", "D2", "D#2", "E2",  //16-20
  "F2", "F#2", "G2", "G#2", "A2",  //21-25
  "A#2", "B2", "C3", "C#3", "D3",  //26-30
  "D#3", "E3", "F3", "F#3", "G3",  //31-35
  "G#3", "A3", "A#3", "B3", "C4",  //36-40
  "C#4", "D4", "D#4", "E4", "F4",  //41-45
  "F#4", "G4", "G#4", "A4", "A#4", //46-50
  "B4", "C5", "C#5", "D5", "D#5",  //51-55
  "E5", "F5", "F#5", "G5", "G#5",  //56-60
  "A5", "A#5", "B5", "C6", "C#6",  //61-65
  "D6", "D#6", "E6", "F6", "F#6",  //66-70
  "G6", "G#6", "A6", "A#6", "B6",  //71-75
  "C7", "C#7", "D7", "D#7", "E7",  //76-80
  "F7", "F#7", "G7", "G#7","A7",   //81-85
  "A#7", "B7", "C8"                //86-88
};
/* 参考频率
float piano_freq_data_12equal[89] = {
  0,
  27.500, 29.135 ,30.868, 32.703 ,34.648,              //1-5
  36.708, 38.891, 41.203, 43.654, 46.249,              //6-10
  48.999, 51.913, 55.000, 58.270, 61.735,              //11-15
  65.406, 69.296, 73.416, 77.782, 82.407,              //16-20
  87.307, 92.499, 97.999, 103.826, 110.000,            //21-25
  116.541, 123.471, 130.813, 138.591, 146.832,         //26-30
  155.563, 164.814, 174.614, 184.997, 195.998,         //31-35
  207.652, 220.000, 233.082, 246.942, 261.626,         //36-40
  277.183, 293.665, 311.127, 329.628, 349.228,         //41-45
  369.994, 391.995, 415.305, 440.000, 466.164,         //46-50
  493.883, 523.251, 554.365, 587.330, 622.254,         //51-55
  659.255, 698.456, 739.989, 783.991, 830.609,         //56-60
  880.000, 932.328, 987.767, 1046.502, 1108.731,       //61-65
  1174.659, 1244.508, 1318.510, 1396.913, 1479.978,    //66-70
  1567.982, 1661.219, 1760.000, 1864.655, 1975.533,    //71-75
  2093.005, 2217.461, 2349.318, 2489.016, 2637.020,    //76-80
  2793.826, 2959.955, 3135.963, 3322.438, 3520.000,    //81-85
  3729.310, 3951.066, 4186.009                         //86-88
};
*/

/************ Piano ************/
/* Piano key names */
const uint8_t string_name_pinao[89] = {
  1,
  1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,
  35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50,51,52,53,54,55,56,57,58,59,60,61,62,63,64,65,66,
  67,68,69,70,71,72,73,74,75,76,77,78,79,80,81,82,83,84,85,86,87,88
};


/* Piano 12 EqualTemperament railsback */
float piano_railsback_freq_data_12equal[89] = {
  0, 
  0, 0, 0, 0, 0,     //1-5
  0, 0, 0, 0, 0,     //6-10
  0, 0, 0, 0, 0,     //11-15
  0, 0, 0, 0, 0,     //16-20
  0, 0, 0, 0, 0,     //21-25
  0, 0, 0, 0, 0,     //26-30
  0, 0, 0, 0, 0,     //31-35
  0, 0, 0, 0, 0,     //36-40
  0, 0, 0, 0, 0,     //41-45
  0, 0, 0, 0, 0,     //46-50
  0, 0, 0, 0, 0,     //51-55
  0, 0, 0, 0, 0,     //56-60
  0, 0, 0, 0, 0,     //61-65
  0, 0, 0, 0, 0,     //66-70
  0, 0, 0, 0, 0,     //71-75
  0, 0, 0, 0, 0,     //76-80
  0, 0, 0, 0, 0,     //81-85
  0, 0, 0,           //86-88
};

/* RailsbackCurve railsback */  //TODO 修改为曲线与标准十二平均律数据差值 
float piano_railsback_freq_data[89] = {
  0, 
  27.500, 29.135 ,30.868, 32.703 ,34.648,            //1
  36.708, 38.891, 41.203, 43.654, 46.249,            //6
  48.999, 51.913, 55.000, 58.270, 61.735,            //11
  65.406, 69.296, 73.416, 77.782, 82.407,            //16
  87.307, 92.499, 97.999, 103.826, 110.000,          //21
  116.541, 123.471, 130.813, 138.591, 146.832,       //26
  155.563, 164.814, 174.614, 184.997, 195.998,       //31
  207.652, 220.000, 233.082, 246.942, 261.626,       //36
  277.183, 293.665, 311.127, 329.628, 349.228,       //41
  369.994, 391.995, 415.305, 440.000, 466.164,       //46
  493.883, 523.251, 554.365, 587.330, 622.254,       //51
  659.255, 698.456, 739.989, 783.991, 830.609,       //56
  880.000, 932.328, 987.767, 1046.502, 1108.731,     //61
  1174.659, 1244.508, 1318.510, 1396.913, 1479.978,  //66
  1567.982, 1661.219, 1760.000, 1864.655, 1975.533,  //71
  2093.005, 2217.461, 2349.318, 2489.016, 2637.020,  //76
  2793.826, 2959.955, 3135.963, 3322.438, 3520.000,  //81
  3729.310, 3951.066, 4186.009                       //86
};



/************ Guitar ************/
/* Guitar Railsback Data */
const float guitar_railsback_freq_data[7] = {
  1,
  0,0,0,0,0,0
};

/* Guitar Standard E */
const uint8_t string_name_guitar_StdE[7] = {
  1,
  20,25,30,35,39,44
};

/* Guitar Drop D */
const uint8_t string_name_guitar_DropD[7] = {
  1,
  18,25,30,35,39,44
};

/* Guitar Drop C# */
const uint8_t string_name_guitar_DropCs[7] = {
  1,
  17,24,29,34,38,43
};

/* Guitar Drop C */
const uint8_t string_name_guitar_DropC[7] = {
  1,
  16,23,28,33,37,42
};

/* Guitar Drop B */
const uint8_t string_name_guitar_DropB[7] = {
  1,
  15,22,27,32,36,41
};

/* Guitar Drop A */
const uint8_t string_name_guitar_DropA[7] = {
  1,
  13,20,25,30,34,39
};

/* Guitar DADGAD */
const uint8_t string_name_guitar_DADGAD[7] = {
  1,
  18,25,30,35,37,42
};

/* Guitar Half step down */
const uint8_t string_name_guitar_HalfStepDown[7] = {
  1,
  19,24,29,34,38,43
};

/* Guitar Full step down */
const uint8_t string_name_guitar_FullStepDown[7] = {
  1,
  18,23,28,33,37,42
};

/* Guitar Full step up */
//const uint8_t string_name_guitar_FullStepUp[7] = {
//  1,
//  
//};

/* Guitar Open C */
const uint8_t string_name_guitar_OpenC[7] = {
  1,
  16,23,28,35,40,43
};

/* Guitar Open D */
const uint8_t string_name_guitar_OpenD[7] = {
  1,
  18,25,30,32,37,42
};

/* Guitar Open E */
const uint8_t string_name_guitar_OpenE[7] = {
  1,
  20,27,32,35,39,44
};

/* Guitar Open F */
//const uint8_t string_name_guitar_OpenF[7] = {
//  1,
//  
//};

/* Guitar Open G */
const uint8_t string_name_guitar_OpenG[7] = {
  1,
  18,23,30,35,39,42
};

/* Guitar Open A */
//const uint8_t string_name_guitar_OpenA[7] = {
//  1,
//  
//};


/**************** struct InstrumentData ****************/
const struct InstrumentData MusicalInstrumentData[] = {
  /* 12 EqualTemperament */
//  {
//    "12 EqualTemperament",
//    "12 EqualTemperament",
//    88,
//    string_name_pinao,
//    piano_railsback_freq_data_12equal
//  },

  /* Guitar Standard E */
  {
    "Guitar",
    "Stand E",
    6,
    string_name_guitar_StdE,
    guitar_railsback_freq_data
  },

  /* Guitar Drop D */
  {
    "Guitar",
    "Drop D",
    6,
    string_name_guitar_DropD,
    guitar_railsback_freq_data
  },

  /* Guitar Drop C# */
  {
    "Guitar",
    "Drop C#",
    6,
    string_name_guitar_DropCs,
    guitar_railsback_freq_data
  },

  /* Guitar Drop C */
  {
    "Guitar",
    "Drop C",
    6,
    string_name_guitar_DropC,
    guitar_railsback_freq_data
  },

  /* Guitar Drop B */
  {
    "Guitar",
    "Drop B",
    6,
    string_name_guitar_DropB,
    guitar_railsback_freq_data
  },

  /* Guitar Drop A */
  {
    "Guitar",
    "Drop A",
    6,
    string_name_guitar_DropA,
    guitar_railsback_freq_data
  },

  /* Guitar DADGAD */
  {
    "Guitar",
    "DADGAD",
    6,
    string_name_guitar_DADGAD,
    guitar_railsback_freq_data
  },

  /* Guitar Half step down */
  {
    "Guitar",
    "Half step down",
    6,
    string_name_guitar_HalfStepDown,
    guitar_railsback_freq_data
  },

  /* Guitar Full step down */
  {
    "Guitar",
    "Full step down",
    6,
    string_name_guitar_FullStepDown,
    guitar_railsback_freq_data
  },

  /* Guitar Full step up 
  {
    "Guitar",
    "Full step up",
    6,
    string_name_guitar_FullStepUp,
    guitar_railsback_freq_data
  },*/

  /* Guitar Open C */
  {
    "Guitar",
    "Open C",
    6,
    string_name_guitar_OpenC,
    guitar_railsback_freq_data
  },

  /* Guitar Open D */
  {
    "Guitar",
    "Open D",
    6,
    string_name_guitar_OpenD,
    guitar_railsback_freq_data
  },

  /* Guitar Open E */
  {
    "Guitar",
    "Open E",
    6,
    string_name_guitar_OpenE,
    guitar_railsback_freq_data
  },

  /* Guitar Open F 
  {
    "Guitar",
    "Open F",
    6,
    string_name_guitar_OpenF,
    guitar_railsback_freq_data
  },*/

  /* Guitar Open G */
  {
    "Guitar",
    "Open G",
    6,
    string_name_guitar_OpenG,
    guitar_railsback_freq_data
  },

  /* Guitar Open A 
  {
    "Guitar",
    "Open A",
    6,
    string_name_guitar_OpenA,
    guitar_railsback_freq_data
  },*/
};





/**
  * @brief  
  *         
  * @note   
  *         
  * @param  None
  * @retval None
  */


/* CODE END ------------------------------------------------------------------*/
/********************************END OF FILE***********************************/