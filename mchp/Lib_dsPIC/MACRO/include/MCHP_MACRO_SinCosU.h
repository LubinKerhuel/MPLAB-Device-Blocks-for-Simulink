/************************************************************************************************
* MPLAB 16-Bit Device Blocks for Simulink                                                       *
*                                                                                               *
* Product Page:  http://www.microchip.com/Developmenttools/ProductDetails.aspx?PartNO=SW007023  *
*         Forum: http://www.microchip.com/forums/f192.aspx                                      *
*         Wiki:  http://microchipdeveloper.com/simulink:start                                   *
*                                                                                               *
* Author:        Lubin Kerhuel, Constantin Velea                                                *
* Company:       Microchip Technology Inc                                                       * 
* Filename:      MCHP_MACRO_SinCos.h                                                            *
* Date:          16/02/2017                                                                     *
* File Version:  1.01                                                                           *
*************************************************************************************************
 
  SOFTWARE LICENSE AGREEMENT:
  Microchip Technology Incorporated ("Microchip") retains all ownership and 
  intellectual property rights in the code accompanying this message and in all 
  derivatives hereto.  You may use this code, and any derivatives created by 
  any person or entity by or on your behalf, exclusively with Microchip's
  proprietary products.  Your acceptance and/or use of this code constitutes 
  agreement to the terms and conditions of this notice.
 
  CODE ACCOMPANYING THIS MESSAGE IS SUPPLIED BY MICROCHIP "AS IS".  NO 
  WARRANTIES, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING, BUT NOT LIMITED 
  TO, IMPLIED WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY AND FITNESS FOR A 
  PARTICULAR PURPOSE APPLY TO THIS CODE, ITS INTERACTION WITH MICROCHIP'S 
  PRODUCTS, COMBINATION WITH ANY OTHER PRODUCTS, OR USE IN ANY APPLICATION. 
 
  YOU ACKNOWLEDGE AND AGREE THAT, IN NO EVENT, SHALL MICROCHIP BE LIABLE, WHETHER 
  IN CONTRACT, WARRANTY, TORT (INCLUDING NEGLIGENCE OR BREACH OF STATUTORY DUTY), 
  STRICT LIABILITY, INDEMNITY, CONTRIBUTION, OR OTHERWISE, FOR ANY INDIRECT, SPECIAL, 
  PUNITIVE, EXEMPLARY, INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, FOR COST OR EXPENSE OF 
  ANY KIND WHATSOEVER RELATED TO THE CODE, HOWSOEVER CAUSED, EVEN IF MICROCHIP HAS BEEN 
  ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE.  TO THE FULLEST EXTENT 
  ALLOWABLE BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS IN ANY WAY RELATED TO 
  THIS CODE, SHALL NOT EXCEED THE PRICE YOU PAID DIRECTLY TO MICROCHIP SPECIFICALLY TO 
  HAVE THIS CODE DEVELOPED.
 
  You agree that you are solely responsible for testing the code and 
  determining its suitability.  Microchip has no obligation to modify, test, 
  certify, or support the code.
 
************************************************************************************************/

#ifndef _MCHP_MACRO_SinCosU_H
#define _MCHP_MACRO_SinCosU_H

/* Prototype: void MCHP_ASM_SinCosU(int * y1, int * y2, unsigned int x, unsigned int y, unsigned int z); */
/* 46 cycles */

/* The function estimates -1/2*cos(x) on [0 pi/2] With the polynomial:    */
/* P = p0 + p2*x^2 + p4*x^4 + p6*x^6                                      */
/* with p0 = -0.5       (-32768 loaded in the DSP accumulator)            */
/*      p2 =  0.616882  ( 40428 in Q0.16)                                 */
/*      p4 = -0.126556  (  8294 in Q0.16, substracted)                    */
/*      p6 =  0.009700  (   636 in Q0.16)                                 */
/* The function output is reconstructed form this estimation.             */



#define MCHP_MACRO_SinCosU(ptr1,ptr2,x,y,z)                                     \
({                                                                              \
int xx = ((unsigned int) (x));                                                  \
int yy = ((unsigned int) (y));                                                  \
int zz = ((unsigned int) (z));                                                  \
short _Accum Acc;                                                               \
int sin;                                                                        \
int cos;                                                                        \
__asm__ volatile(                                                               \
"MUL.UU  %[u1],%[u3],W4     \n\t"  /* Normalize and modulo on input */          \
"MUL.UU  %[u1],%[u2],W6     \n\t"                                               \
"ADD     W5,W6,W4           \n\t"                                               \
"MOV     w4,w0              \n\t"  /* w0 is normalized input x modulo 2*pi */   \
"SL      w4,#2,w1           \n\t"  /* w1 is normalized w0 modulo pi/2 */        \
"PUSH    CORCON             \n\t"                                               \
"MOV     #0x10F1,w6         \n\t"                                               \
"MOV     w6,CORCON          \n\t"                                               \
"MOV     #-32768,w5         \n\t"   /* -0.5 start computing cos */              \
"LAC     w5,%[A]            \n\t"   /* A := -0.5 */                             \
"MUL.UU  w1,w1,w6           \n\t"   /* x^2 */                                   \
"MOV     #40428,w6          \n\t"   /* p2 */                                    \
"MAC     w6*w7,%[A]         \n\t"   /* A += p2 * x^2 */                         \
"MUL.UU  w7,w7,w4           \n\t"   /* x^4 */                                   \
"MOV     #8294,w4           \n\t"   /* p4 */                                    \
"MSC     w4*w5,%[A]         \n\t"   /* A += p4 * x^4 */                         \
"MUL.UU  w5,w7,w4           \n\t"   /* x^6 */                                   \
"MOV     #636, w4           \n\t"   /* p6 */                                    \
"MAC     w4*w5,%[A]         \n\t"   /* A += p6 * x^6. A is 1/2 cos(x) */        \
"BTSS    w0,#15             \n\t"   /* Check input cadrant */                   \
"NEG     %[A]               \n\t"   /* neg A if x > 0 */                        \
"SAC.R   %[A],#1,w2         \n\t"   /* w2 := 2 * A */                           \
"COM     w1,w1              \n\t"   /* w1 := 1-w1 use sin(x) = cos(pi/2-x) */   \
"MOV     #-32768,w5         \n\t"   /* compute sin, idem... */                  \
"LAC     w5,%[A]            \n\t"                                               \
"MUL.UU  w1,w1,w6           \n\t"                                               \
"MOV     #40428,w6          \n\t"                                               \
"MAC     w6*w7,%[A]         \n\t"                                               \
"MUL.UU  w7,w7,w4           \n\t"                                               \
"MOV     #8294,w4           \n\t"                                               \
"MSC     w4*w5,%[A]         \n\t"                                               \
"MUL.UU  w5,w7,w4           \n\t"                                               \
"MOV     #636, w4           \n\t"                                               \
"MAC     w4*w5,%[A]         \n\t"                                               \
"BTSC    w0,#15             \n\t"   /* check input cadrant */                   \
"NEG     %[A]               \n\t"   /* neg A if x < 0 */                        \
"BTSS    w0,#14             \n\t"                                               \
"NEG     %[A]               \n\t"   /* neg A if -pi/2 <= x < pi/2 */            \
"SAC.R   %[A],#1,w1         \n\t"   /* w1 := 2 * A */                           \
"BTSS    W0,#14             \n\t"                                               \
"EXCH    W1,W2              \n\t"   /* exchange results if -pi/2 <= x < pi/2 */ \
"MOV     w2,%[ResultSIN]    \n\t"                                               \
"MOV     w1,%[ResultCOS]    \n\t"                                               \
"POP     CORCON             \n\t"                                               \
: [A] "=w"(Acc), [ResultCOS] "=r"(cos) , [ResultSIN] "=r"(sin)                  \
: [u1] "r"(xx),  [u2] "r"(yy),  [u3] "r"(zz)                                    \
: "w0","w1","w2","w4","w5","w6","w7","cc");                                     \
*ptr1 = sin;                                                                    \
*ptr2 = cos;                                                                    \
})                                                                              \

#endif
