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

#include "gameboy.h"

/* global Gameboy variable included in all files */
Gameboy_t Gameboy;

/*
 * main - entry point
 *
 */
int main(int argc, char *argv[]) {
	/* initialize the host environment */
	if(host_init() != GB_EMU_OK) {
		printf("Could not initialize host environment.\n");
		return 0;
	}

	while(1) {
		/* wait for user to load a rom file and start the emulator */
		host_sys();

		/* start the emulator if a ROM has been loaded */
		if(Gameboy.Status == GB_EMU_STATUS_EXECUTING)
			gb_emu_run();

		/* user requested to shut program down */
		if(Gameboy.Status == GB_EMU_STATUS_SHUTDOWN)
			break;
	}

	printf("Exiting emulator\n");
	return 0;
}

/*
 * gb_emu_start - Start execution of a GameBoy ROM file.
 *
 * @param rom
 *	A pointer to a GameBoy ROM image in memory.
 * @param rom_size
 *	Size of the ROM image pointed to by 'rom', in bytes.
 *
 * @return
 *	If the ROM was successfully loaded, GB_EMU_OK is returned. If the ROM
 *	is not a valid GameBoy ROM or another error occured, GB_EMU_ERROR is
 *	returned.
 */
EMU_EXPORT int gb_emu_start( unsigned char *rom, unsigned int rom_size ) {
	/* unload old cartridge, if any */
	mem_unload_cartridge();

	/* reset the emulator */
	gb_emu_reset();

	/* try to load the cartridge */
	if(GB_EMU_ERROR == mem_load_cartridge(rom, rom_size))
		return GB_EMU_ERROR;

	/* we are executing a game */
	Gameboy.Status = GB_EMU_STATUS_EXECUTING;

	/* everything OK */
	return GB_EMU_OK;
}

/*
 * gb_emu_stop - Stop execution of current ROM file.
 *
 * The function stops the execution of the current ROM file. If no ROM file
 * is being executed when this function is called, it does nothing.
 *
 */
EMU_EXPORT void gb_emu_stop( void ) {
	if( Gameboy.Status == GB_EMU_STATUS_STOPPED )
		return;

	Gameboy.Status = GB_EMU_STATUS_STOPPED;

	/* free resources */
	mem_unload_cartridge();
}

/*
 * gb_emu_pause - Pause execution of current ROM file.
 *
 * @param pause
 *	Pauses or unpauses the emulator. Set to 1 to pause emulator,
 *		0 to resume execution of ROM file.
 *
 * @return
 *	The function returns the old pause status.
 */
EMU_EXPORT int gb_emu_pause( int pause ) {
	int ret;

	if( Gameboy.Status == GB_EMU_STATUS_STOPPED )
		return Gameboy.Status;

	ret	= Gameboy.Status;
	if( pause )
		Gameboy.Status = GB_EMU_STATUS_PAUSED;
	else
		Gameboy.Status = GB_EMU_STATUS_EXECUTING;

	return ret;
}

/*
 * gb_emu_status - Returns the current status of the emulator.
 *
 * @return
 *	The return value can be one of the following constants:
 *		GB_EMU_STATUS_STOPPED	- Not executing a ROM image
 *		GB_EMU_STATUS_EXECUTING	- Executing a ROM image
 *		GB_EMU_STATUS_PAUSED	- Executing a ROM image but paused.
 */
EMU_EXPORT int gb_emu_status( void ) {
	return Gameboy.Status;
}

/*
 * gb_emu_shutdown - Shuts down the emulator and exits the program.
 *
 */
EMU_EXPORT void gb_emu_shutdown( void ) {
	Gameboy.Status = GB_EMU_STATUS_SHUTDOWN;
}

/*
 * gb_emu_reset - Resets the emulator to a safe state.
 *
 * The function initializes the emulator's CPU and I/O registers
 *	with the same values as those found in a real Gameboy after power
 *	up.
 *
 */
void gb_emu_reset( void ) {
	int i;

	Gameboy.CPU.mem_read	= mem_read;
	Gameboy.CPU.mem_write	= mem_write;

	/* setup Z80 CPU registers */
	Gameboy.CPU.pc		= 0x0100;	Gameboy.CPU.sp		= 0xFFFE;
	Gameboy.CPU.u_af.AF	= 0x01B0;	Gameboy.CPU.u_bc.BC	= 0x0013;
	Gameboy.CPU.u_de.DE	= 0x00D8;	Gameboy.CPU.u_hl.HL	= 0x014D;
	
	/* setup I/O registers */
	Gameboy.Memory.IORegs[IO_REG_TIMA]	= 0x00;
	Gameboy.Memory.IORegs[IO_REG_TMA]	= 0x00;
	Gameboy.Memory.IORegs[IO_REG_TAC]	= 0x00;

	Gameboy.Memory.IORegs[IO_REG_NR10]	= 0x80;
	Gameboy.Memory.IORegs[IO_REG_NR11]	= 0xBF;
	Gameboy.Memory.IORegs[IO_REG_NR12]	= 0xF3;
	Gameboy.Memory.IORegs[IO_REG_NR14]	= 0xBF;
	Gameboy.Memory.IORegs[IO_REG_NR21]	= 0x3F;
	Gameboy.Memory.IORegs[IO_REG_NR22]	= 0x00;
	Gameboy.Memory.IORegs[IO_REG_NR24]	= 0xBF;
	Gameboy.Memory.IORegs[IO_REG_NR30]	= 0x7F;
	Gameboy.Memory.IORegs[IO_REG_NR31]	= 0xFF;
	Gameboy.Memory.IORegs[IO_REG_NR32]	= 0x9F;
	Gameboy.Memory.IORegs[IO_REG_NR33]	= 0xBF;
	Gameboy.Memory.IORegs[IO_REG_NR41]	= 0xFF;
	Gameboy.Memory.IORegs[IO_REG_NR42]	= 0x00;
	Gameboy.Memory.IORegs[IO_REG_NR43]	= 0x00;
	Gameboy.Memory.IORegs[IO_REG_NR30]	= 0xBF;
	Gameboy.Memory.IORegs[IO_REG_NR50]	= 0x77;
	Gameboy.Memory.IORegs[IO_REG_NR51]	= 0xF3;
	Gameboy.Memory.IORegs[IO_REG_NR52]	= 0xF1;

	Gameboy.Memory.IORegs[IO_REG_LCDC]	= 0x91;
	Gameboy.Memory.IORegs[IO_REG_SCX]	= 0x00;
	Gameboy.Memory.IORegs[IO_REG_SCY]	= 0x00;
	Gameboy.Memory.IORegs[IO_REG_LYC]	= 0x00;
	Gameboy.Memory.IORegs[IO_REG_BGP]	= 0xFC;
	Gameboy.Memory.IORegs[IO_REG_OBP0]	= 0xFF;
	Gameboy.Memory.IORegs[IO_REG_OBP1]	= 0xFF;
	Gameboy.Memory.IORegs[IO_REG_WY]	= 0x00;
	Gameboy.Memory.IORegs[IO_REG_WX]	= 0x00;

	Gameboy.Memory.IE = 0x00;

	/* reset clock cycle counters */
	for(i = 0; i < CNT_NUM_CNT; i++)
		Gameboy.Counters[ i ] = 0;
}

/*
 * gb_emu_run - The actual main emulation loop.
 *
 * Runs the CPU in a loop until the user requests to stop or
 *	pause the emulator.
 *
 */
void gb_emu_run( void ) {
	s32 i, Ran, Cycles = RUN_CYCLES;

	while(Gameboy.Status == GB_EMU_STATUS_EXECUTING) {
		/* run the CPU */
		Ran = z80_run(&Gameboy.CPU, Cycles);

		/* Gameboy doc contradicts itself on this but IF seems to be reset
			by hardware */
		Gameboy.Memory.IORegs[IO_REG_IF] = 0;

		/* update clock cycle counters */
		for(i = 0; i < CNT_NUM_CNT; i++)
			Gameboy.Counters[ i ] = Gameboy.Counters[ i ] + Cycles - Ran;

		Cycles = Ran + RUN_CYCLES;

		/* is it time to do a HBLANK interrupt yet? */
		if(Gameboy.Counters[CNT_HBLANK] > HBLANK_CYCLES )
			video_do_hblank();

		/* give host system a chance to do maintenance work */
		host_sys();

		/* do we need to cause an interrupt? */
		if(Gameboy.Memory.IORegs[IO_REG_IF] > 0) {
			/* priority ordered */
			for(i = 0; i < 4; i++) {
				if( (Gameboy.Memory.IORegs[IO_REG_IF] & (1 << i)) &&
					(Gameboy.Memory.IE & (1 << i)) ) {
					/* z80_interrupt checks the global interrupt enable
						flip-flop */
					z80_interrupt(&Gameboy.CPU, INT_VEC_TABLE + i * 0x08);
				}
			}
		}
	}
}

/*
 * gb_emu_single_step - Single step the CPU.
 *
 * Executes a single Z80 instruction and prints out CPU
 *	registers.
 *
 * @return
 *	The number of clock cycles the instruction took to execute.
 *
 */
int gb_emu_single_step( void ) {
	int ret = z80_run(&Gameboy.CPU, 1);
#ifdef WIN32
		system("cls");
#else
		system("clear");
#endif
		printf(	"A=0x%x (%i)\tB=0x%x (%i)\tD=0x%x (%i)\tH=0x%x (%i)\n"
				"F=0x%x (%i)\tC=0x%x (%i)\tE=0x%x (%i)\tL=0x%x (%i)\n"
				"\nPC=0x%x (%i)\t(inst. 0x%x)\nSP=0x%x (%i)\n\nIFF=%i\n",
			Gameboy.CPU.u_af.A, Gameboy.CPU.u_af.A, Gameboy.CPU.u_bc.B,
			Gameboy.CPU.u_bc.B, Gameboy.CPU.u_de.D, Gameboy.CPU.u_de.D,
			Gameboy.CPU.u_hl.H, Gameboy.CPU.u_hl.H, Gameboy.CPU.u_af.F,
			Gameboy.CPU.u_af.F, Gameboy.CPU.u_bc.C, Gameboy.CPU.u_bc.C,
			Gameboy.CPU.u_de.E, Gameboy.CPU.u_de.E, Gameboy.CPU.u_hl.L,
			Gameboy.CPU.u_hl.L, Gameboy.CPU.pc, Gameboy.CPU.pc,
			Gameboy.CPU.mem_read(Gameboy.CPU.pc), Gameboy.CPU.sp,
			Gameboy.CPU.sp, Gameboy.CPU.IFF);

		printf("\nPress Enter to single step. Press 'q' to quit.\n");
#ifdef WIN32
		if('q' == getch()) /* quit */
			Gameboy.Status = GB_EMU_STATUS_SHUTDOWN;
#else
		system("read");
#endif
		return -1 * (ret - 1);
}
