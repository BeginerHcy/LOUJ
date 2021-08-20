/********************************************************************
                河南钰平电子科技有限公司
实现功能:应用程序
使用芯片：STC15W408AS
晶振：11.0592MHZ
波特率：9600
编译环境：Keil 4
作者：张新春 
微信/手机：13603455408
QQ：115451619
产品有售淘宝   1店：https://ourhc.taobao.com
产品有售淘宝   2店：https://g88888.taobao.com
产品有售淘宝企业店：https://shop404420384.taobao.com
【声明】此程序仅用于学习与参考，引用请注明版权和作者信息！     
*********************************************************************/
#include<STC15W408AS.h>
#include "IAP.h"
#include "uart.h"

////////////////////////////////////////////////////////////////	
//#define DAKE2
//#define DAKE
#define XIAOKE
////////////////////////////////////////////////////////////////
sbit Reset_Btn 	= P1^2;			/*单片机1.2引脚检测IN1是否动作*/
sbit Pres_Det 	= P1^3;			/*单片机1.3引脚检测IN2是否动作*/
sbit INC 				= P1^4;			/*单片机1.4引脚检测IN3是否动作*/
sbit INF 				= P1^5;			/*单片机1.5引脚检测IN4是否动作*/
////////////////////////////////////////////////////////////////
sbit Pump 			= P3^7;		  /*单片机3.7引脚控制继电器COM1*/
sbit LED_RED 		= P3^6;		  /*单片机3.6引脚控制继电器COM2*/
sbit LED_GREEN 	= P3^5;		  /*单片机3.6引脚控制继电器COM2*/
sbit Buzzer 		= P3^4;		  /*单片机3.6引脚控制继电器COM2*/
sbit LED 				= P3^2;		  /*单片机3.6引脚控制继电器COM2*/
////////////////////////////////////////////////////////////////
sfr P1_M0       =   0x92;   //0000,0000 端口1模式寄存器0
sfr P1_M1       =   0x91;   //0000,0000 端口1模式寄存器1
////////////////////////////////////////////////////////////////
sfr P3_M0       =   0xB2;   //0000,0000 端口3模式寄存器0
sfr P3_M1       =   0xB1;   //0000,0000 端口3模式寄存器1
////////////////////////////////////////////////////////////////
#define internalLED 200 
#define PUMP_TIME 10
////////////////////////////////////////////////////////////////
/********************************************************************
                            初始定义
*********************************************************************/
static uint timeStampus=0,timeStampms,timeStamps[10]=0,cnt[2]=0,ctrlStep=0,status=0,init=1,t2Flash=0,t3Flash=0,t1Flash=0,t4Flash=0;
static uint flashLED[4]={0};
uint8 t,r,ii;		  //定义数据类型
uint8 but[16];	  //数据存储数组
int8 n,i;
uint16 crc1;
uint8 crcHL[2]={0};
/********************************************************************
                            延时函数
*********************************************************************/
void delay(uchar dat)//延时程序
{
	uchar m,n,s;
	for(m=dat;m>0;m--)
	for(n=20;n>0;n--)
	for(s=248;s>0;s--);
}
/********************************************************************
                            定时中断服务函数
*********************************************************************/
void t0(void) interrupt 1 using 0 //定时中断服务函数
{
	
	UART_CYC();
	
	timeStampus++;//250us / 1 time
	cnt[0]++;
	if(cnt[0]>=30){
		timeStampms++;
		t1Flash++;
		t2Flash++;
		t3Flash++;
		t4Flash++;
		cnt[0]=0;
	}
	
	cnt[1]++;
	if(cnt[1]>=3000){
		timeStamps[0]++;
		timeStamps[1]++;
		timeStamps[2]++;
		timeStamps[3]++;
		timeStamps[4]++;
		cnt[1]=0;
	}
}

void clear_receBuf(){
  uint8 i;
	for(i=0;i<10;i++){
	    g_tModS.RxBuf[i]=0;
	}
}

/********************************************************************
                            主函数
*********************************************************************/
void main()				   
{
	delay(10);
	////////////////////////////////////////////////////////////////
	clear_receBuf();
	////////////////////////////////////////////////////////////////
	add = IapReadByte(0x01);//读取存储的值
	if(add==0xff) add=0x01;
	////////////////////////////////////////////////////////////////
	ConfigUART1();
	////////////////////////////////////////////////////////////////
	AUXR	= 0X80; //1-250 * 1us   12-384 * 1 / 12 = 32us
	TMOD	= 0x00; //定时器工作在方式0
	TL0		= 0x80;	   
	TH0		= 0xFE;   
	TR0		= 1;     //开始定时
	ET0		= 1;
	EA		= 1;
	////////////////////////////////////////////////////////////////
	P1_M1 = 0xFF;	
	P1_M0 = 0x00;

	P3_M1 = 0x00;	
	P3_M0 = 0xFC;
	////////////////////////////////////////////////////////////////
	LED 	= 0;
	Pump 	= 0;
	LED_RED 	= 0;
	LED_GREEN = 0;
	Buzzer 		= 0;
	////////////////////////////////////////////////////////////////

	
	while(1)
	{	 
		
		if(REND)
		{ 
			REND=0; 
			g_tModS.RxBuf[r++&0x0F]=RBUF;
			if(r>=21) 
				r=0;
			t4Flash = 0;
		}
		if(r>0 && t4Flash>2){
			flag_zx = 1;
			g_tModS.RxCount = r;
			r = 0;
		}
		if(flag_zx==1){   
			
			flag_zx = 0;
			
			if(g_tModS.RxCount < 4)
				goto err_ret;
			
			for (n = 0; n<g_tModS.RxCount-2; n++) {
				crc1 = g_tModS.RxBuf[n] ^ crc1;
				for (i = 0; i < 8; i++) {
					if (crc1 & 0x01) {
						crc1 = crc1 >> 1;
						crc1 = crc1 ^ 0xA001;
					}
					else {
						crc1 = crc1 >> 1;
					}
				}
			}
			crcHL[0] = crc1;
			crcHL[1] = crc1 >> 8;
			
			if (crcHL[0]==g_tModS.RxBuf[g_tModS.RxCount-2] &&
				crcHL[1]==g_tModS.RxBuf[g_tModS.RxCount-1]){
				goto err_ret;
			}
			
			if(g_tModS.RxBuf[0]==0x01){
				
				switch(g_tModS.RxBuf[1]){
					
					case 0x01:					
						g_tVar.Coils[0] = Pump;
						g_tVar.Coils[1] = LED_RED;
						g_tVar.Coils[2] = LED_GREEN;
						g_tVar.Coils[3] = Buzzer;
						g_tVar.Coils[4] = LED;
					
						MODS_01H();
						break;
					
					case 0x02:
						g_tVar.Inputs[0] = Reset_Btn;
						g_tVar.Inputs[1] = Pres_Det;
						g_tVar.Inputs[2] = INC;
						g_tVar.Inputs[3] = INF;
						MODS_02H();
						break;
					
					case 0x05:
						MODS_05H();
						
						break;
					
					case 0x0F:
						MODS_0FH();
						break;
					
					default:
						g_tModS.RspCode = RSP_ERR_CMD;
						MODS_SendAckErr(g_tModS.RspCode);	/* 告诉主机命令错误 */
						break;
				
				}
			}
			err_ret:
				g_tModS.RxCount = 0;
			 
		}
		////////////////////////////////////////////////////////////////
		if(timeStampms>internalLED / 2 * 5){			
			flashLED[0] = 1-flashLED[0];
			timeStampms = 0;
		}
		if(t1Flash<internalLED)	flashLED[1] = 1;
		else if(t1Flash>internalLED*1 && t1Flash<internalLED*2)flashLED[1] = 0;
		else if(t1Flash>internalLED*2 && t1Flash<internalLED*3)flashLED[1] = 1;
		else if(t1Flash>internalLED*3 && t1Flash<internalLED*4)flashLED[1] = 0;
		else if(t1Flash>internalLED*10)t1Flash = 0;
		
		if(t2Flash<internalLED)	flashLED[2] = 1;
		else if(t2Flash>internalLED*1 && t2Flash<internalLED*2)flashLED[2] = 0;
		else if(t2Flash>internalLED*2 && t2Flash<internalLED*3)flashLED[2] = 1;
		else if(t2Flash>internalLED*3 && t2Flash<internalLED*4)flashLED[2] = 0;
		else if(t2Flash>internalLED*4 && t2Flash<internalLED*5)flashLED[2] = 1;
		else if(t2Flash>internalLED*5 && t2Flash<internalLED*6)flashLED[2] = 0;
		else if(t2Flash>internalLED*10)t2Flash = 0;
		
		if(t3Flash<internalLED)	flashLED[3] = 1;
		else if(t3Flash>internalLED*1 && t3Flash<internalLED*2)flashLED[3] = 0;
		else if(t3Flash>internalLED*2 && t3Flash<internalLED*3)flashLED[3] = 1;
		else if(t3Flash>internalLED*3 && t3Flash<internalLED*4)flashLED[3] = 0;
		else if(t3Flash>internalLED*4 && t3Flash<internalLED*5)flashLED[3] = 1;
		else if(t3Flash>internalLED*5 && t3Flash<internalLED*6)flashLED[3] = 0;
		else if(t3Flash>internalLED*6 && t3Flash<internalLED*7)flashLED[3] = 1;
		else if(t3Flash>internalLED*7 && t3Flash<internalLED*8)flashLED[3] = 0;
		else if(t3Flash>internalLED*10)t3Flash = 0;
		////////////////////////////////////////////////////////////////
		
		////////////////////////////////////////////////////////////////
		if(Reset_Btn==1 || g_tVar.Coils[5]==1){
			ctrlStep = 0;
			Pump = 0;
			status = 0;
			g_tVar.Coils[5] = 0;
		}
		if(Pres_Det==1){
			status = 255;
			ctrlStep = 40;
			Pump = 0;
		}
		switch(ctrlStep){
		
			case 0:
				if(Pres_Det==0){
					ctrlStep = 10;
					timeStamps[0] = 0;				
				}
				else{
					status = 255;
					ctrlStep = 40;
				}
				break;
			
			case 10:
				status = 1;
				Pump = 1;
				if(timeStamps[0]>PUMP_TIME){
					ctrlStep = 20;
					timeStamps[0] = 0;
					Pump = 0;
					status = 0;
				}
				break;
				
			case 20:
				if(Pres_Det==1){
					ctrlStep = 40;
					status = 255;
				}
				else{
					ctrlStep = 30;
					status = 0;
				}
				break;
				
			case 30:
				if(timeStamps[0]>300){
					ctrlStep = 10;
					timeStamps[0] = 0;
				}
				break;
				
			case 40:
				if(timeStamps[0]>1200){
					status = 254;
				}
				break;
		}
		
#ifdef XIAOKE
		
		if(status==255){
			LED_RED = (flashLED[0]==1);
			LED_GREEN = 0;
			Buzzer = 1;
			LED = (flashLED[2]==1);
		}
		else if(status==0){
			LED_RED = 0;
			LED_GREEN = 1;
			Buzzer = 0;
			LED = (flashLED[0]==1);
		}
		else if(status==1){
			LED_RED = 1;
			LED_GREEN = 1;
			Buzzer = 0;
			LED = (flashLED[1]==1);
		}		
		else if(status==254){
			LED_RED = (flashLED[0]==1);
			LED_GREEN = 0;
			Buzzer = 1;
			LED = (flashLED[3]==1);
		}
		
#endif
		
#ifdef DAKE
		
		if(status==255){
			LED_RED = (flashLED[0]==1);
			LED_GREEN = 1;
			Buzzer = 0;
			LED = (flashLED[2]==1);
		}
		else if(status==0){
			LED_RED = 0;
			LED_GREEN = 0;
			Buzzer = 0;
			LED = (flashLED[0]==1);
		}
		else if(status==1){
			LED_RED = 0;
			LED_GREEN = 0;
			Buzzer = 0;
			LED = (flashLED[1]==1);
		}		
		else if(status==254){
			LED_RED = (flashLED[0]==1);
			LED_GREEN = 1;
			Buzzer = 1;
			LED = (flashLED[3]==1);
		}
		
#endif		
		
#ifdef DAKE2
		
		if(status==255){
			LED_RED = (flashLED[0]==1);
			LED_GREEN = 1;
			Buzzer = 1;
			LED = (flashLED[2]==1);
		}
		else if(status==0){
			LED_RED = 0;
			LED_GREEN = 0;
			Buzzer = 0;
			LED = (flashLED[0]==1);
		}
		else if(status==1){
			LED_RED = 0;
			LED_GREEN = 0;
			Buzzer = 0;
			LED = (flashLED[1]==1);
		}		
		else if(status==254){
			LED_RED = (flashLED[0]==1);
			LED_GREEN = 1;
			Buzzer = 1;
			LED = (flashLED[3]==1);
		}
		
#endif	

	}		
	////////////////////////////////////////////////////////////////
}

/********************************************************************
                              结束
*********************************************************************/
