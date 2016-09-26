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


#include "wifi.h"


/*==========================================*
****************P R I V A T E ***************
*==========================================*/


/*========================================*
* D E F I N E S *
*========================================*/




/*==========================================*
* C O N S T A N T S *
*==========================================*/

/*==========================================*
* T Y P E S *
*==========================================*/


/*==========================================*
* V A R I A B L E S *
*==========================================*/



/*==========================================*
* M A C R O S *
*==========================================*/
/*==========================================*
* P R O T O T Y P E S *
*==========================================*/


/*==========================================*
* F U N C T I O N S *
*==========================================*/
enuWifiStateType wifiGetState(void)
{
	USHORT usret;
	DWORD dwState;
	enuWifiStateType wifiState;
	usret=CTOS_WifiStatus(&dwState);
	if(usret==0x6001) return WIFI_IO_PROCESSING;
	if(usret!=d_OK) return WIFI_FAILED;
	switch (dwState)
	{

		case d_WIFI_STATE_AP_CONNECTED:
			wifiState=WIFI_AP_CONNECTED;
		break;
		case d_WIFI_STATE_SCANNING:
			wifiState=WIFI_SCANNING;
		break;
		case d_WIFI_STATE_AP_CONNECTING:
			wifiState=WIFI_AP_CONNECTING;
		break;
		case d_WIFI_STATE_CONNECTING:
			wifiState=WIFI_CONNECTING;
		break;

		case d_WIFI_STATE_SENDING:
			wifiState=WIFI_SENDING;
		break;
		case d_WIFI_STATE_RECEIVING:
			wifiState=WIFI_SCANNING;
		break;
		case d_WIFI_STATE_DISCONNECTING:
			wifiState=WIFI_DISCONNECTING;
		break;
		case d_WIFI_STATE_AP_DISCONNECTING:
			wifiState=WIFI_AP_DISCONNECTING;
		break;
	}
	return wifiState;

}

USHORT wifiOpen(void)
{	
	return CTOS_WifiOpen();
}
USHORT wifiScan(void)
{
	USHORT usret;
	BYTE baBuff[100];
	BYTE NumInfo;
	int i;
	CTOS_stWifiInfo* stInfo[256];
	DWORD dwState;
	usret=CTOS_WifiScan();
	usret=CTOS_WifiStatus(&dwState);
				sprintf(baBuff,"ret=%04x st=%d\n",usret,dwState);
				CTOS_LCDTPrint(baBuff);
				while(usret==0x6001)
				{
					
					CTOS_Delay(500);
					usret=CTOS_WifiStatus(&dwState);

				}
				sprintf(baBuff,"ret=%04x st=%d\n",usret,dwState);
				CTOS_LCDTPrint(baBuff);
	sprintf(baBuff,"ret=%04x\n",usret);
	CTOS_LCDTPrint(baBuff);
	if(usret!=d_OK)
		return usret;
	CTOS_WifiInfoGet(&NumInfo, &stInfo);
sprintf(baBuff,"num=%04x\n",NumInfo);
CTOS_LCDTPrint(baBuff);
	for (i = 0; i < NumInfo; ++i)
	{
		stInfo[i]->ESSID[31]=0;
		CTOS_LCDTPrint("\n");
		CTOS_LCDTPrint(stInfo[i]->ESSID);
		if(strncmp(stInfo[i]->ESSID,"wired",5)==0)
		{
			CTOS_LCDTPrint("la2enaaha");
			break;
		}
	}
}
USHORT wifiConnectSSID(BYTE *baSSid,BYTE* baPassword)
{
	int i,j,k;
	BYTE baBuff[100];
	enuWifiStateType wifiState;
	USHORT usret;
	DWORD dwState;
	
	
	BYTE Protocal[]={d_WIFI_PROTOCAL_WEP,d_WIFI_PROTOCAL_WPA,d_WIFI_PROTOCAL_WPA2};
	BYTE Pairwise[]={d_WIFI_PAIRWISE_TKIP,d_WIFI_PAIRWISE_CCMP,d_WIFI_PAIRWISE_TKIPCCMP};
	BYTE Group[]={d_WIFI_GROUP_TKIP,d_WIFI_GROUP_CCMP,d_WIFI_GROUP_TKIPCCMP};
	for (i = 2; i < 3; ++i)//protocol
	{
		for (j = 2; j < 3; ++j)//pairwise
		{
			for (k = 2; k < 3; ++k)//group
			{
				//try
				CTOS_WifiConnectAPEx(baSSid,strlen(baSSid),Protocal[i],Pairwise[j],Group[k],baPassword,strlen(baPassword));
				
				usret=CTOS_WifiStatus(&dwState);
				sprintf(baBuff,"ret=%04x st=%d\n",usret,dwState);
				CTOS_LCDTPrint(baBuff);
				while(usret==0x6001)
				{
					
					CTOS_Delay(500);
					usret=CTOS_WifiStatus(&dwState);

				}
				sprintf(baBuff,"ret=%04x st=%d\n",usret,dwState);
				CTOS_LCDTPrint(baBuff);

				/*wifiState=wifiGetState();
				
				while(wifiState==WIFI_IO_PROCESSING){
					wifiState=wifiGetState();
					CTOS_LCDTPrint("-");
					sleep(10);
					CTOS_LCDTPrint("+");
				}*/
					
				if(wifiState==WIFI_AP_CONNECTED)break;
				else
					CTOS_LCDTPrint("\n failed\n");

			}
		}
	}
	if(wifiState==WIFI_AP_CONNECTED)return d_OK;
	else return d_WIFI_IO_APCONNECTFAILED;
}


