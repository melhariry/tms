/**
	******************************************************************************
	* file          communication.c
	* author        
	* component     communication
	* description   implementation of communication module
	*               communication module provides the Terminal abstraction layer of 
	*               communication with TMS 
	******************************************************************************
	* copyright POS_GP_TEAM_2016
	*
	******************************************************************************
	*/
/*========================================*
* I N C L U D E S *
*========================================*/
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctosapi.h>

/*========================================*
* I N C L U D E S *
*========================================*/
#include "manager.h"
#include <signal.h>
/*==========================================*
****************P R I V A T E ***************
*==========================================*/

/*==========================================*
* D E F I N E S *
*==========================================*/
#define DELETE_PARAMS 0
#define PUSH_PARAMS 1
#define PULL_PARAMS 2
#define APP_PARAMS 3
#define UPDATE_PARAMS 4


#define CMD_CREATE_POS_RECORD 0x80
//information
#define CMD_TEST_POS          0x01
#define CMD_LIST_FILES        0x02
#define CMD_LIST_APPS         0x04
//file operations
#define CMD_DELETE_FILES      0x08
#define CMD_PUSH_FILES        0x10
#define CMD_PULL_FILES        0x20
//Update Apps
#define CMD_UPDATE_APPS		  0x40





/*==========================================*
* C O N S T A N T S *
*==========================================*/

/*==========================================*
* T Y P E S *
*==========================================*/

/*==========================================*
* V A R I A B L E S *
*==========================================*/
BYTE baResponse[3*1024];
BYTE baStr[3*1024];
/*==========================================*
* M A C R O S *
*==========================================*/
#define TESTBIT(VAR,FLAG) ( (VAR&FLAG) ?TRUE:FALSE)
/*==========================================*
* F U N C T I O N S *
*==========================================*/
typedef struct 
{
  /* data */
  char *fileName;
  //int fileNameSize;
  int type;
}fileType;

void parser(void *list,OUT USHORT * uslistLen, IN BYTE* baRaw,USHORT usMethod)
{//u should keep the raw data if not u need to strcpy and in pull u have to reallocate
  BYTE* token;
  BYTE** bplist;
  fileType* fpList;
  switch(usMethod)
  {
  	case UPDATE_PARAMS:
    case PUSH_PARAMS:
    case PULL_PARAMS:
    case DELETE_PARAMS:
      bplist=(BYTE**)list;
      token=strtok(baRaw,";");
      while(token)
      {
        //strcpy((BYTE*)list[(*uslistLen)++],token);
        bplist[(*uslistLen)++]=token;
        token=strtok(NULL,";");
      }
      break;
    case 9:

      fpList=(fileType*)list;
      token=strtok(baRaw,",;");
      while(token)
      {
        fpList[*uslistLen].fileName=token;
        token=strtok(NULL,",;");
        if(token==NULL)break;
        
        sscanf(token,"%d",&fpList[*uslistLen].type);
        printf("%d\n",fpList[*uslistLen].type);
        (*uslistLen)++;
        
        token=strtok(NULL,",;");

      }
      break;
    

  }

}
void parser2(OUT BYTE* list[],OUT USHORT * uslistLen, IN BYTE* baRaw,USHORT usMethod)
{
	BYTE* token;
	switch(usMethod)
	{
		case PUSH_PARAMS:
		case PULL_PARAMS:
		case DELETE_PARAMS:
			token=strtok(baRaw,";");
			while(token)
			{
				//strcpy((BYTE*)list[(*uslistLen)++],token);  //prefered cuz token is not newly allocated
				list[(*uslistLen)++]=token;
				token=strtok(NULL,";");
			}
			break;

		

	}
}

int getParamsCount(IN BYTE * baRaw)
{
	int i,count=0;
	for(i=0;i<strlen(baRaw);++i)
	{
			if( baRaw[i] == ';' )
			{
				count++;
			}
	}

	return count;
}

USHORT manUp()
{
	USHORT usCommand,usret,usResult;
	

	commInit();

	strcpy(baResponse,"DEMO");
	usret=commSendTms(baResponse,PATH_POSUP);
	gprsSend("POST /MTMS/PosUp.ashx HTTP/1.1\r\n\r\n",strlen("POST /MTMS/PosUp.ashx HTTP/1.1\r\n\r\n"));
	//if(usret!=0)return usret;
	sscanf(baResponse,"%d",&usCommand);
	CTOS_LCDTClearDisplay();

	CTOS_LCDTPrint(baResponse);


	if(TESTBIT(usCommand,CMD_CREATE_POS_RECORD))
	{
		CTOS_LCDTPrint("\nCreatePosRecord");
		manCreatePosRecord();

	}
	if(TESTBIT(usCommand,CMD_DELETE_FILES))
	{
		CTOS_LCDTPrint("\nDeleteFiles");
		if(!manDeleteFile())usResult|=CMD_DELETE_FILES;
	}
	if(TESTBIT(usCommand,CMD_UPDATE_APPS))
	{
		CTOS_LCDTPrint("\nUpdateApp");
		if(!manUpdateApp())usResult|=CMD_UPDATE_APPS;
		
	}
	if(TESTBIT(usCommand,CMD_PUSH_FILES))
	{
		CTOS_LCDTPrint("\nPushFiles");
		if(!manPushFile())usResult|=CMD_PUSH_FILES;
	}
	if(TESTBIT(usCommand,CMD_PULL_FILES))
	{
		CTOS_LCDTPrint("\nPullFiles");
		if(!manPullFile())usResult|=CMD_PULL_FILES;
	}
	
	if(TESTBIT(usCommand,CMD_TEST_POS))
	{
		CTOS_LCDTPrint("\nTEST");
		if(!manTest())usResult|=CMD_TEST_POS;
	}
	if(TESTBIT(usCommand,CMD_LIST_FILES))
	{
		CTOS_LCDTPrint("\nListFiles");
		if(!manFileList())usResult|=CMD_LIST_FILES;
	}
	if(TESTBIT(usCommand,CMD_LIST_APPS))
	{
		CTOS_LCDTPrint("\nListApps");
		if(!manAppList())usResult|=CMD_LIST_APPS;
	}
	
	//send res

	//commClose();
	return usResult;

}
USHORT manCreatePosRecord()
{
	USHORT ret;
    ULONG  ulUsedDiskSize,  ulTotalDiskSize, ulUsedRamSize, ulTotalRamSize;
    ret = CTOS_SystemMemoryStatus (&ulUsedDiskSize ,&ulTotalDiskSize,&ulUsedRamSize,&ulTotalRamSize );
    sprintf(baResponse,"Vendor=Castles&Model=Vega3000&TotalDiskCapacity=%d&TotalRamSize=%d",ulTotalDiskSize,ulTotalRamSize);
	ret=commSendTms(baResponse,PATH_CREATE_POS_RECORD);
	return 0x00;
}
USHORT manTest()
{
	USHORT usResult = exeSystemTest(baStr);
	sprintf(baResponse,"Command=test%s\n",baStr);
	usResult=commSendTms(baResponse,PATH_SUBMIT_COMMAND_RESULT);
	return 0x00;
}

USHORT manAppList()
{
	BYTE key;
	//max 50 apps xD
	USHORT usResult = exeListApps(baStr);
	sprintf(baResponse,"Command=listApps%s",baStr);
	usResult=commSendTms(baResponse,PATH_SUBMIT_COMMAND_RESULT);
	return 0x00;
}

USHORT manFileList()
{
	 BYTE key;
	USHORT usResult = exeListFiles(baStr);
	sprintf(baResponse,"Command=listFiles%s",baStr);
	usResult=commSendTms(baResponse,PATH_SUBMIT_COMMAND_RESULT);
	return 0x00; 
}
USHORT manDeleteFile()
{
	BYTE key;
	USHORT usResult;
	int paramsCount;
	BYTE* *paramsList;
	BYTE *baDelResp;  
	//USHORT deleteResults[100];


	//request parameters
	sprintf(baResponse,"Command=DeleteFile");
	usResult=commSendTms(baResponse,PATH_REQUEST_COMMAND_PARAMETERS);
	
	CTOS_LCDTPrint("\nafter request");
	CTOS_LCDTPrint("\n");
	CTOS_LCDTPrint(baResponse);
	CTOS_KBDGet(&key);
	
	paramsCount=getParamsCount(baResponse);
	
	sprintf(baStr,"\ncount:%d",paramsCount);
	CTOS_LCDTPrint(baStr);
	//CTOS_KBDGet(&key);
	
	if(paramsCount==0)return 0xFF;
	paramsList=(BYTE **)malloc(sizeof(BYTE*)*(paramsCount+5));
	baDelResp=(BYTE *)malloc(sizeof(BYTE)*(paramsCount+3)*100);//100= |&name=<filename>&state=<errorcode>|??
	if(paramsList==NULL||baDelResp==NULL)
	{
		//not enough memory
		return 0xFE;
	}
	//allocate delete result also



	//parse 
	paramsCount=0;
	parser(paramsList , &paramsCount , baResponse , DELETE_PARAMS); 
	
	//CTOS_LCDTPrint("\nafter parse");
	//CTOS_KBDGet(&key);
	

	//execute
	//usResult = exeDeleteFiles(paramsList,paramsCount,deleteResults);
	memset(baDelResp,0,sizeof(baDelResp));
	usResult = exeDeleteFiles(paramsList,paramsCount,baDelResp);
	
	/*if(usResult!=0)
		{
			CTOS_LCDTPrint("\nafter exefailed");
		}
	else
	CTOS_LCDTPrint("\nafter exe");
	*/
	//CTOS_KBDGet(&key);
	//send response

	sprintf(baResponse,"Command=DeleteFile%s",baDelResp);
	usResult=commSendTms(baResponse,PATH_SUBMIT_COMMAND_RESULT); 
	

	free(paramsList);
	free(baDelResp);
	CTOS_LCDTPrint("\nafter free");
	CTOS_KBDGet(&key);
	return 0x00;
}
USHORT manPushFile()
{
	BYTE key;
	USHORT usResult,i;
	int paramsCount;
	BYTE* *paramsList;
	BYTE *baPushResp;  


	//request parameters
	sprintf(baResponse,"Command=PushFile");
	usResult=commSendTms(baResponse,PATH_REQUEST_COMMAND_PARAMETERS);
	
	CTOS_LCDTPrint("\nafter request");
	CTOS_LCDTPrint("\n");
	CTOS_LCDTPrint(baResponse);
	CTOS_KBDGet(&key);
	
	paramsCount=getParamsCount(baResponse);
	
	sprintf(baStr,"\ncount:%d",paramsCount);
	CTOS_LCDTPrint(baStr);
	//CTOS_KBDGet(&key);
	
	if(paramsCount==0)return 0xFF;
	paramsList=(BYTE **)malloc(sizeof(BYTE*)*(paramsCount+5));
	baPushResp=(BYTE *)malloc(sizeof(BYTE)*(paramsCount+3)*100);//100= |&name=<filename>&state=<errorcode>|??
	if(paramsList==NULL)
	{
		//not enough memory
		return 0xFE;
	}
	//allocate delete result also



	//parse 
	paramsCount=0;
	parser(paramsList , &paramsCount , baResponse , PUSH_PARAMS); 
	
	//CTOS_LCDTPrint("\nafter parse");
	//CTOS_KBDGet(&key);
	
	
	//execute
	memset(baPushResp,0,sizeof baPushResp);
	for (i = 0; i < paramsCount; ++i)
	{
		CTOS_LCDTClearDisplay();
		BYTE baPath[30];
		BYTE baFile[100];
		
		CTOS_LCDTPrint(paramsList[i]);
		sprintf
		//check public / private
		if(strncmp(paramsList[i],"pub",3)==0)
		{//public
			sprintf(baPath,"/home/ap/pub/%s",paramsList[i]+4);
			if(access(baPath,F_OK)==-1)
				usResult=commSendFile("/home/ap/pub",paramsList[i]+4);
			else
				usResult=0xFFFF;
		}
		else
		{
			sprintf(baPath,"%s",paramsList[i]+4);
			if(access(baPath,F_OK)==-1)
				usResult=commSendFile(".",paramsList[i]+4);
			else
				usResult=0xFFFF;
		}



		/*
		if(access(paramsList[i],F_OK)!=-1)
		{
			CTOS_LCDTPrint("\nPrivate");
			CTOS_KBDGet(&key);
			usResult=commSendFile(".",paramsList[i]);
		}
		else
		if (access(pubPath,F_OK)!=-1)
		{
			CTOS_LCDTPrint("\nPublic");
			CTOS_KBDGet(&key);
			usResult=commSendFile("/home/ap/pub",paramsList[i]);
		}
		else
		{
			CTOS_LCDTPrint("\nNot Found");
			usResult=0xFFFF;
		}*/
		sprintf(baFile,"&name=%s&state=%d",paramsList[i],usResult);
		strcat(baPushResp,baFile);

		CTOS_KBDGet(&key);
	}
	
	
	
	
	//send response

	sprintf(baResponse,"Command=PushFile%s",baPushResp);
	usResult=commSendTms(baResponse,PATH_SUBMIT_COMMAND_RESULT); 
	

	free(paramsList);
	free(baPushResp);
	CTOS_LCDTPrint("\nafter free");
	CTOS_KBDGet(&key);
	return 0x00;
}


USHORT manPullFile()
{
	BYTE key;
	USHORT usResult,i;
	int paramsCount;
	BYTE* *paramsList;
	BYTE *baPullResp;  
	//request parameters
	sprintf(baResponse,"Command=PullFile");
	usResult=commSendTms(baResponse,PATH_REQUEST_COMMAND_PARAMETERS);
	
	CTOS_LCDTPrint("\nafter request");
	CTOS_LCDTPrint("\n");
	CTOS_LCDTPrint(baResponse);
	CTOS_KBDGet(&key);
	
	paramsCount=getParamsCount(baResponse);
	
	sprintf(baStr,"\ncount:%d",paramsCount);
	CTOS_LCDTPrint(baStr);
	//CTOS_KBDGet(&key);
	
	if(paramsCount==0)return 0xFF;
	paramsList=(BYTE **)malloc(sizeof(BYTE*)*(paramsCount+5));
	//paramsList=(fileType*)malloc(sizeof(fileType)*(paramsCount+2));
	baPullResp=(BYTE *)malloc(sizeof(BYTE)*(paramsCount+3)*100);//100= |&name=<filename>&state=<errorcode>|??
	if(paramsList==NULL||baPullResp==NULL)
	{
		//not enough memory
		return 0xFE;
	}
	//allocate delete result also



	//parse 
	paramsCount=0;
	parser(paramsList , &paramsCount , baResponse , PULL_PARAMS); 
	
	CTOS_LCDTPrint("\nafter parse");
	CTOS_KBDGet(&key);
	
	
	//execute
	memset(baPullResp,0,sizeof baPullResp);
	for (i = 0; i < paramsCount; ++i)
	{
		CTOS_LCDTClearDisplay();
		BYTE baPath[30];
		BYTE baFile[100];
		
		USHORT isPublic;
		if(strncmp(paramsList[i],"pub",3)==0)
		{//public
			
			
			usResult=commRecieveFile("/home/ap/pub",paramsList[i]+4);
			
			
		}
		else
		{
			
			
			usResult=commRecieveFile(".",paramsList[i]);
			
		}

		sprintf(baFile,"&name=%s&state=%d",paramsList[i],usResult);
		
		CTOS_LCDTPrint(baFile);
		strcat(baPullResp,baFile);

		CTOS_KBDGet(&key);
	}
	
	
	
	
	//send response

	sprintf(baResponse,"Command=PullFile%s",baPullResp);
	usResult=commSendTms(baResponse,PATH_SUBMIT_COMMAND_RESULT); 
	

	free(paramsList);
	free(baPullResp);
	CTOS_LCDTPrint("\nafter free");
	CTOS_KBDGet(&key);
	return 0x00;
}
USHORT manUpdateApp()
{
	BYTE key;
	pid_t pid;
	USHORT usResult,i;
	int paramsCount;
	BYTE* *paramsList;
	BYTE baUpdateResp[30];
	//FILE *MCI=fopen("/home/ap/pub/update.mci","wb");
	//FILE *MMCI=fopen("/home/ap/pub/update.mmci","wb");
	FILE *MCI=fopen("update.mci","wb");
	FILE *MMCI=fopen("update.mmci","wb");
	BYTE pathname[200];


	sprintf(baResponse,"Command=UpdateApp");
	usResult=commSendTms(baResponse,PATH_REQUEST_COMMAND_PARAMETERS);

	CTOS_LCDTPrint("\nafter request");
	CTOS_LCDTPrint("\n");
	CTOS_LCDTPrint(baResponse);
	CTOS_KBDGet(&key);
	
	paramsCount=getParamsCount(baResponse);
	
	sprintf(baStr,"\ncount:%d",paramsCount);
	CTOS_LCDTPrint(baStr);
	
	
	if(paramsCount==0)return 0xFF;
	paramsList=(BYTE **)malloc(sizeof(BYTE*)*(paramsCount+5));
	
	if(paramsList==NULL||baUpdateResp==NULL)
	{
		//not enough memory
		return 0xFE;
	}

	//parse 
	paramsCount=0;
	parser(paramsList , &paramsCount , baResponse , UPDATE_PARAMS); 



	//download apps
	for (i = 0; i < paramsCount; ++i)
	{
		//usResult=commRecieveFile("/home/ap/pub/",paramsList[i]);
		usResult=commRecieveFile(".",paramsList[i]);
		fprintf(MCI, "%s\n", paramsList[i]);
	}
	if(getwd(pathname)==NULL)
	{
		//I could get the path of the current folder so I cant continue to UPDATE (needs abs path)
		// Work around L put MCI file into /home/ap/pub ;) : probably getwd will succeed
		return 0xFF;
	}
	
	fprintf(MMCI, "update.mci");
	fclose(MMCI);
	fclose(MCI);
	/*DeleteFile("app.mmci");
	ULONG ulHandle;
	    NewFile("app.mmci", &ulHandle,0,d_FA_PUBLIC);
    FileWrite("app.mci","app.mmci");*/
	strcat(pathname,"/update.mmci"); //m3ana el path xD
	CTOS_LCDTPrint("\n");
	CTOS_LCDTPrint(pathname);
	CTOS_KBDGet(&key);



	pid=fork();
	if(pid==0)
	{
		CTOS_UpdateFromMMCI(pathname,1);//w rabina yostor xD feha khabth xD
		kill(pid, SIGTERM);
	}
	sleep(4);
	commInit();


	//CTOS_UpdateFromMMCI("/home/ap/pub/update.mmci",1);//w rabina yostor xD feha khabth xD
	usResult=CTOS_UpdateGetResult();
	
	sprintf(baUpdateResp,"&status=%04x",usResult);


	//delete apps srcs

	//send response
	
	sprintf(baResponse,"Command=UpdateApp%s",baUpdateResp);
	usResult=commSendTms(baResponse,PATH_SUBMIT_COMMAND_RESULT); 

	free(paramsList);
	CTOS_LCDTPrint("\nafter free");
	CTOS_KBDGet(&key);
	return 0x00;
}



//e7tyaty
/*
USHORT manPullFilex()
{
	BYTE key;
	USHORT usResult,i;
	int paramsCount;
	fileType *paramsList;

	BYTE *baPullResp;  //??
	//USHORT deleteResults[100];


	//request parameters
	sprintf(baResponse,"Command=PullFile");
	usResult=commSendTms(baResponse,PATH_REQUEST_COMMAND_PARAMETERS);
	
	CTOS_LCDTPrint("\nafter request");
	CTOS_LCDTPrint("\n");
	CTOS_LCDTPrint(baResponse);
	CTOS_KBDGet(&key);
	
	paramsCount=getParamsCount(baResponse);
	
	sprintf(baStr,"\ncount:%d",paramsCount);
	CTOS_LCDTPrint(baStr);
	//CTOS_KBDGet(&key);
	
	if(paramsCount==0)return 0xFF;
	//paramsList=(BYTE **)malloc(sizeof(BYTE*)*(paramsCount+5));
	paramsList=(fileType*)malloc(sizeof(fileType)*(paramsCount+2));
	baPullResp=(BYTE *)malloc(sizeof(BYTE)*(paramsCount+3)*100);//100= |&name=<filename>&state=<errorcode>|??
	if(paramsList==NULL||baPullResp==NULL)
	{
		//not enough memory
		return 0xFE;
	}
	//allocate delete result also



	//parse 
	paramsCount=0;
	parser(paramsList , &paramsCount , baResponse , PULL_PARAMS); 
	
	CTOS_LCDTPrint("\nafter parse");
	CTOS_KBDGet(&key);
	
	
	//execute
	memset(baPullResp,0,sizeof baPullResp);
	for (i = 0; i < paramsCount; ++i)
	{
		CTOS_LCDTClearDisplay();
		BYTE pubPath[30];
		BYTE baFile[100];
		
		USHORT isPublic;
		//sscanf(paramsList[i],"%[^,]*,%d",baFile,&isPublic);
		
		

		//sprintf(pubPath,"/home/ap/pub/%s",paramsList[i]);
		if(!paramsList[i].type)
		{
			CTOS_LCDTPrint("\nPrivate");
			CTOS_KBDGet(&key);
			usResult=commRecieveFile(".",paramsList[i].fileName);
			//usResult=commSendFile(".",paramsList[i]);

		}
		else
		//if (isPublic)
		{
			CTOS_LCDTPrint("\nPublic");
			CTOS_KBDGet(&key);
			usResult=commRecieveFile("/home/ap/pub",paramsList[i].fileName);
			//usResult=commSendFile("/home/ap/pub",paramsList[i]);
		}
		sprintf(baFile,"&name=%s&state=%d",paramsList[i],usResult);
		
		CTOS_LCDTPrint(baFile);
		strcat(baPullResp,baFile);

		CTOS_KBDGet(&key);
	}
	
	
	
	
	//send response

	sprintf(baResponse,"Command=PullFile%s",baPullResp);
	usResult=commSendTms(baResponse,PATH_SUBMIT_COMMAND_RESULT); 
	

	free(paramsList);
	free(baPullResp);
	CTOS_LCDTPrint("\nafter free");
	CTOS_KBDGet(&key);
	return 0x00;
}*/