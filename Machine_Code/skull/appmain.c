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
#include "httpclient.h"
#include "util.h"
#include "parser.h"


// constants
BYTE bIPaddress[]="197.34.104.248";



/** 
** The main entry of the terminal application 
**/
BYTE baHeader[200];
BYTE baExtraParam[200];
BYTE baMessage[300];
BYTE baResponse[400];
BYTE baReply[400];
BYTE baTXBuffer[1024];
BYTE baRXBuffer[1024];
BYTE baSerialNumber[40];
USHORT usBufferLen=1024;

char   paramlist[10][50];
int paramLen;
USHORT usCmdstatus;

void gprs_session(){
	GPRS_PARAM_init();
	CTOS_LCDTPrint("\ninit Done");	

	GPRS_open();
	CTOS_LCDTPrint("\nopen Done");	
}
void hello()
{
	BYTE key2;
	gprs_session();
	GetSerialNumber(baSerialNumber);
	sprintf(baExtraParam,"SerialNumber:%s\nVendor:%s",baSerialNumber,"Castles");
    HTTP_BuildHeader(baHeader,"/MTMS/PosUp.ashx","GET",baExtraParam);
    HTTP_BuildMessage(baMessage,baHeader,"");
	GPRS_connect(bIPaddress,80);
	GPRS_send(baMessage,strlen(baMessage));
	CTOS_LCDTPrint(baMessage);
	CTOS_KBDGet(&key);
	do{
		usBufferLen=1024;
		GPRS_recieve(baRXBuffer,&usBufferLen);
		HTTP_GetReply(baReply,baRXBuffer);
		
		parser(baReply,paramlist,&paramLen);
		CTOS_LCDTPrint(paramlist[0]);
		CTOS_KBDGet(&key2);
		usCmdstatus=execute(paramlist, paramLen);

		sprintf(baExtraParam,"CMD:%s\nResponse:%d",paramlist[0],usCmdstatus);
		
    	HTTP_BuildHeader(baHeader,"/MTMS/PosUp.ashx","GET",baExtraParam);
    	HTTP_BuildMessage(baMessage,baHeader,"");
    	CTOS_LCDTPrint(baMessage);
    	CTOS_KBDGet(&key2);
		GPRS_connect(bIPaddress,80);
		GPRS_send(baMessage,strlen(baMessage));
	}while(usCmdstatus!=0xFF);
	GPRS_disconnect();
	GPRS_close();
	//CTOS_LCDTPrint(baRXBuffer);

}
int main(int argc,char *argv[])
{
	BYTE key;  
	// TODO: Add your program here //
	CTOS_LCDTClearDisplay();
	hello();

	 
	 
	 
	
	//CTOS_LCDTPrint(baReply);	




    CTOS_KBDGet(&key);
	exit(0);
}
