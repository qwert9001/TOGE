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

#ifndef _MEMORY_H_
#define _MEMORY_H_

/* Gameboy Memory */
typedef struct {
	unsigned char IE;				/* Interrupt Enable Register */ 
	unsigned char RAM1	[0x0007F];	/* small amount of RAM located at 0xFF80 */

	unsigned char IORegs[0x0004C];	/* Memory-mapped I/O Registers */

	unsigned char OAM	[0x000A0];	/* Object Attribute Memory */
	unsigned char RAM0	[0x02000];	/* 8 Kb fixed RAM bank */

	unsigned char VRAM	[0x02000];	/* 8 Kb video RAM */
	unsigned char *ROM0;			/* pointer to 16 Kb fixed ROM bank */

	unsigned char *SROM;			/* pointer to switched-in ROM bank */
	unsigned char *SRAM;			/* pointer to switched-in RAM bank */

	unsigned char **ROMBanks;		/* 16 KB switchable ROM banks */
	unsigned char **RAMBanks;		/* 8 KB switchable RAM banks */

	unsigned int NumROMBanks;		/* Number of ROM banks cartridge uses */
	unsigned int NumRAMBanks;		/* Number of RAM banks cartridge uses */

	unsigned int ROMBankSelect;		/* index of currently selected ROM bank */
	unsigned int RAMBankSelect;		/* index of currently selected RAM bank */

	unsigned char MBC;				/* Memory bank controller type */
	unsigned int MBCMode;			/* Memory bank controller mode */

} Memory_t;


/* Memory bank controller types */
enum {
	MBC_TYPE_NONE,

	MBC_TYPE_1,
	MBC_TYPE_2,
	MBC_TYPE_3,
	MBC_TYPE_5
};

/* Memory bank controller modes */
enum {
	MBC_MODE_16_8,				/* 16 Mbit ROM and 8 KB RAM */
	MBC_MODE_4_32				/* 4 Mbit ROM and 32 KB RAM */
};

/* Cartridge information stored at 0x100 */
typedef struct {
	unsigned char Stub[4];		/* code stub */
	unsigned char Logo[48];		/* Nintendo logo */
	unsigned char Name[15];		/* game name, zero padded */
	unsigned char GBC;			/* 1 if Gameboy Color cartridge */
	unsigned char License[2];	/* 2-byte License code */
	unsigned char SGB;			/* 3 if cartridge uses Super Gameboy functions */
	unsigned char CartType;
	unsigned char ROMSize;
	unsigned char RAMSize;
	unsigned char Dest;			/* destination code */
	unsigned char License2;		/* deprecated license code */
	unsigned char MaskROM;
	unsigned char CCheck;		/* complement check */
	unsigned char Checksum[2];	/* 2-byte checksum */

} CartInfo_t;

/* Memory map identifiers */
enum {
	MEMORY_ROM0,				/* 0x0000 - 0x3FFF */
	MEMORY_SROM,				/* 0x4000 - 0x7FFF */
	MEMORY_VRAM,				/* 0x8000 - 0x9FFF */
	MEMORY_SRAM,				/* 0xA000 - 0xBFFF */
	MEMORY_RAM0,				/* 0xC000 - 0xDFFF */
	MEMORY_ECHO,				/* 0xE000 - 0xFDFF */
	MEMORY_OAM,					/* 0xFE00 - 0xFE9F */
	MEMORY_INV0,				/* 0xFEA0 - 0xFEFF */
	MEMORY_IOREG,				/* 0xFF00 - 0xFF4B */
	MEMORY_INV1,				/* 0xFF4C - 0xFF7F */
	MEMORY_RAM1,				/* 0xFF80 - 0xFFFE */
	MEMORY_IE					/* 0xFFFF - 0xFFFF */
};

/* Memory-mapped I/O registers */
#define IO_REG_P1		0x00	/* Joypad info */
#define IO_REG_SB		0x01	/* Serial transfer data */
#define IO_REG_SC		0x02	/* SIO control */
#define IO_REG_DIV		0x04	/* Divider register */
#define IO_REG_TIMA		0x05	/* Timer counter */
#define IO_REG_TMA		0x06	/* Timer modulo */
#define IO_REG_TAC		0x07	/* Timer control */
#define IO_REG_IF		0x0F	/* Interrupt flag */

#define IO_REG_NR10		0x10	/* Sound mode 1 sweep register */
#define IO_REG_NR11		0x11	/* Sound mode 1 wave pattern */
#define IO_REG_NR12		0x12	/* Sound mode 1 envelope */
#define IO_REG_NR13		0x13	/* Sound mode 1 freq low */
#define IO_REG_NR14		0x14	/* Sound mode 1 freq high */

#define IO_REG_NR21		0x16	/* Sound mode 2 wave pattern */
#define IO_REG_NR22		0x17	/* Sound mode 2 envelope */
#define IO_REG_NR23		0x18	/* Sound mode 2 freq low */
#define IO_REG_NR24		0x19	/* Sound mode 2 freq high */

#define IO_REG_NR30		0x1A	/* Sound mode 3 on/off */
#define IO_REG_NR31		0x1B	/* Sound mode 3 sound length */
#define IO_REG_NR32		0x1C	/* Sound mode 3 output level */
#define IO_REG_NR33		0x1D	/* Sound mode 3 freq low data */
#define IO_REG_NR34		0x1E	/* Sound mode 3 freq high data */

#define IO_REG_NR41		0x20	/* Sound mode 4 length */
#define IO_REG_NR42		0x21	/* Sound mode 4 envelope */
#define IO_REG_NR43		0x22	/* Sound mode 4 polynomial counter */
#define IO_REG_NR44		0x23	/* Sound mode 4 counter */

#define IO_REG_NR50		0x24	/* Channel control */
#define IO_REG_NR51		0x25	/* Select sound output terminal */
#define IO_REG_NR52		0x26	/* Sound on/off */

#define IO_REG_WPR		0x30	/* Wave pattern RAM 0xFF30-0xFF3F */
#define IO_REG_LCDC		0x40	/* LCD control */
#define IO_REG_STAT		0x41	/* Interrupt selection by LCDC status */
#define IO_REG_SCY		0x42	/* Scroll Y */
#define IO_REG_SCX		0x43	/* Scroll X */
#define IO_REG_LY		0x44	/* LCDC Y coordinate, the current scanline */
#define IO_REG_LYC		0x45	/* LY compare */
#define IO_REG_DMA		0x46	/* DMA transfer and start address */

#define IO_REG_BGP		0x47	/* BG & window palette data */
#define IO_REG_OBP0		0x48	/* Object palette 0 data */
#define IO_REG_OBP1		0x49	/* Object palette 1 data */
#define IO_REG_WY		0x4A	/* Window Y position */
#define IO_REG_WX		0x4B	/* Window X position */

/* function declarations */

int mem_load_cartridge( unsigned char *rom, unsigned int rom_size );
int mem_unload_cartridge( void );
int mem_get_mbc_type( CartInfo_t *CartInfo, unsigned char *MBC );
int mem_normalize_addr( unsigned short *addr );
unsigned char mem_read( unsigned short addr );
void mem_write( unsigned short addr, unsigned char value );
void mem_rom_write( unsigned short addr, unsigned char value );
void mem_do_dma( unsigned char from );

#endif /* _MEMORY_H_ */
