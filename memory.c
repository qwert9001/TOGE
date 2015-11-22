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

/*
 * mem_load_cartridge - Loads a cartridge into the emulator.
 *
 * @param rom
 *	A pointer to a ROM cartridge file in memory.
 * @param rom_size
 *	Size of the cartridge file pointed to by 'rom', in bytes.
 *
 * @return
 *	The function returns GB_EMU_OK if the cartridge was successfully
 *		loaded, otherwise GB_EMU_ERROR is returned.
 *
 */
int mem_load_cartridge( unsigned char *rom, unsigned int rom_size ) {
	CartInfo_t *CartInfo;
	unsigned int i;

	/* get a pointer to the cartridge's information area */
	CartInfo = (CartInfo_t*) (rom + 0x100);

	/* calculate the number of ROM banks from the file size */
	Gameboy.Memory.NumROMBanks = rom_size / 0x4000;

	switch(CartInfo->RAMSize) {
		case 0:
			Gameboy.Memory.NumRAMBanks = 0;
			break;
		case 1:
		case 2:
			Gameboy.Memory.NumRAMBanks = 1;
			break;
		case 3:
			Gameboy.Memory.NumRAMBanks = 4;
			break;
		case 4:
			Gameboy.Memory.NumRAMBanks = 16;
			break;
		default:
			printf("mem_load_cartridge: Unknown RAM size (%i)\n",
				CartInfo->RAMSize);
			return GB_EMU_ERROR;
	}

	/* allocate memory for ROM and RAM banks */
	Gameboy.Memory.ROMBanks = malloc( Gameboy.Memory.NumROMBanks *
		sizeof(unsigned char*));

	for(i = 0; i < Gameboy.Memory.NumROMBanks; i++) {
		Gameboy.Memory.ROMBanks[ i ] = malloc(0x4000);

		/* copy ROM data from file into appropriate bank */
		memcpy( Gameboy.Memory.ROMBanks[ i ], rom + i * 0x4000,
			0x4000 );
	}

	Gameboy.Memory.RAMBanks = malloc( Gameboy.Memory.NumRAMBanks *
		sizeof(unsigned char*));

	for(i = 0; i < Gameboy.Memory.NumRAMBanks; i++)
		Gameboy.Memory.RAMBanks[ i ] = malloc(0x2000);

	/* figure out what kind of memory bank controller cartridge uses */
	if(GB_EMU_ERROR == mem_get_mbc_type(CartInfo, &Gameboy.Memory.MBC)) {
		printf("mem_load_cartridge: Unknown Memory Bank Controller (%i)\n",
			CartInfo->CartType);
		return GB_EMU_ERROR;
	}

	/* memory bank controllers default to 16 Mbit ROM, 8 KB RAM mode */
	Gameboy.Memory.MBCMode = MBC_MODE_16_8;

	/* ROM0 always points to the first entry in ROMBanks which is the
		fixed 16 Kb ROM space */
	Gameboy.Memory.ROM0 = Gameboy.Memory.ROMBanks[ 0 ];

	/* setup SROM to point at the first switchable ROM bank by default.
		There are always at least 2 ROM banks in a cartridge */
	Gameboy.Memory.SROM = Gameboy.Memory.ROMBanks[ 1 ];

	/* setup SRAM to point at the first switchable RAM bank */
	if( Gameboy.Memory.NumRAMBanks > 0 )
		Gameboy.Memory.SRAM = Gameboy.Memory.RAMBanks[ 0 ];

	Gameboy.Memory.ROMBankSelect = 1;
	Gameboy.Memory.RAMBankSelect = 0;

	/* everything OK */
	return GB_EMU_OK;
}

/*
 * mem_unload_cartridge - Unloads a cartridge.
 *
 */
int mem_unload_cartridge( void ) {
	unsigned int i;

	/* free up memory */
	for(i = 0; i < Gameboy.Memory.NumROMBanks; i++)
		free(Gameboy.Memory.ROMBanks[ i ]);
	
	for(i = 0; i < Gameboy.Memory.NumRAMBanks; i++)
		free(Gameboy.Memory.RAMBanks[ i ]);

	free(Gameboy.Memory.ROMBanks);
	free(Gameboy.Memory.RAMBanks);

	Gameboy.Memory.NumROMBanks		= 0;
	Gameboy.Memory.NumRAMBanks		= 0;
	Gameboy.Memory.RAMBankSelect	= 0;
	Gameboy.Memory.ROMBankSelect	= 0;

	Gameboy.Memory.ROMBanks			= NULL;
	Gameboy.Memory.RAMBanks			= NULL;
	Gameboy.Memory.SROM				= NULL;
	Gameboy.Memory.SRAM				= NULL;
	Gameboy.Memory.ROM0				= NULL;

	return GB_EMU_OK;
}

/*
 * mem_get_mbc_type - Figure out which MBC type catridge contains.
 *
 * @param CartInfo
 *	A pointer to a CartInfo_t structure.
 * @param MBC
 *	A pointer to a variable that will receive the MBC type.
 *
 * @return
 *	The function returns GB_EMU_OK on success, otherwise GB_EMU_ERROR
 *		is returned.
 *
 */
int mem_get_mbc_type( CartInfo_t *CartInfo, unsigned char *MBC ) {
	switch(CartInfo->CartType) {
		case 0x00: case 0x08: case 0x09:
			*MBC = MBC_TYPE_NONE;
			break;
		case 0x01: case 0x02: case 0x03:
			*MBC = MBC_TYPE_1;
			break;
		case 0x05: case 0x06:
			*MBC = MBC_TYPE_2;
			break;
		case 0x12: case 0x13:
			*MBC = MBC_TYPE_3;
			break;
		case 0x19: case 0x1A: case 0x1B:
		case 0x1C: case 0x1D: case 0x1E:
			*MBC = MBC_TYPE_5;
			break;
		default:
			return GB_EMU_ERROR;
	}

	return GB_EMU_OK;
}

/*
 * mem_normalize_addr - Get mmap identifier and normalize address.
 *
 * The function returns a memory map identifier for a memory address and
 * 	"normalizes" the address so that it will be in the range between
 *	0x000 - [memory region size].
 *
 * @param addr
 *	Pointer to a variable containing a memory address.
 *
 * @return
 *	Memory map identifier
 *
 */
int mem_normalize_addr( unsigned short *addr ) {
	unsigned short t = *addr;

	if( t < 0x4000 ) {
		return MEMORY_ROM0;
	}
	else if( t < 0x8000 ) {
		*addr = *addr - 0x4000;
		return MEMORY_SROM;
	}
	else if( t < 0xA000 ) {
		*addr = *addr - 0x8000;
		return MEMORY_VRAM;
	}
	else if( t < 0xC000 ) {
		*addr = *addr - 0xA000;
		return MEMORY_SRAM;
	}
	else if( t < 0xE000 ) {
		*addr = *addr - 0xC000;
		return MEMORY_RAM0;
	}
	else if( t < 0xFE00 ) {
		*addr = *addr - 0xE000;
		return MEMORY_ECHO;
	}
	else if( t < 0xFEA0 ) {
		*addr = *addr - 0xFE00;
		return MEMORY_OAM;
	}
	else if( t < 0xFF00 ) {
		*addr = *addr - 0xFEA0;
		return MEMORY_INV0;
	}
	else if( t < 0xFF4C ) {
		*addr = *addr - 0xFF00;
		return MEMORY_IOREG;
	}
	else if( t < 0xFF80 ) {
		*addr = *addr - 0xFF4C;
		return MEMORY_INV1;
	}
	else if( t < 0xFFFF ) {
		*addr = *addr - 0xFF80;
		return MEMORY_RAM1;
	}
	else
		return MEMORY_IE;

}

/*
 * mem_read - Read a byte from Gameboy memory.
 *
 * The address of this function is passed to the Z80 CPU simulator
 *	which then invokes it whenever it needs to fetch a byte from
 *	memory.
 *
 * @param addr
 *	Memory address to read.
 *
 * @return
 *	data in Gameboy memory at 'addr'.
 *
 */
unsigned char mem_read( unsigned short addr ) {
	/* normalize addr and retrieve memory map identifier */
	switch(mem_normalize_addr(&addr)) {

		case MEMORY_ROM0:
			return Gameboy.Memory.ROM0[ addr ];

		case MEMORY_SROM:
			return Gameboy.Memory.SROM[ addr ];

		case MEMORY_VRAM:
			return Gameboy.Memory.VRAM[ addr ];

		case MEMORY_SRAM:
			if(Gameboy.Memory.SRAM)
				return Gameboy.Memory.SRAM[ addr ];
			return 0;

		case MEMORY_RAM0:
			return Gameboy.Memory.RAM0[ addr ];

		/* echo of 8 KB fixed RAM */
		case MEMORY_ECHO:
			return Gameboy.Memory.RAM0[ addr ];

		case MEMORY_OAM:
			return Gameboy.Memory.OAM[ addr ];

		case MEMORY_IOREG:
			switch(addr) {
				case IO_REG_P1:
					return Gameboy.Memory.IORegs[ addr ] | 0x0F;

				case IO_REG_DIV:
					printf("Reading divider reg\n");

				default:
					return Gameboy.Memory.IORegs[ addr ];
			}
			return Gameboy.Memory.IORegs[ addr ];

		case MEMORY_RAM1:
			return Gameboy.Memory.RAM1[ addr ];

		case MEMORY_IE:
			return Gameboy.Memory.IE;

		case MEMORY_INV0:
		case MEMORY_INV1:
			printf("mem_read: read from invalid memory address (0x%x)\n",
				addr);
			return 0;

		default:
			printf("mem_read: read from unknown memory region (0x%x)\n",
				addr);
			return 0;
	}
}

/*
 * mem_write - Write a byte to Gameboy memory.
 *
 * The address of this function is passed to the Z80 CPU simulator
 *	which then invokes it whenever it needs to write a byte to
 *	memory.
 *
 * @param addr
 *	Memory address to write.
 *
 * @param value
 *	Value that will be written into memory at address 'addr'.
 *
 */
void mem_write( unsigned short addr, unsigned char value ) {
	unsigned short temp = addr;
	/* normalize addr and retrieve memory map identifier */
	switch(mem_normalize_addr(&addr)) {
		/* writes to ROM are caught by the memory bank controller */
		case MEMORY_ROM0:
		case MEMORY_SROM:
			mem_rom_write(temp, value);
			break;

		case MEMORY_VRAM:
			Gameboy.Memory.VRAM[ addr ] = value;
			break;

		case MEMORY_SRAM:
			if(Gameboy.Memory.SRAM)
				Gameboy.Memory.SRAM[ addr ] = value;
			break;

		case MEMORY_RAM0:
			Gameboy.Memory.RAM0[ addr ] = value;
			break;

		/* writes into this memory region echo into RAM0 */
		case MEMORY_ECHO:
			Gameboy.Memory.RAM0[ addr ] = value;
			break;

		case MEMORY_OAM:
			Gameboy.Memory.OAM[ addr ] = value;
			break;

		/* some I/O registers need special treatment */
		case MEMORY_IOREG:
			switch(addr) {
				case IO_REG_DMA:
					mem_do_dma(value);
					break;

				case IO_REG_TMA:
				case IO_REG_TAC:
					printf("Writing timer reg\n");
					break;
			}

			Gameboy.Memory.IORegs[ addr ] = value;
			break;

		case MEMORY_RAM1:
			Gameboy.Memory.RAM1[ addr ] = value;
			break;

		case MEMORY_IE:
			Gameboy.Memory.IE = value;
			break;

		case MEMORY_INV0:
		case MEMORY_INV1:
#ifdef DEBUG
			printf("mem_write: write into invalid memory address at 0x%x (v=0x%x)\n",
				temp, value);
#endif
			break;

		default:
			printf("mem_write: write into unknown memory region (0x%x)\n",
				temp);
			break;
	}
}

/*
 * mem_rom_write - Handles memory bank controllers.
 *
 * A Gameboy cartridge can contain a memory bank controller that intercepts
 *	attempts to write to read-only memory to blend in switchable ROM/RAM
 *	banks into the Gameboy's address space.
 *
 * @param addr
 *	Memory address to write.
 *
 * @param value
 *	Value that will be written into memory at address 'addr'.
 */
void mem_rom_write( unsigned short addr, unsigned char value ) {
	if(Gameboy.Memory.MBC == MBC_TYPE_NONE)
		return;

	/* writing into ROM at 0x6000 - 0x7FFF selects the MBC mode */
	if(addr >= 0x6000 && addr < 0x8000) {
		Gameboy.Memory.MBCMode = value & 0x1;
		return;
	}

	/* FIXME: add support for MBC2 and others */
	if(Gameboy.Memory.MBC != MBC_TYPE_1 && Gameboy.Memory.MBC != MBC_TYPE_3) {
		printf("mem_rom_write: unsupported memory bank controller.\n");
		while(1);
	}

	/* writing into 0x2000 - 0x3FFF selects a ROM bank */
	if(addr >= 0x2000 && addr < 0x4000) {
		switch(Gameboy.Memory.MBC) {
			case MBC_TYPE_1:
				/* only the lower 5 bits matter for MBC1 */
				value = value & 31;

				/* clear the lower 5 bits */
				Gameboy.Memory.ROMBankSelect &= ~31;

				/* the remaining 2 bits needed to form the 7 bit index for
					selecting one of the possible 128 ROM banks is provided
					by a write into ROM at 0x4000 - 0x5FFF */
				Gameboy.Memory.ROMBankSelect |= value;

				/* trying to load in ROM bank 0 will load ROM bank 1 */
				if(Gameboy.Memory.ROMBankSelect == 0)
					value = 1;
				else
					value = Gameboy.Memory.ROMBankSelect;

				/* switch it in */
				Gameboy.Memory.SROM = Gameboy.Memory.ROMBanks[ value ];
				return;

			case MBC_TYPE_3:
				/* MBC3 reads the 7 bit index in one go */
				value = value & 127;

				if(value == 0)
					value = 1;

				Gameboy.Memory.ROMBankSelect = value;
				Gameboy.Memory.SROM = Gameboy.Memory.ROMBanks
					[ Gameboy.Memory.ROMBankSelect ];
				return;

			default:
				return;
		}
	}

	/* writing into 0x4000 - 0x5FFF selects the higher 2 bits needed to
		form the ROM bank index for MBC 1. If controller mode is
		MBC_MODE_4_32 this will select one of the 4 possible RAM banks
		instead. */
	if(addr >= 0x4000 && addr < 0x6000) {
		switch(Gameboy.Memory.MBC) {
			case MBC_TYPE_1:
				if(Gameboy.Memory.MBCMode == MBC_MODE_16_8) {
					/* only the lower 2 bits matter */
					value = value & 0x03;

					/* clear the upper 3 bits */
					Gameboy.Memory.ROMBankSelect &= 31;
					Gameboy.Memory.ROMBankSelect |= (value << 5);

					/* trying to load in ROM bank 0 will load ROM bank 1 */
					if(Gameboy.Memory.ROMBankSelect == 0)
						value = 1;
					else
						value = Gameboy.Memory.ROMBankSelect;

					/* switch it in */
					Gameboy.Memory.SROM = Gameboy.Memory.ROMBanks[ value ];
				}
				else {
					/* select one of the 4 possible RAM banks */
					value = value & 0x03;

					Gameboy.Memory.RAMBankSelect = value;

					/* switch it in */
					Gameboy.Memory.SRAM = Gameboy.Memory.RAMBanks
						[ Gameboy.Memory.RAMBankSelect ];
				}
				return;

			case MBC_TYPE_3:
				/* select one of the 4 possible RAM banks */
				value = value & 0x03;

				Gameboy.Memory.RAMBankSelect = value;

				Gameboy.Memory.SRAM = Gameboy.Memory.RAMBanks
					[ Gameboy.Memory.RAMBankSelect ];
				break;

			default:
				return;
		}
	}

}

/*
 * mem_do_dma - Handles DMA transfers.
 *
 * A write to IO register IO_REG_DMA initiates a DMA transfer of 160 bytes
 *	of data from memory to Object Attribute Memory (OAM). The source
 *	address is calculated from the value written to IO_REG_DMA multiplied
 *	by 0x100.
 *
 */
void mem_do_dma( unsigned char from ) {
	/* figure out the source address */
	unsigned short addr = (from * 0x100);

	/* according to Gameboy docs the source address must be between 0x0000
		and 0xF100 */
	switch(mem_normalize_addr(&addr)) {
		case MEMORY_ROM0:
			memcpy(Gameboy.Memory.OAM, &Gameboy.Memory.ROM0[ addr ], 160);
			break;

		case MEMORY_SROM:
			memcpy(Gameboy.Memory.OAM, &Gameboy.Memory.SROM[ addr ], 160);
			break;

		case MEMORY_VRAM:
			memcpy(Gameboy.Memory.OAM, &Gameboy.Memory.VRAM[ addr ], 160);
			break;

		case MEMORY_SRAM:
			if(Gameboy.Memory.SRAM)
				memcpy(Gameboy.Memory.OAM, &Gameboy.Memory.SRAM[ addr ],
					160);
			break;

		case MEMORY_RAM0:
		case MEMORY_ECHO:
			memcpy(Gameboy.Memory.OAM, &Gameboy.Memory.RAM0[ addr ], 160);
			break;
	}
}
