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
/* Define to prevent recursive inclusions ------------------------------------*/

#ifndef GPRS_H
#define GPRS_H

/*========================================*
* I N C L U D E S *
*========================================*/
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctosapi.h>

/*==========================================*
****************P U B L I C ****************
*==========================================*/

/*==========================================*
* D E F I N E S *
*==========================================*/
	
//#define DEBUG_LCD

/*========================================*
* D E F I N E S *
*========================================*/

/*==========================================*
* C O N S T A N T S *
*==========================================*/

/*==========================================*
* T Y P E S *
*==========================================*/

typedef enum
{
	GPRS_ESTABLISHED,
	GPRS_ESTABLISHING,
	GPRS_CONNECTING,
	GPRS_SENDING,
	GPRS_RECEIVING,
	GPRS_DISCONNECTING,
	GPRS_ONHOOKING,
	GPRS_UNKNOWN
} enuGprsStateType;
/*==========================================*
* V A R I A B L E S *
*==========================================*/


/*==========================================*
* M A C R O S *
*==========================================*/

/*==========================================*
* F U N C T I O N S*
*==========================================*/
	enuGprsStateType gprsGetState();
	USHORT gprsOpen();
	USHORT gprsConnect(BYTE *caIP,USHORT usPort);
	USHORT gprsSend(BYTE *baSendBuf,USHORT usSendLen);
	USHORT gprsRecieve(BYTE *baRecvBuf,USHORT *usRecvLen);
	USHORT gprsDisconnect();
	USHORT gprsClose();


	
	//USHORT pollStatusResultLCD(BYTE *func, USHORT theRTN);
	//USHORT pollStatusResult(DWORD* dwStatus);
#endif