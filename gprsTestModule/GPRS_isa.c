/**
  ******************************************************************************
  * file          communication.h
  * author        
  * component     communication
  * description   header file of communication
  *               communication module provides the Terminal abstraction layer of 
  *               communication with TMS 
  ******************************************************************************
  * copyright POS_GP_TEAM_2016
  *
  ******************************************************************************
  */
/*========================================*
* I N C L U D E S *
*========================================*/
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctosapi.h>

#include "GPRS.h"

/*==========================================*
****************P R I V A T E ***************
*==========================================*/

/*==========================================*
* D E F I N E S *
*==========================================*/
	
#define PPP_TO_MS 15000 //45000ms
#define CONNECT_TO_MS 4000 // 5000ms
#define CONNECT_RETRY 2

#define SIZE_SENDBUFF 1024
#define SIZE_RECVBUFF 1024



/*==========================================*
* C O N S T A N T S *
*==========================================*/

/*==========================================*
* T Y P E S *
*==========================================*/
typedef struct
{
	BYTE iSocket; //TCP socket descriptor
	BYTE baLocalIP[4] ;
	BYTE baUrl[30] ;//remote IP
	char APN[26] ;//= "internet.vodafone.net";
}gprsConfigType;


/*==========================================*
* V A R I A B L E S *
*==========================================*/
gprsConfigType gprsConfig= { 0,
							"\x00\x00\x00\x00",
							"127.0.0.1",
							"internet.vodafone.net"};

/*==========================================*
* M A C R O S *
*==========================================*/

/*==========================================*
* F U N C T I O N S *
*==========================================*/

USHORT pollStatusResultLCD(BYTE *func, USHORT theRTN)
{
	BYTE key , str[17];
    
	USHORT usret;
	DWORD status;
	CTOS_LCDTClearDisplay ();
	CTOS_LCDTPrintXY(1, 1, func);
	memset(str, 0 , sizeof(str));
	CTOS_LCDTPrintXY(1, 2, str);
	status = 0;
	while (1)
	{
		usret = CTOS_TCP_GPRSStatus(&status);
		sprintf(str , "Return:%04x " , usret);
		CTOS_LCDTPrintXY(1, 3, str);
		sprintf(str , "Status:%x " , status);
		CTOS_LCDTPrintXY(1, 4, str);
		if (usret == 0x2321) //keep polling status
		{
			CTOS_Delay(400);
			continue;
		}
		else
			break;
	}
	return usret;
}
USHORT pollStatusResult(DWORD* dwStatus)
{
	BYTE key , str[17];
    
	USHORT usret;
	DWORD status;
	
	status = 0;
	while (1)
	{
		usret = CTOS_TCP_GPRSStatus(&status);
		sprintf(str , "Return:%04x " , usret);
		sprintf(str , "Status:%x " , status);
		*dwStatus=status;
		if (usret == 0x2321) //keep polling status
		{
			CTOS_Delay(200);//sleep
			continue;
		}
		else
			break;
	}

	return usret;
}



void GPRS_PARAM_init(void){
	// gprsConfig.usPort=80;

	// strcpy(gprsConfig.baLocalIP,"\x00\x00\x00\x00");
	// strcpy(gprsConfig.APN,"internet.vodafone.net");//Vodafone
}



enuGprsStateType gprsGetState()
{
    
	enuGprsStateType gprsState;
	USHORT usret;
	DWORD dwStatus;
    
	usret = CTOS_TCP_GPRSStatus(&dwStatus);
	switch(dwStatus)
	{
		case TCP_GPRS_STATE_ESTABLISHED:
			gprsState=GPRS_ESTABLISHED;
			break;
		case TCP_GPRS_STATE_ESTABLISHING:
			gprsState=GPRS_ESTABLISHING;
			break;
		case TCP_GPRS_STATE_CONNECTING:
			gprsState=GPRS_CONNECTING;
			break;
		case TCP_GPRS_STATE_SENDING:
			gprsState=GPRS_SENDING;
			break;
		case TCP_GPRS_STATE_RECEIVING:
			gprsState=GPRS_RECEIVING;
			break;
		case TCP_GPRS_STATE_DISCONNECTING:
			gprsState=GPRS_DISCONNECTING;
			break;
		case TCP_GPRS_STATE_ONHOOKING:
			gprsState=GPRS_ONHOOKING;
			break;
		default :
			gprsState=GPRS_UNKNOWN;
	}
	return gprsState;
}
USHORT gprsOpen()
{
	return GPRS_open();
	
}
USHORT gprsConnect(BYTE *baIP,USHORT usPort)
{
	return GPRS_connect(baIP,usPort);
}

USHORT gprsSend(BYTE *baSendBuf,USHORT usSendLen)
{

	return GPRS_send(baSendBuf,usSendLen);	
}
USHORT gprsRecieve(BYTE *baRecvBuf,USHORT *usRecvLen)
{
	
	return GPRS_recieve(baRecvBuf,usRecvLen);
}
USHORT gprsDisconnect()
{
	return GPRS_disconnect();
}
USHORT gprsClose()
{
	return GPRS_close();
}





USHORT GPRS_open(){
	USHORT ret;
	CTOS_TCP_GPRSInit();
	ret = CTOS_PPP_SetTO(PPP_TO_MS);
	ret = pollStatusResultLCD("PPP_TO_MS .....1??", ret);
	if(ret!=d_OK)return ret;
	ret = CTOS_TCP_GPRSOpen(gprsConfig.baLocalIP ,gprsConfig.APN ,"" ,"");
	ret = pollStatusResultLCD("GPRS Open.....2??", ret);
	return ret;
}
USHORT GPRS_connect(BYTE *caIP,USHORT usPort){
	USHORT usrtn;
	/* Set TCP connecting timeout (ms) and retry time */
	usrtn = CTOS_TCP_SetConnectTO(CONNECT_TO_MS);
	usrtn = CTOS_TCP_SetRetryCounter(CONNECT_RETRY);
	usrtn = CTOS_TCP_GPRSConnectURL(&gprsConfig.iSocket ,caIP,usPort);
	usrtn = pollStatusResultLCD("GPRS Connect ", usrtn);
	if (usrtn != d_OK) return usrtn;
	return d_OK;
}
USHORT GPRS_send(BYTE *baSendBuf,USHORT usSendLen){
	USHORT usrtn;
	usrtn = CTOS_TCP_GPRSTx(gprsConfig.iSocket ,baSendBuf ,usSendLen);
	usrtn = pollStatusResultLCD("GPRS Tx", usrtn);
	return usrtn;
}
USHORT GPRS_recieve(BYTE *baRecvBuf,USHORT *usRecvLen){
	USHORT usrtn;
	usrtn = CTOS_TCP_GPRSRx(gprsConfig.iSocket ,baRecvBuf ,usRecvLen);
	usrtn = pollStatusResultLCD("GPRS Rx", usrtn);
	return usrtn;
}
USHORT GPRS_disconnect(){
	USHORT usrtn;
	usrtn = CTOS_TCP_GPRSDisconnect(gprsConfig.iSocket);
	usrtn = pollStatusResultLCD("GPRS Disconnect", usrtn);
	return usrtn;
}
USHORT GPRS_close(){
	USHORT usrtn;
	usrtn = CTOS_TCP_GPRSClose();
	usrtn = pollStatusResultLCD("GPRS Close", usrtn);
	return usrtn;
}

//Utility function msh faker de kant leh xD
void util_strncpy(BYTE *dest,const BYTE *source,ULONG ulLen){
	ULONG i;
	for(i=0;i<ulLen;++i){
		dest[i]=source[i];
	}
	dest[i++]='\r';
	dest[i++]='\n';
	dest[i]='\0';
}
