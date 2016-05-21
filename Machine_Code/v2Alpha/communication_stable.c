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
#include <errno.h>
#include <sys/stat.h>
#include <curl/curl.h>
/*========================================*
* I N C L U D E S *
*========================================*/
#include "communication.h"
#include "util.h"
#include "GPRS.h"
#include "httpclient.h"


/*==========================================*
****************P R I V A T E ***************
*==========================================*/

/*==========================================*
* D E F I N E S *
*==========================================*/
  


  #define SIZE_HTTP_BUFF 1224
  #define SIZE_HTTP_MESSAGE 200
  #define SIZE_HTTP_HEADER 100
  #define SIZE_HTTP_PARAM 50

/*==========================================*
* C O N S T A N T S *
*==========================================*/
const BYTE tmsPaths[][50]={"/MTMS/PosUp.ashx",
                 "/MTMS/CreatePosRecord.ashx",
                 "/MTMS/SubmitCommandResult.ashx",
                  "/MTMS/RequestCommandParameters.ashx",};
/*==========================================*
* T Y P E S *
*==========================================*/

/*==========================================*
* V A R I A B L E S *
*==========================================*/

CURL *curl;
CURLcode res;

BYTE baTmsUrl[]="41.47.132.223";
USHORT usTmsPort=80;
BYTE ftpUser[]="MTMS_USER";
BYTE ftpPass[]="1234";


BYTE baHttpParam[SIZE_HTTP_PARAM];
BYTE baHttpHeader[SIZE_HTTP_HEADER];
BYTE baHttpMessage[SIZE_HTTP_MESSAGE];
BYTE baHttpBuff[SIZE_HTTP_BUFF];
USHORT usHttpBuffLen=SIZE_HTTP_BUFF;

/*==========================================*
* M A C R O S *
*==========================================*/

/*==========================================*
* F U N C T I O N S *
*==========================================*/
USHORT ftpUpload(BYTE *baSrcPath,BYTE *baFileName);
/*USHORT commPingTms(OUT USHORT* usCommand)
{
  //local variables 
  USHORT usRet;
  //check/establish connection
  gprsOpen();
  usret=gprsConnect(baTmsUrl,usTmsPort);
  //build request
  GetSerialNumber(baSerialNumber);
  sprintf(baHttpParam,"SerialNumber:%s\n",baSerialNumber);
  HTTP_BuildHeader(baHttpHeader,tmsPaths[PATH_POSUP],"GET",baHttpParam);  
  //send request
  gprsSend(baHttpHeader,strlen(baHttpHeader));
  //wait response
  usHttpBuffLen=SIZE_HTTP_BUFF;
  usret=gprsRecieve(baHttpBuff,&usHttpBuffLen);
  HTTP_GetMessage(baHttpMessage,baHttpBuff);
  //
  if(sscanf(baHttpMessage,"%d",usCommand)!=1)
    usRet=0xFF;

  CTOS_LCDTClearDisplay();
  CTOS_LCDTPrint(baHttpMessage);

  gprsDisconnect();
  return usRet;
}*/
static size_t
WriteMemoryCallback(void *contents, size_t size, size_t nmemb, void **userp)
{
    BYTE key;
    size_t realsize = size * nmemb;
    if(realsize<=1)return realsize;
    // BYTE *mem = (BYTE *)userp;
    // mem = realloc(mem,  realsize + 1);
 
    // if(mem == NULL)
    // {
    //     /* out of memory! */ 
    //     CTOS_LCDTPrint("not enough memory \n(realloc returned NULL)\n");
    //     return 0;
    // }
 
    // memcpy(mem, contents, realsize);
    // mem[realsize] = 0;
    
    // if(userp!=mem)
    // {
    //     free(userp);
    //     userp=mem;
    // }
    //sprintf(baHttpBuff,"\n%p->%s",*userp,(BYTE*)*userp);
    //CTOS_LCDTPrint(baHttpBuff);
    BYTE *mem = (BYTE *)*userp;
    mem =realloc(mem,  realsize + 1);
    
    if(mem == NULL)
    {
        /* out of memory! */ 
        CTOS_LCDTPrint("not enough memory \n(realloc returned NULL)\n");
        return 0;
    }
     memcpy(mem, contents, realsize);
    mem[realsize] = 0;
    *userp=mem;
    //sprintf(baHttpBuff,"\n%p->%s",*userp,(BYTE*)*userp);
    //CTOS_LCDTPrint(baHttpBuff);
    CTOS_LCDTPrint(userp);

        //CTOS_KBDGet(&key);
    return realsize;
}
USHORT commInit()
{

  gprsOpen();  
  gprsConnect(baTmsUrl,usTmsPort);  
  GetSerialNumber(baSerialNumber);
}
USHORT commClose()
{
    gprsDisconnect();
    gprsClose();
}
USHORT commSendTms(INOUT BYTE *baMessage,IN USHORT usPathIndex)
{
    //local variables
    BYTE key;
    USHORT usret;
    BYTE baUrl[100];
    enuGprsStateType gprsState;
    BYTE *baResponse;
    gprsState = gprsGetState();

  
    //memory allocation 
    baResponse=(BYTE*)malloc(2);
   // sprintf(baHttpBuff,"\n%p->%s",(void*)baResponse,(BYTE*)&baResponse);
   // CTOS_LCDTPrint(baHttpBuff);

    //check GPRS connection
    if(gprsState!=GPRS_ESTABLISHED)
    {
        usret=gprsConnect(baTmsUrl,usTmsPort);
        if(usret!=d_OK)return usret;
    }

    //cURL starts
    curl_global_init(CURL_GLOBAL_ALL);
    /* get a curl handle */ 
    curl = curl_easy_init();
    if(curl) 
    {
        //local for curl : header
        struct curl_slist *params = NULL;
        
        //POST
        /* First set the URL that is about to receive our POST. This URL can
           just as well be a https:// URL if that is what should receive the
           data. */
        sprintf(baUrl,"http://%s/%s",baTmsUrl,tmsPaths[usPathIndex]); 
        curl_easy_setopt(curl, CURLOPT_URL, baUrl);
        /* Now specify the POST data */ 
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS,baMessage);

        //HEADER
        /* Add a custom header */ 
        sprintf(baHttpHeader,"SerialNumber:%s",baSerialNumber);
        params = curl_slist_append(params, baHttpHeader);
        res = curl_easy_setopt(curl, CURLOPT_HTTPHEADER, params);

        //Time to wait response
        curl_easy_setopt(curl, CURLOPT_LOW_SPEED_LIMIT,1);//lower than 1 byte/sec
        curl_easy_setopt(curl, CURLOPT_LOW_SPEED_TIME,30);


        //Response
        /* send all data to this function  */ 
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);

        /* we pass our 'chunk' struct to the callback function */ 
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&baResponse);
        


        /* Perform the request, res will get the return code */ 
        if(gprsState!=GPRS_ESTABLISHED)
        {
            usret=gprsConnect(baTmsUrl,usTmsPort);
            if(usret!=d_OK)return usret;
        }
        res = curl_easy_perform(curl);
        //CTOS_LCDTPrint("\n after Perf");
        //CTOS_KBDGet(&key);
        
        /* Check for errors */ 
        if(res != CURLE_OK){
          //fprintf(stderr, "curl_easy_perform() failed: %s\n",curl_easy_strerror(res));
            sprintf(baUrl,"curl() f:%s\n",curl_easy_strerror(res));
            CTOS_LCDTPrint("\n");
            CTOS_LCDTPrint(baUrl);
            CTOS_LCDTPrint("\n");
            CTOS_LCDTPrint(baUrl+20);
            CTOS_LCDTPrint("\n");
            CTOS_LCDTPrint(baUrl+40);
            CTOS_KBDGet(&key);
            strcpy(baMessage,"0");

        }

        else
        {
           // CTOS_LCDTPrint("\n after pref\n");
    //        sprintf(baHttpBuff,"\n%p->%s",(void*)baResponse,(BYTE*)baResponse);
            //CTOS_LCDTPrint(baHttpBuff);
            //CTOS_LCDTPrint(baResponse);
            strcpy(baMessage,baResponse);
            //CTOS_LCDTPrint("\n after cpy");
            //CTOS_LCDTPrint(baMessage);
            //CTOS_KBDGet(&key);
        }
        /* always cleanup */ 
        
        curl_easy_cleanup(curl);        
    }
    curl_global_cleanup();
    //CTOS_LCDTPrint("\n after clean");
    //CTOS_KBDGet(&key);
    return usret;
}

USHORT commSendTms2(INOUT BYTE *baMessage,IN USHORT usPathIndex)
{
    USHORT usret;
    gprsOpen();
    usret=gprsConnect(baTmsUrl,usTmsPort);
    if(usret!=d_OK)return usret;
    sprintf(baHttpParam,"SerialNumber:%s\nContent-Length:%d",baSerialNumber,strlen(baMessage));
    HTTP_BuildHeader(baHttpHeader,tmsPaths[usPathIndex],"POST",baHttpParam);
    HTTP_BuildRequest(baHttpBuff,baHttpHeader,baMessage);
    gprsSend(baHttpBuff,strlen(baHttpBuff));
    usHttpBuffLen=SIZE_HTTP_BUFF;
    memset(baHttpBuff,0,sizeof baHttpBuff);
    
    usret=gprsRecieve(baHttpBuff,&usHttpBuffLen);
    if(usret!=d_OK)return usret;
    HTTP_GetMessage(baHttpMessage,baHttpBuff);
    memset(baMessage,0,sizeof baMessage);
    strcpy(baMessage,baHttpMessage);
    CTOS_LCDTPrintXY(6,1,baMessage);
    if(strlen(baHttpBuff)==0)
      return 0xFF;//error
    return 0;//ack
}

USHORT commSendFile(IN BYTE * baFilePath,IN BYTE * baFileName)
{
    gprsConnect(baTmsUrl,21);
    return ftpUpload(baFilePath,baFileName);
    
}


USHORT ftpUpload(BYTE *baSrcPath,BYTE *baFileName)
{
    BYTE baUrl[100];
    BYTE baStderr[200];
    CURL *curl;
    CURLcode res;
    ULONG codep;
    struct stat file_info;
    double speed_upload, total_time;
    FILE *fd;
    BYTE baFilePath[100];
    sprintf(baFilePath,"%s/%s",baSrcPath,baFileName);
    //strcat(baSrcPath,baFileName);
    fd = fopen(baFilePath, "rb"); /* open file to upload */ 
    if(!fd) {
        return 1; /* can't continue */ 
    }

    /* to get the file size */ 
    if(fstat(fileno(fd), &file_info) != 0) {
        return 1; /* can't continue */ 
    }
    sprintf(baUrl,"ftp://%s:%s@%s/Terminals/Castles/%s",ftpUser,ftpPass,baTmsUrl,baFileName);//serial number folder



    curl_global_init(CURL_GLOBAL_ALL);//could be called once
    curl = curl_easy_init();
    if(curl) {
        /* upload to this place */ 
        curl_easy_setopt(curl, CURLOPT_URL,baUrl);
        /* tell it to "upload" to the URL */ 
        curl_easy_setopt(curl, CURLOPT_UPLOAD, 1L);

        /* set where to read from (on Windows you need to use READFUNCTION too) */ 
        curl_easy_setopt(curl, CURLOPT_READDATA, fd);

        /* and give the size of the upload (optional) */ 
        //curl_easy_setopt(curl, CURLOPT_INFILESIZE_LARGE,
        //                 (curl_off_t)file_info.st_size);

        /* enable verbose for easier tracing */ 
        curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);

        res = curl_easy_perform(curl);
        
        curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &codep);
        CTOS_LCDTPrint("state=");
        utilPrintI(codep);
        CTOS_LCDTPrint("\n");
        /* Check for errors */ 
        if(res != CURLE_OK) {
          
            sprintf(baStderr, "curl_easy_perform() failed: %s\n",
            curl_easy_strerror(res));
            CTOS_LCDTPrint(baStderr);
            CTOS_LCDTPrint("\n");
            CTOS_LCDTPrint(baStderr+20);
            CTOS_LCDTPrint("\n");
            CTOS_LCDTPrint(baStderr+40);
            CTOS_LCDTPrint("\n");
            CTOS_LCDTPrint(baStderr+60);
            CTOS_LCDTPrint("\n");
            CTOS_LCDTPrint(baStderr+80);

        }
        else {
            /* now extract transfer info */ 
            curl_easy_getinfo(curl, CURLINFO_SPEED_UPLOAD, &speed_upload);
            curl_easy_getinfo(curl, CURLINFO_TOTAL_TIME, &total_time);

            //sprintf(baStderr, "Speed: %.3f bytes/sec during %.3f seconds\n",
            //speed_upload, total_time);
            //CTOS_LCDTPrint(baStderr);
        }
    /* always cleanup */ 
    curl_easy_cleanup(curl);
    }
    curl_global_cleanup();
    return codep;

}

USHORT ftpDownload(BYTE *baSrcPath,BYTE *baFileName)
{
    BYTE baUrl[100];
    BYTE baStderr[200];
    CURL *curl;
    CURLcode res;
    ULONG codep;
    struct stat file_info;
    double speed_upload, total_time;
    FILE *fd;
    BYTE baFilePath[100];
    sprintf(baFilePath,"%s/%s",baSrcPath,baFileName);
    //strcat(baSrcPath,baFileName);
    fd = fopen(baFilePath, "wb"); /* open file to upload */ 
    if(!fd) {
        return 1; /* can't continue */ 
    }

    /* to get the file size */ 
    /*if(fstat(fileno(fd), &file_info) != 0) {
        return 1; /* can't continue */ 
    //}
    sprintf(baUrl,"ftp://%s:%s@%s/Terminals/Castles/%s",ftpUser,ftpPass,baTmsUrl,baFileName);//serial number folder



    curl_global_init(CURL_GLOBAL_ALL);//could be called once
    curl = curl_easy_init();
    if(curl) {
        /* upload to this place */ 
        curl_easy_setopt(curl, CURLOPT_URL,baUrl);
        
        
       
         curl_easy_setopt(curl, CURLOPT_WRITEDATA, fd);

        /* and give the size of the upload (optional) */ 
        //curl_easy_setopt(curl, CURLOPT_INFILESIZE_LARGE,
        //                 (curl_off_t)file_info.st_size);

        /* enable verbose for easier tracing */ 
        curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);

        res = curl_easy_perform(curl);
        
        curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &codep);
        CTOS_LCDTPrint("xxstate=");
        utilPrintI(codep);
        CTOS_LCDTPrint("\n");
        /* Check for errors */ 
        if(res != CURLE_OK) {
          
            sprintf(baStderr, "curl_easy_perform() failed: %s\n",
            curl_easy_strerror(res));
            CTOS_LCDTPrint(baStderr);
            CTOS_LCDTPrint("\n");
            CTOS_LCDTPrint(baStderr+20);
            CTOS_LCDTPrint("\n");
            CTOS_LCDTPrint(baStderr+40);
            CTOS_LCDTPrint("\n");
            CTOS_LCDTPrint(baStderr+60);
            CTOS_LCDTPrint("\n");
            CTOS_LCDTPrint(baStderr+80);

        }
        
    /* always cleanup */ 
    curl_easy_cleanup(curl);
    }
    curl_global_cleanup();
    fclose(fd);
    return codep;

}

USHORT commRecieveFile(IN BYTE * baFilePath,IN BYTE * baFileName)
{
    USHORT sysret;
    BYTE cmdline[300];
    gprsConnect(baTmsUrl,21);
    //sprintf(cmdline,"wget -O %s/%s ftp://%s:%s@%s/Terminals/Castles/%s > stdout.txt 2> stderr.txt",baFilePath,baFileName,ftpUser,ftpPass,baTmsUrl,baFileName);
    //sysret=system(cmdline);
    //return sysret;
    return ftpDownload(baFilePath,baFileName);
    
}

USHORT wgetftpDownload(BYTE* baHost,BYTE* baUser,BYTE* baPass,BYTE* baPath,BYTE* baFileName)
{
  USHORT sysret;
  BYTE cmdline[300];
  sprintf(cmdline,"wget -O /home/ap/pub/%s ftp://%s:%s@%s/%s/%s > stdout.txt 2> stderr.txt",baFileName,baUser,baPass,baHost,baPath,baFileName);
  sysret=system(cmdline);
  return sysret;
}
USHORT commDownloadFile(BYTE* baFileName)
{
 //return ftpDownload(baTmsUrl,"MTMS_FTP","1234","Terminals/Castles",baFileName);
}


