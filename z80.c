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

#include "z80.h"
#include "gameboy.h"

/**
 * z80_run - Runs Z80 CPU for a specified number of clock cycles.
 *
 * @param z80
 *	A pointer to a z80_machine_t structure.
 * @param cycles
 *	The number of clock cycles to execute before returning
 *		from the function.
 *
 * @return
 *	The difference of the number of requested clock cycles and
 *		the actual number of clock cycles the CPU ran. This
 *		may be 0 or a negative value.
 */
s32 z80_run( z80_machine_t *z80, s32 cycles ) {
	u8 opc, op1;
	u32 t, t2;
	s32 left = cycles;

	while(left > 0) {
		/* fetch next opcode from instruction stream */
		opc = z80->mem_read(z80->pc++);

		/* subtract cycles for this instruction */
		left = left - z80_ictbl[opc];

		switch(opc) {
			case OP_LD_B_N:
				z80->u_bc.B = GETBYTE(z80);
				break;

			case OP_LD_C_N:
				z80->u_bc.C = GETBYTE(z80);
				break;

			case OP_LD_D_N:
				z80->u_de.D = GETBYTE(z80);
				break;

			case OP_LD_E_N:
				z80->u_de.E = GETBYTE(z80);
				break;

			case OP_LD_H_N:
				z80->u_hl.H = GETBYTE(z80);
				break;

			case OP_LD_L_N:
				z80->u_hl.L = GETBYTE(z80);
				break;

			case OP_LD_A_A:
				z80->u_af.A = z80->u_af.A;
				break;

			case OP_LD_A_B:
				z80->u_af.A = z80->u_bc.B;
				break;

			case OP_LD_A_C:
				z80->u_af.A = z80->u_bc.C;
				break;

			case OP_LD_A_D:
				z80->u_af.A = z80->u_de.D;
				break;

			case OP_LD_A_E:
				z80->u_af.A = z80->u_de.E;
				break;

			case OP_LD_A_H:
				z80->u_af.A = z80->u_hl.H;
				break;

			case OP_LD_A_L:
				z80->u_af.A = z80->u_hl.L;
				break;

			case OP_LD_A_ADDR_HL:
				z80->u_af.A = z80->mem_read(z80->u_hl.HL);
				break;

			case OP_LD_B_B:
				z80->u_bc.B = z80->u_bc.B;
				break;

			case OP_LD_B_C:
				z80->u_bc.B = z80->u_bc.C;
				break;

			case OP_LD_B_D:
				z80->u_bc.B = z80->u_de.D;
				break;

			case OP_LD_B_E:
				z80->u_bc.B = z80->u_de.E;
				break;

			case OP_LD_B_H:
				z80->u_bc.B = z80->u_hl.H;
				break;

			case OP_LD_B_L:
				z80->u_bc.B = z80->u_hl.L;
				break;

			case OP_LD_B_ADDR_HL:
				z80->u_bc.B = z80->mem_read(z80->u_hl.HL);
				break;

			case OP_LD_C_B:
				z80->u_bc.C = z80->u_bc.B;
				break;

			case OP_LD_C_C:
				z80->u_bc.C = z80->u_bc.C;
				break;

			case OP_LD_C_D:
				z80->u_bc.C = z80->u_de.D;
				break;

			case OP_LD_C_E:
				z80->u_bc.C = z80->u_de.E;
				break;

			case OP_LD_C_H:
				z80->u_bc.C = z80->u_hl.H;
				break;

			case OP_LD_C_L:
				z80->u_bc.C = z80->u_hl.L;
				break;

			case OP_LD_C_ADDR_HL:
				z80->u_bc.C = z80->mem_read(z80->u_hl.HL);
				break;

			case OP_LD_D_B:
				z80->u_de.D = z80->u_bc.B;
				break;

			case OP_LD_D_C:
				z80->u_de.D = z80->u_bc.C;
				break;

			case OP_LD_D_D:
				z80->u_de.D = z80->u_de.D;
				break;

			case OP_LD_D_E:
				z80->u_de.D = z80->u_de.E;
				break;

			case OP_LD_D_H:
				z80->u_de.D = z80->u_hl.H;
				break;

			case OP_LD_D_L:
				z80->u_de.D = z80->u_hl.L;
				break;

			case OP_LD_D_ADDR_HL:
				z80->u_de.D = z80->mem_read(z80->u_hl.HL);
				break;

			case OP_LD_E_B:
				z80->u_de.E = z80->u_bc.B;
				break;

			case OP_LD_E_C:
				z80->u_de.E = z80->u_bc.C;
				break;

			case OP_LD_E_D:
				z80->u_de.E = z80->u_de.D;
				break;

			case OP_LD_E_E:
				z80->u_de.E = z80->u_de.E;
				break;

			case OP_LD_E_H:
				z80->u_de.E = z80->u_hl.H;
				break;

			case OP_LD_E_L:
				z80->u_de.E = z80->u_hl.L;
				break;

			case OP_LD_E_ADDR_HL:
				z80->u_de.E = z80->mem_read(z80->u_hl.HL);
				break;

			case OP_LD_H_B:
				z80->u_hl.H = z80->u_bc.B;
				break;

			case OP_LD_H_C:
				z80->u_hl.H = z80->u_bc.C;
				break;

			case OP_LD_H_D:
				z80->u_hl.H = z80->u_de.D;
				break;

			case OP_LD_H_E:
				z80->u_hl.H = z80->u_de.E;
				break;

			case OP_LD_H_H:
				z80->u_hl.H = z80->u_hl.H;
				break;

			case OP_LD_H_L:
				z80->u_hl.H = z80->u_hl.L;
				break;

			case OP_LD_H_ADDR_HL:
				z80->u_hl.H = z80->mem_read(z80->u_hl.HL);
				break;

			case OP_LD_L_B:
				z80->u_hl.L = z80->u_bc.B;
				break;

			case OP_LD_L_C:
				z80->u_hl.L = z80->u_bc.C;
				break;

			case OP_LD_L_D:
				z80->u_hl.L = z80->u_de.D;
				break;

			case OP_LD_L_E:
				z80->u_hl.L = z80->u_de.E;
				break;

			case OP_LD_L_H:
				z80->u_hl.L = z80->u_hl.H;
				break;

			case OP_LD_L_L:
				z80->u_hl.L = z80->u_hl.L;
				break;

			case OP_LD_L_ADDR_HL:
				z80->u_hl.L = z80->mem_read(z80->u_hl.HL);
				break;

			case OP_LD_ADDR_HL_B:
				z80->mem_write(z80->u_hl.HL, z80->u_bc.B);
				break;

			case OP_LD_ADDR_HL_C:
				z80->mem_write(z80->u_hl.HL, z80->u_bc.C);
				break;

			case OP_LD_ADDR_HL_D:
				z80->mem_write(z80->u_hl.HL, z80->u_de.D);
				break;

			case OP_LD_ADDR_HL_E:
				z80->mem_write(z80->u_hl.HL, z80->u_de.E);
				break;

			case OP_LD_ADDR_HL_H:
				z80->mem_write(z80->u_hl.HL, z80->u_hl.H);
				break;

			case OP_LD_ADDR_HL_L:
				z80->mem_write(z80->u_hl.HL, z80->u_hl.L);
				break;

			case OP_LD_ADDR_HL_N:
				z80->mem_write(z80->u_hl.HL, GETBYTE(z80));
				break;

			case OP_LD_A_ADDR_BC:
				z80->u_af.A = z80->mem_read(z80->u_bc.BC);
				break;

			case OP_LD_A_ADDR_DE:
				z80->u_af.A = z80->mem_read(z80->u_de.DE);
				break;

			case OP_LD_A_ADDR_NN:
				z80->u_af.A = z80->mem_read(GETSHORT(z80));
				break;

			case OP_LD_A_N:
				z80->u_af.A = GETBYTE(z80);
				break;

			case OP_LD_B_A:
				z80->u_bc.B = z80->u_af.A;
				break;

			case OP_LD_C_A:
				z80->u_bc.C = z80->u_af.A;
				break;

			case OP_LD_D_A:
				z80->u_de.D = z80->u_af.A;
				break;

			case OP_LD_E_A:
				z80->u_de.E = z80->u_af.A;
				break;

			case OP_LD_H_A:
				z80->u_hl.H = z80->u_af.A;
				break;

			case OP_LD_L_A:
				z80->u_hl.L = z80->u_af.A;
				break;

			case OP_LD_ADDR_BC_A:
				z80->mem_write(z80->u_bc.BC, z80->u_af.A);
				break;

			case OP_LD_ADDR_DE_A:
				z80->mem_write(z80->u_de.DE, z80->u_af.A);
				break;

			case OP_LD_ADDR_HL_A:
				z80->mem_write(z80->u_hl.HL, z80->u_af.A);
				break;

			case OP_LD_ADDR_NN_A:
				z80->mem_write(GETSHORT(z80), z80->u_af.A);
				break;

			case OP_LD_A_ADDR_C_FF00:
				z80->u_af.A = z80->mem_read(0xFF00 + z80->u_bc.C);
				break;

			case OP_LD_ADDR_C_FF00_A:
				z80->mem_write(0xFF00 + z80->u_bc.C, z80->u_af.A);
				break;

			case OP_LDD_A_ADDR_HL:
				z80->u_af.A = z80->mem_read(z80->u_hl.HL--);
				break;

			case OP_LDD_ADDR_HL_A:
				z80->mem_write(z80->u_hl.HL--, z80->u_af.A);
				break;

			case OP_LDI_A_ADDR_HL:
				z80->u_af.A = z80->mem_read(z80->u_hl.HL++);
				break;

			case OP_LDI_ADDR_HL_A:
				z80->mem_write(z80->u_hl.HL++, z80->u_af.A);
				break;

			case OP_LD_ADDR_N_FF00_A:
				z80->mem_write(0xFF00 + GETBYTE(z80), z80->u_af.A);
				break;

			case OP_LD_A_ADDR_N_FF00:
				z80->u_af.A = z80->mem_read(0xFF00 + GETBYTE(z80));
				break;

			case OP_LD_BC_NN:
				z80->u_bc.BC = GETSHORT(z80);
				break;

			case OP_LD_DE_NN:
				z80->u_de.DE = GETSHORT(z80);
				break;

			case OP_LD_HL_NN:
				z80->u_hl.HL = GETSHORT(z80);
				break;

			case OP_LD_SP_NN:
				z80->sp = GETSHORT(z80);
				break;

			case OP_LD_SP_HL:
				z80->sp = z80->u_hl.HL;
				break;

			case OP_LD_HL_SP_N:
				CLRFLAG(z80, FL_ZERO|FL_SUB);
				op1 = GETBYTE(z80);
				(t = z80->sp + (s8)op1) & 0xFFFF0000 ? SETFLAG(z80, FL_CARRY) :
					CLRFLAG(z80, FL_CARRY);
				((z80->sp & 0x0F) + (op1 & 0x0F)) > 0x0F ?
					SETFLAG(z80, FL_HCARRY) : CLRFLAG(z80, FL_HCARRY);
				z80->u_hl.HL = t & 0xFFFF;
				break;

			case OP_LD_ADDR_NN_SP:
				t = GETSHORT(z80);
#ifdef LITTLE_ENDIAN
				z80->mem_write((u16)t, *((u8*)&z80->sp));
				z80->mem_write((u16)(t + 1), *(((u8*)&z80->sp) + 1));
#else
				z80->mem_write((u16)(t + 1), *(((u8*)&z80->sp) + 1));
				z80->mem_write((u16)t, *((u8*)&z80->sp));
#endif
				break;

			case OP_PUSH_AF:
				z80->mem_write(--z80->sp, z80->u_af.F);
				z80->mem_write(--z80->sp, z80->u_af.A);
				break;

			case OP_PUSH_BC:
				z80->mem_write(--z80->sp, z80->u_bc.C);
				z80->mem_write(--z80->sp, z80->u_bc.B);
				break;

			case OP_PUSH_DE:
				z80->mem_write(--z80->sp, z80->u_de.E);
				z80->mem_write(--z80->sp, z80->u_de.D);
				break;

			case OP_PUSH_HL:
				z80->mem_write(--z80->sp, z80->u_hl.L);
				z80->mem_write(--z80->sp, z80->u_hl.H);
				break;

			case OP_POP_AF:
				z80->u_af.A = z80->mem_read(z80->sp++);
				z80->u_af.F = z80->mem_read(z80->sp++);
				break;

			case OP_POP_BC:
				z80->u_bc.B = z80->mem_read(z80->sp++);
				z80->u_bc.C = z80->mem_read(z80->sp++);
				break;

			case OP_POP_DE:
				z80->u_de.D = z80->mem_read(z80->sp++);
				z80->u_de.E = z80->mem_read(z80->sp++);
				break;

			case OP_POP_HL:
				z80->u_hl.H = z80->mem_read(z80->sp++);
				z80->u_hl.L = z80->mem_read(z80->sp++);
				break;

			case OP_ADD_A_A:
				CLRFLAG(z80, FL_SUB);
				((z80->u_af.A & 0x0F) + (z80->u_af.A & 0x0F)) > 0x0F ?
					SETFLAG(z80, FL_HCARRY) : CLRFLAG(z80, FL_HCARRY);
				(t = z80->u_af.A + z80->u_af.A) & 0xFF00 ? SETFLAG(z80, FL_CARRY) :
					CLRFLAG(z80, FL_CARRY);
				(z80->u_af.A = (u8)(t & 0xFF)) ? CLRFLAG(z80, FL_ZERO) :
					SETFLAG(z80, FL_ZERO);
				break;

			case OP_ADD_A_B:
				CLRFLAG(z80, FL_SUB);
				((z80->u_af.A & 0x0F) + (z80->u_bc.B & 0x0F)) > 0x0F ?
					SETFLAG(z80, FL_HCARRY) : CLRFLAG(z80, FL_HCARRY);
				(t = z80->u_af.A + z80->u_bc.B) & 0xFF00 ? SETFLAG(z80, FL_CARRY) :
					CLRFLAG(z80, FL_CARRY);
				(z80->u_af.A = (u8)(t & 0xFF)) ? CLRFLAG(z80, FL_ZERO) :
					SETFLAG(z80, FL_ZERO);
				break;

			case OP_ADD_A_C:
				CLRFLAG(z80, FL_SUB);
				((z80->u_af.A & 0x0F) + (z80->u_bc.C & 0x0F)) > 0x0F ?
					SETFLAG(z80, FL_HCARRY) : CLRFLAG(z80, FL_HCARRY);
				(t = z80->u_af.A + z80->u_bc.C) & 0xFF00 ? SETFLAG(z80, FL_CARRY) :
					CLRFLAG(z80, FL_CARRY);
				(z80->u_af.A = (u8)(t & 0xFF)) ? CLRFLAG(z80, FL_ZERO) :
					SETFLAG(z80, FL_ZERO);
				break;

			case OP_ADD_A_D:
				CLRFLAG(z80, FL_SUB);
				((z80->u_af.A & 0x0F) + (z80->u_de.D & 0x0F)) > 0x0F ?
					SETFLAG(z80, FL_HCARRY) : CLRFLAG(z80, FL_HCARRY);
				(t = z80->u_af.A + z80->u_de.D) & 0xFF00 ? SETFLAG(z80, FL_CARRY) :
					CLRFLAG(z80, FL_CARRY);
				(z80->u_af.A = (u8)(t & 0xFF)) ? CLRFLAG(z80, FL_ZERO) :
					SETFLAG(z80, FL_ZERO);
				break;

			case OP_ADD_A_E:
				CLRFLAG(z80, FL_SUB);
				((z80->u_af.A & 0x0F) + (z80->u_de.E & 0x0F)) > 0x0F ?
					SETFLAG(z80, FL_HCARRY) : CLRFLAG(z80, FL_HCARRY);
				(t = z80->u_af.A + z80->u_de.E) & 0xFF00 ? SETFLAG(z80, FL_CARRY) :
					CLRFLAG(z80, FL_CARRY);
				(z80->u_af.A = (u8)(t & 0xFF)) ? CLRFLAG(z80, FL_ZERO) :
					SETFLAG(z80, FL_ZERO);
				break;

			case OP_ADD_A_H:
				CLRFLAG(z80, FL_SUB);
				((z80->u_af.A & 0x0F) + (z80->u_hl.H & 0x0F)) > 0x0F ?
					SETFLAG(z80, FL_HCARRY) : CLRFLAG(z80, FL_HCARRY);
				(t = z80->u_af.A + z80->u_hl.H) & 0xFF00 ? SETFLAG(z80, FL_CARRY) :
					CLRFLAG(z80, FL_CARRY);
				(z80->u_af.A = (u8)(t & 0xFF)) ? CLRFLAG(z80, FL_ZERO) :
					SETFLAG(z80, FL_ZERO);
				break;

			case OP_ADD_A_L:
				CLRFLAG(z80, FL_SUB);
				((z80->u_af.A & 0x0F) + (z80->u_hl.L & 0x0F)) > 0x0F ?
					SETFLAG(z80, FL_HCARRY) : CLRFLAG(z80, FL_HCARRY);
				(t = z80->u_af.A + z80->u_hl.L) & 0xFF00 ? SETFLAG(z80, FL_CARRY) :
					CLRFLAG(z80, FL_CARRY);
				(z80->u_af.A = (u8)(t & 0xFF)) ? CLRFLAG(z80, FL_ZERO) :
					SETFLAG(z80, FL_ZERO);
				break;

			case OP_ADD_A_ADDR_HL:
				CLRFLAG(z80, FL_SUB);
				op1 = z80->mem_read(z80->u_hl.HL);
				((z80->u_af.A & 0x0F) + (op1 & 0x0F)) > 0x0F ?
					SETFLAG(z80, FL_HCARRY) : CLRFLAG(z80, FL_HCARRY);
				(t = z80->u_af.A + op1) & 0xFF00 ? SETFLAG(z80, FL_CARRY) :
					CLRFLAG(z80, FL_CARRY);
				(z80->u_af.A = (u8)(t & 0xFF)) ? CLRFLAG(z80, FL_ZERO) :
					SETFLAG(z80, FL_ZERO);
				break;

			case OP_ADD_A_N:
				CLRFLAG(z80, FL_SUB);
				op1 = GETBYTE(z80);
				((z80->u_af.A & 0x0F) + (op1 & 0x0F)) > 0x0F ?
					SETFLAG(z80, FL_HCARRY) : CLRFLAG(z80, FL_HCARRY);
				(t = z80->u_af.A + op1) & 0xFF00 ? SETFLAG(z80, FL_CARRY) :
					CLRFLAG(z80, FL_CARRY);
				(z80->u_af.A = (u8)(t & 0xFF)) ? CLRFLAG(z80, FL_ZERO) :
					SETFLAG(z80, FL_ZERO);
				break;

			case OP_ADC_A_A:
				CLRFLAG(z80, FL_SUB);
				t2 = GETFLAG(z80, FL_CARRY);
				((z80->u_af.A & 0x0F) + ((z80->u_af.A + t2) & 0x0F)) > 0x0F ?
					SETFLAG(z80, FL_HCARRY) : CLRFLAG(z80, FL_HCARRY);
				(t = z80->u_af.A + z80->u_af.A + t2) & 0xFF00 ?
					SETFLAG(z80, FL_CARRY) : CLRFLAG(z80, FL_CARRY);
				(z80->u_af.A = (u8)(t & 0xFF)) ? CLRFLAG(z80, FL_ZERO) :
					SETFLAG(z80, FL_ZERO);
				break;

			case OP_ADC_A_B:
				CLRFLAG(z80, FL_SUB);
				t2 = GETFLAG(z80, FL_CARRY);
				((z80->u_af.A & 0x0F) + ((z80->u_bc.B + t2) & 0x0F)) > 0x0F ?
					SETFLAG(z80, FL_HCARRY) : CLRFLAG(z80, FL_HCARRY);
				(t = z80->u_af.A + z80->u_bc.B + t2) & 0xFF00 ?
					SETFLAG(z80, FL_CARRY) : CLRFLAG(z80, FL_CARRY);
				(z80->u_af.A = (u8)(t & 0xFF)) ? CLRFLAG(z80, FL_ZERO) :
					SETFLAG(z80, FL_ZERO);
				break;

			case OP_ADC_A_C:
				CLRFLAG(z80, FL_SUB);
				t2 = GETFLAG(z80, FL_CARRY);
				((z80->u_af.A & 0x0F) + ((z80->u_bc.C + t2) & 0x0F)) > 0x0F ?
					SETFLAG(z80, FL_HCARRY) : CLRFLAG(z80, FL_HCARRY);
				(t = z80->u_af.A + z80->u_bc.C + t2) & 0xFF00 ?
					SETFLAG(z80, FL_CARRY) : CLRFLAG(z80, FL_CARRY);
				(z80->u_af.A = (u8)(t & 0xFF)) ? CLRFLAG(z80, FL_ZERO) :
					SETFLAG(z80, FL_ZERO);
				break;

			case OP_ADC_A_D:
				CLRFLAG(z80, FL_SUB);
				t2 = GETFLAG(z80, FL_CARRY);
				((z80->u_af.A & 0x0F) + ((z80->u_de.D + t2) & 0x0F)) > 0x0F ?
					SETFLAG(z80, FL_HCARRY) : CLRFLAG(z80, FL_HCARRY);
				(t = z80->u_af.A + z80->u_de.D + t2) & 0xFF00 ?
					SETFLAG(z80, FL_CARRY) : CLRFLAG(z80, FL_CARRY);
				(z80->u_af.A = (u8)(t & 0xFF)) ? CLRFLAG(z80, FL_ZERO) :
					SETFLAG(z80, FL_ZERO);
				break;

			case OP_ADC_A_E:
				CLRFLAG(z80, FL_SUB);
				t2 = GETFLAG(z80, FL_CARRY);
				((z80->u_af.A & 0x0F) + ((z80->u_de.E + t2) & 0x0F)) > 0x0F ?
					SETFLAG(z80, FL_HCARRY) : CLRFLAG(z80, FL_HCARRY);
				(t = z80->u_af.A + z80->u_de.E + t2) & 0xFF00 ?
					SETFLAG(z80, FL_CARRY) : CLRFLAG(z80, FL_CARRY);
				(z80->u_af.A = (u8)(t & 0xFF)) ? CLRFLAG(z80, FL_ZERO) :
					SETFLAG(z80, FL_ZERO);
				break;

			case OP_ADC_A_H:
				CLRFLAG(z80, FL_SUB);
				t2 = GETFLAG(z80, FL_CARRY);
				((z80->u_af.A & 0x0F) + ((z80->u_hl.H + t2) & 0x0F)) > 0x0F ?
					SETFLAG(z80, FL_HCARRY) : CLRFLAG(z80, FL_HCARRY);
				(t = z80->u_af.A + z80->u_hl.H + t2) & 0xFF00 ?
					SETFLAG(z80, FL_CARRY) : CLRFLAG(z80, FL_CARRY);
				(z80->u_af.A = (u8)(t & 0xFF)) ? CLRFLAG(z80, FL_ZERO) :
					SETFLAG(z80, FL_ZERO);
				break;

			case OP_ADC_A_L:
				CLRFLAG(z80, FL_SUB);
				t2 = GETFLAG(z80, FL_CARRY);
				((z80->u_af.A & 0x0F) + ((z80->u_hl.L + t2) & 0x0F)) > 0x0F ?
					SETFLAG(z80, FL_HCARRY) : CLRFLAG(z80, FL_HCARRY);
				(t = z80->u_af.A + z80->u_hl.L + t2) & 0xFF00 ?
					SETFLAG(z80, FL_CARRY) : CLRFLAG(z80, FL_CARRY);
				(z80->u_af.A = (u8)(t & 0xFF)) ? CLRFLAG(z80, FL_ZERO) :
					SETFLAG(z80, FL_ZERO);
				break;

			case OP_ADC_A_N:
				CLRFLAG(z80, FL_SUB);
				op1 = GETBYTE(z80);
				t2 = GETFLAG(z80, FL_CARRY);
				((z80->u_af.A & 0x0F) + ((op1 + t2) & 0x0F)) > 0x0F ?
					SETFLAG(z80, FL_HCARRY) : CLRFLAG(z80, FL_HCARRY);
				(t = z80->u_af.A + op1 + t2) & 0xFF00 ?
					SETFLAG(z80, FL_CARRY) : CLRFLAG(z80, FL_CARRY);
				(z80->u_af.A = (u8)(t & 0xFF)) ? CLRFLAG(z80, FL_ZERO) :
					SETFLAG(z80, FL_ZERO);
				break;

			case OP_ADC_A_ADDR_HL:
				CLRFLAG(z80, FL_SUB);
				op1 = z80->mem_read(z80->u_hl.HL);
				t2 = GETFLAG(z80, FL_CARRY);
				((z80->u_af.A & 0x0F) + ((op1 + t2) & 0x0F)) > 0x0F ?
					SETFLAG(z80, FL_HCARRY) : CLRFLAG(z80, FL_HCARRY);
				(t = z80->u_af.A + op1 + t2) & 0xFF00 ?
					SETFLAG(z80, FL_CARRY) : CLRFLAG(z80, FL_CARRY);
				(z80->u_af.A = (u8)(t & 0xFF)) ? CLRFLAG(z80, FL_ZERO) :
					SETFLAG(z80, FL_ZERO);
				break;

			/* GBCPUman.pdf states HCARRY and CARRY bit should be set
				if no borrow occurs for SUB/SBC/DEC operations. Official
				z80 cpu manual states the exact opposite. It's probably
				a mistake in the Gameboy document. */
			case OP_SUB_A:
				SETFLAG(z80, FL_SUB|FL_ZERO);
				CLRFLAG(z80, FL_HCARRY|FL_CARRY);
				z80->u_af.A = 0;
				break;

			case OP_SUB_B:
				SETFLAG(z80, FL_SUB);
				((z80->u_bc.B & 0x0F) > (z80->u_af.A & 0x0F)) ?
					SETFLAG(z80, FL_HCARRY) : CLRFLAG(z80, FL_HCARRY);
				(z80->u_bc.B > z80->u_af.A) ? SETFLAG(z80, FL_CARRY) :
					CLRFLAG(z80, FL_CARRY);
				z80->u_af.A = z80->u_af.A - z80->u_bc.B;
				z80->u_af.A ? CLRFLAG(z80, FL_ZERO) : SETFLAG(z80, FL_ZERO);
				break;

			case OP_SUB_C:
				SETFLAG(z80, FL_SUB);
				((z80->u_bc.C & 0x0F) > (z80->u_af.A & 0x0F)) ?
					SETFLAG(z80, FL_HCARRY) : CLRFLAG(z80, FL_HCARRY);
				(z80->u_bc.C > z80->u_af.A) ? SETFLAG(z80, FL_CARRY) :
					CLRFLAG(z80, FL_CARRY);
				z80->u_af.A = z80->u_af.A - z80->u_bc.C;
				z80->u_af.A ? CLRFLAG(z80, FL_ZERO) : SETFLAG(z80, FL_ZERO);
				break;

			case OP_SUB_D:
				SETFLAG(z80, FL_SUB);
				((z80->u_de.D & 0x0F) > (z80->u_af.A & 0x0F)) ?
					SETFLAG(z80, FL_HCARRY) : CLRFLAG(z80, FL_HCARRY);
				(z80->u_de.D > z80->u_af.A) ? SETFLAG(z80, FL_CARRY) :
					CLRFLAG(z80, FL_CARRY);
				z80->u_af.A = z80->u_af.A - z80->u_de.D;
				z80->u_af.A ? CLRFLAG(z80, FL_ZERO) : SETFLAG(z80, FL_ZERO);
				break;

			case OP_SUB_E:
				SETFLAG(z80, FL_SUB);
				((z80->u_de.E & 0x0F) > (z80->u_af.A & 0x0F)) ?
					SETFLAG(z80, FL_HCARRY) : CLRFLAG(z80, FL_HCARRY);
				(z80->u_de.E > z80->u_af.A) ? SETFLAG(z80, FL_CARRY) :
					CLRFLAG(z80, FL_CARRY);
				z80->u_af.A = z80->u_af.A - z80->u_de.E;
				z80->u_af.A ? CLRFLAG(z80, FL_ZERO) : SETFLAG(z80, FL_ZERO);
				break;

			case OP_SUB_H:
				SETFLAG(z80, FL_SUB);
				((z80->u_hl.H & 0x0F) > (z80->u_af.A & 0x0F)) ?
					SETFLAG(z80, FL_HCARRY) : CLRFLAG(z80, FL_HCARRY);
				(z80->u_hl.H > z80->u_af.A) ? SETFLAG(z80, FL_CARRY) :
					CLRFLAG(z80, FL_CARRY);
				z80->u_af.A = z80->u_af.A - z80->u_hl.H;
				z80->u_af.A ? CLRFLAG(z80, FL_ZERO) : SETFLAG(z80, FL_ZERO);
				break;

			case OP_SUB_L:
				SETFLAG(z80, FL_SUB);
				((z80->u_hl.L & 0x0F) > (z80->u_af.A & 0x0F)) ?
					SETFLAG(z80, FL_HCARRY) : CLRFLAG(z80, FL_HCARRY);
				(z80->u_hl.L > z80->u_af.A) ? SETFLAG(z80, FL_CARRY) :
					CLRFLAG(z80, FL_CARRY);
				z80->u_af.A = z80->u_af.A - z80->u_hl.L;
				z80->u_af.A ? CLRFLAG(z80, FL_ZERO) : SETFLAG(z80, FL_ZERO);
				break;

			case OP_SUB_ADDR_HL:
				SETFLAG(z80, FL_SUB);
				op1 = z80->mem_read(z80->u_hl.HL);
				((op1 & 0x0F) > (z80->u_af.A & 0x0F)) ?
					SETFLAG(z80, FL_HCARRY) : CLRFLAG(z80, FL_HCARRY);
				(op1 > z80->u_af.A) ? SETFLAG(z80, FL_CARRY) :
					CLRFLAG(z80, FL_CARRY);
				z80->u_af.A = z80->u_af.A - op1;
				z80->u_af.A ? CLRFLAG(z80, FL_ZERO) : SETFLAG(z80, FL_ZERO);
				break;

			case OP_SUB_N:
				SETFLAG(z80, FL_SUB);
				op1 = GETBYTE(z80);
				((op1 & 0x0F) > (z80->u_af.A & 0x0F)) ?
					SETFLAG(z80, FL_HCARRY) : CLRFLAG(z80, FL_HCARRY);
				(op1 > z80->u_af.A) ? SETFLAG(z80, FL_CARRY) :
					CLRFLAG(z80, FL_CARRY);
				z80->u_af.A = z80->u_af.A - op1;
				z80->u_af.A ? CLRFLAG(z80, FL_ZERO) : SETFLAG(z80, FL_ZERO);
				break;

			case OP_SBC_A_A:
				SETFLAG(z80, FL_SUB);
				if(GETFLAG(z80, FL_CARRY)) {
					z80->u_af.A = 0xFF;
					CLRFLAG(z80, FL_ZERO);
					SETFLAG(z80, FL_HCARRY|FL_CARRY);
				} else {
					z80->u_af.A = 0;
					SETFLAG(z80, FL_ZERO);
					CLRFLAG(z80, FL_HCARRY|FL_CARRY);
				}
				break;

			case OP_SBC_A_B:
	/*			SETFLAG(z80, FL_SUB);
				t2 = GETFLAG(z80, FL_CARRY);
				(((u8)(z80->u_bc.B + t2) & 0x0F) > (z80->u_af.A & 0x0F)) ?
					SETFLAG(z80, FL_HCARRY) : CLRFLAG(z80, FL_HCARRY);
				((z80->u_bc.B + t2) > z80->u_af.A) ? SETFLAG(z80, FL_CARRY) :
					CLRFLAG(z80, FL_CARRY);
				z80->u_af.A = z80->u_af.A - (z80->u_bc.B + t2);
				z80->u_af.A ? CLRFLAG(z80, FL_ZERO) : SETFLAG(z80, FL_ZERO);*/

				/* FIXME: Is this correct now? The commented-out version
					above could potentially go wrong because (z80->u_bc.B + t2)
					will be treated as integer and wouldn't overflow if the carry
					bit was set and the operand was 255 */
				SETFLAG(z80, FL_SUB);
				op1 = z80->u_bc.B + GETFLAG(z80, FL_CARRY);
				((op1 & 0x0F) > (z80->u_af.A & 0x0F)) ? SETFLAG(z80, FL_HCARRY) :
					CLRFLAG(z80, FL_HCARRY);
				(op1 > z80->u_af.A) ? SETFLAG(z80, FL_CARRY) :
					CLRFLAG(z80, FL_CARRY);
				z80->u_af.A = z80->u_af.A - op1;
				z80->u_af.A ? CLRFLAG(z80, FL_ZERO) : SETFLAG(z80, FL_ZERO);
				break;

			case OP_SBC_A_C:
/*				SETFLAG(z80, FL_SUB);
				t2 = GETFLAG(z80, FL_CARRY);
				(((u8)(z80->u_bc.C + t2) & 0x0F) > (z80->u_af.A & 0x0F)) ?
					SETFLAG(z80, FL_HCARRY) : CLRFLAG(z80, FL_HCARRY);
				((z80->u_bc.C + t2) > z80->u_af.A) ? SETFLAG(z80, FL_CARRY) :
					CLRFLAG(z80, FL_CARRY);
				z80->u_af.A = z80->u_af.A - (z80->u_bc.C + t2);
				z80->u_af.A ? CLRFLAG(z80, FL_ZERO) : SETFLAG(z80, FL_ZERO);*/

				SETFLAG(z80, FL_SUB);
				op1 = z80->u_bc.C + GETFLAG(z80, FL_CARRY);
				((op1 & 0x0F) > (z80->u_af.A & 0x0F)) ? SETFLAG(z80, FL_HCARRY) :
					CLRFLAG(z80, FL_HCARRY);
				(op1 > z80->u_af.A) ? SETFLAG(z80, FL_CARRY) :
					CLRFLAG(z80, FL_CARRY);
				z80->u_af.A = z80->u_af.A - op1;
				z80->u_af.A ? CLRFLAG(z80, FL_ZERO) : SETFLAG(z80, FL_ZERO);
				break;

			case OP_SBC_A_D:
/*				SETFLAG(z80, FL_SUB);
				t2 = GETFLAG(z80, FL_CARRY);
				(((u8)(z80->u_de.D + t2) & 0x0F) > (z80->u_af.A & 0x0F)) ?
					SETFLAG(z80, FL_HCARRY) : CLRFLAG(z80, FL_HCARRY);
				((z80->u_de.D + t2) > z80->u_af.A) ? SETFLAG(z80, FL_CARRY) :
					CLRFLAG(z80, FL_CARRY);
				z80->u_af.A = z80->u_af.A - (z80->u_de.D + t2);
				z80->u_af.A ? CLRFLAG(z80, FL_ZERO) : SETFLAG(z80, FL_ZERO);*/

				SETFLAG(z80, FL_SUB);
				op1 = z80->u_de.D + GETFLAG(z80, FL_CARRY);
				((op1 & 0x0F) > (z80->u_af.A & 0x0F)) ? SETFLAG(z80, FL_HCARRY) :
					CLRFLAG(z80, FL_HCARRY);
				(op1 > z80->u_af.A) ? SETFLAG(z80, FL_CARRY) :
					CLRFLAG(z80, FL_CARRY);
				z80->u_af.A = z80->u_af.A - op1;
				z80->u_af.A ? CLRFLAG(z80, FL_ZERO) : SETFLAG(z80, FL_ZERO);
				break;

			case OP_SBC_A_E:
/*				SETFLAG(z80, FL_SUB);
				t2 = GETFLAG(z80, FL_CARRY);
				(((u8)(z80->u_de.E + t2) & 0x0F) > (z80->u_af.A & 0x0F)) ?
					SETFLAG(z80, FL_HCARRY) : CLRFLAG(z80, FL_HCARRY);
				((z80->u_de.E + t2) > z80->u_af.A) ? SETFLAG(z80, FL_CARRY) :
					CLRFLAG(z80, FL_CARRY);
				z80->u_af.A = z80->u_af.A - (z80->u_de.E + t2);
				z80->u_af.A ? CLRFLAG(z80, FL_ZERO) : SETFLAG(z80, FL_ZERO);*/

				SETFLAG(z80, FL_SUB);
				op1 = z80->u_de.E + GETFLAG(z80, FL_CARRY);
				((op1 & 0x0F) > (z80->u_af.A & 0x0F)) ? SETFLAG(z80, FL_HCARRY) :
					CLRFLAG(z80, FL_HCARRY);
				(op1 > z80->u_af.A) ? SETFLAG(z80, FL_CARRY) :
					CLRFLAG(z80, FL_CARRY);
				z80->u_af.A = z80->u_af.A - op1;
				z80->u_af.A ? CLRFLAG(z80, FL_ZERO) : SETFLAG(z80, FL_ZERO);
				break;

			case OP_SBC_A_H:
/*				SETFLAG(z80, FL_SUB);
				t2 = GETFLAG(z80, FL_CARRY);
				(((u8)(z80->u_hl.H + t2) & 0x0F) > (z80->u_af.A & 0x0F)) ?
					SETFLAG(z80, FL_HCARRY) : CLRFLAG(z80, FL_HCARRY);
				((z80->u_hl.H + t2) > z80->u_af.A) ? SETFLAG(z80, FL_CARRY) :
					CLRFLAG(z80, FL_CARRY);
				z80->u_af.A = z80->u_af.A - (z80->u_hl.H + t2);
				z80->u_af.A ? CLRFLAG(z80, FL_ZERO) : SETFLAG(z80, FL_ZERO);*/

				SETFLAG(z80, FL_SUB);
				op1 = z80->u_hl.H + GETFLAG(z80, FL_CARRY);
				((op1 & 0x0F) > (z80->u_af.A & 0x0F)) ? SETFLAG(z80, FL_HCARRY) :
					CLRFLAG(z80, FL_HCARRY);
				(op1 > z80->u_af.A) ? SETFLAG(z80, FL_CARRY) :
					CLRFLAG(z80, FL_CARRY);
				z80->u_af.A = z80->u_af.A - op1;
				z80->u_af.A ? CLRFLAG(z80, FL_ZERO) : SETFLAG(z80, FL_ZERO);
				break;

			case OP_SBC_A_L:
/*				SETFLAG(z80, FL_SUB);
				t2 = GETFLAG(z80, FL_CARRY);
				(((u8)(z80->u_hl.L + t2) & 0x0F) > (z80->u_af.A & 0x0F)) ?
					SETFLAG(z80, FL_HCARRY) : CLRFLAG(z80, FL_HCARRY);
				((z80->u_hl.L + t2) > z80->u_af.A) ? SETFLAG(z80, FL_CARRY) :
					CLRFLAG(z80, FL_CARRY);
				z80->u_af.A = z80->u_af.A - (z80->u_hl.L + t2);
				z80->u_af.A ? CLRFLAG(z80, FL_ZERO) : SETFLAG(z80, FL_ZERO);*/

				SETFLAG(z80, FL_SUB);
				op1 = z80->u_hl.L + GETFLAG(z80, FL_CARRY);
				((op1 & 0x0F) > (z80->u_af.A & 0x0F)) ? SETFLAG(z80, FL_HCARRY) :
					CLRFLAG(z80, FL_HCARRY);
				(op1 > z80->u_af.A) ? SETFLAG(z80, FL_CARRY) :
					CLRFLAG(z80, FL_CARRY);
				z80->u_af.A = z80->u_af.A - op1;
				z80->u_af.A ? CLRFLAG(z80, FL_ZERO) : SETFLAG(z80, FL_ZERO);
				break;

			case OP_SBC_A_ADDR_HL:
/*				SETFLAG(z80, FL_SUB);
				op1 = z80->mem_read(z80->u_hl.HL);
				t2 = GETFLAG(z80, FL_CARRY);
				(((u8)(op1 + t2) & 0x0F) > (z80->u_af.A & 0x0F)) ?
					SETFLAG(z80, FL_HCARRY) : CLRFLAG(z80, FL_HCARRY);
				((op1 + t2) > z80->u_af.A) ? SETFLAG(z80, FL_CARRY) :
					CLRFLAG(z80, FL_CARRY);
				z80->u_af.A = z80->u_af.A - (op1 + t2);
				z80->u_af.A ? CLRFLAG(z80, FL_ZERO) : SETFLAG(z80, FL_ZERO);*/

				SETFLAG(z80, FL_SUB);
				op1 = z80->mem_read(z80->u_hl.HL) + GETFLAG(z80, FL_CARRY);
				((op1 & 0x0F) > (z80->u_af.A & 0x0F)) ? SETFLAG(z80, FL_HCARRY) :
					CLRFLAG(z80, FL_HCARRY);
				(op1 > z80->u_af.A) ? SETFLAG(z80, FL_CARRY) :
					CLRFLAG(z80, FL_CARRY);
				z80->u_af.A = z80->u_af.A - op1;
				z80->u_af.A ? CLRFLAG(z80, FL_ZERO) : SETFLAG(z80, FL_ZERO);
				break;

			case OP_SBC_A_N:
/*				SETFLAG(z80, FL_SUB);
				op1 = GETBYTE(z80);
				t2 = GETFLAG(z80, FL_CARRY);
				(((u8)(op1 + t2) & 0x0F) > (z80->u_af.A & 0x0F)) ?
					SETFLAG(z80, FL_HCARRY) : CLRFLAG(z80, FL_HCARRY);
				((op1 + t2) > z80->u_af.A) ? SETFLAG(z80, FL_CARRY) :
					CLRFLAG(z80, FL_CARRY);
				z80->u_af.A = z80->u_af.A - (op1 + t2);
				z80->u_af.A ? CLRFLAG(z80, FL_ZERO) : SETFLAG(z80, FL_ZERO);*/

				SETFLAG(z80, FL_SUB);
				op1 = GETBYTE(z80) + GETFLAG(z80, FL_CARRY);
				((op1 & 0x0F) > (z80->u_af.A & 0x0F)) ? SETFLAG(z80, FL_HCARRY) :
					CLRFLAG(z80, FL_HCARRY);
				(op1 > z80->u_af.A) ? SETFLAG(z80, FL_CARRY) :
					CLRFLAG(z80, FL_CARRY);
				z80->u_af.A = z80->u_af.A - op1;
				z80->u_af.A ? CLRFLAG(z80, FL_ZERO) : SETFLAG(z80, FL_ZERO);
				break;

			case OP_AND_A_A:
				CLRFLAG(z80, FL_SUB|FL_CARRY);
				SETFLAG(z80, FL_HCARRY);
				z80->u_af.A = z80->u_af.A & z80->u_af.A;
				z80->u_af.A ? CLRFLAG(z80, FL_ZERO) : SETFLAG(z80, FL_ZERO);
				break;

			case OP_AND_A_B:
				CLRFLAG(z80, FL_SUB|FL_CARRY);
				SETFLAG(z80, FL_HCARRY);
				z80->u_af.A = z80->u_af.A & z80->u_bc.B;
				z80->u_af.A ? CLRFLAG(z80, FL_ZERO) : SETFLAG(z80, FL_ZERO);
				break;

			case OP_AND_A_C:
				CLRFLAG(z80, FL_SUB|FL_CARRY);
				SETFLAG(z80, FL_HCARRY);
				z80->u_af.A = z80->u_af.A & z80->u_bc.C;
				z80->u_af.A ? CLRFLAG(z80, FL_ZERO) : SETFLAG(z80, FL_ZERO);
				break;

			case OP_AND_A_D:
				CLRFLAG(z80, FL_SUB|FL_CARRY);
				SETFLAG(z80, FL_HCARRY);
				z80->u_af.A = z80->u_af.A & z80->u_de.D;
				z80->u_af.A ? CLRFLAG(z80, FL_ZERO) : SETFLAG(z80, FL_ZERO);
				break;

			case OP_AND_A_E:
				CLRFLAG(z80, FL_SUB|FL_CARRY);
				SETFLAG(z80, FL_HCARRY);
				z80->u_af.A = z80->u_af.A & z80->u_de.E;
				z80->u_af.A ? CLRFLAG(z80, FL_ZERO) : SETFLAG(z80, FL_ZERO);
				break;

			case OP_AND_A_H:
				CLRFLAG(z80, FL_SUB|FL_CARRY);
				SETFLAG(z80, FL_HCARRY);
				z80->u_af.A = z80->u_af.A & z80->u_hl.H;
				z80->u_af.A ? CLRFLAG(z80, FL_ZERO) : SETFLAG(z80, FL_ZERO);
				break;

			case OP_AND_A_L:
				CLRFLAG(z80, FL_SUB|FL_CARRY);
				SETFLAG(z80, FL_HCARRY);
				z80->u_af.A = z80->u_af.A & z80->u_hl.L;
				z80->u_af.A ? CLRFLAG(z80, FL_ZERO) : SETFLAG(z80, FL_ZERO);
				break;

			case OP_AND_A_ADDR_HL:
				CLRFLAG(z80, FL_SUB|FL_CARRY);
				SETFLAG(z80, FL_HCARRY);
				z80->u_af.A = z80->u_af.A & z80->mem_read(z80->u_hl.HL);
				z80->u_af.A ? CLRFLAG(z80, FL_ZERO) : SETFLAG(z80, FL_ZERO);
				break;

			case OP_AND_A_N:
				CLRFLAG(z80, FL_SUB|FL_CARRY);
				SETFLAG(z80, FL_HCARRY);
				z80->u_af.A = z80->u_af.A & GETBYTE(z80);
				z80->u_af.A ? CLRFLAG(z80, FL_ZERO) : SETFLAG(z80, FL_ZERO);
				break;

			case OP_OR_A_A:
				CLRFLAG(z80, FL_SUB|FL_HCARRY|FL_CARRY);
				z80->u_af.A = z80->u_af.A | z80->u_af.A;
				z80->u_af.A ? CLRFLAG(z80, FL_ZERO) : SETFLAG(z80, FL_ZERO);
				break;

			case OP_OR_A_B:
				CLRFLAG(z80, FL_SUB|FL_HCARRY|FL_CARRY);
				z80->u_af.A = z80->u_af.A | z80->u_bc.B;
				z80->u_af.A ? CLRFLAG(z80, FL_ZERO) : SETFLAG(z80, FL_ZERO);
				break;

			case OP_OR_A_C:
				CLRFLAG(z80, FL_SUB|FL_HCARRY|FL_CARRY);
				z80->u_af.A = z80->u_af.A | z80->u_bc.C;
				z80->u_af.A ? CLRFLAG(z80, FL_ZERO) : SETFLAG(z80, FL_ZERO);
				break;

			case OP_OR_A_D:
				CLRFLAG(z80, FL_SUB|FL_HCARRY|FL_CARRY);
				z80->u_af.A = z80->u_af.A | z80->u_de.D;
				z80->u_af.A ? CLRFLAG(z80, FL_ZERO) : SETFLAG(z80, FL_ZERO);
				break;

			case OP_OR_A_E:
				CLRFLAG(z80, FL_SUB|FL_HCARRY|FL_CARRY);
				z80->u_af.A = z80->u_af.A | z80->u_de.E;
				z80->u_af.A ? CLRFLAG(z80, FL_ZERO) : SETFLAG(z80, FL_ZERO);
				break;

			case OP_OR_A_H:
				CLRFLAG(z80, FL_SUB|FL_HCARRY|FL_CARRY);
				z80->u_af.A = z80->u_af.A | z80->u_hl.H;
				z80->u_af.A ? CLRFLAG(z80, FL_ZERO) : SETFLAG(z80, FL_ZERO);
				break;

			case OP_OR_A_L:
				CLRFLAG(z80, FL_SUB|FL_HCARRY|FL_CARRY);
				z80->u_af.A = z80->u_af.A | z80->u_hl.L;
				z80->u_af.A ? CLRFLAG(z80, FL_ZERO) : SETFLAG(z80, FL_ZERO);
				break;

			case OP_OR_A_ADDR_HL:
				CLRFLAG(z80, FL_SUB|FL_HCARRY|FL_CARRY);
				z80->u_af.A = z80->u_af.A | z80->mem_read(z80->u_hl.HL);
				z80->u_af.A ? CLRFLAG(z80, FL_ZERO) : SETFLAG(z80, FL_ZERO);
				break;

			case OP_OR_A_N:
				CLRFLAG(z80, FL_SUB|FL_HCARRY|FL_CARRY);
				z80->u_af.A = z80->u_af.A | GETBYTE(z80);
				z80->u_af.A ? CLRFLAG(z80, FL_ZERO) : SETFLAG(z80, FL_ZERO);
				break;

			case OP_XOR_A_A:
				CLRFLAG(z80, FL_SUB|FL_HCARRY|FL_CARRY);
				z80->u_af.A = z80->u_af.A ^ z80->u_af.A;
				z80->u_af.A ? CLRFLAG(z80, FL_ZERO) : SETFLAG(z80, FL_ZERO);
				break;

			case OP_XOR_A_B:
				CLRFLAG(z80, FL_SUB|FL_HCARRY|FL_CARRY);
				z80->u_af.A = z80->u_af.A ^ z80->u_bc.B;
				z80->u_af.A ? CLRFLAG(z80, FL_ZERO) : SETFLAG(z80, FL_ZERO);
				break;

			case OP_XOR_A_C:
				CLRFLAG(z80, FL_SUB|FL_HCARRY|FL_CARRY);
				z80->u_af.A = z80->u_af.A ^ z80->u_bc.C;
				z80->u_af.A ? CLRFLAG(z80, FL_ZERO) : SETFLAG(z80, FL_ZERO);
				break;

			case OP_XOR_A_D:
				CLRFLAG(z80, FL_SUB|FL_HCARRY|FL_CARRY);
				z80->u_af.A = z80->u_af.A ^ z80->u_de.D;
				z80->u_af.A ? CLRFLAG(z80, FL_ZERO) : SETFLAG(z80, FL_ZERO);
				break;

			case OP_XOR_A_E:
				CLRFLAG(z80, FL_SUB|FL_HCARRY|FL_CARRY);
				z80->u_af.A = z80->u_af.A ^ z80->u_de.E;
				z80->u_af.A ? CLRFLAG(z80, FL_ZERO) : SETFLAG(z80, FL_ZERO);
				break;

			case OP_XOR_A_H:
				CLRFLAG(z80, FL_SUB|FL_HCARRY|FL_CARRY);
				z80->u_af.A = z80->u_af.A ^ z80->u_hl.H;
				z80->u_af.A ? CLRFLAG(z80, FL_ZERO) : SETFLAG(z80, FL_ZERO);
				break;

			case OP_XOR_A_L:
				CLRFLAG(z80, FL_SUB|FL_HCARRY|FL_CARRY);
				z80->u_af.A = z80->u_af.A ^ z80->u_hl.L;
				z80->u_af.A ? CLRFLAG(z80, FL_ZERO) : SETFLAG(z80, FL_ZERO);
				break;

			case OP_XOR_A_ADDR_HL:
				CLRFLAG(z80, FL_SUB|FL_HCARRY|FL_CARRY);
				z80->u_af.A = z80->u_af.A ^ z80->mem_read(z80->u_hl.HL);
				z80->u_af.A ? CLRFLAG(z80, FL_ZERO) : SETFLAG(z80, FL_ZERO);
				break;

			case OP_XOR_A_N:
				CLRFLAG(z80, FL_SUB|FL_HCARRY|FL_CARRY);
				z80->u_af.A = z80->u_af.A ^ GETBYTE(z80);
				z80->u_af.A ? CLRFLAG(z80, FL_ZERO) : SETFLAG(z80, FL_ZERO);
				break;

			case OP_CP_A:
				SETFLAG(z80, FL_SUB|FL_ZERO);
				CLRFLAG(z80, FL_HCARRY|FL_CARRY);
				break;

			case OP_CP_B:
				SETFLAG(z80, FL_SUB);
				((z80->u_bc.B & 0x0F) > (z80->u_af.A & 0x0F)) ?
					SETFLAG(z80, FL_HCARRY) : CLRFLAG(z80, FL_HCARRY);
				(z80->u_bc.B > z80->u_af.A) ? SETFLAG(z80, FL_CARRY) :
					CLRFLAG(z80, FL_CARRY);
				(z80->u_af.A == z80->u_bc.B) ? SETFLAG(z80, FL_ZERO) :
					CLRFLAG(z80, FL_ZERO);
				break;

			case OP_CP_C:
				SETFLAG(z80, FL_SUB);
				((z80->u_bc.C & 0x0F) > (z80->u_af.A & 0x0F)) ?
					SETFLAG(z80, FL_HCARRY) : CLRFLAG(z80, FL_HCARRY);
				(z80->u_bc.C > z80->u_af.A) ? SETFLAG(z80, FL_CARRY) :
					CLRFLAG(z80, FL_CARRY);
				(z80->u_af.A == z80->u_bc.C) ? SETFLAG(z80, FL_ZERO) :
					CLRFLAG(z80, FL_ZERO);
				break;

			case OP_CP_D:
				SETFLAG(z80, FL_SUB);
				((z80->u_de.D & 0x0F) > (z80->u_af.A & 0x0F)) ?
					SETFLAG(z80, FL_HCARRY) : CLRFLAG(z80, FL_HCARRY);
				(z80->u_de.D > z80->u_af.A) ? SETFLAG(z80, FL_CARRY) :
					CLRFLAG(z80, FL_CARRY);
				(z80->u_af.A == z80->u_de.D) ? SETFLAG(z80, FL_ZERO) :
					CLRFLAG(z80, FL_ZERO);
				break;

			case OP_CP_E:
				SETFLAG(z80, FL_SUB);
				((z80->u_de.E & 0x0F) > (z80->u_af.A & 0x0F)) ?
					SETFLAG(z80, FL_HCARRY) : CLRFLAG(z80, FL_HCARRY);
				(z80->u_de.E > z80->u_af.A) ? SETFLAG(z80, FL_CARRY) :
					CLRFLAG(z80, FL_CARRY);
				(z80->u_af.A == z80->u_de.E) ? SETFLAG(z80, FL_ZERO) :
					CLRFLAG(z80, FL_ZERO);
				break;

			case OP_CP_H:
				SETFLAG(z80, FL_SUB);
				((z80->u_hl.H & 0x0F) > (z80->u_af.A & 0x0F)) ?
					SETFLAG(z80, FL_HCARRY) : CLRFLAG(z80, FL_HCARRY);
				(z80->u_hl.H > z80->u_af.A) ? SETFLAG(z80, FL_CARRY) :
					CLRFLAG(z80, FL_CARRY);
				(z80->u_af.A == z80->u_hl.H) ? SETFLAG(z80, FL_ZERO) :
					CLRFLAG(z80, FL_ZERO);
				break;

			case OP_CP_L:
				SETFLAG(z80, FL_SUB);
				((z80->u_hl.L & 0x0F) > (z80->u_af.A & 0x0F)) ?
					SETFLAG(z80, FL_HCARRY) : CLRFLAG(z80, FL_HCARRY);
				(z80->u_hl.L > z80->u_af.A) ? SETFLAG(z80, FL_CARRY) :
					CLRFLAG(z80, FL_CARRY);
				(z80->u_af.A == z80->u_hl.L) ? SETFLAG(z80, FL_ZERO) :
					CLRFLAG(z80, FL_ZERO);
				break;

			case OP_CP_ADDR_HL:
				SETFLAG(z80, FL_SUB);
				op1 = z80->mem_read(z80->u_hl.HL);
				((op1 & 0x0F) > (z80->u_af.A & 0x0F)) ?
					SETFLAG(z80, FL_HCARRY) : CLRFLAG(z80, FL_HCARRY);
				(op1 > z80->u_af.A) ? SETFLAG(z80, FL_CARRY) :
					CLRFLAG(z80, FL_CARRY);
				(z80->u_af.A == op1) ? SETFLAG(z80, FL_ZERO) :
					CLRFLAG(z80, FL_ZERO);
				break;

			case OP_CP_N:
				SETFLAG(z80, FL_SUB);
				op1 = GETBYTE(z80);
				((op1 & 0x0F) > (z80->u_af.A & 0x0F)) ?
					SETFLAG(z80, FL_HCARRY) : CLRFLAG(z80, FL_HCARRY);
				(op1 > z80->u_af.A) ? SETFLAG(z80, FL_CARRY) :
					CLRFLAG(z80, FL_CARRY);
				(z80->u_af.A == op1) ? SETFLAG(z80, FL_ZERO) :
					CLRFLAG(z80, FL_ZERO);
				break;

			case OP_INC_A:
				CLRFLAG(z80, FL_SUB);
				((z80->u_af.A & 0x0F) + 1) > 0x0F ? SETFLAG(z80, FL_HCARRY) :
					CLRFLAG(z80, FL_HCARRY);
				(++z80->u_af.A) ? CLRFLAG(z80, FL_ZERO) : SETFLAG(z80, FL_ZERO);
				break;

			case OP_INC_B:
				CLRFLAG(z80, FL_SUB);
				((z80->u_bc.B & 0x0F) + 1) > 0x0F ? SETFLAG(z80, FL_HCARRY) :
					CLRFLAG(z80, FL_HCARRY);
				(++z80->u_bc.B) ? CLRFLAG(z80, FL_ZERO) : SETFLAG(z80, FL_ZERO);
				break;

			case OP_INC_C:
				CLRFLAG(z80, FL_SUB);
				((z80->u_bc.C & 0x0F) + 1) > 0x0F ? SETFLAG(z80, FL_HCARRY) :
					CLRFLAG(z80, FL_HCARRY);
				(++z80->u_bc.C) ? CLRFLAG(z80, FL_ZERO) : SETFLAG(z80, FL_ZERO);
				break;

			case OP_INC_D:
				CLRFLAG(z80, FL_SUB);
				((z80->u_de.D & 0x0F) + 1) > 0x0F ? SETFLAG(z80, FL_HCARRY) :
					CLRFLAG(z80, FL_HCARRY);
				(++z80->u_de.D) ? CLRFLAG(z80, FL_ZERO) : SETFLAG(z80, FL_ZERO);
				break;

			case OP_INC_E:
				CLRFLAG(z80, FL_SUB);
				((z80->u_de.E & 0x0F) + 1) > 0x0F ? SETFLAG(z80, FL_HCARRY) :
					CLRFLAG(z80, FL_HCARRY);
				(++z80->u_de.E) ? CLRFLAG(z80, FL_ZERO) : SETFLAG(z80, FL_ZERO);
				break;

			case OP_INC_H:
				CLRFLAG(z80, FL_SUB);
				((z80->u_hl.H & 0x0F) + 1) > 0x0F ? SETFLAG(z80, FL_HCARRY) :
					CLRFLAG(z80, FL_HCARRY);
				(++z80->u_hl.H) ? CLRFLAG(z80, FL_ZERO) : SETFLAG(z80, FL_ZERO);
				break;

			case OP_INC_L:
				CLRFLAG(z80, FL_SUB);
				((z80->u_hl.L & 0x0F) + 1) > 0x0F ? SETFLAG(z80, FL_HCARRY) :
					CLRFLAG(z80, FL_HCARRY);
				(++z80->u_hl.L) ? CLRFLAG(z80, FL_ZERO) : SETFLAG(z80, FL_ZERO);
				break;

			case OP_INC_ADDR_HL:
				CLRFLAG(z80, FL_SUB);
				op1 = z80->mem_read(z80->u_hl.HL);
				((op1 & 0x0F) + 1) > 0x0F ? SETFLAG(z80, FL_HCARRY) :
					CLRFLAG(z80, FL_HCARRY);
				(++op1) ? CLRFLAG(z80, FL_ZERO) : SETFLAG(z80, FL_ZERO);
				z80->mem_write(z80->u_hl.HL, op1);
				break;

			case OP_DEC_A:
				SETFLAG(z80, FL_SUB);
				(z80->u_af.A & 0x0F) ? CLRFLAG(z80, FL_HCARRY) :
					SETFLAG(z80, FL_HCARRY);
				z80->u_af.A--;
				z80->u_af.A ? CLRFLAG(z80, FL_ZERO) : SETFLAG(z80, FL_ZERO);
				break;

			case OP_DEC_B:
				SETFLAG(z80, FL_SUB);
				(z80->u_bc.B & 0x0F) ? CLRFLAG(z80, FL_HCARRY) :
					SETFLAG(z80, FL_HCARRY);
				z80->u_bc.B--;
				z80->u_bc.B ? CLRFLAG(z80, FL_ZERO) : SETFLAG(z80, FL_ZERO);
				break;

			case OP_DEC_C:
				SETFLAG(z80, FL_SUB);
				(z80->u_bc.C & 0x0F) ? CLRFLAG(z80, FL_HCARRY) :
					SETFLAG(z80, FL_HCARRY);
				z80->u_bc.C--;
				z80->u_bc.C ? CLRFLAG(z80, FL_ZERO) : SETFLAG(z80, FL_ZERO);
				break;

			case OP_DEC_D:
				SETFLAG(z80, FL_SUB);
				(z80->u_de.D & 0x0F) ? CLRFLAG(z80, FL_HCARRY) :
					SETFLAG(z80, FL_HCARRY);
				z80->u_de.D--;
				z80->u_de.D ? CLRFLAG(z80, FL_ZERO) : SETFLAG(z80, FL_ZERO);
				break;

			case OP_DEC_E:
				SETFLAG(z80, FL_SUB);
				(z80->u_de.E & 0x0F) ? CLRFLAG(z80, FL_HCARRY) :
					SETFLAG(z80, FL_HCARRY);
				z80->u_de.E--;
				z80->u_de.E ? CLRFLAG(z80, FL_ZERO) : SETFLAG(z80, FL_ZERO);
				break;

			case OP_DEC_H:
				SETFLAG(z80, FL_SUB);
				(z80->u_hl.H & 0x0F) ? CLRFLAG(z80, FL_HCARRY) :
					SETFLAG(z80, FL_HCARRY);
				z80->u_hl.H--;
				z80->u_hl.H ? CLRFLAG(z80, FL_ZERO) : SETFLAG(z80, FL_ZERO);
				break;

			case OP_DEC_L:
				SETFLAG(z80, FL_SUB);
				(z80->u_hl.L & 0x0F) ? CLRFLAG(z80, FL_HCARRY) :
					SETFLAG(z80, FL_HCARRY);
				z80->u_hl.L--;
				z80->u_hl.L ? CLRFLAG(z80, FL_ZERO) : SETFLAG(z80, FL_ZERO);
				break;

			case OP_DEC_ADDR_HL:
				SETFLAG(z80, FL_SUB);
				op1 = z80->mem_read(z80->u_hl.HL);
				(op1 & 0x0F) ? CLRFLAG(z80, FL_HCARRY) :
					SETFLAG(z80, FL_HCARRY);
				op1--;
				op1 ? CLRFLAG(z80, FL_ZERO) : SETFLAG(z80, FL_ZERO);
				z80->mem_write(z80->u_hl.HL, op1);
				break;

			case OP_ADD_HL_BC:
				CLRFLAG(z80, FL_SUB);
				((z80->u_hl.HL & 0xFFF) + (z80->u_bc.BC & 0xFFF)) > 0xFFF ?
					SETFLAG(z80, FL_HCARRY) : CLRFLAG(z80, FL_HCARRY);
				(t = z80->u_hl.HL + z80->u_bc.BC) & 0xFFFF0000 ?
					SETFLAG(z80, FL_CARRY) : CLRFLAG(z80, FL_CARRY);
				z80->u_hl.HL = (u16)(t & 0xFFFF);
				break;

			case OP_ADD_HL_DE:
				CLRFLAG(z80, FL_SUB);
				((z80->u_hl.HL & 0xFFF) + (z80->u_de.DE & 0xFFF)) > 0xFFF ?
					SETFLAG(z80, FL_HCARRY) : CLRFLAG(z80, FL_HCARRY);
				(t = z80->u_hl.HL + z80->u_de.DE) & 0xFFFF0000 ?
					SETFLAG(z80, FL_CARRY) : CLRFLAG(z80, FL_CARRY);
				z80->u_hl.HL = (u16)(t & 0xFFFF);
				break;

			case OP_ADD_HL_HL:
				CLRFLAG(z80, FL_SUB);
				((z80->u_hl.HL & 0xFFF) + (z80->u_hl.HL & 0xFFF)) > 0xFFF ?
					SETFLAG(z80, FL_HCARRY) : CLRFLAG(z80, FL_HCARRY);
				(t = z80->u_hl.HL + z80->u_hl.HL) & 0xFFFF0000 ?
					SETFLAG(z80, FL_CARRY) : CLRFLAG(z80, FL_CARRY);
				z80->u_hl.HL = (u16)(t & 0xFFFF);
				break;

			case OP_ADD_HL_SP:
				CLRFLAG(z80, FL_SUB);
				((z80->u_hl.HL & 0xFFF) + (z80->sp & 0xFFF)) > 0xFFF ?
					SETFLAG(z80, FL_HCARRY) : CLRFLAG(z80, FL_HCARRY);
				(t = z80->u_hl.HL + z80->sp) & 0xFFFF0000 ?
					SETFLAG(z80, FL_CARRY) : CLRFLAG(z80, FL_CARRY);
				z80->u_hl.HL = (u16)(t & 0xFFFF);
				break;

			case OP_ADD_SP_N:
				CLRFLAG(z80, FL_ZERO|FL_SUB);
				op1 = GETBYTE(z80);
				(t = z80->sp + (s8)op1) & 0xFFFF0000 ?
					SETFLAG(z80, FL_CARRY) : CLRFLAG(z80, FL_CARRY);
				((z80->sp & 0x0F) + ((s8)(op1 & 0x0F)) > 0x0F) ?
					SETFLAG(z80, FL_HCARRY) : CLRFLAG(z80, FL_HCARRY);
				z80->sp = (u16)(t & 0xFFFF);
				break;

			case OP_INC_BC:
				z80->u_bc.BC++;
				break;

			case OP_INC_DE:
				z80->u_de.DE++;
				break;

			case OP_INC_HL:
				z80->u_hl.HL++;
				break;

			case OP_INC_SP:
				z80->sp++;
				break;

			case OP_DEC_BC:
				z80->u_bc.BC--;
				break;

			case OP_DEC_DE:
				z80->u_de.DE--;
				break;

			case OP_DEC_HL:
				z80->u_hl.HL--;
				break;

			case OP_DEC_SP:
				z80->sp--;
				break;

			case OP_CB_PREFIX:
				/* fetch actual opcode */
				opc = z80->mem_read(z80->pc++);
				/* subtract cycle count for instruction */
				left = left - z80_cb_ictbl[opc];
				switch(opc) {
					case OP_SWAP_A:
						CLRFLAG(z80, FL_SUB|FL_HCARRY|FL_CARRY);
						(z80->u_af.A = (z80->u_af.A << 4) | (z80->u_af.A >> 4)) ?
							CLRFLAG(z80, FL_ZERO) : SETFLAG(z80, FL_ZERO);
						break;

					case OP_SWAP_B:
						CLRFLAG(z80, FL_SUB|FL_HCARRY|FL_CARRY);
						(z80->u_bc.B = (z80->u_bc.B << 4) | (z80->u_bc.B >> 4)) ?
							CLRFLAG(z80, FL_ZERO) : SETFLAG(z80, FL_ZERO);
						break;

					case OP_SWAP_C:
						CLRFLAG(z80, FL_SUB|FL_HCARRY|FL_CARRY);
						(z80->u_bc.C = (z80->u_bc.C << 4) | (z80->u_bc.C >> 4)) ?
							CLRFLAG(z80, FL_ZERO) : SETFLAG(z80, FL_ZERO);
						break;

					case OP_SWAP_D:
						CLRFLAG(z80, FL_SUB|FL_HCARRY|FL_CARRY);
						(z80->u_de.D = (z80->u_de.D << 4) | (z80->u_de.D >> 4)) ?
							CLRFLAG(z80, FL_ZERO) : SETFLAG(z80, FL_ZERO);
						break;

					case OP_SWAP_E:
						CLRFLAG(z80, FL_SUB|FL_HCARRY|FL_CARRY);
						(z80->u_de.E = (z80->u_de.E << 4) | (z80->u_de.E >> 4)) ?
							CLRFLAG(z80, FL_ZERO) : SETFLAG(z80, FL_ZERO);
						break;

					case OP_SWAP_H:
						CLRFLAG(z80, FL_SUB|FL_HCARRY|FL_CARRY);
						(z80->u_hl.H = (z80->u_hl.H << 4) | (z80->u_hl.H >> 4)) ?
							CLRFLAG(z80, FL_ZERO) : SETFLAG(z80, FL_ZERO);
						break;

					case OP_SWAP_L:
						CLRFLAG(z80, FL_SUB|FL_HCARRY|FL_CARRY);
						(z80->u_hl.L = (z80->u_hl.L << 4) | (z80->u_hl.L >> 4)) ?
							CLRFLAG(z80, FL_ZERO) : SETFLAG(z80, FL_ZERO);
						break;

					case OP_SWAP_ADDR_HL:
						CLRFLAG(z80, FL_SUB|FL_HCARRY|FL_CARRY);
						op1 = z80->mem_read(z80->u_hl.HL);
						(op1 = (op1 << 4) | (op1 >> 4)) ?
							CLRFLAG(z80, FL_ZERO) : SETFLAG(z80, FL_ZERO);
						z80->mem_write(z80->u_hl.HL, op1);
						break;

					case OP_RLC_A:
						CLRFLAG(z80, FL_SUB|FL_HCARRY);
						(z80->u_af.A >> 7) ? SETFLAG(z80, FL_CARRY) : CLRFLAG(z80, FL_CARRY);
						(z80->u_af.A = (z80->u_af.A << 1) | GETFLAG(z80, FL_CARRY)) ?
							CLRFLAG(z80, FL_ZERO) : SETFLAG(z80, FL_ZERO);
						break;

					case OP_RLC_B:
						CLRFLAG(z80, FL_SUB|FL_HCARRY);
						(z80->u_bc.B >> 7) ? SETFLAG(z80, FL_CARRY) : CLRFLAG(z80, FL_CARRY);
						(z80->u_bc.B = (z80->u_bc.B << 1) | GETFLAG(z80, FL_CARRY)) ?
							CLRFLAG(z80, FL_ZERO) : SETFLAG(z80, FL_ZERO);
						break;

					case OP_RLC_C:
						CLRFLAG(z80, FL_SUB|FL_HCARRY);
						(z80->u_bc.C >> 7) ? SETFLAG(z80, FL_CARRY) : CLRFLAG(z80, FL_CARRY);
						(z80->u_bc.C = (z80->u_bc.C << 1) | GETFLAG(z80, FL_CARRY)) ?
							CLRFLAG(z80, FL_ZERO) : SETFLAG(z80, FL_ZERO);
						break;

					case OP_RLC_D:
						CLRFLAG(z80, FL_SUB|FL_HCARRY);
						(z80->u_de.D >> 7) ? SETFLAG(z80, FL_CARRY) : CLRFLAG(z80, FL_CARRY);
						(z80->u_de.D = (z80->u_de.D << 1) | GETFLAG(z80, FL_CARRY)) ?
							CLRFLAG(z80, FL_ZERO) : SETFLAG(z80, FL_ZERO);
						break;

					case OP_RLC_E:
						CLRFLAG(z80, FL_SUB|FL_HCARRY);
						(z80->u_de.E >> 7) ? SETFLAG(z80, FL_CARRY) : CLRFLAG(z80, FL_CARRY);
						(z80->u_de.E = (z80->u_de.E << 1) | GETFLAG(z80, FL_CARRY)) ?
							CLRFLAG(z80, FL_ZERO) : SETFLAG(z80, FL_ZERO);
						break;

					case OP_RLC_H:
						CLRFLAG(z80, FL_SUB|FL_HCARRY);
						(z80->u_hl.H >> 7) ? SETFLAG(z80, FL_CARRY) : CLRFLAG(z80, FL_CARRY);
						(z80->u_hl.H = (z80->u_hl.H << 1) | GETFLAG(z80, FL_CARRY)) ?
							CLRFLAG(z80, FL_ZERO) : SETFLAG(z80, FL_ZERO);
						break;

					case OP_RLC_L:
						CLRFLAG(z80, FL_SUB|FL_HCARRY);
						(z80->u_hl.L >> 7) ? SETFLAG(z80, FL_CARRY) : CLRFLAG(z80, FL_CARRY);
						(z80->u_hl.L = (z80->u_hl.L << 1) | GETFLAG(z80, FL_CARRY)) ?
							CLRFLAG(z80, FL_ZERO) : SETFLAG(z80, FL_ZERO);
						break;

					case OP_RLC_ADDR_HL:
						CLRFLAG(z80, FL_SUB|FL_HCARRY);
						op1 = z80->mem_read(z80->u_hl.HL);
						(op1 >> 7) ? SETFLAG(z80, FL_CARRY) : CLRFLAG(z80, FL_CARRY);
						(op1 = (op1 << 1) | GETFLAG(z80, FL_CARRY)) ?
							CLRFLAG(z80, FL_ZERO) : SETFLAG(z80, FL_ZERO);
						z80->mem_write(z80->u_hl.HL, op1);
						break;

					case OP_RL_A:
						CLRFLAG(z80, FL_SUB|FL_HCARRY);
						op1 = z80->u_af.A >> 7;
						(z80->u_af.A = (z80->u_af.A << 1) | GETFLAG(z80, FL_CARRY)) ?
						CLRFLAG(z80, FL_ZERO) : SETFLAG(z80, FL_ZERO);
						op1 ? SETFLAG(z80, FL_CARRY) : CLRFLAG(z80, FL_CARRY);
						break;

					case OP_RL_B:
						CLRFLAG(z80, FL_SUB|FL_HCARRY);
						op1 = z80->u_bc.B >> 7;
						(z80->u_bc.B = (z80->u_bc.B << 1) | GETFLAG(z80, FL_CARRY)) ?
						CLRFLAG(z80, FL_ZERO) : SETFLAG(z80, FL_ZERO);
						op1 ? SETFLAG(z80, FL_CARRY) : CLRFLAG(z80, FL_CARRY);
						break;

					case OP_RL_C:
						CLRFLAG(z80, FL_SUB|FL_HCARRY);
						op1 = z80->u_bc.C >> 7;
						(z80->u_bc.C = (z80->u_bc.C << 1) | GETFLAG(z80, FL_CARRY)) ?
						CLRFLAG(z80, FL_ZERO) : SETFLAG(z80, FL_ZERO);
						op1 ? SETFLAG(z80, FL_CARRY) : CLRFLAG(z80, FL_CARRY);
						break;

					case OP_RL_D:
						CLRFLAG(z80, FL_SUB|FL_HCARRY);
						op1 = z80->u_de.D >> 7;
						(z80->u_de.D = (z80->u_de.D << 1) | GETFLAG(z80, FL_CARRY)) ?
						CLRFLAG(z80, FL_ZERO) : SETFLAG(z80, FL_ZERO);
						op1 ? SETFLAG(z80, FL_CARRY) : CLRFLAG(z80, FL_CARRY);
						break;

					case OP_RL_E:
						CLRFLAG(z80, FL_SUB|FL_HCARRY);
						op1 = z80->u_de.E >> 7;
						(z80->u_de.E = (z80->u_de.E << 1) | GETFLAG(z80, FL_CARRY)) ?
						CLRFLAG(z80, FL_ZERO) : SETFLAG(z80, FL_ZERO);
						op1 ? SETFLAG(z80, FL_CARRY) : CLRFLAG(z80, FL_CARRY);
						break;

					case OP_RL_H:
						CLRFLAG(z80, FL_SUB|FL_HCARRY);
						op1 = z80->u_hl.H >> 7;
						(z80->u_hl.H = (z80->u_hl.H << 1) | GETFLAG(z80, FL_CARRY)) ?
						CLRFLAG(z80, FL_ZERO) : SETFLAG(z80, FL_ZERO);
						op1 ? SETFLAG(z80, FL_CARRY) : CLRFLAG(z80, FL_CARRY);
						break;

					case OP_RL_L:
						CLRFLAG(z80, FL_SUB|FL_HCARRY);
						op1 = z80->u_hl.L >> 7;
						(z80->u_hl.L = (z80->u_hl.L << 1) | GETFLAG(z80, FL_CARRY)) ?
						CLRFLAG(z80, FL_ZERO) : SETFLAG(z80, FL_ZERO);
						op1 ? SETFLAG(z80, FL_CARRY) : CLRFLAG(z80, FL_CARRY);
						break;

					case OP_RL_ADDR_HL:
						CLRFLAG(z80, FL_SUB|FL_HCARRY);
						opc = z80->mem_read(z80->u_hl.HL);
						op1 = opc >> 7;
						(opc = (opc << 1) | GETFLAG(z80, FL_CARRY)) ?
						CLRFLAG(z80, FL_ZERO) : SETFLAG(z80, FL_ZERO);
						op1 ? SETFLAG(z80, FL_CARRY) : CLRFLAG(z80, FL_CARRY);
						z80->mem_write(z80->u_hl.HL, opc);
						break;

					case OP_RRC_A:
						CLRFLAG(z80, FL_SUB|FL_HCARRY);
						(z80->u_af.A & 0x01) ? SETFLAG(z80, FL_CARRY) : CLRFLAG(z80, FL_CARRY);
						(z80->u_af.A = (z80->u_af.A >> 1) | (GETFLAG(z80, FL_CARRY) << 7)) ?
							CLRFLAG(z80, FL_ZERO) : SETFLAG(z80, FL_ZERO);
						break;

					case OP_RRC_B:
						CLRFLAG(z80, FL_SUB|FL_HCARRY);
						(z80->u_bc.B & 0x01) ? SETFLAG(z80, FL_CARRY) : CLRFLAG(z80, FL_CARRY);
						(z80->u_bc.B = (z80->u_bc.B >> 1) | (GETFLAG(z80, FL_CARRY) << 7)) ?
							CLRFLAG(z80, FL_ZERO) : SETFLAG(z80, FL_ZERO);
						break;

					case OP_RRC_C:
						CLRFLAG(z80, FL_SUB|FL_HCARRY);
						(z80->u_bc.C & 0x01) ? SETFLAG(z80, FL_CARRY) : CLRFLAG(z80, FL_CARRY);
						(z80->u_bc.C = (z80->u_bc.C >> 1) | (GETFLAG(z80, FL_CARRY) << 7)) ?
							CLRFLAG(z80, FL_ZERO) : SETFLAG(z80, FL_ZERO);
						break;

					case OP_RRC_D:
						CLRFLAG(z80, FL_SUB|FL_HCARRY);
						(z80->u_de.D & 0x01) ? SETFLAG(z80, FL_CARRY) : CLRFLAG(z80, FL_CARRY);
						(z80->u_de.D = (z80->u_de.D >> 1) | (GETFLAG(z80, FL_CARRY) << 7)) ?
							CLRFLAG(z80, FL_ZERO) : SETFLAG(z80, FL_ZERO);
						break;

					case OP_RRC_E:
						CLRFLAG(z80, FL_SUB|FL_HCARRY);
						(z80->u_de.E & 0x01) ? SETFLAG(z80, FL_CARRY) : CLRFLAG(z80, FL_CARRY);
						(z80->u_de.E = (z80->u_de.E >> 1) | (GETFLAG(z80, FL_CARRY) << 7)) ?
							CLRFLAG(z80, FL_ZERO) : SETFLAG(z80, FL_ZERO);
						break;

					case OP_RRC_H:
						CLRFLAG(z80, FL_SUB|FL_HCARRY);
						(z80->u_hl.H & 0x01) ? SETFLAG(z80, FL_CARRY) : CLRFLAG(z80, FL_CARRY);
						(z80->u_hl.H = (z80->u_hl.H >> 1) | (GETFLAG(z80, FL_CARRY) << 7)) ?
							CLRFLAG(z80, FL_ZERO) : SETFLAG(z80, FL_ZERO);
						break;

					case OP_RRC_L:
						CLRFLAG(z80, FL_SUB|FL_HCARRY);
						(z80->u_hl.L & 0x01) ? SETFLAG(z80, FL_CARRY) : CLRFLAG(z80, FL_CARRY);
						(z80->u_hl.L = (z80->u_hl.L >> 1) | (GETFLAG(z80, FL_CARRY) << 7)) ?
							CLRFLAG(z80, FL_ZERO) : SETFLAG(z80, FL_ZERO);
						break;

					case OP_RRC_ADDR_HL:
						CLRFLAG(z80, FL_SUB|FL_HCARRY);
						op1 = z80->mem_read(z80->u_hl.HL);
						(op1 & 0x01) ? SETFLAG(z80, FL_CARRY) : CLRFLAG(z80, FL_CARRY);
						(op1 = (op1 >> 1) | (GETFLAG(z80, FL_CARRY) << 7)) ?
							CLRFLAG(z80, FL_ZERO) : SETFLAG(z80, FL_ZERO);
						z80->mem_write(z80->u_hl.HL, op1);
						break;

					case OP_RR_A:
						CLRFLAG(z80, FL_SUB|FL_HCARRY);
						op1 = (z80->u_af.A & 0x01);
						(z80->u_af.A = (z80->u_af.A >> 1) | (GETFLAG(z80, FL_CARRY) << 7)) ?
							CLRFLAG(z80, FL_ZERO) : SETFLAG(z80, FL_ZERO);
						op1 ? SETFLAG(z80, FL_CARRY) : CLRFLAG(z80, FL_CARRY);
						break;

					case OP_RR_B:
						CLRFLAG(z80, FL_SUB|FL_HCARRY);
						op1 = (z80->u_bc.B & 0x01);
						(z80->u_bc.B = (z80->u_bc.B >> 1) | (GETFLAG(z80, FL_CARRY) << 7)) ?
							CLRFLAG(z80, FL_ZERO) : SETFLAG(z80, FL_ZERO);
						op1 ? SETFLAG(z80, FL_CARRY) : CLRFLAG(z80, FL_CARRY);
						break;

					case OP_RR_C:
						CLRFLAG(z80, FL_SUB|FL_HCARRY);
						op1 = (z80->u_bc.C & 0x01);
						(z80->u_bc.C = (z80->u_bc.C >> 1) | (GETFLAG(z80, FL_CARRY) << 7)) ?
							CLRFLAG(z80, FL_ZERO) : SETFLAG(z80, FL_ZERO);
						op1 ? SETFLAG(z80, FL_CARRY) : CLRFLAG(z80, FL_CARRY);
						break;

					case OP_RR_D:
						CLRFLAG(z80, FL_SUB|FL_HCARRY);
						op1 = (z80->u_de.D & 0x01);
						(z80->u_de.D = (z80->u_de.D >> 1) | (GETFLAG(z80, FL_CARRY) << 7)) ?
							CLRFLAG(z80, FL_ZERO) : SETFLAG(z80, FL_ZERO);
						op1 ? SETFLAG(z80, FL_CARRY) : CLRFLAG(z80, FL_CARRY);
						break;

					case OP_RR_E:
						CLRFLAG(z80, FL_SUB|FL_HCARRY);
						op1 = (z80->u_de.E & 0x01);
						(z80->u_de.E = (z80->u_de.E >> 1) | (GETFLAG(z80, FL_CARRY) << 7)) ?
							CLRFLAG(z80, FL_ZERO) : SETFLAG(z80, FL_ZERO);
						op1 ? SETFLAG(z80, FL_CARRY) : CLRFLAG(z80, FL_CARRY);
						break;

					case OP_RR_H:
						CLRFLAG(z80, FL_SUB|FL_HCARRY);
						op1 = (z80->u_hl.H & 0x01);
						(z80->u_hl.H = (z80->u_hl.H >> 1) | (GETFLAG(z80, FL_CARRY) << 7)) ?
							CLRFLAG(z80, FL_ZERO) : SETFLAG(z80, FL_ZERO);
						op1 ? SETFLAG(z80, FL_CARRY) : CLRFLAG(z80, FL_CARRY);
						break;

					case OP_RR_L:
						CLRFLAG(z80, FL_SUB|FL_HCARRY);
						op1 = (z80->u_hl.L & 0x01);
						(z80->u_hl.L = (z80->u_hl.L >> 1) | (GETFLAG(z80, FL_CARRY) << 7)) ?
							CLRFLAG(z80, FL_ZERO) : SETFLAG(z80, FL_ZERO);
						op1 ? SETFLAG(z80, FL_CARRY) : CLRFLAG(z80, FL_CARRY);
						break;

					case OP_RR_ADDR_HL:
						CLRFLAG(z80, FL_SUB|FL_HCARRY);
						opc = z80->mem_read(z80->u_hl.HL);
						op1 = (opc & 0x01);
						(opc = (opc >> 1) | (GETFLAG(z80, FL_CARRY) << 7)) ?
							CLRFLAG(z80, FL_ZERO) : SETFLAG(z80, FL_ZERO);
						op1 ? SETFLAG(z80, FL_CARRY) : CLRFLAG(z80, FL_CARRY);
						z80->mem_write(z80->u_hl.HL, opc);
						break;

					case OP_SLA_A:
						CLRFLAG(z80, FL_SUB|FL_HCARRY);
						(z80->u_af.A >> 7) ? SETFLAG(z80, FL_CARRY) : CLRFLAG(z80, FL_CARRY);
						(z80->u_af.A = z80->u_af.A << 1) ? CLRFLAG(z80, FL_ZERO) :
							SETFLAG(z80, FL_ZERO);
						break;

					case OP_SLA_B:
						CLRFLAG(z80, FL_SUB|FL_HCARRY);
						(z80->u_bc.B >> 7) ? SETFLAG(z80, FL_CARRY) : CLRFLAG(z80, FL_CARRY);
						(z80->u_bc.B = z80->u_bc.B << 1) ? CLRFLAG(z80, FL_ZERO) :
							SETFLAG(z80, FL_ZERO);
						break;

					case OP_SLA_C:
						CLRFLAG(z80, FL_SUB|FL_HCARRY);
						(z80->u_bc.C >> 7) ? SETFLAG(z80, FL_CARRY) : CLRFLAG(z80, FL_CARRY);
						(z80->u_bc.C = z80->u_bc.C << 1) ? CLRFLAG(z80, FL_ZERO) :
							SETFLAG(z80, FL_ZERO);
						break;

					case OP_SLA_D:
						CLRFLAG(z80, FL_SUB|FL_HCARRY);
						(z80->u_de.D >> 7) ? SETFLAG(z80, FL_CARRY) : CLRFLAG(z80, FL_CARRY);
						(z80->u_de.D = z80->u_de.D << 1) ? CLRFLAG(z80, FL_ZERO) :
							SETFLAG(z80, FL_ZERO);
						break;

					case OP_SLA_E:
						CLRFLAG(z80, FL_SUB|FL_HCARRY);
						(z80->u_de.E >> 7) ? SETFLAG(z80, FL_CARRY) : CLRFLAG(z80, FL_CARRY);
						(z80->u_de.E = z80->u_de.E << 1) ? CLRFLAG(z80, FL_ZERO) :
							SETFLAG(z80, FL_ZERO);
						break;

					case OP_SLA_H:
						CLRFLAG(z80, FL_SUB|FL_HCARRY);
						(z80->u_hl.H >> 7) ? SETFLAG(z80, FL_CARRY) : CLRFLAG(z80, FL_CARRY);
						(z80->u_hl.H = z80->u_hl.H << 1) ? CLRFLAG(z80, FL_ZERO) :
							SETFLAG(z80, FL_ZERO);
						break;

					case OP_SLA_L:
						CLRFLAG(z80, FL_SUB|FL_HCARRY);
						(z80->u_hl.L >> 7) ? SETFLAG(z80, FL_CARRY) : CLRFLAG(z80, FL_CARRY);
						(z80->u_hl.L = z80->u_hl.L << 1) ? CLRFLAG(z80, FL_ZERO) :
							SETFLAG(z80, FL_ZERO);
						break;

					case OP_SLA_ADDR_HL:
						CLRFLAG(z80, FL_SUB|FL_HCARRY);
						op1 = z80->mem_read(z80->u_hl.HL);
						(op1 >> 7) ? SETFLAG(z80, FL_CARRY) : CLRFLAG(z80, FL_CARRY);
						(op1 = op1 << 1) ? CLRFLAG(z80, FL_ZERO) : SETFLAG(z80, FL_ZERO);
						z80->mem_write(z80->u_hl.HL, op1);
						break;

					case OP_SRA_A:
						CLRFLAG(z80, FL_SUB|FL_HCARRY);
						(z80->u_af.A & 0x01) ? SETFLAG(z80, FL_CARRY) : CLRFLAG(z80, FL_CARRY);
						op1 = z80->u_af.A & 0x80;
						(z80->u_af.A = (z80->u_af.A >> 1) | op1) ? CLRFLAG(z80, FL_ZERO) :
							SETFLAG(z80, FL_ZERO);
						break;

					case OP_SRA_B:
						CLRFLAG(z80, FL_SUB|FL_HCARRY);
						(z80->u_bc.B & 0x01) ? SETFLAG(z80, FL_CARRY) : CLRFLAG(z80, FL_CARRY);
						op1 = z80->u_bc.B & 0x80;
						(z80->u_bc.B = (z80->u_bc.B >> 1) | op1) ? CLRFLAG(z80, FL_ZERO) :
							SETFLAG(z80, FL_ZERO);
						break;

					case OP_SRA_C:
						CLRFLAG(z80, FL_SUB|FL_HCARRY);
						(z80->u_bc.C & 0x01) ? SETFLAG(z80, FL_CARRY) : CLRFLAG(z80, FL_CARRY);
						op1 = z80->u_bc.C & 0x80;
						(z80->u_bc.C = (z80->u_bc.C >> 1) | op1) ? CLRFLAG(z80, FL_ZERO) :
							SETFLAG(z80, FL_ZERO);
						break;

					case OP_SRA_D:
						CLRFLAG(z80, FL_SUB|FL_HCARRY);
						(z80->u_de.D & 0x01) ? SETFLAG(z80, FL_CARRY) : CLRFLAG(z80, FL_CARRY);
						op1 = z80->u_de.D & 0x80;
						(z80->u_de.D = (z80->u_de.D >> 1) | op1) ? CLRFLAG(z80, FL_ZERO) :
							SETFLAG(z80, FL_ZERO);
						break;

					case OP_SRA_E:
						CLRFLAG(z80, FL_SUB|FL_HCARRY);
						(z80->u_de.E & 0x01) ? SETFLAG(z80, FL_CARRY) : CLRFLAG(z80, FL_CARRY);
						op1 = z80->u_de.E & 0x80;
						(z80->u_de.E = (z80->u_de.E >> 1) | op1) ? CLRFLAG(z80, FL_ZERO) :
							SETFLAG(z80, FL_ZERO);
						break;

					case OP_SRA_H:
						CLRFLAG(z80, FL_SUB|FL_HCARRY);
						(z80->u_hl.H & 0x01) ? SETFLAG(z80, FL_CARRY) : CLRFLAG(z80, FL_CARRY);
						op1 = z80->u_hl.H & 0x80;
						(z80->u_hl.H = (z80->u_hl.H >> 1) | op1) ? CLRFLAG(z80, FL_ZERO) :
							SETFLAG(z80, FL_ZERO);
						break;

					case OP_SRA_L:
						CLRFLAG(z80, FL_SUB|FL_HCARRY);
						(z80->u_hl.L & 0x01) ? SETFLAG(z80, FL_CARRY) : CLRFLAG(z80, FL_CARRY);
						op1 = z80->u_hl.L & 0x80;
						(z80->u_hl.L = (z80->u_hl.L >> 1) | op1) ? CLRFLAG(z80, FL_ZERO) :
							SETFLAG(z80, FL_ZERO);
						break;

					case OP_SRA_ADDR_HL:
						CLRFLAG(z80, FL_SUB|FL_HCARRY);
						opc = z80->mem_read(z80->u_hl.HL);
						(opc & 0x01) ? SETFLAG(z80, FL_CARRY) : CLRFLAG(z80, FL_CARRY);
						op1 = opc & 0x80;
						(opc = (opc >> 1) | op1) ? CLRFLAG(z80, FL_ZERO) :
							SETFLAG(z80, FL_ZERO);
						z80->mem_write(z80->u_hl.HL, opc);
						break;

					case OP_SRL_A:
						CLRFLAG(z80, FL_SUB|FL_HCARRY);
						(z80->u_af.A & 0x01) ? SETFLAG(z80, FL_CARRY) : CLRFLAG(z80, FL_CARRY);
						(z80->u_af.A = z80->u_af.A >> 1) ? CLRFLAG(z80, FL_ZERO) :
							SETFLAG(z80, FL_ZERO);
						break;

					case OP_SRL_B:
						CLRFLAG(z80, FL_SUB|FL_HCARRY);
						(z80->u_bc.B & 0x01) ? SETFLAG(z80, FL_CARRY) : CLRFLAG(z80, FL_CARRY);
						(z80->u_bc.B = z80->u_bc.B >> 1) ? CLRFLAG(z80, FL_ZERO) :
							SETFLAG(z80, FL_ZERO);
						break;

					case OP_SRL_C:
						CLRFLAG(z80, FL_SUB|FL_HCARRY);
						(z80->u_bc.C & 0x01) ? SETFLAG(z80, FL_CARRY) : CLRFLAG(z80, FL_CARRY);
						(z80->u_bc.C = z80->u_bc.C >> 1) ? CLRFLAG(z80, FL_ZERO) :
							SETFLAG(z80, FL_ZERO);
						break;

					case OP_SRL_D:
						CLRFLAG(z80, FL_SUB|FL_HCARRY);
						(z80->u_de.D & 0x01) ? SETFLAG(z80, FL_CARRY) : CLRFLAG(z80, FL_CARRY);
						(z80->u_de.D = z80->u_de.D >> 1) ? CLRFLAG(z80, FL_ZERO) :
							SETFLAG(z80, FL_ZERO);
						break;

					case OP_SRL_E:
						CLRFLAG(z80, FL_SUB|FL_HCARRY);
						(z80->u_de.E & 0x01) ? SETFLAG(z80, FL_CARRY) : CLRFLAG(z80, FL_CARRY);
						(z80->u_de.E = z80->u_de.E >> 1) ? CLRFLAG(z80, FL_ZERO) :
							SETFLAG(z80, FL_ZERO);
						break;

					case OP_SRL_H:
						CLRFLAG(z80, FL_SUB|FL_HCARRY);
						(z80->u_hl.H & 0x01) ? SETFLAG(z80, FL_CARRY) : CLRFLAG(z80, FL_CARRY);
						(z80->u_hl.H = z80->u_hl.H >> 1) ? CLRFLAG(z80, FL_ZERO) :
							SETFLAG(z80, FL_ZERO);
						break;

					case OP_SRL_L:
						CLRFLAG(z80, FL_SUB|FL_HCARRY);
						(z80->u_hl.L & 0x01) ? SETFLAG(z80, FL_CARRY) : CLRFLAG(z80, FL_CARRY);
						(z80->u_hl.L = z80->u_hl.L >> 1) ? CLRFLAG(z80, FL_ZERO) :
							SETFLAG(z80, FL_ZERO);
						break;

					case OP_SRL_ADDR_HL:
						CLRFLAG(z80, FL_SUB|FL_HCARRY);
						op1 = z80->mem_read(z80->u_hl.HL);
						(op1 & 0x01) ? SETFLAG(z80, FL_CARRY) : CLRFLAG(z80, FL_CARRY);
						(op1 = op1 >> 1) ? CLRFLAG(z80, FL_ZERO) : SETFLAG(z80, FL_ZERO);
						z80->mem_write(z80->u_hl.HL, op1);
						break;

					case OP_BIT_A_0:
					case OP_BIT_A_1:
					case OP_BIT_A_2:
					case OP_BIT_A_3:
					case OP_BIT_A_4:
					case OP_BIT_A_5:
					case OP_BIT_A_6:
					case OP_BIT_A_7:
						CLRFLAG(z80, FL_SUB);
						SETFLAG(z80, FL_HCARRY);
						op1 = (opc >> 3) & 0x07;
						(z80->u_af.A & (1 << op1)) ? CLRFLAG(z80, FL_ZERO) : SETFLAG(z80, FL_ZERO);
						break;

					case OP_BIT_B_0:
					case OP_BIT_B_1:
					case OP_BIT_B_2:
					case OP_BIT_B_3:
					case OP_BIT_B_4:
					case OP_BIT_B_5:
					case OP_BIT_B_6:
					case OP_BIT_B_7:
						CLRFLAG(z80, FL_SUB);
						SETFLAG(z80, FL_HCARRY);
						op1 = (opc >> 3) & 0x07;
						(z80->u_bc.B & (1 << op1)) ? CLRFLAG(z80, FL_ZERO) : SETFLAG(z80, FL_ZERO);
						break;

					case OP_BIT_C_0:
					case OP_BIT_C_1:
					case OP_BIT_C_2:
					case OP_BIT_C_3:
					case OP_BIT_C_4:
					case OP_BIT_C_5:
					case OP_BIT_C_6:
					case OP_BIT_C_7:
						CLRFLAG(z80, FL_SUB);
						SETFLAG(z80, FL_HCARRY);
						op1 = (opc >> 3) & 0x07;
						(z80->u_bc.C & (1 << op1)) ? CLRFLAG(z80, FL_ZERO) : SETFLAG(z80, FL_ZERO);
						break;

					case OP_BIT_D_0:
					case OP_BIT_D_1:
					case OP_BIT_D_2:
					case OP_BIT_D_3:
					case OP_BIT_D_4:
					case OP_BIT_D_5:
					case OP_BIT_D_6:
					case OP_BIT_D_7:
						CLRFLAG(z80, FL_SUB);
						SETFLAG(z80, FL_HCARRY);
						op1 = (opc >> 3) & 0x07;
						(z80->u_de.D & (1 << op1)) ? CLRFLAG(z80, FL_ZERO) : SETFLAG(z80, FL_ZERO);
						break;

					case OP_BIT_E_0:
					case OP_BIT_E_1:
					case OP_BIT_E_2:
					case OP_BIT_E_3:
					case OP_BIT_E_4:
					case OP_BIT_E_5:
					case OP_BIT_E_6:
					case OP_BIT_E_7:
						CLRFLAG(z80, FL_SUB);
						SETFLAG(z80, FL_HCARRY);
						op1 = (opc >> 3) & 0x07;
						(z80->u_de.E & (1 << op1)) ? CLRFLAG(z80, FL_ZERO) : SETFLAG(z80, FL_ZERO);
						break;

					case OP_BIT_H_0:
					case OP_BIT_H_1:
					case OP_BIT_H_2:
					case OP_BIT_H_3:
					case OP_BIT_H_4:
					case OP_BIT_H_5:
					case OP_BIT_H_6:
					case OP_BIT_H_7:
						CLRFLAG(z80, FL_SUB);
						SETFLAG(z80, FL_HCARRY);
						op1 = (opc >> 3) & 0x07;
						(z80->u_hl.H & (1 << op1)) ? CLRFLAG(z80, FL_ZERO) : SETFLAG(z80, FL_ZERO);
						break;

					case OP_BIT_L_0:
					case OP_BIT_L_1:
					case OP_BIT_L_2:
					case OP_BIT_L_3:
					case OP_BIT_L_4:
					case OP_BIT_L_5:
					case OP_BIT_L_6:
					case OP_BIT_L_7:
						CLRFLAG(z80, FL_SUB);
						SETFLAG(z80, FL_HCARRY);
						op1 = (opc >> 3) & 0x07;
						(z80->u_hl.L & (1 << op1)) ? CLRFLAG(z80, FL_ZERO) : SETFLAG(z80, FL_ZERO);
						break;

					case OP_BIT_ADDR_HL_0:
					case OP_BIT_ADDR_HL_1:
					case OP_BIT_ADDR_HL_2:
					case OP_BIT_ADDR_HL_3:
					case OP_BIT_ADDR_HL_4:
					case OP_BIT_ADDR_HL_5:
					case OP_BIT_ADDR_HL_6:
					case OP_BIT_ADDR_HL_7:
						CLRFLAG(z80, FL_SUB);
						SETFLAG(z80, FL_HCARRY);
						op1 = (opc >> 3) & 0x07;
						(z80->mem_read(z80->u_hl.HL) & (1 << op1)) ? CLRFLAG(z80, FL_ZERO) :
							SETFLAG(z80, FL_ZERO);
						break;

					case OP_SET_A_0:
					case OP_SET_A_1:
					case OP_SET_A_2:
					case OP_SET_A_3:
					case OP_SET_A_4:
					case OP_SET_A_5:
					case OP_SET_A_6:
					case OP_SET_A_7:
						op1 = (opc >> 3) & 0x07;
						z80->u_af.A = z80->u_af.A | (1 << op1);
						break;

					case OP_SET_B_0:
					case OP_SET_B_1:
					case OP_SET_B_2:
					case OP_SET_B_3:
					case OP_SET_B_4:
					case OP_SET_B_5:
					case OP_SET_B_6:
					case OP_SET_B_7:
						op1 = (opc >> 3) & 0x07;
						z80->u_bc.B = z80->u_bc.B | (1 << op1);
						break;

					case OP_SET_C_0:
					case OP_SET_C_1:
					case OP_SET_C_2:
					case OP_SET_C_3:
					case OP_SET_C_4:
					case OP_SET_C_5:
					case OP_SET_C_6:
					case OP_SET_C_7:
						op1 = (opc >> 3) & 0x07;
						z80->u_bc.C = z80->u_bc.C | (1 << op1);
						break;

					case OP_SET_D_0:
					case OP_SET_D_1:
					case OP_SET_D_2:
					case OP_SET_D_3:
					case OP_SET_D_4:
					case OP_SET_D_5:
					case OP_SET_D_6:
					case OP_SET_D_7:
						op1 = (opc >> 3) & 0x07;
						z80->u_de.D = z80->u_de.D | (1 << op1);
						break;

					case OP_SET_E_0:
					case OP_SET_E_1:
					case OP_SET_E_2:
					case OP_SET_E_3:
					case OP_SET_E_4:
					case OP_SET_E_5:
					case OP_SET_E_6:
					case OP_SET_E_7:
						op1 = (opc >> 3) & 0x07;
						z80->u_de.E = z80->u_de.E | (1 << op1);
						break;

					case OP_SET_H_0:
					case OP_SET_H_1:
					case OP_SET_H_2:
					case OP_SET_H_3:
					case OP_SET_H_4:
					case OP_SET_H_5:
					case OP_SET_H_6:
					case OP_SET_H_7:
						op1 = (opc >> 3) & 0x07;
						z80->u_hl.H = z80->u_hl.H | (1 << op1);
						break;

					case OP_SET_L_0:
					case OP_SET_L_1:
					case OP_SET_L_2:
					case OP_SET_L_3:
					case OP_SET_L_4:
					case OP_SET_L_5:
					case OP_SET_L_6:
					case OP_SET_L_7:
						op1 = (opc >> 3) & 0x07;
						z80->u_hl.L = z80->u_hl.L | (1 << op1);
						break;

					case OP_SET_ADDR_HL_0:
					case OP_SET_ADDR_HL_1:
					case OP_SET_ADDR_HL_2:
					case OP_SET_ADDR_HL_3:
					case OP_SET_ADDR_HL_4:
					case OP_SET_ADDR_HL_5:
					case OP_SET_ADDR_HL_6:
					case OP_SET_ADDR_HL_7:
						op1 = (opc >> 3) & 0x07;
						opc = z80->mem_read(z80->u_hl.HL);
						opc = opc | (1 << op1);
						z80->mem_write(z80->u_hl.HL, opc);
						break;

					case OP_RES_A_0:
					case OP_RES_A_1:
					case OP_RES_A_2:
					case OP_RES_A_3:
					case OP_RES_A_4:
					case OP_RES_A_5:
					case OP_RES_A_6:
					case OP_RES_A_7:
						op1 = (opc >> 3) & 0x07;
						z80->u_af.A &= ~(1 << op1);
						break;

					case OP_RES_B_0:
					case OP_RES_B_1:
					case OP_RES_B_2:
					case OP_RES_B_3:
					case OP_RES_B_4:
					case OP_RES_B_5:
					case OP_RES_B_6:
					case OP_RES_B_7:
						op1 = (opc >> 3) & 0x07;
						z80->u_bc.B &= ~(1 << op1);
						break;

					case OP_RES_C_0:
					case OP_RES_C_1:
					case OP_RES_C_2:
					case OP_RES_C_3:
					case OP_RES_C_4:
					case OP_RES_C_5:
					case OP_RES_C_6:
					case OP_RES_C_7:
						op1 = (opc >> 3) & 0x07;
						z80->u_bc.C &= ~(1 << op1);
						break;

					case OP_RES_D_0:
					case OP_RES_D_1:
					case OP_RES_D_2:
					case OP_RES_D_3:
					case OP_RES_D_4:
					case OP_RES_D_5:
					case OP_RES_D_6:
					case OP_RES_D_7:
						op1 = (opc >> 3) & 0x07;
						z80->u_de.D &= ~(1 << op1);
						break;

					case OP_RES_E_0:
					case OP_RES_E_1:
					case OP_RES_E_2:
					case OP_RES_E_3:
					case OP_RES_E_4:
					case OP_RES_E_5:
					case OP_RES_E_6:
					case OP_RES_E_7:
						op1 = (opc >> 3) & 0x07;
						z80->u_de.E &= ~(1 << op1);
						break;

					case OP_RES_H_0:
					case OP_RES_H_1:
					case OP_RES_H_2:
					case OP_RES_H_3:
					case OP_RES_H_4:
					case OP_RES_H_5:
					case OP_RES_H_6:
					case OP_RES_H_7:
						op1 = (opc >> 3) & 0x07;
						z80->u_hl.H &= ~(1 << op1);
						break;

					case OP_RES_L_0:
					case OP_RES_L_1:
					case OP_RES_L_2:
					case OP_RES_L_3:
					case OP_RES_L_4:
					case OP_RES_L_5:
					case OP_RES_L_6:
					case OP_RES_L_7:
						op1 = (opc >> 3) & 0x07;
						z80->u_hl.L &= ~(1 << op1);
						break;

					case OP_RES_ADDR_HL_0:
					case OP_RES_ADDR_HL_1:
					case OP_RES_ADDR_HL_2:
					case OP_RES_ADDR_HL_3:
					case OP_RES_ADDR_HL_4:
					case OP_RES_ADDR_HL_5:
					case OP_RES_ADDR_HL_6:
					case OP_RES_ADDR_HL_7:
						op1 = (opc >> 3) & 0x07;
						opc = z80->mem_read(z80->u_hl.HL);
						opc &= ~(1 << op1);
						z80->mem_write(z80->u_hl.HL, opc);
						break;
		
				}
				break;

			case OP_DAA:
				opc = z80->u_af.A;
				op1	= 0;
				if(z80->u_af.A >= 0xFF || GETFLAG(z80, FL_CARRY))
					op1 = 0x60;
				else
					CLRFLAG(z80, FL_CARRY);
				if((z80->u_af.A & 0x0F) > 0x09 || GETFLAG(z80, FL_HCARRY))
					op1 |= 0x06;
				if(GETFLAG(z80, FL_SUB))
					z80->u_af.A = z80->u_af.A + op1;
				else
					z80->u_af.A = z80->u_af.A - op1;
				(opc >> 4) ^ (z80->u_af.A >> 4) ? SETFLAG(z80, FL_HCARRY) :
					CLRFLAG(z80, FL_HCARRY);
				z80->u_af.A ? CLRFLAG(z80, FL_SUB) : SETFLAG(z80, FL_SUB);
				break;

			case OP_CPL:
				SETFLAG(z80, FL_SUB|FL_HCARRY);
				z80->u_af.A = ~z80->u_af.A;
				break;

			case OP_CCF:
				CLRFLAG(z80, FL_SUB|FL_HCARRY);
				GETFLAG(z80, FL_CARRY) ? CLRFLAG(z80, FL_CARRY) : SETFLAG(z80, FL_CARRY);
				break;

			case OP_SCF:
				CLRFLAG(z80, FL_SUB|FL_HCARRY);
				SETFLAG(z80, FL_CARRY);
				break;

			case OP_NOP:
				break;

			case OP_HALT:
				break;

			/* FIXME */
			case OP_STOP:
				printf("OP_STOP\n");
				while(1);

				break;

			case OP_DI:
				z80->IFF = 0;
				break;

			case OP_EI:
				z80->IFF = 1;
				break;

			/* FIXME: GBCPUman.pdf states FL_ZERO is set if result is zero. Z80 CPU
				manual says FL_ZERO is unaffected. It's probably a mistake in the
				Gameboy docs. */
			case OP_RLCA:
				CLRFLAG(z80, FL_SUB|FL_HCARRY);
				(z80->u_af.A >> 7) ? SETFLAG(z80, FL_CARRY) : CLRFLAG(z80, FL_CARRY);
			/*	(z80->u_af.A = (z80->u_af.A << 1) | GETFLAG(z80, FL_CARRY)) ?
					CLRFLAG(z80, FL_ZERO) : SETFLAG(z80, FL_ZERO);*/
				z80->u_af.A = (z80->u_af.A << 1) | GETFLAG(z80, FL_CARRY);
				break;

			case OP_RLA:
				CLRFLAG(z80, FL_SUB|FL_HCARRY);
				op1 = z80->u_af.A >> 7;
			/*	(z80->u_af.A = (z80->u_af.A << 1) | GETFLAG(z80, FL_CARRY)) ?
					CLRFLAG(z80, FL_ZERO) : SETFLAG(z80, FL_ZERO);*/
				z80->u_af.A = (z80->u_af.A << 1) | GETFLAG(z80, FL_CARRY);

				op1 ? SETFLAG(z80, FL_CARRY) : CLRFLAG(z80, FL_CARRY);
				break;

			case OP_RRCA:
				CLRFLAG(z80, FL_SUB|FL_HCARRY);
				(z80->u_af.A & 0x01) ? SETFLAG(z80, FL_CARRY) : CLRFLAG(z80, FL_CARRY);
			/*	(z80->u_af.A = (z80->u_af.A >> 1) | (GETFLAG(z80, FL_CARRY) << 7)) ?
					CLRFLAG(z80, FL_ZERO) : SETFLAG(z80, FL_ZERO);*/
				z80->u_af.A = (z80->u_af.A >> 1) | (GETFLAG(z80, FL_CARRY) << 7);
				break;

			case OP_RRA:
				CLRFLAG(z80, FL_SUB|FL_HCARRY);
				op1 = (z80->u_af.A & 0x01);
			/*	(z80->u_af.A = (z80->u_af.A >> 1) | (GETFLAG(z80, FL_CARRY) << 7)) ?
					CLRFLAG(z80, FL_ZERO) : SETFLAG(z80, FL_ZERO);*/
				z80->u_af.A = (z80->u_af.A >> 1) | (GETFLAG(z80, FL_CARRY) << 7);

				op1 ? SETFLAG(z80, FL_CARRY) : CLRFLAG(z80, FL_CARRY);
				break;

			case OP_JP_NN:
				t = GETSHORT(z80);
				z80->pc = (u16)t;
				break;

			case OP_JP_NZ_NN:
				t = GETSHORT(z80);
				if(!GETFLAG(z80, FL_ZERO)) z80->pc = (u16)t;
				break;

			case OP_JP_Z_NN:
				t = GETSHORT(z80);
				if(GETFLAG(z80, FL_ZERO)) z80->pc = (u16)t;
				break;

			case OP_JP_NC_NN:
				t = GETSHORT(z80);
				if(!GETFLAG(z80, FL_CARRY)) z80->pc = (u16)t;
				break;

			case OP_JP_C_NN:
				t = GETSHORT(z80);
				if(GETFLAG(z80, FL_CARRY)) z80->pc = (u16)t;
				break;

			case OP_JP_ADDR_HL:
				z80->pc = z80->u_hl.HL;
				break;

			case OP_JR_N:
				op1 = GETBYTE(z80);
				z80->pc = z80->pc + (s8)op1;
				break;

			case OP_JR_NZ_N:
				op1 = GETBYTE(z80);
				if(!GETFLAG(z80, FL_ZERO)) z80->pc = z80->pc + (s8)op1;
				break;

			case OP_JR_Z_N:
				op1 = GETBYTE(z80);
				if(GETFLAG(z80, FL_ZERO)) z80->pc = z80->pc + (s8)op1;
				break;

			case OP_JR_NC_N:
				op1 = GETBYTE(z80);
				if(!GETFLAG(z80, FL_CARRY)) z80->pc = z80->pc + (s8)op1;
				break;

			case OP_JR_C_N:
				op1 = GETBYTE(z80);
				if(GETFLAG(z80, FL_CARRY)) z80->pc = z80->pc + (s8)op1;
				break;

			case OP_CALL_NN:
				t = GETSHORT(z80);
#ifdef LITTLE_ENDIAN
				z80->mem_write(--z80->sp, *((u8*)&z80->pc));
				z80->mem_write(--z80->sp, *(((u8*)&z80->pc) + 1));
#else
				z80->mem_write(--z80->sp, *(((u8*)&z80->pc) + 1));
				z80->mem_write(--z80->sp, *((u8*)&z80->pc));
#endif
				z80->pc = (u16)t;
				break;

			case OP_CALL_NZ_NN:
				t = GETSHORT(z80);
				if(!GETFLAG(z80, FL_ZERO)) {
#ifdef LITTLE_ENDIAN
					z80->mem_write(--z80->sp, *((u8*)&z80->pc));
					z80->mem_write(--z80->sp, *(((u8*)&z80->pc) + 1));
#else
					z80->mem_write(--z80->sp, *(((u8*)&z80->pc) + 1));
					z80->mem_write(--z80->sp, *((u8*)&z80->pc));
#endif
					z80->pc = (u16)t;
				}
				break;

			case OP_CALL_Z_NN:
				t = GETSHORT(z80);
				if(GETFLAG(z80, FL_ZERO)) {
#ifdef LITTLE_ENDIAN
					z80->mem_write(--z80->sp, *((u8*)&z80->pc));
					z80->mem_write(--z80->sp, *(((u8*)&z80->pc) + 1));
#else
					z80->mem_write(--z80->sp, *(((u8*)&z80->pc) + 1));
					z80->mem_write(--z80->sp, *((u8*)&z80->pc));
#endif
					z80->pc = (u16)t;
				}
				break;

			case OP_CALL_NC_NN:
				t = GETSHORT(z80);
				if(!GETFLAG(z80, FL_CARRY)) {
#ifdef LITTLE_ENDIAN
					z80->mem_write(--z80->sp, *((u8*)&z80->pc));
					z80->mem_write(--z80->sp, *(((u8*)&z80->pc) + 1));
#else
					z80->mem_write(--z80->sp, *(((u8*)&z80->pc) + 1));
					z80->mem_write(--z80->sp, *((u8*)&z80->pc));
#endif
					z80->pc = (u16)t;
				}
				break;

			case OP_CALL_C_NN:
				t = GETSHORT(z80);
				if(GETFLAG(z80, FL_CARRY)) {
#ifdef LITTLE_ENDIAN
					z80->mem_write(--z80->sp, *((u8*)&z80->pc));
					z80->mem_write(--z80->sp, *(((u8*)&z80->pc) + 1));
#else
					z80->mem_write(--z80->sp, *(((u8*)&z80->pc) + 1));
					z80->mem_write(--z80->sp, *((u8*)&z80->pc));
#endif
					z80->pc = (u16)t;
				}
				break;

			case OP_RST_00:
			case OP_RST_08:
			case OP_RST_10:
			case OP_RST_18:
			case OP_RST_20:
			case OP_RST_28:
			case OP_RST_30:
			case OP_RST_38:
				op1 = ((opc >> 3) & 0x07) * 8;
#ifdef LITTLE_ENDIAN
				z80->mem_write(--z80->sp, *((u8*)&z80->pc));
				z80->mem_write(--z80->sp, *(((u8*)&z80->pc) + 1));
#else
				z80->mem_write(--z80->sp, *(((u8*)&z80->pc) + 1));
				z80->mem_write(--z80->sp, *((u8*)&z80->pc));
#endif
				z80->pc = op1;
				break;

			case OP_RET:
#ifdef LITTLE_ENDIAN
				*(((u8*)&z80->pc) + 1) = z80->mem_read(z80->sp++);
				*((u8*)&z80->pc) = z80->mem_read(z80->sp++);
#else
				*((u8*)&z80->pc) = z80->mem_read(z80->sp++);
				*(((u8*)&z80->pc) + 1) = z80->mem_read(z80->sp++);
#endif
				break;

			case OP_RET_NZ:
				if(!GETFLAG(z80, FL_ZERO)) {
#ifdef LITTLE_ENDIAN
					*(((u8*)&z80->pc) + 1) = z80->mem_read(z80->sp++);
					*((u8*)&z80->pc) = z80->mem_read(z80->sp++);
#else
					*((u8*)&z80->pc) = z80->mem_read(z80->sp++);
					*(((u8*)&z80->pc) + 1) = z80->mem_read(z80->sp++);
#endif
				}
				break;

			case OP_RET_Z:
				if(GETFLAG(z80, FL_ZERO)) {
#ifdef LITTLE_ENDIAN
					*(((u8*)&z80->pc) + 1) = z80->mem_read(z80->sp++);
					*((u8*)&z80->pc) = z80->mem_read(z80->sp++);
#else
					*((u8*)&z80->pc) = z80->mem_read(z80->sp++);
					*(((u8*)&z80->pc) + 1) = z80->mem_read(z80->sp++);
#endif
				}
				break;

			case OP_RET_NC:
				if(!GETFLAG(z80, FL_CARRY)) {
#ifdef LITTLE_ENDIAN
					*(((u8*)&z80->pc) + 1) = z80->mem_read(z80->sp++);
					*((u8*)&z80->pc) = z80->mem_read(z80->sp++);
#else
					*((u8*)&z80->pc) = z80->mem_read(z80->sp++);
					*(((u8*)&z80->pc) + 1) = z80->mem_read(z80->sp++);
#endif
				}
				break;

			case OP_RET_C:
				if(GETFLAG(z80, FL_CARRY)) {
#ifdef LITTLE_ENDIAN
					*(((u8*)&z80->pc) + 1) = z80->mem_read(z80->sp++);
					*((u8*)&z80->pc) = z80->mem_read(z80->sp++);
#else
					*((u8*)&z80->pc) = z80->mem_read(z80->sp++);
					*(((u8*)&z80->pc) + 1) = z80->mem_read(z80->sp++);
#endif
				}
				break;

			case OP_RETI:
#ifdef LITTLE_ENDIAN
				*(((u8*)&z80->pc) + 1) = z80->mem_read(z80->sp++);
				*((u8*)&z80->pc) = z80->mem_read(z80->sp++);
#else
				*((u8*)&z80->pc) = z80->mem_read(z80->sp++);
				*(((u8*)&z80->pc) + 1) = z80->mem_read(z80->sp++);
#endif
				/* enable interrupts */
				z80->IFF = 1;
				break;

			default:
				printf("encountered unknown opcode 0x%x at 0x%x\n", opc, z80->pc);
				return ERRHALT;
		}

	}

	return left;
}

/**
 * z80_interrupt - Causes an interrupt in the Z80 CPU.
 *
 * @param z80
 *	A pointer to a z80_machine_t structure.
 * @param isr_addr
 *	The address of a interrupt service routine that the CPU will
 *		jump to.
 *
 * @return
 *	If the Z80 CPU was interrupted, 1 is returned. If the CPU was
 *		not interrupted because interrupts were disabled, 0 is
 *		returned.
 */
s32 z80_interrupt( z80_machine_t *z80, u16 isr_addr ) {
	/* don't do anything if interrupts are not enabled */
	if(!z80->IFF)
		return 0;

#ifdef LITTLE_ENDIAN
	z80->mem_write(--z80->sp, *((u8*)&z80->pc));
	z80->mem_write(--z80->sp, *(((u8*)&z80->pc) + 1));
#else
	z80->mem_write(--z80->sp, *(((u8*)&z80->pc) + 1));
	z80->mem_write(--z80->sp, *((u8*)&z80->pc));
#endif

	/* disable interrupts */
	z80->IFF = 0;

	/* jump to interrupt service routine */
	z80->pc = isr_addr;

	return 1;
}
