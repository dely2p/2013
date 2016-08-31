// include --------------------------------------------------------------
#include "S3C2410.h"
#include "myLIB.h"
#include "2410slib.h"
#include "2410addr.h"
#include "image/ma1.h"
#include "image/ma2.h"
#include "image/ma3.h"
#include "image/ma4.h"
#include "image/ma5.h"
#include "image/ma6.h"
#include <stdlib.h>
#include <time.h>

#include "image/button.h"
#include "image/button_background.h"

// define ---------------------------------------------------------------
#define LCD_RIGHT_BOUNDARY 	480
#define LCD_LEFT_BOUNDARY  	0
#define LCD_UP_BOUNDARY    	0
#define LCD_DOWN_BOUNDARY  	272
#define GRID  		   		5
#define GRID_CENTER    		GRID/2
#define TABLE_X     		LCD_RIGHT_BOUNDARY/GRID
#define TABLE_Y     		LCD_DOWN_BOUNDARY/GRID
//
#define ADCPRS 39
#define C1      523     // 도
#define D1      587     // 레
#define E1      659     // 미
#define F1      699     // 파
#define G1      784     // 솔
#define A1      880     // 라
#define B1      988     // 시
#define C2      C1*2    // 도
#define D2      D1*2    // 레
#define E2      E1*2    // 미
#define F2      F1*2    // 파
#define G2      G1*2    // 솔
#define A2      A1*2    // 라
#define B2      C1*2    // 시
#define END		0


#define DLY_16   5000/4    // 1/16 musical note
#define DLY_8	 5000/2	   // 1/8  musical note
#define DLY_4    5000      // 1/4  musical note
#define DLY_2    5000*2    // 1/2  musical note


// external function ----------------------------------------------------
extern void Delay_1(int);
extern void Init_LCD(void);
extern void PutPixel(unsigned int x,unsigned int y,unsigned int c);
extern void DrawLine(int x1,int y1,int x2,int y2,int color);

// external variable ----------------------------------------------------
extern int whichUart;
extern int (*frameBuffer)[LCD_XSIZE_TFT/2];

// function -------------------------------------------------------------
void Wait(unsigned int count);
void DrawRectangle(int x1, int y1, int x2, int y2, unsigned short color);
void DrawCircle(int x1, int y1,int r, unsigned short color);
void Init_TS(void);
void __irq Adc_or_TsAuto(void);
void random_f(void);

void MakeTable(void);
void calibrate(void);
void fallingB(void);
// global variable ------------------------------------------------------
unsigned char keycode[128];	// 키보드 버퍼
int i,j,k = 0;
volatile int ADC_x, ADC_y;
int randNum[7]={0,2,5,4,1,6,3};
int nx1=0,nx2=80,nx3=160,nx4=0,nx5=80,nx6=160;
int ny1=0,ny2=0,ny3=0,ny4=136,ny5=136,ny6=136;
int p1,p2,p3,p4,p5,p6;
int check=0;
int check_pic1,check_pic2;
int pic_n;
int tmp;


// calibrate variable ---------------------------------------------------
int tx[6];
int ty[6];
int fx[3] = {-2, 1, 3};							// x좌표 보정값
int fy[3] = {-1, 1, 4};							// y좌표 보정값

int cal_x[TABLE_X+1];							// x좌표 테이블 
int cal_y[TABLE_Y+1];							// y좌표 테이블 

int TP_X1 = TABLE_X/10;							// LCD 1/10지점 x좌표
int TP_Y1 = TABLE_Y/10;							// LCD 1/10지점 y좌표
int TP_X2 = TABLE_X-TABLE_X/10;					// LCD 9/10지점 x좌표
int TP_Y2 = TABLE_Y-TABLE_Y/10;					// LCD 9/10지점 y좌표
int TP_CX = TABLE_X/2;							// LCD center 	x좌표
int TP_CY = TABLE_Y/2;							// LCD center 	y좌표
int cal_cnt = 0;
int color =0x480c;
int plus=1;
int num=0;
int x_p=50,y_p=10;
void Delay_1(int);
unsigned char g2;	// Delay (myLIB.h)
static void Play(unsigned long freq)
{
	rTCON=(rTCON&0xfffff0ff);

	rTCNTB1=(PCLK/4)/(freq*2)-1;
	rTCMPB1=rTCNTB1/2;
	rTCON|=(0x02<<8);							// updade TCNTB1 & TCMPB1
	rTCON=(rTCON&0xfffff0ff)|(0x09<<8);			// Interval Mode(auto reload), Inverter Off, Start
} 

static void DelayForPlay(unsigned short time)	// resolution=0.1ms
{
	rWTCON=(39<<8)+(3<<3)+(0<<0);				// resolution=0.1ms
    rWTDAT=time+10;								// Using WDT
    rWTCNT=time+10;
    	
    rWTCON|=(1<<5);

    while(rWTCNT>10);
    rWTCON = 0;
}
int Buzzer(int cnt) 
{	
	int save_BUP, save_B, save_TCFG1;
	save_BUP = rGPBUP;
	save_B  = rGPBCON;
	save_TCFG1 = rTCFG1;
 
	rGPBCON = (rGPBCON&~(0x03<<2))|(0x02<<2);	// Config TOUT1
	rGPBUP = rGPBUP & ~(0x2) | 0x2;				// GPB1
	rTCFG1 = (rTCFG1&0xffffff0f)|(1<<4);		// Mux=1/4 for Timer1
//-------------------------------------------------------------------
		switch(cnt){
		
			case 1:
				Play(C1);
				DelayForPlay(DLY_8);
			
				break;
			case 2:
				Play(D1);
				DelayForPlay(DLY_8);
			
				break;
			case 3:
				Play(E1);
				DelayForPlay(DLY_8);
			
				break;
			case 4:
				Play(F1);
				DelayForPlay(DLY_8);
			
				break;
			case 5:
				Play(G1);
				DelayForPlay(DLY_8);
			
				break;
			case 6:
				Play(A1);
				DelayForPlay(DLY_8);
			
				break;
			case 7:
				Play(B1);
				DelayForPlay(DLY_8);
				
				break;
			case 8:
				Play(C2);
				DelayForPlay(DLY_8);	
			
				break;
			default:	
				break;	
				
	
}


//-------------------------------------------------------------------						
	rTCON=(rTCON&0xfffff0ff)|(0x00<<8);          // Stop   
	rGPBUP = save_BUP; 
	rGPBCON = save_B;
	rTCFG1 = save_TCFG1;
	
	return 1;
}
// main routine ---------------------------------------------------------      
int Main(void)
{	
	unsigned int TimeSec,TimeMin,TimeHour;
	int PreSec;
	int RandomValue;
	int TickTime;
	int flag=0;
	int x,y,temp;
	int i=0, j=0, check = 0;
	TimeSec = TimeMin = TimeHour = 0;
	PreSec = -1;
	

   	ChangeClockDivider(1,1);        			// 1:2:4 HCLK = 133MHz, PCLK = 66.5MHz
   	ChangeMPllValue(0x7d,0x1,0x1);  			// FCLK=266MHz 
   	Delay(100);					  				// 10ms 
   	whichUart=0;
   	Uart_Init();
   	Uart_Printf("Hello HRP-SC2410M(IMO)Board \n");
  
   	
   	// Start user main function ----------------------------------------------
	rGPGCON =(rGPGCON & ~(0xf<<18));
	Init_LCD();
	Init_TS();
	
	//RTC INIT
	rRTCCON = ((rRTCCON & ~0x1) | 0x1);
	rRTCCON = ((rRTCCON & ~0x8) | 0x8);
	rRTCCON = (rRTCCON & ~0x8);
	
	//RTC RESET	
	rRTCRST = ((rRTCRST & ~0xF) | 0xD);
	
	//RTC TIME SET
	rBCDSEC = (rBCDSEC & ~0x7F);
	rBCDMIN = ((rBCDMIN & ~0x7F));
	rBCDHOUR = (rBCDHOUR & ~0x3F);	
	
	
while(1)
{
	for(x=nx1;x<nx1+80;x++)
		for(y=ny1;y<ny1+136;y++)
			frameBuffer[y][x]=ma1[(y-ny1)*240+(x-nx1)];	
			
	for(x=nx2;x<nx2+80;x++)
		for(y=ny2;y<ny2+136;y++)
			frameBuffer[y][x]=ma2[(y-ny2)*240+(x-nx2)];
			
	for(x=nx3;x<nx3+80;x++)
		for(y=ny3;y<ny3+136;y++)
			frameBuffer[y][x]=ma3[(y-ny3)*240+(x-nx3)];
			
	for(x=nx4;x<nx4+80;x++)
		for(y=ny4;y<ny4+136;y++)
			frameBuffer[y][x]=ma4[(y-ny4)*240+(x-nx4)];
			
	for(x=nx5;x<nx5+80;x++)
		for(y=ny5;y<ny5+136;y++)
			frameBuffer[y][x]=ma5[(y-ny5)*240+(x-nx5)];
			
	for(x=nx6;x<nx6+80;x++)
		for(y=ny6;y<ny6+136;y++)
			frameBuffer[y][x]=ma6[(y-ny6)*240+(x-nx6)];
	
//Uart_Printf("%d %d %d %d %d %d\n",randNum[1],randNum[2],randNum[3],randNum[4],randNum[5],randNum[6]);
		for(i=1;i<=6;i++){
			temp = randNum[i];
			Uart_Printf("%d ",temp);
			if(i==1){ //첫째칸
				switch(temp){
					case 1:
						nx1=0;
						ny1=0;
						break;
					case 2:
						nx1=80;
						ny1=0;
						Uart_Printf("%d ",temp);
						break;
					case 3:
						nx1=160;
						ny1=0;
						break;
					case 4:
						nx1=0;
						ny1=136;
						break;
					case 5:
						nx1=80;
						ny1=136;
						break;
					case 6:
						nx1=160;
						ny1=136;
						break;
					default:
						break;
							
				}
				p1=temp;
			}
			else if(i==2){
			
				switch(temp){
					case 1:
						nx2=0;
						ny2=0;
						break;
					case 2:
						nx2=80;
						ny2=0;
						break;
					case 3:
						nx2=160;
						ny2=0;
						break;
					case 4:
						nx2=0;
						ny2=136;
						break;
					case 5:
						nx2=80;
						ny2=136;
						break;
					case 6:
						nx2=160;
						ny2=136;
						break;
					default:
						break;
							
				}
				p2=temp;
			}
			else if(i==3){
			
				switch(temp){
					case 1:
						nx3=0;
						ny3=0;
						break;
					case 2:
						nx3=80;
						ny3=0;
						break;
					case 3:
						nx3=160;
						ny3=0;
						break;
					case 4:
						nx3=0;
						ny3=136;
						break;
					case 5:
						nx3=80;
						ny3=136;
						break;
					case 6:
						nx3=160;
						ny3=136;
						break;
					default:
						break;
							
				}
				p3=temp;
			}
			else if(i==4){
				switch(temp){
					case 1:
						nx4=0;
						ny4=0;
						break;
					case 2:
						nx4=80;
						ny4=0;
						break;
					case 3:
						nx4=160;
						ny4=0;
						break;
					case 4:
						nx4=0;
						ny4=136;
						break;
					case 5:
						nx4=80;
						ny4=136;
						break;
					case 6:
						nx4=160;
						ny4=136;
						break;
					default:
						break;
							
				}
				p4=temp;
			}
			else if(i==5){
				switch(temp){
					case 1:
						nx5=0;
						ny5=0;
						break;
					case 2:
						nx5=80;
						ny5=0;
						break;
					case 3:
						nx5=160;
						ny5=0;
						break;
					case 4:
						nx5=0;
						ny5=136;
						break;
					case 5:
						nx5=80;
						ny5=136;
						break;
					case 6:
						nx5=160;
						ny5=136;
						break;
					default:
						break;
						
				}
				p5=temp;
			}
			else if(i==6){
				switch(temp){
					case 1:
						nx6=0;
						ny6=0;
						break;
					case 2:
						nx6=80;
						ny6=0;
						break;
					case 3:
						nx6=160;
						ny6=0;
						break;
					case 4:
						nx6=0;
						ny6=136;
						break;
					case 5:
						nx6=80;
						ny6=136;
						break;
					case 6:
						nx6=160;
						ny6=136;
						break;
					default:
						break;
							
				}
				p6=temp;
			}
			
		}
	
			
	
		
	}
			
	// End of user function --------------------------------------------------
   	return 0;
} 




// Touch Screen interrupt registration --------------------------------------- 
void Init_TS(void)
{
	for(i=1;i<4;i++)							// calibration table
	{	
		for(j;j<TABLE_X*i/3;j++)
			cal_x[j] = GRID*j + fx[i-1]*GRID + GRID_CENTER;
		for(k;k<TABLE_Y*i/3;k++)
			cal_y[k] = GRID*k + fy[i-1]*GRID + GRID_CENTER;
	}

	
    Uart_Printf("[Touch Screen Test.]\n");
    Uart_Printf("Auto X/Y position conversion mode test\n");

    rADCDLY=(50000);							// ADC Start or Interval Delay

    rADCCON = (1<<14)|(ADCPRS<<6)|(0<<3)|(0<<2)|(0<<1)|(0);	
    // Enable Prescaler,Prescaler,AIN5/7 fix,Normal,Disable read start,No operation
    rADCTSC=(0<<8)|(1<<7)|(1<<6)|(0<<5)|(1<<4)|(0<<3)|(0<<2)|(3);
    // Down,YM:GND,YP:AIN5,XM:Hi-z,XP:AIN7,XP pullup En,Normal,Waiting for interrupt mode


    pISR_ADC=(unsigned)Adc_or_TsAuto;           // interrupt service routine init
    rINTMSK=~(BIT_ADC);                         // Interrupt Mask Clear
    rINTSUBMSK=~(BIT_SUB_TC);                            

    Uart_Printf("\nType any key to exit!!!\n");         
    Uart_Printf("\nStylus Down, please...... \n");

// Calibration 시에 지워야 하는 영역 **********************************************************
//    Uart_Getch();              							 // 키입력시 ADC int 중단
    
//  masking INT ADC ------------------------------------------------------------------
//   rINTSUBMSK|=BIT_SUB_TC;                     // Interrupt Masking
//    rINTMSK|=(BIT_ADC);
//   Uart_Printf("[End Touch Screen Test.]\n");
//  ----------------------------------------------------------------------------------   

// ********************************************************************************************
}

// Touch Screen interrupt service routine ------------------------------------
void __irq Adc_or_TsAuto(void)
{
    rINTSUBMSK|=(BIT_SUB_ADC|BIT_SUB_TC);	  	// Mask sub interrupt (ADC and TC) 
    
    if(rADCDAT0 & 0x8000)						// When reserved bit == 1
    {
		Uart_Printf("\nStylus Up!!\n");
		rADCTSC &= 0xff;                        // Stylus Down
		color =0xf81f;
    }
    else 
    { 
		Uart_Printf("\nStylus Down!!\n");
		rADCTSC=(0<<8)|(1<<7)|(1<<6)|(0<<5)|(1<<4)|(1<<3)|(1<<2)|(0);
		// Stylus Down,Don't care,Don't care,Don't care,Don't care,XP pullup Dis,Auto,No operation
		
		rADCCON|=0x1;							// Start Auto conversion
		while(rADCCON & 0x1);					// check if Enable_start is low
		while(!(0x8000&rADCCON));				// Check ECFLG
				
		ADC_x = (rADCDAT1&0x3ff)*LCD_RIGHT_BOUNDARY/0x3ff;	
		ADC_y = (rADCDAT0&0x3ff)*LCD_DOWN_BOUNDARY/0x3ff;
		ADC_x /=GRID;	
		ADC_y /=GRID;
		
		if(cal_cnt!=0) calibrate();				// for calibration
		
		PutPixel(cal_x[ADC_x], cal_y[ADC_y], 0x0);
		
		
		if(cal_x[ADC_x]>0 && cal_x[ADC_x]<160 && cal_y[ADC_y]>0 && cal_y[ADC_y]<136)
		{	
			pic_n=1;
			 Buzzer(1);
		}
		if(cal_x[ADC_x]>=160 && cal_x[ADC_x]<320 && cal_y[ADC_y]>0 && cal_y[ADC_y]<136)
		
		{	
			pic_n=2;
			 Buzzer(2);
		}
		if(cal_x[ADC_x]>=320 && cal_x[ADC_x]<480 && cal_y[ADC_y]>0 && cal_y[ADC_y]<136)
		
		{	
			pic_n=3;
			 Buzzer(3);
		}
		if(cal_x[ADC_x]>=0 && cal_x[ADC_x]<160 && cal_y[ADC_y]>136 && cal_y[ADC_y]<272)
		
		{	
			pic_n=4;
			 Buzzer(4);
		}
		if(cal_x[ADC_x]>=160 && cal_x[ADC_x]<320 && cal_y[ADC_y]>136 && cal_y[ADC_y]<272)
		
		{	
			pic_n=5;
			 Buzzer(5);
		}
		if(cal_x[ADC_x]>=320 && cal_x[ADC_x]<480 && cal_y[ADC_y]>136 && cal_y[ADC_y]<272)
		
		{	
			pic_n=6;
			 Buzzer(6);
		}
		
		Uart_Printf("%d %d\n",cal_x[ADC_x], cal_y[ADC_x]);
		check++;
		if(check==1){
			check_pic1 = pic_n;
			Uart_Printf("c1: %d\n",check_pic1);
		}
		else if(check==2){
			check_pic2 = pic_n;
			Uart_Printf("c2: %d\n",check_pic2);
														//2,5,4,1,6,3 => c1:2번째칸 c2:5번째칸 => 2,6,4,1,5,3
			tmp=randNum[check_pic1]; //tmp=randNum[2]=5
			
			randNum[check_pic1]=randNum[check_pic2]; //randNum[2]=randNum[5] => randNum[2]=6 =>2,6,4,1,6,3
			
			randNum[check_pic2]=tmp; //randNum[5]=5 =>2,6,4,1,5,3
			Uart_Printf("%d %d %d %d %d %d\n",randNum[1],randNum[2],randNum[3],randNum[4],randNum[5],randNum[6]);

			
			check=0;
		}
		
		rADCTSC=(1<<8)|(1<<7)|(1<<6)|(0<<5)|(1<<4)|(0<<3)|(0<<2)|(3);
   		// Stylus Up,YM=GND,YP:AIN[5],XM=Hi-z,XP:AIN[7],XP pullup En,Normal,Waiting mode
    }
    
    rSUBSRCPND|=BIT_SUB_TC;
    rINTSUBMSK=~(BIT_SUB_TC);					// Unmask sub interrupt (TC)     
    ClearPending(BIT_ADC);
    
}
			
// Sub routine ---------------------------------------------------------------


void DrawRectangle(int x1, int y1, int x2, int y2, unsigned short color)
{
	int i;
	for(i=y1; i<=y2; i++)
		DrawLine(x1, i, x2, i, color); 	
}
void DrawCircle(int x1, int y1,int r, unsigned short color)
{
int i,j,a=r*r;
	
	for(i=0;i<a;i++){
		for(j=0;j<a;j++){
			if((x1-i)*(x1-i)+(y1-j)*(y1-j)<=a)
				PutPixel(i,j,color);
		}
	}
}

void MakeTable(void)
{
	Uart_Printf("\n\n---calibration mode----------------------------\n");
	Uart_Printf("---stylus down on red pixel point TWICE\n");	
//
	cal_cnt = 1;
	for(i=0;i<=TABLE_X;i++)						// table reset
		cal_x[i] = GRID*i + GRID_CENTER;
	for(i=0;i<=TABLE_Y;i++)
		cal_y[i] = GRID*i + GRID_CENTER;
		
	for(i=0;i<LCD_RIGHT_BOUNDARY;i++)			// lcd clear
		for(j=0;j<LCD_DOWN_BOUNDARY;j++)
			PutPixel(i, j, 0xFFFF);
	DrawRectangle(cal_x[TP_X1]-GRID_CENTER, cal_y[TP_Y1]-GRID_CENTER, cal_x[TP_X1]+GRID_CENTER, cal_y[TP_Y1]+GRID_CENTER, 0xf800);	
	// stylus point 1
}

void calibrate(void)
{
	tx[cal_cnt-1] = ADC_x;
	ty[cal_cnt-1] = ADC_y;
	Uart_Printf("TX : %d TY : %d\n",ADC_x,ADC_y);
	
	cal_cnt = (cal_cnt == 7) ? cal_cnt : cal_cnt + 1;
	
	if((cal_cnt == 1) || (cal_cnt == 2))			// stylus point 1
		DrawRectangle(cal_x[TP_X1]-GRID_CENTER, cal_y[TP_Y1]-GRID_CENTER, cal_x[TP_X1]+GRID_CENTER, cal_y[TP_Y1]+GRID_CENTER, 0xf800);
	else if((cal_cnt == 3) || (cal_cnt == 4))		// stylus point 2	
	   	DrawRectangle(cal_x[TP_CX]-GRID_CENTER, cal_y[TP_CY]-GRID_CENTER, cal_x[TP_CX]+GRID_CENTER, cal_y[TP_CY]+GRID_CENTER, 0xf800);
	else if((cal_cnt == 5) || (cal_cnt == 6))		// stylus point 3   	
   		DrawRectangle(cal_x[TP_X2]-GRID_CENTER, cal_y[TP_Y2]-GRID_CENTER, cal_x[TP_X2]+GRID_CENTER, cal_y[TP_Y2]+GRID_CENTER, 0xf800);
	
	if(cal_cnt == 7) 
	{
		DrawRectangle(cal_x[TP_X1], cal_y[TP_Y1], cal_x[TP_X2], cal_y[TP_Y2], 0x1111);	
		fx[0] = TP_X1 - (tx[0] + tx[1])/2;			// calculation 
		fy[0] = TP_Y1 - (ty[0] + ty[1])/2;	
		fx[1] = TP_CX - (tx[2] + tx[3])/2;
		fy[1] = TP_CY - (ty[2] + ty[3])/2;	
		fx[2] = TP_X2 - (tx[4] + tx[5])/2;
		fy[2] = TP_Y2 - (ty[4] + ty[5])/2;
		
		j = k = 0;
		for(i=1;i<4;i++)							// calibrate table
		{
			Uart_Printf("calibrated coordinates X%d : %d Y%d : %d\n",i,fx[i-1],i,fy[i-1]);
			for(j;j<TABLE_X*i/3;j++)
				cal_x[j] += (fx[i-1]*GRID);
			for(k;k<TABLE_Y*i/3;k++)
				cal_y[k] += (fy[i-1]*GRID);
		}
		Uart_Printf("\nplz correct your calibration value\n");		
		cal_cnt = 0;					
	}
}	