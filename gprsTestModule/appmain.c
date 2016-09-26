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
/** 
** The main entry of the terminal application 
**/
BYTE baIP[]="197.34.97.36";
BYTE baRxBuff[1024];
USHORT usRxBuffLen;
int main(int argc,char *argv[])
{
	BYTE key;
	// TODO: Add your program here //
	CTOS_LCDTClearDisplay();

    gprsOpen();
	gprsConnect(baIP,80);
	gprsSend("GET /MTMS/PosUp.ashx HTTP/1.1\nContent-Type: application/x-www-form-urlencoded\nAccept: */*\nAccept-Encoding: gzip, deflate\nAccept-Language: en-US,en;q=0.8,ar;q=0.6,es;q=0.4\n\n",strlen("GET /MTMS/PosUp.ashx HTTP/1.1\nContent-Type: application/x-www-form-urlencoded\nAccept: */*\nAccept-Encoding: gzip, deflate\nAccept-Language: en-US,en;q=0.8,ar;q=0.6,es;q=0.4\n\n"));
	usRxBuffLen=1024;
	gprsRecieve(baRxBuff,&usRxBuffLen);
	CTOS_LCDTPrint(baRxBuff);
	CTOS_KBDGet(&key);
	gprsDisconnect();
	gprsClose();        
    
            
    CTOS_KBDGet(&key);
	
	exit(0);
}
