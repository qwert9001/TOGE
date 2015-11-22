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

#ifndef _GAMEBOY_H_
#define _GAMEBOY_H_

#define EMU_VERSION 4125

#if !defined(EMU_EXPORT)
 #define EMU_EXPORT
#endif

#if !defined(EMU_IMPORT)
 #define EMU_IMPORT
#endif

/* include system-specific master header */
#ifdef WIN32
 #include "win32.h"
#elif PS2
 #include "ps2.h"
#endif

#include "memory.h"
#include "video.h"
#include "z80.h"

/* function return values */
#define GB_EMU_OK					1
#define GB_EMU_ERROR				0

/* status codes returned by the gb_emu_status function */
#define GB_EMU_STATUS_STOPPED		0x00
#define GB_EMU_STATUS_EXECUTING		0x01
#define GB_EMU_STATUS_PAUSED		0x02
#define GB_EMU_STATUS_SHUTDOWN		0x04


/* The following system-specific callback functions must be implemented
	when porting the emulator to a new platform. */

/*
 * host_init - Initializes the host environment.
 *
 * Do whatever needs to be done here to set up the host environment. For
 * Win32 this means creating a window and a DIB section for blitting. For
 * PS2 it means initializing the GS and allocating frame buffers, etc.
 *
 * @return
 *	The function should return GB_EMU_OK if initialization was successful
 *	and GB_EMU_ERROR if an error occured.
 */
#ifdef WIN32
 #define host_init win32_init
#elif PS2
 #define host_init ps2_init
#else
 #error Please supply system-specific host_init function.
#endif

/*
 * host_sys - Perform host-specific tasks.
 *
 * This is called in regular intervals by the emulator to give the host
 * environment a chance to perform host-specific tasks. For Win32 this
 * means retrieving and dispatching messages to the window's wndproc.
 * For the PS2 it means querying the gamepad to see if the emulator
 * should be stopped, etc.
 *
 */
#ifdef WIN32
 #define host_sys win32_sys
#elif PS2
 #define host_sys ps2_init
#else
 #error Please supply system-specific host_sys function.
#endif

/*
 * host_blt - Perform block transfer of bitmap data.
 *
 * This is called by the emulator when a VBLANK interrupt occurs to
 * redraw the screen. For Win32 this means invalidating the window
 * and calling StretchBlt to blt the image data into the window's
 * device context.
 *
 */
#ifdef WIN32
 #define host_blt win32_blt
#elif PS2
 #define host_blt ps2_blt
#else
 #error Please supply system-specific host_blt function.
#endif

/*
 * host_plot - Plot a pixel to the display.
 *
 * This is called by the emulator when it needs to plot a pixel to
 *	the screen. For Win32 this manipulates the DIB bit values
 *	returned CreateDIBSection.
 *
 */
#ifdef WIN32
 #define host_plot win32_plot
#elif PS2
 #define host_plot win32_plot
#else
 #error Please supply system-specific host_plot function.
#endif


/* The following functions are exported by the emulator and can be called
	from the host environment to control the emulator. */

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
EMU_EXPORT int gb_emu_start( unsigned char *rom, unsigned int rom_size );

/*
 * gb_emu_stop - Stop execution of current ROM file.
 *
 * The function stops the execution of the current ROM file. If no ROM file
 * is being executed when this function is called, it does nothing.
 *
 */
EMU_EXPORT void gb_emu_stop( void );

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
EMU_EXPORT int gb_emu_pause( int pause );

/*
 * gb_emu_status - Returns the current status of the emulator.
 *
 * @return
 *	The return value can be one of the following constants:
 *		GB_EMU_STATUS_STOPPED	- Not executing a ROM image
 *		GB_EMU_STATUS_EXECUTING	- Executing a ROM image
 *		GB_EMU_STATUS_PAUSED	- Executing a ROM image but paused.
 */
EMU_EXPORT int gb_emu_status( void );

/*
 * gb_emu_shutdown - Shuts down the emulator and exits the program.
 *
 */
EMU_EXPORT void gb_emu_shutdown( void );

/*
 * End of EMU_EXPORTS
 *
 */

/* function declarations */
void gb_emu_reset( void );
void gb_emu_run( void );
int gb_emu_single_step( void );

/* clock cycle counters for HBLANK, timers, etc. */
enum {
	CNT_HBLANK,

	CNT_NUM_CNT
};

typedef struct {
	z80_machine_t CPU;
	int Status;
	Memory_t Memory;
	int Counters[CNT_NUM_CNT];

} Gameboy_t;

extern Gameboy_t Gameboy;

/* number of clock cycles to run before doing cyclic tasks */
#define RUN_CYCLES	256

/* interrupt vector table starts at 0x0040 in memory */
#define INT_VEC_TABLE	0x0040

/* interrupts */
#define INT_TRANSPIN	(1 << 4)	/* transition from high to low of P10-P13 */
#define INT_SIO			(1 << 3)	/* SIO completed */
#define INT_TIMER		(1 << 2)
#define INT_LCDC		(1 << 1)
#define INT_VBLANK		(1 << 0)

#endif /* _GAMEBOY_H_ */
