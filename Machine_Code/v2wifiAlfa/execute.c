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

#include "files.h"
/*==========================================*
****************P R I V A T E ***************
*==========================================*/

/*==========================================*
* D E F I N E S *
*==========================================*/


/*==========================================*
* C O N S T A N T S *
*==========================================*/

/*==========================================*
* T Y P E S *
*==========================================*/


/*==========================================*
* V A R I A B L E S *
*==========================================*/


/*==========================================*
* M A C R O S *
*==========================================*/

/*==========================================*
* F U N C T I O N S *
*==========================================*/
extern  int alphasort();
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
      CTOS_LCDTPrint(baNames);
      strcat(baNames,"\n");
      
    }
}

USHORT  exeListApps(BYTE* baAppList)
{
  BYTE key;
  CTOS_stCAPInfo appInfo;
  BYTE appNames[1000];
  BYTE *tokens[50];
  int i;
  USHORT index;
  USHORT usNoTokens ;
  BYTE baTemp[100];
  ls("/home/ap/",appNames,TRUE);
  split(appNames, "\n", tokens,&usNoTokens);
  memset(baAppList,0,sizeof baAppList);
  for(i=0;i<usNoTokens;++i)
  {
    sscanf(tokens[i],"%d",&index);
    CTOS_APGet(index,&appInfo);
    memset(baTemp,0,sizeof baTemp);
    sprintf(baTemp,"&Name=%s&Version=0x%x%x&Com=%s",appInfo.baName,appInfo.baVersion[0],appInfo.baVersion[1],appInfo.baCompany);
    //strcpy(baAppList,baTemp);
    strcat(baAppList,baTemp);

  }
  return 0;
}
USHORT lsl(BYTE *pathname,CTOS_FILE_ATTRIB fileInfo[],BOOL app)
{
    int count,i;
    struct direct **files;
    struct stat info;
    int file_select();
    int app_select();
    if(app)
      count = scandir(pathname, &files, app_select, alphasort);
    else
      count = scandir(pathname, &files, file_select, alphasort);
   
    for (i=1;i<count+1;++i){
      strcpy(fileInfo[i-1].Filename,files[i-1]->d_name);
      
       CTOS_FileGetSize(fileInfo[i-1].Filename,&fileInfo[i-1].Filesize);
       continue;

        if ( stat(files[i-1]->d_name, &info) == -1 )    /* cannot stat   */
       {
        CTOS_LCDTPrint("\nf:");
        CTOS_LCDTPrint(strerror(errno));
        CTOS_LCDTPrint("\n");
        CTOS_LCDTPrint(files[i-1]->d_name);

        }

        else          /* else show info  */
        {
            
            fileInfo[i-1].Filesize=info.st_size;
            //CTOS_LCDTPrint("\n suc:");
            //CTOS_LCDTPrint(fileInfo[i-1].Filename);
        
        }

    }
    return count;
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

  CTOS_FILE_ATTRIB stAttrib[256];  //Structure array to retrieve the attribute of file //
  USHORT usFileNo;
  USHORT usRet;
  int i;
  BYTE baTemp[100];
  
  memset(baFileList,0,sizeof baFileList);
  lsR("/home/ap/pub","pub",baFileList,0);
  //CTOS_KBDGet(&usRet);
  lsR(".","pri",baFileList,0);



  return 0;
  //List the attributes of all files //
  //public
  usFileNo=lsl("/home/ap/pub",stAttrib,0);
  for (i=0; i<usFileNo; i++)
  {
    memset(baTemp,0,sizeof baTemp);
    sprintf(baTemp,"&name=%s&attribute=255&size=%ld",stAttrib[i].Filename,stAttrib[i].Filesize);
    strcat(baFileList,baTemp);
  }  
  usFileNo=lsl(".",stAttrib,0);
  for (i=0; i<usFileNo; i++)
  {
    memset(baTemp,0,sizeof baTemp);
    sprintf(baTemp,"&name=%s&attribute=0&size=%ld",stAttrib[i].Filename,stAttrib[i].Filesize);
    strcat(baFileList,baTemp);
  }  



  return 0;
  usRet = CTOS_FileDirAttrib(stAttrib, &usFileNo);
  memset(baFileList,0, sizeof baFileList);
  for (i=0; i<usFileNo; i++)
  {
    memset(baTemp,0,sizeof baTemp);
    CTOS_FileGetSize(stAttrib[i].Filename,&stAttrib[i].Filesize); 
    sprintf(baTemp,"&name=%s&attribute=%d&size=%ld",stAttrib[i].Filename,stAttrib[i].FileAttrib,stAttrib[i].Filesize);
    
    strcat(baFileList,baTemp);
    //}
  }


 
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
USHORT  exeDeleteFiles(IN BYTE * fileNames[],IN int length, OUT BYTE deleteResults[])
{
    int i;
    USHORT usret=0;
    SHORT sfile;
    BYTE baTemp[100];
    for(i =0;i<length; ++i)
    {
        usret=usret<<1;
        //usfile= DeleteFile(fileNames[i]);    
        if(strncmp(fileNames[i],"pub",3)==0)
            sprintf(baTemp,"/home/ap/%s",fileNames[i]);
        else
            sprintf(baTemp,"./%s",fileNames[i]+4);
        CTOS_LCDTPrint(baTemp);
        sfile=exeDeleteFile(baTemp);
        usret|=sfile==0?0:255;
        sprintf(baTemp,"&Name=%s&State=%x",fileNames[i],sfile==0?0:255);// can be done in manager
        strcat(deleteResults,baTemp);
    }
    //CTOS_LCDTPrint("\n");

    return usret;
}



USHORT  exeSystemTest(BYTE* baTestResult)
{
  CTOS_LCDTClearDisplay();
          
  CTOS_LCDTPrintXY(1, 1, "Please wait\n system testing is running\n");
  BYTE baBuffer[40],str[10];
  USHORT ret;
  strcpy(baTestResult,"");

  ///////////////////////////////////////////////////Test begins here/////////////////////////////////////////////////////////
  //retrieve system memory info 
  ULONG  ulUsedDiskSize,  ulTotalDiskSize, ulUsedRamSize, ulTotalRamSize;
  ret = CTOS_SystemMemoryStatus (&ulUsedDiskSize ,&ulTotalDiskSize,&ulUsedRamSize,&ulTotalRamSize );
  
  //strcpy(baBuffer,"&memory=");
  //sprintf(str,"%d",ret);
  //strcat(baBuffer,str);
  memset(str,0,sizeof(str));

  //Test failed 
  if(ret == d_OK)
  {
       // sprintf(str , ",%lu" , ulTotalDiskSize);
        //strcat(baBuffer,str);
        //memset(str, 0 , sizeof(str));
        
        sprintf(str , "&UsedDiskSize=%lu" , ulUsedDiskSize);
        strcat(baBuffer,str);
        memset(str, 0 , sizeof(str));
        
         //sprintf(str , ",%lu" , ulTotalRamSize);
         //strcat(baBuffer,str);
         //memset(str, 0 , sizeof(str)); 
         
         sprintf(str , "&UsedRamSize=%lu" , ulUsedRamSize);
         strcat(baBuffer,str);
         memset(str, 0 , sizeof(str));
          
  }
    
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
    
  //clear LCD display
  CTOS_LCDTClearDisplay();
    return d_OK;
    
}

