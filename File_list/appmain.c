/** 
**	A Template for developing new terminal application
**/

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
/** These two files are necessary for calling CTOS API **/
#include <ctosapi.h>
#include "GPRS.h"
#include "files.h"


BYTE bTXBuffer[1024];
BYTE bBuffer[1024];
BYTE bRXBuffer[1024];
BYTE bIPaddress[]="197.34.97.23";

/** 
** The main entry of the terminal application 
**/
USHORT send_file_list(){
	FILETYPE stFileList[255];
	USHORT usFileNo,usret;
	BYTE status,key;
	ULONG ulBuffLen=1024;
	ULONG ulHeaderLen=1024;
	CTOS_TCP_GPRSStatus(&status);
	if(status!=TCP_GPRS_STATE_ESTABLISHED){
		return 255;
		CTOS_LCDTPrint("\nfe khapth");	
	}
	CTOS_LCDTPrint("\nbefore list");	
	//CTOS_KBDGet(&key);

	usret=listAllFiles(stFileList,&usFileNo);
	CTOS_LCDTPrint("\nafter list");	
	//CTOS_KBDGet(&key);
	
    if(usret!=d_OK)return usret;
	
	usret=packingFilesInfo(stFileList,usFileNo,bBuffer,&ulBuffLen);
	
	
	if(usret!=d_OK){CTOS_LCDTPrint("\nafter packing error");
	CTOS_KBDGet(&key);
	return usret;}
	CTOS_LCDTPrint("\nnsend?\n");	
	CTOS_KBDGet(&key);
	ulBuffLen=strlen(bBuffer);
	sprintf(bTXBuffer,"POST /public/display.php HTTP/1.0\nContent-Type: application/x-www-form-urlencoded\nAccept: */*\nContent-length:%d\n\n%s\n\n",ulBuffLen,bBuffer);
	
	
	//CTOS_LCDTPrint(bTXBuffer);	
	//CTOS_KBDGet(&key);
	ulBuffLen=strlen(bTXBuffer);
	sprintf(bBuffer, "\nTX Len = %d \n", ulBuffLen);
			CTOS_LCDTPrint (bBuffer);
	usret=GPRS_send(bTXBuffer,ulBuffLen);
	CTOS_LCDTPrint("\nba3d elsend");	
	CTOS_KBDGet(&key);
	if(usret!=d_OK)return usret;
	return d_OK;
}
USHORT send_get_request(){
    USHORT status;
    USHORT usret;
    ULONG ulBuffLen=1024;
    CTOS_TCP_GPRSStatus(&status);

	if(status!=TCP_GPRS_STATE_ESTABLISHED){
		return 255;
		CTOS_LCDTPrint("\nfe khapth");	
	}
	strcpy(bTXBuffer,"GET /localiiswebsite/Default2.aspx HTTP/1.0\r\nContent-Type: text/plain; charset=utf-8\r\nAccept: */*\r\nAccept-Language: en-US,en;q=0.8,ar;q=0.6,es;q=0.4\r\n\r\n");
	ulBuffLen=strlen(bTXBuffer);
	usret=GPRS_send(bTXBuffer,ulBuffLen);
	if(usret!=d_OK)return usret;
	return d_OK;
}	
void gprs_session(){
	GPRS_PARAM_init();
	CTOS_LCDTPrint("\ninit Done");	

	GPRS_open();
	CTOS_LCDTPrint("\nopen Done");	
	
	
}
int main(int argc,char *argv[])
{
	BYTE key,tempkey;
    USHORT usret;
    ULONG ulBuffLen;
	// TODO: Add your program here //
	CTOS_LCDTClearDisplay();
    CTOS_LCDTPrintXY(1, 1, "Hello");
    gprs_session();
    while(1){
    	CTOS_LCDTClearDisplay();
    	CTOS_LCDTPrintXY(1, 2, "send File list");
    	CTOS_LCDTPrintXY(1, 3, "GET");
	    CTOS_KBDGet(&key);	
	    switch(key){
	    	case d_KBD_1:
	    	CTOS_LCDTClearDisplay();
    		CTOS_LCDTPrintXY(1, 1, "KBD:1");
	    	//gprs_session();
		    	GPRS_connect(bIPaddress,80);
				CTOS_LCDTPrint("\nconnection Done");
	    		send_file_list();
	    		ulBuffLen=1024;
	    		GPRS_recieve(bRXBuffer,&ulBuffLen);
	    		CTOS_LCDTPrint(bRXBuffer);
	    		memset(bRXBuffer,0,sizeof(bRXBuffer));
	    		CTOS_KBDGet(&tempkey);
	    		break;
	    	case d_KBD_2:
	    	CTOS_LCDTClearDisplay();
    		CTOS_LCDTPrintXY(1, 1, "KBD:2");
	    	//gprs_session();
	    		GPRS_connect(bIPaddress,80);
				CTOS_LCDTPrint("\nconnection Done");
	    		send_get_request();
	    		ulBuffLen=1024;
	    		GPRS_recieve(bRXBuffer,&ulBuffLen);
	    		CTOS_LCDTPrint(bRXBuffer);
	    		memset(bRXBuffer,0,sizeof(bRXBuffer));
	    		CTOS_KBDGet(&tempkey);
	    		break;

	    		
	    	case d_KBD_0:
	    	CTOS_LCDTClearDisplay();
    		CTOS_LCDTPrintXY(1, 1, "KBD:0");
	    		goto Terminate;
	    	default :
	    		break;
	    }
    }   
    Terminate: 
    GPRS_disconnect();
 	GPRS_close();
	CTOS_KBDGet(&key);
	exit(0);
}
