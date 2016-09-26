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


#define CMD_CREATE_POS_RECORD 0xFF
//information
#define CMD_TEST_POS          0x01
#define CMD_LIST_FILES        0x02
#define CMD_LIST_APPS         0x04
//file operations
#define CMD_DELETE_FILES      0x08
#define CMD_PUSH_FILES        0x10
#define CMD_PULL_FILES        0x20
//Update Apps





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
    case PUSH_PARAMS:
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
    case PULL_PARAMS:

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

	strcpy(baResponse,"taban");
	usret=commSendTms(baResponse,PATH_POSUP);
	//if(usret!=0)return usret;
	sscanf(baResponse,"%d",&usCommand);
	CTOS_LCDTClearDisplay();

	CTOS_LCDTPrint(baResponse);


	if(TESTBIT(usCommand,CMD_CREATE_POS_RECORD))
	{
		CTOS_LCDTPrint("\nCreatePosRecord");

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
	if(TESTBIT(usCommand,CMD_DELETE_FILES))
	{
		CTOS_LCDTPrint("\nDeleteFiles");
		if(!manDeleteFile())usResult|=CMD_DELETE_FILES;
	}
	if(TESTBIT(usCommand,CMD_PUSH_FILES))
	{
		CTOS_LCDTPrint("\nPushFiles");
		if(!manPushFile())usResult|=CMD_PUSH_FILES;
	}
	if(TESTBIT(usCommand,CMD_PULL_FILES))
	{
		CTOS_LCDTPrint("\nPullFiles");
		//if(!manPullFile())usResult|=CMD_PULL_FILES;
		commRecieveFile("/home/ap/pub","Server1.txt");
	}
	
	//send res

	//commClose();
	return usResult;

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
	sprintf(baResponse,"Command=listApp%s",baStr);
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
	BYTE *baPushResp;  //??
	//USHORT deleteResults[100];


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
		BYTE pubPath[30];
		BYTE baFile[100];
		
		CTOS_LCDTPrint(paramsList[i]);
		sprintf(pubPath,"/home/ap/pub/%s",paramsList[i]);
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
		}
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
}