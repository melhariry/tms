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

/*========================================*
* D E F I N E S *
*========================================*/
#define PPP_TO_MS 25000 //45000ms
#define CONNECT_TO_MS 5000 // 5000ms
#define CONNECT_RETRY 2

#define SIZE_SENDBUFF 1024
#define SIZE_RECVBUFF 1024

struct GPRS_PARAM{
	BYTE iSocket; //TCP socket descriptor
	BYTE baLocalIP[4] ;
	BYTE baRemotelIP[4]; //Host IP Address 
	USHORT usPort;
	char APN[26] ;//= "internet.vodafone.net";
	USHORT usSendLen;
	USHORT usRecvLen;
	BYTE baSendBuf[1024]; //Send buffer
	BYTE baRecvBuf[1100]; //Receive buffer
	
	



}stGPRSParam;
void setHostIP(BYTE *baHOST){
	int i;
	for(i=0;i<4;++i){
		stGPRSParam.baRemotelIP[i]=baHOST[i];
	}
}
void GPRS_PARAM_init(void){
	stGPRSParam.usPort=80;

	strcpy(stGPRSParam.baLocalIP,"\x00\x00\x00\x00");
	strcpy(stGPRSParam.APN,"internet.vodafone.net");//Vodafone
}


//Utility function
void util_strncpy(BYTE *dest,const BYTE *source,ULONG ulLen){
	ULONG i;
	for(i=0;i<ulLen;++i){
		dest[i]=source[i];
	}
	dest[i++]='\r';
	dest[i++]='\n';
	dest[i]='\0';
}


/*===================================================================
* FUNCTION NAME: pollStatusResult
* DESCRIPTION: Show the status and return value of GPRSStatus and return until not
get IO_PROCESSING state.
* RETURN: The return value of GPRSStatus
 * * NOTES: none
*==================================================================*/
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
			CTOS_Delay(2000);
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
			CTOS_Delay(2000);//sleep
			continue;
		}
		else
			break;
	}

	return usret;
}
USHORT GPRS_open(){
	USHORT ret;
	CTOS_TCP_GPRSInit();
	ret = CTOS_PPP_SetTO(PPP_TO_MS);
	ret = pollStatusResultLCD("PPP_TO_MS .....1??", ret);
	if(ret!=d_OK)return ret;
	ret = CTOS_TCP_GPRSOpen(stGPRSParam.baLocalIP ,stGPRSParam.APN ,"" ,"");
	ret = pollStatusResultLCD("GPRS Open.....2??", ret);
	return ret;
}
USHORT GPRS_connect(BYTE *caIP,USHORT usPort){
	USHORT usrtn;
	/* Set TCP connecting timeout (ms) and retry time */
	usrtn = CTOS_TCP_SetConnectTO(CONNECT_TO_MS);
	usrtn = CTOS_TCP_SetRetryCounter(CONNECT_RETRY);
	usrtn = CTOS_TCP_GPRSConnectURL(&stGPRSParam.iSocket ,caIP,usPort);
	usrtn = pollStatusResultLCD("GPRS Connect ", usrtn);
	if (usrtn != d_OK) return usrtn;
	return d_OK;
}
USHORT GPRS_send(BYTE *baSendBuf,USHORT usSendLen){
	USHORT usrtn;
	usrtn = CTOS_TCP_GPRSTx(stGPRSParam.iSocket ,baSendBuf ,usSendLen);
	usrtn = pollStatusResultLCD("GPRS Tx", usrtn);
	return usrtn;
}
USHORT GPRS_recieve(BYTE *baRecvBuf,USHORT *usRecvLen){
	USHORT usrtn;
	usrtn = CTOS_TCP_GPRSRx(stGPRSParam.iSocket ,baRecvBuf ,usRecvLen);
	usrtn = pollStatusResultLCD("GPRS Rx", usrtn);
	return usrtn;
}
USHORT GPRS_disconnect(){
	USHORT usrtn;
	usrtn = CTOS_TCP_GPRSDisconnect(stGPRSParam.iSocket);
	usrtn = pollStatusResultLCD("GPRS Disconnect", usrtn);
	return usrtn;
}
USHORT GPRS_close(){
	USHORT usrtn;
	usrtn = CTOS_TCP_GPRSClose();
	usrtn = pollStatusResultLCD("GPRS Close", usrtn);
	return usrtn;
}

#endif