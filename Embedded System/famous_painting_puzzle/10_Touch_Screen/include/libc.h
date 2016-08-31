/*
 * =====================================================================
 * NAME: 
 *       libc.h
 *
 * Descriptions : 
 *       Definition of S3C2410 Library prototype
 *
 * Developing Environment : 
 *       CodeWarrior 2.0 or Later
 *
 * Author : 
 *       Ahn, HyoBok  (hbahn@dignsys.com)
 *       DIGNSYS Inc. (www.dignsys.com)
 * =====================================================================
 */

#ifndef __libc_h__
#define __libc_h__

#ifdef __cplusplus
extern "C" {
#endif

/*
 * =====================================================================
 * Variable Definition
 * =====================================================================
 */
#define min(x1,x2) (((x1)<(x2))? (x1):(x2))
#define max(x1,x2) (((x1)>(x2))? (x1):(x2))

#define ONESEC0 (62500)		//16us resolution, max 1.04 sec
#define ONESEC1 (31250)		//32us resolution, max 2.09 sec
#define ONESEC2 (15625)		//64us resolution, max 4.19 sec
#define ONESEC3 (7812)		//128us resolution, max 8.38 sec
#define ONESEC4 (PCLK/128/(0xff+1))  //@60Mhz, 128*4us resolution, max 32.53 sec

#define NULL 0

#define DESC_SEC		(0x2|(1<<4))
#define CB			(3<<2)  //cache_on, write_back
#define CNB			(2<<2)  //cache_on, write_through 
#define NCB			(1<<2)  //cache_off,WR_BUF on
#define NCNB			(0<<2)  //cache_off,WR_BUF off
#define AP_RW			(3<<10) //supervisor=RW, user=RW
#define AP_RO			(2<<10) //supervisor=RW, user=RO

#define DOMAIN_FAULT	(0x0)
#define DOMAIN_CHK		(0x1) 
#define DOMAIN_NOTCHK	(0x3) 
#define DOMAIN0		(0x0<<5)
#define DOMAIN1		(0x1<<5)

#define DOMAIN0_ATTR	(DOMAIN_CHK<<0) 
#define DOMAIN1_ATTR	(DOMAIN_FAULT<<2) 

#define RW_CB			(AP_RW|DOMAIN0|CB|DESC_SEC)
#define RW_CNB			(AP_RW|DOMAIN0|CNB|DESC_SEC)
#define RW_NCNB		(AP_RW|DOMAIN0|NCNB|DESC_SEC)
#define RW_FAULT		(AP_RW|DOMAIN1|NCNB|DESC_SEC)

/*
 * =====================================================================
 * Libraries on the libc.c
 * =====================================================================
 */
void Delay(int time);	//Watchdog Timer is used.
void ChangeMPllValue(int m,int p,int s);
void ChangeClockDivider(int hdivn,int pdivn);
void ChangeUPllValue(int m,int p,int s);

void MMU_Init(void);
void ChangeRomCacheStatus(int attr);
void MMU_SetMTT(int vaddrStart,int vaddrEnd,int paddrStart,int attr);

void *malloc(unsigned nbyte); 
void free(void *pt);

void Port_Init(void);
void Led_Display(int data);

void Uart_SendByte(int data);
void Uart_SendString(char *pt);

int  Uart_GetIntNum(void);
void Uart_Printf(char *fmt,...);

void Timer_Start(int divider);    //Watchdog Timer is used.
int  Timer_Stop(void);            //Watchdog Timer is used.

void MemFill(unsigned long ptr, unsigned long pattern, int size);
void MemDump(unsigned long ptr, int size);

/*
 * =====================================================================
 * Libraries on the libs.s
 * =====================================================================
 */
int SET_IF(void);
void WR_IF(int cpsrValue);
void CLR_IF(void);

void MMU_EnableICache(void);
void MMU_DisableICache(void);
void MMU_EnableDCache(void);
void MMU_DisableDCache(void);
void MMU_EnableAlignFault(void);
void MMU_DisableAlignFault(void);
void MMU_EnableMMU(void);
void MMU_DisableMMU(void);
void MMU_SetTTBase(unsigned long base);
void MMU_SetDomain(unsigned long domain);

void MMU_SetFastBusMode(void);          //GCLK=HCLK
void MMU_SetAsyncBusMode(void);         //GCLK=FCLK @(FCLK>=HCLK)

void MMU_InvalidateIDCache(void);
void MMU_InvalidateICache(void);
void MMU_InvalidateICacheMVA(unsigned long mva);
void MMU_PrefetchICacheMVA(unsigned long mva);
void MMU_InvalidateDCache(void);
void MMU_InvalidateDCacheMVA(unsigned long mva);
void MMU_CleanDCacheMVA(unsigned long mva);
void MMU_CleanInvalidateDCacheMVA(unsigned long mva);
void MMU_CleanDCacheIndex(unsigned long index);
void MMU_CleanInvalidateDCacheIndex(unsigned long index); 
void MMU_WaitForInterrupt(void);
        
void MMU_InvalidateTLB(void);
void MMU_InvalidateITLB(void);
void MMU_InvalidateITLBMVA(unsigned long mva);
void MMU_InvalidateDTLB(void);
void MMU_InvalidateDTLBMVA(unsigned long mva);

void MMU_SetDCacheLockdownBase(unsigned long base);
void MMU_SetICacheLockdownBase(unsigned long base);

void MMU_SetDTLBLockdown(unsigned long baseVictim);
void MMU_SetITLBLockdown(unsigned long baseVictim);

void MMU_SetProcessId(unsigned long pid);


/*
 * =====================================================================
 * Assembly testing 
 * =====================================================================
 */
/* Testing sum of decimal */
int SUM_OF_DEC(int start, int end);

/*
 * =====================================================================
 *  device testing
 * =====================================================================
 */
/* Buzzer */
void Buzzer(void);
void BuzzerTest(void);

/* UART */
void Uart_TxEmpty(int ch);
void Uart_Init(int baud);
char Uart_Getch(void);
char Uart_GetKey(void);

/* RTC */
void Display_Rtc(void);
void Rtc_Init(void);
void Rtc_TimeSet(void);

#ifdef __cplusplus
}
#endif

#endif  /* __libc_h__ */
