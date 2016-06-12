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
#include "logger.h"
#include <fcntl.h>
#include "execute.h"

#include <unistd.h>
#include <sys/types.h>
#include <sys/dir.h>
#include <sys/param.h>
#include <stdio.h>
#include <errno.h>
//#include <signum.h>
#include <signal.h>
/** 
** The main entry of the terminal application 
**/
#include "curl/curl.h"




#include <stdio.h>
#include <signal.h>
#include <syslog.h>
#include <errno.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
 
#define DAEMON_NAME "simpledaemon"

 BYTE baStr[10000];
 
void daemonShutdown();
void signal_handler(int sig);
void daemonize(char *rundir, char *pidfile);


int pidFilehandle;
FILE *logger;

void signal_handler(int sig)
{
    switch(sig)
    {
        case SIGHUP:
            //syslog(LOG_WARNING, "Received SIGHUP signal.");
        	//fprintf(logger, "Received SIGHUP signal.\n"); 
            break;
        case SIGINT:
        case SIGTERM:
            CTOS_LCDTPrint( "Daemon exiting");
            //fprintf(logger, "Daemon exiting\n"); 
            daemonShutdown();
            exit(EXIT_SUCCESS);
            break;
        default:
            //syslog(LOG_WARNING, "Unhandled signal %s", strsignal(sig));

        //fprintf(logger, "Unhandled signal %s\n", strsignal(sig)); 
            break;
    }
}
 
void daemonShutdown()
{
    close(pidFilehandle);
}

void daemonize(char *rundir, char *pidfile)
{
    int pid, sid, i;
    char str[10];
    struct sigaction newSigAction;
    sigset_t newSigSet;
     
    /* Check if parent process id is set */
    if (getppid() == 1)
    {
        /* PPID exists, therefore we are already a daemon */
        return;
    }

    /* Set signal mask - signals we want to block */
    sigemptyset(&newSigSet);
    sigaddset(&newSigSet, SIGCHLD);  /* ignore child - i.e. we don't need to wait for it */
    sigaddset(&newSigSet, SIGTSTP);  /* ignore Tty stop signals */
    sigaddset(&newSigSet, SIGTTOU);  /* ignore Tty background writes */
    sigaddset(&newSigSet, SIGTTIN);  /* ignore Tty background reads */
    sigprocmask(SIG_BLOCK, &newSigSet, NULL);   /* Block the above specified signals */
     
    /* Set up a signal handler */
    newSigAction.sa_handler = signal_handler;
    sigemptyset(&newSigAction.sa_mask);
    newSigAction.sa_flags = 0;
     
        /* Signals to handle */
        sigaction(SIGHUP, &newSigAction, NULL);     /* catch hangup signal */
        sigaction(SIGTERM, &newSigAction, NULL);    /* catch term signal */
        sigaction(SIGINT, &newSigAction, NULL);     /* catch interrupt signal */

     
    /* Fork*/
    pid = fork();
     
    if (pid < 0)
    {
        /* Could not fork */
        exit(EXIT_FAILURE);
    }
     
    if (pid > 0)
    {
        /* Child created ok, so exit parent process */
        sprintf(baStr,"Child process created\n: %d\n", pid);
        CTOS_LCDTPrint(baStr);
        
        exit(EXIT_SUCCESS);
    }
     
    /* Child continues */
     
   // umask(027); /* Set file permissions 750 */ //check it
   // umask(022); /* Set file permissions 755 */ //check it
     
    /* Get a new process group */
    sid = setsid();
     
    if (sid < 0)
    {
        exit(EXIT_FAILURE);
    }
     
    /* close all descriptors */
   /* for (i = getdtablesize(); i >= 0; --i)
    {
        close(i);
    }*/
     
    /* Route I/O connections */

    /* Open STDIN */
    //i = open("/dev/null", O_RDWR);

    /* STDOUT */
    //dup(i);

    /* STDERR */
    //dup(i);
     
    //chdir(rundir); /* change running directory */
     
    /* Ensure only one copy */
    pidFilehandle = open(pidfile, O_RDWR|O_CREAT, 0600);
     
    if (pidFilehandle == -1 )
    {
        /* Couldn't open lock file */
        //CTOS_LCDTPrint( "Could not open PID lock file  exiting");
        
        exit(EXIT_FAILURE);
    }
     
    /* Try to lock file */
    // if (lockf(pidFilehandle,F_TLOCK,0) == -1)
    // {
    //     /* Couldn't get lock on lock file */
    //    // CTOS_LCDTPrint( "Could not lock PID lock file  exiting");
    //      fprintf(logger, "Could not lock PID lock file  exiting\n" ); 
    //     exit(EXIT_FAILURE);
    // }

     
    /* Get and format PID */
    sprintf(str,"%d\n",getpid());
     
    /* write pid to lockfile */
    write(pidFilehandle, str, strlen(str));
}
 
int main()
{
    /* Debug logging
    setlogmask(LOG_UPTO(LOG_DEBUG));
    openlog(DAEMON_NAME, LOG_CONS, LOG_USER);
    */
     
    /* Logging */
    //setlogmask(LOG_UPTO(LOG_INFO));
    //openlog(DAEMON_NAME, LOG_CONS | LOG_PERROR, LOG_USER);
    
    remove("/tmp/daemon2.pid");
    //logger=fopen("/home/ap/pub/MTMS/daemon_session.txt","rb");
   

    
    /* Deamonize */
   daemonize(".", "/tmp/daemon2.pid");
    sysLog("Daemon running") ;
   
    timer=30;
    while (1)
    {
      
        
         manUp();
     
         sysLog("Daemon sleeps") ;
        sleep(timer);
        sysLog("Daemon wakeup") ;
        

    }
}




int mainxx(int argc,char *argv[])
{
	BYTE key;
	USHORT usRet;
	timer=30;
	pid_t pid;
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
		//system("chmod 0777 /home/ap/pub/MTMS/* >/home/ap/pub/MTMS/chmod.txt");
	//system("ls -lR /home/ap/pub >/home/ap/pub/MTMS/ls.txt");
	//return 0;
		//system("ps aux > /home/ap/pub/MTMS/service.txt");
	/*if(daemon(0,1)!=0)
	{
		CTOS_LCDTClearDisplay();
		CTOS_LCDTPrint(strerror(errno));
		CTOS_KBDGet(&key);
		while(1);
		//return 1;
	}*/
	
	//if(pid==0){
	//POSdeamonize();
    //skeleton_daemon();
        
	//manLoadCo	nfig();
	//while(1)
	//{
	  	//CTOS_Sound(1500, 100);
	  	//CTOS_LCDTClearDisplay();
	  	//CTOS_LCDTPrint("what\n");
	  	manUp();
        sprintf(baStr,"timer=%d\n",timer);
        //CTOS_LCDTPrint(baStr);
	  //	sleep(50);
	    //sleep(timer);
	    //CTOS_LCDTPrint("wakeup");
	    //CTOS_LCDTPrintXY(5,5,"end\n");
		//CTOS_KBDGet(&key);
	//}
	//}	
	
	exit(0);
}
