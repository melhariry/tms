/**
  ******************************************************************************
  * file          WIFI.h
  * author        
  * component     WIFI
  * description   header file of wifi
  *               
  ******************************************************************************
  * copyright POS_GP_TEAM_2016
  *
  ******************************************************************************
  */
/* Define to prevent recursive inclusions ------------------------------------*/

#ifndef WIFI_H
#define WIFI_H

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
	
#define DEBUG_LCD

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
	
	WIFI_FAILED,
	WIFI_IO_PROCESSING,
	WIFI_AP_CONNECTED,
	WIFI_SCANNING,
	WIFI_AP_CONNECTING,
	WIFI_CONNECTING,
	WIFI_SENDING,
	WIFI_RECEIVING,
	WIFI_DISCONNECTING,
	WIFI_AP_DISCONNECTING

} enuWifiStateType;
/*==========================================*
* V A R I A B L E S *
*==========================================*/


/*==========================================*
* M A C R O S *
*==========================================*/

/*==========================================*
* P R O T O T Y P E S*
*==========================================*/
	enuWifiStateType wifiGetState(void);
	USHORT wifiOpen(void);
	USHORT wifiScan(void);
	USHORT wifiConnectSSID(BYTE *baSSID,BYTE* baPassword);
	USHORT wifiConnectSocket(BYTE* bsocket,BYTE *baIP,USHORT usPort);
	USHORT wifiSend(BYTE *baSendBuf,USHORT usSendLen);
	USHORT wifiRecieve(BYTE *baRecvBuf,USHORT *usRecvLen);
	USHORT wifiDisconnect(void);
	USHORT wifiClose(void);
#endif