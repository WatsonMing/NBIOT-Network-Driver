#include "command.h"
/*******************ȫ�ֱ���*********************/
/*************************************************
*������
*IMEI
*IMSI=SN
*************************************************/
enum TaskList_BC95 TaskMAX=Task_UDPSocket;
unsigned char ip[]="123.56.250.155";
unsigned char port[]="8002";
unsigned char IMEI[15];
unsigned char IMSI[15];
unsigned char TaskFlag=0;
/********************************************************************
��ջ�����
*********************************************************************/
void cleanRxBuff(void)
{
	USART_RX_STA=0;
	memset(USART_RX_BUF,0,sizeof(USART_RX_BUF));
	ReceivedFlag=0;
}
/********************************************************************
//ģ���Ƿ���������
OK
*********************************************************************/
bit At(void)
{
	unsigned char received[]="OK";
	SendString("AT\r\n");
	delay_ms(100);
	if(strcmp(received,USART_RX_BUF)==0)
	{
		return SUCCESS;
	}
	else return ERROR;
}
/********************************************************************
CFUN�Ĺ�������Ƶ����
+CFUN:1
OK
*********************************************************************/
bit RadioFrequency(void)
{
	unsigned char received[]="+CFUN:1OK";
	SendString("AT+CFUN?\r\n");
	delay_ms(100);
	if(strcmp(received,USART_RX_BUF)==0)
	{
		return SUCCESS;
	}
	else return ERROR;
}
/********************************************************************
//���Ų�ѯ
460111176368594
OK
*********************************************************************/
bit GetIMEI(void)
{
	unsigned char len;
	unsigned char *received;
	unsigned char buff[15];
	memset(buff,'\0',sizeof(buff));
	SendString("AT+CIMI\r\n");
	delay_ms(100);
	received=USART_RX_BUF;
	if(strlen(received)>=14)
	{
		strncpy(buff,received,15);
		strncpy(IMEI,buff,15);		
		return SUCCESS;
	}
	else return ERROR; 
}
/********************************************************************
//�źŲ�ѯ
+CSQ:27,99
OK
*********************************************************************/
bit SignalCheck(void)
{
	unsigned char received[]="+CSQ:99,99OK";
	SendString("AT+CSQ\r\n");
	delay_ms(100);
	if(strcmp(received,USART_RX_BUF)==0)
	{
		return ERROR;
	}
	else  return SUCCESS;
}
/********************************************************************
//apn����
OK
*********************************************************************/
bit ApnSet(void)
{
	unsigned char received[]="OK";
	unsigned char display[]="APN Set Success";
	SendString("AT+CGDCONT=1,\"IP\",\"CTNB\"\r\n");
	delay_ms(100);
	if(strcmp(received,USART_RX_BUF)==0)
	{
		return SUCCESS;
	}
	else return ERROR;
}
/********************************************************************
//�ֶ�����GPRS
OK
*********************************************************************/
bit GPRS(void)
{
	unsigned char received[]="OK";
	SendString("AT+CGATT=1\r\n");
	delay_ms(100);
	if(strcmp(received,USART_RX_BUF)==0)
	{
		return SUCCESS;
	}else return ERROR;
}
/********************************************************************
//GPRS�Ƿ����
+CGATT:1
OK
*********************************************************************/
bit GPRSCheak(void)
{
	unsigned char received[]="+CGATT:1OK";
	SendString("AT+CGATT?\r\n");
	delay_ms(100);
	if(strcmp(received,USART_RX_BUF)==0)
	{
		return SUCCESS;
	}else return ERROR;
}
/********************************************************************
//����udpͨ��
0
OK
*********************************************************************/
bit UDPSocket(void)
{
	unsigned char received[]="0OK";
	SendString("AT+NSOCR=DGRAM,17,3005,1\r\n");
	delay_ms(100);
	if(strcmp(received,USART_RX_BUF)==0)
	{
		return SUCCESS;
	}
	else return ERROR;
}
/********************************************************************
//����1��5��ʾ����ҵ�����ʹ��
CEREG:0,1
OK
*********************************************************************/
bit ServiceCheck(void)
{
	unsigned char received[]="+CEREG:0,1OK";
	SendString("AT+CEREG?\r\n");
	delay_ms(100);
	if(strcmp(received,USART_RX_BUF)==0)
	{
		return SUCCESS;
	}else return ERROR;
}
/********************************************************************
//��ѯ�ź�����״̬��1��ʾ�����ӡ���0��ʾ�����С��� 
+CSCON:0,1
OK
*********************************************************************/
bit Connect(void)
{
	unsigned char received[]="+CSCON:0,0OK";
	SendString("AT+CSCON?\r\n");	
	delay_ms(100);
	if(strcmp(received,USART_RX_BUF)==0)
	{
		return SUCCESS;
	}
	else return ERROR;
}
/********************************************************************
��ȡ�豸SN
+CGSN:863703035872998
OK
********************************************************************/
bit GetIMSI(void)
{
	unsigned char font[6];
	unsigned char *received;
	unsigned char buff[15];
	memset(font,'\0',sizeof(font));
	SendString("AT+CGSN=1\r\n");
	delay_ms(200);
	received=USART_RX_BUF;
	strncpy(font,received,6);
	if(strcmp(font,"+CGSN:")==0)
	{
		memset(buff, '\0', sizeof(buff));
		strncpy(buff,received+6,15);
		strncpy(IMSI,buff,15);
		return SUCCESS;
	}
	else return ERROR;
}

void Hex2Str( const unsigned char *sSrc,unsigned char *sDest, unsigned char nSrcLen )
{
    unsigned char  i;
    unsigned char szTmp[3];
    for( i = 0; i < nSrcLen; i++ )
    {
        sSendString( szTmp, "%02X", (unsigned char) sSrc[i] );
        memcpy( &sDest[i * 2], szTmp, 2 );
    }
}
/********************************************************************
����udp���ݰ�
0,data����
OK
*********************************************************************/
void SendData(unsigned char * data)
{
	unsigned char dat[200]="\0";
	unsigned char send[300]="\0";
	unsigned char len;
	len=strlen(data);
	Hex2Str(data,dat,len);//�ַ���ת16������
	            //socket,ip,port,len(data),asc(data)
	sSendString(send,"AT+NSOST=0,%s,%s,%d,%s\r\n",ip,port,len,dat);	
	delay_ms(100);	
    cleanRxBuff();
	SendString(send);
}
/************************************************************************************/
void nextTask(void)
{
	cleanRxBuff();
	TaskFlag++;	
}
/*************************************************************************************
ͨ��ģ���ʼ����ɣ����з������ݹ���
���أ�IMSI����IMEI
IMEI���绰������
IMSI��ͨ��оƬSN
**************************************************************************************/
unsigned char* Bc95_Init(void)
{
	for(TaskFlag;TaskFlag<=TaskMAX;TaskFlag++)
	{
		switch(TaskFlag)
		{
			case 0:
				if(At()==SUCCESS){nextTask();}
				else{ break;}
			case 1:
				if(GetIMEI()==SUCCESS){nextTask();}
				else{TaskFlag=0;break;}
			case 2:
				if(GetIMSI()==SUCCESS){nextTask();}
				else{TaskFlag=1;break;}
			case 3:
				if(SignalCheck()==SUCCESS){nextTask();}
				else { TaskFlag=2;break;}
			case 4:
				if(RadioFrequency()==SUCCESS){nextTask();}
				else{ TaskFlag=3;break;}
			case 5:
				if(ApnSet()==SUCCESS){nextTask();}
				else { TaskFlag=4;break;}			
			case 6:
				if(GPRS()==SUCCESS){nextTask();}
				else{ TaskFlag=5;break;}
			case 7:
				if(GPRSCheak()==SUCCESS){nextTask();}
				else{ TaskFlag=6;break;}
			case 8:
				if(ServiceCheck()==SUCCESS){nextTask();}
				else{ TaskFlag=7;break;}
			case 9:
				if(Connect()==SUCCESS){nextTask();}
				else{ TaskFlag=8;break;}
			case 10:
				if(UDPSocket()==SUCCESS){nextTask();}
				else{TaskFlag=9; break;}	
		}
		delay_ms(500);
		cleanRxBuff();
	}
	return IMSI;
}

