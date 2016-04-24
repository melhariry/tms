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

// int FTP2()
// {
//   CURLcode ret;
//   CURL *hnd;

//   hnd = curl_easy_init();
//   curl_easy_setopt(hnd, CURLOPT_INFILESIZE_LARGE, (curl_off_t)6);
//   curl_easy_setopt(hnd, CURLOPT_URL, "ftp://MTMS_FTP:1234@197.34.98.233/Terminals/Castles/taban.txt");
//   curl_easy_setopt(hnd, CURLOPT_HEADER, 1L);
//   curl_easy_setopt(hnd, CURLOPT_UPLOAD, 1L);
//   curl_easy_setopt(hnd, CURLOPT_USERAGENT, "curl/7.35.0");
//   curl_easy_setopt(hnd, CURLOPT_MAXREDIRS, 50L);
//   curl_easy_setopt(hnd, CURLOPT_VERBOSE, 1L);
// //  curl_easy_setopt(hnd, CURLOPT_TCP_KEEPALIVE, 1L);

//    Here is a list of options the curl code used that cannot get generated
//      as source easily. You may select to either not use them or implement
//      them yourself.

//   CURLOPT_WRITEDATA set to a objectpointer
//   CURLOPT_WRITEFUNCTION set to a functionpointer
//   CURLOPT_READDATA set to a objectpointer
//   CURLOPT_READFUNCTION set to a functionpointer
//   CURLOPT_SEEKDATA set to a objectpointer
//   CURLOPT_SEEKFUNCTION set to a functionpointer
//   CURLOPT_ERRORBUFFER set to a objectpointer
//   CURLOPT_STDERR set to a objectpointer
//   CURLOPT_DEBUGFUNCTION set to a functionpointer
//   CURLOPT_DEBUGDATA set to a objectpointer
//   CURLOPT_HEADERFUNCTION set to a functionpointer
//   CURLOPT_HEADERDATA set to a objectpointer

  

//   ret = curl_easy_perform(hnd);

//   curl_easy_cleanup(hnd);
//   hnd = NULL;

//   return (int)ret;
// }

USHORT xftpUpload()
{
    BYTE baStderr[200];
  CURL *curl;
  CURLcode res;
  struct stat file_info;
  double speed_upload, total_time;
  FILE *fd;
 
  fd = fopen("/home/ap/pub/elGP3.txt", "rb"); /* open file to upload */ 
  //fscanf(fd,"%s",baStderr);
  //CTOS_LCDTPrint(baStderr);
  if(!fd) {
 
    return 1; /* can't continue */ 
  }
 
  /* to get the file size */ 
  if(fstat(fileno(fd), &file_info) != 0) {
 
    return 1; /* can't continue */ 
  }
 
  curl = curl_easy_init();
  if(curl) {
    /* upload to this place */ 
    curl_easy_setopt(curl, CURLOPT_URL,
                     "ftp://MTMS_FTP:1234@197.34.98.233/Terminals/Castles/elGP3.txt");
 
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
 
      sprintf(baStderr, "Speed: %.3f bytes/sec during %.3f seconds\n",
              speed_upload, total_time);
      CTOS_LCDTPrint(baStderr);
 
    }
    /* always cleanup */ 
    curl_easy_cleanup(curl);
  }
}



int main(int argc,char *argv[])
{
	BYTE key;
  USHORT usRet;
  BYTE baStr;    
  FILE * f=fopen("test1.txt","w");
  fprintf(f, "%s\n", "test1");
  fclose(f);
  /*f=fopen("test2.txt","w");
  fprintf(f, "%s\n", "test2");
  fclose(f);
  f=fopen("test3.txt","w");
  fprintf(f, "%s\n", "test3");
  fclose(f);*/
	// TODO: Add your program here //
  
  //system("cat * >./tests.txt");
	CTOS_LCDTClearDisplay();
  manUp();
       
  /*
    gprsOpen();
    gprsConnect("197.34.98.233",21);
        //commRecieveFile("lol","aylaa");
        CTOS_LCDTClearDisplay();
        //manFileList();
        //CTOS_KBDGet(&key);
        if(ftpUpload()){
        	CTOS_LCDTPrint("not ok");
        }
        else{
        	CTOS_LCDTPrint("ok");
        }
            
        CTOS_KBDGet(&key);
        */
	CTOS_KBDGet(&key);
	exit(0);
}
