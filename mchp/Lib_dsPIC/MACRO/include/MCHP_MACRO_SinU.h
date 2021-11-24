/************************************************************************************************
; MPLAB 16-Bit Device Blocks for Simulink                                                       *
;                                                                                               *
; Product Page:  http://www.microchip.com/Developmenttools/ProductDetails.aspx?PartNO=SW007023  *
;         Forum: http://www.microchip.com/forums/f192.aspx                                      *
;         Wiki:  http://microchipdeveloper.com/simulink:start                                   *
;                                                                                               *
; Author:        Lubin Kerhuel, Constantin Velea                                                *
; Company:       Microchip Technology Inc                                                       * 
; Filename:      MCHP_MACRO_SinU.h                                                              *
; Date:          16/02/2017                                                                     *
; File Version:  1.01                                                                           *
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

#ifndef _MCHP_MACRO_SinU_H
#define _MCHP_MACRO_SinU_H

/* Prototype: int MCHP_MACRO_SinU( unsigned int u1, unsigned int u2, unsigned int u3); */

/* The function estimates -1/2*cos(x) on [0 pi/2] With the polynomial:    */
/* P = p0 + p2*x^2 + p4*x^4 + p6*x^6                                      */
/* with p0 = -0.5       (-32768 loaded in the DSP accumulator)            */
/*      p2 =  0.616882  ( 40428 in Q0.16)                                 */
/*      p4 = -0.126556  (  8294 in Q0.16, substracted)                    */
/*      p6 =  0.009700  (   636 in Q0.16)                                 */
/* The function output is reconstructed form this estimation.             */

#define MCHP_MACRO_SinU(x,y,z)                                                    \
({                                                                                \
int xx = ((int) (x));                                                             \
int yy = ((unsigned int) (y));                                                    \
int zz = ((unsigned int) (z));                                                    \
short _Accum Acc;                                                                 \
int out;                                                                          \
__asm__ volatile(                                                                 \
"MUL.UU  %[u1],%[u3],W4     \n\t"    /* Normalize and modulo on input */          \
"MUL.UU  %[u1],%[u2],W6     \n\t"                                                 \
"ADD     W5,W6,W4           \n\t"                                                 \
"MOV     w4,w0              \n\t"    /* w0 is normalized input x modulo 2*pi */   \
"BTSS    w0,#14             \n\t"    /* if -pi/2 <= x < pi/2 */                   \
"COM     w4,w4              \n\t"    /*   then w4 = 1-w4 */                       \
"SL      w4,#2,w4           \n\t"    /* w4 = w4*4 (represent [0 pi/2]) */         \
"PUSH    CORCON             \n\t"                                                 \
"MOV     #0x10F1,w6         \n\t"                                                 \
"MOV     w6,CORCON          \n\t"                                                 \
"MOV     #-32768,w5         \n\t"   /* -0.5 start computing cos */                \
"LAC     w5,%[A]            \n\t"   /* A := -0.5 */                               \
"MUL.UU  w4,w4,w6           \n\t"   /* x^2 */                                     \
"MOV     #40428,w6          \n\t"   /* p2 */                                      \
"MAC     w6*w7,%[A]         \n\t"   /* A += p2 * x^2 */                           \
"MUL.UU  w7,w7,w4           \n\t"   /* x^4 */                                     \
"MOV     #8294,w4           \n\t"   /* p4 */                                      \
"MSC     w4*w5,%[A]         \n\t"   /* A += p4 * x^4 */                           \
"MUL.UU  w5,w7,w4           \n\t"   /* x^6 */                                     \
"MOV     #636, w4           \n\t"   /* p6 */                                      \
"MAC     w4*w5,%[A]         \n\t"   /* A += p6 * x^6. A is 1/2 cos(x) */          \
"BTSS    w0,#15             \n\t"   /* if x > 0 */                                \
"NEG     %[A]               \n\t"   /*    then neg A */                           \
"SAC.R   %[A],#1,%[Result]  \n\t"   /* Result := 2 * A */                         \
"POP     CORCON             \n\t"                                                 \
: [A] "=w"(Acc), [Result] "=r"(out)                                               \
: [u1] "r"(xx),  [u2] "r"(yy),  [u3] "r"(zz)                                      \
: "w0","w4","w5","w6","w7","cc");                                                 \
out;                                                                              \
})                                                                                \

#endif

