/************************************************************************************************
; MPLAB 16-Bit Device Blocks for Simulink                                                       *
;                                                                                               *
; Product Page:  http://www.microchip.com/Developmenttools/ProductDetails.aspx?PartNO=SW007023  *
;         Forum: http://www.microchip.com/forums/f192.aspx                                      *
;         Wiki:  http://microchipdeveloper.com/simulink:start                                   *
;                                                                                               *
; Author:        Lubin Kerhuel, Constantin Velea                                                *
; Company:       Microchip Technology Inc                                                       * 
; Filename:      MCHP_MACRO_AddSumSat.h                                                         *
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
#ifndef MCHP_16bit_ADD_SUB_MACRO_H
#define MCHP_16bit_ADD_SUB_MACRO_H

#include <xc.h>
#include "MCHP_MACRO_Saturate.h"

#define MCHP_addSat_CAO_s16_s16_s16(x,y)                                                                \
({                                                                                                      \
register int a = (int) (x);                                                                             \
register int b = (int) (y);                                                                             \
register int c;                                                                                         \
__asm__ volatile(                                                                                       \
"add    %[u1],%[u2],%[u3]   \n\t"    /* Add 16 bits values */                                           \
"bra    NOV, add_end_%=     \n\t"    /* exit if no overflow. */                                         \
"mov    #0x8000, %[u3]      \n\t"    /* saturate to neg (0x8000) */                                     \
"btss   %[u2],#15           \n\t"    /* or pos max (0x7FFF) */                                          \
"mov    #0x7FFF,%[u3]       \n\t"    /* depending on (both) operand sign. */                            \
" add_end_%= :              \n\t"    /* overflow can occur only when both operands have same sign */    \
: [u3] "=&r"(c)                                                                                         \
: [u1] "r"(a) , [u2] "r"(b)                                                                             \
: "cc");                                                                                                \
c;                                                                                                      \
})  

#define MCHP_addSat_CAO_u16_u16_u16(x,y)  /* uint16 (with Saturation) <= uint16 + uint16*/      \
({                                                                                              \
register unsigned int a = (unsigned int) (x);                                                   \
register unsigned int b = (unsigned int) (y);                                                   \
register unsigned int c;                                                                        \
__asm__ volatile(                                                                               \
"add    %[u1],%[u2],%[u3]   \n\t"     /* add 16 bits parts */                                   \
"btsc    SR,#0              \n\t"     /* if Carry flag is set */                                \
"SETM    %[u3]              \n\t"     /* set output to MAX_UINT16 saturation: 0xFFFF */         \
: [u3] "=r"(c)                                                                                  \
: [u1] "r"(a) , [u2] "r"(b)                                                                     \
: "cc");                                                                                        \
c;                                                                                              \
})                                                                                              \

#define MCHP_addSat_CAO_s32_s32_s32(x,y) /* int32(with saturation) <= int32 + int32 */                      \
({                                                                                                          \
register long int a = (long int) (x);                                                                       \
register long int b = (long int) (y);                                                                       \
register long int c;                                                                                        \
register int tmpMCHP;                                                                                       \
__asm__ volatile(                                                                                           \
"add    %[u1],%[u2],%[u3]       \n\t"    /* add Lo 16 bits parts */                                         \
"addc   %d[u1],%d[u2],%d[u3]    \n\t"    /* add Hi 16 bits with repported carry */                          \
"bra    NOV, add_end_%=         \n\t"    /* exit if no overflow */                                          \
"mov    #0x7FFF,%d[u3]          \n\t"    /* Set Positive Saturation */                                      \
"mov    #0xFFFF,%[u3]           \n\t"    /* to 0x7FFF FFFF */                                               \
"lsr    %d[u1],#15,%[tmppMCHP]  \n\t"    /* if (both) operands negative (sign bit)*/                        \
"add    %[tmppMCHP],%[u3],%[u3] \n\t"    /* add sign bit (1) to obtain  */                                  \
"addc   %d[u3],#0,%d[u3]        \n\t"    /* negative saturation -1 = 0x8000 0000 */                         \
" add_end_%= :                  \n\t"    /* overflow can occur only when both operands have same sign */    \
: [u3] "=&r"(c) ,[tmppMCHP] "=r"(tmpMCHP)                                                                   \
: [u1] "r"(a) , [u2] "r"(b)                                                                                 \
: "cc");                                                                                                    \
c;                                                                                                          \
})    

#define MCHP_addSat_CAO_u32_u32_u32(x,y)     /* uint32 (with saturation) <= uint32 + uint32*/               \
({                                                                                                          \
register unsigned long int a = (unsigned long int) (x);                                                     \
register unsigned long int b = (unsigned long int) (y);                                                     \
register unsigned long int c;                                                                               \
__asm__ volatile(                                                                                           \
"add    %[u1],%[u2],%[u3]       \n\t"    /* add Lo 16 bits parts */                                         \
"addc   %d[u1],%d[u2],%d[u3]    \n\t"    /* add Hi 16 bits with repported carry */                          \
"bra    NC, add_end_%=          \n\t"    /* exit if Carry flag is clear */                                  \
"mov    #0xFFFF, %d[u3]         \n\t"    /* Set Positive Saturation */                                      \
"mov    #0xFFFF, %[u3]          \n\t"    /* to 0xFFFF FFFF */                                               \
" add_end_%= :                  \n\t"    /* overflow can occur only when both operands have same sign */    \
: [u3] "=r"(c)                                                                                              \
: [u1] "r"(a) , [u2] "r"(b)                                                                                 \
: "cc");                                                                                                    \
c;                                                                                                          \
})           

#define MCHP_addSat_CAO_u32_u32_u16(x,y)    /* uint32 (with saturation) <= uint32 + uint16  */              \
({                                                                                                          \
register unsigned long int  a = (unsigned long int) (x);                                                    \
register unsigned short int b = (unsigned short int)(y);                                                    \
register unsigned long int  c;                                                                              \
__asm__ volatile(                                                                                           \
"add    %[u1],%[u2],%[u3]       \n\t"    /* add Lo 16 bits parts */                                         \
"addc   %d[u1],#0,%d[u3]        \n\t"    /* add Hi 16 bits with repported carry */                          \
"bra    NC, add_end_%=          \n\t"    /* exit if Carry flag is clear */                                  \
"mov    #0xFFFF,%d[u3]          \n\t"    /* Set Positive Saturation */                                      \
"mov    #0xFFFF,%[u3]           \n\t"    /* to 0xFFFF FFFF */                                               \
" add_end_%= :                  \n\t"    /* overflow can occur only when both operands have same sign */    \
: [u3] "=r"(c)                                                                                              \
: [u1] "r"(a) , [u2] "r"(b)                                                                                 \
: "cc");                                                                                                    \
c;                                                                                                          \
})    

#define MCHP_addSat_CAO_u32_u32_s16(x,y)     /* uint32 (with saturation) <= uint32 + int16  */                          \
({                                                                                                                      \
/* Side remark: Mixed-sign addition never turns on the overflow flag */                                                 \
register unsigned long int  a = (unsigned long int) (x);                                                                \
register long int  b = (long int)(y);       /* s16 operand will be sign extended */                                     \
register unsigned long int  c;                                                                                          \
register unsigned long int tmpMCHP;                                                                                     \
__asm__ volatile(                                                                                                       \
"btsc   %d[u2], #15                         \n\t"   /* check if s16 is negative */                                      \
"bra    secondOpearandIsNeg_%=              \n\t"   /* branch if s16 is negative */                                     \
"add    %[u1],%[u2],%[u3]                   \n\t"   /* add Lo 16 bits parts */                                          \
"addc   %d[u1],%d[u2],%d[u3]                \n\t"   /* add Hi 16 bits with reported carry */                            \
"bra    NC, add_end_%=                      \n\t"   /* exit if Carry flag is clear */                                   \
"mov    #0xFFFF,%d[u3]                      \n\t"   /* set Positive Saturation */                                       \
"mov    #0xFFFF,%[u3]                       \n\t"   /* to 0xFFFF FFFF */                                                \
"bra    add_end_%=                          \n\t"   /* exit */                                                          \
" secondOpearandIsNeg_%= :                  \n\t"   /* if s16 is negative */                                            \
"NEG    %[u2],%[tmppMCHP]                   \n\t"   /* compute abs(s16) */                                              \
"BTSC   %[tmppMCHP],#15                     \n\t"                                                                       \
"COM    %[tmppMCHP],%[tmppMCHP]             \n\t"                                                                       \
"SUB    %[tmppMCHP],%[u1],%d[tmppMCHP]      \n\t"   /* check if u32 is greater than abs(s16) */                         \
"mov    #0x0000,%d[tmppMCHP]                \n\t"   /* use MSW of "temp" variable as a dummy reg */                     \
"SUBB   %d[tmppMCHP],%d[u1],%d[tmppMCHP]    \n\t"                                                                       \
"BRA    NC,firstOperandIsGreater_%=         \n\t"   /* if uint32 is greater than abs(s16), skip saturation with zero */ \
"mov    #0x0000,%[u3]                       \n\t"   /* saturate result to zero (LSW of the result)*/                    \
"mov    #0x0000,%d[u3]                      \n\t"   /* saturate result to zero (MSW of the result)*/                    \
"bra    add_end_%=                          \n\t"   /* exit */                                                          \
" firstOperandIsGreater_%= :                \n\t"   /* if uint32 is greater than abs(s16)*/                             \
"add    %[u1], %[u2], %[u3]                 \n\t"   /* add Lo 16 bits parts */                                          \
"addc   %d[u1], %d[u2], %d[u3]              \n\t"   /* add Hi 16 bits with reported carry */                            \
" add_end_%= :                              \n\t"   /* overflow can occur only when both operands have same sign */     \
: [u3] "=&r"(c) ,[tmppMCHP] "=&r"(tmpMCHP)                                                                              \
: [u1] "r"(a) , [u2] "r"(b)                                                                                             \
: "cc");                                                                                                                \
c;                                                                                                                      \
}) 

// Addition with saturation for Cast-Before-Operation (Simulink default behaviour for 32bit-mixed datatypes addition)
#define MCHP_addSat_CBO_u32_u32_s16(x,y)     /* uint32 (with saturation) <= uint32 + int16  */                                                                              \
({                                                                                                                                                                          \
/* Side remark: Mixed-sign addition never turns on the overflow flag */                                                                                                     \
register unsigned long int  a = (unsigned long int) (x);                                                                                                                    \
register long int  b = (long int)(y);       /* s16 operand will be sign extended */                                                                                         \
register unsigned long int  c;                                                                                                                                              \
__asm__ volatile(                                                                                                                                                           \
"btsc   %[u2],#15                   \n\t"   /* check if s16 is negative */                                                                                                  \
"bra    secondOpearandIsNeg_%=      \n\t"   /* branch if s16 is negative */                                                                                                 \
"add    %[u1],%[u2],%[u3]           \n\t"   /* add Lo 16 bits parts */                                                                                                      \
"addc   %d[u1],%d[u2],%d[u3]        \n\t"   /* add Hi 16 bits with reported carry */                                                                                        \
"bra    NC, add_end_%=              \n\t"   /* exit if Carry flag is clear */                                                                                               \
"mov    #0xFFFF,%d[u3]              \n\t"   /* set Positive Saturation */                                                                                                   \
"mov    #0xFFFF,%[u3]               \n\t"   /* to 0xFFFF FFFF */                                                                                                            \
"bra    add_end_%=                  \n\t"   /* exit */                                                                                                                      \
" secondOpearandIsNeg_%= :          \n\t"   /* if s16 is negative */                                                                                                        \
"mov    %[u1],%[u3]                 \n\t"   /* s16 will be set to zero before operation takes place (Simulink behaviour) and the result of the addition is u32 value */     \
"mov    %d[u1],%d[u3]               \n\t"   /* saturate result to zero (MSW of the result)*/                                                                                \
" add_end_%= :                      \n\t"   /* end */                                                                                                                       \
: [u3] "=&r"(c)                                                                                                                                                             \
: [u1] "r"(a) , [u2] "r"(b)                                                                                                                                                 \
: "cc");                                                                                                                                                                    \
c;                                                                                                                                                                          \
}) 

#define MCHP_addSat_CAO_s32_u32_s16(x,y)  /* int32 (with saturation) <= uint32 + int16 */                   \
({                                                                                                          \
/* Side remark:  the saturation can only be positive, no underflow can possibly occur */                    \
register unsigned long int a = (unsigned long int) (x);                                                     \
register long int b = (long int)(y);       /* s16 operand will be sign extended */                          \
register long int c =0;                                                                                     \
__asm__ volatile(                                                                                           \
"btsc   %d[u1],#15                  \n\t"   /* add Lo 16 bits parts */                                      \
"bra    saturate_%=                 \n\t"   /* add Lo 16 bits parts */                                      \
"add    %[u1],%[u2],%[u3]           \n\t"   /* add Lo 16 bits parts */                                      \
"addc   %d[u1],%d[u2],%d[u3]        \n\t"   /* add Hi 16 bits with reported carry */                        \
"bra    NOV, add_end_%=             \n\t"   /* exit if no Carry occurred */                                 \
" saturate_%= :                     \n\t"   /* saturate */                                                  \
"mov    #0x7FFF,%d[u3]              \n\t"   /* Set int32 MAX Saturation */                                  \
"mov    #0xFFFF,%[u3]               \n\t"   /* to 0x7FFF FFFF */                                            \
" add_end_%= :                      \n\t"   /* overflow can occur only when both operands have same sign */ \
: [u3] "=&r"(c)                                                                                             \
: [u1] "r"(a) , [u2] "r"(b)                                                                                 \
: "cc");                                                                                                    \
c;                                                                                                          \
}) 


// Addition with saturation for Cast-Before-Operation (Simulink default behaviour for 32bit-mixed datatypes addition)
#define MCHP_addSat_CBO_u32_s32_u16(x,y)  /* int32 (with saturation) <= uint32 + int16 */                                                                                   \
({                                                                                                                                                                          \
/* Side remark: Mixed-sign addition never turns on the overflow flag */                                                                                                     \
register long int  a = (long int) (x);                                                                                                                                      \
register unsigned long int  b = (unsigned long int)(y); /* s16 operand will be sign extended */                                                                             \
register unsigned long int  c;                                                                                                                                              \
__asm__ volatile(                                                                                                                                                           \
"btsc   %d[u1],#15                  \n\t"   /* check if s16 is negative */                                                                                                  \
"bra    firstOpearandIsNeg_%=       \n\t"   /* branch if s16 is negative */                                                                                                 \
"add    %[u1],%[u2],%[u3]           \n\t"   /* add Lo 16 bits parts */                                                                                                      \
"addc   %d[u1], %d[u2], %d[u3]      \n\t"   /* add Hi 16 bits with reported carry */                                                                                        \
"bra    NC, add_end_%=              \n\t"   /* exit if Carry flag is clear */                                                                                               \
"mov    #0xFFFF,%d[u3]              \n\t"   /* set Positive Saturation */                                                                                                   \
"mov    #0xFFFF,%[u3]               \n\t"   /* to 0xFFFF FFFF */                                                                                                            \
"bra    add_end_%=                  \n\t"   /* exit */                                                                                                                      \
" firstOpearandIsNeg_%= :           \n\t"   /* if s16 is negative */                                                                                                        \
"mov    %[u2],%[u3]                 \n\t"   /* s16 will be set to zero before operation takes place (Simulink behaviour) and the result of the addition is u32 value */     \
"mov    %d[u2],%d[u3]               \n\t"   /* saturate result to zero (MSW of the result)*/                                                                                \
" add_end_%= :                      \n\t"   /* end */                                                                                                                       \
: [u3] "=&r"(c)                                                                                                                                                             \
: [u1] "r"(a) , [u2] "r"(b)                                                                                                                                                 \
: "cc");                                                                                                                                                                    \
c;                                                                                                                                                                          \
}) 


// Addition with saturation for Cast-Before-Operation (Simulink default behaviour for 32bit-mixed datatypes addition)
#define MCHP_addSat_CBO_u32_s32_s16(x,y)  /* int32 (with saturation) <= uint32 + int16 */                   \
({                                                                                                          \
register long int a = (long int) (x);                                                                       \
register long int b = (long int) (y);                                                                       \
register unsigned long int c = 0x0;                                                                         \
__asm__ volatile(                                                                                           \
"btsc   %d[u1],#15              \n\t"    /* test if s32 is negative */                                      \
"mov.d  %[u3],%[u1]             \n\t"    /* if s32 is negative, saturate it to Zero */                      \
"btsc   %d[u2],#15              \n\t"    /* test if s16 is negative */                                      \
"mov.d  %[u3],%[u2]             \n\t"    /* if s16 is negative, saturate it to Zero */                      \
"add    %[u1],%[u2],%[u3]       \n\t"    /* add Lo 16 bits parts */                                         \
"addc   %d[u1],%d[u2],%d[u3]    \n\t"    /* add Hi 16 bits with repported carry */                          \
" add_end_%= :                  \n\t"    /* overflow can occur only when both operands have same sign */    \
: [u3] "+r"(c)                                                                                              \
: [u1] "r"(a) , [u2] "r"(b)                                                                                 \
: "cc");                                                                                                    \
c;                                                                                                          \
})  



#define MCHP_addSat_CAO_u32_s32_u16(x,y)   /* uint32 (with saturation) <= int32 + uint16 */             \
({                                                                                                      \
register long int  a = (long int) (x);                                                                  \
register unsigned long int  b = (unsigned long int)(y);     /* s16 operand will be sign extended */     \
register unsigned long int  c;                                                                          \
__asm__ volatile(                                                                                       \
"btsc   %d[u1],#15                  \n\t"   /* check if s32 is negative */                              \
"bra    firstOpearandIsNeg_%=       \n\t"   /* branch if s32 is negative */                             \
"add    %[u1],%[u2],%[u3]           \n\t"   /* add Lo 16 bits parts */                                  \
"addc   %d[u1],%d[u2],%d[u3]        \n\t"   /* add Hi 16 bits with reported carry */                    \
"bra    NC, add_end_%=              \n\t"   /* exit if Carry flag is clear */                           \
"mov    #0xFFFF,%d[u3]              \n\t"   /* set Positive Saturation */                               \
"mov    #0xFFFF,%[u3]               \n\t"   /* to 0xFFFF FFFF */                                        \
"bra    add_end_%=                  \n\t"   /* exit */                                                  \
" firstOpearandIsNeg_%= :           \n\t"   /* if s32 is negative */                                    \
"add    %[u1],%[u2],%[u3]           \n\t"   /* add Lo 16 bits parts */                                  \
"addc   %d[u1],%d[u2],%d[u3]        \n\t"   /* add Hi 16 bits with reported carry */                    \
"bra    NN, add_end_%=              \n\t"   /* exit if result is not negative */                        \
"mov    #0x0000,%[u3]               \n\t"   /* if result is negative, saturate to zero */               \
"mov    #0x0000,%d[u3]              \n\t"   /* to 0x0000 0000 */                                        \
" add_end_%= :                      \n\t"   /* exit */                                                  \
: [u3] "=&r"(c)                                                                                         \
: [u1] "r"(a) , [u2] "r"(b)                                                                             \
: "cc");                                                                                                \
c;                                                                                                      \
})    


#define MCHP_addSat_CAO_u32_s32_s16(x,y) /* uint32 (with saturation) <= int32 + int16  */               \
({                                                                                                      \
register long int a = (long int) (x);                                                                   \
register long int b = (long int) (y);                                                                   \
register long int c;                                                                                    \
register long int tmpMCHP = 0x0;                                                                        \
__asm__ volatile(                                                                                       \
"add    %[u1],%[u2],%[u3]       \n\t"   /* add Lo 16 bits parts */                                      \
"addc   %d[u1],%d[u2],%d[u3]    \n\t"   /* add Hi 16 bits with repported carry */                       \
"bra    OV, overflow_%=         \n\t"   /* branch if overflow occurred*/                                \
"btsc   %d[u3],#15              \n\t"   /* otherwise, check if the result is negative */                \
"mov.d  %[tmppMCHP],%[u3]       \n\t"   /* if the result is negative, saturate the output to zero */    \
"bra    add_end_%=              \n\t"   /* exit */                                                      \
" overflow_%= :                 \n\t"   /* rountine to handle overflow case  */                         \
"btss   SR,#3                   \n\t"   /* in overflow condition, use N flag information. If N flag is clear, it means that OV occurred by adding two negative values */ \
"mov.d  %[tmppMCHP],%[u3]       \n\t"   /* saturate to zero if N flag is cleared */                     \
" add_end_%= :                  \n\t"   /* exit */                                                      \
: [u3] "+r"(c) ,[tmppMCHP] "+r"(tmpMCHP)                                                                \
: [u1] "r"(a) , [u2] "r"(b)                                                                             \
: "cc");                                                                                                \
c;                                                                                                      \
}) 

#define MCHP_addSat_CAO_s8_s8_s8(x,y)  /* signed char8 (with saturation) <= signed char8 + signed char8 */  \
({                                                                                                          \
register signed char a = (signed char) (x);                                                                 \
register signed char b = (signed char) (y);                                                                 \
register signed char c;                                                                                     \
__asm__ volatile(                                                                                           \
"add.b  %[u1],%[u2],%[u3]   \n\t"   /* Add 8 bits values */                                                 \
"bra    NOV, add_end_%=     \n\t"   /* exit if no overflow. */                                              \
"mov.b  #0x80,%[u3]         \n\t"   /* saturate to neg min (0x80) */                                        \
"btss   %[u2],#7            \n\t"   /* or pos max (0x7F) */                                                 \
"mov    #0x7F,%[u3]         \n\t"   /* depending on (both) operand sign. */                                 \
" add_end_%= :              \n\t"   /* overflow can occur only when both operands have same sign */         \
: [u3] "=&r"(c)                                                                                             \
: [u1] "r"(a) , [u2] "r"(b)                                                                                 \
: "cc");                                                                                                    \
c;                                                                                                          \
})  

#define MCHP_addSat_CAO_u8_u8_u8(x,y)  /* unsigned char8 (with saturation) <= unsigned char8 + unsigned char8 */    \
({                                                                                                                  \
register unsigned char a = (unsigned char) (x);                                                                     \
register unsigned char b = (unsigned char) (y);                                                                     \
register unsigned char c;                                                                                           \
__asm__ volatile(                                                                                                   \
"add.b   %[u1],%[u2],%[u3]      \n\t"       /* add 8 bits parts */                                                  \
"btsc    SR,#0                  \n\t"       /* if Carry flag is set */                                              \
"mov.b   #0xFF,%[u3]            \n\t"       /* set output to MAX_unsigned char8 saturation: 0x00FF */               \
: [u3] "=r"(c)                                                                                                      \
: [u1] "r"(a) , [u2] "r"(b)                                                                                         \
: "cc");                                                                                                            \
c;                                                                                                                  \
})                                                                                                                  \

/*--------------------------------------- Reversed-operands for primitive macros -------------------------------------------------------------------- */
#define MCHP_addSat_CAO_u32_u16_u32(x,y)    MCHP_addSat_CAO_u32_u32_u16(y,x)   /* uint32(with saturation) <= uint16 + uint32 */
#define MCHP_addSat_CAO_s32_s32_s16(x,y)    MCHP_addSat_CAO_s32_s32_s32(x,y)   /* int32 (with saturation) <= int32 + int16   */ 
#define MCHP_addSat_CAO_s32_s16_s32(x,y)    MCHP_addSat_CAO_s32_s32_s32(x,y)   /* int32 (with saturation) <= int16 + int32   */ 

/*------------------------------------------------ Special cases for addition ----------------------------------------------------------------------- */
#define MCHP_addSat_CAO_s32_s32_u16(x,y)    MCHP_addSat_CAO_s32_s32_s32(x,y)    /* int32 (with saturation) <= int32 + uint16  */ 
#define MCHP_addSat_CAO_s32_u16_s32(x,y)    MCHP_addSat_CAO_s32_s32_s32(x,y)    /* int32 (with saturation) <= uint16 + int32  */ 
#define MCHP_addSat_CBO_u32_s16_u32(x,y)    MCHP_addSat_CBO_u32_u32_s16(y,x)    /* uint32 (with saturation) <= int16 + uint32 */ 
#define MCHP_addSat_CAO_s32_s16_u32(x,y)    MCHP_addSat_CAO_s32_u32_s16(y,x)    /* int32 (with saturation) <= uint16 + int32  */
#define MCHP_addSat_CBO_u32_u16_s32(x,y)    MCHP_addSat_CBO_u32_s32_u16(y,x)    /* uint32 (with saturation) <= uint16 + int32 */
#define MCHP_addSat_CBO_u32_s16_s32(x,y)    MCHP_addSat_CBO_u32_s32_s16(y,x)    /* uint32 (with saturation) <= int16 + int32  */
#define MCHP_addSat_CAO_u32_s16_u32(x,y)    MCHP_addSat_CAO_u32_u32_s16(y,x)    /* uint32 (with saturation) <= int16 + uint32 */
#define MCHP_addSat_CAO_u32_u16_s32(x,y)    MCHP_addSat_CAO_u32_s32_u16(y,x)    /* uint32 (with saturation) <= uint16 + int32 */
#define MCHP_addSat_CAO_u32_s16_s32(x,y)    MCHP_addSat_CAO_u32_s32_s16(y,x)    /* uint32 (with saturation) <= int16 + int32  */

#define MCHP_addSat_CAO_s32_u32_u16(x,y)    MCHP_SATURATE_U32_TO_S32(MCHP_addSat_CAO_u32_u32_u16(x,y))  /* int32 (with saturation) <= uint32 + uint16 */
#define MCHP_addSat_CAO_s32_u16_u32(x,y)    MCHP_SATURATE_U32_TO_S32(MCHP_addSat_CAO_u32_u32_u32(y,x))  /* int32 (with saturation) <= uint16 + uint32 */

/*-------------------------------------------- 8-bit/16-bit mixed-sign addition --------------------------------------------------------------------- */
#define MCHP_addSat_CAO_s16_s8_s16(x,y)     MCHP_addSat_CAO_s16_s16_s16(x,y)   /* int16 (with saturation) <= int8 + int16    */
#define MCHP_addSat_CAO_s16_s16_s8(x,y)     MCHP_addSat_CAO_s16_s16_s16(x,y)   /* int16 (with saturation) <= int16 + int8    */
#define MCHP_addSat_CAO_u16_u8_u16(x,y)     MCHP_addSat_CAO_u16_u16_u16(x,y)   /* uint16 (with saturation) <= uint8 + uint16 */
#define MCHP_addSat_CAO_u16_u16_u8(x,y)     MCHP_addSat_CAO_u16_u16_u16(x,y)   /* uint16 (with saturation) <= uint16 + uint8 */

#define MCHP_addSat_CAO_s8_s8_s16(x,y)      MCHP_SATURATE_S16_TO_S8(MCHP_addSat_CAO_s16_s16_s16(x,y))  /* int8 (with saturation) <= int8 + int16      */

#define MCHP_addSat_CAO_s8_s16_s8(x,y)      MCHP_SATURATE_S16_TO_S8(MCHP_addSat_CAO_s16_s16_s16(x,y))  /* int8 (with saturation) <= int16 + int8      */
#define MCHP_addSat_CAO_s8_s16_s16(x,y)     MCHP_SATURATE_S16_TO_S8(MCHP_addSat_CAO_s16_s16_s16(x,y))  /* int8 (with saturation) <= int16 + int16     */
#define MCHP_addSat_CAO_s16_s32_s16(x,y)    MCHP_SATURATE_S32_TO_S16(MCHP_addSat_CAO_s32_s32_s16(x,y)) /* int16 (with saturation) <= int32 + int16    */
#define MCHP_addSat_CAO_s16_s16_s32(x,y)    MCHP_SATURATE_S32_TO_S16(MCHP_addSat_CAO_s32_s16_s32(x,y)) /* int16 (with saturation) <= int16 + int32    */
#define MCHP_addSat_CAO_s16_s32_s32(x,y)    MCHP_SATURATE_S32_TO_S16(MCHP_addSat_CAO_s32_s32_s32(x,y)) /* int16 (with saturation) <= int32 + int32    */

#define MCHP_addSat_CAO_u8_u8_u16(x,y)      MCHP_SATURATE_U16_TO_U8(MCHP_addSat_CAO_u16_u16_u16(x,y))  /* uint8 (with saturation) <= uint8 + uint16   */
#define MCHP_addSat_CAO_u8_u16_u8(x,y)      MCHP_SATURATE_U16_TO_U8(MCHP_addSat_CAO_u16_u16_u16(x,y))  /* uint8 (with saturation) <= uint16 + uint8   */
#define MCHP_addSat_CAO_u8_u16_u16(x,y)     MCHP_SATURATE_U16_TO_U8(MCHP_addSat_CAO_u16_u16_u16(x,y))  /* uint8 (with saturation) <= uint16 + uint16  */
#define MCHP_addSat_CAO_u16_u32_u32(x,y)    MCHP_SATURATE_U32_TO_U16(MCHP_addSat_CAO_u32_u32_u32(x,y)) /* uint16 (with saturation) <= uint32 + uint32 */
#define MCHP_addSat_CAO_u16_u16_u32(x,y)    MCHP_SATURATE_U32_TO_U16(MCHP_addSat_CAO_u32_u16_u32(x,y)) /* uint16 (with saturation) <= uint16 + uint32 */
#define MCHP_addSat_CAO_u16_u32_u16(x,y)    MCHP_SATURATE_U32_TO_U16(MCHP_addSat_CAO_u32_u32_u16(x,y)) /* uint16 (with saturation) <= uint32 + uint32 */




/*=====================================================================================================================================================*/
/*                                                  SUBTRACTION WITH SATURATION                                                                        */     
/*=====================================================================================================================================================*/

#define MCHP_subSat_CAO_s16_s16_s16(x,y) /* int16(with saturation) <= int16 - int16 */                      \
({                                                                                                          \
register int a = (int) (x);                                                                                 \
register int b = (int) (y);                                                                                 \
register int c;                                                                                             \
__asm__ volatile(                                                                                           \
"sub    %[u1],%[u2],%[u3]       \n\t"   /* subtract 16 bits values */                                       \
"bra    NOV, sub_end_%=         \n\t"   /* branch if underflow */                                           \
"mov    #0x7FFF,%[u3]           \n\t"   /* saturate to max int16 */                                         \
"btss   %[u2],#15               \n\t"   /* check if u2 is negative */                                       \
"mov    #0x8000,%[u3]           \n\t"   /* if u2 is negative, saturate to min int16 */                      \
" sub_end_%= :                  \n\t"   /* underflow can occur only when operands have opposite signs */    \
: [u3] "=&r"(c)                                                                                             \
: [u1] "r"(a) , [u2] "r"(b)                                                                                 \
: "cc");                                                                                                    \
c;                                                                                                          \
}) 


#define MCHP_subSat_CAO_u16_u16_u16(x,y)  /*uint16(with saturation)<=uint16 - uint16*/      \
({                                                                                          \
register unsigned int a = (unsigned int) (x);                                               \
register unsigned int b = (unsigned int) (y);                                               \
register unsigned int c;                                                                    \
__asm__ volatile(                                                                           \
"sub    %[u1],%[u2],%[u3]     \n\t"   /* subtract 16 bits values */                         \
"btss   SR,#0                 \n\t"   /* skip next instr if Carry(!Borrow) flag is set */   \
"clr    %[u3]                 \n\t"   /* otherwise, saturate to MIN uint16 */               \
: [u3] "=r"(c)                                                                              \
: [u1] "r"(a) , [u2] "r"(b)                                                                 \
: "cc");                                                                                    \
c;                                                                                          \
})


#define MCHP_subSat_CAO_s32_s32_s32(x,y)   /* int32(with saturation<= int32 - int32 */                              \
({                                                                                                                  \
/* Underflow can occur when opperands have opposite signs */                                                        \
register long int a = (long int) (x);                                                                               \
register long int b = (long int) (y);                                                                               \
register long int c;                                                                                                \
register int tmpMCHP;                                                                                               \
__asm__ volatile(                                                                                                   \
"sub    %[u1],%[u2],%[u3]           \n\t"    /* sub Lo 16 bits parts */                                             \
"subb   %d[u1],%d[u2],%d[u3]        \n\t"    /* sub Hi 16 bits with reported borrow */                              \
"bra    NOV, sub_end_%=             \n\t"    /* exit if no underflow. Carry negated flag is used as Borrow flag */  \
"mov    #0x7FFF,%d[u3]              \n\t"    /* Set NEG Saturation */                                               \
"mov    #0xFFFF,%[u3]               \n\t"    /* to 0x7FFF FFFF */                                                   \
"lsr    %d[u1],#15,%[tmppMCHP]      \n\t"    /* if (both) operands negative(sign bit)*/                             \
"add    %[tmppMCHP],%[u3],%[u3]     \n\t"    /* add sign bit (1) to obtain  */                                      \
"addc   %d[u3],#0,%d[u3]            \n\t"    /* positive saturation  = 0x7FFF FFFF */                               \
" sub_end_%= :                      \n\t"    /* underflow can occur only when operands have opposite signs */       \
: [u3] "=&r"(c) ,[tmppMCHP] "=r"(tmpMCHP)                                                                           \
: [u1] "r"(a) , [u2] "r"(b)                                                                                         \
: "cc");                                                                                                            \
c;                                                                                                                  \
})            
 

#define MCHP_subSat_CAO_u32_u32_u32(x,y)     /* uint32 (with saturation) <= uint32 - uint32*/                                                                           \
({                                                                                                                                                                      \
register unsigned long int a = (unsigned long int) (x);                                                                                                                 \
register unsigned long int b = (unsigned long int) (y);                                                                                                                 \
register unsigned long int c;                                                                                                                                           \
__asm__ volatile(                                                                                                                                                       \
"sub    %[u1],%[u2],%[u3]       \n\t"    /* sub Lo 16 bits parts */                                                                                                     \
"subb   %d[u1],%d[u2],%d[u3]    \n\t"    /* sub Hi 16 bits with reported borrow */                                                                                      \
"bra    C, sub_end_%=           \n\t"    /* saturate to MIN_UINT16 if Carry flag is cleared, exit otherwise. For sub instruction, this means that we had a Borrow */    \
"mov    #0x0000,%d[u3]          \n\t"    /* Set MAX_UINT16 Positive Saturation */                                                                                       \
"mov    #0x0000,%[u3]           \n\t"    /* to 0x0000 0000 */                                                                                                           \
" sub_end_%= :                  \n\t"    /* only underflow conditions can occur when subtracting two uint32 variables */                                                \
: [u3] "=r"(c)                                                                                                                                                          \
: [u1] "r"(a) , [u2] "r"(b)                                                                                                                                             \
: "cc");                                                                                                                                                                \
c;                                                                                                                                                                      \
})  


#define MCHP_subSat_CAO_s32_u16_u32(x,y)   /* int32(with saturation<= u16 - u32 */  \
({                                                                                  \
register unsigned long int a = (unsigned long int) (x);                             \
register unsigned long int b = (unsigned long int) (y);                             \
register long int c;                                                                \
__asm__ volatile(                                                                   \
"btsc   %d[u2],#15              \n\t"   /* check MSbit of u32 */                    \
"bra    secondOpHasMSb1_%=      \n\t"   /* branch if MSbit of u32 is set */         \
"sub    %[u1],%[u2],%[u3]       \n\t"   /* sub Lo 16 bits parts */                  \
"subb   %d[u1],%d[u2],%d[u3]    \n\t"   /* sub Hi 16 bits with reported borrow */   \
"bra    sub_end_%=              \n\t"   /* exit */                                  \
" secondOpHasMSb1_%= :          \n\t"   /* when MSbit of u32 is set */              \
"sub    %[u1],%[u2],%[u3]       \n\t"   /* sub Lo 16 bits parts */                  \
"subb   %d[u1],%d[u2],%d[u3]    \n\t"   /* sub Hi 16 bits with reported borrow */   \
"bra    N,sub_end_%=            \n\t"   /* exit if result is negative */            \
"mov    #0x8000,%d[u3]          \n\t"   /* otherwise, saturate to MIN_INT32  */     \
"mov    #0x0000,%[u3]           \n\t"   /* to 0x8000 0000 */                        \
" sub_end_%= :                  \n\t"                                               \
: [u3] "=&r"(c)                                                                     \
: [u1] "r"(a) , [u2] "r"(b)                                                         \
: "cc");                                                                            \
c;                                                                                  \
})            


#define MCHP_subSat_CAO_u32_u32_s16(x,y)    /* uint32 (with saturation) <= uint32 - int16  */                                                               \
/* this macro can also handle u32_u32_s32 */                                                                                                                \
({                                                                                                                                                          \
register unsigned long int  a = (unsigned long int) (x);                                                                                                    \
register long int  b = (long int)(y);       /* s16 operand will be sign extended */                                                                         \
register unsigned long int  c;                                                                                                                              \
__asm__ volatile(                                                                                                                                           \
"btsc   %d[u2],#15                  \n\t"   /* check if s16 is negative */                                                                                  \
"bra    secondOpearandIsNeg_%=      \n\t"   /* branch if s16 is negative */                                                                                 \
"sub    %[u1],%[u2],%[u3]           \n\t"   /* sub Lo 16 bits parts */                                                                                      \
"subb   %d[u1],%d[u2],%d[u3]        \n\t"   /* sub Hi 16 bits with reported borrow */                                                                       \
"bra    C,sub_end_%=                \n\t"   /* exit if no borrow occured. Otherwise, saturate if borrow occured */                                          \
"mov    #0x0000,%d[u3]              \n\t"   /* set MIN_UINT32 Saturation */                                                                                 \
"mov    #0x0000,%[u3]               \n\t"   /* to 0x0000 0000 */                                                                                            \
"bra    sub_end_%=                  \n\t"   /* exit */                                                                                                      \
" secondOpearandIsNeg_%= :          \n\t"   /* if s16 is negative */                                                                                        \
"sub    %[u1],%[u2],%[u3]           \n\t"   /* If second operand is negative, it basically means that we must add u32 and abs(s16) in unsigned_32 mode*/    \
"subb   %d[u1],%d[u2],%d[u3]        \n\t"                                                                                                                   \
"BRA    NC,sub_end_%=               \n\t"   /* if uint32 is greater than abs(s16), skip saturation to MAX */                                                \
"mov    #0xFFFF, %[u3]              \n\t"   /* saturate result to FFFF (LSW of the result)*/                                                                \
"mov    #0xFFFF, %d[u3]             \n\t"   /* saturate result to FFFF (MSW of the result)*/                                                                \
" sub_end_%= :                      \n\t"   /* overflow can occur only when both operands have same sign */                                                 \
: [u3] "=&r"(c)                                                                                                                                             \
: [u1] "r"(a) , [u2] "r"(b)                                                                                                                                 \
: "cc");                                                                                                                                                    \
c;                                                                                                                                                          \
})
   

#define MCHP_subSat_CAO_u32_s16_u32(x,y)    /* uint32 (with saturation) <= uint32 - int16  */                                                               \
({                                                                                                                                                          \
register long int  a = (long int) (x);                                                                                                                      \
register unsigned long int  b = (unsigned long int)(y);                                                                                                     \
register unsigned long int  c;                                                                                                                              \
__asm__ volatile(                                                                                                                                           \
"btsc   %d[u1],#15                  \n\t"   /* check if s16 is negative */                                                                                  \
"bra    firstOpearandIsNeg_%=       \n\t"   /* branch if s16 is negative */                                                                                 \
"sub    %[u1],%[u2],%[u3]           \n\t"   /* sub Lo 16 bits parts */                                                                                      \
"subb   %d[u1],%d[u2],%d[u3]        \n\t"   /* sub Hi 16 bits with reported borrow */                                                                       \
"bra    NN,sub_end_%=               \n\t"   /* exit if no borrow occured. Otherwise, saturate if borrow occured */                                          \
" firstOpearandIsNeg_%= :           \n\t"   /* if s16 is negative */                                                                                        \
"mov    #0x0000,%[u3]               \n\t"   /* saturate result to FFFF (LSW of the result)*/                                                                \
"mov    #0x0000,%d[u3]              \n\t"   /* saturate result to FFFF (MSW of the result)*/                                                                \
" sub_end_%= :                      \n\t"   /* overflow can occur only when both operands have same sign */                                                 \
: [u3] "=&r"(c)                                                                                                                                             \
: [u1] "r"(a) , [u2] "r"(b)                                                                                                                                 \
: "cc");                                                                                                                                                    \
c;                                                                                                                                                          \
})


// Subtraction with saturation for Cast-Before-Operation (Simulink default behaviour for 32bit-mixed datatypes subtraction)
#define MCHP_subSat_CBO_u32_u32_s16(x,y)     /* uint32 (with saturation) <= uint32 - int16  */                                                                              \
({                                                                                                                                                                          \
register unsigned long int  a = (unsigned long int) (x);                                                                                                                    \
register long int  b = (long int)(y);       /* s16 operand will be sign extended */                                                                                         \
register unsigned long int  c;                                                                                                                                              \
__asm__ volatile(                                                                                                                                                           \
"btsc   %[u2], #15                  \n\t"   /* check if s16 is negative */                                                                                                  \
"bra    secondOpearandIsNeg_%=      \n\t"   /* branch if s16 is negative */                                                                                                 \
"sub    %[u1],%[u2],%[u3]           \n\t"   /* sub Lo 16 bits parts */                                                                                                      \
"subb   %d[u1],%d[u2],%d[u3]        \n\t"   /* sub Hi 16 bits with Borrow */                                                                                                \
"bra    C, sub_end_%=               \n\t"   /* exit if Carry flag is clear */                                                                                               \
"mov    #0x0000,%d[u3]              \n\t"   /* set saturation to MIN_UINT16 */                                                                                              \
"mov    #0x0000,%[u3]               \n\t"   /* to 0x0000 0000 */                                                                                                            \
"bra    sub_end_%=                  \n\t"   /* exit */                                                                                                                      \
" secondOpearandIsNeg_%= :          \n\t"   /* if s16 is negative */                                                                                                        \
"mov    %[u1],%[u3]                 \n\t"   /* s16 will be set to zero before operation takes place (Simulink behaviour) and the result of the addition is u32 value */     \
"mov    %d[u1],%d[u3]               \n\t"   /* saturate result to zero (MSW of the result)*/                                                                                \
" sub_end_%= :                      \n\t"   /* end */                                                                                                                       \
: [u3] "=&r"(c)                                                                                                                                                             \
: [u1] "r"(a) , [u2] "r"(b)                                                                                                                                                 \
: "cc");                                                                                                                                                                    \
c;                                                                                                                                                                          \
}) 


// Subtraction with saturation for Cast-Before-Operation (Simulink default behaviour for 32bit-mixed datatypes subtraction)
#define MCHP_subSat_CBO_u32_s16_u32(x,y)     /* uint32 (with saturation) <= uint32 - int16  */                                                                              \
({                                                                                                                                                                          \
register long int  a = (long int) (x);                                                                                                                                      \
register unsigned long int  b = (unsigned long int)(y);                                                                                                                     \
register unsigned long int  c;                                                                                                                                              \
__asm__ volatile(                                                                                                                                                           \
"btsc   %[u1],#15                   \n\t"   /* check if s16 is negative */                                                                                                  \
"bra    firstOperandIsNeg_%=        \n\t"   /* branch if s16 is negative */                                                                                                 \
"sub    %[u1],%[u2],%[u3]           \n\t"   /* sub Lo 16 bits parts */                                                                                                      \
"subb   %d[u1],%d[u2],%d[u3]        \n\t"   /* sub Hi 16 bits with Borrow */                                                                                                \
"bra    NN, sub_end_%=              \n\t"   /* exit if Carry flag is clear */                                                                                               \
" firstOperandIsNeg_%= :            \n\t"   /* if s16 is negative */                                                                                                        \
"mov    #0x0000,%d[u3]              \n\t"   /* set saturation to MIN_UINT16 */                                                                                              \
"mov    #0x0000,%[u3]               \n\t"   /* to 0x0000 0000 */                                                                                                            \
" sub_end_%= :                      \n\t"   /* end */                                                                                                                       \
: [u3] "=r"(c)                                                                                                                                                              \
: [u1] "r"(a) , [u2] "r"(b)                                                                                                                                                 \
: "cc");                                                                                                                                                                    \
c;                                                                                                                                                                          \
}) 


#define MCHP_subSat_CAO_s32_u32_s16(x,y)  /* int32 (with saturation) <= uint32 - int16 */                       \
({                                                                                                              \
register unsigned long int a = (unsigned long int) (x);                                                         \
register long int b = (long int)(y);       /* s16 operand will be sign extended */                              \
register long int c;                                                                                            \
__asm__ volatile(                                                                                               \
"btsc   %d[u1], #15                 \n\t"   /* check if MSb of u32 is set */                                    \
"bra    signedSubtraction_%=        \n\t"   /* brach if MSb of u32 is set */                                    \
"sub    %[u1],%[u2],%[u3]           \n\t"   /* sub Lo 16 bits parts */                                          \
"subb   %d[u1],%d[u2],%d[u3]        \n\t"   /* sub Hi 16 bits with reported borrow */                           \
"bra    NOV, sub_end_%=             \n\t"   /* exit if no Overflow occurred*/                                   \
"mov    #0x7FFF,%d[u3]              \n\t"   /* Set int32 MAX Saturation */                                      \
"mov    #0xFFFF,%[u3]               \n\t"   /* to 0x7FFF FFFF */                                                \
"bra    sub_end_%=                  \n\t"   /* exit  */                                                         \
" signedSubtraction_%= :            \n\t"   /* if MSb of int32 is set*/                                         \
"sub    %[u1],%[u2],%[u3]           \n\t"   /* sub Lo 16 bits parts */                                          \
"subb   %d[u1],%d[u2],%d[u3]        \n\t"   /* sub Hi 16 bits with reported borrow */                           \
"bra    NC, sub_end_%=              \n\t"   /* exit if no Borrow reported */                                    \
"mov    #0x7FFF,%d[u3]              \n\t"   /* Set int32 MAX Saturation */                                      \
"mov    #0xFFFF,%[u3]               \n\t"   /* to 0x7FFF FFFF */                                                \
" sub_end_%= :                      \n\t"   /* underflow can occur only when operands have opposite signs */    \
: [u3] "=&r"(c)                                                                                                 \
: [u1] "r"(a) , [u2] "r"(b)                                                                                     \
: "cc");                                                                                                        \
c;                                                                                                              \
}) 


#define MCHP_subSat_CAO_s32_s16_u32(x,y)  /* int32 (with saturation) <= uint32 - int16 */                       \
({                                                                                                              \
register long int a = (long int) (x);                                                                           \
register unsigned long int b = (unsigned long int)(y);                                                          \
register long int c;                                                                                            \
__asm__ volatile(                                                                                               \
"btsc   %d[u1],#15                  \n\t"   /* check if s16 is negative */                                      \
"bra    firstOpNegative%=           \n\t"   /* branch if s16 is negative */                                     \
"btsc   %d[u2],#15                  \n\t"   /* check if MSb of u32 is set */                                    \
"bra    secondOpNegative%=          \n\t"   /* branch if MSb of u32 is set */                                   \
"sub    %[u1],%[u2],%[u3]           \n\t"   /* sub Lo 16 bits parts */                                          \
"subb   %d[u1],%d[u2],%d[u3]        \n\t"   /* sub Hi 16 bits with reported borrow */                           \
"bra    NOV, sub_end_%=             \n\t"   /* exit if no Overflow occurred*/                                   \
"bra    saturateToMin%=             \n\t"   /* if no overflow occured, brach to Saturation routine */           \
"bra    sub_end_%=                  \n\t"   /* otherwise exit*/                                                 \
" secondOpNegative%= :              \n\t"   /* when seocnd operand is "negative" */                             \
"sub    %[u1],%[u2],%[u3]           \n\t"   /* sub Lo 16 bits parts */                                          \
"subb   %d[u1],%d[u2],%d[u3]        \n\t"   /* sub Hi 16 bits with reported borrow */                           \
"bra    NOV, saturateToMin%=        \n\t"   /* saturate if no Overflow occurred*/                               \
"bra    sub_end_%=                  \n\t"   /* exit */                                                          \
" firstOpNegative%= :               \n\t"   /* when s16 is negative */                                          \
"btsc   %d[u2], #15                 \n\t"   /* test if MSbit of u32 is set */                                   \
"bra    saturateToMin%=             \n\t"   /* saturate if MSbit of u32 is set*/                                \
"sub    %[u1],%[u2],%[u3]           \n\t"   /* sub Lo 16 bits parts */                                          \
"subb   %d[u1],%d[u2],%d[u3]        \n\t"   /* sub Hi 16 bits with reported borrow */                           \
"bra    NOV, sub_end_%=             \n\t"   /* exit if no Overflow occurred*/                                   \
" saturateToMin%= :                 \n\t"   /* saturation rountine*/                                            \
"mov    #0x8000,%d[u3]              \n\t"   /* Set MIN_INT32 Saturation */                                      \
"mov    #0x0000,%[u3]               \n\t"   /* to 0x8000 0000 */                                                \
" sub_end_%= :                      \n\t"   /* exit */                                                          \
: [u3] "=&r"(c)                                                                                                 \
: [u1] "r"(a) , [u2] "r"(b)                                                                                     \
: "cc");                                                                                                        \
c;                                                                                                              \
}) 


// Subtraction with saturation for Cast-Before-Operation (Simulink default behaviour for 32bit-mixed datatypes)
#define MCHP_subSat_CBO_u32_s32_u16(x,y)  /* int32 (with saturation) <= uint32 - int16 */                                   \
({                                                                                                                          \
register long int  a = (long int)(x);                                                                                       \
register unsigned long int  b = (unsigned long int)(y);                                                                     \
register unsigned long int  c;                                                                                              \
__asm__ volatile(                                                                                                           \
"btsc   %d[u1],#15                  \n\t"   /* check if s32 is negative */                                                  \
"bra    saturateToZero_%=           \n\t"   /* if s32 operand is negative, then saturate the output to ZERO right away */   \
"sub    %[u1],%[u2],%[u3]           \n\t"   /* sub Lo 16 bits parts */                                                      \
"subb   %d[u1],%d[u2],%d[u3]        \n\t"   /* sub Hi 16 bits with reported borrow */                                       \
"bra    NN, sub_end_%=              \n\t"   /* if the result is positive, then exit */                                      \
" saturateToZero_%= :               \n\t"   /* if the result is negative */                                                 \
"mov    #0x0000,%d[u3]              \n\t"   /* set MIN_UINT32 saturation */                                                 \
"mov    #0x0000,%[u3]               \n\t"   /* to 0x0000 0000*/                                                             \
" sub_end_%= :                      \n\t"   /* exit */                                                                      \
: [u3] "+&r"(c)                                                                                                             \
: [u1] "r"(a) , [u2] "r"(b)                                                                                                 \
: "cc");                                                                                                                    \
c;                                                                                                                          \
}) 


// Subtraction with saturation for Cast-Before-Operation (Simulink default behaviour for 32bit-mixed datatypes)
#define MCHP_subSat_CBO_u32_s32_u16(x,y)  /* int32 (with saturation) <= uint32 - int16 */                                   \
({                                                                                                                          \
register long int  a = (long int)(x);                                                                                       \
register unsigned long int  b = (unsigned long int)(y);                                                                     \
register unsigned long int  c;                                                                                              \
__asm__ volatile(                                                                                                           \
"btsc   %d[u1],#15                  \n\t"   /* check if s32 is negative */                                                  \
"bra    saturateToZero_%=           \n\t"   /* if s32 operand is negative, then saturate the output to ZERO right away */   \
"sub    %[u1],%[u2],%[u3]           \n\t"   /* sub Lo 16 bits parts */                                                      \
"subb   %d[u1],%d[u2],%d[u3]        \n\t"   /* sub Hi 16 bits with reported borrow */                                       \
"bra    NN, sub_end_%=              \n\t"   /* if the result is positive, then exit */                                      \
" saturateToZero_%= :               \n\t"   /* if the result is negative */                                                 \
"mov    #0x0000,%d[u3]              \n\t"   /* set MIN_UINT32 saturation */                                                 \
"mov    #0x0000,%[u3]               \n\t"   /* to 0x0000 0000*/                                                             \
" sub_end_%= :                      \n\t"   /* exit */                                                                      \
: [u3] "+&r"(c)                                                                                                             \
: [u1] "r"(a) , [u2] "r"(b)                                                                                                 \
: "cc");                                                                                                                    \
c;                                                                                                                          \
}) 


// Subtraction with saturation for Cast-Before-Operation (Simulink default behaviour for 32bit-mixed datatypes)
#define MCHP_subSat_CBO_u32_s32_s16(x,y)  /* int32 (with saturation) <= uint32 - int16 */       \
({                                                                                              \
register long int a = (long int) (x);                                                           \
register long int b = (long int) (y);                                                           \
register unsigned long int c = 0x0000;                                                          \
__asm__ volatile(                                                                               \
"btsc   %d[u1],#15                  \n\t"   /* test if s32 is negative */                       \
"mov.d  %[u3],%[u1]                 \n\t"   /* if s32 is negative, saturate it to Zero */       \
"btsc   %d[u2],#15                  \n\t"   /* test if s16 is negative */                       \
"mov.d  %[u3],%[u2]                 \n\t"   /* if s16 is negative, saturate it to Zero */       \
"sub    %[u1],%[u2],%[u3]           \n\t"   /* sub Lo 16 bits parts */                          \
"subb   %d[u1],%d[u2],%d[u3]        \n\t"   /* sub Hi 16 bits with borrow */                    \
"bra    NN, sub_end_%=              \n\t"   /* if the result is positive, then exit */          \
" saturateToZero_%= :               \n\t"   /* if the result is negative */                     \
"mov    #0x0000,%d[u3]              \n\t"   /* set MIN_UINT32 saturation */                     \
"mov    #0x0000,%[u3]               \n\t"   /* to 0x0000 0000*/                                 \
" sub_end_%= :                      \n\t"                                                       \
: [u3] "+r"(c)                                                                                  \
: [u1] "r"(a) , [u2] "r"(b)                                                                     \
: "cc");                                                                                        \
c;                                                                                              \
})  


// Subtraction with saturation for Cast-Before-Operation (Simulink default behaviour for 32bit-mixed datatypes)
#define MCHP_subSat_CBO_u32_u16_s32(x,y)  /* int32 (with saturation) <= uint32 - int16 */                       \
({                                                                                                              \
register unsigned long int a = (unsigned long int) (x);                                                         \
register long int b = (long int) (y);                                                                           \
register unsigned long int c ;                                                                                  \
__asm__ volatile(                                                                                               \
"btsc   %d[u2],#15                  \n\t"   /* test if s32 is negative */                                       \
"bra    secondOpIsNeg%=             \n\t"   /* if s32 is positive, then continue */                             \
"sub    %[u1],%[u2],%[u3]           \n\t"   /* sub Lo 16 bits parts */                                          \
"subb   %d[u1],%d[u2],%d[u3]        \n\t"   /* sub Hi 16 bits with borrow */                                    \
"bra    NN, sub_end_%=              \n\t"   /* if the result is negative, it should be saturated to zero */     \
"mov    #0x0,%[u3]                  \n\t"   /* set MIN_UINT32 saturation */                                     \
"mov    #0x0,%d[u3]                 \n\t"   /* to 0x0000 0000*/                                                 \
"bra    sub_end_%=                  \n\t"   /* exit */                                                          \
" secondOpIsNeg%= :                 \n\t"   /* if s32 is negative */                                            \
"mov    %[u1],%[u3]                 \n\t"   /* the result is the value of u16 */                                \
"mov    %d[u1],%d[u3]               \n\t"                                                                       \
" sub_end_%= :                      \n\t"                                                                       \
: [u3] "+r"(c)                                                                                                  \
: [u1] "r"(a) , [u2] "r"(b)                                                                                     \
: "cc");                                                                                                        \
c;                                                                                                              \
})  


#define MCHP_subSat_CAO_u32_s32_u16(x,y)   /* uint32 (with saturation) <= int32 - uint16 */                                                                 \
({                                                                                                                                                          \
register long int  a = (long int) (x);                                                                                                                      \
register unsigned long int  b = (unsigned long int)(y);                                                                                                     \
register unsigned long int  c;                                                                                                                              \
__asm__ volatile(                                                                                                                                           \
"btsc   %d[u1],#15                  \n\t"   /* check if s16 is negative */                                                                                  \
"bra    firstOpearandIsNeg_%=       \n\t"   /* branch if s16 is negative */                                                                                 \
"sub    %[u1],%[u2],%[u3]           \n\t"   /* sub Lo 16 bits parts */                                                                                      \
"subb   %d[u1],%d[u2],%d[u3]        \n\t"   /* sub Hi 16 bits with reported borrow */                                                                       \
"bra    NN,sub_end_%=               \n\t"   /* exit if no borrow occured. Otherwise, saturate if borrow occured */                                          \
" firstOpearandIsNeg_%= :           \n\t"   /* if s16 is negative */                                                                                        \
"mov    #0x0000,%[u3]               \n\t"   /* saturate result to FFFF (LSW of the result)*/                                                                \
"mov    #0x0000,%d[u3]              \n\t"   /* saturate result to FFFF (MSW of the result)*/                                                                \
" sub_end_%= :                      \n\t"   /* overflow can occur only when both operands have same sign */                                                 \
: [u3] "=&r"(c)                                                                                                                                             \
: [u1] "r"(a) , [u2] "r"(b)                                                                                                                                 \
: "cc");                                                                                                                                                    \
c;                                                                                                                                                          \
}) 


#define MCHP_subSat_CAO_u32_s32_s16(x,y) /* uint32 (with saturation) <= int32 - int16  */                                                                                   \
({                                                                                                                                                                          \
register long int a = (long int) (x);                                                                                                                                       \
register long int b = (long int) (y);                                                                                                                                       \
register long int c;                                                                                                                                                        \
register long int tmpMCHP = 0x0;                                                                                                                                            \
__asm__ volatile(                                                                                                                                                           \
"sub    %[u1],%[u2],%[u3]       \n\t"   /* sub Lo 16 bits parts */                                                                                                          \
"subb   %d[u1],%d[u2],%d[u3]    \n\t"   /* sub Hi 16 bits with repported carry */                                                                                           \
"bra    NOV, label_%=           \n\t"   /* branch if overflow occurred*/                                                                                                    \
"btss   %d[u3],#15              \n\t"   /* otherwise, check if the result is negative */                                                                                    \
"mov.d  %[tmppMCHP],%[u3]       \n\t"   /* if the result is negative, saturate the output to zero */                                                                        \
"bra    sub_end_%=              \n\t"   /* exit */                                                                                                                          \
" label_%= :                    \n\t"   /* rountine to handle overflow case  */                                                                                             \
"btsc   SR,#3                   \n\t"   /* in overflow condition, use N flag information. If N flag is clear, it means that OV occurred by adding two negative values */    \
"mov.d  %[tmppMCHP],%[u3]       \n\t"   /* saturate to zero if N flag is cleared */                                                                                         \
" sub_end_%= :                  \n\t"   /* exit */                                                                                                                          \
: [u3] "+r"(c) ,[tmppMCHP] "+r"(tmpMCHP)                                                                                                                                    \
: [u1] "r"(a) , [u2] "r"(b)                                                                                                                                                 \
: "cc");                                                                                                                                                                    \
c;                                                                                                                                                                          \
}) 


#define MCHP_subSat_CAO_s8_s8_s8(x,y)  /* signed char8 (with saturation) <= signed char8 - signed char8 */      \
({                                                                                                              \
register signed char a = (signed char) (x);                                                                     \
register signed char b = (signed char) (y);                                                                     \
register signed char c;                                                                                         \
__asm__ volatile(                                                                                               \
"sub.b  %[u1], %[u2], %[u3] \n\t"   /* sub 8 bits values */                                                     \
"bra    NOV, sub_end_%=     \n\t"   /* exit if no overflow. */                                                  \
"mov.b  #0x7F, %[u3]        \n\t"   /* saturate to max (0x7F) */                                                \
"btss   %[u2], #7           \n\t"   /* or neg (0x80) */                                                         \
"mov    #0x80,%[u3]         \n\t"   /* depending on (both) operand sign. */                                     \
" sub_end_%= :              \n\t"   /* underflow/overflow can occur only when operands have opposite signs */   \
: [u3] "=&r"(c)                                                                                                 \
: [u1] "r"(a) , [u2] "r"(b)                                                                                     \
: "cc");                                                                                                        \
c;                                                                                                              \
})  


#define MCHP_subSat_CAO_u8_u8_u8(x,y)  /* unsigned char8 (with saturation) <= unsigned char8 - unsigned char8 */    \
({                                                                                                                  \
register unsigned char a = (unsigned char) (x);                                                                     \
register unsigned char b = (unsigned char) (y);                                                                     \
register unsigned char c;                                                                                           \
__asm__ volatile(                                                                                                   \
"sub.b   %[u1],%[u2],%[u3]      \n\t"       /* sub 8 bits parts */                                                  \
"btss    SR,#0                  \n\t"       /* if Borrow flag is set */                                             \
"mov.b   #0x00,%[u3]            \n\t"       /* set output to 0x00 */                                                \
: [u3] "=r"(c)                                                                                                      \
: [u1] "r"(a) , [u2] "r"(b)                                                                                         \
: "cc");                                                                                                            \
c;                                                                                                                  \
})                                                                                                                  \

/*--------------------------------------- Reversed-operands for primitive macros (Subtraction) -------------------------------------------------------------------- */
#define MCHP_subSat_CAO_u32_u32_u16(x,y)    MCHP_subSat_CAO_u32_u32_u32(x,y)   /* uint32(with saturation) <= uint32 - uint16 */
#define MCHP_subSat_CAO_u32_u16_u32(x,y)    MCHP_subSat_CAO_u32_u32_u32(x,y)   /* uint32(with saturation) <= uint16 - uint32 */
#define MCHP_subSat_CAO_s32_s32_s16(x,y)    MCHP_subSat_CAO_s32_s32_s32(x,y)   /* int32 (with saturation) <= int32 - int16   */ 
#define MCHP_subSat_CAO_s32_s16_s32(x,y)    MCHP_subSat_CAO_s32_s32_s32(x,y)   /* int32 (with saturation) <= int16 - int32   */ 

/*------------------------------------------------ Special cases for subtraction ----------------------------------------------------------------------- */
#define MCHP_subSat_CAO_s32_s32_u16(x,y)    MCHP_subSat_CAO_s32_s32_s32(x,y)    /* int32 (with saturation) <= int32 - uint16  */ 
#define MCHP_subSat_CAO_s32_u16_s32(x,y)    MCHP_subSat_CAO_s32_s32_s32(x,y)    /* int32 (with saturation) <= uint16 - int32  */ 
#define MCHP_subSat_CBO_u32_s16_s32(x,y)    MCHP_subSat_CBO_u32_s32_s16(x,y)    /* uint32 (with saturation) <= int16 - int32  */
#define MCHP_subSat_CAO_u32_u16_s32(x,y)    MCHP_subSat_CAO_u32_u32_s16(x,y)    /* uint32 (with saturation) <= uint16 - int32 */
#define MCHP_subSat_CAO_u32_s16_s32(x,y)    MCHP_subSat_CAO_u32_s32_s16(x,y)    /* uint32 (with saturation) <= int16 - int32  */
#define MCHP_subSat_CAO_s32_u32_u16(x,y)    MCHP_SATURATE_U32_TO_S32(MCHP_subSat_CAO_u32_u32_u16(x,y))  /* int32 (with saturation) <= uint32 - uint16 */

/*-------------------------------------------- 8-bit/16-bit mixed-sign subtraction --------------------------------------------------------------------- */
#define MCHP_subSat_CAO_s16_s8_s16(x,y)     MCHP_subSat_CAO_s16_s16_s16(x,y)   /* int16 (with saturation) <= int8 - int16    */
#define MCHP_subSat_CAO_s16_s16_s8(x,y)     MCHP_subSat_CAO_s16_s16_s16(x,y)   /* int16 (with saturation) <= int16 - int8    */
#define MCHP_subSat_CAO_u16_u8_u16(x,y)     MCHP_subSat_CAO_u16_u16_u16(x,y)   /* uint16 (with saturation) <= uint8 - uint16 */
#define MCHP_subSat_CAO_u16_u16_u8(x,y)     MCHP_subSat_CAO_u16_u16_u16(x,y)   /* uint16 (with saturation) <= uint16 - uint8 */

#define MCHP_subSat_CAO_s8_s8_s16(x,y)      MCHP_SATURATE_S16_TO_S8(MCHP_subSat_CAO_s16_s16_s16(x,y))  /* int8 (with saturation) <= int8 - int16      */

#define MCHP_subSat_CAO_s8_s16_s8(x,y)      MCHP_SATURATE_S16_TO_S8(MCHP_subSat_CAO_s16_s16_s16(x,y))  /* int8 (with saturation) <= int16 - int8      */
#define MCHP_subSat_CAO_s8_s16_s16(x,y)     MCHP_SATURATE_S16_TO_S8(MCHP_subSat_CAO_s16_s16_s16(x,y))  /* int8 (with saturation) <= int16 - int16     */
#define MCHP_subSat_CAO_s16_s32_s16(x,y)    MCHP_SATURATE_S32_TO_S16(MCHP_subSat_CAO_s32_s32_s16(x,y)) /* int16 (with saturation) <= int32 - int16    */
#define MCHP_subSat_CAO_s16_s16_s32(x,y)    MCHP_SATURATE_S32_TO_S16(MCHP_subSat_CAO_s32_s16_s32(x,y)) /* int16 (with saturation) <= int16 - int32    */
#define MCHP_subSat_CAO_s16_s32_s32(x,y)    MCHP_SATURATE_S32_TO_S16(MCHP_subSat_CAO_s32_s32_s32(x,y)) /* int16 (with saturation) <= int32 - int32    */

#define MCHP_subSat_CAO_u8_u8_u16(x,y)      MCHP_SATURATE_U16_TO_U8(MCHP_subSat_CAO_u16_u16_u16(x,y))  /* uint8 (with saturation) <= uint8 - uint16   */
#define MCHP_subSat_CAO_u8_u16_u8(x,y)      MCHP_SATURATE_U16_TO_U8(MCHP_subSat_CAO_u16_u16_u16(x,y))  /* uint8 (with saturation) <= uint16 - uint8   */
#define MCHP_subSat_CAO_u8_u16_u16(x,y)     MCHP_SATURATE_U16_TO_U8(MCHP_subSat_CAO_u16_u16_u16(x,y))  /* uint8 (with saturation) <= uint16 - uint16  */
#define MCHP_subSat_CAO_u16_u32_u32(x,y)    MCHP_SATURATE_U32_TO_U16(MCHP_subSat_CAO_u32_u32_u32(x,y)) /* uint16 (with saturation) <= uint32 - uint32 */
#define MCHP_subSat_CAO_u16_u16_u32(x,y)    MCHP_SATURATE_U32_TO_U16(MCHP_subSat_CAO_u32_u16_u32(x,y)) /* uint16 (with saturation) <= uint16 - uint32 */
#define MCHP_subSat_CAO_u16_u32_u16(x,y)    MCHP_SATURATE_U32_TO_U16(MCHP_subSat_CAO_u32_u32_u16(x,y)) /* uint16 (with saturation) <= uint32 - uint32 */

#endif
