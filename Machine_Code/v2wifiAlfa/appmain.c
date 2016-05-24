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
#include "execute.h"

#include <unistd.h>
#include <sys/types.h>
#include <sys/dir.h>
#include <sys/param.h>
#include <stdio.h>
/** 
** The main entry of the terminal application 
**/
#include "curl/curl.h"
#include "wifi.h"
BYTE baStr[10000];
BYTE baFileList[1000];
void system_start()
{
	//build FileSystem
	//mkdir("/home/ap/pub/MTMS",S_IRUSE);
	//load configurations

}
BYTE pathname[200];
BYTE baBuffer[100];
extern  int alphasort();
int file_selectX(struct direct   *entry)
 
		{if ((strcmp(entry->d_name, ".") == 0) ||
						(strcmp(entry->d_name, "..") == 0))
						 return (FALSE);
				else
								return (TRUE);
		}
void lsx(){
    CTOS_LCDTClearDisplay();
	 int count,i;
	 BYTE key;
				struct direct **files;
				int file_selectX();
 
	if (getwd(pathname) == NULL )
	{ //printf("Error getting path\n");
	    CTOS_LCDTPrint("Error getting path\n");
		//exit(0);
	}
	sprintf(baBuffer,"cwd = %s\n",pathname);
	CTOS_LCDTPrint(baBuffer);

    
	count = scandir(pathname, &files, file_selectX, alphasort);

	/* If no files found, make a non-selectable menu item */
	if(count <= 0)
	{		 
		sprintf(baBuffer,"nothing\n");
		//exit(0);
	}
	sprintf(baBuffer,"N files= %d\n",count);
	for (i=1;i<count+1;++i){
		sprintf(baBuffer,"%s\n",files[i-1]->d_name);
		CTOS_LCDTPrint(baBuffer);
	}
	CTOS_KBDGet(&key);		 
	
	
}
int main(int argc,char *argv[])
{
	BYTE key;
	USHORT usRet;
	
	DWORD event;
	USHORT noFile;
	CTOS_FILE_ATTRIB files[200];

	//system_start();
	//while(1)
	{
		/*CTOS_LCDTClearDisplay();
		chdir("/home/ap/pub");
    	lsx();
    	CTOS_KBDGet(&key);
    	chdir("/home/ap/pub/MTMS");
    	lsx();
    	CTOS_KBDGet(&key);
    	chdir("/home/ap/pub/la2aDir");
    	lsx();*/
		//noFile=lsl("/home/ap/pub",files,0);
		
		//exeListFiles(baFileList);
    	system("ifconfig wlan0 up >/home/ap/pub/stdout.txt");
    	system("iwlist wlan0 scan >/home/ap/pub/stdout.txt");
    	//wifiConnectSSID("wired","wifimachine");
	  	//manUp();
	    //CTOS_APFork("TransApp");
	    //sleep(7);

	    //fork 
	    //sleep
		CTOS_KBDGet(&key);
	}
	
	
	exit(0);
}
