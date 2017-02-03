/* ipc.h.  Microkernel IPC header for Or1ksim

   Copyright (C) 2000 Damjan Lampret
   Copyright (C) 2008, 2010 Embecosm Limited

   Contributor Damjan Lampret <lampret@opencores.org>
   Contributor Jeremy Bennett <jeremy.bennett@embecosm.com>

   This file is part of OpenRISC 1000 Architectural Simulator.

   This program is free software; you can redistribute it and/or modify it
   under the terms of the GNU General Public License as published by the Free
   Software Foundation; either version 3 of the License, or (at your option)
   any later version.

   This program is distributed in the hope that it will be useful, but WITHOUT
   ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
   FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
   more details.

   You should have received a copy of the GNU General Public License along
   with this program.  If not, see <http:  www.gnu.org/licenses/>.  */

/* ----------------------------------------------------------------------------
   This code is commented throughout for use with Doxygen.
   --------------------------------------------------------------------------*/

/* This file is part of test microkernel for OpenRISC 1000. */
/* spr-defs.h -- Defines OR1K architecture specific special-purpose registers

   Copyright (C) 1999 Damjan Lampret, lampret@opencores.org
   Copyright (C) 2008 Embecosm Limited

   Contributor Jeremy Bennett <jeremy.bennett@embecosm.com>

   This file is part of OpenRISC 1000 Architectural Simulator.

   This program is free software; you can redistribute it and/or modify it
   under the terms of the GNU General Public License as published by the Free
   Software Foundation; either version 3 of the License, or (at your option)
   any later version.

   This program is distributed in the hope that it will be useful, but WITHOUT
   ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
   FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
   more details.

   You should have received a copy of the GNU General Public License along
   with this program.  If not, see <http://www.gnu.org/licenses/>. */

/* This program is commented throughout in a fashion suitable for processing
   with Doxygen. */


#ifndef SPR_DEFS__H
#define SPR_DEFS__H

/* Definition of special-purpose registers (SPRs). */

#define MAX_GRPS (32)
#define MAX_SPRS_PER_GRP_BITS (11)
#define MAX_SPRS_PER_GRP (1 << MAX_SPRS_PER_GRP_BITS)
#define MAX_SPRS (0x10000)

/* Base addresses for the groups */
#define SPRGROUP_SYS    (0<< MAX_SPRS_PER_GRP_BITS)
#define SPRGROUP_DMMU   (1<< MAX_SPRS_PER_GRP_BITS)
#define SPRGROUP_IMMU   (2<< MAX_SPRS_PER_GRP_BITS)
#define SPRGROUP_DC     (3<< MAX_SPRS_PER_GRP_BITS)
#define SPRGROUP_IC     (4<< MAX_SPRS_PER_GRP_BITS)
#define SPRGROUP_MAC    (5<< MAX_SPRS_PER_GRP_BITS)
#define SPRGROUP_D      (6<< MAX_SPRS_PER_GRP_BITS)
#define SPRGROUP_PC     (7<< MAX_SPRS_PER_GRP_BITS)
#define SPRGROUP_PM     (8<< MAX_SPRS_PER_GRP_BITS)
#define SPRGROUP_PIC    (9<< MAX_SPRS_PER_GRP_BITS)
#define SPRGROUP_TT     (10<< MAX_SPRS_PER_GRP_BITS)
#define SPRGROUP_FP     (11<< MAX_SPRS_PER_GRP_BITS)
#define SPRGROUP_HWLP   (12<< MAX_SPRS_PER_GRP_BITS)
#define NUM_HWLP 32


/* System control and status group */
#define SPR_VR          (SPRGROUP_SYS + 0)
#define SPR_UPR         (SPRGROUP_SYS + 1)
#define SPR_CPUCFGR     (SPRGROUP_SYS + 2)
#define SPR_DMMUCFGR    (SPRGROUP_SYS + 3)
#define SPR_IMMUCFGR    (SPRGROUP_SYS + 4)
#define SPR_DCCFGR      (SPRGROUP_SYS + 5)
#define SPR_ICCFGR      (SPRGROUP_SYS + 6)
#define SPR_DCFGR       (SPRGROUP_SYS + 7)
#define SPR_PCCFGR      (SPRGROUP_SYS + 8)
#define SPR_VR2         (SPRGROUP_SYS + 9)
#define SPR_AVR         (SPRGROUP_SYS + 10)
#define SPR_EVBAR       (SPRGROUP_SYS + 11)
#define SPR_AECR        (SPRGROUP_SYS + 12)
#define SPR_AESR        (SPRGROUP_SYS + 13)
#define SPR_NPC         (SPRGROUP_SYS + 16)  /* CZ 21/06/01 */
#define SPR_SR          (SPRGROUP_SYS + 17)  /* CZ 21/06/01 */
#define SPR_PPC         (SPRGROUP_SYS + 18)  /* CZ 21/06/01 */
#define SPR_FPCSR       (SPRGROUP_SYS + 20)  /* CZ 21/06/01 */
#define SPR_EPCR_BASE   (SPRGROUP_SYS + 32)  /* CZ 21/06/01 */
#define SPR_EPCR_LAST   (SPRGROUP_SYS + 47)  /* CZ 21/06/01 */
#define SPR_EEAR_BASE   (SPRGROUP_SYS + 48)
#define SPR_EEAR_LAST   (SPRGROUP_SYS + 63)
#define SPR_ESR_BASE    (SPRGROUP_SYS + 64)
#define SPR_ISR0        (SPRGROUP_SYS + 65)
#define SPR_ISR1        (SPRGROUP_SYS + 66)
#define SPR_ISR2        (SPRGROUP_SYS + 67)
#define SPR_ISR3        (SPRGROUP_SYS + 68)
#define SPR_ISR4        (SPRGROUP_SYS + 69)
#define SPR_ISR5        (SPRGROUP_SYS + 70)
#define SPR_ISR6        (SPRGROUP_SYS + 71)
#define SPR_ISR7        (SPRGROUP_SYS + 72)
#define SPR_ESR_LAST    (SPRGROUP_SYS + 79)
#define SPR_GPR_BASE    (SPRGROUP_SYS + 1024)

// Added for OR10N
#define SPR_CORE_ID    (SPRGROUP_SYS + 0x680)
#define SPR_CLUSTER_ID (SPRGROUP_SYS + 0x681)

#define SPR_HWLP_START_BASE (SPRGROUP_HWLP + 0x00)
#define SPR_HWLP_END_BASE   (SPRGROUP_HWLP + 0x10)
#define SPR_HWLP_COUNT_BASE (SPRGROUP_HWLP + 0x20)

/* Data MMU group */
#define SPR_DMMUCR      (SPRGROUP_DMMU + 0)
#define SPR_DTLBMR_BASE(WAY)    (SPRGROUP_DMMU + 0x200 + (WAY) * 0x100)
#define SPR_DTLBMR_LAST(WAY)    (SPRGROUP_DMMU + 0x27f + (WAY) * 0x100)
#define SPR_DTLBTR_BASE(WAY)    (SPRGROUP_DMMU + 0x280 + (WAY) * 0x100)
#define SPR_DTLBTR_LAST(WAY)    (SPRGROUP_DMMU + 0x2ff + (WAY) * 0x100)

/* Instruction MMU group */
#define SPR_IMMUCR      (SPRGROUP_IMMU + 0)
#define SPR_ITLBMR_BASE(WAY)    (SPRGROUP_IMMU + 0x200 + (WAY) * 0x100)
#define SPR_ITLBMR_LAST(WAY)    (SPRGROUP_IMMU + 0x27f + (WAY) * 0x100)
#define SPR_ITLBTR_BASE(WAY)    (SPRGROUP_IMMU + 0x280 + (WAY) * 0x100)
#define SPR_ITLBTR_LAST(WAY)    (SPRGROUP_IMMU + 0x2ff + (WAY) * 0x100)

/* Data cache group */
#define SPR_DCCR        (SPRGROUP_DC + 0)
#define SPR_DCBPR       (SPRGROUP_DC + 1)
#define SPR_DCBFR       (SPRGROUP_DC + 2)
#define SPR_DCBIR       (SPRGROUP_DC + 3)
#define SPR_DCBWR       (SPRGROUP_DC + 4)
#define SPR_DCBLR       (SPRGROUP_DC + 5)
#define SPR_DCR_BASE(WAY)       (SPRGROUP_DC + 0x200 + (WAY) * 0x200)
#define SPR_DCR_LAST(WAY)       (SPRGROUP_DC + 0x3ff + (WAY) * 0x200)

/* Instruction cache group */
#define SPR_ICCR        (SPRGROUP_IC + 0)
#define SPR_ICBPR       (SPRGROUP_IC + 1)
#define SPR_ICBIR       (SPRGROUP_IC + 2)
#define SPR_ICBLR       (SPRGROUP_IC + 3)
#define SPR_ICR_BASE(WAY)       (SPRGROUP_IC + 0x200 + (WAY) * 0x200)
#define SPR_ICR_LAST(WAY)       (SPRGROUP_IC + 0x3ff + (WAY) * 0x200)

/* MAC group */
#define SPR_MACLO       (SPRGROUP_MAC + 1)
#define SPR_MACHI       (SPRGROUP_MAC + 2)

/* Debug group */
#define SPR_DVR(N)      (SPRGROUP_D + (N))
#define SPR_DCR(N)      (SPRGROUP_D + 8 + (N))
#define SPR_DMR1        (SPRGROUP_D + 16)
#define SPR_DMR2        (SPRGROUP_D + 17)
#define SPR_DWCR0       (SPRGROUP_D + 18)
#define SPR_DWCR1       (SPRGROUP_D + 19)
#define SPR_DSR         (SPRGROUP_D + 20)
#define SPR_DRR         (SPRGROUP_D + 21)

/* Performance counters group */
#define SPR_PCCR(N)     (SPRGROUP_PC + (N))
#define SPR_PCER        (SPRGROUP_PC + 32)
#define SPR_PCMR        (SPRGROUP_PC + 33)

/* Power management group */
#define SPR_PMR (SPRGROUP_PM + 0)

/* PIC group */
#define SPR_PICMR (SPRGROUP_PIC + 0)
#define SPR_PICPR (SPRGROUP_PIC + 1)
#define SPR_PICSR (SPRGROUP_PIC + 2)
#define SPR_PICWMR (SPRGROUP_PIC + 3)

/* Tick Timer group */
#define SPR_TTMR (SPRGROUP_TT + 0)
#define SPR_TTCR (SPRGROUP_TT + 1)

/* Hardware Loop group */
#define SPR_HWLP_START(ID)    (SPRGROUP_HWLP + (ID) + 0)
#define SPR_HWLP_END(ID)      (SPRGROUP_HWLP + (ID) + 0x10)
#define SPR_HWLP_COUNTER(ID)  (SPRGROUP_HWLP + (ID) + 0x20)

/*
 * Bit definitions for the Version Register
 *
 */
#define SPR_VR_VER      0xff000000  /* Processor version */
#define SPR_VR_CFG      0x00ff0000  /* Processor configuration */
#define SPR_VR_RES      0x00ff0000  /* Reserved */
#define SPR_VR_REV      0x0000003f  /* Processor revision */

#define SPR_VR_VER_OFF  24
#define SPR_VR_CFG_OFF  16
#define SPR_VR_REV_OFF  0

/*
 * Bit definitions for the Unit Present Register
 *
 */
#define SPR_UPR_UP         0x00000001  /* UPR present */
#define SPR_UPR_DCP        0x00000002  /* Data cache present */
#define SPR_UPR_ICP        0x00000004  /* Instruction cache present */
#define SPR_UPR_DMP        0x00000008  /* Data MMU present */
#define SPR_UPR_IMP        0x00000010  /* Instruction MMU present */
#define SPR_UPR_MP         0x00000020  /* MAC present */
#define SPR_UPR_DUP        0x00000040  /* Debug unit present */
#define SPR_UPR_PCUP       0x00000080  /* Performance counters unit present */
#define SPR_UPR_PMP        0x00000100  /* Power management present */
#define SPR_UPR_PICP       0x00000200  /* PIC present */
#define SPR_UPR_TTP        0x00000400  /* Tick timer present */
#define SPR_UPR_RES        0x00fe0000  /* Reserved */
#define SPR_UPR_CUP        0xff000000  /* Context units present */

/*
 * JPB: Bit definitions for the CPU configuration register
 *
 */
#define SPR_CPUCFGR_NSGF   0x0000000f  /* Number of shadow GPR files */
#define SPR_CPUCFGR_CGF    0x00000010  /* Custom GPR file */
#define SPR_CPUCFGR_OB32S  0x00000020  /* ORBIS32 supported */
#define SPR_CPUCFGR_OB64S  0x00000040  /* ORBIS64 supported */
#define SPR_CPUCFGR_OF32S  0x00000080  /* ORFPX32 supported */
#define SPR_CPUCFGR_OF64S  0x00000100  /* ORFPX64 supported */
#define SPR_CPUCFGR_OV64S  0x00000200  /* ORVDX64 supported */
#define SPR_CPUCFGR_ND     0x00000400  /* No delay slots */
#define SPR_CPUCFGR_AVRP   0x00000800  /* Architecture Version Register (AVR) Present */
#define SPR_CPUCFGR_EVBARP 0x00001000  /* Exception Vector Base Address Register Present */
#define SPR_CPUCFGR_ISRP   0x00002000  /* Implementation-Specific Registers (ISR0-7) Present */
#define SPR_CPUCFGR_AECSRP 0x00004000  /* Arithmetic Exception Control/Status Registers (ARCR/AESR) Present */
#define SPR_CPUCFGR_RES    0xffff8000  /* Reserved */

/*
 * JPB: Bit definitions for the Debug configuration register and other
 * constants.
 *
 */

#define SPR_DCFGR_NDP      0x00000007  /* Number of matchpoints mask */
#define SPR_DCFGR_NDP1     0x00000000  /* One matchpoint supported */
#define SPR_DCFGR_NDP2     0x00000001  /* Two matchpoints supported */
#define SPR_DCFGR_NDP3     0x00000002  /* Three matchpoints supported */
#define SPR_DCFGR_NDP4     0x00000003  /* Four matchpoints supported */
#define SPR_DCFGR_NDP5     0x00000004  /* Five matchpoints supported */
#define SPR_DCFGR_NDP6     0x00000005  /* Six matchpoints supported */
#define SPR_DCFGR_NDP7     0x00000006  /* Seven matchpoints supported */
#define SPR_DCFGR_NDP8     0x00000007  /* Eight matchpoints supported */
#define SPR_DCFGR_WPCI     0x00000008  /* Watchpoint counters implemented */

#define MATCHPOINTS_TO_NDP(n) (1 == n ? SPR_DCFGR_NDP1 : \
                               2 == n ? SPR_DCFGR_NDP2 : \
                               3 == n ? SPR_DCFGR_NDP3 : \
                               4 == n ? SPR_DCFGR_NDP4 : \
                               5 == n ? SPR_DCFGR_NDP5 : \
                               6 == n ? SPR_DCFGR_NDP6 : \
                               7 == n ? SPR_DCFGR_NDP7 : SPR_DCFGR_NDP8)
#define MAX_MATCHPOINTS  8
#define MAX_WATCHPOINTS  (MAX_MATCHPOINTS + 2)

/*
 * Bit definitions for Version Register 2
 *
 */

#define SPR_VR2_CPUID      0xff000000  /* CPU Identification number */
#define SPR_VR2_VER        0x00ffffff  /* Version */

/*
 * Bit definitions for Architecture Version Register
 *
 */

#define SPR_AVR_RES        0x000000ff  /* Reserved */
#define SPR_AVR_REV        0x0000ff00  /* Architecture Revision Number */
#define SPR_AVR_MIN        0x00ff0000  /* Minor Architecture Version Number */
#define SPR_AVR_MAJ        0xff000000  /* Major Architecture Version Number */

/*
 * Bit definitions for Exception Vector Base Address Register
 *
 */

#define SPR_EVBAR_RES      0x00001fff  /* Reserved */
#define SPR_EVBAR_EVBA     0xffffe000  /* Exception Vector Base Address */

/*
 * Bit definitions for the Arithmetic Exception Control Register
 *
 */
#define SPR_AECR_CYADDE    0x00000001 /* unsigned overflow in add */
#define SPR_AECR_OVADDE    0x00000002 /* signed overflow in add */
#define SPR_AECR_CYMULE    0x00000004 /* unsigned overflow in mul */
#define SPR_AECR_OVMULE    0x00000008 /* signed overflow in mul */
#define SPR_AECR_DBZE      0x00000010 /* divide by zero */
#define SPR_AECR_CYMACADDE 0x00000020 /* unsigned overflow in mac add */
#define SPR_AECR_OVMACADDE 0x00000040 /* signed overflow in mac add */

/*
 * Bit definitions for the Arithmetic Exception Status Register
 *
 */
#define SPR_AESR_CYADDE    0x00000001 /* unsigned overflow in add */
#define SPR_AESR_OVADDE    0x00000002 /* signed overflow in add */
#define SPR_AESR_CYMULE    0x00000004 /* unsigned overflow in mul */
#define SPR_AESR_OVMULE    0x00000008 /* signed overflow in mul */
#define SPR_AESR_DBZE      0x00000010 /* divide by zero */
#define SPR_AESR_CYMACADDE 0x00000020 /* unsigned overflow in mac add */
#define SPR_AESR_OVMACADDE 0x00000040 /* signed overflow in mac add */

/*
 * Bit definitions for the Supervision Register
 *
 */
#define SPR_SR_SM          0x00000001  /* Supervisor Mode */
#define SPR_SR_TEE         0x00000002  /* Tick timer Exception Enable */
#define SPR_SR_IEE         0x00000004  /* Interrupt Exception Enable */
#define SPR_SR_DCE         0x00000008  /* Data Cache Enable */
#define SPR_SR_ICE         0x00000010  /* Instruction Cache Enable */
#define SPR_SR_DME         0x00000020  /* Data MMU Enable */
#define SPR_SR_IME         0x00000040  /* Instruction MMU Enable */
#define SPR_SR_LEE         0x00000080  /* Little Endian Enable */
#define SPR_SR_CE          0x00000100  /* CID Enable */
#define SPR_SR_F           0x00000200  /* Condition Flag */
#define SPR_SR_CY          0x00000400  /* Carry flag */
#define SPR_SR_OV          0x00000800  /* Overflow flag */
#define SPR_SR_OVE         0x00001000  /* Overflow flag Exception */
#define SPR_SR_DSX         0x00002000  /* Delay Slot Exception */
#define SPR_SR_EPH         0x00004000  /* Exception Prefix High */
#define SPR_SR_FO          0x00008000  /* Fixed one */
#define SPR_SR_SUMRA       0x00010000  /* Supervisor SPR read access */
#define SPR_SR_RES         0x0ffe0000  /* Reserved */
#define SPR_SR_CID         0xf0000000  /* Context ID */

/*
 * Bit definitions for the Data MMU Control Register
 *
 */
#define SPR_DMMUCR_P2S     0x0000003e  /* Level 2 Page Size */
#define SPR_DMMUCR_P1S     0x000007c0  /* Level 1 Page Size */
#define SPR_DMMUCR_VADDR_WIDTH  0x0000f800  /* Virtual ADDR Width */
#define SPR_DMMUCR_PADDR_WIDTH  0x000f0000  /* Physical ADDR Width */

/*
 * Bit definitions for the Instruction MMU Control Register
 *
 */
#define SPR_IMMUCR_P2S     0x0000003e  /* Level 2 Page Size */
#define SPR_IMMUCR_P1S     0x000007c0  /* Level 1 Page Size */
#define SPR_IMMUCR_VADDR_WIDTH  0x0000f800  /* Virtual ADDR Width */
#define SPR_IMMUCR_PADDR_WIDTH  0x000f0000  /* Physical ADDR Width */

/*
 * Bit definitions for the Data TLB Match Register
 *
 */
#define SPR_DTLBMR_V       0x00000001  /* Valid */
#define SPR_DTLBMR_PL1     0x00000002  /* Page Level 1 (if 0 then PL2) */
#define SPR_DTLBMR_CID     0x0000003c  /* Context ID */
#define SPR_DTLBMR_LRU     0x000000c0  /* Least Recently Used */
#define SPR_DTLBMR_VPN     0xffffe000  /* Virtual Page Number */

/*
 * Bit definitions for the Data TLB Translate Register
 *
 */
#define SPR_DTLBTR_CC      0x00000001  /* Cache Coherency */
#define SPR_DTLBTR_CI      0x00000002  /* Cache Inhibit */
#define SPR_DTLBTR_WBC     0x00000004  /* Write-Back Cache */
#define SPR_DTLBTR_WOM     0x00000008  /* Weakly-Ordered Memory */
#define SPR_DTLBTR_A       0x00000010  /* Accessed */
#define SPR_DTLBTR_D       0x00000020  /* Dirty */
#define SPR_DTLBTR_URE     0x00000040  /* User Read Enable */
#define SPR_DTLBTR_UWE     0x00000080  /* User Write Enable */
#define SPR_DTLBTR_SRE     0x00000100  /* Supervisor Read Enable */
#define SPR_DTLBTR_SWE     0x00000200  /* Supervisor Write Enable */
#define SPR_DTLBTR_PPN     0xffffe000  /* Physical Page Number */

#define DTLB_PR_NOLIMIT  ( SPR_DTLBTR_URE  | \
                           SPR_DTLBTR_UWE  | \
                           SPR_DTLBTR_SRE  | \
                           SPR_DTLBTR_SWE  )

/*
 * Bit definitions for the Instruction TLB Match Register
 *
 */
#define SPR_ITLBMR_V       0x00000001  /* Valid */
#define SPR_ITLBMR_PL1     0x00000002  /* Page Level 1 (if 0 then PL2) */
#define SPR_ITLBMR_CID     0x0000003c  /* Context ID */
#define SPR_ITLBMR_LRU     0x000000c0  /* Least Recently Used */
#define SPR_ITLBMR_VPN     0xffffe000  /* Virtual Page Number */

/*
 * Bit definitions for the Instruction TLB Translate Register
 *
 */
#define SPR_ITLBTR_CC      0x00000001  /* Cache Coherency */
#define SPR_ITLBTR_CI      0x00000002  /* Cache Inhibit */
#define SPR_ITLBTR_WBC     0x00000004  /* Write-Back Cache */
#define SPR_ITLBTR_WOM     0x00000008  /* Weakly-Ordered Memory */
#define SPR_ITLBTR_A       0x00000010  /* Accessed */
#define SPR_ITLBTR_D       0x00000020  /* Dirty */
#define SPR_ITLBTR_SXE     0x00000040  /* User Read Enable */
#define SPR_ITLBTR_UXE     0x00000080  /* User Write Enable */
#define SPR_ITLBTR_PPN     0xffffe000  /* Physical Page Number */

#define ITLB_PR_NOLIMIT  ( SPR_ITLBTR_SXE  |    \
                           SPR_ITLBTR_UXE  )

/*
 * Bit definitions for Data Cache Control register
 *
 */
#define SPR_DCCR_EW        0x000000ff  /* Enable ways */

/*
 * Bit definitions for Insn Cache Control register
 *
 */
#define SPR_ICCR_EW        0x000000ff  /* Enable ways */

/*
 * Bit definitions for Data Cache Configuration Register
 *
 */

#define SPR_DCCFGR_NCW          0x00000007
#define SPR_DCCFGR_NCS          0x00000078
#define SPR_DCCFGR_CBS          0x00000080
#define SPR_DCCFGR_CWS          0x00000100
#define SPR_DCCFGR_CCRI         0x00000200
#define SPR_DCCFGR_CBIRI        0x00000400
#define SPR_DCCFGR_CBPRI        0x00000800
#define SPR_DCCFGR_CBLRI        0x00001000
#define SPR_DCCFGR_CBFRI        0x00002000
#define SPR_DCCFGR_CBWBRI       0x00004000

#define SPR_DCCFGR_NCW_OFF      0
#define SPR_DCCFGR_NCS_OFF      3
#define SPR_DCCFGR_CBS_OFF      7

/*
 * Bit definitions for Instruction Cache Configuration Register
 *
 */
#define SPR_ICCFGR_NCW          0x00000007
#define SPR_ICCFGR_NCS          0x00000078
#define SPR_ICCFGR_CBS          0x00000080
#define SPR_ICCFGR_CCRI         0x00000200
#define SPR_ICCFGR_CBIRI        0x00000400
#define SPR_ICCFGR_CBPRI        0x00000800
#define SPR_ICCFGR_CBLRI        0x00001000

#define SPR_ICCFGR_NCW_OFF      0
#define SPR_ICCFGR_NCS_OFF      3
#define SPR_ICCFGR_CBS_OFF      7

/*
 * Bit definitions for Data MMU Configuration Register
 *
 */

#define SPR_DMMUCFGR_NTW        0x00000003
#define SPR_DMMUCFGR_NTS        0x0000001C
#define SPR_DMMUCFGR_NAE        0x000000E0
#define SPR_DMMUCFGR_CRI        0x00000100
#define SPR_DMMUCFGR_PRI        0x00000200
#define SPR_DMMUCFGR_TEIRI      0x00000400
#define SPR_DMMUCFGR_HTR        0x00000800

#define SPR_DMMUCFGR_NTW_OFF    0
#define SPR_DMMUCFGR_NTS_OFF    2

/*
 * Bit definitions for Instruction MMU Configuration Register
 *
 */

#define SPR_IMMUCFGR_NTW        0x00000003
#define SPR_IMMUCFGR_NTS        0x0000001C
#define SPR_IMMUCFGR_NAE        0x000000E0
#define SPR_IMMUCFGR_CRI        0x00000100
#define SPR_IMMUCFGR_PRI        0x00000200
#define SPR_IMMUCFGR_TEIRI      0x00000400
#define SPR_IMMUCFGR_HTR        0x00000800

#define SPR_IMMUCFGR_NTW_OFF    0
#define SPR_IMMUCFGR_NTS_OFF    2

/*
 * Bit definitions for Debug Control registers
 *
 */
#define SPR_DCR_DP      0x00000001  /* DVR/DCR present */
#define SPR_DCR_CC      0x0000000e  /* Compare condition */
#define SPR_DCR_SC      0x00000010  /* Signed compare */
#define SPR_DCR_CT      0x000000e0  /* Compare to */

/* Bit results with SPR_DCR_CC mask */
#define SPR_DCR_CC_MASKED 0x00000000
#define SPR_DCR_CC_EQUAL  0x00000002
#define SPR_DCR_CC_LESS   0x00000004
#define SPR_DCR_CC_LESSE  0x00000006
#define SPR_DCR_CC_GREAT  0x00000008
#define SPR_DCR_CC_GREATE 0x0000000a
#define SPR_DCR_CC_NEQUAL 0x0000000c

/* Bit results with SPR_DCR_CT mask */
#define SPR_DCR_CT_DISABLED 0x00000000
#define SPR_DCR_CT_IFEA     0x00000020
#define SPR_DCR_CT_LEA      0x00000040
#define SPR_DCR_CT_SEA      0x00000060
#define SPR_DCR_CT_LD       0x00000080
#define SPR_DCR_CT_SD       0x000000a0
#define SPR_DCR_CT_LSEA     0x000000c0
#define SPR_DCR_CT_LSD      0x000000e0
/* SPR_DCR_CT_LSD doesn't seem to be implemented anywhere in or1ksim. 2004-1-30 HP */

/*
 * Bit definitions for Debug Mode 1 register
 *
 */
#define SPR_DMR1_CW       0x000fffff  /* Chain register pair data */
#define SPR_DMR1_CW0_AND  0x00000001
#define SPR_DMR1_CW0_OR   0x00000002
#define SPR_DMR1_CW0      (SPR_DMR1_CW0_AND | SPR_DMR1_CW0_OR)
#define SPR_DMR1_CW1_AND  0x00000004
#define SPR_DMR1_CW1_OR   0x00000008
#define SPR_DMR1_CW1      (SPR_DMR1_CW1_AND | SPR_DMR1_CW1_OR)
#define SPR_DMR1_CW2_AND  0x00000010
#define SPR_DMR1_CW2_OR   0x00000020
#define SPR_DMR1_CW2      (SPR_DMR1_CW2_AND | SPR_DMR1_CW2_OR)
#define SPR_DMR1_CW3_AND  0x00000040
#define SPR_DMR1_CW3_OR   0x00000080
#define SPR_DMR1_CW3      (SPR_DMR1_CW3_AND | SPR_DMR1_CW3_OR)
#define SPR_DMR1_CW4_AND  0x00000100
#define SPR_DMR1_CW4_OR   0x00000200
#define SPR_DMR1_CW4      (SPR_DMR1_CW4_AND | SPR_DMR1_CW4_OR)
#define SPR_DMR1_CW5_AND  0x00000400
#define SPR_DMR1_CW5_OR   0x00000800
#define SPR_DMR1_CW5      (SPR_DMR1_CW5_AND | SPR_DMR1_CW5_OR)
#define SPR_DMR1_CW6_AND  0x00001000
#define SPR_DMR1_CW6_OR   0x00002000
#define SPR_DMR1_CW6      (SPR_DMR1_CW6_AND | SPR_DMR1_CW6_OR)
#define SPR_DMR1_CW7_AND  0x00004000
#define SPR_DMR1_CW7_OR   0x00008000
#define SPR_DMR1_CW7      (SPR_DMR1_CW7_AND | SPR_DMR1_CW7_OR)
#define SPR_DMR1_CW8_AND  0x00010000
#define SPR_DMR1_CW8_OR   0x00020000
#define SPR_DMR1_CW8      (SPR_DMR1_CW8_AND | SPR_DMR1_CW8_OR)
#define SPR_DMR1_CW9_AND  0x00040000
#define SPR_DMR1_CW9_OR   0x00080000
#define SPR_DMR1_CW9      (SPR_DMR1_CW9_AND | SPR_DMR1_CW9_OR)
#define SPR_DMR1_RES1      0x00300000  /* Reserved */
#define SPR_DMR1_ST       0x00400000  /* Single-step trace*/
#define SPR_DMR1_BT       0x00800000  /* Branch trace */
#define SPR_DMR1_RES2     0xff000000  /* Reserved */

/*
 * Bit definitions for Debug Mode 2 register. AWTC and WGB corrected by JPB
 *
 */
#define SPR_DMR2_WCE0      0x00000001  /* Watchpoint counter 0 enable */
#define SPR_DMR2_WCE1      0x00000002  /* Watchpoint counter 0 enable */
#define SPR_DMR2_AWTC      0x00000ffc  /* Assign watchpoints to counters */
#define SPR_DMR2_AWTC_OFF           2  /* Bit offset to AWTC field */
#define SPR_DMR2_WGB       0x003ff000  /* Watchpoints generating breakpoint */
#define SPR_DMR2_WGB_OFF           12  /* Bit offset to WGB field */
#define SPR_DMR2_WBS       0xffc00000  /* JPB: Watchpoint status */
#define SPR_DMR2_WBS_OFF           22  /* Bit offset to WBS field */

/*
 * Bit definitions for Debug watchpoint counter registers
 *
 */
#define SPR_DWCR_COUNT      0x0000ffff  /* Count */
#define SPR_DWCR_MATCH      0xffff0000  /* Match */
#define SPR_DWCR_MATCH_OFF          16  /* Match bit offset */

/*
 * Bit definitions for Debug stop register
 *
 */
#define SPR_DSR_RSTE    0x00000001  /* Reset exception */
#define SPR_DSR_BUSEE   0x00000002  /* Bus error exception */
#define SPR_DSR_DPFE    0x00000004  /* Data Page Fault exception */
#define SPR_DSR_IPFE    0x00000008  /* Insn Page Fault exception */
#define SPR_DSR_TTE     0x00000010  /* Tick Timer exception */
#define SPR_DSR_AE      0x00000020  /* Alignment exception */
#define SPR_DSR_IIE     0x00000040  /* Illegal Instruction exception */
#define SPR_DSR_IE      0x00000080  /* Interrupt exception */
#define SPR_DSR_DME     0x00000100  /* DTLB miss exception */
#define SPR_DSR_IME     0x00000200  /* ITLB miss exception */
#define SPR_DSR_RE      0x00000400  /* Range exception */
#define SPR_DSR_SCE     0x00000800  /* System call exception */
#define SPR_DSR_FPE     0x00001000  /* Floating Point Exception */
#define SPR_DSR_TE      0x00002000  /* Trap exception */

/*
 * Bit definitions for Debug reason register
 *
 */
#define SPR_DRR_RSTE    0x00000001  /* Reset exception */
#define SPR_DRR_BUSEE   0x00000002  /* Bus error exception */
#define SPR_DRR_DPFE    0x00000004  /* Data Page Fault exception */
#define SPR_DRR_IPFE    0x00000008  /* Insn Page Fault exception */
#define SPR_DRR_TTE     0x00000010  /* Tick Timer exception */
#define SPR_DRR_AE      0x00000020  /* Alignment exception */
#define SPR_DRR_IIE     0x00000040  /* Illegal Instruction exception */
#define SPR_DRR_IE      0x00000080  /* Interrupt exception */
#define SPR_DRR_DME     0x00000100  /* DTLB miss exception */
#define SPR_DRR_IME     0x00000200  /* ITLB miss exception */
#define SPR_DRR_RE      0x00000400  /* Range exception */
#define SPR_DRR_SCE     0x00000800  /* System call exception */
#define SPR_DRR_FPE     0x00001000  /* Floating Point Exception */
#define SPR_DRR_TE      0x00002000  /* Trap exception */

/*
 * Bit definitions for Performance counters mode registers
 *
 */
#define SPR_PCER_CYCLES         0  /* Count the number of cycles the core was running */
#define SPR_PCER_INSTR          1  /* Count the number of instructions executed */
#define SPR_PCER_LD_STALL               2  /* Number of load data hazards */
#define SPR_PCER_JMP_STALL              3  /* Number of jump register data hazards */
#define SPR_PCER_IMISS          4  /* Cycles waiting for instruction fetches. i.e. the number of instructions wasted due to non-ideal caches */
#define SPR_PCER_WBRANCH                5  /* Number of wrong predicted branches */
#define SPR_PCER_WBRANCH_CYC    6  /* Cycles wasted due to wrong predicted branches */
#define SPR_PCER_LD             7  /* Number of memory loads executed. Misaligned accesses are counted twice */
#define SPR_PCER_ST             8  /* Number of memory stores executed. Misaligned accesses are counted twice */
#define SPR_PCER_JUMP           9  /* Number of jump instructions seen, i.e. j, jr, jal, jalr */
#define SPR_PCER_BRANCH         10  /* Number of jump instructions seen, i.e. bf, bnf */
#define SPR_PCER_DELAY_NOP              11  /* Number of empty delay slots, i.e. delay slots filled with a nop */
#define SPR_PCER_LD_EXT         12  /* Number of memory loads to EXT executed. Misaligned accesses are counted twice. Every non-TCDM access is considered external */
#define SPR_PCER_ST_EXT         13  /* Number of memory stores to EXT executed. Misaligned accesses are counted twice. Every non-TCDM access is considered external */
#define SPR_PCER_LD_EXT_CYC     14  /* Cycles used for memory loads to EXT. Every non-TCDM access is considered external */
#define SPR_PCER_ST_EXT_CYC     15  /* Cycles used for memory stores to EXT. Every non-TCDM access is considered external */
#define SPR_PCER_TCDM_CONT      16  /* Cycles wasted due to TCDM/log-interconnect contention */

// Gives from the event ID, the HW mask that can be stored (with an OR with other events mask) to the PCER
#define SPR_PCER_EVENT_MASK(eventId)    (1<<(eventId))
#define SPR_PCER_ALL_EVENTS_MASK  0xffffffff

#define SPR_PCMR_ACTIVE           0x1 /* Activate counting */
#define SPR_PCMR_SATURATE         0x2 /* Activate saturation */

#define SPR_PCER_NAME(id) (id == 0 ? "Cycles" : id == 1 ? "Instructions" : id == 2 ? "LD_Stall" : id == 3 ? "Jmp_Stall" : id == 4 ? "IMISS" : id == 5 ? "WBRANCH" : id == 6 ? "WBRANCH_CYC" : id == 7 ? "LD" : id == 8 ? "ST" : id == 9 ? "JUMP" : id == 10 ? "BRANCH" : id == 11 ? "DELAY_NOP" : id == 12 ? "LD_EXT" : id == 13 ? "ST_EXT" : id == 14 ? "LD_EXT_CYC" : id == 15 ? "ST_EXT_CYC" : id == 16 ? "TCDM_CONT" : "NA")

/*
 * Bit definitions for the Power management register
 *
 */
#define SPR_PMR_SDF     0x0000000f  /* Slow down factor */
#define SPR_PMR_DME     0x00000010  /* Doze mode enable */
#define SPR_PMR_SME     0x00000020  /* Sleep mode enable */
#define SPR_PMR_DCGE    0x00000040  /* Dynamic clock gating enable */
#define SPR_PMR_SUME    0x00000080  /* Suspend mode enable */

/*
 * Bit definitions for PICMR
 *
 */
#define SPR_PICMR_IUM   0xfffffffc  /* Interrupt unmask */

/*
 * Bit definitions for PICPR
 *
 */
#define SPR_PICPR_IPRIO 0xfffffffc  /* Interrupt priority */

/*
 * Bit definitions for PICSR
 *
 */
#define SPR_PICSR_IS    0xffffffff  /* Interrupt status */

/*
 * Bit definitions for Tick Timer Control Register
 *
 */
#define SPR_TTCR_CNT    0xffffffff  /* Count, time period */
#define SPR_TTMR_TP     0x0fffffff  /* Time period */
#define SPR_TTMR_IP     0x10000000  /* Interrupt Pending */
#define SPR_TTMR_IE     0x20000000  /* Interrupt Enable */
#define SPR_TTMR_DI     0x00000000  /* Disabled */
#define SPR_TTMR_RT     0x40000000  /* Restart tick */
#define SPR_TTMR_SR     0x80000000  /* Single run */
#define SPR_TTMR_CR     0xc0000000  /* Continuous run */
#define SPR_TTMR_M      0xc0000000  /* Tick mode */

/*
 * Bit definitions for the FP Control Status Register
 *
 */
#define SPR_FPCSR_FPEE  0x00000001  /* Floating Point Exception Enable */
#define SPR_FPCSR_RM    0x00000006  /* Rounding Mode */
#define SPR_FPCSR_OVF   0x00000008  /* Overflow Flag */
#define SPR_FPCSR_UNF   0x00000010  /* Underflow Flag */
#define SPR_FPCSR_SNF   0x00000020  /* SNAN Flag */
#define SPR_FPCSR_QNF   0x00000040  /* QNAN Flag */
#define SPR_FPCSR_ZF    0x00000080  /* Zero Flag */
#define SPR_FPCSR_IXF   0x00000100  /* Inexact Flag */
#define SPR_FPCSR_IVF   0x00000200  /* Invalid Flag */
#define SPR_FPCSR_INF   0x00000400  /* Infinity Flag */
#define SPR_FPCSR_DZF   0x00000800  /* Divide By Zero Flag */
#define SPR_FPCSR_ALLF (SPR_FPCSR_OVF | SPR_FPCSR_UNF | SPR_FPCSR_SNF | \
                        SPR_FPCSR_QNF | SPR_FPCSR_ZF | SPR_FPCSR_IXF |  \
                        SPR_FPCSR_IVF | SPR_FPCSR_INF | SPR_FPCSR_DZF)

#define FPCSR_RM_RN (0<<1)
#define FPCSR_RM_RZ (1<<1)
#define FPCSR_RM_RIP (2<<1)
#define FPCSR_RM_RIN (3<<1)

/*
 * l.nop constants
 *
 */
#define NOP_NOP          0x0000      /* Normal nop instruction */
#define NOP_EXIT         0x0001      /* End of simulation */
#define NOP_REPORT       0x0002      /* Simple report */
/*#define NOP_PRINTF       0x0003       Simprintf instruction (obsolete)*/
#define NOP_PUTC         0x0004      /* JPB: Simputc instruction */
#define NOP_CNT_RESET    0x0005      /* Reset statistics counters */
#define NOP_GET_TICKS    0x0006      /* JPB: Get # ticks running */
#define NOP_GET_PS       0x0007      /* JPB: Get picosecs/cycle */
#define NOP_TRACE_ON     0x0008      /* Turn on tracing */
#define NOP_TRACE_OFF    0x0009      /* Turn off tracing */
#define NOP_RANDOM       0x000a      /* Return 4 random bytes */
#define NOP_OR1KSIM      0x000b      /* Return non-zero if this is Or1ksim */
#define NOP_EXIT_SILENT  0x000c      /* End of simulation, quiet version */
#define NOP_GETC         0x000d      /* Simgetc instruction */
#define NOP_PUTC_BUFFERED 0x000e      /* Buffered putc */
#define NOP_PUTC_FLUSH   0x000f      /* Buffered putc flush */
#define NOP_CUSTOM       0x0100      /* Custom call, used by system simulator */
#define NOP_CUSTOM_OFFSET      8
#define NOP_CUSTOM_REGS_OFFSET 13

#endif  /* SPR_DEFS__H */
