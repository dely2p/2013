
#include "s3c2410.h"
#include "libc.h"

#define C1      523     // Do
#define C1_     554
#define D1      587     // Re
#define D1_     622
#define E1      659     // Mi
#define F1      699     // Pa
#define F1_     740
#define G1      784     // Sol
#define G1_     831
#define A1      880     // La
#define A1_     932
#define B1      988     // Si

#define C2      C1*2    // Do
#define C2_     C1_*2
#define D2      D1*2    // Re
#define D2_     D1_*2
#define E2      E1*2    // Mi
#define F2      F1*2    // Pa
#define F2_     F1_*2
#define G2      G1*2    // Sol
#define G2_     G1_*2
#define A2      A1*2    // La
#define A2_     A1_*2
#define B2      B1*2    // Si

#define DLY_2   DLY_4*2 // 1/2 musical note
#define DLY_4   5000    // 1/4 musical note
#define DLY_8   DLY_4/2 // 1/8 musical note
#define DLY_16  DLY_8/2 // 1/16 musical note

static void DelayForPlay(unsigned short time)	// resolution=0.1ms
{
	 rWTCON=(39<<8)+(3<<3)+(0<<0);	// resolution=0.1ms
     rWTDAT=time+10;			    // Using WDT
     rWTCNT=time+10;
	/* YOUR CODE HERE */
     rWTCON|=(1<<5);

     while(rWTCNT>10);
     rWTCON = 0;
}

static void Play(unsigned long freq)
{
	rTCON=(rTCON&0xfffff0ff);
	/* YOUR CODE HERE */
	rTCNTB1=(PCLK/4)/(freq*2)-1;
	rTCMPB1=rTCNTB1/2;
	rTCON|=(0x02<<8);                   // manual update
	rTCON=(rTCON&0xfffff0ff)|(0x09<<8);	// Interval Mode, Inverter Off, Start
} 

void Buzzer(void) 
{
	int save_BUP, save_B, save_TCFG1;
    
	save_BUP = rGPBUP;
	save_B  = rGPBCON;
	save_TCFG1 = rTCFG1;
 
	/* YOUR CODE HERE */
	rGPBCON = (rGPBCON&~(0x03<<2))|(0x02<<2);	// Config TOUT1
	rGPBUP = rGPBUP & ~(0x2) | 0x2;			// GPB1
	rTCFG1 = (rTCFG1&0xffffff0f)|(1<<4);		// Mux=1/4 for Timer1 
       
	Play(C1);
	DelayForPlay(DLY_4);
	Play(D1);
	DelayForPlay(DLY_4);
	Play(E1);
	DelayForPlay(DLY_4);
	Play(F1);
	DelayForPlay(DLY_4);
	Play(G1);
	DelayForPlay(DLY_4);
	Play(A1);
	DelayForPlay(DLY_4);
	Play(B1);
	DelayForPlay(DLY_4);
	Play(C2);
	DelayForPlay(DLY_4);
    
	rTCON=(rTCON&0xfffff0ff)|(0x00<<8);               // Stop   
	rGPBUP = save_BUP; 
	rGPBCON = save_B;
	rTCFG1 = save_TCFG1;
}

void BuzzerTest(void) 
{
	Uart_Printf("[ Buzzer Test ]\n\n");    
	Buzzer();
	//Uart_Printf("[ Buzzer Test End ]\n\n");
}
