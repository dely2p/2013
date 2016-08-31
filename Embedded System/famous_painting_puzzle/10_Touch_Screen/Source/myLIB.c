#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include "S3C2410.h"
#include "myLIB.h"
#include "MMU.h"
extern unsigned char g2;
extern int vsprintf(char*,char*,va_list);
char string[256];

void ChangeClockDivider(int hdivn,int pdivn)
{
     // hdivn,pdivn FCLK:HCLK:PCLK
     //     0,0         1:1:1 
     //     0,1         1:1:2 
     //     1,0         1:2:2
     //     1,1         1:2:4
}

void ChangeMPllValue(int mdiv,int pdiv,int sdiv)
{
    rMPLLCON = (mdiv<<12) | (pdiv<<4) | sdiv;
}

void ChangeUPllValue(int mdiv,int pdiv,int sdiv)
{
    rUPLLCON = (mdiv<<12) | (pdiv<<4) | sdiv;
}


void Uart_Init(void)
{
   rUFCON0=0x00;     	// FIFO Disable
   rUMCON0=0x00;    
   
   rULCON0=0x03;     	// Normal,No parity,1 stop,8 bit
   rUCON0=0x245;    	// rx=edge,tx=level,disable timeout int.,enable rx error int.,normal,interrupt or polling
   rUBRDIV0=(int)(266000000/4/16/115200+0.5-1);		// 266/4MHz, 115200bps
}    


char Uart_GetCh_k(void)
{
   while(!(rUTRSTAT0 & 0x1)); 			// Receive data read
   return rURXH0;
}


char Uart_GetKey(void)
{
   if(!(rUTRSTAT0 & 0x1)) return 0;			// Receive data read
   else return rURXH0;
}


void Uart_PutCh(int data)
{
   rUTXH0=data;	
   while(!(rUTRSTAT0 & 0x4)); 			// Wait until THR is empty.
}	


void Uart_PutStr(char *pt)
{
   while(*pt)
      Uart_PutCh(*pt++);
}


void Uart_Printf(char *fmt,...)
{
   va_list ap;
   char string[256];
   
   va_start(ap,fmt);
   vsprintf(string,fmt,ap);
   Uart_PutStr(string);
   va_end(ap);
}


void Delay_1(unsigned short time)				// resolution=0.1ms
{
	unsigned int i;

	for(;time>0;time--)
		for(i=0;i<100;i++);
}	


int  random(int val)
{
   return rand()%val; 
}	

void Delay(unsigned short time)				// resolution=0.1ms
{
	unsigned int i;

   i=rWTCON;
   i=rWTDAT;
   rWTCON=(39<<8)+(3<<3)+(0<<0);			// resolution=0.1ms 
   rWTDAT=time+10;					// Using WDT
   rWTCNT=time+10;     
   rWTCON|=(1<<5);                                                      
   
   while(rWTCNT>10);     
}

	
/*#define _MMUTT_STARTADDRESS     0x30ff8000
    
void MMU_Init(void)
{
    int i,j;
    //========================== IMPORTANT NOTE =========================
    //The current stack and code area can't be re-mapped in this routine.
    //If you want memory map mapped freely, your own sophiscated MMU
    //initialization code is needed.
    //===================================================================

    MMU_DisableDCache();
    MMU_DisableICache();

    //If write-back is used,the DCache should be cleared.
    for(i=0;i<64;i++)
    	for(j=0;j<8;j++)
    	    MMU_CleanInvalidateDCacheIndex((i<<26)|(j<<5));
    MMU_InvalidateICache();
    
    #if 0
    //To complete MMU_Init() fast, Icache may be turned on here.
    MMU_EnableICache(); 
    #endif
    
    MMU_DisableMMU();
    MMU_InvalidateTLB();

    //MMU_SetMTT(int vaddrStart,int vaddrEnd,int paddrStart,int attr)
    MMU_SetMTT(0x00000000,0x07f00000,0x00000000,RW_CNB);  //bank0
    MMU_SetMTT(0x08000000,0x0ff00000,0x08000000,RW_CNB);  //bank1
    MMU_SetMTT(0x10000000,0x17f00000,0x10000000,RW_NCNB); //bank2
    MMU_SetMTT(0x18000000,0x1ff00000,0x18000000,RW_NCNB); //bank3
    MMU_SetMTT(0x20000000,0x27f00000,0x20000000,RW_NCNB); //bank4
    MMU_SetMTT(0x28000000,0x2ff00000,0x28000000,RW_NCNB); //bank5
    MMU_SetMTT(0x30000000,0x30f00000,0x30000000,RW_CNB);  //bank6-1
    MMU_SetMTT(0x31000000,0x33e00000,0x31000000,RW_NCNB); //bank6-2
    MMU_SetMTT(0x33f00000,0x33f00000,0x33f00000,RW_CB);   //bank6-3
    
    MMU_SetMTT(0x38000000,0x3ff00000,0x88000000,RW_NCNB); //bank7
    
    MMU_SetMTT(0x40000000,0x5af00000,0x40000000,RW_NCNB);//SFR+StepSram    
    MMU_SetMTT(0x5b000000,0xfff00000,0x5b000000,RW_FAULT);//not used

    MMU_SetTTBase(_MMUTT_STARTADDRESS);
    MMU_SetDomain(0x55555550|DOMAIN1_ATTR|DOMAIN0_ATTR); 
    	//DOMAIN1: no_access, DOMAIN0,2~15=client(AP is checked)
    MMU_SetProcessId(0x0);
    MMU_EnableAlignFault();
    	
    MMU_EnableMMU();
    MMU_EnableICache();
    MMU_EnableDCache(); //DCache should be turned on after MMU is turned on.
}    


// attr=RW_CB,RW_CNB,RW_NCNB,RW_FAULT
void ChangeRomCacheStatus(int attr)
{
    int i,j;
    MMU_DisableDCache();
    MMU_DisableICache();
    //If write-back is used,the DCache should be cleared.
    for(i=0;i<64;i++)
    	for(j=0;j<8;j++)
    	    MMU_CleanInvalidateDCacheIndex((i<<26)|(j<<5));
    MMU_InvalidateICache();
    MMU_DisableMMU();
    MMU_InvalidateTLB();
    MMU_SetMTT(0x00000000,0x07f00000,0x00000000,attr);	//bank0
    MMU_SetMTT(0x08000000,0x0ff00000,0x08000000,attr);	//bank1
    MMU_EnableMMU();
    MMU_EnableICache();
    MMU_EnableDCache();
}    
    

void MMU_SetMTT(int vaddrStart,int vaddrEnd,int paddrStart,int attr)
{
    U32 *pTT;
    int i,nSec;
    pTT=(U32 *)_MMUTT_STARTADDRESS+(vaddrStart>>20);
    nSec=(vaddrEnd>>20)-(vaddrStart>>20);
    for(i=0;i<=nSec;i++)*pTT++=attr |(((paddrStart>>20)+i)<<20);
}    

*/
