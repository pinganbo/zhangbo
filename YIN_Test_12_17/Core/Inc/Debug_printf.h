#ifndef _DEBUG_PRINTF_H_
#define _DEBUG_PRINTF_H_
#ifdef __cplusplus
 extern "C" {
#endif

#define MAX_DEBBUF			(1024)

typedef struct{
	uint8_t valid;
	uint8_t cmd[MAX_DEBBUF];
	uint16_t cmdLen;
}stDebugCmd;

void myPrintf(char* str, ...);

void myPrintfChar(uint8_t* buf);
void myPrintfHex(uint32_t num);
void myPrintfDec(uint32_t num);

uint8_t SERDEB_CmdValid(void);
void SERDEB_PushCmd(uint8_t *cmd, uint16_t len);
void SERDEB_Handler(void);

#ifdef __cplusplus
}
#endif
#endif
