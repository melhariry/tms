/*========================================*
 * I N C L U D E S *
 *========================================*/
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
/** These two files are necessary for calling CTOS API **/
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
 * D E F I N E S *
 *========================================*/

/* ==================================================================
 * FUNCTION NAME: show_errormsg
 * DESCRIPTION: Show error message on the LCD screen
 * RETURN: none.
 * NOTES: none.
 * ================================================================ */ 



/* ==========================================================================
 * FUNCTION NAME: main
 * DESCRIPTION:   function main is responsible for all the tasks listed in the
 *                introduction to this demo.
 * RETURN:        none.
 * NOTES:         none.
 * ========================================================================== */
 char xfiles[200][255];
 void print(char list[][255],int offset)
 {
 	int i;
 	for (i = 0; i < 2; ++i)
 	{
 		CTOS_LCDTPrint(list[i+offset]);
 	}
 }
 unsigned long fsize(char* file)
{
    FILE * f = fopen(file, "r");
    fseek(f, 0, SEEK_END);
    unsigned long len = (unsigned long)ftell(f);
    fclose(f);
    return len;
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
extern  int alphasort();
 USHORT lsR(BYTE *pathname,BYTE *parent,BYTE list[][255],BOOL app,int offset)
{
    int count,i,chcount=0;
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
      
      //sprintf(bufStr,"&Name=%s&Type=%d&Parent=%s&Size=%d",files[i]->d_name,files[i]->d_type,parent,size);
      sprintf(bufStr,"Name=%s\nType=%d\nParent=%s\nSize=%d\n----------\n",files[i]->d_name,files[i]->d_type,parent,size);
     // sprintf(bufStr,"%s\n",files[i]->d_name);//|%d|%s|%d,files[i]->d_type,parent,size);
      //CTOS_LCDTClearDisplay();
      //CTOS_LCDTPrint(bufStr);
      //strcat(list,bufStr);
      
	  strcpy(list[i+offset],bufStr);
	  //strcpy(list[0],"taban");
      //CTOS_KBDGet(&key);
      if(files[i]->d_type==1)
      {
        sprintf(bufStr,"%s/%s",pathname,files[i]->d_name);

        chcount+=lsR(bufStr,files[i]->d_name,list,app,offset+count);
        //offset+=chcount;
      }

    }
    return chcount+count;
}
int main(int argc,char *argv[])
{
    
    CTOS_LCDTSelectFontSize(0x101E);
    //CTOS_LCDTPrint("Name|Type|Parent|Size");
    //exeListFiles(list);
    int count=lsR("/home/ap/pub/","pub",xfiles,0,0);
    

    int offset=0;
    USHORT key;
      CTOS_KBDGet(&key);
    while(1)
    {	
    	CTOS_LCDTClearDisplay();
    	print(xfiles,offset);

    	//CTOS_LCDTPrint(list+offset);

	    CTOS_KBDGet(&key);
	    switch(key)
	    {
	    	case d_KBD_CANCEL:
	    		exit(0);
	    		break;
	    	case d_KBD_DOT:
	    		offset++;
	    		if(offset>count)
	    			offset--;
	    		break;
	    	case d_KBD_9:
	    		offset--;
	    		if(offset<0)
	    			offset++;
	    		break;
	    	default:
	    		exit(0);
	    	
	    }
    }
    exit(0);
}
