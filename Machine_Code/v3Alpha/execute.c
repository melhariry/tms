/**
  ******************************************************************************
  * file          execute.c
  * author        
  * component     execute
  * description   execute implementation
  *               execute module executes actions on device
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

#include <errno.h>
#include <sys/types.h>
#include <sys/dir.h>
#include <sys/param.h>
#include  <sys/types.h>
#include  <dirent.h>
#include  <sys/stat.h>
#include  <string.h>

/*========================================*
* I N C L U D E S *
*========================================*/
#include "execute.h"
#include "test.h"
#include "util.h"

//#include "files.h"
/*==========================================*
****************P R I V A T E ***************
*==========================================*/

/*==========================================*
* D E F I N E S *
*==========================================*/
#define PATHMAX 512

/*==========================================*
* C O N S T A N T S *
*==========================================*/

/*==========================================*
* T Y P E S *
*==========================================*/


/*==========================================*
* V A R I A B L E S *
*==========================================*/
BYTE absPath[PATHMAX];
BYTE exebuff[1024];
BYTE appNames[1000];
/*==========================================*
* M A C R O S *
*==========================================*/

/*==========================================*
* F U N C T I O N S *
*==========================================*/
extern  int alphasort();

void ls(BYTE *pathname,BYTE* baNames,BOOL app)
{
    int count,i;
    struct direct **files;
    int file_select();
    int app_select();
    if(app)
      count = scandir(pathname, &files, app_select, alphasort);
    else
      count = scandir(pathname, &files, file_select, alphasort);
    memset(baNames,0,sizeof baNames);

    for (i=1;i<count+1;++i){
    
      //sprintf(baBuffer,"%s\n",files[i-1]->d_name);
      strcat(baNames,files[i-1]->d_name);
     // CTOS_LCDTPrint(baNames);
      strcat(baNames,"\n");
      
    }
}
int app_select(struct direct   *entry)
{
  if ((strcmp(entry->d_name, ".") == 0) ||(strcmp(entry->d_name, "..") == 0))
    return (FALSE);
  else
  if ((strcmp(entry->d_name, "pub") == 0))
    return (FALSE);
  return (TRUE);
}
int file_select(struct direct   *entry)
{
  if ((strcmp(entry->d_name, ".") == 0) ||(strcmp(entry->d_name, "..") == 0))
    return (FALSE);
  else
  return (TRUE);
}
USHORT  exeListApps(BYTE* baAppList)
{
  sysLogCall("exeListApps");
  BYTE key;
  CTOS_stCAPInfo appInfo;
  
  BYTE *tokens[50];
  int i;
  USHORT index;
  USHORT usNoTokens ;
  ls("/home/ap/",appNames,TRUE);
  split(appNames, "\n", tokens,&usNoTokens);
  memset(baAppList,0,sizeof baAppList);
  for(i=0;i<usNoTokens;++i)
  {
    sscanf(tokens[i],"%d",&index);
    CTOS_APGet(index,&appInfo);//API
    memset(exebuff,0,sizeof exebuff);
    /*CTOS_LCDTPrint(appInfo.baName);
    CTOS_LCDTPrint("\n");
*/
    sprintf(exebuff,"&Name=%s&Version=%d&Com=%s",appInfo.baName,(int)appInfo.baVersion[0]<<8|appInfo.baVersion[1],appInfo.baCompany);
    //strcpy(baAppList,exebuff);
    strcat(baAppList,exebuff);

  }
  sysLogRet("exeListApps",0);
  return 0;
}
unsigned long fsize(char* file)
{
    FILE * f = fopen(file, "r");
    fseek(f, 0, SEEK_END);
    unsigned long len = (unsigned long)ftell(f);
    fclose(f);
    return len;
}
USHORT lsR(BYTE *pathname,BYTE *parent,BYTE* list,BOOL app)
{
    int count,i;
    BYTE bufStr[210];
    struct direct **files;
    struct stat info;
    int file_select();
    int app_select();
    int size;
    USHORT key;
    if(app)
      count = scandir(pathname, &files, app_select, alphasort);
    else
      count = scandir(pathname, &files, file_select, alphasort);
   
    for (i=0;i<count;++i)
    {
      if(files[i]->d_type==8) {
        sprintf(bufStr,"%s/%s",pathname,files[i]->d_name);
        size=fsize(bufStr);
        files[i]->d_type=0;
      }
      else
      {
        size=0;
        files[i]->d_type=1;
      }
      
      sprintf(bufStr,"&Name=%s&Type=%d&Parent=%s&Size=%d",files[i]->d_name,files[i]->d_type,parent,size);
      //CTOS_LCDTClearDisplay();
      //CTOS_LCDTPrint(bufStr);
      strcat(list,bufStr);
      //CTOS_KBDGet(&key);
      if(files[i]->d_type==1)
      {
        sprintf(bufStr,"%s/%s",pathname,files[i]->d_name);
        lsR(bufStr,files[i]->d_name,list,app);
      }

    }
    return count;
}
USHORT  exeListFiles(BYTE* baFileList)
{
  sysLogCall("exeListFiles");
  USHORT usFileNo;
  USHORT usRet;
  int i;
  
  
  memset(baFileList,0,sizeof baFileList);
  lsR("/home/ap/pub","pub",baFileList,0);
  lsR(".","pri",baFileList,0);

  sysLogRet("exeListFiles",0);
  return 0;
}


/*USHORT  exeDeleteFiles(IN BYTE * fileNames[],IN int length, OUT USHORT  deleteResults[])
{

  int i;
  USHORT usret=0;
  for(i =0;i<length; ++i)
  {
     usret=usret<<1;
     deleteResults[i] = DeleteFile(fileNames[i]);    
     usret|=deleteResults[i]?1:0;
  }
  //CTOS_LCDTPrint("\n");

  return usret;
}*/
SHORT exeDeleteFile(BYTE *filePath)
{
  return remove(filePath);//api
}
int exegetParamsCount(IN BYTE * baRaw,IN BYTE *delimiter)
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
void exeparser(void *list,OUT USHORT * uslistLen, IN BYTE* baRaw,IN BYTE *delimiter)
{
  BYTE* token;
  BYTE** bplist;
  //list=malloc(sizeof (BYTE)*getParamsCount(baRaw,delimiter));

  bplist=(BYTE**)list;

  token=strtok(baRaw,delimiter);
  while(token)
  {
    bplist[(*uslistLen)++]=token;
    token=strtok(NULL,delimiter);
  }

}
USHORT  exeDeleteFiles(IN BYTE * filePath[],IN int length, OUT BYTE deleteResults[])
{
    sysLogCall("exeDeleteFiles");
    int i,j;
    USHORT usret=0;
    SHORT sfile;
    
    
    USHORT depth=0;
    BYTE **path;
    for(i =0;i<length; ++i)
    {
        depth=0;
        //CTOS_LCDTClearDisplay();
        path=(BYTE**)calloc(exegetParamsCount(filePath[i],"/")+2,sizeof (BYTE*));
        depth=0;
        exeparser(path,&depth,filePath[i],"/\0");
        memset(absPath,0,sizeof absPath);
        if(strcmp(path[0],"pub")==0)
        {
          strcpy(absPath,"/home/ap");
          for(j=0;j<depth;++j)
          {
            strcat(absPath,"/");
            strcat(absPath,path[j]);
          }
        }
        else
        if(strcmp(path[0],"pri")==0)
        {
          if (getwd(absPath) == NULL )
          {
            strcpy(absPath,"./");
          }
          for(j=1;j<depth;++j)
          {
            strcat(absPath,"/");
            strcat(absPath,path[j]);
          } 
        }
        
       
        if(strcmp(path[depth-1],"pub")!=0)
          sfile=0xff;
        else
           sfile=exeDeleteFile(absPath);
        usret|=(sfile==0)<<i;
        sprintf(exebuff,"&error=%s&path=%s&Name=%s&Status=%d&ParentFolder=%s",strerror(errno),absPath,path[depth-1],sfile,path[depth-2]);
        strcat(deleteResults,exebuff);
        for(j=0;j<depth;++j)
        {
          /*CTOS_LCDTPrint("\n");
          CTOS_LCDTPrint(path[j]);
          CTOS_KBDGet(&usret);*/
          path[j]=NULL;
        }
        
        free(path);
        {
          /*usret=usret<<1;
          //usfile= DeleteFile(fileNames[i]);    
          if(strncmp(fileNames[i],"pub",3)==0)
          {
              sprintf(exebuff,"/home/ap/%s",fileNames[i]);
              if(strncmp(fileNames[i]+4,"MTMS",4)==0)
                strcpy(baParent,"MTMS");
              else
                strcpy(baParent,"pub");

          }
          else
          {
            strcpy(baParent,"pri");
            sprintf(exebuff,"./%s",fileNames[i]+4);
          }
          
          CTOS_LCDTPrint(exebuff);
          sfile=exeDeleteFile(exebuff);
          usret|=sfile==0?0:255;
          sprintf(exebuff,"&Name=%s&Status=%d&ParentFolder=%s",fileNames[i],sfile==0?0:255,baParent);// can be done in manager
          strcat(deleteResults,exebuff);
          */
        }

        
    }
  
    sysLogRet("exeDeleteFiles",usret);
    return usret;
}



USHORT  exeSystemTest(BYTE* baTestResult)
{
  sysLogCall("exeSystemTest");
  //CTOS_LCDTClearDisplay();
          
  //CTOS_LCDTPrintXY(1, 1, "Please wait\n system testing is running\n");
  BYTE baBuffer[40],str[10];
  USHORT ret;
  memset(baTestResult,0,sizeof baTestResult);
  memset(baBuffer,0,sizeof baBuffer);

  ///////////////////////////////////////////////////Test begins here/////////////////////////////////////////////////////////
  //retrieve system memory info 
  ULONG  ulUsedDiskSize,  ulTotalDiskSize, ulUsedRamSize, ulTotalRamSize;
  ret = CTOS_SystemMemoryStatus (&ulUsedDiskSize ,&ulTotalDiskSize,&ulUsedRamSize,&ulTotalRamSize );
  
  //strcpy(baBuffer,"&memory=");
  //sprintf(str,"%d",ret);
  //strcat(baBuffer,str);
  memset(str,0,sizeof(str));

  //Test failed 
  if(ret != d_OK)
  {
    ulUsedDiskSize=-1;
    ulUsedRamSize=-1;
  } 
  sprintf(str , "&UsedDiskSize=%lu" , ulUsedDiskSize/1024);
  strcat(baBuffer,str);
  memset(str, 0 , sizeof(str));

  //sprintf(str , ",%lu" , ulTotalRamSize);
  //strcat(baBuffer,str);
  //memset(str, 0 , sizeof(str)); 

  sprintf(str , "&UsedRamSize=%lu" , ulUsedRamSize/1024);
  strcat(baBuffer,str);
  memset(str, 0 , sizeof(str));
          
  

    
  //write memory test results to file
  strcat(baTestResult , baBuffer);
  memset(baBuffer,0,sizeof baBuffer);

  //test Crypto
  ret = CryptoTest();
  strcpy(baBuffer,"&crypto=");
  sprintf(str,"%d",ret);
  strcat(baBuffer,str);
  memset(str,0,sizeof(str));

  //write Crypto test result to file
  strcat(baTestResult , baBuffer);
  memset(baBuffer,0,sizeof baBuffer);


  //test printer
  ret = PrinterTest();
  strcpy(baBuffer,"&printer=");
  sprintf(str,"%d",ret);
  strcat(baBuffer,str);
  memset(str,0,sizeof(str));

  //write printer test result to file 
  strcat(baTestResult , baBuffer);
  memset(baBuffer,0,sizeof baBuffer);


  //test timer 
  //ret = TimerTest();
  strcpy(baBuffer,"&timer=0");
  // sprintf(str,"%d",ret);
  strcat(baBuffer,str);
  memset(str,0,sizeof(str));*

  //write timer test result to file
  strcat(baTestResult , baBuffer);
  memset(baBuffer,0,sizeof baBuffer);

  //test RTC
  ret = RTCTest();
  strcpy(baBuffer,"&rtc=");
  sprintf(str,"%d",ret);
  strcat(baBuffer,str);
  memset(str,0,sizeof(str));

  //write Rtc test result to file
  strcat(baTestResult , baBuffer);
  memset(baBuffer,0,sizeof baBuffer);


  //test Buzzer
  ret = BuzzerTest();
  strcpy(baBuffer,"&buzzer=");
  sprintf(str,"%d",ret);
  strcat(baBuffer,str);
  memset(str,0,sizeof(str));

  //write buzzer test result to file
  strcat(baTestResult , baBuffer);
  memset(baBuffer,0,sizeof baBuffer);


  //test LED
  ret = LEDTest();
  strcpy(baBuffer,"&led=");
  sprintf(str,"%d",ret);
  strcat(baBuffer,str);
  memset(str,0,sizeof(str));

  //write LED test result to file
  strcat(baTestResult , baBuffer);
  memset(baBuffer,0,sizeof baBuffer);

  sysLogRet("exeSystemTest",0);
  return d_OK;
    
}

