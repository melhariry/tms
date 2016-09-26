/** 
**	A Template for developing new terminal application
**/

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
/** These two files are necessary for calling CTOS API **/
#include <ctosapi.h>
#include <curl/curl.h>
#include <sys/stat.h>
#include "manager.h"
#include <fcntl.h>
/** 
** The main entry of the terminal application 
**/
#include "curl/curl.h"
BYTE baStr[10000];    
int main(int argc,char *argv[])
{
	BYTE key;
	USHORT usRet;
	
	DWORD event;
	USHORT noFile;
	CTOS_FILE_ATTRIB files[200];
	//while(1)
	{
		CTOS_LCDTClearDisplay();

		//noFile=lsl("/home/ap/pub",files,0);
		

	  	manUp();
	    //CTOS_APFork("TransApp");
	    //sleep(7);

	    //fork 
	    //sleep
		//CTOS_KBDGet(&key);
	}
	
	
	exit(0);
}
