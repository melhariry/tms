/**
  ******************************************************************************
  * file          common.h
  * author        
  * component     common
  * description   common file contains constants and variable 
  *               that maybe used by different components in application.
  ******************************************************************************
  * copyright POS_GP_TEAM_2016
  *
  ******************************************************************************
  */
/* Define to prevent recursive inclusions ------------------------------------*/

#ifndef COMMON_H
#define COMMON_H

/*========================================*
* I N C L U D E S *
*========================================*/
#include <string.h>
#include <ctosapi.h>

/*==========================================*
****************P U B L I C ****************
*==========================================*/

/*==========================================*
* D E F I N E S *
*==========================================*/
	
/*==========================================*
* C O N S T A N T S *
*==========================================*/

/*==========================================*
* T Y P E S *
*==========================================*/

/*==========================================*
* V A R I A B L E S *
*==========================================*/
	BYTE baSerialNumber[40];
	//BYTE baRemoteIP[]="197.34.110.15";
	//BYTE bIPaddress[]="197.34.110.15";
	BYTE bIPaddress[20];
	BYTE baHeader[200];
	//BYTE baExtraParam[200];
	BYTE baMessage[300];
	BYTE baResponse[100];
	BYTE baPayload[100];
	BYTE baReply[400];

/*==========================================*
* M A C R O S *
*==========================================*/

/*==========================================*
* F U N C T I O N S *
*==========================================*/







#endif