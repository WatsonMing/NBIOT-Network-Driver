#ifndef __COMMAND_H_
#define __COMMAND_H_

#include "usart.h"

#define SUCCESS 1
#define ERROR 0
/*************************************�����嵥************************************/
enum TaskList_BC95
{
	Task_AT,
	Task_IMEI,
	Task_IMSI,
	Task_RF,
	Task_Signal,
	Task_APNSet,
	Task_GPRS,
	Task_GPRSCheck,
	Task_ServiceCheck,
	Task_Connect,
	Task_UDPSocket,
};
/************************************��������**************************************/
bit At(void);
bit GetIMEI(void);
bit GetIMSI(void);
bit RadioFrequency(void);
bit SignalCheck(void);
bit ApnSet(void);
bit GPRS(void);
bit GPRSCheak(void);
bit ServiceCheck(void);
bit Connect(void);
bit UDPSocket(void);
unsigned char* Bc95_Init(void);//����ID
void SendData(unsigned char * data);

#endif