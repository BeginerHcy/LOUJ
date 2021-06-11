/********************************************************************
                ������ƽ���ӿƼ����޹�˾
ʵ�ֹ���:Ӧ�ó���
ʹ��оƬ��STC15W408AS
����11.0592MHZ
�����ʣ�9600
���뻷����Keil 4
���ߣ����´� 
΢��/�ֻ���13603455408
QQ��115451619
��Ʒ�����Ա�   1�꣺https://ourhc.taobao.com
��Ʒ�����Ա�   2�꣺https://g88888.taobao.com
��Ʒ�����Ա���ҵ�꣺https://shop404420384.taobao.com
���������˳��������ѧϰ��ο���������ע����Ȩ��������Ϣ��     
*********************************************************************/
#include<STC15W408AS.h>
#define uchar unsigned char//�궨���޷����ַ���
#define uint unsigned int  //�궨���޷�������
	
sbit Reset_Btn = P1^2;			/*��Ƭ��1.2���ż��IN1�Ƿ���*/
sbit Pres_Det = P1^3;			/*��Ƭ��1.3���ż��IN2�Ƿ���*/
sbit INC = P1^4;			/*��Ƭ��1.4���ż��IN3�Ƿ���*/
sbit INF = P1^5;			/*��Ƭ��1.5���ż��IN4�Ƿ���*/

sbit Pump = P3^7;		  /*��Ƭ��3.7���ſ��Ƽ̵���COM1*/
sbit LED_RED = P3^6;		  /*��Ƭ��3.6���ſ��Ƽ̵���COM2*/
sbit LED_GREEN = P3^5;		  /*��Ƭ��3.6���ſ��Ƽ̵���COM2*/
sbit Buzzer = P3^4;		  /*��Ƭ��3.6���ſ��Ƽ̵���COM2*/
sbit LED = P3^2;		  /*��Ƭ��3.6���ſ��Ƽ̵���COM2*/

sfr P1_M0        =   0x92;   //0000,0000 �˿�1ģʽ�Ĵ���0
sfr P1_M1        =   0x91;   //0000,0000 �˿�1ģʽ�Ĵ���1

sfr P3_M0        =   0xB2;   //0000,0000 �˿�3ģʽ�Ĵ���0
sfr P3_M1        =   0xB1;   //0000,0000 �˿�3ģʽ�Ĵ���1
////////////////////////////////

/********************************************************************
                            ��ʼ����
*********************************************************************/
static uint timeStampus=0,timeStampms,timeStamps[10]=0,cnt[2]=0,ctrlStep=0,status=0,init=1;

/********************************************************************
                            ��ʱ����
*********************************************************************/
void delay(uchar dat)//��ʱ����
{
	uchar m,n,s;
	for(m=dat;m>0;m--)
	for(n=20;n>0;n--)
	for(s=248;s>0;s--);
}
/********************************************************************
                            ��ʱ�жϷ�����
*********************************************************************/
void t0(void) interrupt 1 using 0 //��ʱ�жϷ�����
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
                            ������
*********************************************************************/
void main()				   
{

	TMOD=0x02; //��ʱ�������ڷ�ʽ2
	TH0=0x06; //��TH0 TL0 ��ֵ
	TL0=0x06;
	TR0=1; //��ʼ��ʱ
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
                              ����
*********************************************************************/
