/** 
**	A Template for developing new terminal application
**/

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
/** These two files are necessary for calling CTOS API **/
#include <ctosapi.h>


/** 
** The main entry of the terminal application 
**/
int main(int argc,char *argv[])
{
	BYTE key;
    ULONG count;
        
	// TODO: Add your program here //
	while(1){
        count++;
        if(count==1<<30)
        {
            CTOS_LCDTClearDisplay();    
            CTOS_LCDTPrintXY(1, 1, "trans");
            count=0;
        }
        //CTOS_KBDGet(&key);
	}
	exit(0);
}
