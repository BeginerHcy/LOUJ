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
#define uchar unsigned char//宏定义无符号字符型
#define uint unsigned int  //宏定义无符号整型
	
sbit Reset_Btn = P1^2;			/*单片机1.2引脚检测IN1是否动作*/
sbit Pres_Det = P1^3;			/*单片机1.3引脚检测IN2是否动作*/
sbit INC = P1^4;			/*单片机1.4引脚检测IN3是否动作*/
sbit INF = P1^5;			/*单片机1.5引脚检测IN4是否动作*/

sbit Pump = P3^7;		  /*单片机3.7引脚控制继电器COM1*/
sbit LED_RED = P3^6;		  /*单片机3.6引脚控制继电器COM2*/
sbit LED_GREEN = P3^5;		  /*单片机3.6引脚控制继电器COM2*/
sbit Buzzer = P3^4;		  /*单片机3.6引脚控制继电器COM2*/
sbit LED = P3^2;		  /*单片机3.6引脚控制继电器COM2*/

sfr P1_M0        =   0x92;   //0000,0000 端口1模式寄存器0
sfr P1_M1        =   0x91;   //0000,0000 端口1模式寄存器1

sfr P3_M0        =   0xB2;   //0000,0000 端口3模式寄存器0
sfr P3_M1        =   0xB1;   //0000,0000 端口3模式寄存器1
////////////////////////////////

/********************************************************************
                            初始定义
*********************************************************************/
static uint timeStampus=0,timeStampms,timeStamps[10]=0,cnt[2]=0,ctrlStep=0,status=0,init=1;

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
	timeStampus++;//250us / 1 time
	cnt[0]++;
	if(cnt[0]>=4){
		timeStampms++;
		cnt[0]=0;
	}
	
	cnt[1]++;
	if(cnt[1]>=400){
		timeStamps[0]++;
		timeStamps[1]++;
		timeStamps[2]++;
		timeStamps[3]++;
		timeStamps[4]++;
		cnt[1]=0;
	}
}

/********************************************************************
                            主函数
*********************************************************************/
void main()				   
{

	TMOD=0x02; //定时器工作在方式2
	TH0=0x06; //对TH0 TL0 赋值
	TL0=0x06;
	TR0=1; //开始定时
	ET0=1;
	EA=1;
	
	P1_M1 = 0xFF;	
	P1_M0 = 0x00;

	P3_M1 = 0x00;	
	P3_M0 = 0xFF;
	
	LED = 0;
	Pump = 0;
	LED_RED = 0;
	LED_GREEN = 0;
	Buzzer = 0;
	while(1)
	{	  	 
		if(timeStampms>600){			
			LED = !LED;
			timeStampms = 0;
		}
		
		if(Reset_Btn==1){
			ctrlStep = 0;
			Pump = 0;
			status = 0;
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
				if(timeStamps[0]>15){
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
				//	status = 1;
				}
				break;
				
			case 40:
				if(timeStamps[0]>1200){
					status = 254;
				}
				break;
		}
		
		if(status==255){
			LED_RED = LED;
			LED_GREEN = 0;
			Buzzer = 1;
			
		}
		else if(status==0){
			LED_RED = 0;
			LED_GREEN = 1;
			Buzzer = 0;
		}
		else if(status==1){
			LED_RED = 1;
			LED_GREEN = 1;
			Buzzer = 0;
		}		
		else if(status==254){
			LED_RED = LED;
			LED_GREEN = 0;
			Buzzer = 1;
		}
	}		
}

/********************************************************************
                              结束
*********************************************************************/
