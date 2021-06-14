#ifndef __STC_UART_H__
#define __STC_UART_H__

#include "STC15W408AS.h"
#include<math.h>
#include <intrins.h>
#include <string.h>
/********************************************************************
                            定义特殊寄存器
*********************************************************************/

sbit RXB=P3^0;
sbit TXB=P3^1;
sbit RS485_DIR=P3^2;

/*数据类型定义*/
typedef signed char	int8;
typedef signed int	int16;
typedef signed long	int32;
typedef unsigned char	uint8;
typedef unsigned int	uint16;
typedef unsigned long	uint32;
#define uchar unsigned char
#define uint unsigned int

#define COIL_NUM  				11
#define COIL_BASE 				0x0000
#define DI_NUM  					4
#define DI_BASE 					0x0000

#define SLAVE_REG_BASE  	0x0000
#define SLAVE_REG_NUM   	2

#define SLAVE_INREG_BASE  0x0000
#define SLAVE_INREG_NUM   2

#define RSP_OK						0		
#define RSP_ERR_CMD				0x01	
#define RSP_ERR_REG_ADDR	0x02	
#define RSP_ERR_VALUE			0x03	
#define RSP_ERR_WRITE			0x04	


#define S_RX_BUF_SIZE			20
#define S_TX_BUF_SIZE			20

typedef struct
{
	uint8 RxBuf[S_RX_BUF_SIZE];
	uint8 RxCount;
	uint8 RxStatus;
	uint8 RxNewFlag;

	uint8 RspCode;

	uint8 TxBuf[S_TX_BUF_SIZE];
	uint8 TxCount;
}MODS_T;

typedef struct
{

	uint16 InputRegister[SLAVE_INREG_NUM];
	
	int8 Coils[COIL_NUM];
	int8 Inputs[DI_NUM];
	
}VAR_T;

extern MODS_T g_tModS;
extern VAR_T g_tVar;
extern uint8 TBUF,RBUF;
extern uint8 TDAT,RDAT;
extern uint8 TCNT,RCNT;
extern uint8 TBIT,RBIT;
extern bit TING,RING;
extern bit TEND,REND;
extern uint8 TCNT,RCNT;

void ConfigUART1();
void senduart2();
void delay(uchar dat);//延时程序
void clear_receBuf();

extern uint8 add;    //µôµç±£³Ö485µÄµØÖ·
extern uint8 j;
extern uint8 dat;    

extern bit flag_zx;
extern bit bz1;
extern bit bz2;
extern bit bz3;
extern bit bz4;
extern bit bz5;
extern bit bz6;
extern bit bz7;
extern bit bz8;

extern uint8 sendPosi;     //·¢ËÍ»º³åÇøÖ¸Õë
extern uint8 receCount;    //½ÓÊÕ»º³åÇøÖ¸Õë
extern uint8 sendCount;    //ÐèÒª·¢ËÍÊý¾Ý¸öÊý
extern uint8 receTimeOut;  //Í¨Ñ¶³¬Ê±Öµ
extern uint8 byteCount;    //·¢ËÍ¸öÊý

extern uint8 sendBuf[10];
extern uint8 receBuf[10];

void UART_CYC();
void MODS_01H(void);
void MODS_02H(void);
void MODS_0FH(void);
uint16 BEBufToUint16(uint8 *_pBuf);
uint16 CRC16_Modbus(uint8 *_pBuf, uint16 _usLen);
void MODS_SendWithCRC(uint8 *_pBuf, uint8 _ucLen);
void RS485_SendBuf(uint8 *_ucaBuf, uint16 _usLen);
void MODS_SendAckOk(void);
void MODS_05H(void);
void MODS_SendAckErr(uint8 _ucErrCode);	
#endif