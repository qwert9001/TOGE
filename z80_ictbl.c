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

/* instruction cycle count table */
u32 z80_ictbl[256] = {
/*			00	01	02	03	04	05	06	07	08	09	0A	0B	0C	0D	0E	0F	*/

/* 00 */	4,	12,	8,	8,	4,	4,	8,	4,	20,	8,	8,	8,	4,	4,	8,	4,
/* 01 */	4,	12,	8,	8,	4,	4,	8,	4,	8,	8,	8,	8,	4,	4,	8,	4,
/* 02 */	8,	12,	8,	8,	0,	4,	8,	4,	8,	8,	8,	8,	4,	4,	8,	4,
/* 03 */	8,	12,	8,	8,	12,	12,	12,	4,	8,	8,	8,	8,	4,	4,	8,	4,
/* 04 */	4,	4,	4,	4,	4,	4,	8,	4,	4,	4,	4,	4,	4,	4,	8,	4,
/* 05 */	4,	4,	4,	4,	4,	4,	8,	4,	4,	4,	4,	4,	4,	4,	8,	4,
/* 06 */	4,	4,	4,	4,	4,	4,	8,	4,	4,	4,	4,	4,	4,	4,	8,	4,
/* 07 */	8,	8,	8,	8,	8,	8,	4,	8,	4,	4,	4,	4,	4,	4,	8,	4,
/* 08 */	4,	4,	4,	4,	4,	4,	8,	4,	4,	4,	4,	4,	4,	4,	8,	4,
/* 09 */	4,	4,	4,	4,	4,	4,	8,	4,	4,	4,	4,	4,	4,	4,	8,	4,
/* 0A */	4,	4,	4,	4,	4,	4,	8,	4,	4,	4,	4,	4,	4,	4,	8,	4,
/* 0B */	4,	4,	4,	4,	4,	4,	8,	4,	4,	4,	4,	4,	4,	4,	8,	4,
/* 0C */	8,	12,	12,	12,	12,	16,	8,	32,	8,	8,	12,	0,	12,	12,	8,	32,
/* 0D */	8,	12,	12,	0,	12,	16,	8,	32,	8,	8,	12,	0,	12,	0,	8,	32,
/* 0E */	12,	12,	8,	0,	0,	16,	8,	32,	16,	4,	16,	0,	0,	0,	8,	32,
/* 0F */	12,	12,	8,	4,	0,	16,	8,	32,	12,	8,	16,	4,	0,	0,	8,	32
};

/* instruction cycle count table for CB prefixed opcodes */
u32 z80_cb_ictbl[256] = {
/*			00	01	02	03	04	05	06	07	08	09	0A	0B	0C	0D	0E	0F	*/

/* 00 */	8,	8,	8,	8,	8,	8,	16,	8,	8,	8,	8,	8,	8,	8,	16,	8,
/* 01 */	8,	8,	8,	8,	8,	8,	16,	8,	8,	8,	8,	8,	8,	8,	16,	8,
/* 02 */	8,	8,	8,	8,	8,	8,	16,	8,	8,	8,	8,	8,	8,	8,	16,	8,
/* 03 */	8,	8,	8,	8,	8,	8,	16,	8,	8,	8,	8,	8,	8,	8,	16,	8,
/* 04 */	8,	8,	8,	8,	8,	8,	16,	8,	8,	8,	8,	8,	8,	8,	16,	8,
/* 05 */	8,	8,	8,	8,	8,	8,	16,	8,	8,	8,	8,	8,	8,	8,	16,	8,
/* 06 */	8,	8,	8,	8,	8,	8,	16,	8,	8,	8,	8,	8,	8,	8,	16,	8,
/* 07 */	8,	8,	8,	8,	8,	8,	16,	8,	8,	8,	8,	8,	8,	8,	16,	8,
/* 08 */	8,	8,	8,	8,	8,	8,	16,	8,	8,	8,	8,	8,	8,	8,	16,	8,
/* 09 */	8,	8,	8,	8,	8,	8,	16,	8,	8,	8,	8,	8,	8,	8,	16,	8,
/* 0A */	8,	8,	8,	8,	8,	8,	16,	8,	8,	8,	8,	8,	8,	8,	16,	8,
/* 0B */	8,	8,	8,	8,	8,	8,	16,	8,	8,	8,	8,	8,	8,	8,	16,	8,
/* 0C */	8,	8,	8,	8,	8,	8,	16,	8,	8,	8,	8,	8,	8,	8,	16,	8,
/* 0D */	8,	8,	8,	8,	8,	8,	16,	8,	8,	8,	8,	8,	8,	8,	16,	8,
/* 0E */	8,	8,	8,	8,	8,	8,	16,	8,	8,	8,	8,	8,	8,	8,	16,	8,
/* 0F */	8,	8,	8,	8,	8,	8,	16,	8,	8,	8,	8,	8,	8,	8,	16,	8
};
