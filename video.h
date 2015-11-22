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

#ifndef _VIDEO_H_
#define _VIDEO_H_

#endif /* _VIDEO_H_ */

/* number of clock cycles before horizontal blank happens */
#define HBLANK_CYCLES		456

/* LCDC Status register bits */

/* this bit of the STATUS register is set by hardware
	during the VBLANK period */
#define STAT_VBLANK			(1 << 0)

/* this bit of the STATUS register is set by hardware
	when the LYC register is equal to the LY register */
#define STAT_COIN			(1 << 2)

/* if this user-selectable bit is set, an LCDC interrupt
	is caused when LYC == LY */
#define STAT_COIN_SELECT	(1 << 6)

/* LCD Control register bits */

/* this bit enables or disables the LCD display. If this is 0
	the display does not display anything */
#define LCD_ENABLE			(1 << 7)

/* window tile map display select */
#define LCD_TILEMAP_WND		(1 << 6)

/* enables or disables the window display */
#define LCD_ENABLE_WND		(1 << 5)

/* background and window tile data select */
#define LCD_TILEDATA		(1 << 4)

/* background tile map display select */
#define LCD_TILEMAP_BG		(1 << 3)

/* object sprize size. 8x8 if bit is cleared, 8x16 if bit is
	set */
#define LCD_OBJ_SIZE		(1 << 2)

/* enables or disables the sprite display */
#define LCD_ENABLE_OBJ		(1 << 1)

/* enables or disables both window and background display
	at the same time */
#define LCD_ENABLE_BGWND	(1 << 0)

/* sprite object attribute entry */
typedef struct {
	unsigned char x;		/* sprite x coordinate */
	unsigned char y;		/* sprite y coordinate */
	unsigned char index;	/* index into sprite pattern table */
	unsigned char flags;	/* flags, see below */
} Sprite_t;

/* use colors from OBJ1PAL register instead of OBJ0PAL */
#define SPRITE_OBJ1PAL		(1 << 4)

/* sprite will be flipped horizontally */
#define SPRITE_FLIP_X		(1 << 5)

/* sprite will be flipped vertically */
#define SPRITE_FLIP_Y		(1 << 6)

/* if this is off, sprite is displayed on top of background and
	window */
#define SPRITE_PRIORITY		(1 << 7)

/* function declarations */

void video_do_hblank( void );
void video_draw_scanline( unsigned char scanline );
void video_draw_window( unsigned char scanline );
void video_draw_background( unsigned char scanline );
void video_draw_sprites( unsigned char scanline );
