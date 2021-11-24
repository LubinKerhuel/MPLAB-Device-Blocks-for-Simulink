/************************************************************************************************
; MPLAB 16-Bit Device Blocks for Simulink              		       								*
;																	   							*
; Product Page:  http://www.microchip.com/Developmenttools/ProductDetails.aspx?PartNO=SW007023  *
;         Forum: http://www.microchip.com/forums/f192.aspx         								*
;         Wiki:  http://microchipdeveloper.com/simulink:start      								*
; 																	   							*
; Author:        Lubin Kerhuel, Constantin Velea                  								*
; Company:       Microchip Technology Inc                         								* 
; Filename:      MCHP_INLINE_SinCosU.h                                    						*
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

#ifndef _MCHP_INLINE_SinCosU_H
#define	_MCHP_INLINE_SinCosU_H

/* Prototype: void MCHP_ASM_SinCosU(unsigned int * y1, unsigned int * y2, unsigned int u1, unsigned int u2, unsigned int u3); */

extern inline __attribute__((always_inline))void MCHP_INLINE_SinCosU(unsigned int *ptr1,unsigned int *ptr2,unsigned int x,unsigned int y,int z){                                                

__asm__ volatile(                       
"PUSH    ACCAL              \n\t"
"PUSH    ACCAH              \n\t"
"PUSH    ACCAU              \n\t"
"PUSH    CORCON             \n\t"
"MOV     #0x10F1, w7        \n\t"         
"MOV     w7,CORCON          \n\t"         
"PUSH    %[p2]              \n\t"         
"PUSH    %[p1]              \n\t"         
"MUL.UU  %[u1],%[u3],W4     \n\t"         
"MUL.UU  %[u1],%[u2],W0     \n\t"         
"ADD     W0,W5,W0           \n\t"         
"PUSH    W0                 \n\t"         
"MOV     #0x4000,w7         \n\t"         
"SUB     w7,w0,w0           \n\t"         
"MOV     w0,w1              \n\t"         
"BTSC    w1,#14             \n\t"         
"COM     w1,w1              \n\t"         
"SL      w1,#2,w1           \n\t"        
"MOV     #-32768,w7         \n\t"         
"LAC     w7,A               \n\t"         
"MUL.UU  w1,w1,w4           \n\t"         
"MOV     #40420,w7          \n\t"         
"MAC     w5*w7,A            \n\t"         
"MUL.UU  w5,w5,w6           \n\t"         
"MOV     #8276,w4           \n\t"         
"MSC     w4*w7,A            \n\t"         
"MUL.UU  w5,w7,w4           \n\t"         
"MOV     #625, w7           \n\t"         
"MAC     w5*w7,A            \n\t"         
"MOV     #16384,w7          \n\t"         
"ADD     w0,w7,w0           \n\t"         
"BTSS    SR,#3              \n\t"         
"NEG     A                  \n\t"         
"POP     W0                 \n\t"         
"POP     W7                 \n\t"         
"SAC.R   A,#1,[W7]          \n\t"         
"MOV     w0,w1              \n\t"         
"BTSC    w1,#14             \n\t"         
"COM     w1,w1              \n\t"         
"SL      w1,#2,w1           \n\t"         
"MOV     #-32768,w7         \n\t"         
"LAC     w7,A               \n\t"         
"MUL.UU  w1,w1,w4           \n\t"         
"MOV     #40420,w7          \n\t"         
"MAC     w5*w7,A            \n\t"         
"MUL.UU  w5,w5,w6           \n\t"         
"MOV     #8276,w4           \n\t"         
"MSC     w4*w7,A            \n\t"         
"MUL.UU  w5,w7,w4           \n\t"         
"MOV     #625, w7           \n\t"         
"MAC     w5*w7,A            \n\t"         
"MOV     #16384,w7          \n\t"         
"ADD     w0,w7,w0           \n\t"         
"BTSS    SR,#3              \n\t"         
"NEG     A                  \n\t"         
"POP     W7                 \n\t"         
"SAC.R   A,#1,[W7]          \n\t"         
"POP     CORCON             \n\t"
"POP     ACCAU              \n\t"
"POP     ACCAH              \n\t"
"POP     ACCAL              \n\t"
: // No Outputs                       
: [p1] "r"(ptr1),  [p2] "r"(ptr2),  [u1] "r"(x),  [u2] "r"(y),  [u3] "r"(z) 
: "w0","w1","w2","w3","w4","w5","w6","w7","cc", "memory"); 
}                                      

#endif