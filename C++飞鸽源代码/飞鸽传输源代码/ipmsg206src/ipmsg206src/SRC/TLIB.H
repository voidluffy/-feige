/* @(#)Copyright (C) H.Shirouzu 1996-2004   tlib.h	Ver0.95 */
/* ========================================================================
	Project  Name			: Win32 Lightweight  Class Library Test
	Module Name				: Main Header
	Create					: 1996-06-01(Sat)
	Update					: 2004-01-01(Thu)
	Copyright				: H.Shirouzu
	Reference				: 
	======================================================================== */

#ifndef TLIB_H
#define TLIB_H

#ifndef STRICT
#define STRICT
#endif

/* for crypto api */
#define _WIN32_WINNT 0x0600

#include <windows.h>
#include <windowsx.h>

#include "commctrl.h"
#include <regstr.h>
#include <shlobj.h>

#define WINEXEC_ERR_MAX		31
#define TLIB_SLEEPTIMER		32000

#define IsNewShell()	(LOBYTE(LOWORD(GetVersion())) >= 4)
#define IsWin95()		(LOBYTE(LOWORD(GetVersion())) >= 4 && GetVersion() >= 0x80000000)
#define IsWinNT()		(LOBYTE(LOWORD(GetVersion())) >= 4 && GetVersion() < 0x80000000)
#define IsWin2K()		(LOBYTE(LOWORD(GetVersion())) >= 5 && GetVersion() < 0x80000000)
#define IsWinXP()		(LOBYTE(LOWORD(GetVersion())) >= 6 && GetVersion() < 0x80000000)
#define IsWin31()		(LOBYTE(LOWORD(GetVersion())) == 3 && HIBYTE(LOWORD(GetVersion())) < 20)
#define IsNT350()		(LOBYTE(LOWORD(GetVersion())) == 3 && HIBYTE(LOWORD(GetVersion())) == 50)

#define BUTTON_CLASS		"BUTTON"
#define COMBOBOX_CLASS		"COMBOBOX"
#define EDIT_CLASS			"EDIT"
#define LISTBOX_CLASS		"LISTBOX"
#define MDICLIENT_CLASS		"MDICLIENT"
#define SCROLLBAR_CLASS		"SCROLLBAR"
#define STATIC_CLASS		"STATIC"

struct WINPOS {
	int		x;
	int		y;
	int		cx;
	int		cy;
};

class TWin {
protected:
	RECT			rect;
	HACCEL			hAccel;
	TWin			*parent;
	BOOL			sleepBusy;	// for TWin::Sleep() only

public:
	TWin(TWin *_parent = NULL);
	virtual	~TWin();

	HWND			hWnd;

	virtual void	Show(int mode = SW_SHOWDEFAULT);
	virtual BOOL	Create(LPCSTR className=NULL, LPCSTR title="", DWORD style=(WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN), DWORD exStyle=0, HMENU hMenu=NULL);
	virtual	void	Destroy(void);

	virtual BOOL	EvCommand(WORD wNotifyCode, WORD wID, LPARAM hwndCtl);
	virtual BOOL	EvSysCommand(WPARAM uCmdType, POINTS pos);
	virtual BOOL	EvCreate(LPARAM lParam);
	virtual BOOL	EvClose(void);
	virtual BOOL	EvNcDestroy(void);
	virtual BOOL	EvQueryEndSession(BOOL nSession, BOOL nLogOut);
	virtual BOOL	EvEndSession(BOOL nSession, BOOL nLogOut);
	virtual BOOL	EvQueryOpen(void);
	virtual BOOL	EvPaint(void);
	virtual BOOL	EvNcPaint(HRGN hRgn);
	virtual BOOL	EvSize(UINT fwSizeType, WORD nWidth, WORD nHeight);
	virtual BOOL	EvGetMinMaxInfo(MINMAXINFO *info);
	virtual BOOL	EvTimer(WPARAM timerID, TIMERPROC proc);
	virtual BOOL	EvSetCursor(HWND cursorWnd, WORD nHitTest, WORD wMouseMsg);
	virtual BOOL	EvMouseMove(UINT fwKeys, POINTS pos);
	virtual BOOL	EvNcHitTest(POINTS pos, LRESULT *result);
	virtual BOOL	EvMeasureItem(UINT ctlID, MEASUREITEMSTRUCT *lpMis);
	virtual BOOL	EvDrawItem(UINT ctlID, DRAWITEMSTRUCT *lpDis);
	virtual BOOL	EvMenuSelect(UINT uItem, UINT fuFlag, HMENU hMenu);
	virtual BOOL	EvDropFiles(HDROP hDrop);
	virtual BOOL	EvNotify(UINT ctlID, NMHDR *pNmHdr);
	virtual BOOL	EvContextMenu(HWND childWnd, POINTS pos);
	virtual BOOL	EvHotKey(int hotKey);

	virtual BOOL	EventScroll(UINT uMsg, int nCode, int nPos, HWND scrollBar);

	virtual BOOL	EventButton(UINT uMsg, int nHitTest, POINTS pos);
	virtual BOOL	EventInitMenu(UINT uMsg, HMENU hMenu, UINT uPos, BOOL fSystemMenu);
	virtual BOOL	EventCtlColor(UINT uMsg, HDC hDcCtl, HWND hWndCtl, HBRUSH *result);
	virtual BOOL	EventFocus(UINT uMsg, HWND focusWnd);
	virtual BOOL	EventUser(UINT uMsg, WPARAM wParam, LPARAM lParam);

	virtual UINT	GetDlgItemText(int ctlId, LPTSTR buf, int len);
	virtual BOOL	SetDlgItemText(int ctlId, LPTSTR buf);
	virtual HWND	GetDlgItem(int ctlId);
	virtual	int		MessageBox(LPCTSTR msg, LPCTSTR title="msg", UINT style=MB_OK);
	virtual BOOL	BringWindowToTop(void);
	virtual BOOL	SetForegroundWindow(void);
	virtual BOOL	SetForceForegroundWindow(void);
	virtual BOOL	PostMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
	virtual LRESULT	SendMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
	virtual LONG	SendDlgItemMessage(int ctlId, UINT uMsg, WPARAM wParam, LPARAM lParam);
	virtual BOOL	GetWindowRect(RECT *rect);
	virtual HWND	SetActiveWindow(void);
	virtual BOOL	SetWindowText(char *text);
	virtual LONG	SetWindowLong(int index, LONG val);
	virtual WORD	SetWindowWord(int index, WORD val);
	virtual LONG	GetWindowLong(int index);
	virtual WORD	GetWindowWord(int index);
	virtual TWin	*GetParent(void) { return parent; };
	virtual BOOL	MoveWindow(int x, int y, int cx, int cy, int bRepaint);
	virtual BOOL	Sleep(UINT mSec);
	virtual BOOL	Idle(void);

	virtual	BOOL	PreProcMsg(MSG *msg);
	virtual	LRESULT	WinProc(UINT uMsg, WPARAM wParam, LPARAM lParam);
	virtual	LRESULT	DefWindowProc(UINT uMsg, WPARAM wParam, LPARAM lParam);
};

class TDlg : public TWin {
protected:
	LPSTR	resName;
	UINT	resId;
	BOOL	modalFlg;

public:
	TDlg(LPSTR _name, TWin *_parent = NULL);
	TDlg(UINT	resid, TWin *_parent = NULL);
	virtual ~TDlg();

	virtual BOOL	Create(HINSTANCE hI = NULL);
	virtual	void	Destroy(void);
	virtual int		Exec(void);
	virtual void	EndDialog(int);
	UINT			ResId(void) { return resId; }

	virtual BOOL	EvCreate(LPARAM lParam);
	virtual BOOL	EvCommand(WORD wNotifyCode, WORD wID, LPARAM hwndCtl);
	virtual BOOL	EvSysCommand(WPARAM uCmdType, POINTS pos);
	virtual BOOL	EvQueryOpen(void);

	virtual	BOOL	PreProcMsg(MSG *msg);
	virtual LRESULT	WinProc(UINT uMsg, WPARAM wParam, LPARAM lParam);
};

class TSubClass : public TWin {
protected:
	WNDPROC		oldProc;

public:
	TSubClass(TWin *_parent = NULL);

	virtual BOOL	CreateByWnd(HWND _hWnd);
	virtual	LRESULT	DefWindowProc(UINT uMsg, WPARAM wParam, LPARAM lParam);
};

class TSubClassCtl : public TSubClass {
protected:
public:
	TSubClassCtl(TWin *_parent);

	virtual	BOOL	PreProcMsg(MSG *msg);
};

class TApp {
protected:
	static TWin			**wndArray;
	static int			wndCnt;
	static TWin			*SearchWnd(HWND hW);
	static BOOL			SearchWndIndex(TWin *win, int *index);
	static void			AddWinSub(TWin *win);

	HINSTANCE			hCtl3d;
	LPSTR				cmdLine;
	int					nCmdShow;
	TWin				*mainWnd;

	virtual BOOL		InitApp(void);

public:
	TApp(HINSTANCE _hI, LPSTR _cmdLine, int _nCmdShow);
	virtual ~TApp();
	virtual void		InitWindow() = 0;
	virtual int			Run();

	static	BOOL		PreProcMsg(MSG *msg);
	static TWin			*preWnd;
	static char			*defaultClass;
	static HINSTANCE	hI;
	static void			AddWin(TWin *win);
	static void			AddWinByWnd(TWin *win, HWND hWnd);
	static void			DelWin(TWin *win);
	static LRESULT CALLBACK WinProc(HWND hW, UINT uMsg, WPARAM wParam, LPARAM lParam);
};

struct TListObj {
	TListObj *prior, *next;
};

class TList {
protected:
	TListObj	top;

public:
	TList(void);
	virtual void		AddObj(TListObj *obj);
	static  void		DelObj(TListObj *obj);
	virtual TListObj	*TopObj(void);
	virtual TListObj	*NextObj(TListObj *obj);
};

#define FREE_LIST	0
#define USED_LIST	1
#define	RLIST_MAX	2
class TRecycleList {
protected:
	char	*data;
	TList	list[RLIST_MAX];

public:
	TRecycleList(int init_cnt, int size);
	~TRecycleList();
	TListObj *GetObj(int list_type);
	void PutObj(int list_type, TListObj *obj);
};

#define MAX_KEYARRAY	30

class TRegistry {
protected:
	HKEY	topKey;
	int		openCnt;
	HKEY	hKey[MAX_KEYARRAY];

public:
	TRegistry(LPCSTR company, LPSTR appName=NULL);
	TRegistry(HKEY top_key);
	~TRegistry();

	void	ChangeTopKey(HKEY topKey);
	BOOL	ChangeApp(LPCSTR company, LPSTR appName=NULL);
	BOOL	GetInt(LPCSTR key, int *val);
	BOOL	SetInt(LPCSTR key, int val);
	BOOL	GetLong(LPCSTR key, long *val);
	BOOL	SetLong(LPCSTR key, long val);
	BOOL	GetStr(LPCSTR key, LPSTR str, int size);
	BOOL	SetStr(LPCSTR key, LPCSTR str);
	BOOL	GetByte(LPCSTR key, BYTE *data, int *size);
	BOOL	SetByte(LPCSTR key, const BYTE *data, int size);
	BOOL	OpenKey(LPCSTR subKey, BOOL createFlg=FALSE);
	BOOL	CreateKey(LPCSTR subKey) { return OpenKey(subKey, TRUE); }
	BOOL	CloseKey(void);
	BOOL	DeleteKey(LPCSTR str);
	BOOL	DeleteValue(LPCSTR str);
	BOOL	EnumKey(DWORD cnt, LPSTR buf, int size);
	BOOL	EnumValue(DWORD cnt, LPSTR buf, int size, DWORD *type=NULL);
	BOOL	DeleteChildTree(LPSTR subkey=NULL);
};

#endif
