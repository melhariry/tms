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
#include <errno.h>
/** 
** The main entry of the terminal application 
**/
#include "curl/curl.h"

BYTE baStr[10000];
BYTE baFileList[1000];

BYTE pathname[200];
BYTE baBuffer[100];
extern  int alphasort();
int file_selectX(struct direct *entry);
int file_selectX(struct direct *entry)
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
				//int file_selectX(struct direct   *);
 
	/*if (getwd(pathname) == NULL )
	{ //printf("Error getting path\n");
	    CTOS_LCDTPrint("Error getting path\n");
		//exit(0);
	}*/
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


void loadConfig(void);
void loadConfig()
{
	//read DefaultConfig
}
int main(int argc,char *argv[])
{
	BYTE key;
	USHORT usRet;
	timer=1;
	//system_start();
	//getwd(pathname);
	//strcpy(pathname,"/home/ap/pub/MTMS");
	//lsx();
	//CTOS_KBDGet(&key);
	/*system("rm /home/ap/pub/MTMS/README.txt");
	if(access("/home/ap/pub/MTMS/README.txt",R_OK|W_OK|X_OK)==-1)
	{
		CTOS_LCDTPrint("\n");
		CTOS_LCDTPrint(strerror(errno));
	}
	else
	{
		chmod("/home/ap/pub/MTMS/README.txt",0777);

		if(remove("/home/ap/pub/MTMS/README.txt")!=-1)
		{
			CTOS_LCDTPrint("\n");
		CTOS_LCDTPrint(strerror(errno));
		}

		CTOS_LCDTPrint("\nDeleted?\n");
	}*/
		system("chmod 0777 /home/ap/pub/MTMS/* >/home/ap/pub/MTMS/chmod.txt");
	system("ls -lR /home/ap/pub >/home/ap/pub/MTMS/ls.txt");
	//return 0;
	//while(1)
	{
	  	manUp();

	    //sleep(timer);
		CTOS_KBDGet(&key);
	}
	
	
	exit(0);
}
