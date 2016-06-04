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
BYTE manbuff[3*1024];
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
    case DELETE_PARAMS:
    case PULL_PARAMS:
      bplist=(BYTE**)list;
      token=strtok(baRaw,";");
      while(token)
      {
        //strcpy((BYTE*)list[(*uslistLen)++],token);
        bplist[(*uslistLen)++]=token;
        token=strtok(NULL,";");
      }
      break;
    case 99:

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
	sysLogCall("manUp");
	USHORT usCommand,usret,usResult;
	FILE *Config;
	int i;
	//char manbuff[100];
	char *token;
	char pArr[5][50];
	BYTE iscorrupted=0;
	uint16_t u16hostport;
	/*8if(access("TMSConfig.csv",R_OK)!=-1)
	{
		Config=fopen("TMSConfig.csv","rb");
		i=0;
		while(fgets(pArr[i],sizeof pArr[i],Config)){
			pArr[i][strlen(pArr[i])-1]='\0';
			++i;
		}

		
		//commConfig(pArr[0],pArr[2],pArr[3]);
		//CTOS_LCDTPrint(pArr[0]);
		//CTOS_LCDTPrint("\n");
		//CTOS_LCDTPrint(pArr[2]);
		//CTOS_LCDTPrint("\n");
		//CTOS_LCDTPrint(pArr[3]);
		//CTOS_LCDTPrint("\n");
       
		sscanf(pArr[4],"%d",&timer);
		
		fclose(Config);
        //CTOS_KBDGet(&i);
	}
	*/
	commInit();
	strcpy(baResponse,"DEMO");
	usret=commSendTms(baResponse,PATH_POSUP);
	//if(usret!=0)return usret;
	if(!strlen(baResponse))
	{
		//CTOS_LCDTPrint("NO RESPONSE");
		//CTOS_KBDGet(&usret);
		return 0xffff;
	}
	sscanf(baResponse,"%hu",&usCommand);
	//CTOS_LCDTClearDisplay();

	//CTOS_LCDTPrint(baResponse);
	//CTOS_KBDGet(&usret);

	if(TESTBIT(usCommand,CMD_CREATE_POS_RECORD))
	{
		//CTOS_LCDTPrint("\nCreatePosRecord");
		manCreatePosRecord();

	}
	if(TESTBIT(usCommand,CMD_DELETE_FILES))
	{
		//CTOS_LCDTPrint("\nDeleteFiles");
		if(!manDeleteFile())usResult|=CMD_DELETE_FILES;
	}
	if(TESTBIT(usCommand,CMD_UPDATE_APPS))
	{
		//CTOS_LCDTPrint("\nUpdateApp");
		if(!manUpdateApp())usResult|=CMD_UPDATE_APPS;
		
	}
	if(TESTBIT(usCommand,CMD_PUSH_FILES))
	{
		//CTOS_LCDTPrint("\nPushFiles");
		if(!manPushFile())usResult|=CMD_PUSH_FILES;
	}
	if(TESTBIT(usCommand,CMD_PULL_FILES))
	{
		//CTOS_LCDTPrint("\nPullFiles");
		if(!manPullFile())usResult|=CMD_PULL_FILES;
	}
	
	if(TESTBIT(usCommand,CMD_TEST_POS))
	{
		//CTOS_LCDTPrint("\nTEST");
		if(!manTest())usResult|=CMD_TEST_POS;
	}
	if(TESTBIT(usCommand,CMD_LIST_FILES))
	{
		//CTOS_LCDTPrint("\nListFiles");
		if(!manFileList())usResult|=CMD_LIST_FILES;
	}
	if(TESTBIT(usCommand,CMD_LIST_APPS))
	{
		//CTOS_LCDTPrint("\nListApps");
		if(!manAppList())usResult|=CMD_LIST_APPS;
	}

	
	//send res
	//CTOS_LCDTPrint("\nFinish");
	strcpy(baResponse,"Command=Finish");
	usret=commSendTms(baResponse,PATH_SUBMIT_COMMAND_RESULT);
	
	//commClose();
	sysLogRet("manUp",usResult);
	return usResult;

}
USHORT manCreatePosRecord()
{
	sysLogCall("manCreatePosRecord");
	USHORT ret;
    ULONG  ulUsedDiskSize,  ulTotalDiskSize, ulUsedRamSize, ulTotalRamSize;
    ret = CTOS_SystemMemoryStatus (&ulUsedDiskSize ,&ulTotalDiskSize,&ulUsedRamSize,&ulTotalRamSize );
    sprintf(baResponse,"Vendor=Castles&Model=Vega3000&TotalDiskCapacity=%lu&TotalRamSize=%lu",ulTotalDiskSize/1024,ulTotalRamSize/1024);//MB
	
	ret=commSendTms(baResponse,PATH_CREATE_POS_RECORD);

	sysLogRet("manCreatePosRecord",0);
	return 0x00;
}
USHORT manTest()
{
	sysLogCall("manTest");
	USHORT usResult = exeSystemTest(manbuff);
	sprintf(baResponse,"Command=TestHealth%s\n",manbuff);
	usResult=commSendTms(baResponse,PATH_SUBMIT_COMMAND_RESULT);
	sysLogRet("manTest",0);
	return 0x00;
}

USHORT manAppList()
{
	sysLogCall("manAppList");
	BYTE key;
	//max 50 apps xD
	USHORT usResult = exeListApps(manbuff);
	sprintf(baResponse,"Command=ListApps%s",manbuff);
	usResult=commSendTms(baResponse,PATH_SUBMIT_COMMAND_RESULT);
	sysLogRet("manAppList",0);
	return 0x00;
}

USHORT manFileList()
{
	sysLogCall("manFileList");
	BYTE key;
	memset(manbuff,0,sizeof manbuff);
	USHORT usResult = exeListFiles(manbuff);
	sprintf(baResponse,"Command=ListFiles%s",manbuff);
	usResult=commSendTms(baResponse,PATH_SUBMIT_COMMAND_RESULT);
	sysLogRet("manFileList",0);
	return 0x00; 
}
USHORT manDeleteFile()
{
	sysLogCall("manDeleteFile");
	BYTE key;
	USHORT usResult;
	USHORT paramsCount;
	BYTE* *paramsList;
	BYTE *baDelResp;  
	//USHORT deleteResults[100];
	//request parameters
	sprintf(baResponse,"Command=DeleteFile");
	usResult=commSendTms(baResponse,PATH_REQUEST_COMMAND_PARAMETERS);
	
	//CTOS_LCDTPrint("\nafter request");
	//CTOS_LCDTPrint("\n");
	//CTOS_LCDTPrint(baResponse);
	//CTOS_KBDGet(&key);
	
	paramsCount=getParamsCount(baResponse);
	
	sprintf(manbuff,"\ncount:%d",paramsCount);
	//CTOS_LCDTPrint(manbuff);
	////CTOS_KBDGet(&key);
	
	if(paramsCount==0)return 0xFF;
	paramsList=(BYTE **)malloc(sizeof(BYTE*)*(paramsCount+5));
	baDelResp=(BYTE *)malloc(sizeof(BYTE)*(paramsCount+3)*300);//100= |&name=<filename>&state=<errorcode>|??
	if(paramsList==NULL||baDelResp==NULL)
	{
		//not enough memory
		return 0xFE;
	}
	//allocate delete result also



	//parse 
	paramsCount=0;
	parser(paramsList , &paramsCount , baResponse , DELETE_PARAMS); 
	
	////CTOS_LCDTPrint("\nafter parse");
	////CTOS_KBDGet(&key);
	

	//execute
	//usResult = exeDeleteFiles(paramsList,paramsCount,deleteResults);
	memset(baDelResp,0,sizeof(baDelResp));

	usResult = exeDeleteFiles(paramsList,paramsCount,baDelResp);
	
	/*if(usResult!=0)
		{
			//CTOS_LCDTPrint("\nafter exefailed");
		}
	else
	//CTOS_LCDTPrint("\nafter exe");
	*/
	////CTOS_KBDGet(&key);
	//send response

	sprintf(baResponse,"Command=DeleteFile%s",baDelResp);
	usResult=commSendTms(baResponse,PATH_SUBMIT_COMMAND_RESULT); 
	

	free(paramsList);
	free(baDelResp);
	//CTOS_LCDTPrint("\nafter free");
	//CTOS_KBDGet(&key);
	sysLogRet("manDeleteFile",0);
	return 0x00;
}
USHORT manPushFile()
{
	sysLogCall("manPushFile");
	BYTE key;
	USHORT usResult,i;
	USHORT paramsCount;
	BYTE* *paramsList;
	BYTE *baPushResp;  
	BYTE baPath[200];
	BYTE baFile[300];

	//request parameters
	sprintf(baResponse,"Command=PushFile");
	usResult=commSendTms(baResponse,PATH_REQUEST_COMMAND_PARAMETERS);
	
	//CTOS_LCDTPrint("\nafter request");
	//CTOS_LCDTPrint("\n");
	//CTOS_LCDTPrint(baResponse);
	//CTOS_KBDGet(&key);
	
	paramsCount=getParamsCount(baResponse);
	
	sprintf(manbuff,"\ncount:%d",paramsCount);
	//CTOS_LCDTPrint(manbuff);
	////CTOS_KBDGet(&key);
	
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
	
	////CTOS_LCDTPrint("\nafter parse");
	////CTOS_KBDGet(&key);
	
	
	//execute
	memset(baPushResp,0,sizeof baPushResp);
	for (i = 0; i < paramsCount; ++i)
	{
		//CTOS_LCDTClearDisplay();
		
		
		//CTOS_LCDTPrint(paramsList[i]);
		
		//check public / private
		if(strncmp(paramsList[i],"pub",3)==0)
		{//public
			sprintf(baPath,"/home/ap/pub/%s",paramsList[i]+4);
			//CTOS_LCDTPrint("\n");
			//CTOS_LCDTPrint(baPath);
			if(access(baPath,F_OK)!=-1)
			{
				//CTOS_LCDTPrint("\nxfound");
				usResult=commSendFile(baPath,paramsList[i]);
			}
			else
				usResult=0xFFFF;
		}
		else
		{
			//CTOS_LCDTPrint("\nxpri");
			sprintf(baPath,"%s",paramsList[i]+4);
			if(access(baPath,F_OK)!=-1)
			{
				//CTOS_LCDTPrint("\nxfound");
				usResult=commSendFile(baPath,paramsList[i]);
			}
			else

				usResult=0xFFFF;
		}
		sprintf(baFile,"&Path=%s&Status=%d",paramsList[i],usResult==226?0:usResult);
		strcat(baPushResp,baFile);

		//CTOS_KBDGet(&key);
	}
	
	
	
	
	//send response

	sprintf(baResponse,"Command=PushFile%s",baPushResp);
	usResult=commSendTms(baResponse,PATH_SUBMIT_COMMAND_RESULT); 
	

	free(paramsList);
	free(baPushResp);
	//CTOS_LCDTPrint("\nafter free");
	//CTOS_KBDGet(&key);
	sysLogRet("manPushFile",0);
	return 0x00;
}


USHORT manPullFile()
{
	sysLogCall("manPullFile");
	BYTE key;
	USHORT usResult,i;
	USHORT paramsCount;
	BYTE* *paramsList;
	BYTE *baPullResp;  
	BYTE baPath[255];
	BYTE baFile[255];
	//request parameters
	sprintf(baResponse,"Command=PullFile");
	usResult=commSendTms(baResponse,PATH_REQUEST_COMMAND_PARAMETERS);
	sprintf(manbuff,"Event: manPullFile :Requesting Params=%04x\n",usResult);
    sysLog(manbuff);
	
	//CTOS_LCDTPrint("\nafter request");
	//CTOS_LCDTPrint("\n");
	//CTOS_LCDTPrint(baResponse);
	//CTOS_KBDGet(&key);
	
	paramsCount=getParamsCount(baResponse);
	
	sprintf(manbuff,"\ncount:%d",paramsCount);
	//CTOS_LCDTPrint(manbuff);
	////CTOS_KBDGet(&key);
	
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
	
	//CTOS_LCDTPrint("\nafter parse");
	//CTOS_KBDGet(&key);
	
	
	//execute
	memset(baPullResp,0,sizeof baPullResp);
	for (i = 0; i < paramsCount; ++i)
	{
		//CTOS_LCDTClearDisplay();
		
		
		USHORT isPublic;
		if(strncmp(paramsList[i],"pub/",4)==0)
		{//public
			sprintf(baPath,"/home/ap/pub/%s",paramsList[i]+4);
			//CTOS_LCDTPrint("\n7lw\n");
			usResult=commRecieveFile(paramsList[i],baPath);
			
			
		}
		else
		if(strncmp(paramsList[i],"pri/",4)==0)
		{
			
			sprintf(baPath,"./%s",paramsList[i]+4);
			usResult=commRecieveFile(paramsList[i],baPath);
			
		}
		else
		if(strncmp(paramsList[i],"hot",3)==0||strncmp(paramsList[i],"Tran",4)==0)
		{
			sprintf(baPath,"/home/ap/pub/MTMS/%s",paramsList[i]);
			usResult=commRecieveRoot(paramsList[i],baPath);
		}
		else
		if(strncmp(paramsList[i],"TMS",3)==0)
		{
			sprintf(baPath,"./%s",paramsList[i]);
			usResult=commRecieveRoot(paramsList[i],baPath);
		}

		sprintf(baFile,"&Path=%s&Status=%d",paramsList[i],usResult==226?0:usResult);
		sprintf(manbuff,"Event: manPullFile :PullFile=%04x\n",usResult);
	    sysLog(manbuff);
		
		strcat(baPullResp,baFile);
		//CTOS_LCDTPrint("\n");
		//CTOS_LCDTPrint(baFile);
		//CTOS_KBDGet(&key);
	}
	

	
	//send response

	sprintf(baResponse,"Command=PullFile%s",baPullResp);


	usResult=commSendTms(baResponse,PATH_SUBMIT_COMMAND_RESULT); 


	free(paramsList);
	free(baPullResp);
	
	//sprintf(manbuff,"Event: manPullFile :return=%04x\n",usResult);
    //sysLog(manbuff);
    sysLogRet("manPullFile",0);
	return 0x00;
}
USHORT manUpdateApp()
{
	sysLogCall("manUpdateApp");
	BYTE key;
	pid_t pid;
	USHORT usResult,i;
	USHORT paramsCount;
	BYTE* *paramsList;
	BYTE baUpdateResp[300];
	//FILE *MCI=fopen("/home/ap/pub/update.mci","wb");
	//FILE *MMCI=fopen("/home/ap/pub/update.mmci","wb");
	FILE *MCI=fopen("update.mci","wb");
	FILE *MMCI=fopen("update.mmci","wb");
	BYTE pathname[200];


	sprintf(baResponse,"Command=UpdateApp");
	usResult=commSendTms(baResponse,PATH_REQUEST_COMMAND_PARAMETERS);


	
	paramsCount=getParamsCount(baResponse);
	
	sprintf(manbuff,"\ncount:%d",paramsCount);
	//CTOS_LCDTPrint(manbuff);
	
	
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
		//usResult=commRecieveFile(".",paramsList[i]);
		usResult=commRecieveCAP(paramsList[i],paramsList[i]);
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
	//CTOS_LCDTPrint("\n");
	//CTOS_LCDTPrint(pathname);
	//CTOS_KBDGet(&key);



	pid=fork();
	if(pid==0)
	{
		CTOS_UpdateFromMMCI(pathname,1);//w rabina yostor xD feha khabth xD

		kill(pid, SIGTERM);
		CTOS_LCDTPrint("tabaaan\n");
	}
	sleep(4);
	commInit();


	//CTOS_UpdateFromMMCI("/home/ap/pub/update.mmci",1);//w rabina yostor xD feha khabth xD
	usResult=CTOS_UpdateGetResult();
	memset(baUpdateResp,0,sizeof baUpdateResp);
	for (i = 0; i < paramsCount; ++i)
	{
		sprintf(manbuff,"&Name=%s&Status=%d",paramsList[i],usResult==0x64?0:0xFF);
		strcat(baUpdateResp,manbuff);

	}


	//delete apps srcs

	//send response
	 
	sprintf(baResponse,"Command=UpdateApp%s",baUpdateResp);
	usResult=commSendTms(baResponse,PATH_SUBMIT_COMMAND_RESULT); 

	free(paramsList);
	//CTOS_LCDTPrint("\nafter free");
	//CTOS_KBDGet(&key);
	sysLogRet("manPullFile",0);
	return 0x00;
}