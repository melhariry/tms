#include "util.h"
 #include <sys/types.h>
 #include <sys/stat.h>
 #include <sys/socket.h>
 #include <sys/un.h>
 #include <netinet/in.h>
 #include <arpa/inet.h>
#include <errno.h>

/*========================================*
* Machine utilities 
*========================================*/
void split(IN BYTE* baStr,BYTE* baDelimiters,OUT BYTE *tokens[],USHORT* usNoToken)
{
	
	USHORT i=0;
	tokens[i]=strtok(baStr,baDelimiters);
	while(tokens[i])
	{
		++i;
		tokens[i]=strtok(NULL,baDelimiters);

	}
	*usNoToken=i;

}
/* ==================================================================
* FUNCTION NAME: unpack
* DESCRIPTION:
* RETURN: none.
* NOTES: none.
* ================================================================ */
void unpack(OUT char* pStr, IN BYTE* pData, IN USHORT Len, IN char* pSpilt)
{
	USHORT i;
	USHORT offset;
	BYTE s_len;
	s_len = strlen(pSpilt);
	for(i = 0; i < Len; ++i)
	{
	offset = (s_len + 2) * i;
	pStr[offset] = ith(pData[i] >> 4);
	pStr[offset+1] = ith(pData[i]);
	if(i < Len - 1)
	{
	memcpy(&pStr[offset+2], pSpilt, s_len);
	}
	}
	pStr[(s_len + 2) * i] = 0;
}

USHORT GetSerialNumber(OUT BYTE * SerialNumber)
{
	BYTE packedSN[17];
	USHORT ret=CTOS_GetSerialNumber(packedSN);
	if(ret!=d_OK)
		return ret;
	unpack(SerialNumber,packedSN,8,"");
	return d_OK;
}

USHORT GetGPRSIP(OUT BYTE * GPRSIP)
{
	BYTE baLocalIP[5];
	USHORT ret=CTOS_TCP_GPRSGetIP(baLocalIP);
	if(ret!=d_OK)
		return ret;
	sprintf(GPRSIP,"%d.%d.%d.%d",baLocalIP[0],baLocalIP[1],baLocalIP[2],baLocalIP[3]);
	return d_OK;
}


char * sock_addr(int s,char *buf,size_t bufsiz) 
{
	char line[100];
    int z;         /* Status return code */
    struct sockaddr_in adr_inet;/* AF_INET */
    int len_inet;  /* length */
/*
 * Obtain the address of the socket:
 */
    len_inet = sizeof adr_inet;
    z = getsockname(s, (struct sockaddr *)&adr_inet, &len_inet);
    if ( z == -1 ) {
    	CTOS_LCDTPrint("\n");
    	strcpy(line,strerror(errno));
    	CTOS_LCDTPrint(line+20);
       return NULL; /* Failed */
    }
/*
 * Convert address into a string
 * form that can be displayed:
 */
    snprintf(buf,bufsiz, "%s:%u",
    inet_ntoa(adr_inet.sin_addr),
    (unsigned)ntohs(adr_inet.sin_port));
    CTOS_LCDTPrint(buf);
    return buf;
 }

USHORT utilGetPort(BYTE socket)
{
	
	USHORT usPort=1,z;

	struct sockaddr addr;
	BYTE line[100];
	USHORT addrLen=sizeof(addr);
	sock_addr(socket,line,&addrLen);
return 233;
	z=getsockname ( socket, (struct sockaddr *)&addr, &addrLen );
	sscanf(addr.sa_data,".*:%d",&usPort);
	/*addr.sa_data[0]+='0';
	addr.sa_data[1]+='0';
	addr.sa_data[2]+='0';
	addr.sa_data[3]+='0';
	addr.sa_data[4]+='0';
	addr.sa_data[5]+='0';
	addr.sa_data[6]+='0';
	addr.sa_data[7]+='0';*/
	//CTOS_LCDTPrint(addr.sa_data);
	//CTOS_LCDTPrint((struct sockaddr_in *)&addr)->sin_port);
	return usPort;
}



/*========================================*
* string utilities s
*========================================*/
void utilPrintI(ULONG x)
{
	char str[20];
	sprintf(str,"%d",x);
	CTOS_LCDTPrint(str);
}

BYTE ith(BYTE hex_digit)
{
	hex_digit &= 0x0F;
	if(hex_digit > 9)
	{
	return hex_digit - 10 + 'A';
	}
	return hex_digit + '0';
}
/* =================================================================
* FUNCTION NAME: IntToStr
* DESCRIPTION: Integer transform string
* RETURN: 1 Byte
* NOTES: none.
* ================================================================ */
BYTE IntToStr(BYTE* buf, DWORD v)
{
	DWORD upp;
	DWORD i;
	upp = 0;
	i = v;
	while(i)
	{
	i /= 10;
	upp++;
	}
	if(!upp)
	{
	upp = 1;
	}
	for(i = 0; i < upp; i++)
	{
	buf[upp-1-i] = (v % 10) + '0';
	v /= 10;
	}
	return upp;
}



//Utility function
void util_strncpy(BYTE *dest,const BYTE *source,ULONG ulLen){
	ULONG i;
	for(i=0;i<ulLen;++i){
		dest[i]=source[i];
	}
	dest[i++]='\r';
	dest[i++]='\n';
	dest[i]='\0';
}