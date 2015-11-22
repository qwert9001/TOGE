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

/* the 4 shades of gray a Gameboy can display */
static int GBColors[4] = {
	0xFFFFFF,
	0xD694AD,
	0xAD4A5A,
	0x000000
};


/*
 * video_do_hblank - Handles a horizontal blank.
 *
 * This function gets called when a HBLANK interrupt occurs.
 *	It draws the current scanline, sets up the STATUS
 *	register bits and causes VBLANK and LCDC interrupts.
 * 
 */
void video_do_hblank( void ) {
	/* reset the clock cycle counter */
	Gameboy.Counters[CNT_HBLANK] = 0;

	/* increment current scanline */
	Gameboy.Memory.IORegs[IO_REG_LY]++;

	if(Gameboy.Memory.IORegs[IO_REG_LY] > 153)
		Gameboy.Memory.IORegs[IO_REG_LY] = 0;
	
	if(Gameboy.Memory.IORegs[IO_REG_LY] >= 144) {
		/* it's the VBLANK period, so set the VBLANK mode bit */
		Gameboy.Memory.IORegs[IO_REG_STAT] |= STAT_VBLANK;

		/* cause a VBLANK interrupt */
		if(Gameboy.Memory.IORegs[IO_REG_LY] == 144) {
			Gameboy.Memory.IORegs[IO_REG_IF] |= INT_VBLANK;

			/* blit to screen */
			host_blt();
		}
	}
	else {
		/* reset the VBLANK mode bit */
		Gameboy.Memory.IORegs[IO_REG_STAT] &= ~STAT_VBLANK;

		/* draw the current scanline */
		video_draw_scanline(Gameboy.Memory.IORegs[IO_REG_LY]);
	}

	/* set coincidence bit of STATUS register if LYC and LY
		registers are equal */
	if(Gameboy.Memory.IORegs[IO_REG_LYC] == Gameboy.Memory.IORegs
		[IO_REG_LY]) {
		Gameboy.Memory.IORegs[IO_REG_STAT] |= STAT_COIN;

		/* if coincidence select bit is on, cause an LCDC interrupt */
		if(Gameboy.Memory.IORegs[IO_REG_STAT] & STAT_COIN_SELECT )
			Gameboy.Memory.IORegs[IO_REG_IF] |= INT_LCDC;
	}
	else {
		/* reset coincidence bit of STATUS register */
		Gameboy.Memory.IORegs[IO_REG_STAT] &= ~STAT_COIN;
	}
}

/*
 * video_draw_scanline - Draws a single scanline.
 *
 * This function draws the current scanline. This is rather difficult
 *	because of the nature of the Gameboy's video hardware and its tile
 *	and sprite based video memory which doesn't go along well with
 *	a linear graphics framebuffer.
 *
 */
void video_draw_scanline( unsigned char scanline ) {
	/* don't draw anything if display is disabled */
	if(!(Gameboy.Memory.IORegs[IO_REG_LCDC] & LCD_ENABLE))
		return;

	/* draw window and background if they are enabled */
	if(Gameboy.Memory.IORegs[IO_REG_LCDC] & LCD_ENABLE_BGWND) {
		/* draw the background portion */
		video_draw_background(scanline);

		/* window can be disabled seperately */
	//	if(Gameboy.Memory.IORegs[IO_REG_LCDC] & LCD_ENABLE_WND)
	//		video_draw_window(scanline);
	}

	/* draw sprites */
	if(Gameboy.Memory.IORegs[IO_REG_LCDC] & LCD_ENABLE_OBJ)
		video_draw_sprites(scanline);
}

/*
 * video_draw_window - Draws the window.
 *
 * This function draws the window portion of a single scanline. The window
 *	overlays the background and none of its tiles are ever transparent.
 *
 */
void video_draw_window( unsigned char scanline ) {
	unsigned char *Tiledata, *Tilemap, *Tile, TileIndex, is_signed, line;
	int i, y_offset, x_offset;
	static int wndline;

	/* is window even visible? */
	if(Gameboy.Memory.IORegs[IO_REG_WY] >= 144 || Gameboy.Memory.IORegs
		[IO_REG_WX] >= 166)
		return;
	
	/* window is below current scanline */
	if(Gameboy.Memory.IORegs[IO_REG_WY] > scanline)
		return;

	/* figure out which tiledata to use for the window */
	if(Gameboy.Memory.IORegs[IO_REG_LCDC] & LCD_TILEDATA) {
		/* use the tiledata array in VRAM from 0x8000 to 0x8FFF. The
			indeces in the tilemap array will be treated as unsigned values
			from 0 to 255. */
		Tiledata	= &Gameboy.Memory.VRAM[0x0000];
		is_signed	= 0;
	} else {
		/* use the tiledata at 0x8800 to 0x97FF. The indeces in the
			tilemap array will be treated as signed values from -128 to
			127 so the first and last tiles are at (a sprite graphic is
			16 byte):
				0x9000 + (-128 * 16) = 0x8800
				0x9000 + (+127 * 16) = 0x97F0
		*/
		Tiledata	= &Gameboy.Memory.VRAM[0x1000];
		is_signed	= 1;
	}

	/* figure out which tilemap to use for the window */
	if(Gameboy.Memory.IORegs[IO_REG_LCDC] & LCD_TILEMAP_WND) {
		/* use tilemap at 0x9C00 */
		Tilemap = &Gameboy.Memory.VRAM[0x1C00];
	} else {
		/* use tilemap at 0x9800 */
		Tilemap = &Gameboy.Memory.VRAM[0x1800];
	}

	/* reset current window line after VBLANK */
	if(Gameboy.Memory.IORegs[IO_REG_LY] == 0)
		wndline = 0;

	/* a tile is 8x8 pixels */
	for(i = 0; i < 160; i += 8) {
		/* retrieve the index into the tiledata array from the tilemap
			array */
		TileIndex = Tilemap[ (wndline >> 3) * 32 + (i >> 3) ];

		if(is_signed) {
			/* TileIndex is treated as a signed offset */
			Tile = &Tiledata[ ((signed char)TileIndex) * 16 ];
		} else {
			/* Tileindex is treated as an unsigned offset */
			Tile = &Tiledata[ TileIndex * 16 ];
		}

		/* Tile now points to the actual tiledata in VRAM. Now get to the
			Y offset within the tile graphic corresponding to the
			current windowline. */
		Tile = Tile + (wndline % 8) * 2;

		/* draw this line from the tile */
		for(line = 0; line < 8; line++) {
			/* extract color information */
			unsigned char b1 = (*(Tile + 0) & (0x80 >> (line + (i & 0x07))))
									? 1 : 0;
			unsigned char b2 = (*(Tile + 1) & (0x80 >> (line + (i & 0x07))))
									? 1 : 0;
			unsigned char c	 = (b2 << 1) | b1;

			/* IO_REG_BGP contains the palette data for window also */
			y_offset = (wndline + Gameboy.Memory.IORegs[IO_REG_WY]) * 160;
			x_offset = i + line;

			/* IO_REG_WX is the offset from absolute screen coordinates by 7 */
			if(Gameboy.Memory.IORegs[IO_REG_WX])
				x_offset = x_offset + Gameboy.Memory.IORegs[IO_REG_WX] - 7;

			host_plot( y_offset + x_offset, GBColors[ Gameboy.Memory.IORegs
				[IO_REG_BGP] >> (c * 2) & 0x03 ]);
		}
	}
	wndline++;
}

/*
 * video_draw_background - Draws the background.
 *
 * This function draws the background portion of a single scanline. The
 *	sprite portion of the scanline is rendered after this, so sprites
 *	are always on top of the background.
 *
 *	Inspired by gbe by Chuck Mason and Steven Fuller.
 *
 */
void video_draw_background( unsigned char scanline ) {
	unsigned char *Tiledata, *Tilemap, *Tile, is_signed, TileIndex,
					line, StartX, StartY, left;
	int i;

	/* figure out which tiledata to use for the background */
	if(Gameboy.Memory.IORegs[IO_REG_LCDC] & LCD_TILEDATA) {
		/* use the tiledata array in VRAM from 0x8000 to 0x8FFF. The
			indeces in the tilemap array will be treated as unsigned values
			from 0 to 255. */
		Tiledata	= &Gameboy.Memory.VRAM[0x0000];
		is_signed	= 0;
	} else {
		/* use the tiledata at 0x8800 to 0x97FF. The indeces in the
			tilemap array will be treated as signed values from -128 to
			127 so the first and last tiles are at (a sprite graphic is
			16 byte):
				0x9000 + (-128 * 16) = 0x8800
				0x9000 + (+127 * 16) = 0x97F0
		*/
		Tiledata	= &Gameboy.Memory.VRAM[0x1000];
		is_signed	= 1;
	}

	/* figure out which tilemap to use for the background */
	if(Gameboy.Memory.IORegs[IO_REG_LCDC] & LCD_TILEMAP_BG) {
		/* use tilemap at 0x9C00 */
		Tilemap = &Gameboy.Memory.VRAM[0x1C00];
	} else {
		/* use tilemap at 0x9800 */
		Tilemap = &Gameboy.Memory.VRAM[0x1800];
	}
	
	/* at any time there will be at least (160/8 = 20) tiles displayed
		horizontally and (144/8 = 18) tiles displayed vertically on the screen.
		If the scroll registers do not contain values that are multiples of 8,
		21 and 19 tiles are displayed, respectively, of which the first and the
		last will only be drawn partially. */

	/* a tile is 8x8 pixels */
	for(i = 0; i < 160; i += 8) {
		/* these are bytes so they will overflow if they get bigger than 255
			and so account for the wrap around effect of the background */
		StartX = Gameboy.Memory.IORegs[IO_REG_SCX] + i;
		StartY = Gameboy.Memory.IORegs[IO_REG_SCY] + scanline;

		/* retrieve the index into the tiledata array from the tilemap
			array */
		TileIndex = Tilemap[ (StartY >> 3) * 32 + (StartX >> 3) ];

		if(is_signed) {
			/* TileIndex is treated as a signed offset */
			Tile = &Tiledata[ ((signed char)TileIndex) * 16 ];
		} else {
			/* Tileindex is treated as an unsigned offset */
			Tile = &Tiledata[ TileIndex * 16 ];
		}

		/* Tile now points to the actual tiledata in VRAM. Now get to the
			Y offset within the sprite graphic corresponding to the
			current scanline. */
		Tile = Tile + (StartY % 8) * 2;

		/* draw this line from the tile */
		for(line = 0; line < 8; line++) {
			/* extract color information */
			unsigned char b1 = (*(Tile + 0) & (0x80 >> (line + (StartX & 0x07))))
									? 1 : 0;
			unsigned char b2 = (*(Tile + 1) & (0x80 >> (line + (StartX & 0x07))))
									? 1 : 0;
			unsigned char c	 = (b2 << 1) | b1;

			/* IO_REG_BGP contains the palette data */
			host_plot(scanline * 160 + i + line, GBColors[ (Gameboy.Memory.IORegs
				[IO_REG_BGP] >> (c * 2)) & 0x03 ]);

			/* if IO_REG_SCX is not a multiple of 8, an odd number of tiles will
				need to be displayed, so figure out how many pixels of the first tile
				we need to draw. */
			if(StartX & 0x07) {
				if(line == (7 - (StartX & 0x07))) {
					/* adjust i because we will have to draw one more tile */
					i = i - (StartX & 0x07);
					left = 8 - line; /* remember how many pixels to draw of the last
											tile. */
					break;
				}
			}
			/* last tile is only drawn partially if IO_REG_SCX is not a multiple
				of 8 */
			if(Gameboy.Memory.IORegs[IO_REG_SCX] % 8) {
				if( i > 152 && line >= left )
					break;
			}
		}
	}
}

/*
 * video_draw_sprites - Draws sprites that are within a scanline.
 *
 */
void video_draw_sprites( unsigned char scanline ) {
	unsigned char *OAM, *Tile, Height, Attr, Index, i, x, y, Offset, line,
					wbit, b1, b2, c, Palette;

	/* point to the end of object attribute memory */
	OAM = &Gameboy.Memory.OAM[0xA0];

	/* sprites are either 8x8 or 8x16 pixels */
	if(Gameboy.Memory.IORegs[IO_REG_LCDC] & LCD_OBJ_SIZE)
		Height = 16;
	else
		Height = 8;

	/* go through sprites from right to left because, when overlapping, sprites
		with lower memory addresses in OAM have priority. */
	for(i = 40; i > 0; i--) {
		Attr	= *--OAM;
		Index	= *--OAM;
		x		= *--OAM;
		y		= *--OAM;

		/* sprite is invisible if y is zero */
		if(!y)
			continue;

		/* sprite coordinates are rather odd, they do not describe the upper
			left corner of the sprite graphic but the lower right corner so
			account for that here */
		if(x < 8) {
			/* sprite will only be drawn partially */
			Offset = 8 - x;
			x = 0;
		} else {
			Offset = 0;
			x = x - 8;
		}

		y = y - 16;

		/* sprite is out of view */
		if(x >= 160)
			continue;

		/* skip sprite if it is above or below the current scanline */
		if(scanline < y || scanline >= (y + Height))
			continue;

		/* least significant bit is ignored if height mode is 16 pixels */
		if(Height == 16)
			Index = Index & 0xFE;

		/* sprite tile data is located at the very beginning of VRAM at
			0x8000 */
		Tile = &Gameboy.Memory.VRAM[ Index * 16 ];

		/* sprite is flipped vertically so read from tiledata backwards */
		if(Attr & SPRITE_FLIP_Y)
			Tile = Tile + ((Height - 1) - (scanline - y)) * 2;
		else
			Tile = Tile + (scanline - y) * 2; /* 2 bytes represent a line in a
														sprite */
		/* figure out which color palette to use */
		if(Attr & SPRITE_OBJ1PAL)
			Palette = Gameboy.Memory.IORegs[IO_REG_OBP1];
		else
			Palette = Gameboy.Memory.IORegs[IO_REG_OBP0];

		/* draw the line of the sprite */
		for(line = Offset; line < 8; line++) {
			/* sprite is flipped horizontally so read out in reverse order */
			if(Attr & SPRITE_FLIP_X)
				wbit = 7 - line;
			else
				wbit = line;

			/* extract color information */
			b1	= (*(Tile + 0) & (0x80 >> wbit)) ? 1 : 0;
			b2	= (*(Tile + 1) & (0x80 >> wbit)) ? 1 : 0;
			c	= (b2 << 1) | b1;

			/* 0 means transparency */
			if(c != 0)
				host_plot(scanline * 160 + x + line, GBColors[ (Palette >> (c * 2))
							& 0x03 ]);
		}
	}
}
