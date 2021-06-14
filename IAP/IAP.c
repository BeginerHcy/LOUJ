#include "IAP.h"


void IapIdle(){//禁用 ISP/IAP／EEPROM functionmake 单片机 在 一 安全 状态
    IAP_CONTR = 0;	   //
	IAP_CMD = 0;	   //
	IAP_TRIG = 0;	   //
	IAP_ADDRH = 0x80;  //
	IAP_ADDRL = 0;	   //
}
uint8 IapReadByte(uint16 addr){//读数据
   uint8 dat;
   IAP_CONTR = ENABLE_IAP;
   IAP_CMD = CMD_READ; 
   IAP_ADDRL = addr;
   IAP_ADDRH = addr >> 8;
   IAP_TRIG = 0x5A;
   IAP_TRIG = 0xA5;
   _nop_();
   dat = IAP_DATA;
   IapIdle();
   return dat;
}
void IapProgramByte(uint16 addr, uint8 dat){//写数据
   IAP_CONTR = ENABLE_IAP;
   IAP_CMD = CMD_PROGRAM;
   IAP_ADDRL = addr;
   IAP_ADDRH = addr >> 8;
   IAP_DATA = dat;
   IAP_TRIG = 0x5A;
   IAP_TRIG = 0xA5;
   _nop_();
   IapIdle();
}
void IapEraseSector(uint16 addr){//擦除数据
   IAP_CONTR = ENABLE_IAP;
   IAP_CMD = CMD_ERASE;
   IAP_ADDRL = addr;
   IAP_ADDRH = addr >> 8;
   IAP_TRIG = 0x5A;
   IAP_TRIG = 0xA5;
   _nop_();
   IapIdle();
}