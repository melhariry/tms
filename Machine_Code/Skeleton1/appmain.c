/** 
**	A Template for developing new terminal application
**/

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
/** These two files are necessary for calling CTOS API **/
#include <ctosapi.h>

#include "manager.h"
/** 
** The main entry of the terminal application 
**/
int main(int argc,char *argv[])
{
	BYTE key;
    USHORT usRet;    
	// TODO: Add your program here //
	CTOS_LCDTClearDisplay();
            
        usRet=manTest();
        if(usRet){
        	CTOS_LCDTPrint("not ok");
        }
        else{
        	CTOS_LCDTPrint("ok");
        }
            
        CTOS_KBDGet(&key);
	
	exit(0);
}
