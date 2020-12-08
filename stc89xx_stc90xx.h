#ifndef __STC89C5xRC_RDP_H_
#define __STC89C5xRC_RDP_H_

#include <mcs51/compiler.h>
/////////////////////////////////////////////////

/* The following is STC additional SFR */

/* SFR(AUXR, 0x8e); */
/* SFR(AUXR1, 0xa2); */
/* SFR(IPH, 0xb7); */

SFR(P4, 0xe8);
SBIT(P47, 0xe8, 7);
SBIT(P46, 0xe8, 6);
SBIT(P45, 0xe8, 5);       //ISP下载需勾选"ALE脚用作P4.5口"
SBIT(P44, 0xe8, 4);
SBIT(P43, 0xe8, 3);
SBIT(P42, 0xe8, 2);
SBIT(P41, 0xe8, 1);
SBIT(P40, 0xe8, 0);

SFR(XICON, 0xc0);

SFR(WDT_CONTR, 0xe1);

SFR(ISP_DATA, 0xe2);
SFR(ISP_ADDRH, 0xe3);
SFR(ISP_ADDRL, 0xe4);
SFR(ISP_CMD, 0xe5);
SFR(ISP_TRIG, 0xe6);
SFR(ISP_CONTR, 0xe7);

/* Above is STC additional SFR */

/*--------------------------------------------------------------------------
REG51F.H

Header file for 8xC31/51, 80C51Fx, 80C51Rx+
Copyright (c) 1988-1999 Keil Elektronik GmbH and Keil Software, Inc.
All rights reserved.

Modification according to DataSheet from April 1999
 - SFR's AUXR and AUXR1 added for 80C51Rx+ derivatives
--------------------------------------------------------------------------*/

/*  BYTE Registers  */
SFR(P0, 0x80);
SBIT(P00, 0x80, 0);
SBIT(P01, 0x80, 1);
SBIT(P02, 0x80, 2);
SBIT(P03, 0x80, 3);
SBIT(P04, 0x80, 4);
SBIT(P05, 0x80, 5);
SBIT(P06, 0x80, 6);
SBIT(P07, 0x80, 7);
SFR(P1, 0x90);
SBIT(P10, 0x90, 0);
SBIT(P11, 0x90, 1);
SBIT(P12, 0x90, 2);
SBIT(P13, 0x90, 3);
SBIT(P14, 0x90, 4);
SBIT(P15, 0x90, 5);
SBIT(P16, 0x90, 6);
SBIT(P17, 0x90, 7);
SFR(P2, 0xA0);
SBIT(P20, 0xA0, 0);
SBIT(P21, 0xA0, 1);
SBIT(P22, 0xA0, 2);
SBIT(P23, 0xA0, 3);
SBIT(P24, 0xA0, 4);
SBIT(P25, 0xA0, 5);
SBIT(P26, 0xA0, 6);
SBIT(P27, 0xA0, 7);
SFR(P3, 0xB0);
SBIT(P30, 0xB0, 0);
SBIT(P31, 0xB0, 1);
SBIT(P32, 0xB0, 2);
SBIT(P33, 0xB0, 3);
SBIT(P34, 0xB0, 4);
SBIT(P35, 0xB0, 5);
SBIT(P36, 0xB0, 6);
SBIT(P37, 0xB0, 7);
SFR(PSW, 0xD0);
SFR(ACC, 0xE0);
SFR(B, 0xF0);
SFR(SP, 0x81);
SFR(DPL, 0x82);
SFR(DPH, 0x83);
SFR(PCON, 0x87);
SFR(TCON, 0x88);
SFR(TMOD, 0x89);
SFR(TL0, 0x8A);
SFR(TL1, 0x8B);
SFR(TH0, 0x8C);
SFR(TH1, 0x8D);
SFR(IE, 0xA8);
SFR(IP, 0xB8);
SFR(SCON, 0x98);
SFR(SBUF, 0x99);

/*  80C51Fx/Rx Extensions  */
SFR(AUXR, 0x8E);
SFR(AUXR1, 0xA2);
SFR(SADDR, 0xA9);
SFR(IPH, 0xB7);
SFR(SADEN, 0xB9);
SFR(T2CON, 0xC8);
SFR(T2MOD, 0xC9);
SFR(RCAP2L, 0xCA);
SFR(RCAP2H, 0xCB);
SFR(TL2, 0xCC);
SFR(TH2, 0xCD);

/* PCA SFR
SFR(CCON, 0xD8);
SFR(CMOD, 0xD9);
SFR(CCAPM0, 0xDA);
SFR(CCAPM1, 0xDB);
SFR(CCAPM2, 0xDC);
SFR(CCAPM3, 0xDD);
SFR(CCAPM4, 0xDE);
SFR(CL, 0xE9);
SFR(CCAP0L, 0xEA);
SFR(CCAP1L, 0xEB);
SFR(CCAP2L, 0xEC);
SFR(CCAP3L, 0xED);
SFR(CCAP4L, 0xEE);
SFR(CH, 0xF9);
SFR(CCAP0H, 0xFA);
SFR(CCAP1H, 0xFB);
SFR(CCAP2H, 0xFC);
SFR(CCAP3H, 0xFD);
SFR(CCAP4H, 0xFE);
*/

/*  BIT Registers  */
/*  PSW   */
SBIT(CY, 0xD0, 7);
SBIT(AC, 0xD0, 6);
SBIT(F0, 0xD0, 5);
SBIT(RS1, 0xD0, 4);
SBIT(RS0, 0xD0, 3);
SBIT(OV, 0xD0, 2);
SBIT(F1, 0xD0, 1);
SBIT(P, 0xD0, 0);

/*  TCON  */
SBIT(TF1, 0x88, 7);
SBIT(TR1, 0x88, 6);
SBIT(TF0, 0x88, 5);
SBIT(TR0, 0x88, 4);
SBIT(IE1, 0x88, 3);
SBIT(IT1, 0x88, 2);
SBIT(IE0, 0x88, 1);
SBIT(IT0, 0x88, 0);

/*  IE   */
SBIT(EA, 0xA8, 7);
SBIT(EC, 0xA8, 6);
SBIT(ET2, 0xA8, 5);
SBIT(ES, 0xA8, 4);
SBIT(ET1, 0xA8, 3);
SBIT(EX1, 0xA8, 2);
SBIT(ET0, 0xA8, 1);
SBIT(EX0, 0xA8, 0);

/*  IP   */ 
/*  SBIT(PPC, 0xB8, 6);*/
SBIT(PT2, 0xB8, 5);
SBIT(PS, 0xB8, 4);
SBIT(PT1, 0xB8, 3);
SBIT(PX1, 0xB8, 2);
SBIT(PT0, 0xB8, 1);
SBIT(PX0, 0xB8, 0);

/*  P3  */
SBIT(RD, 0xB0, 7);
SBIT(WR, 0xB0, 6);
SBIT(T1, 0xB0, 5);
SBIT(T0, 0xB0, 4);
SBIT(INT1, 0xB0, 3);
SBIT(INT0, 0xB0, 2);
SBIT(TXD, 0xB0, 1);
SBIT(RXD, 0xB0, 0);

/*  SCON  */
SBIT(SM0, 0x98, 7); // alternatively "FE"
SBIT(FE, 0x98, 7);
SBIT(SM1, 0x98, 6);
SBIT(SM2, 0x98, 5);
SBIT(REN, 0x98, 4);
SBIT(TB8, 0x98, 3);
SBIT(RB8, 0x98, 2);
SBIT(TI, 0x98, 1);
SBIT(RI, 0x98, 0);
             
/*  P1  */
/* PCA
SBIT(CEX4, 0x90, 7);
SBIT(CEX3, 0x90, 6);
SBIT(CEX2, 0x90, 5);
SBIT(CEX1, 0x90, 4);
SBIT(CEX0, 0x90, 3);
SBIT(ECI, 0x90, 2);
*/

SBIT(T2EX, 0x90, 1);
SBIT(T2, 0x90, 0);

/*  T2CON  */
SBIT(TF2, 0xC8, 7);
SBIT(EXF2, 0xC8, 6);
SBIT(RCLK, 0xC8, 5);
SBIT(TCLK, 0xC8, 4);
SBIT(EXEN2, 0xC8, 3);
SBIT(TR2, 0xC8, 2);
SBIT(C_T2, 0xC8, 1);
SBIT(CP_RL2, 0xC8, 0);

/*  CCON  */
/*  PCA
SBIT(CF, 0xD8, 7);
SBIT(CR, 0xD8, 6);

SBIT(CCF4, 0xD8, 4);
SBIT(CCF3, 0xD8, 3);
SBIT(CCF2, 0xD8, 2);
SBIT(CCF1, 0xD8, 1);
SBIT(CCF0, 0xD8, 0);
*/

/////////////////////////////////////////////////

#endif

