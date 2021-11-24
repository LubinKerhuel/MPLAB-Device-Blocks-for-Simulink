/************************************************************************************************
; MPLAB 16-Bit Device Blocks for Simulink              		       								*
;																	   							*
; Product Page:  http://www.microchip.com/Developmenttools/ProductDetails.aspx?PartNO=SW007023  *
;         Forum: http://www.microchip.com/forums/f192.aspx         								*
;         Wiki:  http://microchipdeveloper.com/simulink:start      								*
; 																	   							*
; Author:        Lubin Kerhuel, Constantin Velea                  								*
; Company:       Microchip Technology Inc                         								* 
; Filename:      MCHP_MACRO_ABS.h                                    						    *
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

#ifndef _MCHP_MACRO_ABS_H
#define	_MCHP_MACRO_ABS_H

/* Prototype: unsigned int MCHP_ASM_ABS(unsigned int u1); */

#define MCHP_MACRO_ABS(y)	            \
({                                      \
register int x = (int) (y);             \
__asm__ volatile(                       \
"BTSC   %[u1],#15           \n\t"       \
"NEG    %[u1],%[u1]         \n\t"       \
"BTSC   %[u1],#15           \n\t"       \
"COM    %[u1],%[u1]         \n\t"       \
: [u1] "+r"(x)                          \
:                                       \
: "cc");                                \
x;                                      \
})                                      \

#endif
