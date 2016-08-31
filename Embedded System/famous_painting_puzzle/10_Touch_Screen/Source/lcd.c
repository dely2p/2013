//#include <string.h>
#include "2410addr.h"
#include "s3c2410.h"
#include "2410lib.h"

//extern const unsigned short ScreenBitmap[];
#define LCDFRAMEBUFFER    0x33800000   

int (*frameBuffer)[LCD_XSIZE_TFT/2];

void Lcd_Port_Init(void)
{
    rGPCUP=0xffffffff; 			// Disable Pull-up register
    rGPCCON=0xaaaaaaaa; 		// Initialize VD[7:0],LCDVF[2:0],VM,VFRAME,VLINE,VCLK,LEND 
    rGPDUP=0xffffffff; 			// Disable Pull-up register
    rGPDCON=0xaaaaaaaa; 		// Initialize VD[23:8]
    Uart_Printf("Initializing LCD Port Init..........\n");
}

void Init_LCD(void)
{	
  	Lcd_Port_Init();
  
   	rLCDCON1 =  (1           <<  8) |   /* VCLK = HCLK / ((CLKVAL + 1) * 2) -> About 7 Mhz  */
				(MVAL_USED   <<  7) |   /* 0 : Each Frame                                   */
				(3           <<  5) |   /* TFT LCD Pannel                                   */
				(12          <<  1) |   /* 16bpp Mode                                       */
				(0           <<  0) ;   /* Disable LCD Output                               */

  	rLCDCON2 =  (VBPD        << 24) |   /* VBPD          :   1                              */
				(LINEVAL_TFT << 14) |   /* Virtical Size : 272 - 1                          */
				(VFPD        <<  6) |   /* VFPD          :   2                              */
				(VSPW        <<  0) ;   /* VSPW          :   1                              */

  	rLCDCON3 =  (HBPD        << 19) |   /* HBPD          :   6                              */
				(HOZVAL_TFT  <<  8) |   /* HOZVAL_TFT    : 480 - 1                          */
				(HFPD        <<  0) ;   /* HFPD          :   2                              */


  	rLCDCON4 =  (MVAL        <<  8) |   /* MVAL          :  13                              */
				(HSPW        <<  0) ;   /* HSPW          :   4                              */

	rLCDCON5 =  (0           << 12) |   /* BPP24BL       : LSB valid                        */
				(1           << 11) |   /* FRM565 MODE   : 5:6:5 Format                     */
				(0           << 10) |   /* INVVCLK       : VCLK Falling Edge                */
				(0           <<  9) |   /* INVVLINE      : Inverted Polarity                */
				(1           <<  8) |   /* INVVFRAME     : Inverted Polarity                */
				(0           <<  7) |   /* INVVD         : Normal                           */
				(0           <<  6) |   /* INVVDEN       : Normal                           */
				(0           <<  5) |   /* INVPWREN      : Normal                           */
				(0           <<  4) |   /* INVENDLINE    : Normal                           */
				(0           <<  3) |   /* PWREN         : Disable PWREN                    */
				(0           <<  2) |   /* ENLEND        : Disable LEND signal              */
				(0           <<  1) |   /* BSWP          : Swap Disable                     */
				(0           <<  0) ;   /* HWSWP         : Swap Disable                     */

    rLCDSADDR1 	= ((LCDFRAMEBUFFER >> 22) << 21) | ((M5D(LCDFRAMEBUFFER >> 1)) <<  0);

	rLCDSADDR2 	= M5D((LCDFRAMEBUFFER + (LCD_XSIZE_TFT * LCD_YSIZE_TFT * 2)) >> 1);

	rLCDSADDR3 	= (((LCD_XSIZE_TFT - LCD_XSIZE_TFT) / 1) << 11) | (LCD_XSIZE_TFT / 1);

	rLCDINTMSK |= (3);
	rTPAL       = 0x0;
	rGPGUP	  	= rGPGUP   & (~(1<<4)) | (1<<4);	    	// Pull-up disbale
	rGPGCON	  	= rGPGCON  & (~(3<<8)) | (3<<8);
	rLCDCON5    = rLCDCON5 & (~(1<<3)) | (1<<3);			// PWREN
	rLCDCON5    = rLCDCON5 & (~(1<<5)) | (0<<5);			// INVPWREN
	
	memset((void *)LCDFRAMEBUFFER, 0xFF, ARRAY_SIZE_TFT_16BIT);
	frameBuffer=(int (*)[LCD_XSIZE_TFT/2])LCDFRAMEBUFFER;
	
	
	rLCDCON1   |= 1;										/* Enable LCD Output*/
	Uart_Printf("Initializing LCD register...........\n");
}

void PutPixel(unsigned int x,unsigned int y,unsigned int c)
{
    if(x<LCD_XSIZE_TFT && y<LCD_YSIZE_TFT)
        frameBuffer[(y)][(x)/2]=
        ( frameBuffer[(y)][x/2] & ~(0xffff0000>>((x)%2)*16) ) | ( (c&0x0000ffff)<<((2-1-((x)%2))*16) );
}

void DrawLine(int x1,int y1,int x2,int y2,int color)
{
	int dx,dy,e;
	dx=x2-x1; 
	dy=y2-y1;
    
	if(dx>=0)
	{
		if(dy >= 0) // dy>=0
		{
			if(dx>=dy) // 1/8 octant
			{
				e=dy-dx/2;
				while(x1<=x2)
				{
					PutPixel(x1,y1,color);
					if(e>0){y1+=1;e-=dx;}	
					x1+=1;
					e+=dy;
				}
			}
			else		// 2/8 octant
			{
				e=dx-dy/2;
				while(y1<=y2)
				{
					PutPixel(x1,y1,color);
					if(e>0){x1+=1;e-=dy;}	
					y1+=1;
					e+=dx;
				}
			}
		}
		else		   // dy<0
		{
			dy=-dy;   // dy=abs(dy)

			if(dx>=dy) // 8/8 octant
			{
				e=dy-dx/2;
				while(x1<=x2)
				{
					PutPixel(x1,y1,color);
					if(e>0){y1-=1;e-=dx;}	
					x1+=1;
					e+=dy;
				}
			}
			else		// 7/8 octant
			{
				e=dx-dy/2;
				while(y1>=y2)
				{
					PutPixel(x1,y1,color);
					if(e>0){x1+=1;e-=dy;}	
					y1-=1;
					e+=dx;
				}
			}
		}	
	}
	else //dx<0
	{
		dx=-dx;		//dx=abs(dx)
		if(dy >= 0) // dy>=0
		{
			if(dx>=dy) // 4/8 octant
			{
				e=dy-dx/2;
				while(x1>=x2)
				{
					PutPixel(x1,y1,color);
					if(e>0){y1+=1;e-=dx;}	
					x1-=1;
					e+=dy;
				}
			}
			else		// 3/8 octant
			{
				e=dx-dy/2;
				while(y1<=y2)
				{
					PutPixel(x1,y1,color);
					if(e>0){x1-=1;e-=dy;}	
					y1+=1;
					e+=dx;
				}
			}
		}
		else		   // dy<0
		{
			dy=-dy;   // dy=abs(dy)

			if(dx>=dy) // 5/8 octant
			{
				e=dy-dx/2;
				while(x1>=x2)
				{
					PutPixel(x1,y1,color);
					if(e>0){y1-=1;e-=dx;}	
					x1-=1;
					e+=dy;
				}
			}
			else		// 6/8 octant
			{
				e=dx-dy/2;
				while(y1>=y2)
				{
					PutPixel(x1,y1,color);
					if(e>0){x1-=1;e-=dy;}	
					y1-=1;
					e+=dx;
				}
			}
		}	
	}
}
