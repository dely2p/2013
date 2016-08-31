;; ******************************************************
;; * NAME    : Startup Code ver 1.0			              *  
;; * Date    : 2003/01/17				                    *
;; * Author  : Jong-su, Lee				                 *
;; * E-mail  : tech@armkorea.com			                 *
;; * Homepage: www.armkorea.com				              *
;; * Description:					                          *
;; *   S3C2410X ARM9 Module Rev 1.0			              *
;; ******************************************************

   GET	S3C2410X.inc
   IMPORT  Main    				; The main entry of mon program 

;****************** User Option ******************

STACK_BASEADDR EQU	0x33ff8000			; 8MB SDRAM
  
UserStack      EQU	(STACK_BASEADDR-0x3800)
SVCStack       EQU	0x30040000;(STACK_BASEADDR-0x2800)	
UndefStack     EQU	(STACK_BASEADDR-0x2400)	
AbortStack     EQU	(STACK_BASEADDR-0x2000)	
IRQStack       EQU	(STACK_BASEADDR-0x1000)	
FIQStack       EQU	(STACK_BASEADDR-0x0)   

; Exception handler Base
_ISR_STARTADDRESS	EQU 0x33ffff00

USERMODE      EQU 	0x10
FIQMODE     	EQU 	0x11
IRQMODE     	EQU 	0x12
SVCMODE     	EQU 	0x13
ABORTMODE   	EQU 	0x17
UNDEFMODE   	EQU 	0x1b
MODEMASK    	EQU 	0x1f
NOINT       	EQU 	0xc0

;****************** Macro     ********************
	MACRO
	WR32	$ADDR, $DATA		; [ADDR]=DATA

	LDR	r0, =$ADDR
	LDR	r1, =$DATA
	STR	r1, [r0]
	MEND

	MACRO
	WR16	$ADDR, $DATA		; [ADDR]=DATA

	LDR	r0, =$ADDR
	LDR	r1, =$DATA
	STRH	r1, [r0]
	MEND
	
	MACRO
	WR8	$ADDR, $DATA		; [ADDR]=DATA

	LDR	r0, =$ADDR
	LDR	r1, =$DATA
	STRB	r1, [r0]
	MEND	
	
	IMPORT  |Image$$RO$$Limit|  	; End of ROM code (=start of ROM data)
	IMPORT  |Image$$RW$$Base|   	; Base of RAM to initialise
	IMPORT  |Image$$ZI$$Base|   	; Base and limit of area
	IMPORT  |Image$$ZI$$Limit|  	; to zero initialise		
			
   AREA    Init,CODE,READONLY
   ENTRY 
;--------------------------------------------------------------------------------------------------
; The Reset Entry Point
;--------------------------------------------------------------------------------------------------
ResetHandler

	WR32	WTCON, (47<<8)+(0<<5)+(0<<0)		; Disable Watch-Dog Timer (66KHz=15us)
	WR32	INTMSK, 0xFFFFFFFF			; All Interrupt Disable
	WR32	INTSUBMSK, 0x7FF			; All sub-interrupt disable

	WR32	CLKDIVN, (0<<2)+(1<<1)+(1<<0)		; 1:2:4
	WR32	LOCKTIME, 0xFFFFFF
;	WR32	MPLLCON, (0xA1<<12)+(3<<4)+(1)		; Fin=12MHz, Fout=202.8MHz
;	WR32	MPLLCON, (0x7D<<12)+(4<<4)+(0)		; Fin=12MHz, Fout=266MHz
;	WR32	UPLLCON, (0x28<<12)+(1<<4)+(2)		; Fin=12MHz, Fout=48MHz

  	; Setup IRQ handler
	ldr	r0,=HandleIRQ       ;This routine is needed
	ldr	r1,=IsrIRQ          ;if there isn't 'subs pc,lr,#4' at 0x18, 0x1c
	str	r1,[r0]
	
	BL	InitPort
	;BL	InitMemoryController	
	BL	InitStacks			     	; Init Supervisor Stack
	BL	InitVariables				; Init Global Variables
	
	WR32	HandleIRQ, IsrIRQ			; Enabe IRQ
	
	; IMPORT  __main
	;b	__main
	bl	Main	
	LDR	lr, =Main
	MOV	pc, lr		
	; Don't use main() because ......
	LDR lr,=0x2000
	MOV pc,lr
	B	.
	
IsrIRQ  
	sub	sp,sp,#4       ;reserved for PC
	stmfd	sp!,{r8-r9}   
	
	ldr	r9,=INTOFFSET
	ldr	r9,[r9]
	ldr	r8,=HandleEINT0
	add	r8,r8,r9,lsl #2
	ldr	r8,[r8]
	str	r8,[sp,#8]
	ldmfd	sp!,{r8-r9,pc}
	
;--------------------------------------------------------------------------------------------------
InitPort	
;--------------------------------------------------------------------------------------------------
   LDR		r0, =GPHCON				; Set TxD0/RxD0
   LDR		r1, [r0]
   BIC		r1, r1, #0xF0
   ORR		r1, r1, #0xA0
   STR		r1, [r0]
   
   LDR		r0, =GPBCON				; Set GPB5/6
   LDR		r1, [r0]
   BIC		r1, r1, #(0xF<<10)
   ORR		r1, r1, #(0x5<<10)
   STR		r1, [r0]
   
   LDR		r0, =GPBDAT				; Initial Value
   LDR		r1, [r0]
   ORR		r1, r1, #(3<<5);
   STR		r1, [r0]
   
   MOV		pc, lr
   LTORG
	
;--------------------------------------------------------------------------------------------------
InitMemoryController	
;--------------------------------------------------------------------------------------------------
   LDR	r0,=SMRDATA
   LDR	r1,=BWSCON				;BWSCON Address
   ADD	r2, r0, #52				;End address of SMRDATA
0       
   LDR	r3, [r0], #4    
   STR	r3, [r1], #4    
   CMP	r2, r0		
   BNE	%B0
   MOV	pc, lr

SMRDATA DATA

   DCD (0+(0<<4)+(0<<8)+(0<<12)+(0<<16)+(0<<20)+(2<<24)+(0<<28))
   DCD ((0<<13)+(0<<11)+(7<<8)+(0<<6)+(0<<4)+(0<<2)+(0))   		;GCS0
   DCD ((0<<13)+(0<<11)+(7<<8)+(0<<6)+(0<<4)+(0<<2)+(0))   		;GCS1 
   DCD ((0<<13)+(0<<11)+(7<<8)+(0<<6)+(0<<4)+(0<<2)+(0))   		;GCS2
   DCD ((0<<13)+(0<<11)+(7<<8)+(0<<6)+(0<<4)+(0<<2)+(0))   		;GCS3
   DCD ((0<<13)+(0<<11)+(7<<8)+(0<<6)+(0<<4)+(0<<2)+(0))   		;GCS4
   DCD ((0<<13)+(0<<11)+(7<<8)+(0<<6)+(0<<4)+(0<<2)+(0))   		;GCS5
   DCD ((3<<15)+(1<<2)+(0))    									;GCS6
   DCD ((0<<13)+(0<<11)+(7<<8)+(0<<6)+(0<<4)+(0<<2)+(0))   		;GCS7
   DCD ((1<<23)+(0<<22)+(0<<20)+(3<<18)+(2<<16)+1113)    
   DCD 0x32            											;SCLK power saving mode, BANKSIZE 128M/128M
   DCD 0x30            											;MRSR6 CL=3clk
   DCD 0x30            											;MRSR7

		
   ALIGN	
	
;--------------------------------------------------------------------------------------------------
InitStacks	
;--------------------------------------------------------------------------------------------------	
	MRS	r0,cpsr
	BIC	r0,r0,#MODEMASK
	ORR	r1,r0,#UNDEFMODE|NOINT
	MSR	cpsr_cxsf,r1				;UndefMode
	LDR	sp,=UndefStack
	
	ORR	r1,r0,#ABORTMODE|NOINT
	MSR	cpsr_cxsf,r1				;AbortMode
	LDR	sp,=AbortStack

	ORR	r1,r0,#IRQMODE|NOINT
	MSR	cpsr_cxsf,r1				;IRQMode
	LDR	sp,=IRQStack
    
	ORR	r1,r0,#FIQMODE|NOINT
	MSR	cpsr_cxsf,r1				;FIQMode
	LDR	sp,=FIQStack

	BIC	r0,r0,#MODEMASK|NOINT
	ORR	r1,r0,#SVCMODE
	MSR	cpsr_cxsf,r1				;SVCMode
	LDR	sp,=SVCStack
	
	MOV	pc,lr 
	LTORG	
	
;--------------------------------------------------------------------------------------------------
InitVariables
;--------------------------------------------------------------------------------------------------
	LDR	r0, =|Image$$RO$$Limit| 	 
	LDR	r1, =|Image$$RW$$Base|		 
	LDR	r3, =|Image$$ZI$$Base|		
			
	CMP	r0, r1				 
	BEQ	%F1
0		
	CMP	r1, r3					; Init RW Section
	LDRCC	r2, [r0], #4
	STRCC	r2, [r1], #4
	BCC	%B0
1		
	LDR	r1, =|Image$$ZI$$Limit| 	 
	MOV	r2, #0
2		
	CMP	r3, r1					; Init ZI Section
	STRCC	r2, [r3], #4
	BCC	%B2	
	
	MOV	pc, lr
	LTORG
	
	
	
	
	
    	AREA RamData, DATA, READWRITE	
;--------------------------------------------------------------------------------------------------
;	Data Region
;--------------------------------------------------------------------------------------------------

	^	_ISR_STARTADDRESS
HandleReset       #	   4	; 0x00 Reset Exception
HandleUndef 	   #   	4	; 0x04 Undefine Instruction Exception
HandleSWI   	   #   	4	; 0x08 Software Interrupt Exception
HandlePabort      #   	4	; 0x0C Prefetch Abort Exception
HandleDabort      #   	4	; 0x10 Data Abort Exception
HandleReserved    #   	4	; 0x14
HandleIRQ   	   #   	4	; 0x18 IRQ Exception
HandleFIQ   	   #   	4	; 0x1C FIQ Exception

HandleEINT0   	   #   	4	; Interrupt Resouces
HandleEINT1   	   #   	4
HandleEINT2   	   #   	4
HandleEINT3   	   #   	4
HandleEINT4_7	   #   	4
HandleEINT8_23	   #   	4
HandleRSV6	      #   	4
HandleBATFLT   	#   	4
HandleTICK   	   #   	4
HandleWDT	      #   	4
HandleTIMER0 	   #   	4
HandleTIMER1 	   #   	4
HandleTIMER2 	   #   	4
HandleTIMER3 	   #   	4
HandleTIMER4 	   #   	4
HandleUART2  	   #   	4
HandleLCD 	      #   	4
HandleDMA0     	#   	4
HandleDMA1	      #   	4
HandleDMA2	      #   	4
HandleDMA3	      #   	4
HandleMMC	      #   	4
HandleSPI0     	#   	4
HandleUART1	      #   	4
HandleRSV24	      #   	4
HandleUSBD	      #   	4
HandleUSBH	      #   	4
HandleIIC      	#   	4
HandleUART0    	#   	4
HandleSPI1     	#   	4
HandleRTC      	#   	4
HandleADC      	#   	4
	
	
	END
