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

#ifndef _WIN32_H_
#define _WIN32_H_

#include "gameboy.h"
#include <windows.h>
#include <tchar.h>
#include <io.h>
#include "resource.h"
#include <time.h>

/* exported by host environment */
EMU_IMPORT int win32_init( void );
EMU_IMPORT void win32_sys( void );
EMU_IMPORT void win32_blt( void );
EMU_IMPORT void win32_plot( unsigned int p, int c );

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam,
						 LPARAM lParam);
BOOL CALLBACK DlgAboutProc( HWND hDlg, UINT uMsg, WPARAM wParam,
						   LPARAM lParam );
BOOL CALLBACK DlgControlProc( HWND hDlg, UINT uMsg, WPARAM wParam,
						   LPARAM lParam );

LPBYTE GetROMFile( LPDWORD lpFileSize );
VOID ReleaseROMFile( LPBYTE lpROM );

#define CLASSNAME	_T("GAMEBOYEMU")
#define WINDOWNAME	_T("GameBoy Emulator")

/* WinAPI stuff */
int main(int argc, char *argv[]);

BOOL MakeURLControl( HWND hWnd, PTCHAR szUrl );
BOOL MakeAnimControl( HWND hWnd, DWORD *lpResId, INT nNumBitmap, INT nSpeed );
VOID CenterWindow( HWND hWnd, HWND hWndParent );
VOID ShowConsole( INT nShow );

#endif /* _WIN32_H_ */
