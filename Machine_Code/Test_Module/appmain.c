/** 
**	A Template for developing new terminal application
**/

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
/** These two files are necessary for calling CTOS API **/
#include <ctosapi.h>
#include "files.h"
#include "GPRS.h"
#include "test.h"

/** 
** The main entry of the terminal application 
**/
BYTE bTXBuffer[1024];
BYTE bRXBuffer[1024];
BYTE bIPaddress[]="197.34.106.209";
BYTE baBuffer[1024];
USHORT usBufferLen;
BYTE baHeader[]="GET /MTMS/PosUp.ashx HTTP/1.0\nContent-Type: application/x-www-form-urlencoded\nAccept: */*\n";
void gprs_session(){
	GPRS_PARAM_init();
	CTOS_LCDTPrint("\ninit Done");	

	GPRS_open();
	CTOS_LCDTPrint("\nopen Done");	
}
void send_file(BYTE *baBuffer,USHORT usBufferLen){
	GPRS_connect(bIPaddress,80);
	sprintf(bTXBuffer,"%sContent-length:%d\n\n%s\n\n",baHeader,usBufferLen,baBuffer);

	GPRS_send(bTXBuffer,strlen(bTXBuffer));

}
void send_serial(){
    GPRS_connect(bIPaddress,80);
    
	sprintf(bTXBuffer,"%sSerialNumber:%s\n\n\n\n",baHeader,"9453.343234.455");
    
    GPRS_send(bTXBuffer,strlen(bTXBuffer));
    
}
int main(int argc,char *argv[])
{
	BYTE key;
    BYTE baFileName[30];  

//	SystemTest(baFileName);
	strcpy(baFileName,"test_result.txt");

	usBufferLen=1024;
	FileRead(baFileName,baBuffer,&usBufferLen);
	gprs_session();
	//send_file(baBuffer,usBufferLen);
	send_serial();
	usBufferLen=1024;
	GPRS_recieve(bRXBuffer,&usBufferLen);
    CTOS_LCDTPrint(bRXBuffer);
    CTOS_KBDGet(&key);
	
	exit(0);
}
