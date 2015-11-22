/*
=================================================================
Copyright (C) 2008 Torben Koenke

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
MA  02110-1301, USA.
=================================================================
*/

#ifndef _Z80_H_
#define _Z80_H_

#include <stdio.h>
#include <stdlib.h>

/* define GAMEBOY_CPU to compile a custom Z80 core as is used in
	Nintendo's Gameboy */
#define GAMEBOY_CPU

/* try to figure out endianess */
#if defined(__i386__) || defined(__ia64__) || defined(WIN32) || \
	(defined(__mips__) && defined(__MIPSEL__))
 #define LITTLE_ENDIAN
#elif defined(__m68k__) || defined(mc68000) || defined(_M_M68K) \
	|| (defined(__mips__) && defined(__MIPSEB__))
 #define BIG_ENDIAN
#else
 #error "Unknown architecture, please define endianess"
#endif

/* change these to match your system's architecture */
#if defined(__i386__) || defined(__ia64__) || defined(WIN32)
 typedef unsigned char u8;
 typedef unsigned short u16;
 typedef unsigned int u32;
 typedef char s8;
 typedef short s16;
 typedef int s32;
#else
 #error "Unknown architecture, please define datatype size"
#endif

/* disable annoying but harmless size mismatch warning */
#ifdef WIN32
 #pragma warning(disable: 4761)
#endif

#define ERRHALT	0xFFFF0000
#define GETBYTE(x) (x->mem_read(x->pc++))

#ifdef LITTLE_ENDIAN
 #define GETSHORT(x) ((x->mem_read(++x->pc) << 8) | x->mem_read(x->pc++ - 1))
#else
 #define GETSHORT(x) ((x->mem_read(x->pc++) << 8) | x->mem_read(x->pc++))
#endif

#define FL_ZERO		(1 << 7)
#define FL_SUB		(1 << 6)
#define FL_HCARRY	(1 << 5)
#define FL_CARRY	(1 << 4)
#define SETFLAG(r, f) (r->u_af.F |= (f))
#define GETFLAG(r, f) ((r->u_af.F & (f)) ? 1:0)
#define CLRFLAG(r, f) (r->u_af.F &= ~(f))

typedef struct {
	u16 pc;			/* program counter */
	u16 sp;			/* stack pointer */
	union {
		struct {
#ifndef LITTLE_ENDIAN
			u8 A,F;
#else
			u8 F,A;
#endif
		};
		u16 AF;
	} u_af;
	union {
		struct {
#ifndef LITTLE_ENDIAN
			u8 B,C;
#else
			u8 C,B;
#endif
		};
		u16 BC;
	} u_bc;
	union {
		struct {
#ifndef LITTLE_ENDIAN
			u8 D,E;
#else
			u8 E,D;
#endif
		};
		u16 DE;
	} u_de;
	union {
		struct {
#ifndef LITTLE_ENDIAN
			u8 H,L;
#else
			u8 L,H;
#endif
		};
		u16 HL;
	} u_hl;
	u8 (*mem_read)(u16 addr);
	void (*mem_write)(u16 addr, u8 data);
	u8 IFF; /* interrupt enable flip-flop */
} z80_machine_t;

typedef enum {
	OP_LD_B_N			= 0x06,		/* load immediate byte into B */
	OP_LD_C_N			= 0x0E,		/* load immediate byte into C */
	OP_LD_D_N			= 0x16,		/* load immediate byte into D */
	OP_LD_E_N			= 0x1E,		/* load immediate byte into E */
	OP_LD_H_N			= 0x26,		/* load immediate byte into H */
	OP_LD_L_N			= 0x2E,		/* load immediate byte into L */

	OP_LD_A_A			= 0x7F,		/* load A into A */
	OP_LD_A_B			= 0x78,		/* load B into A */
	OP_LD_A_C			= 0x79,		/* load C into A */
	OP_LD_A_D			= 0x7A,		/* load D into A */
	OP_LD_A_E			= 0x7B,		/* load E into A */
	OP_LD_A_H			= 0x7C,		/* load H into A */
	OP_LD_A_L			= 0x7D,		/* load L into A */
	OP_LD_A_ADDR_HL		= 0x7E,		/* load [HL] into A */

	OP_LD_B_B			= 0x40,		/* load B into B */
	OP_LD_B_C			= 0x41,		/* load C into B */
	OP_LD_B_D			= 0x42,		/* load D into B */
	OP_LD_B_E			= 0x43,		/* load E into B */
	OP_LD_B_H			= 0x44,		/* load H into B */
	OP_LD_B_L			= 0x45,		/* load L into B */
	OP_LD_B_ADDR_HL		= 0x46,		/* load [HL] into B */

	OP_LD_C_B			= 0x48,		/* load B into C */
	OP_LD_C_C			= 0x49,		/* load C into C */
	OP_LD_C_D			= 0x4A,		/* load D into C */
	OP_LD_C_E			= 0x4B,		/* load E into C */
	OP_LD_C_H			= 0x4C,		/* load H into C */
	OP_LD_C_L			= 0x4D,		/* load L into C */
	OP_LD_C_ADDR_HL		= 0x4E,		/* load [HL] into C */

	OP_LD_D_B			= 0x50,		/* load B into D */
	OP_LD_D_C			= 0x51,		/* load C into D */
	OP_LD_D_D			= 0x52,		/* load D into D */
	OP_LD_D_E			= 0x53,		/* load E into D */
	OP_LD_D_H			= 0x54,		/* load H into D */
	OP_LD_D_L			= 0x55,		/* load L into D */
	OP_LD_D_ADDR_HL		= 0x56,		/* load [HL] into D */

	OP_LD_E_B			= 0x58,		/* load B into E */
	OP_LD_E_C			= 0x59,		/* load C into E */
	OP_LD_E_D			= 0x5A,		/* load D into E */
	OP_LD_E_E			= 0x5B,		/* load E into E */
	OP_LD_E_H			= 0x5C,		/* load H into E */
	OP_LD_E_L			= 0x5D,		/* load L into E */
	OP_LD_E_ADDR_HL		= 0x5E,		/* load [HL] into E */

	OP_LD_H_B			= 0x60,		/* load B into H */
	OP_LD_H_C			= 0x61,		/* load C into H */
	OP_LD_H_D			= 0x62,		/* load D into H */
	OP_LD_H_E			= 0x63,		/* load E into H */
	OP_LD_H_H			= 0x64,		/* load H into H */
	OP_LD_H_L			= 0x65,		/* load L into H */
	OP_LD_H_ADDR_HL		= 0x66,		/* load [HL] into H */

	OP_LD_L_B			= 0x68,		/* load B into L */
	OP_LD_L_C			= 0x69,		/* load C into L */
	OP_LD_L_D			= 0x6A,		/* load D into L */
	OP_LD_L_E			= 0x6B,		/* load E into L */
	OP_LD_L_H			= 0x6C,		/* load H into L */
	OP_LD_L_L			= 0x6D,		/* load L into L */
	OP_LD_L_ADDR_HL		= 0x6E,		/* load [HL] into L */

	OP_LD_ADDR_HL_B		= 0x70,		/* load B into [HL] */
	OP_LD_ADDR_HL_C		= 0x71,		/* load C into [HL] */
	OP_LD_ADDR_HL_D		= 0x72,		/* load D into [HL] */
	OP_LD_ADDR_HL_E		= 0x73,		/* load E into [HL] */
	OP_LD_ADDR_HL_H		= 0x74,		/* load H into [HL] */
	OP_LD_ADDR_HL_L		= 0x75,		/* load L into [HL] */
	OP_LD_ADDR_HL_N		= 0x36,		/* load imm. byte into [HL] */

	OP_LD_A_ADDR_BC		= 0x0A,		/* load [BC] into A */
	OP_LD_A_ADDR_DE		= 0x1A,		/* load [DE] into A */
	OP_LD_A_ADDR_NN		= 0xFA,		/* load [imm.2b] into A */
	OP_LD_A_N			= 0x3E,		/* load imm.b into A */

	OP_LD_B_A			= 0x47,		/* load A into B */
	OP_LD_C_A			= 0x4F,		/* load A into C */
	OP_LD_D_A			= 0x57,		/* load A into D */
	OP_LD_E_A			= 0x5F,		/* load A into E */
	OP_LD_H_A			= 0x67,		/* load A into H */
	OP_LD_L_A			= 0x6F,		/* load A into L */
	OP_LD_ADDR_BC_A		= 0x02,		/* load A into [BC] */
	OP_LD_ADDR_DE_A		= 0x12,		/* load A into [DE] */
	OP_LD_ADDR_HL_A		= 0x77,		/* load A into [HL] */
	OP_LD_ADDR_NN_A		= 0xEA,		/* load A into [imm.2b] */

	OP_LD_A_ADDR_C_FF00	= 0xF2,		/* load [C + 0xFF00] into A */
	OP_LD_ADDR_C_FF00_A	= 0xE2,		/* load A into [C + 0xFF00] */

	OP_LDD_A_ADDR_HL	= 0x3A,		/* load [HL] into A. Decr. HL */
	OP_LDD_ADDR_HL_A	= 0x32,		/* load A into [HL]. Decr. HL */
	OP_LDI_A_ADDR_HL	= 0x2A,		/* load [HL] into A. Incr. HL */
	OP_LDI_ADDR_HL_A	= 0x22,		/* load A into [HL]. Incr. HL */
	
	OP_LD_ADDR_N_FF00_A	= 0xE0,		/* load A into [imm.b + 0xFF00] */
	OP_LD_A_ADDR_N_FF00	= 0xF0,		/* load [imm.b + 0xFF00] into A */

	/* 16-Bit loads */
	OP_LD_BC_NN			= 0x01,		/* load imm.2b into BC */
	OP_LD_DE_NN			= 0x11,		/* load imm.2b into DE */
	OP_LD_HL_NN			= 0x21,		/* load imm.2b into HL */
	OP_LD_SP_NN			= 0x31,		/* load imm.2b into SP */

	OP_LD_SP_HL			= 0xF9,		/* load HL into SP */
	OP_LD_HL_SP_N		= 0xF8,		/* load SP + imm.b into HL */
	OP_LD_ADDR_NN_SP	= 0x08,		/* load SP into [imm.2b] */

	OP_PUSH_AF			= 0xF5,		/* push AF onto stack */
	OP_PUSH_BC			= 0xC5,		/* push BC onto stack */
	OP_PUSH_DE			= 0xD5,		/* push DE onto stack */
	OP_PUSH_HL			= 0xE5,		/* push HL onto stack */

	OP_POP_AF			= 0xF1,		/* pop from stack into AF */
	OP_POP_BC			= 0xC1,		/* pop from stack into BC */
	OP_POP_DE			= 0xD1,		/* pop from stack into DE */
	OP_POP_HL			= 0xE1,		/* pop from stack into HL */

	/* ABOVE INSTRUCTIONS 100 % CORRECT */

	/* 8-Bit ALU */
	OP_ADD_A_A			= 0x87,		/* add A to A */
	OP_ADD_A_B			= 0x80,		/* add B to A */
	OP_ADD_A_C			= 0x81,		/* add C to A */
	OP_ADD_A_D			= 0x82,		/* add D to A */
	OP_ADD_A_E			= 0x83,		/* add E to A */
	OP_ADD_A_H			= 0x84,		/* add H to A */
	OP_ADD_A_L			= 0x85,		/* add L to A */
	OP_ADD_A_ADDR_HL	= 0x86,		/* add [HL] to A */
	OP_ADD_A_N			= 0xC6,		/* add imm.b to A */

	OP_ADC_A_A			= 0x8F,		/* add A + Carry to A */
	OP_ADC_A_B			= 0x88,		/* add B + Carry to A */
	OP_ADC_A_C			= 0x89,		/* add C + Carry to A */
	OP_ADC_A_D			= 0x8A,		/* add D + Carry to A */
	OP_ADC_A_E			= 0x8B,		/* add E + Carry to A */
	OP_ADC_A_H			= 0x8C,		/* add H + Carry to A */
	OP_ADC_A_L			= 0x8D,		/* add L + Carry to A */
	OP_ADC_A_ADDR_HL	= 0x8E,		/* add [HL] + Carry to A */
	OP_ADC_A_N			= 0xCE,		/* add imm.b + Carry to A */

	OP_SUB_A			= 0x97,		/* subtract A from A */
	OP_SUB_B			= 0x90,		/* subtract B from A */
	OP_SUB_C			= 0x91,		/* subtract C from A */
	OP_SUB_D			= 0x92,		/* subtract D from A */
	OP_SUB_E			= 0x93,		/* subtract E from A */
	OP_SUB_H			= 0x94,		/* subtract H from A */
	OP_SUB_L			= 0x95,		/* subtract L from A */
	OP_SUB_ADDR_HL		= 0x96,		/* subtract [HL] from A */
	OP_SUB_N			= 0xD6,		/* subtract imm.b from A */

	OP_SBC_A_A			= 0x9F,		/* subtract A + Carry from A */
	OP_SBC_A_B			= 0x98,		/* subtract B + Carry from A */
	OP_SBC_A_C			= 0x99,		/* subtract C + Carry from A */
	OP_SBC_A_D			= 0x9A,		/* subtract D + Carry from A */
	OP_SBC_A_E			= 0x9B,		/* subtract E + Carry from A */
	OP_SBC_A_H			= 0x9C,		/* subtract H + Carry from A */
	OP_SBC_A_L			= 0x9D,		/* subtract L + Carry from A */
	OP_SBC_A_ADDR_HL	= 0x9E,		/* subtract [HL] + Carry from A */
	OP_SBC_A_N			= 0xDE,		/* subtract imm.b + Carry from A */

	OP_AND_A_A			= 0xA7,		/* AND A with A */
	OP_AND_A_B			= 0xA0,		/* AND A with B */
	OP_AND_A_C			= 0xA1,		/* AND A with C */
	OP_AND_A_D			= 0xA2,		/* AND A with D */
	OP_AND_A_E			= 0xA3,		/* AND A with E */
	OP_AND_A_H			= 0xA4,		/* AND A with H */
	OP_AND_A_L			= 0xA5,		/* AND A with L */
	OP_AND_A_ADDR_HL	= 0xA6,		/* AND A with [HL] */
	OP_AND_A_N			= 0xE6,		/* AND A with imm.b */

	OP_OR_A_A			= 0xB7,		/* OR A with A */
	OP_OR_A_B			= 0xB0,		/* OR A with B */
	OP_OR_A_C			= 0xB1,		/* OR A with C */
	OP_OR_A_D			= 0xB2,		/* OR A with D */
	OP_OR_A_E			= 0xB3,		/* OR A with E */
	OP_OR_A_H			= 0xB4,		/* OR A with H */
	OP_OR_A_L			= 0xB5,		/* OR A with L */
	OP_OR_A_ADDR_HL		= 0xB6,		/* OR A with [HL] */
	OP_OR_A_N			= 0xF6,		/* OR A with imm.b */

	OP_XOR_A_A			= 0xAF,		/* XOR A with A */
	OP_XOR_A_B			= 0xA8,		/* XOR A with B */
	OP_XOR_A_C			= 0xA9,		/* XOR A with C */
	OP_XOR_A_D			= 0xAA,		/* XOR A with D */
	OP_XOR_A_E			= 0xAB,		/* XOR A with E */
	OP_XOR_A_H			= 0xAC,		/* XOR A with H */
	OP_XOR_A_L			= 0xAD,		/* XOR A with L */
	OP_XOR_A_ADDR_HL	= 0xAE,		/* XOR A with [HL] */
	OP_XOR_A_N			= 0xEE,		/* XOR A with imm.b */

	OP_CP_A				= 0xBF,		/* Compare A with A */
	OP_CP_B				= 0xB8,		/* Compare B with A */
	OP_CP_C				= 0xB9,		/* Compare C with A */
	OP_CP_D				= 0xBA,		/* Compare D with A */
	OP_CP_E				= 0xBB,		/* Compare E with A */
	OP_CP_H				= 0xBC,		/* Compare H with A */
	OP_CP_L				= 0xBD,		/* Compare L with A */
	OP_CP_ADDR_HL		= 0xBE,		/* Compare [HL] with A */
	OP_CP_N				= 0xFE,		/* Compare imm.b with A */

	OP_INC_A			= 0x3C,		/* A++ */
	OP_INC_B			= 0x04,		/* B++ */
	OP_INC_C			= 0x0C,		/* C++ */
	OP_INC_D			= 0x14,		/* D++ */
	OP_INC_E			= 0x1C,		/* E++ */
	OP_INC_H			= 0x24,		/* H++ */
	OP_INC_L			= 0x2C,		/* L++ */
	OP_INC_ADDR_HL		= 0x34,		/* [HL]++ */

	OP_DEC_A			= 0x3D,		/* A-- */
	OP_DEC_B			= 0x05,		/* B-- */
	OP_DEC_C			= 0x0D,		/* C-- */
	OP_DEC_D			= 0x15,		/* D-- */
	OP_DEC_E			= 0x1D,		/* E-- */
	OP_DEC_H			= 0x25,		/* H-- */
	OP_DEC_L			= 0x2D,		/* L-- */
	OP_DEC_ADDR_HL		= 0x35,		/* [HL]-- */

	OP_ADD_HL_BC		= 0x09,		/* Add BC to HL */
	OP_ADD_HL_DE		= 0x19,		/* add DE to HL */
	OP_ADD_HL_HL		= 0x29,		/* add HL to HL */
	OP_ADD_HL_SP		= 0x39,		/* add SP to HL */
	OP_ADD_SP_N			= 0xE8,		/* add signed imm.b to SP */

	OP_INC_BC			= 0x03,		/* BC++ */
	OP_INC_DE			= 0x13,		/* DE++ */
	OP_INC_HL			= 0x23,		/* HL++ */
	OP_INC_SP			= 0x33,		/* SP++ */

	OP_DEC_BC			= 0x0B,		/* BC-- */
	OP_DEC_DE			= 0x1B,		/* DE-- */
	OP_DEC_HL			= 0x2B,		/* HL-- */
	OP_DEC_SP			= 0x3B,		/* SP-- */

	OP_CB_PREFIX		= 0xCB,		/* CB prefixed opcode */
	
	OP_DAA				= 0x27,		/* A BCD Adjust */
	OP_CPL				= 0x2F,		/* Complement A */
	OP_CCF				= 0x3F,		/* Complement Carry flag */
	OP_SCF				= 0x37,		/* Set Carry flag */

	OP_NOP				= 0x00,		/* No Operation */
	OP_HALT				= 0x76,		/* Halt CPU until next Interrupt */
	OP_STOP				= 0x10,		/* Stop CPU and power down display
										until button press. This is actually
										0x1000 */
	OP_DI				= 0xF3,		/* Disable Interrupts */
	OP_EI				= 0xFB,		/* Enable Interrupts */
	
	OP_RLCA				= 0x07,		/* Rotate A left with Carry */
	OP_RLA				= 0x17,		/* Rotate A left through Carry */
	OP_RRCA				= 0x0F,		/* Rotate A right with Carry */
	OP_RRA				= 0x1F,		/* Rotate A right through Carry */

	OP_JP_NN			= 0xC3,		/* Jump to imm.2b address */
	OP_JP_NZ_NN			= 0xC2,		/* Jump if not zero */
	OP_JP_Z_NN			= 0xCA,		/* Jump if zero */
	OP_JP_NC_NN			= 0xD2,		/* Jump if not carry */
	OP_JP_C_NN			= 0xDA,		/* Jump if carry */
	OP_JP_ADDR_HL		= 0xE9,		/* Jump to [HL] */
	OP_JR_N				= 0x18,		/* Rel. jump to pc + imm.b */
	OP_JR_NZ_N			= 0x20,		/* Rel. jump if not zero */
	OP_JR_Z_N			= 0x28,		/* Rel. jump if zero */
	OP_JR_NC_N			= 0x30,		/* Rel. jump if not carry */
	OP_JR_C_N			= 0x38,		/* Rel. jump if carry */

	OP_CALL_NN			= 0xCD,		/* push pc + 1, jump to imm.2b addr */
	OP_CALL_NZ_NN		= 0xC4,		/* If not zero, push pc + 1, jump */
	OP_CALL_Z_NN		= 0xCC,		/* If zero, push pc + 1, jump */
	OP_CALL_NC_NN		= 0xD4,		/* If not carry, push pc + 1, jump */
	OP_CALL_C_NN		= 0xDC,		/* If carry, push pc + 1, jump */

	OP_RST_00			= 0xC7,		/* push pc, jump to 0x0000 */
	OP_RST_08			= 0xCF,		/* push pc, jump to 0x0008 */
	OP_RST_10			= 0xD7,		/* push pc, jump to 0x0010 */
	OP_RST_18			= 0xDF,		/* push pc, jump to 0x0018 */
	OP_RST_20			= 0xE7,		/* push pc, jump to 0x0020 */
	OP_RST_28			= 0xEF,		/* push pc, jump to 0x0028 */
	OP_RST_30			= 0xF7,		/* push pc, jump to 0x0030 */
	OP_RST_38			= 0xFF,		/* push pc, jump to 0x0038 */

	OP_RET				= 0xC9,		/* pop 2b from stack into pc */
	OP_RET_NZ			= 0xC0,		/* If not zero, pop 2b from stack into pc */
	OP_RET_Z			= 0xC8,		/* If zero, pop 2b from stack into pc */
	OP_RET_NC			= 0xD0,		/* If not carry, pop 2b from stack into pc */
	OP_RET_C			= 0xD8,		/* If carry, pop 2b from stack into pc */

	OP_RETI				= 0xD9,		/* pop 2b from stack into, enable interrupts */

} opcode_t;

/* CB prefixed opcodes */
typedef enum {
	OP_SWAP_A			= 0x37,		/* Swap nibbles of A */
	OP_SWAP_B			= 0x30,		/* Swap nibbles of B */
	OP_SWAP_C			= 0x31,		/* Swap nibbles of C */
	OP_SWAP_D			= 0x32,		/* Swap nibbles of D */
	OP_SWAP_E			= 0x33,		/* Swap nibbles of E */
	OP_SWAP_H			= 0x34,		/* Swap nibbles of H */
	OP_SWAP_L			= 0x35,		/* Swap nibbles of L */
	OP_SWAP_ADDR_HL		= 0x36,		/* Swap nibbles of [HL] */

	OP_RLC_A			= 0x07,		/* Rotate A left with Carry */
	OP_RLC_B			= 0x00,		/* Rotate B left with Carry */
	OP_RLC_C			= 0x01,		/* Rotate C left with Carry */
	OP_RLC_D			= 0x02,		/* Rotate D left with Carry */
	OP_RLC_E			= 0x03,		/* Rotate E left with Carry */
	OP_RLC_H			= 0x04,		/* Rotate H left with Carry */
	OP_RLC_L			= 0x05,		/* Rotate L left with Carry */
	OP_RLC_ADDR_HL		= 0x06,		/* Rotate [HL] left with Carry */

	OP_RL_A				= 0x17,		/* Rotate A left through Carry */
	OP_RL_B				= 0x10,		/* Rotate B left through Carry */
	OP_RL_C				= 0x11,		/* Rotate C left through Carry */
	OP_RL_D				= 0x12,		/* Rotate D left through Carry */
	OP_RL_E				= 0x13,		/* Rotate E left through Carry */
	OP_RL_H				= 0x14,		/* Rotate H left through Carry */
	OP_RL_L				= 0x15,		/* Rotate L left through Carry */
	OP_RL_ADDR_HL		= 0x16,		/* Rotate [HL] left through Carry */

	OP_RRC_A			= 0x0F,		/* Rotate A right with Carry */
	OP_RRC_B			= 0x08,		/* Rotate B right with Carry */
	OP_RRC_C			= 0x09,		/* Rotate C right with Carry */
	OP_RRC_D			= 0x0A,		/* Rotate D right with Carry */
	OP_RRC_E			= 0x0B,		/* Rotate E right with Carry */
	OP_RRC_H			= 0x0C,		/* Rotate H right with Carry */
	OP_RRC_L			= 0x0D,		/* Rotate L right with Carry */
	OP_RRC_ADDR_HL		= 0x0E,		/* Rotate [HL] right with Carry */

	OP_RR_A				= 0x1F,		/* Rotate A right through Carry */
	OP_RR_B				= 0x18,		/* Rotate B right through Carry */
	OP_RR_C				= 0x19,		/* Rotate C right through Carry */
	OP_RR_D				= 0x1A,		/* Rotate D right through Carry */
	OP_RR_E				= 0x1B,		/* Rotate E right through Carry */
	OP_RR_H				= 0x1C,		/* Rotate H right through Carry */
	OP_RR_L				= 0x1D,		/* Rotate L right through Carry */
	OP_RR_ADDR_HL		= 0x1E,		/* Rotate [HL] right through Carry */

	OP_SLA_A			= 0x27,		/* Shift A left arithmetic */
	OP_SLA_B			= 0x20,		/* Shift B left arithmetic */
	OP_SLA_C			= 0x21,		/* Shift C left arithmetic */
	OP_SLA_D			= 0x22,		/* Shift D left arithmetic */
	OP_SLA_E			= 0x23,		/* Shift E left arithmetic */
	OP_SLA_H			= 0x24,		/* Shift H left arithmetic */
	OP_SLA_L			= 0x25,		/* Shift L left arithmetic */
	OP_SLA_ADDR_HL		= 0x26,		/* Shift [HL] left arithmetic */

	OP_SRA_A			= 0x2F,		/* Shift A right arithmetic */
	OP_SRA_B			= 0x28,		/* Shift B right arithmetic */
	OP_SRA_C			= 0x29,		/* Shift C right arithmetic */
	OP_SRA_D			= 0x2A,		/* Shift D right arithmetic */
	OP_SRA_E			= 0x2B,		/* Shift E right arithmetic */
	OP_SRA_H			= 0x2C,		/* Shift H right arithmetic */
	OP_SRA_L			= 0x2D,		/* Shift L right arithmetic */
	OP_SRA_ADDR_HL		= 0x2E,		/* Shift [HL] right arithmetic */

	OP_SRL_A			= 0x3F,		/* Shift A right logic */
	OP_SRL_B			= 0x38,		/* Shift B right logic */
	OP_SRL_C			= 0x39,		/* Shift C right logic */
	OP_SRL_D			= 0x3A,		/* Shift D right logic */
	OP_SRL_E			= 0x3B,		/* Shift E right logic */
	OP_SRL_H			= 0x3C,		/* Shift H right logic */
	OP_SRL_L			= 0x3D,		/* Shift L right logic */
	OP_SRL_ADDR_HL		= 0x3E,		/* Shift [HL] right logic */

	OP_BIT_A_0			= 0x47,		/* Test bit 0 of A */
	OP_BIT_A_1			= 0x4F,		/* Test bit 1 of A */
	OP_BIT_A_2			= 0x57,		/* Test bit 2 of A */
	OP_BIT_A_3			= 0x5F,		/* Test bit 3 of A */
	OP_BIT_A_4			= 0x67,		/* Test bit 4 of A */
	OP_BIT_A_5			= 0x6F,		/* Test bit 5 of A */
	OP_BIT_A_6			= 0x77,		/* Test bit 6 of A */
	OP_BIT_A_7			= 0x7F,		/* Test bit 7 of A */

	OP_BIT_B_0			= 0x40,		/* Test bit 0 of B */
	OP_BIT_B_1			= 0x48,		/* Test bit 1 of B */
	OP_BIT_B_2			= 0x50,		/* Test bit 2 of B */
	OP_BIT_B_3			= 0x58,		/* Test bit 3 of B */
	OP_BIT_B_4			= 0x60,		/* Test bit 4 of B */
	OP_BIT_B_5			= 0x68,		/* Test bit 5 of B */
	OP_BIT_B_6			= 0x70,		/* Test bit 6 of B */
	OP_BIT_B_7			= 0x78,		/* Test bit 7 of B */

	OP_BIT_C_0			= 0x41,		/* Test bit 0 of C */
	OP_BIT_C_1			= 0x49,		/* Test bit 1 of C */
	OP_BIT_C_2			= 0x51,		/* Test bit 2 of C */
	OP_BIT_C_3			= 0x59,		/* Test bit 3 of C */
	OP_BIT_C_4			= 0x61,		/* Test bit 4 of C */
	OP_BIT_C_5			= 0x69,		/* Test bit 5 of C */
	OP_BIT_C_6			= 0x71,		/* Test bit 6 of C */
	OP_BIT_C_7			= 0x79,		/* Test bit 7 of C */

	OP_BIT_D_0			= 0x42,		/* Test bit 0 of D */
	OP_BIT_D_1			= 0x4A,		/* Test bit 1 of D */
	OP_BIT_D_2			= 0x52,		/* Test bit 2 of D */
	OP_BIT_D_3			= 0x5A,		/* Test bit 3 of D */
	OP_BIT_D_4			= 0x62,		/* Test bit 4 of D */
	OP_BIT_D_5			= 0x6A,		/* Test bit 5 of D */
	OP_BIT_D_6			= 0x72,		/* Test bit 6 of D */
	OP_BIT_D_7			= 0x7A,		/* Test bit 7 of D */

	OP_BIT_E_0			= 0x43,		/* Test bit 0 of E */
	OP_BIT_E_1			= 0x4B,		/* Test bit 1 of E */
	OP_BIT_E_2			= 0x53,		/* Test bit 2 of E */
	OP_BIT_E_3			= 0x5B,		/* Test bit 3 of E */
	OP_BIT_E_4			= 0x63,		/* Test bit 4 of E */
	OP_BIT_E_5			= 0x6B,		/* Test bit 5 of E */
	OP_BIT_E_6			= 0x73,		/* Test bit 6 of E */
	OP_BIT_E_7			= 0x7B,		/* Test bit 7 of E */

	OP_BIT_H_0			= 0x44,		/* Test bit 0 of H */
	OP_BIT_H_1			= 0x4C,		/* Test bit 1 of H */
	OP_BIT_H_2			= 0x54,		/* Test bit 2 of H */
	OP_BIT_H_3			= 0x5C,		/* Test bit 3 of H */
	OP_BIT_H_4			= 0x64,		/* Test bit 4 of H */
	OP_BIT_H_5			= 0x6C,		/* Test bit 5 of H */
	OP_BIT_H_6			= 0x74,		/* Test bit 6 of H */
	OP_BIT_H_7			= 0x7C,		/* Test bit 7 of H */

	OP_BIT_L_0			= 0x45,		/* Test bit 0 of L */
	OP_BIT_L_1			= 0x4D,		/* Test bit 1 of L */
	OP_BIT_L_2			= 0x55,		/* Test bit 2 of L */
	OP_BIT_L_3			= 0x5D,		/* Test bit 3 of L */
	OP_BIT_L_4			= 0x65,		/* Test bit 4 of L */
	OP_BIT_L_5			= 0x6D,		/* Test bit 5 of L */
	OP_BIT_L_6			= 0x75,		/* Test bit 6 of L */
	OP_BIT_L_7			= 0x7D,		/* Test bit 7 of L */

	OP_BIT_ADDR_HL_0	= 0x46,		/* Test bit 0 of [HL] */
	OP_BIT_ADDR_HL_1	= 0x4E,		/* Test bit 1 of [HL] */
	OP_BIT_ADDR_HL_2	= 0x56,		/* Test bit 2 of [HL] */
	OP_BIT_ADDR_HL_3	= 0x5E,		/* Test bit 3 of [HL] */
	OP_BIT_ADDR_HL_4	= 0x66,		/* Test bit 4 of [HL] */
	OP_BIT_ADDR_HL_5	= 0x6E,		/* Test bit 5 of [HL] */
	OP_BIT_ADDR_HL_6	= 0x76,		/* Test bit 6 of [HL] */
	OP_BIT_ADDR_HL_7	= 0x7E,		/* Test bit 7 of [HL] */

	OP_SET_A_0			= 0xC7,		/* Set bit 0 of A */
	OP_SET_A_1			= 0xCF,		/* Set bit 1 of A */
	OP_SET_A_2			= 0xD7,		/* Set bit 2 of A */
	OP_SET_A_3			= 0xDF,		/* Set bit 3 of A */
	OP_SET_A_4			= 0xE7,		/* Set bit 4 of A */
	OP_SET_A_5			= 0xEF,		/* Set bit 5 of A */
	OP_SET_A_6			= 0xF7,		/* Set bit 6 of A */
	OP_SET_A_7			= 0xFF,		/* Set bit 7 of A */

	OP_SET_B_0			= 0xC0,		/* Set bit 0 of B */
	OP_SET_B_1			= 0xC8,		/* Set bit 1 of B */
	OP_SET_B_2			= 0xD0,		/* Set bit 2 of B */
	OP_SET_B_3			= 0xD8,		/* Set bit 3 of B */
	OP_SET_B_4			= 0xE0,		/* Set bit 4 of B */
	OP_SET_B_5			= 0xE8,		/* Set bit 5 of B */
	OP_SET_B_6			= 0xF0,		/* Set bit 6 of B */
	OP_SET_B_7			= 0xF8,		/* Set bit 7 of B */

	OP_SET_C_0			= 0xC1,		/* Set bit 0 of C */
	OP_SET_C_1			= 0xC9,		/* Set bit 1 of C */
	OP_SET_C_2			= 0xD1,		/* Set bit 2 of C */
	OP_SET_C_3			= 0xD9,		/* Set bit 3 of C */
	OP_SET_C_4			= 0xE1,		/* Set bit 4 of C */
	OP_SET_C_5			= 0xE9,		/* Set bit 5 of C */
	OP_SET_C_6			= 0xF1,		/* Set bit 6 of C */
	OP_SET_C_7			= 0xF9,		/* Set bit 7 of C */

	OP_SET_D_0			= 0xC2,		/* Set bit 0 of D */
	OP_SET_D_1			= 0xCA,		/* Set bit 1 of D */
	OP_SET_D_2			= 0xD2,		/* Set bit 2 of D */
	OP_SET_D_3			= 0xDA,		/* Set bit 3 of D */
	OP_SET_D_4			= 0xE2,		/* Set bit 4 of D */
	OP_SET_D_5			= 0xEA,		/* Set bit 5 of D */
	OP_SET_D_6			= 0xF2,		/* Set bit 6 of D */
	OP_SET_D_7			= 0xFA,		/* Set bit 7 of D */

	OP_SET_E_0			= 0xC3,		/* Set bit 0 of E */
	OP_SET_E_1			= 0xCB,		/* Set bit 1 of E */
	OP_SET_E_2			= 0xD3,		/* Set bit 2 of E */
	OP_SET_E_3			= 0xDB,		/* Set bit 3 of E */
	OP_SET_E_4			= 0xE3,		/* Set bit 4 of E */
	OP_SET_E_5			= 0xEB,		/* Set bit 5 of E */
	OP_SET_E_6			= 0xF3,		/* Set bit 6 of E */
	OP_SET_E_7			= 0xFB,		/* Set bit 7 of E */

	OP_SET_H_0			= 0xC4,		/* Set bit 0 of H */
	OP_SET_H_1			= 0xCC,		/* Set bit 1 of H */
	OP_SET_H_2			= 0xD4,		/* Set bit 2 of H */
	OP_SET_H_3			= 0xDC,		/* Set bit 3 of H */
	OP_SET_H_4			= 0xE4,		/* Set bit 4 of H */
	OP_SET_H_5			= 0xEC,		/* Set bit 5 of H */
	OP_SET_H_6			= 0xF4,		/* Set bit 6 of H */
	OP_SET_H_7			= 0xFC,		/* Set bit 7 of H */

	OP_SET_L_0			= 0xC5,		/* Set bit 0 of L */
	OP_SET_L_1			= 0xCD,		/* Set bit 1 of L */
	OP_SET_L_2			= 0xD5,		/* Set bit 2 of L */
	OP_SET_L_3			= 0xDD,		/* Set bit 3 of L */
	OP_SET_L_4			= 0xE5,		/* Set bit 4 of L */
	OP_SET_L_5			= 0xED,		/* Set bit 5 of L */
	OP_SET_L_6			= 0xF5,		/* Set bit 6 of L */
	OP_SET_L_7			= 0xFD,		/* Set bit 7 of L */

	OP_SET_ADDR_HL_0	= 0xC6,		/* Set bit 0 of [HL] */
	OP_SET_ADDR_HL_1	= 0xCE,		/* Set bit 1 of [HL] */
	OP_SET_ADDR_HL_2	= 0xD6,		/* Set bit 2 of [HL] */
	OP_SET_ADDR_HL_3	= 0xDE,		/* Set bit 3 of [HL] */
	OP_SET_ADDR_HL_4	= 0xE6,		/* Set bit 4 of [HL] */
	OP_SET_ADDR_HL_5	= 0xEE,		/* Set bit 5 of [HL] */
	OP_SET_ADDR_HL_6	= 0xF6,		/* Set bit 6 of [HL] */
	OP_SET_ADDR_HL_7	= 0xFE,		/* Set bit 7 of [HL] */

	OP_RES_A_0			= 0x87,		/* Reset bit 0 of A */
	OP_RES_A_1			= 0x8F,		/* Reset bit 1 of A */
	OP_RES_A_2			= 0x97,		/* Reset bit 2 of A */
	OP_RES_A_3			= 0x9F,		/* Reset bit 3 of A */
	OP_RES_A_4			= 0xA7,		/* Reset bit 4 of A */
	OP_RES_A_5			= 0xAF,		/* Reset bit 5 of A */
	OP_RES_A_6			= 0xB7,		/* Reset bit 6 of A */
	OP_RES_A_7			= 0xBF,		/* Reset bit 7 of A */

	OP_RES_B_0			= 0x80,		/* Reset bit 0 of B */
	OP_RES_B_1			= 0x88,		/* Reset bit 1 of B */
	OP_RES_B_2			= 0x90,		/* Reset bit 2 of B */
	OP_RES_B_3			= 0x98,		/* Reset bit 3 of B */
	OP_RES_B_4			= 0xA0,		/* Reset bit 4 of B */
	OP_RES_B_5			= 0xA8,		/* Reset bit 5 of B */
	OP_RES_B_6			= 0xB0,		/* Reset bit 6 of B */
	OP_RES_B_7			= 0xB8,		/* Reset bit 7 of B */

	OP_RES_C_0			= 0x81,		/* Reset bit 0 of C */
	OP_RES_C_1			= 0x89,		/* Reset bit 1 of C */
	OP_RES_C_2			= 0x91,		/* Reset bit 2 of C */
	OP_RES_C_3			= 0x99,		/* Reset bit 3 of C */
	OP_RES_C_4			= 0xA1,		/* Reset bit 4 of C */
	OP_RES_C_5			= 0xA9,		/* Reset bit 5 of C */
	OP_RES_C_6			= 0xB1,		/* Reset bit 6 of C */
	OP_RES_C_7			= 0xB9,		/* Reset bit 7 of C */

	OP_RES_D_0			= 0x82,		/* Reset bit 0 of D */
	OP_RES_D_1			= 0x8A,		/* Reset bit 1 of D */
	OP_RES_D_2			= 0x92,		/* Reset bit 2 of D */
	OP_RES_D_3			= 0x9A,		/* Reset bit 3 of D */
	OP_RES_D_4			= 0xA2,		/* Reset bit 4 of D */
	OP_RES_D_5			= 0xAA,		/* Reset bit 5 of D */
	OP_RES_D_6			= 0xB2,		/* Reset bit 6 of D */
	OP_RES_D_7			= 0xBA,		/* Reset bit 7 of D */

	OP_RES_E_0			= 0x83,		/* Reset bit 0 of E */
	OP_RES_E_1			= 0x8B,		/* Reset bit 1 of E */
	OP_RES_E_2			= 0x93,		/* Reset bit 2 of E */
	OP_RES_E_3			= 0x9B,		/* Reset bit 3 of E */
	OP_RES_E_4			= 0xA3,		/* Reset bit 4 of E */
	OP_RES_E_5			= 0xAB,		/* Reset bit 5 of E */
	OP_RES_E_6			= 0xB3,		/* Reset bit 6 of E */
	OP_RES_E_7			= 0xBB,		/* Reset bit 7 of E */

	OP_RES_H_0			= 0x84,		/* Reset bit 0 of H */
	OP_RES_H_1			= 0x8C,		/* Reset bit 1 of H */
	OP_RES_H_2			= 0x94,		/* Reset bit 2 of H */
	OP_RES_H_3			= 0x9C,		/* Reset bit 3 of H */
	OP_RES_H_4			= 0xA4,		/* Reset bit 4 of H */
	OP_RES_H_5			= 0xAC,		/* Reset bit 5 of H */
	OP_RES_H_6			= 0xB4,		/* Reset bit 6 of H */
	OP_RES_H_7			= 0xBC,		/* Reset bit 7 of H */

	OP_RES_L_0			= 0x85,		/* Reset bit 0 of L */
	OP_RES_L_1			= 0x8D,		/* Reset bit 1 of L */
	OP_RES_L_2			= 0x95,		/* Reset bit 2 of L */
	OP_RES_L_3			= 0x9D,		/* Reset bit 3 of L */
	OP_RES_L_4			= 0xA5,		/* Reset bit 4 of L */
	OP_RES_L_5			= 0xAD,		/* Reset bit 5 of L */
	OP_RES_L_6			= 0xB5,		/* Reset bit 6 of L */
	OP_RES_L_7			= 0xBD,		/* Reset bit 7 of L */

	OP_RES_ADDR_HL_0	= 0x86,		/* Reset bit 0 of [HL] */
	OP_RES_ADDR_HL_1	= 0x8E,		/* Reset bit 1 of [HL] */
	OP_RES_ADDR_HL_2	= 0x96,		/* Reset bit 2 of [HL] */
	OP_RES_ADDR_HL_3	= 0x9E,		/* Reset bit 3 of [HL] */
	OP_RES_ADDR_HL_4	= 0xA6,		/* Reset bit 4 of [HL] */
	OP_RES_ADDR_HL_5	= 0xAE,		/* Reset bit 5 of [HL] */
	OP_RES_ADDR_HL_6	= 0xB6,		/* Reset bit 6 of [HL] */
	OP_RES_ADDR_HL_7	= 0xBE,		/* Reset bit 7 of [HL] */

} cb_opcode_t;

/* function declarations */
s32 z80_run( z80_machine_t *z80, s32 cycles );
s32 z80_interrupt( z80_machine_t *z80, u16 isr_addr );

extern u32 z80_ictbl[];
extern u32 z80_cb_ictbl[];


#endif /* _Z80_H_ */
