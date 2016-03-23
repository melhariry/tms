/**
  ******************************************************************************
  * file          gprs.h
  * author        
  * component     gprs
  * description   implementation of gprs component
  *               gprs component is responsible for handling gprs module xD
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


/*========================================*
* D E F I N E S *
*========================================*/
#define PPP_TO_MS 35000 //45000ms
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
* P R O T O T Y P E S *
*==========================================*/
USHORT pollStatusResultLCD(BYTE *func, USHORT theRTN);
USHORT pollStatusResult(void);
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
USHORT pollStatusResult()
{  
	USHORT usret;
	DWORD status;	
	status = 0;
	while (1)
	{
		usret = CTOS_TCP_GPRSStatus(&status);
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
	USHORT ret;
	enuGprsStateType gprsState=gprsGetState();
	CTOS_TCP_GPRSInit();
	/* Set TCP connecting timeout (ms) and retry time */
	ret = CTOS_TCP_SetConnectTO(CONNECT_TO_MS);
	ret = CTOS_TCP_SetRetryCounter(CONNECT_RETRY);
	ret = CTOS_PPP_SetTO(PPP_TO_MS);
	#ifdef DEBUG_LCD
	ret = pollStatusResultLCD("PPP_TO_MS .....1??", ret);
	#else
	ret=pollStatusResult();
	#endif
	if(ret!=d_OK)return ret;
	ret = CTOS_TCP_GPRSOpen(gprsConfig.baLocalIP ,gprsConfig.APN ,"" ,"");
	#ifdef DEBUG_LCD
	ret = pollStatusResultLCD("GPRS Open.....3??", ret);
	#endif
	return ret;
	
	
}
USHORT gprsConnect(BYTE *baIP,USHORT usPort)
{
	USHORT usrtn;

	usrtn = CTOS_TCP_GPRSConnectURL(&gprsConfig.iSocket ,baIP,usPort);
    #ifdef DEBUG_LCD
	usrtn = pollStatusResultLCD("GPRS Connect ", usrtn);
	#else
	usrtn=pollStatusResult();
	#endif
	if (usrtn != d_OK) return usrtn;
	return d_OK;
}

USHORT gprsSend(BYTE *baSendBuf,USHORT usSendLen)
{
	
	USHORT usrtn;
	enuGprsStateType gprsState=gprsGetState();
	//why blocking xD
	while(gprsState==GPRS_SENDING)
		gprsState=gprsGetState();

	usrtn = CTOS_TCP_GPRSTx(gprsConfig.iSocket ,baSendBuf ,usSendLen);
	#ifdef DEBUG_LCD
	usrtn = pollStatusResultLCD("GPRS Tx", usrtn);
	#else
	usrtn=pollStatusResult();
	#endif
	return usrtn;
}
USHORT gprsRecieve(BYTE *baRecvBuf,USHORT *usRecvLen)
{
	USHORT usrtn;
	enuGprsStateType gprsState=gprsGetState();
	while(gprsState==GPRS_RECEIVING)
		gprsState=gprsGetState();
	usrtn = CTOS_TCP_GPRSRx(gprsConfig.iSocket ,baRecvBuf ,usRecvLen);
	#ifdef DEBUG_LCD
	usrtn = pollStatusResultLCD("GPRS Rx", usrtn);
	#else
	usrtn=pollStatusResult();
	#endif
	return usrtn;
}
USHORT gprsDisconnect()
{
	USHORT usrtn;
	usrtn = CTOS_TCP_GPRSDisconnect(gprsConfig.iSocket);
	#ifdef DEBUG_LCD
	usrtn = pollStatusResultLCD("GPRS Disconnect", usrtn);
	#else
	usrtn=pollStatusResult();
	#endif
	return usrtn;
}
USHORT gprsClose()
{
	USHORT usrtn;
	usrtn = CTOS_TCP_GPRSClose();
	#ifdef DEBUG_LCD
	usrtn = pollStatusResultLCD("GPRS Close", usrtn);
	#else
	usrtn=pollStatusResult();
	#endif
	return usrtn;
}
