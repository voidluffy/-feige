static char *tapp_id = 
	"@(#)Copyright (C) H.Shirouzu 1996-2001   tapp.cpp	Ver0.95";
/* ========================================================================
	Project  Name			: Win32 Lightweight  Class Library Test
	Module Name				: Application Frame Class
	Create					: 1996-06-01(Sat)
	Update					: 2001-12-06(Thu)
	Copyright				: H.Shirouzu
	Reference				: 
	======================================================================== */

#include "tlib.h"

TWin**		TApp::wndArray		= NULL;
int			TApp::wndCnt		= 0;
TWin*		TApp::preWnd		= NULL;
HINSTANCE	TApp::hI			= NULL;
char*		TApp::defaultClass	= "tapp";

HINSTANCE LoadCtl3d(void);

TApp::TApp(HINSTANCE _hI, LPSTR _cmdLine, int _nCmdShow)
{
	hI			= _hI;
	cmdLine		= _cmdLine;
	nCmdShow	= _nCmdShow;
	mainWnd		= NULL;

	::CoInitialize(NULL);
	hCtl3d = LoadCtl3d();
	::InitCommonControls();
}

TApp::~TApp()
{
	delete mainWnd;

	if (hCtl3d)
		::FreeLibrary(hCtl3d);
	::CoUninitialize();
}

int TApp::Run(void)
{
	MSG		msg;

	InitApp();
	InitWindow();

	while (::GetMessage(&msg, NULL, 0, 0))
	{
		if (PreProcMsg(&msg))
			continue;

		::TranslateMessage(&msg);
		::DispatchMessage(&msg);
	}

	return	msg.wParam;
}

BOOL TApp::PreProcMsg(MSG *msg)	// for TranslateAccel & IsDialogMessage
{
	for (HWND hWnd=msg->hwnd; hWnd != NULL; hWnd=::GetParent(hWnd))
	{
		TWin	*win = SearchWnd(hWnd);

		if (win != NULL)
			return	win->PreProcMsg(msg);
	}

	return	FALSE;
}

LRESULT CALLBACK TApp::WinProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	TWin *win = SearchWnd(hWnd);

	if (win)
		return	win->WinProc(uMsg, wParam, lParam);

	if ((win = preWnd) != NULL)
	{
		preWnd = NULL;
		AddWinByWnd(win, hWnd);
		return	win->WinProc(uMsg, wParam, lParam);
	}

	return	DefWindowProc(hWnd, uMsg, wParam, lParam);
}

void TApp::AddWin(TWin *win)
{
	preWnd = win;
}

void TApp::AddWinByWnd(TWin *win, HWND hWnd)
{
	win->hWnd = hWnd;
	AddWinSub(win);
}

void TApp::AddWinSub(TWin *win)
{
	if (SearchWnd(win->hWnd) != NULL)
		return;

#define BIG_ALLOC	100
	TWin **_wndArray = wndArray;

	if ((wndCnt % BIG_ALLOC) == 0)
		_wndArray = (TWin **)realloc(wndArray, sizeof(TWin *) * (wndCnt + BIG_ALLOC));

	if (_wndArray == NULL)
		return;

	(wndArray = _wndArray)[wndCnt++] = win;
	return;
}

void TApp::DelWin(TWin *win)
{
	int		index;

	if (SearchWndIndex(win, &index) != TRUE)
	{
		if (preWnd == win)
			preWnd = NULL;
		return;
	}

	memmove(wndArray + index, wndArray + index +1, sizeof(TWin *) * (--wndCnt - index));
//	wndArray = (TWin **)realloc(wndArray, sizeof(TWin *) * wndCnt);
}

BOOL TApp::InitApp(void)	// reference kwc
{
	WNDCLASS wc;

	memset(&wc, 0, sizeof(wc));
	wc.style			= (CS_BYTEALIGNCLIENT | CS_BYTEALIGNWINDOW | CS_DBLCLKS);
	wc.lpfnWndProc		= WinProc;
	wc.cbClsExtra 		= 0;
	wc.cbWndExtra		= 0;
	wc.hInstance		= hI;
	wc.hIcon			= NULL;
	wc.hCursor			= LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground	= NULL;
	wc.lpszMenuName		= NULL;
	wc.lpszClassName	= defaultClass;

	if (::FindWindow(defaultClass, NULL) == NULL)
	{
		if (::RegisterClass(&wc) == 0)
			return FALSE;
	}

	return	TRUE;
}

TWin* TApp::SearchWnd(HWND hWnd)	//Ç¢Ç∏ÇÍÇ®Ç¢Ç®Ç¢ÇQï™íTçıÇ…Ç≈Ç‡...
{
	for (int cnt=0; cnt < wndCnt; cnt++)
		if (wndArray[cnt]->hWnd == hWnd)
			return	wndArray[cnt];

	return	NULL;
}

BOOL TApp::SearchWndIndex(TWin *win, int *index)
{
	for (*index=0; *index < wndCnt; (*index)++)
		if (wndArray[*index] == win)
			return	TRUE;

	return	FALSE;
}

#if 0
IsXpManifest(void)
{
	static	BOOL	done = FALSE;
	static	BOOL	isXpManifest = FALSE;

	if (done)
		return	isXpManifest;

	done = TRUE;

	HMODULE		hComctl;
	char		path[MAX_PATH];

	if ((hComctl = ::GetModuleHandle("comctl32")) == NULL)
		return	isXpManifest = FALSE;

	if (::GetModuleFileName(hComctl, path, sizeof(path)) == 0)
		return	isXpManifest = FALSE;

	GetFileVersionInfo();
	VerQueryValue();
	
}
#endif

HINSTANCE LoadCtl3d(void)
{
	if (IsNewShell())
		return	NULL;

	HINSTANCE	hCtl3d;
	BOOL (WINAPI *Ctl3dRegister)(HANDLE);
	BOOL (WINAPI *Ctl3dAutoSubclass)(HANDLE);

	hCtl3d = ::LoadLibrary("ctl3d32.dll");
	Ctl3dRegister = (BOOL (WINAPI *)(HANDLE))::GetProcAddress(hCtl3d, "Ctl3dRegister");
	Ctl3dAutoSubclass = (BOOL (WINAPI *)(HANDLE))::GetProcAddress(hCtl3d, "Ctl3dAutoSubclass");

	if (Ctl3dRegister && Ctl3dAutoSubclass)
	{
		Ctl3dRegister(TApp::hI);
		Ctl3dAutoSubclass(TApp::hI);
	}

	return	hCtl3d;
}

