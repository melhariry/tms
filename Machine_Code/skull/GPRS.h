/**
  ******************************************************************************
  * file          communication.h
  * author        
  * component     communication
  * description   header file of communication
  *               communication module provides the Terminal abstraction layer of 
  *               communication with TMS 
  ******************************************************************************
  * copyright POS_GP_TEAM_2016
  *
  ******************************************************************************
  */
/* Define to prevent recursive inclusions ------------------------------------*/


#ifndef GPRS_H
#define GPRS_H







/*========================================*
* I N C L U D E S *
*========================================*/
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctosapi.h>

/*==========================================*
****************P U B L I C ****************
*==========================================*/

/*==========================================*
* D E F I N E S *
*==========================================*/
	

/*========================================*
* D E F I N E S *
*========================================*/
#define PPP_TO_MS 35000 ,//45000ms
#define CONNECT_TO_MS 4000 ,// 5000ms
#define CONNECT_RETRY 2

#define SIZE_SENDBUFF 1024
#define SIZE_RECVBUFF 1024
/*==========================================*
* C O N S T A N T S *
*==========================================*/

/*==========================================*
* T Y P E S *
*==========================================*/
typedef enum
{
	GPRS_ESTABLISHED,
	GPRS_ESTABLISHING,
	GPRS_CONNECTING,
	GPRS_SENDING,
	GPRS_RECEIVING,
	GPRS_DISCONNECTING,
	GPRS_ONHOOKING,
	GPRS_UNKNOWN
} enuGprsStateType;

typedef enum
{
	d_TCP_IO_PROCESSING,//2321h
	d_TCP_IO_BUSY,//2322h
	d_TCP_URL_NOT_FOUND,//2370h
	d_TCP_PING_FAILED,//2371h
	d_TCP_PPP_CONNECTION_TERMINATED,//2372h
	d_TCP_SOCKET_FULL,//2380h
	d_TCP_CONNECTION_NOT_ESTABLISHED,//2381h
	d_TCP_BAD_FCS,//2382h
	d_TCP_PPP_TIMEOUT,//2383h
	d_TCP_PROTOCOL_ERROR,//2384h
	d_TCP_LENGTH_ERROR,//2385h
	d_TCP_PPP_SEND_TIMEOUT,//2386h
	d_TCP_PPP_SEND_ERROR,//2387h
	d_TCP_SOCKET_FAILED,//2388h
	d_TCP_SOCKET_IO_MODE_FAILED,//2389h
	d_TCP_RX_RECEIVE_TIMEOUT,//238Ah
	d_TCP_INVALID_PARA,//238Bh
	d_TCP_CONNECT_TIMEOUT,//2390h
	d_TCP_DISCONNECT_FAIL,//2391h
	d_TCP_RESET,//2392h
	d_TCP_SEQNENCE_INCORRECT,//2393h
	d_TCP_NO_SERVER,//2394h
	d_TCP_RETRANSMISSION,//2395h
	d_TCP_PROTOCOL,//2396h
	d_TCP_IP_FORMAT_WRONG,//2397h
	d_TCP_FORMAT_WRONG,//2398h
	d_TCP_SEND_TIMEOUT,//2399h
	d_TCP_NO_ACK,//239Ah
	d_TCP_BUF_FULL,//239Bh
	d_TCP_RECEIVE_NON_UDP_PACKAGE,//239Ch
	d_TCP_IP_ADDRESS_NOT_EXIST,//239Dh
	d_TCP_RECEIVE_DATA_FAILED,//239Eh
	d_TCP_SEND_DATA_FAILED,//239Fh
	d_TCP_LCP_TIMEOUT,//23A0h
	d_TCP_LCP_ACK,//23A1h
	d_TCP_LCP_NAK,//23A2h
	d_TCP_LCP_REJECT,//23A3h
	d_TCP_LCP_CODE_REJECT,//23A4h
	d_TCP_LCP_PROTO_REJECT,//23A5h
	d_TCP_LCP_TERM_REJECT,//23A6h
	d_TCP_PEER_LCP_ACK,//23A7h
	d_TCP_PEER_LCP_NAK,//23A8h
	d_TCP_PEER_LCP_REJ,//23A9h
	d_TCP_PEER_LCP_CODE_REJECT,//23AAh
	d_TCP_PEER_LCP_PROTO_REJECT,//23ABh
	d_TCP_PEER_LCP_TERM_REQUEST,//23ACh
	d_TCP_PEER_LCP_TERM_ACK,//23ADh
	d_TCP_SEND_EPIPE_FAILED,//23AEh
	d_TCP_RECEIVE_EPIPE_FAILED,//23AFh
	d_TCP_CHAP_TIMEOUT,//23B0h
	d_TCP_CHAP_RESPONSE,//23B1h
	d_TCP_CHAP_CODE_REJECT,//23B2h
	d_TCP_PEER_CHAP_SUCCESS,//23B3h
	d_TCP_PEER_CHAP_AUTH_FAIL,//23B4h
	d_TCP_IPCP_TIMEOUT,//23C0h
	d_TCP_IPCP_ACK,//23C1h
	d_TCP_IPCP_REJECT,//23C2h
	d_TCP_IPCP_CODE_REJECT,//23C3h
	d_TCP_IPCP_PROTO_REJECT,//23C4h
	d_TCP_PEER_IPCP_ACK,//23C5h
	d_TCP_PEER_IPCP_NAK,//23C6h
	d_TCP_IPCP_TOSS,//23C7h
	d_TCP_PEER_IPCP_REJ,//23C8h
	d_TCP_CCP_REJECT,//23CAh
	d_TCP_BIND_FAILED,//23CBh
	d_TCP_CHANNEL_TYPE_FAILED,//23E0h
	d_TCP_CHANNEL_OFFLINE,//23E1h
	d_TCP_GPRS_AUTH_SETTING_FAILED,//23E2h
	d_TCP_GSM_OPEN_FAILED,//23E3h
	d_TCP_GPRS_ATTACH_FAILED,//23E4h
	d_TCP_GPRS_PPP_CONNECT_FAILED,//23E5h
	d_TCP_GPRS_IS_CONNECTED,//23E6h
	d_TCP_USER_INTERRUPT//23FFh
} enuGprsErrorType;
/*==========================================*
* V A R I A B L E S *
*==========================================*/

/*==========================================*
* M A C R O S *
*==========================================*/

/*==========================================*
* F U N C T I O N S*
*==========================================*/
	USHORT gprsOpen();
	USHORT gprsConnect(BYTE *caIP,USHORT usPort);
	USHORT gprsSend(BYTE *baSendBuf,USHORT usSendLen);
	USHORT gprsRecieve(BYTE *baRecvBuf,USHORT *usRecvLen);
	USHORT gprsDisconnect();
	USHORT gprsClose();

/*
	void setHostIP(BYTE *baHOST);
	void GPRS_PARAM_init(void);
	void util_strncpy(BYTE *dest,const BYTE *source,ULONG ulLen);
	USHORT pollStatusResultLCD(BYTE *func, USHORT theRTN);
	USHORT pollStatusResult(DWORD* dwStatus);
	USHORT GPRS_open();
	USHORT GPRS_connect(BYTE *caIP,USHORT usPort);
	USHORT GPRS_send(BYTE *baSendBuf,USHORT usSendLen);
	USHORT GPRS_recieve(BYTE *baRecvBuf,USHORT *usRecvLen);
	USHORT GPRS_disconnect();
	USHORT GPRS_close();
*/
#endif