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

#include "win32.h"

static LPBYTE	g_lpBits;
static HBITMAP	g_hBitmap;
static HWND		g_hWnd;

/*
 * win32_init - Initializes the Win32 host environment.
 *
 */
EMU_IMPORT int win32_init( void ) {
	WNDCLASS wnd;
	LPBITMAPINFO lpBmi;

	/* use VirtualAlloc to make sure data is aligned on a DWORD boundary */
/*	lpBmi = VirtualAlloc(NULL, sizeof(BITMAPINFO) + 4 * sizeof(RGBQUAD),
				MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);*/

	/* switched to 24 from 8 bit colordepth */
	lpBmi = VirtualAlloc(NULL, sizeof(BITMAPINFO), MEM_RESERVE | MEM_COMMIT,
				PAGE_READWRITE);

	lpBmi->bmiHeader.biSize			= sizeof(lpBmi->bmiHeader);
	lpBmi->bmiHeader.biWidth		= 160;
	lpBmi->bmiHeader.biHeight		= -144;
	lpBmi->bmiHeader.biPlanes		= 1;
	lpBmi->bmiHeader.biBitCount		= 24 /* 8 */;
	lpBmi->bmiHeader.biCompression	= BI_RGB;

	/* this is not used with 24 bit colordepth */	
/*	for(i = 0; i < 4; i++) {
		lpBmi->bmiColors[i].rgbRed		= i * 75;
		lpBmi->bmiColors[i].rgbGreen	= i * 75;
		lpBmi->bmiColors[i].rgbBlue		= i * 75;
	}
*/

	/* create a DIB for drawing */
	if(!(g_hBitmap = CreateDIBSection(NULL, lpBmi, DIB_RGB_COLORS, &g_lpBits,
		NULL, 0)))
		return GB_EMU_ERROR;

	VirtualFree(lpBmi, 0, MEM_RELEASE);

	/* create window */
	wnd.cbClsExtra		= 0;
	wnd.cbWndExtra		= 0;
	wnd.hbrBackground	= 0;
	wnd.hCursor			= LoadCursor(NULL, IDC_ARROW);
	wnd.hIcon			= LoadIcon(NULL, IDC_ICON);
	wnd.hInstance		= GetModuleHandle(0);
	wnd.lpfnWndProc		= WndProc;
	wnd.lpszClassName	= CLASSNAME;
	wnd.lpszMenuName	= MAKEINTRESOURCE(IDR_MENU);
	wnd.style			= CS_VREDRAW | CS_HREDRAW;

	if(!RegisterClass(&wnd))
		return GB_EMU_ERROR;

	g_hWnd = CreateWindow(CLASSNAME, WINDOWNAME, WS_OVERLAPPEDWINDOW &
							~WS_MAXIMIZEBOX, CW_USEDEFAULT, CW_USEDEFAULT,
							320, 288, NULL, NULL, GetModuleHandle(0), NULL);
	if(!g_hWnd)
		return GB_EMU_ERROR;

	ShowWindow(g_hWnd, SW_SHOW);
	UpdateWindow(g_hWnd);

	/* initialization okay */
	return GB_EMU_OK;
}

/*
 * win32_sys - Retrieves and dispatches pending window messages.
 *
 */
EMU_IMPORT void win32_sys( void ) {
	MSG msg;
	
	if(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {

		/* pass on to window procedure */
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	/* if we are not executing a ROM, cut down on mad polling */
	if(gb_emu_status() != GB_EMU_STATUS_EXECUTING)
		Sleep(10);
}

/*
 * win32_blt - Blts the DIB to the window.
 *
 */
EMU_IMPORT void win32_blt( void ) {
	/* this triggers a WM_PAINT */
	InvalidateRect(g_hWnd, NULL, FALSE);
}

/*
 * win32_plot - Sets a pixel in the DIB.
 *
 */
EMU_IMPORT void win32_plot( unsigned int p, int c ) {
/*	g_lpBits[p] = c; */

	g_lpBits[p * 3 + 0] = (c >> 16) & 0xFF;
	g_lpBits[p * 3 + 1] = (c >>  8) & 0xFF;
	g_lpBits[p * 3 + 2] = (c) & 0xFF;
}

/*
 *		*** End of EMU_IMPORT functions ***
 */

/*
 * WinMain - Program Entry point.
 *
 * For Win32 this is the entry point instead of main(). This is so we
 *	can compile the application as subsystem:windows. We can still allocate
 *	a console for the process later when the user actually turns it on.
 *
 * I want to keep the core emulator code completely ignorant of the host
 *	environment so that porting will not require polluting any of the actual
 *	emulator code with ifdefs.
 *	
 * The true entry point for the application is WinMainCRTStartup() which calls
 *	either WinMain or main depending on the selected subsystem.
 *
 */
int APIENTRY WinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR     lpCmdLine,
                     int       nCmdShow) {
#ifdef _DEBUG
	ShowConsole(SW_SHOW);
#endif
	/* just call the emulator's main() function */
	main(0, NULL);
}

/*
 * WndProc - Window procedure of main application window.
 *
 */
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam,
						 LPARAM lParam) {
	LPBYTE		lpROM;
	DWORD		dwSize;
	HMENU		hMenu;
	RECT		rt;
	HDC			hDC, hDCMem;
	PAINTSTRUCT ps;

	switch(uMsg) {
		case WM_COMMAND:
			switch(LOWORD(wParam)) {
				case IDM_FILE_LOAD_ROM:
					/* start emulator */
					if(lpROM = GetROMFile(&dwSize)) {
						if(GB_EMU_ERROR == gb_emu_start(lpROM, dwSize)) {
							/* do something! */
						}
						ReleaseROMFile(lpROM);
					}
					break;

				case IDM_FILE_EXIT:
					DestroyWindow(hWnd);
					break;

				case IDM_SETTINGS_PAUSE:
					hMenu = GetMenu(hWnd);
					if(GetMenuState(hMenu, IDM_SETTINGS_PAUSE, 0) & MF_CHECKED) {
						/* resume game */
						CheckMenuItem(hMenu, IDM_SETTINGS_PAUSE, MF_UNCHECKED);
						gb_emu_pause(0);
					} else {
						/* can only pause when executing a game */
						if(gb_emu_status() != GB_EMU_STATUS_EXECUTING)
							return 0;

						/* pause game */
						CheckMenuItem(hMenu, IDM_SETTINGS_PAUSE, MF_CHECKED);
						gb_emu_pause(1);
					}
					break;

				case IDM_SETTINGS_CONSOLE:
					hMenu = GetMenu(hWnd);
					if(GetMenuState(hMenu, IDM_SETTINGS_CONSOLE, 0) & MF_CHECKED) {
						CheckMenuItem(hMenu, IDM_SETTINGS_CONSOLE, MF_UNCHECKED);
						ShowConsole(SW_HIDE);
					} else {
						CheckMenuItem(hMenu, IDM_SETTINGS_CONSOLE, MF_CHECKED);
						ShowConsole(SW_SHOWNOACTIVATE);
					}
					break;

				case IDM_SETTINGS_CONTROLS:
					DialogBox(GetModuleHandle(0), MAKEINTRESOURCE(IDD_CONTROLS),
						hWnd, DlgControlProc);
					break;

				case IDM_HELP_ABOUT:
					DialogBox(GetModuleHandle(0), MAKEINTRESOURCE(IDD_ABOUT),
						hWnd, DlgAboutProc);
					break;
			}
			break;

		case WM_PAINT:
			GetClientRect(hWnd, &rt);
			hDC = BeginPaint(hWnd, &ps);
			hDCMem = CreateCompatibleDC(hDC);
			SelectObject(hDCMem, g_hBitmap);

			StretchBlt(hDC, 0, 0, (rt.right - rt.left),
				(rt.bottom - rt.top), hDCMem, 0, 0, 160, 144,
				SRCCOPY);
			ReleaseDC(hWnd, hDC);
			DeleteDC(hDCMem);
			EndPaint(hWnd, &ps);
			break;

		/* shut emulator down */
		case WM_DESTROY:
			gb_emu_shutdown();
			break;

		default:
			return DefWindowProc(hWnd, uMsg, wParam, lParam);
	}
	return 0;
}

/*
 * DlgAboutProc - Window procedure of About dialog.
 *
 */
BOOL CALLBACK DlgAboutProc( HWND hDlg, UINT uMsg, WPARAM wParam,
						   LPARAM lParam ) {
	TCHAR	szBuild[64];
	DWORD	dwTime, dwResID[2] = { IDB_BITMAP1, IDB_BITMAP2 };

	switch(uMsg) {
		case WM_INITDIALOG:
			CenterWindow(hDlg, GetParent(hDlg));
			
			/* build creation string */
			dwTime = ((PIMAGE_NT_HEADERS)((LPBYTE)GetModuleHandle(0) +
				((PIMAGE_DOS_HEADER)GetModuleHandle(0))->e_lfanew))->
				FileHeader.TimeDateStamp; 

			_sntprintf(szBuild, sizeof(szBuild), _T("Build %i %02i/%02i/%02i"),
				EMU_VERSION, gmtime(&dwTime)->tm_mon + 1, gmtime(&dwTime)->tm_mday,
				gmtime(&dwTime)->tm_year - 100);

			SetWindowText(GetDlgItem(hDlg, IDC_BUILD), szBuild);
			return TRUE;

		case WM_COMMAND:
			switch(LOWORD(wParam)) {
				case ID_OK:
				case IDCANCEL:
					EndDialog(hDlg, LOWORD(wParam));
					return TRUE;
			}
			break;
	}
	return FALSE;
}

/*
 * DlgControlProc - Window procedure of Controls dialog.
 *
 */
BOOL CALLBACK DlgControlProc( HWND hDlg, UINT uMsg, WPARAM wParam,
						   LPARAM lParam ) {
	switch(uMsg) {
		case WM_COMMAND:
			switch(LOWORD(wParam)) {
				case ID_OK:
				case IDCANCEL:
					EndDialog(hDlg, LOWORD(wParam));
					return TRUE;
			}
			break;
	}
	return FALSE;
}

/*
 * GetROMFile - Lets the user pick a ROM file and then loads it.
 *
 * @param lpFileSize
 *	A pointer that will receive the filesize of the selected ROM file,
 *	in bytes.
 *
 * @return
 *	A pointer to the selected ROM file in memory on success. If the user
 *	pressed 'Cancel' in the 'Open File' dialog, NULL is returned.
 *
 */
LPBYTE GetROMFile( LPDWORD lpFileSize ) {
	OPENFILENAME ofn;
	TCHAR szFile[260];
	HANDLE hFile;
	LPBYTE lpROM;
	DWORD dwRead;

	ZeroMemory(&ofn, sizeof(ofn));

	ofn.lStructSize		= sizeof(ofn);
	ofn.hwndOwner		= g_hWnd;
	ofn.lpstrFile		= szFile;
	ofn.lpstrFile[0]	= '\0';
	ofn.nMaxFile		= sizeof(szFile);
	ofn.lpstrFilter		= _T("Gameboy ROM Files\0*.gb\0All Files\0*.*\0");
	ofn.nFilterIndex	= 1;
	ofn.lpstrFileTitle	= NULL;
	ofn.nMaxFileTitle	= 0;
	ofn.lpstrInitialDir	= NULL;
	ofn.Flags			= OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

	if(!GetOpenFileName(&ofn))
		return NULL;

	/* load the file into memory */
	if((hFile = CreateFile(ofn.lpstrFile, GENERIC_READ, 0, NULL, OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL, NULL)) == INVALID_HANDLE_VALUE)
		return NULL;
	
	*lpFileSize = GetFileSize(hFile, NULL);
	if(!(lpROM = malloc(*lpFileSize)))
		return NULL;

	if(FALSE == ReadFile(hFile, lpROM, *lpFileSize, &dwRead, NULL)) {
		CloseHandle(hFile);
		free(lpROM);
		return NULL;
	}

	CloseHandle(hFile);
	return lpROM;
}

/*
 * ReleaseROMFile - Releases a ROM file returned by GetROMFile.
 *
 * @param lpROM
 *	A pointer to a ROM file in memory.
 *
 */
VOID ReleaseROMFile( LPBYTE lpROM ) {
	if(lpROM)
		free(lpROM);
}

/*
 * WinAPI control stuff for Hyperlink control etc.
 *
 */

typedef struct {
	WNDPROC	oldProc;
	HFONT	hFont;
	PTCHAR	lpUrl;
	HCURSOR	hCursor;
} Hyperlink_t;

LRESULT CALLBACK HyperlinkProc(HWND hWnd, UINT uMsg, WPARAM wParam,
							   LPARAM lParam ) {
	LOGFONT		lf;
	HFONT		hFont, hOld;
	TCHAR		szBuffer[128];
	HDC			hDC;
	PAINTSTRUCT	ps;
	RECT		rt;
	INT			nOldBG, nOldColor;
	Hyperlink_t *lpData = (Hyperlink_t*) GetWindowLong(hWnd, GWL_USERDATA);


	switch(uMsg) {
		case WM_NCDESTROY:
			SetWindowLong(hWnd, GWL_WNDPROC, (LONG)lpData->oldProc);
			if(lpData->hFont)
				DeleteObject(lpData->hFont);
			if(lpData->lpUrl)
				VirtualFree(lpData->lpUrl, 0, MEM_RELEASE);
			VirtualFree(lpData, 0, MEM_RELEASE);
			return 0;

		case WM_SETFONT:
			if(!(hFont = (HFONT)wParam))
				hFont = (HFONT)GetStockObject(SYSTEM_FONT);
			GetObject(hFont, sizeof(LOGFONT), &lf);
			lf.lfUnderline = TRUE;

			if(lpData->hFont)
				DeleteObject(lpData->hFont);
			lpData->hFont = CreateFontIndirect(&lf);
			return 0;

		case WM_SETCURSOR:
			/* IDC_HAND */
			if(!lpData->hCursor)
				lpData->hCursor = LoadCursor(NULL, MAKEINTRESOURCE(32649));
			SetCursor(lpData->hCursor);
			return 1;

		case WM_LBUTTONUP:
			ShellExecute(NULL, _T("open"), lpData->lpUrl, NULL, NULL,
				SW_SHOWNORMAL);
			break;

		case WM_PAINT:
			hDC = BeginPaint(hWnd, &ps);
			GetWindowText(hWnd, szBuffer, sizeof(szBuffer));
			GetClientRect(hWnd, &rt);
			nOldBG = SetBkMode(hDC,TRANSPARENT);
			nOldColor = SetTextColor(hDC, RGB(0, 0, 255));
			hOld = (HFONT) SelectObject(hDC, lpData->hFont);
			DrawText(hDC, szBuffer, -1, &rt, DT_SINGLELINE);
			SetTextColor(hDC, nOldColor);
			SetBkMode(hDC, nOldBG);
			SelectObject(hDC, hOld);
			EndPaint(hWnd, &ps);
			return 0;
	}
	return CallWindowProc(lpData->oldProc, hWnd, uMsg, wParam, lParam);
}

BOOL MakeURLControl( HWND hWnd, PTCHAR szUrl ) {
	HFONT	hFont;
	DWORD	dwStyle;
	Hyperlink_t *lpData = (Hyperlink_t*) VirtualAlloc(NULL, sizeof(Hyperlink_t),
							MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
	if(!lpData)
		return FALSE;

	memset(lpData, 0, sizeof(Hyperlink_t));

	if(!(lpData->lpUrl = (PTCHAR) VirtualAlloc(NULL, _tcslen(szUrl) + 1,
		MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE))) {
		VirtualFree(lpData, 0,MEM_RELEASE);
		return FALSE;
	}

	_tcscpy(lpData->lpUrl, szUrl);

	/* subclass control */
	dwStyle = GetWindowLong(hWnd, GWL_STYLE);
	dwStyle = dwStyle | SS_NOTIFY;

	SetWindowLong(hWnd, GWL_STYLE, (LONG)dwStyle);
	SetWindowLong(hWnd, GWL_USERDATA, (LONG)lpData);

	lpData->oldProc = (WNDPROC) SetWindowLong(hWnd, GWL_WNDPROC,
		(LONG)HyperlinkProc );

	hFont = (HFONT) SendMessage(hWnd, WM_GETFONT, 0, 0);
	SendMessage(hWnd, WM_SETFONT, (WPARAM)hFont, 0);

	return TRUE;
}

/* Animation control */
typedef struct {
	HBITMAP *hBitmap;
	INT		nNumBitmap;
	INT		nSpeed;
	INT		nIndex;
	WNDPROC	oldProc;
} AnimCtrl_t;

LRESULT CALLBACK AnimProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	PAINTSTRUCT	ps;
	HDC			hDCMem;
	HBITMAP		hOld;
	RECT		rt;
	AnimCtrl_t	*lpData = (AnimCtrl_t*) GetWindowLong(hWnd, GWL_USERDATA);

	switch(uMsg) {
		case WM_NCDESTROY:
			SetWindowLong(hWnd, GWL_WNDPROC, (LONG)lpData->oldProc);
			VirtualFree(lpData, 0, MEM_RELEASE);
			return 0;

		case WM_TIMER:
			lpData->nIndex = (lpData->nIndex + 1) % lpData->nNumBitmap;
			InvalidateRect(hWnd, NULL, FALSE);
			return 0;

	  case WM_PAINT:
            BeginPaint(hWnd, &ps);
			hDCMem	= CreateCompatibleDC(ps.hdc);
			hOld	= (HBITMAP)SelectObject( hDCMem, lpData->hBitmap[lpData->nIndex] );

			GetClientRect( hWnd, &rt );
			BitBlt( ps.hdc, 0, 0, rt.right, rt.bottom, hDCMem, 0, 0, SRCCOPY );

			SelectObject( hDCMem, hOld );
			DeleteDC(hDCMem);
			EndPaint(hWnd, &ps);
			return 0;

	}

	return CallWindowProc(lpData->oldProc, hWnd, uMsg, wParam, lParam);
}

BOOL MakeAnimControl( HWND hWnd, DWORD *lpResId, INT nNumBitmap, INT nSpeed ) {
	INT i;
	AnimCtrl_t *lpData = (AnimCtrl_t*) VirtualAlloc(NULL, sizeof(Hyperlink_t),
							MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
	if(!lpData)
		return FALSE;

	lpData->nSpeed		= nSpeed;
	lpData->nIndex		= 0;
	lpData->nNumBitmap	= nNumBitmap;
	lpData->hBitmap		= VirtualAlloc(NULL, sizeof(HBITMAP) * nNumBitmap,
							MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
	if(!lpData->hBitmap) {
		VirtualFree(lpData, 0, MEM_RELEASE);
		return FALSE;
	}

	for(i = 0; i < nNumBitmap; i++) {
		if(!(lpData->hBitmap[i] = (HBITMAP)LoadImage(GetModuleHandle(0),
			MAKEINTRESOURCE(lpResId[i]), IMAGE_BITMAP, 0, 0, 0))) {
			VirtualFree(lpData->hBitmap, 0, MEM_RELEASE );
			VirtualFree(lpData, 0, MEM_RELEASE);
			return FALSE;
		}
	}

	/* subclass control */
	SetWindowLong(hWnd, GWL_USERDATA, (LONG)lpData);
	lpData->oldProc = (WNDPROC) SetWindowLong(hWnd, GWL_WNDPROC, (LONG)AnimProc );

	SetTimer(hWnd, 0, nSpeed, 0);

	return TRUE;
}

VOID CenterWindow( HWND hWnd, HWND hWndParent ) {
    RECT rect, rectP;
    int width, height;      
    int screenwidth, screenheight;
    int x, y;

    GetWindowRect(hWnd, &rect);
    GetWindowRect(hWndParent, &rectP);
        
    width  = rect.right  - rect.left;
    height = rect.bottom - rect.top;

    x = ((rectP.right-rectP.left) -  width) / 2 + rectP.left;
    y = ((rectP.bottom-rectP.top) - height) / 2 + rectP.top;

    screenwidth  = GetSystemMetrics(SM_CXSCREEN);
    screenheight = GetSystemMetrics(SM_CYSCREEN);
    
    if(x < 0) x = 0;
    if(y < 0) y = 0;
    if(x + width  > screenwidth)  x = screenwidth  - width;
    if(y + height > screenheight) y = screenheight - height;

    MoveWindow(hWnd, x, y, width, height, FALSE);
}



VOID ShowConsole( INT nShow ) {
	static HWND (WINAPI *pfnGetConsoleWindow)( VOID );
	static INT nInitConsole, hCrt;
	FILE *hf;

	if(!nInitConsole) {
		/* allocate a console for the process */
		AllocConsole();
		hCrt = _open_osfhandle((long)GetStdHandle(STD_OUTPUT_HANDLE),
			0x4000);
	   
		hf = _fdopen( hCrt, "w" );
		*stdout = *hf;
		setvbuf( stdout, NULL, _IONBF, 0 );

		/* I really should upgrade my 1998 Platform SDK I guess but I just
			can't be arsed... */
		pfnGetConsoleWindow = (HWND(WINAPI*)(VOID))GetProcAddress(
			LoadLibrary(_T("Kernel32")), "GetConsoleWindow");

		nInitConsole = 1;

		ShowWindow(pfnGetConsoleWindow(), SW_HIDE);
		SetForegroundWindow(g_hWnd);
	}
   
	ShowWindow(pfnGetConsoleWindow(), nShow); 
}
