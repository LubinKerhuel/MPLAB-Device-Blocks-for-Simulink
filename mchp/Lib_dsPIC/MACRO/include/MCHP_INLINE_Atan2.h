/************************************************************************************************
; MPLAB 16-Bit Device Blocks for Simulink              		       								*
;																	   							*
; Product Page:  http://www.microchip.com/Developmenttools/ProductDetails.aspx?PartNO=SW007023  *
;         Forum: http://www.microchip.com/forums/f192.aspx         								*
;         Wiki:  http://microchipdeveloper.com/simulink:start      								*
; 																	   							*
; Author:        Lubin Kerhuel, Constantin Velea                  								*
; Company:       Microchip Technology Inc                         								* 
; Filename:      MCHP_INLINE_Atan2.h                                    						*
; Date:          16/02/2017                                       								*
; File Version:  1.00                                             								*
;************************************************************************************************
;
; SOFTWARE LICENSE AGREEMENT:
; Microchip Technology Incorporated ("Microchip") retains all ownership and 
; intellectual property rights in the code accompanying this message and in all 
; derivatives hereto.  You may use this code, and any derivatives created by 
; any person or entity by or on your behalf, exclusively with Microchip's
; proprietary products.  Your acceptance and/or use of this code constitutes 
; agreement to the terms and conditions of this notice.
;
; CODE ACCOMPANYING THIS MESSAGE IS SUPPLIED BY MICROCHIP "AS IS".  NO 
; WARRANTIES, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING, BUT NOT LIMITED 
; TO, IMPLIED WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY AND FITNESS FOR A 
; PARTICULAR PURPOSE APPLY TO THIS CODE, ITS INTERACTION WITH MICROCHIP'S 
; PRODUCTS, COMBINATION WITH ANY OTHER PRODUCTS, OR USE IN ANY APPLICATION. 
;
; YOU ACKNOWLEDGE AND AGREE THAT, IN NO EVENT, SHALL MICROCHIP BE LIABLE, WHETHER 
; IN CONTRACT, WARRANTY, TORT (INCLUDING NEGLIGENCE OR BREACH OF STATUTORY DUTY), 
; STRICT LIABILITY, INDEMNITY, CONTRIBUTION, OR OTHERWISE, FOR ANY INDIRECT, SPECIAL, 
; PUNITIVE, EXEMPLARY, INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, FOR COST OR EXPENSE OF 
; ANY KIND WHATSOEVER RELATED TO THE CODE, HOWSOEVER CAUSED, EVEN IF MICROCHIP HAS BEEN 
; ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE.  TO THE FULLEST EXTENT 
; ALLOWABLE BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS IN ANY WAY RELATED TO 
; THIS CODE, SHALL NOT EXCEED THE PRICE YOU PAID DIRECTLY TO MICROCHIP SPECIFICALLY TO 
; HAVE THIS CODE DEVELOPED.
;
; You agree that you are solely responsible for testing the code and 
; determining its suitability.  Microchip has no obligation to modify, test, 
; certify, or support the code.
;
;************************************************************************************************/

#ifndef _MCHP_INLINE_Atan2_H
#define	_MCHP_INLINE_Atan2_H

/* Prototype: unsigned int MCHP_ASM_Atan2( unsigned int u2, unsigned int u1); */

extern inline __attribute__((always_inline))int MCHP_INLINE_Atan2(int x,int y){ 
   
int out = 0;  

__asm__ volatile(                         
"PUSH   ACCAL               \n\t"
"PUSH   ACCAH               \n\t"
"PUSH   ACCAU               \n\t"
"PUSH   CORCON              \n\t"        
"MOV    %[u2],W1            \n\t"         
"MOV    %[u1],W2            \n\t"         
"MOV 	#25736,W0           \n\t"         
"BTSS  	W1,#15              \n\t"         
"MOV 	#0,W0               \n\t"         
"CP0 	W2                  \n\t"         
"BTSC 	SR,#1               \n\t"         
"GOTO 	Label_Atan2_%=      \n\t"         
"MOV	W2,W0               \n\t"         
"push.D W0                 	\n\t"         
"MOV 	#0x10F1, w4         \n\t"         
"MOV	w4,CORCON           \n\t"         
"BTSC   w1,#15              \n\t"         
"NEG    w1,w1               \n\t"         
"BTSC   w1,#15              \n\t"         
"COM    w1,w1               \n\t"         
"BTSC   w2,#15              \n\t"         
"NEG    w2,w2               \n\t"         
"BTSC   w2,#15              \n\t"         
"COM    w2,w2               \n\t"         
"MOV 	W2,W3               \n\t"         
"CPSGT  W2,W1              	\n\t"         
"EXCH   W1,W2              	\n\t"         
"CLR 	W0                 	\n\t"         
"PUSH   RCOUNT              \n\t"         
"REPEAT	#17                 \n\t"         
"DIV.UD W0, W2           	\n\t"         
"POP    RCOUNT             	\n\t"         
"MOV 	W0,W4              	\n\t"         
"MOV 	#41652,W6          	\n\t"         
"MPY    W4*W6, A           	\n\t"         
"MAC    W4*W6, A           	\n\t"         
"MUL.UU W0,W0,W4           	\n\t"         
"MOV 	#2001,W6           	\n\t"         
"MAC    W5*W6, A           	\n\t"         
"MUL.UU W0,W5,W4           	\n\t"         
"MOV	#37775,W6          	\n\t"         
"MSC    W5*W6, A           	\n\t"         
"MUL.UU W0,W5,W4           	\n\t"         
"MOV 	#22196,W6          	\n\t"         
"MAC    W5*W6, A           	\n\t"         
"MUL.UU W0,W5,W4           	\n\t"         
"MOV 	#4187,W6           	\n\t"         
"MSC    W5*W6, A           	\n\t"         
"SAC.R  A,#3, W0           	\n\t"         
"MOV 	#0x4000,W1         	\n\t"         
"CPSNE  W2,W3              	\n\t"         
"SUB 	W1,W0,W0           	\n\t"         
"POP.D  W2                 	\n\t"         
"XOR  	W2,W3,W5           	\n\t"         
"BTSC   W5,#15             	\n\t"         
"NEG    W0,W0              	\n\t"         
"BTSC 	SR,#1              	\n\t"         
"DEC 	W0,W0              	\n\t"         
"MOV 	#0x8000,W1         	\n\t"         
"BTSC   W3,#15             	\n\t"         
"SUB 	W0,W1,W0           	\n\t"         
"MOV 	#51472,W4          	\n\t"         
"MUL.SU W0,W4,W0           	\n\t"         
"LSR 	W0,#15,W0          	\n\t"         
"ADD    W0,W1,W0           	\n\t"         
" Label_Atan2_%= :          \n\t"         
"ADD    W0,#0x0000,%[Result]\n\t"         
"POP    CORCON              \n\t"
"POP    ACCAU               \n\t"
"POP    ACCAH               \n\t"
"POP    ACCAL               \n\t"      
: [Result] "+r"(out)      
: [u1] "r"(x),  [u2] "r"(y)               
: "w0","w1","w2","w3","w4","w5","w6","w7","cc", "memory"); 
return out;                                    
}                                      

#endif

// Attention! : use a " " blank space before LABEL_%= or otherwise, the concatenation
// with assembler generated number for label (%=) will not take place, resulting
// into an unknown "Label_&= :" mnemonic.