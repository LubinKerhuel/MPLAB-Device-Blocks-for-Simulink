/************************************************************************************************
; MPLAB 16-Bit Device Blocks for Simulink                                                       *
;                                                                                               *
; Product Page:  http://www.microchip.com/Developmenttools/ProductDetails.aspx?PartNO=SW007023  *
;         Forum: http://www.microchip.com/forums/f192.aspx                                      *
;         Wiki:  http://microchipdeveloper.com/simulink:start                                   *
;                                                                                               *
; Author:        Lubin Kerhuel, Constantin Velea                                                *
; Company:       Microchip Technology Inc                                                       *
; Filename:      MCHP_MACRO_Atan2.h                                                             *
; Date:          16/02/2017                                                                     *
; File Version:  1.00                                                                           *
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

#ifndef _MCHP_MACRO_Atan2_H
#define _MCHP_MACRO_Atan2_H

/* Prototype: unsigned int MCHP_ASM_Atan2( unsigned int u2, unsigned int u1); */

/* Estimate Atan2 function. First, the ratio of u1 and u2 is computed (possibly reversing order, negate...), avoiding possible 0 divide */
/* so as to get a quotient in [0 1]. Atan is computed using a polynomial which estimate atan(x)/(pi/4) (its output is thus normalized) */
/* Result is then reconstructed based on inputs values. */

/* P = p0 + p1*x + p2*x^2 + p3*x^3 + + p4*x^4 + p5*x^5                    */
/* with p0 =  0                                                           */
/*      p1 =  1.271118  ( 83304 in Q0.16 or 41652 * 2)                    */
/*      p2 =  0.030532  (  2001 in Q0.16, substracted)                    */
/*      p3 = -0.576985  (-37774 in Q0.16)                                 */
/*      p4 =  0.338684  ( 22196 in Q0.16)                                 */
/*      p5 = -0.063873  (  4186 in Q0.16, substracted)                    */
/* The function output is reconstructed form this estimation.             */

/* Slight improvement with a order 6 polynomial (not worth it)			  */
/* P = p0 + p1*x + p2*x^2 + p3*x^3 + + p4*x^4 + p5*x^5 + p6*x^6           */
/* with p0 =  0                                                           */
/*      p1 =  1.272920  ( 83422 in Q0.16 )                                */
/*      p2 =  0.008718  (  571 in Q0.16  )                    			  */
/*      p3 = -0.481812  (-31576 in Q0.16 )                                */
/*      p4 =  0.153245  ( 10043 in Q0.16 )                                */
/*      p5 =  0.103993  (  6815 in Q0.16 )                    			  */
/*      p5 = -0.057074  (  3740 in Q0.16 )                     			  */
/* The function output is reconstructed form this estimation.             */

#define MCHP_MACRO_Atan2(x,y)           \
({                                      \
unsigned int xx = (int) (x);            \
unsigned int yy = (int) (y);            \
volatile int out;                       \
short _Accum Acc;                       \
__asm__ volatile(                       \
"MOV    %[u2],W1            \n\t"       \
"MOV    %[u1],W2            \n\t"       \
"MOV    #25736,%[Result]    \n\t"       \
"BTSS   W1,#15              \n\t"       \
"MOV    #0,%[Result]        \n\t"       \
"CP0    W2                  \n\t"       \
"BTSC   SR,#1               \n\t"       \
"GOTO   Label_Atan2_%=      \n\t"       \
"PUSH   CORCON              \n\t"       \
"MOV    W2,W0               \n\t"       \
"push.D W0                  \n\t"       \
"MOV    #0x10F1, w4         \n\t"       \
"MOV    w4,CORCON           \n\t"       \
"BTSC   w1,#15              \n\t"       \
"NEG    w1,w1               \n\t"       \
"BTSC   w1,#15              \n\t"       \
"COM    w1,w1               \n\t"       \
"BTSC   w2,#15              \n\t"       \
"NEG    w2,w2               \n\t"       \
"BTSC   w2,#15              \n\t"       \
"COM    w2,w2               \n\t"       \
"MOV    W2,W3               \n\t"       \
"CPSGT  W2,W1               \n\t"       \
"EXCH   W1,W2               \n\t"       \
"CLR    W0                  \n\t"       \
"PUSH   RCOUNT              \n\t"       \
"REPEAT #17                 \n\t"       \
"DIV.UD W0, W2              \n\t"       \
"POP    RCOUNT              \n\t"       \
"MOV    W0,W4               \n\t"       \
"MOV    #41652,W6           \n\t"       \
"MPY    W4*W6,%[A]          \n\t"       \
"MAC    W4*W6,%[A]          \n\t"       \
"MUL.UU W0,W0,W4            \n\t"       \
"MOV    #2001,W6            \n\t"       \
"MAC    W5*W6,%[A]          \n\t"       \
"MUL.UU W0,W5,W4            \n\t"       \
"MOV    #37775,W6           \n\t"       \
"MSC    W5*W6,%[A]          \n\t"       \
"MUL.UU W0,W5,W4            \n\t"       \
"MOV    #22196,W6           \n\t"       \
"MAC    W5*W6,%[A]          \n\t"       \
"MUL.UU W0,W5,W4            \n\t"       \
"MOV    #4187,W6            \n\t"       \
"MSC    W5*W6,%[A]          \n\t"       \
"SAC.R  %[A],#3, W0         \n\t"       \
"MOV    #0x4000,W1          \n\t"       \
"CPSNE  W2,W3               \n\t"       \
"SUB    W1,W0,W0            \n\t"       \
"POP.D  W2                  \n\t"       \
"XOR    W2,W3,W5            \n\t"       \
"BTSC   W5,#15              \n\t"       \
"NEG    W0,W0               \n\t"       \
"BTSC   SR,#1               \n\t"       \
"DEC    W0,W0               \n\t"       \
"MOV    #0x8000,W1          \n\t"       \
"BTSC   W3,#15              \n\t"       \
"SUB    W0,W1,W0            \n\t"       \
"MOV    #51472,W4           \n\t"       \
"MUL.SU W0,W4,W0            \n\t"       \
"LSR    W0,#15,W0           \n\t"       \
"ADD    W0,W1,%[Result]     \n\t"       \
"POP    CORCON              \n\t"       \
" Label_Atan2_%= :          \n\t"       \
: [A] "=w"(Acc), [Result] "=r"(out)     \
: [u1] "r"(xx),  [u2] "r"(yy)             \
: "w0","w1","w2","w3","w4","w5","w6","w7","cc"); \
out;                                    \
})                                      \

#endif

/* Attention: use a " " blank space before LABEL_%= or otherwise, the concatenation */
/* with assembler generated number for label (%=) will not take place, resulting */
/* into an unknown "Label_&= :" mnemonic. */
