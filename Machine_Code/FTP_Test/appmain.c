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


/** 
** The main entry of the terminal application 
**/
int main(int argc,char *argv[])
{
    BYTE baSendBuf[1024],baRecvBuf[1024]; //Send buffer
    USHORT usSendLen,usRecvLen;
    BYTE key;
    
        
	// TODO: Add your program here //
	CTOS_LCDTClearDisplay();
            
    GPRS_open();
    GPRS_connect("197.34.110.199",22);
    //send username to authenticate  
    strcpy(baSendBuf, "open \"M.Asaker@197.34.110.199\" 22" ); //Set sending buffer
    usSendLen = usRecvLen = strlen(baSendBuf);  
    GPRS_send(baSendBuf,usSendLen);
    CTOS_KBDGet(&key);
    usRecvLen = 1024;
    GPRS_recieve(baRecvBuf,&usRecvLen);
    CTOS_LCDTPrint (baRecvBuf);
     CTOS_KBDGet(&key);
    strcpy(baSendBuf, "Trust new Hostkey: Once" ); 
    usSendLen = usRecvLen = strlen(baSendBuf);
    GPRS_send(baSendBuf,usSendLen);
    CTOS_KBDGet(&key);
    
    usRecvLen = 1024;
    GPRS_recieve(baRecvBuf,&usRecvLen);
    CTOS_LCDTPrint (baRecvBuf);
     CTOS_KBDGet(&key);
    strcpy(baSendBuf, "Pass: Asaker" ); 
    usSendLen = usRecvLen = strlen(baSendBuf);
    GPRS_send(baSendBuf,usSendLen);
    CTOS_KBDGet(&key);
    
    usRecvLen = 1024;
    GPRS_recieve(baRecvBuf,&usRecvLen);
    CTOS_LCDTPrint (baRecvBuf);
     CTOS_KBDGet(&key);
      
    GPRS_close();
	
	exit(0);
}
