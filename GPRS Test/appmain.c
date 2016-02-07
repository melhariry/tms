/***********************************************************************
* FILE NAME: GPRS.c
* PROGRAMMER: Vance Ke
* DESCRIPTION: Test GPRS Functions
* REVISION: 01.00
***********************************************************************/
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
#define PPP_TO_MS 45000 //45000ms
#define CONNECT_TO_MS 10000 // 5000ms
#define CONNECT_RETRY 2
/*===================================================================
* FUNCTION NAME: pollStatusResult
* DESCRIPTION: Show the status and return value of GPRSStatus and return until not
get IO_PROCESSING state.
* RETURN: The return value of GPRSStatus
 * * NOTES: none
*==================================================================*/
USHORT pollStatusResult(BYTE *func, USHORT theRTN)
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
void strncpy(BYTE *dest,const BYTE *source,ULONG ulLen){
	ULONG i;
	for(i=0;i<ulLen;++i){
		dest[i]=source[i];
	}
	dest[i++]='\r';
	dest[i++]='\n';
	dest[i]='\0';

}
void printResponse(BYTE *baResponse,USHORT usRespLen){
	USHORT i;
	USHORT Line=(usRespLen+15)/15;
	BYTE baStr[20];
	for(i=0;i<Line;++i){
		strncpy(baStr,baResponse+15*i,15);
		CTOS_LCDTPrint(baStr);
	}
}
/*==================================================================
* FUNCTION NAME: main
* DESCRIPTION: Main entry for the GPRS testing sample code.
* RETURN: none
* NOTES: none
*================================================================= */
int main(void)
{
	BYTE iSocket; //TCP socket descriptor
	USHORT usrtn , usrtn2 = d_OK;
	BYTE key;
	BYTE bStr[30];
	BYTE baLocalIP[4] = {0};
	BYTE baRemotelIP[4] ={ 50, 63 , 202 , 11}; //Host IP Address, el zetonna  
	USHORT usPort = 80;
	char APN[26] = "internet.vodafone.net";
	USHORT usSendLen;
	USHORT usRecvLen;
	BYTE baSendBuf[1024]; //Send buffer
	BYTE baRecvBuf[1100]; //Receive buffer
	CTOS_BackLightSet(d_BKLIT_LCD,d_ON);
	//strcpy(baSendBuf, "GET / HTTP/1.0\r\nConnection: close\r\nAccept-Charset: ISO-8859-1,UTF-8;q=0.7,*;q=0.7\r\nCache-Control: no-cache\r\nAccept-Language: de,en;q=0.7,en-us;q=0.3\r\nReferer: http://web-sniffer.net/\r\n\r\n" ); //Set sending buffer
	//strcpy(baSendBuf, "GET /public/hello.html HTTP/1.0\r\nConnection: close\r\n	Accept-Charset: ISO-8859-1,UTF-8;q=0.7,*;q=0.7\r\nCache-Control: no-cache\r\nAccept-Language: de,en;q=0.7,en-us;q=0.3\r\n\r\n" ); //Set sending buffer
	strcpy(baSendBuf, "GET /public/hello.html HTTP/1.0\r\nContent-Type: text/plain; charset=utf-8\r\nAccept: */*\r\nAccept-Language: en-US,en;q=0.8,ar;q=0.6,es;q=0.4\r\n\r\n" ); //Set sending buffer
	usSendLen = usRecvLen = strlen(baSendBuf);
	while (1)
	{
		START_MENU:
			CTOS_LCDTClearDisplay ();
			CTOS_LCDTPrintXY(1, 1, "\fr TCP GPRS Test \fn");
			CTOS_LCDTPrintXY(1, 3, " OK -> Start");
			CTOS_LCDTPrintXY(1, 4, " Cancel -> Exit");
			CTOS_KBDGet ( &key );
			if( key == d_KBD_DOT )
				break;
			else if( key != d_KBD_ENTER )
				goto START_MENU;
			/* Initialize(Reset) the GSM device */
			CTOS_LCDTClearDisplay ();
			CTOS_LCDTPrint ( "GPRS Init...\n" );
			CTOS_TCP_GPRSInit();
			CTOS_LCDTPrint ("OK");
			CTOS_KBDGet ( &key );
			/* Set PPP timeout (ms) */
			usrtn = CTOS_PPP_SetTO(PPP_TO_MS);
			/* Do GPRS open with related config value */
			CTOS_LCDTClearDisplay ();
			usrtn = CTOS_TCP_GPRSOpen(baLocalIP ,APN ,"" ,"");
			usrtn = pollStatusResult("GPRS Open.....", usrtn);
			if (usrtn != d_OK)
				goto EXIT_ERR;

			/* Get the local IP address of the current connection */
			CTOS_LCDTPrint ( "\n\n" );
			CTOS_LCDTPrint ( "Get IP...\n" );
			usrtn = CTOS_TCP_GPRSGetIP(baLocalIP);
			if(usrtn != d_OK)
				goto EXIT_ERR_CLOSE;
			sprintf(bStr, "%d.%d.%d.%d",
			baLocalIP[0],baLocalIP[1],baLocalIP[2],baLocalIP[3]);
			CTOS_LCDTPrint (bStr );
			CTOS_KBDGet ( &key );
			/* Set TCP connecting timeout (ms) and retry time */
			usrtn = CTOS_TCP_SetConnectTO(CONNECT_TO_MS);
			usrtn = CTOS_TCP_SetRetryCounter(CONNECT_RETRY);
			/* Connect to the remote server */
			//usrtn = CTOS_TCP_GPRSConnectEx(&iSocket ,baRemotelIP ,usPort);
			//usrtn = CTOS_TCP_GPRSConnectURL(&iSocket ,"www.google.com" ,usPort);
			usrtn = CTOS_TCP_GPRSConnectURL(&iSocket ,"197.34.104.81" ,usPort);
			usrtn = pollStatusResult("GPRS Connect ", usrtn);
			if (usrtn != d_OK)
				goto EXIT_ERR_CLOSE;
			CTOS_KBDGet ( &key );
			/* Send TCP packet data out */
			CTOS_LCDTPrint ("Transmitting...\n" );
			usrtn = CTOS_TCP_GPRSTx(iSocket ,baSendBuf ,usSendLen);
			usrtn = pollStatusResult("GPRS Tx", usrtn);
			CTOS_LCDTPrint ("\n" );
			sprintf(bStr, "TX Len = %d \n", usSendLen);
			CTOS_LCDTPrint (bStr);
			if (usrtn != d_OK)
			goto EXIT_ERR_DISCONNECT;
			CTOS_KBDGet ( &key );
			/* Receive TCP packet data ---
			* When get d_OK with insufficient data , you need to call CTOS_TCP_GPRSRx()
			again to get left. */
			CTOS_LCDTPrint ("Receiving... \n" );
			usRecvLen=1000;
			usrtn = CTOS_TCP_GPRSRx(iSocket ,baRecvBuf ,&usRecvLen);
			usrtn = pollStatusResult("GPRS Rx", usrtn);
			CTOS_LCDTPrint ("\n" );
			sprintf(bStr, "RX Len = %d \n", usRecvLen);
			CTOS_LCDTPrint (bStr);
			if (usrtn != d_OK)
			goto EXIT_ERR_DISCONNECT;
			CTOS_KBDGet ( &key );
			//=================
			CTOS_LCDTClearDisplay ();
			//printResponse(baRecvBuf+200,usRecvLen-200);
			CTOS_LCDTPrint (baRecvBuf);
			CTOS_KBDGet ( &key );
			//=================
			/* Disconnect the remote server */
			usrtn = CTOS_TCP_GPRSDisconnect(iSocket);
			usrtn = pollStatusResult("GPRS Disconnect", usrtn);
			if (usrtn != d_OK)
			goto EXIT_ERR_DISCONNECT;
			CTOS_KBDGet ( &key );
			#if 1 /* Close the GSM device -- SYNC way */
				CTOS_LCDTClearDisplay ();
				CTOS_LCDTPrintXY(1, 1, "GPRS Close...");
				usrtn = CTOS_TCP_GPRSClose();
				sprintf(bStr , "Return:%04x " , usrtn);
				CTOS_LCDTPrintXY(1, 3, bStr);
			#else /* Close the GSM device -- ASYNC way */
				usrtn = CTOS_TCP_GPRSClose_A();
				usrtn = pollStatusResult("GPRS Close A...", usrtn);
			#endif
			if (usrtn != d_OK)
				goto EXIT_ERR_CLOSE;
			CTOS_LCDTPrint ("\nOK");
			CTOS_KBDGet ( &key );
			CTOS_LCDTClearDisplay ();
	}
	exit(0);
	EXIT_ERR_DISCONNECT:
		usrtn2 = CTOS_TCP_GPRSDisconnect(iSocket);
		usrtn2 = pollStatusResult("GPRS Disconnect*", usrtn2);
		EXIT_ERR_CLOSE:
		usrtn2 = CTOS_TCP_GPRSClose();
		usrtn2 = pollStatusResult("GPRS Close* ", usrtn2);
	EXIT_ERR:
		CTOS_KBDGet ( &key );
		CTOS_LCDTPrint ("\n" );
		sprintf(bStr, "Err Ret:%04x ", usrtn);
		CTOS_LCDTPrint(bStr );
		CTOS_LCDTPrint ("\n" );
		if (usrtn2 != d_OK)
		{
			sprintf(bStr, "Err Exit:%04x ", usrtn2);
			CTOS_LCDTPrint(bStr );
		}
		CTOS_KBDGet ( &key );
		CTOS_LCDTClearDisplay ();
		exit(1);
}
// eof