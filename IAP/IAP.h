#ifndef __STC_IAP_H__
#define __STC_IAP_H__


#include "STC15W408AS.h"
#include "uart.h"
#include<math.h>		  //¼ÆËãÐ¡ÊýµãµÄÍ·ÎÄ¼þ
#include <intrins.h>	  //ÓÃnopº¯ÊýÑÓÊ±µÄÍ·ÎÄ¼þ

#define ENABLE_IAP 0x83 //系统工作时钟<12MHz 时，对IAP_CONTR 寄存器设置此值
#define CMD_IDLE 0
#define CMD_READ 1
#define CMD_PROGRAM 2
#define CMD_ERASE 3
#define ENABLE_IAP 0x83//SYSCLK<12MH
#define IAP_ADDRESS 0x0000	//存储地址


void IapIdle();//禁用 ISP/IAP／EEPROM functionmake 单片机 在 一 安全 状态
uint8 IapReadByte(uint16 addr);//读数据
void IapProgramByte(uint16 addr, uint8 dat);//写数据
void IapEraseSector(uint16 addr);//擦除数据


#endif