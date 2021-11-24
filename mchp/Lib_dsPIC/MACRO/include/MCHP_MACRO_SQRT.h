/************************************************************************************************
; MPLAB 16-Bit Device Blocks for Simulink                                                       *
;                                                                                               *
; Product Page:  http://www.microchip.com/Developmenttools/ProductDetails.aspx?PartNO=SW007023  *
;         Forum: http://www.microchip.com/forums/f192.aspx                                      *
;         Wiki:  http://microchipdeveloper.com/simulink:start                                   *
;                                                                                               *
; Author:        Lubin Kerhuel, Constantin Velea                                                *
; Company:       Microchip Technology Inc                                                       * 
; Filename:      MCHP_MACRO_SQRT.h                                                          *
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

#ifndef _MCHP_MACRO_SQRT_H
#define _MCHP_MACRO_SQRT_H

#include <xc.h>

/* sqrt approximation using a polynomial approximation and the "folding" property of sqrt function:     */
/* sqrt(1/4*x) = 1/2*sqrt(x)                                                                            */
/* sqrt function has to be approximate in [0.5 1].                                                      */
/* a first polynomial candidate is:                                                                     */
/* s = p0 + p1*x + p2*x^2 + p3*x^3 + p4*x^4 + p5*x^5                                                    */
/* where s = sqrt((x+1)/2) for x in [0 1] (we obtain sqrt(y) for y in [0.5 1] with x = 2*y-1)           */
/*                                                                                                      */
/* p0 = 0.707109047930089      ( 46341.0985651463 in Q0.16)                                             */
/* p1 = 0.353452616938739      ( 23163.8707036972 in Q0.16)                                             */
/* p2 = −0.0872938053798862    ( −5720.88682937622 in Q0.16)                                            */
/* p3 =  0.0392130854306743    (  2569.86876678467 in Q0.16)                                            */
/* p4 = −0.0159601379418746    (  1045.96360015869 in Q0.16)                                            */
/* p5 =  0.00348088500322774   (   228.123279571533 in Q0.16)                                           */
/*                                                                                                      */
/* Another polynomial candidate minimizing quantizaton error:                                           */
/* s = 2*( sqrt((x+1)/2) - sqrt(2)/2 )  ; s output range is larger than previous proposed polynomial.   */
/* with:                                                                                                */
/* p0 =  0                      (     0 in Q0.16)                                                       */
/* p1 =  0.706984569893393      ( 46332.9407725334 in Q0.16)                                            */
/* p2 = −0.175010736333206      (−11469.5036163330 in Q0.16)                                            */
/* p3 =  0.0793782031396404     (  5202.12992095947 in Q0.16)                                           */
/* p4 = −0.0328723080456257     ( −2154.31958007812 in Q0.16)                                           */
/* p5 =  0.00731084850849584  (   479.123767852783 in Q0.16)                                            */
/*                                                                                                      */
/*                                                                                                      */


#define MCHP_MACRO_sqrt32_Q15(u1)                                                              \
({                                                                                             \
short _Accum Acc;                                                                              \
register long int x = (long int) (u1);                                                         \
volatile register int out = 0;                                                                 \
if (x > 0){                                                                                    \
__asm__ volatile(                                                                               \
"FF1l    %d[input1],w3       \n\t"  /* Find first bit =1 from left ( no bit is 0, bit 15 is 1) */ \
"SL      %d[input1],w3,w5    \n\t"  /* apply shift on MSB */                                    \
"SUBR    w3,#16,w6           \n\t"  /* Extract bytes from LSB */                                \
"LSR     %[input1],w6,w6    \n\t"   /* This is hardly required, affect only input values 1,2 and 3 */   \
"ADD     w5,w6,w6           \n\t"   /* Merge MSB bytes with LSB bytes after shift */            \
"CP0     w3                \n\t"                                                                \
"BRA     NZ,sqrt32_Qxx_continue_%=   \n\t"                                                      \
"FF1l    %[input1],w3       \n\t"     /* Find first bit =1 from left il LSB                 */  \
"SL      %[input1],w3,w6    \n\t"     /* apply shift on MSB                                 */  \
"ADD     #16,w3            \n\t"      /* equivalent to a shift on 32 bit value by 16 + w3   */  \
" sqrt32_Qxx_continue_%= :  \n\t"     /* continue                                           */  \
"PUSH    CORCON             \n\t"     /* Set up Accmulator for unsigned ops                 */  \
"MOV     #0x10F1, w4        \n\t"                                                               \
"MOV     w4,CORCON          \n\t"                                                               \
"MOV     #46333, w4         \n\t"     /* p1 = 0.706984569893393                             */  \
"MPY     w4*w6, %[A]        \n\t"     /* A = p1 * x                                         */  \
"MUL.UU  w6,w6,w4           \n\t"     /* x^2                                                */  \
"MOV     #11467, w4          \n\t"    /* p2 = −0.175010736333206                            */  \
"MSC     w4*w5,%[A]         \n\t"     /* A += p2 * x^2                                      */  \
"MUL.UU  w6,w5,w4           \n\t"     /* x^3                                                */  \
"MOV     #5202, w4          \n\t"     /* p3 =  0.0793782031396404                           */  \
"MAC     w4*w5,%[A]         \n\t"     /* A += p3 * x^3                                      */  \
"MUL.UU  w6,w5,w4           \n\t"     /* x^4                                                */  \
"MOV     #2154, w4          \n\t"     /* p4 = −0.0328723080456257                           */  \
"MSC     w4*w5,%[A]         \n\t"     /* A += p4 * x^4                                      */  \
"MUL.UU  w6,w5,w4           \n\t"     /* x^5                                                */  \
"MOV     #479, w4           \n\t"     /* p5 =  0.00731084850849584                          */  \
"MAC     w4*w5,%[A]         \n\t"     /* A += p5 * x^5                                      */  \
"MOV     #23170,w4          \n\t"     /* sqrt(2)/2 in Q1.15                                 */  \
"ADD     w4,#-2,%[A]        \n\t"     /* A += sqrt(2)/2 * 2                                 */  \
"LSR     w3,#1,w5           \n\t"     /* divide by 2                                        */  \
"SFTAC   %[A],w5            \n\t"     /* Shift A. ! Shift within [-16 16] otherwie trap     */  \
"SAC.R   %[A],#1, w7        \n\t"     /* Extract A/2                                        */  \
"MOV     #0,w6              \n\t"                                                               \
"MOV     #46341,w4          \n\t"     /* sqrt(2)/2 in Q0.16                                 */  \
"BTSC    w3,#0              \n\t"     /* if shift is odd factof of  multiple of sqrt(2)     */  \
"MUL.SU  w7,w4,w6           \n\t"     /* multiply by sqrt(2)/2                              */  \
"LSR     w6,#15,w6          \n\t"     /* no impact if mult is not executed (w6=0)           */  \
"ADD     w6,w7,%[Result]    \n\t"     /* take care of LSB if mult occurs                    */  \
"POP     CORCON             \n\t"                                                               \
: [Result] "=r"(out), [A] "=w"(Acc)                                                             \
: [input1] "r"(x)                                                                               \
: "w3","w4","w5","w6","w7","cc");                                                               \
}                                                                                               \
out;                                                                                            \
})    
 


#define MCHP_MACRO_sqrt32_Qxx(u1,u2)                                                           \
({                                                                                             \
short _Accum Acc;                                                                              \
long int x = (long int) (u1);                                                                  \
register unsigned int y = (unsigned int) (u2);                                                 \
volatile register int out = 0;                                                                 \
if (x > 0){                                                                                    \
__asm__ volatile(                                                                              \
"FF1l    %d[input1],w3       \n\t"  /* Find first bit =1 from left ( no bit is 0, bit 15 is 1) */ \
"SL      %d[input1],w3,w5    \n\t"  /* apply shift on MSB */                                    \
"SUBR    w3,#16,w6           \n\t"  /* Extract bytes from LSB */                                \
"LSR     %[input1],w6,w6    \n\t"   /* This is hardly required, affect only input values 1,2 and 3 */   \
"ADD     w5,w6,w6           \n\t"   /* Merge MSB bytes with LSB bytes after shift */            \
"CP0     w3                \n\t"                                                                \
"BRA     NZ,sqrt32_Qxx_continue_%=   \n\t"                                                      \
"FF1l    %[input1],w3       \n\t"     /* Find first bit =1 from left il LSB                 */  \
"SL      %[input1],w3,w6    \n\t"     /* apply shift on MSB                                 */  \
"ADD     #16,w3            \n\t"      /* equivalent to a shift on 32 bit value by 16 + w3   */  \
" sqrt32_Qxx_continue_%= :  \n\t"     /* continue                                           */  \
"PUSH    CORCON             \n\t"     /* Set up Accmulator for unsigned ops                 */  \
"MOV     #0x10F1, w4        \n\t"                                                               \
"MOV     w4,CORCON          \n\t"                                                               \
"MOV     #46333, w4         \n\t"     /* p1 = 0.706984569893393                             */  \
"MPY     w4*w6, %[A]        \n\t"     /* A = p1 * x                                         */  \
"MUL.UU  w6,w6,w4           \n\t"     /* x^2                                                */  \
"MOV     #11467, w4          \n\t"    /* p2 = −0.175010736333206                            */  \
"MSC     w4*w5,%[A]         \n\t"     /* A += p2 * x^2                                      */  \
"MUL.UU  w6,w5,w4           \n\t"     /* x^3                                                */  \
"MOV     #5202, w4          \n\t"     /* p3 =  0.0793782031396404                           */  \
"MAC     w4*w5,%[A]         \n\t"     /* A += p3 * x^3                                      */  \
"MUL.UU  w6,w5,w4           \n\t"     /* x^4                                                */  \
"MOV     #2154, w4          \n\t"     /* p4 = −0.0328723080456257                           */  \
"MSC     w4*w5,%[A]         \n\t"     /* A += p4 * x^4                                      */  \
"MUL.UU  w6,w5,w4           \n\t"     /* x^5                                                */  \
"MOV     #479, w4           \n\t"     /* p5 =  0.00731084850849584                          */  \
"MAC     w4*w5,%[A]         \n\t"     /* A += p5 * x^5                                      */  \
"MOV     #23170,w4          \n\t"     /* sqrt(2)/2 in Q1.15                                 */  \
"ADD     w4,#-2,%[A]        \n\t"     /* A += sqrt(2)/2 * 2                                 */  \
"ADD     w3,%[input2],w3    \n\t"     /* Add shift required by input/output scaling ratio   */  \
"LSR     w3,#1,w5           \n\t"     /* divide by 2                                        */  \
"SFTAC   %[A],w5            \n\t"     /* Shift A. ! Shift within [-16 16] otherwie trap     */  \
"SAC.R   %[A],#1, w7        \n\t"     /* Extract A/2                                        */  \
"MOV     #0,w6              \n\t"                                                               \
"MOV     #46341,w4          \n\t"     /* sqrt(2)/2 in Q0.16                                 */  \
"BTSC    w3,#0              \n\t"     /* if shift is odd factof of  multiple of sqrt(2)     */  \
"MUL.SU  w7,w4,w6           \n\t"     /* multiply by sqrt(2)/2                              */  \
"LSR     w6,#15,w6          \n\t"     /* no impact if mult is not executed (w6=0)           */  \
"ADD     w6,w7,%[Result]    \n\t"     /* take care of LSB if mult occurs                    */  \
"POP     CORCON             \n\t"                                                               \
: [Result] "=r"(out), [A] "=w"(Acc)                                                             \
: [input1] "r"(x) , [input2]"r"(y)                                                              \
: "w3","w4","w5","w6","w7","cc");                                                               \
}                                                                                               \
out;                                                                                            \
})    




 #define MCHP_MACRO_sqrt32_Qxx_Sat(u1,u2,u3,rawOutputSat)                                       \
({                                                                                              \
short _Accum Acc;                                                                               \
long int x = (long int) (u1);                                                                   \
register unsigned int y = (unsigned int) (u2);                                                  \
volatile register int out = 0;                                                                  \
if (x >= ((long int) u3)){                                                                      \
    out = rawOutputSat;                                                                         \
}                                                                                               \
else if (x > 0){                                                                                \
__asm__ volatile(                                                                               \
"FF1l    %d[input1],w3       \n\t"  /* Find first bit =1 from left ( no bit is 0, bit 15 is 1) */ \
"SL      %d[input1],w3,w5    \n\t"  /* apply shift on MSB */                                    \
"SUBR    w3,#16,w6           \n\t"  /* Extract bytes from LSB */                                \
"LSR     %[input1],w6,w6    \n\t"   /* This is hardly required, affect only input values 1,2 and 3 */   \
"ADD     w5,w6,w6           \n\t"   /* Merge MSB bytes with LSB bytes after shift */            \
"CP0     w3                \n\t"                                                                \
"BRA     NZ,sqrt32_Qxx_Sat_continue_%=   \n\t"                                                  \
"FF1l    %[input1],w3       \n\t"     /* Find first bit =1 from left il LSB                 */  \
"SL      %[input1],w3,w6    \n\t"     /* apply shift on MSB                                 */  \
"ADD     #16,w3            \n\t"      /* equivalent to a shift on 32 bit value by 16 + w3   */  \
" sqrt32_Qxx_Sat_continue_%= :  \n\t" /* continue                                           */  \
"PUSH    CORCON             \n\t"     /* Set up Accmulator for unsigned ops                 */  \
"MOV     #0x10F1, w4        \n\t"                                                               \
"MOV     w4,CORCON          \n\t"                                                               \
"MOV     #46333, w4         \n\t"     /* p1 = 0.706984569893393                             */  \
"MPY     w4*w6, %[A]        \n\t"     /* A = p1 * x                                         */  \
"MUL.UU  w6,w6,w4           \n\t"     /* x^2                                                */  \
"MOV     #11467, w4          \n\t"    /* p2 = −0.175010736333206                            */  \
"MSC     w4*w5,%[A]         \n\t"     /* A += p2 * x^2                                      */  \
"MUL.UU  w6,w5,w4           \n\t"     /* x^3                                                */  \
"MOV     #5202, w4          \n\t"     /* p3 =  0.0793782031396404                           */  \
"MAC     w4*w5,%[A]         \n\t"     /* A += p3 * x^3                                      */  \
"MUL.UU  w6,w5,w4           \n\t"     /* x^4                                                */  \
"MOV     #2154, w4          \n\t"     /* p4 = −0.0328723080456257                           */  \
"MSC     w4*w5,%[A]         \n\t"     /* A += p4 * x^4                                      */  \
"MUL.UU  w6,w5,w4           \n\t"     /* x^5                                                */  \
"MOV     #479, w4           \n\t"     /* p5 =  0.00731084850849584                          */  \
"MAC     w4*w5,%[A]         \n\t"     /* A += p5 * x^5                                      */  \
"MOV     #23170,w4          \n\t"     /* sqrt(2)/2 in Q1.15                                 */  \
"ADD     w4,#-2,%[A]        \n\t"     /* A += sqrt(2)/2 * 2                                 */  \
"ADD     w3,%[input2],w3    \n\t"     /* Add shift required by input/output scaling ratio   */  \
"LSR     w3,#1,w5           \n\t"     /* divide by 2                                        */  \
"SFTAC   %[A],w5            \n\t"     /* Shift A. ! Shift within [-16 16] otherwie trap     */  \
"SAC.R   %[A],#1, w7        \n\t"     /* Extract A/2                                        */  \
"MOV     #0,w6              \n\t"                                                               \
"MOV     #46341,w4          \n\t"     /* sqrt(2)/2 in Q0.16                                 */  \
"BTSC    w3,#0              \n\t"     /* if shift is odd factof of  multiple of sqrt(2)     */  \
"MUL.SU  w7,w4,w6           \n\t"     /* multiply by sqrt(2)/2                              */  \
"LSR     w6,#15,w6          \n\t"     /* no impact if mult is not executed (w6=0)           */  \
"ADD     w6,w7,%[Result]    \n\t"     /* take care of LSB if mult occurs                    */  \
"POP     CORCON             \n\t"                                                               \
: [Result] "=r"(out), [A] "=w"(Acc)                                                             \
: [input1] "r"(x) , [input2]"r"(y)                                                              \
: "w3","w4","w5","w6","w7","cc");                                                               \
}                                                                                               \
out;                                                                                            \
})    
        
#define MCHP_MACRO_sqrt_Q15(u1)         \
({                                      \
short _Accum Acc;                       \
int x = (int) (u1);                     \
volatile int out = 0;                   \
if (x > 0){                             \
__asm__ volatile(                       \
"FF1l    %[input1],w3       \n\t"       \
"SL      %[input1],w3,w6    \n\t"       \
"PUSH    CORCON             \n\t"       \
"MOV     #0x10F1, w4        \n\t"       \
"MOV     w4,CORCON          \n\t"       \
"MOV     #46333, w4         \n\t"     /* p1 = 0.706984569893393                             */  \
"MPY     w4*w6, %[A]        \n\t"     /* A = p1 * x                                         */  \
"MUL.UU  w6,w6,w4           \n\t"     /* x^2                                                */  \
"MOV     #11467, w4          \n\t"    /* p2 = −0.175010736333206                            */  \
"MSC     w4*w5,%[A]         \n\t"     /* A += p2 * x^2                                      */  \
"MUL.UU  w6,w5,w4           \n\t"     /* x^3                                                */  \
"MOV     #5202, w4          \n\t"     /* p3 =  0.0793782031396404                           */  \
"MAC     w4*w5,%[A]         \n\t"     /* A += p3 * x^3                                      */  \
"MUL.UU  w6,w5,w4           \n\t"     /* x^4                                                */  \
"MOV     #2154, w4          \n\t"     /* p4 = −0.0328723080456257                           */  \
"MSC     w4*w5,%[A]         \n\t"     /* A += p4 * x^4                                      */  \
"MUL.UU  w6,w5,w4           \n\t"     /* x^5                                                */  \
"MOV     #479, w4           \n\t"     /* p5 =  0.00731084850849584                          */  \
"MAC     w4*w5,%[A]         \n\t"     /* A += p5 * x^5                                      */  \
"MOV     #23170,w4          \n\t"     /* sqrt(2)/2 in Q1.15                                 */  \
"ADD     w4,#-2,%[A]        \n\t"     /* A += sqrt(2)/2 * 2                                 */  \
"LSR     w3,#1,w5           \n\t"     /* divide by 2                                        */  \
"SFTAC   %[A],w5            \n\t"     /* Shift A. ! Shift within [-16 16] otherwie trap     */  \
"SAC.R   %[A],#1, w7        \n\t"     /* Extract A/2                                        */  \
"MOV     #0,w6              \n\t"                                                               \
"MOV     #46341,w4          \n\t"     /* sqrt(2)/2 in Q0.16                                 */  \
"BTSC    w3,#0              \n\t"     /* if shift is odd factof of  multiple of sqrt(2)     */  \
"MUL.SU  w7,w4,w6           \n\t"     /* multiply by sqrt(2)/2                              */  \
"LSR     w6,#15,w6          \n\t"     /* no impact if mult is not executed (w6=0)           */  \
"ADD     w6,w7,%[Result]    \n\t"     /* take care of LSB if mult occurs                    */  \
"POP     CORCON             \n\t"                                                               \
: [Result] "=r"(out), [A] "=w"(Acc)     \
: [input1] "r"(x)                       \
: "w3","w4","w5","w6","w7","cc");       \
}                                       \
out;                                    \
}) 



#define MCHP_MACRO_sqrt_Qxx(u1,u2)      \
({                                      \
short _Accum Acc;                       \
int x = (int) (u1);                     \
int y = (int) (u2);                     \
volatile int out = 0;                   \
if (x > 0){                             \
__asm__ volatile(                       \
"FF1l    %[input1],w3       \n\t"       \
"SL      %[input1],w3,w6    \n\t"       \
"PUSH    CORCON             \n\t"       \
"MOV     #0x10F1, w4        \n\t"       \
"MOV     w4,CORCON          \n\t"       \
"MOV     #46333, w4         \n\t"     /* p1 = 0.706984569893393                             */  \
"MPY     w4*w6, %[A]        \n\t"     /* A = p1 * x                                         */  \
"MUL.UU  w6,w6,w4           \n\t"     /* x^2                                                */  \
"MOV     #11467, w4          \n\t"    /* p2 = −0.175010736333206                            */  \
"MSC     w4*w5,%[A]         \n\t"     /* A += p2 * x^2                                      */  \
"MUL.UU  w6,w5,w4           \n\t"     /* x^3                                                */  \
"MOV     #5202, w4          \n\t"     /* p3 =  0.0793782031396404                           */  \
"MAC     w4*w5,%[A]         \n\t"     /* A += p3 * x^3                                      */  \
"MUL.UU  w6,w5,w4           \n\t"     /* x^4                                                */  \
"MOV     #2154, w4          \n\t"     /* p4 = −0.0328723080456257                           */  \
"MSC     w4*w5,%[A]         \n\t"     /* A += p4 * x^4                                      */  \
"MUL.UU  w6,w5,w4           \n\t"     /* x^5                                                */  \
"MOV     #479, w4           \n\t"     /* p5 =  0.00731084850849584                          */  \
"MAC     w4*w5,%[A]         \n\t"     /* A += p5 * x^5                                      */  \
"MOV     #23170,w4          \n\t"     /* sqrt(2)/2 in Q1.15                                 */  \
"ADD     w4,#-2,%[A]        \n\t"     /* A += sqrt(2)/2 * 2                                 */  \
"ADD     w3,%[input2],w3    \n\t"     /* Add shift required by input/output scaling ratio   */  \
"LSR     w3,#1,w5           \n\t"     /* divide by 2                                        */  \
"SFTAC   %[A],w5            \n\t"     /* Shift A. ! Shift within [-16 16] otherwie trap     */  \
"SAC.R   %[A],#1, w7        \n\t"     /* Extract A/2                                        */  \
"MOV     #0,w6              \n\t"                                                               \
"MOV     #46341,w4          \n\t"     /* sqrt(2)/2 in Q0.16                                 */  \
"BTSC    w3,#0              \n\t"     /* if shift is odd factof of  multiple of sqrt(2)     */  \
"MUL.SU  w7,w4,w6           \n\t"     /* multiply by sqrt(2)/2                              */  \
"LSR     w6,#15,w6          \n\t"     /* no impact if mult is not executed (w6=0)           */  \
"ADD     w6,w7,%[Result]    \n\t"     /* take care of LSB if mult occurs                    */  \
"POP     CORCON             \n\t"                                                               \
: [Result] "=r"(out), [A] "=w"(Acc)     \
: [input1] "r"(x), [input2]"r"(y)       \
: "w3","w4","w5","w6","w7","cc");       \
}                                       \
out;                                    \
})                                      \




#define MCHP_MACRO_sqrt_Qxx_Sat(u1,u2,u3,rawOutputSat)   \
({                                          \
short _Accum Acc;                           \
int x = (int) (u1);                         \
int y = (int) (u2);                         \
int z = (int) (u3);                         \
volatile int out = 0;                       \
if (x >= z){                                \
    out = rawOutputSat;                     \
}                                           \
else if (x > 0){                            \
__asm__ volatile(                           \
"FF1l    %[input1],w3       \n\t"           \
"SL      %[input1],w3,w6    \n\t"           \
"PUSH    CORCON             \n\t"           \
"MOV     #0x10F1, w4        \n\t"           \
"MOV     w4,CORCON          \n\t"           \
"MOV     #46333, w4         \n\t"     /* p1 = 0.706984569893393                             */  \
"MPY     w4*w6, %[A]        \n\t"     /* A = p1 * x                                         */  \
"MUL.UU  w6,w6,w4           \n\t"     /* x^2                                                */  \
"MOV     #11467, w4          \n\t"    /* p2 = −0.175010736333206                            */  \
"MSC     w4*w5,%[A]         \n\t"     /* A += p2 * x^2                                      */  \
"MUL.UU  w6,w5,w4           \n\t"     /* x^3                                                */  \
"MOV     #5202, w4          \n\t"     /* p3 =  0.0793782031396404                           */  \
"MAC     w4*w5,%[A]         \n\t"     /* A += p3 * x^3                                      */  \
"MUL.UU  w6,w5,w4           \n\t"     /* x^4                                                */  \
"MOV     #2154, w4          \n\t"     /* p4 = −0.0328723080456257                           */  \
"MSC     w4*w5,%[A]         \n\t"     /* A += p4 * x^4                                      */  \
"MUL.UU  w6,w5,w4           \n\t"     /* x^5                                                */  \
"MOV     #479, w4           \n\t"     /* p5 =  0.00731084850849584                          */  \
"MAC     w4*w5,%[A]         \n\t"     /* A += p5 * x^5                                      */  \
"MOV     #23170,w4          \n\t"     /* sqrt(2)/2 in Q1.15                                 */  \
"ADD     w4,#-2,%[A]        \n\t"     /* A += sqrt(2)/2 * 2                                 */  \
"ADD     w3,%[input2],w3    \n\t"     /* Add shift required by input/output scaling ratio   */  \
"LSR     w3,#1,w5           \n\t"     /* divide by 2                                        */  \
"SFTAC   %[A],w5            \n\t"     /* Shift A. ! Shift within [-16 16] otherwie trap     */  \
"SAC.R   %[A],#1, w7        \n\t"     /* Extract A/2                                        */  \
"MOV     #0,w6              \n\t"                                                               \
"MOV     #46341,w4          \n\t"     /* sqrt(2)/2 in Q0.16                                 */  \
"BTSC    w3,#0              \n\t"     /* if shift is odd factof of  multiple of sqrt(2)     */  \
"MUL.SU  w7,w4,w6           \n\t"     /* multiply by sqrt(2)/2                              */  \
"LSR     w6,#15,w6          \n\t"     /* no impact if mult is not executed (w6=0)           */  \
"ADD     w6,w7,%[Result]    \n\t"     /* take care of LSB if mult occurs                    */  \
"POP     CORCON             \n\t"                                                               \
: [Result] "=r"(out), [A] "=w"(Acc)         \
: [input1] "r"(x), [input2]"r"(y)           \
: "w3","w4","w5","w6","w7","cc");           \
}                                           \
out;                                        \
})                                          \


#endif
