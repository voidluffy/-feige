static char *miscdlg_id = 
	"@(#)Copyright (C) H.Shirouzu 1996-2004   miscdlg.cpp	Ver2.05";
/* ========================================================================
	Project  Name			: IP Messenger for Win32
	Module Name				: Misc Dialog
	Create					: 1996-12-15(Sun)
	Update					: 2004-01-01(Thu)
	Copyright				: H.Shirouzu
	Reference				: 
	======================================================================== */

#include "tlib.h"
#include "resource.h"
#include "ipmsg.h"
#include "msgstr.h"
#include "blowfish.h"

int TMsgDlg::createCnt = 0;

/*
	TMsgDlgは、制約の多いMessageBox()の代用として作成
*/
TMsgDlg::TMsgDlg(TWin *_parent) : TListDlg(MESSAGE_DIALOG, _parent)
{
	hAccel = ::LoadAccelerators(TApp::hI, (LPCSTR)IPMSG_ACCEL);
}

/*
	終了ルーチン
*/
TMsgDlg::~TMsgDlg()
{
	createCnt--;
}

/*
	Window生成ルーチン
*/
BOOL TMsgDlg::Create(char *text, char *title, int _showMode)
{
	showMode = _showMode;

	if (TDlg::Create() == FALSE)
		return	FALSE;

	SetDlgItemText(MESSAGE_TEXT, text);
	SetWindowText(title);
	if (showMode == SW_SHOW) {
		::EnableWindow(hWnd, TRUE);
		::ShowWindow(hWnd, ((GetAsyncKeyState(VK_LBUTTON)|GetAsyncKeyState(VK_RBUTTON)|GetAsyncKeyState(VK_MBUTTON)) & 0x8000) ? SW_SHOWNOACTIVATE : SW_SHOW);
	}
	else {
		::ShowWindow(hWnd, SW_SHOWMINNOACTIVE);
		::EnableWindow(hWnd, TRUE);
	}

	if (strstr(text, "\r\n"))
	{
		WINDOWPLACEMENT wp;
		wp.length = sizeof(wp);
		::GetWindowPlacement(GetDlgItem(MESSAGE_TEXT), &wp);
		wp.rcNormalPosition.top -= ::GetSystemMetrics(SM_CYCAPTION) / 4;
		::SetWindowPlacement(GetDlgItem(MESSAGE_TEXT), &wp);
	}
	return	TRUE;
}

/*
	WM_COMMAND CallBack
*/
BOOL TMsgDlg::EvCommand(WORD wNotifyCode, WORD wID, LPARAM hWndCtl)
{
	switch (wID)
	{
	case MISC_ACCEL:
	case HIDE_ACCEL:
		::PostMessage(GetMainWnd(), WM_COMMAND, wID, 0);
		return	TRUE;

	case IDOK:
	case IDCANCEL:
		EndDialog(FALSE);
		::PostMessage(GetMainWnd(), WM_MSGDLG_EXIT, (WPARAM)0, (LPARAM)this);
		return	TRUE;
	}
	return	FALSE;
}

/*
	Window 生成時の CallBack。Screenの中心を起点として、すでに開いている
	TMsgDlgの枚数 * Caption Sizeを ずらして表示
*/
BOOL TMsgDlg::EvCreate(LPARAM lParam)
{
	SetDlgIcon(hWnd);

	if (rect.left == CW_USEDEFAULT)
	{
		GetWindowRect(&rect);
		int xsize = rect.right - rect.left, ysize = rect.bottom - rect.top;
		int	cx = ::GetSystemMetrics(SM_CXFULLSCREEN), cy = ::GetSystemMetrics(SM_CYFULLSCREEN);
		int	x = (cx - xsize)/2 + createCnt * ::GetSystemMetrics(SM_CYCAPTION);
		int y = (cy - ysize)/2 + createCnt * ::GetSystemMetrics(SM_CYCAPTION);

		MoveWindow((x < 0) ? 0 : x % (cx - xsize), (y < 0) ? 0 : y % (cy - ysize), xsize, ysize, FALSE);
	}

	createCnt++;
	return	TRUE;
}

/*
	About Dialog初期化処理
*/
TAboutDlg::TAboutDlg(TWin *_parent) : TDlg(ABOUT_DIALOG, _parent)
{
}

/*
	Window 生成時の CallBack
*/
BOOL TAboutDlg::EvCreate(LPARAM lParam)
{
	SetDlgIcon(hWnd);

	if (rect.left == CW_USEDEFAULT)
	{
		GetWindowRect(&rect);
		int xsize = rect.right - rect.left, ysize = rect.bottom - rect.top;
		int	cx = ::GetSystemMetrics(SM_CXFULLSCREEN), cy = ::GetSystemMetrics(SM_CYFULLSCREEN);
		int	x = (cx - xsize)/2;
		int y = (cy - ysize)/2;

		MoveWindow((x < 0) ? 0 : x % (cx - xsize), (y < 0) ? 0 : y % (cy - ysize), xsize, ysize, FALSE);
	}
	else
		MoveWindow(rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top, FALSE);

	return	TRUE;
}

BOOL TAboutDlg::EvCommand(WORD wNotifyCode, WORD wID, LPARAM hWndCtl)
{
	switch (wID)
	{
	case IDOK:
	case IDCANCEL:
		EndDialog(TRUE);
		return	TRUE;

	case IPMSG_ICON: case IPMSGWEB_BUTTON:
		if (wID == IPMSGWEB_BUTTON || wNotifyCode == 1)
			::ShellExecute(NULL, NULL, IPMSGURL_MSGSTR, NULL, NULL, SW_SHOW);
		return	TRUE;
	}
	return	FALSE;
}

/*
	listview control の subclass化
	Focus を失ったときにも、選択色を変化させないための小細工
*/
#define INVALID_INDEX	-1
TListViewEx::TListViewEx(TWin *_parent) : TSubClassCtl(_parent)
{
	focus_index = INVALID_INDEX;
}

BOOL TListViewEx::EventFocus(UINT uMsg, HWND hFocusWnd)
{
	LV_ITEM	lvi;

	memset(&lvi, 0, sizeof(lvi));
	lvi.mask = LVIF_STATE;
	lvi.stateMask = LVIS_FOCUSED;
	int	maxItem = SendMessage(LVM_GETITEMCOUNT, 0, 0);
	int itemNo;

	for (itemNo=0; itemNo < maxItem; itemNo++)
		if (SendMessage(LVM_GETITEMSTATE, itemNo, (LPARAM)LVIS_FOCUSED) & LVIS_FOCUSED)
			break;

	if (uMsg == WM_SETFOCUS)
	{
		if (itemNo == maxItem) {
			lvi.state = LVIS_FOCUSED;
			if (focus_index == INVALID_INDEX)
				focus_index = 0;
			SendMessage(LVM_SETITEMSTATE, focus_index, (LPARAM)&lvi);
			SendMessage(LVM_SETSELECTIONMARK, 0, focus_index);
		}
		return	FALSE;
	}
	else {	// WM_KILLFOCUS
		if (itemNo != maxItem) {
			SendMessage(LVM_SETITEMSTATE, itemNo, (LPARAM)&lvi);
			focus_index = itemNo;
		}
		return	TRUE;	// WM_KILLFOCUS は伝えない
	}
}

BOOL TListViewEx::EventButton(UINT uMsg, int nHitTest, POINTS pos)
{
	switch (uMsg)
	{
	case WM_RBUTTONDOWN:
		return	TRUE;
	case WM_RBUTTONUP:
		::PostMessage(parent->hWnd, uMsg, nHitTest, *(LPARAM *)&pos);
		return	TRUE;
	}
	return	FALSE;
}

BOOL TListViewEx::EventUser(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if (focus_index == INVALID_INDEX)
		return	FALSE;

	switch (uMsg) {
	case LVM_INSERTITEM:
		if (((LV_ITEM *)lParam)->iItem <= focus_index)
			focus_index++;
		break;
	case LVM_DELETEITEM:
		if ((int)wParam == focus_index)
			focus_index = INVALID_INDEX;
		else if ((int)wParam < focus_index)
			focus_index--;
		break;
	case LVM_DELETEALLITEMS:
		focus_index = INVALID_INDEX;
		break;
	}
	return	FALSE;
}

/*
	edit control の subclass化
*/
TEditSub::TEditSub(Cfg *_cfg, TWin *_parent) : TSubClassCtl(_parent)
{
	cfg = _cfg;
}

BOOL TEditSub::EventButton(UINT uMsg, int nHitTest, POINTS pos)
{
	switch (uMsg)
	{
	case WM_LBUTTONDBLCLK:
		PostMessage(WM_EDIT_DBLCLK, 0, 0);
		break;
	}
	return	FALSE;
}

BOOL TEditSub::EventUser(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_EDIT_DBLCLK:
		DWORD	startStr, endStr;

		SendMessage(EM_GETSEL, (WPARAM)&startStr, (LPARAM)&endStr);
		if (startStr != endStr && endStr < MAX_UDPBUF)
		{
			char	*url_ptr, *buf = new char[MAX_UDPBUF], proto[MAX_NAMEBUF];
			UrlObj	*obj = NULL;
			BOOL	modify = FALSE;

#ifdef JAPANESE
			char	*kspace = "　";
#define IS_KSPACE(c1,c2)	(kspace[0] == c1 && kspace[1] == c2)
#else
#define IS_KSPACE(c1,c2)	(0)		// 常に偽
#endif
			SendMessage(WM_GETTEXT, (WPARAM)MAX_UDPBUF, (LPARAM)buf);

			// WinXP の EDITBOX では '?' や ')' や '`'が選択区切りに。
			// こっそりそんなとこ変えるなよ...
			// XP SP1 ではさらに記号全般が区切り文字になった模様。やれやれ。
			if (startStr > 0) {
				for ( ; startStr > 0; startStr--, modify=TRUE) {
					char	ch = buf[startStr - 1];
					char	pre_ch = startStr >= 2 ? buf[startStr -2] : '\0';
					if (strchr("\r\n\t ", ch) || IS_KSPACE(pre_ch, ch))
						break;
				}
			}
			if (endStr -2 < startStr || !IS_KSPACE(buf[endStr -2], buf[endStr -1])) {
				for ( ; buf[endStr-1] != '\0' && buf[endStr-1] != ' '; endStr++, modify=TRUE) {
					char	ch = buf[endStr];
					char	nxt_ch = buf[endStr] != '\0' ? buf[endStr +1] : '\0';
					if (strchr("\r\n\t ", ch) || IS_KSPACE(ch, nxt_ch))
						break;
				}
			}
			if (modify)
				SendMessage(EM_SETSEL, (WPARAM)startStr, (LPARAM)endStr);
			memmove(buf, buf + startStr, (int)(endStr - startStr));
			buf[endStr - startStr] = 0;

			if ((url_ptr = strstr(buf, URL_STR)) && (url_ptr - buf) < sizeof(proto))
			{
				memcpy(proto, buf, url_ptr - buf), proto[url_ptr - buf] = 0;
				for (int cnt=0; proto[cnt]; cnt++)
				{
					if ((obj = SearchUrlObj(&cfg->urlList, proto + cnt)) != NULL)
					{
						url_ptr = buf + cnt;
						break;
					}
				}
			}
			if (obj && *obj->program)
			{
				if ((int)::ShellExecute(NULL, NULL, obj->program, url_ptr ? url_ptr : buf, NULL, SW_SHOW) <= WINEXEC_ERR_MAX)
					MessageBox(obj->program, CANTEXEC_MSGSTR, MB_OK|MB_ICONINFORMATION);
			}
			else if (!url_ptr && cfg->ShellExec || url_ptr && cfg->DefaultUrl)
				::ShellExecute(NULL, NULL, url_ptr ? url_ptr : buf, NULL, NULL, SW_SHOW);
			delete	[] buf;
		}
		return	TRUE;
	}
	return	FALSE;
}


/*
	static control の subclass化
*/
TSeparateSub::TSeparateSub(TWin *_parent) : TSubClassCtl(_parent)
{
	hCursor = ::LoadCursor(NULL, IDC_SIZENS);
}

BOOL TSeparateSub::EvSetCursor(HWND cursorWnd, WORD nHitTest, WORD wMouseMsg)
{
	::SetCursor(hCursor);
	return	TRUE;
}

BOOL TSeparateSub::EvNcHitTest(POINTS pos, LRESULT *result)
{
	*result = HTCLIENT;
	return	TRUE;	// きちんとイベントを飛ばすため(win3.1/nt3.51)
}

BOOL TSeparateSub::EventButton(UINT uMsg, int nHitTest, POINTS pos)
{
	switch (uMsg)
	{
	case WM_LBUTTONDOWN:
		parent->SendMessage(WM_SENDDLG_RESIZE, 0, 0);
		return	TRUE;
	}
	return	FALSE;
}


/*
	不在通知文設定ダイアログ
*/
extern char	*DefaultAbsence[], *DefaultAbsenceHead[];

TAbsenceDlg::TAbsenceDlg(Cfg *_cfg, TWin *_parent) : TDlg(ABSENCE_DIALOG, _parent)
{
	cfg = _cfg;
	hAccel = ::LoadAccelerators(TApp::hI, (LPCSTR)IPMSG_ACCEL);
}

TAbsenceDlg::~TAbsenceDlg()
{
}

BOOL TAbsenceDlg::EvCreate(LPARAM lParam)
{
	SetDlgIcon(hWnd);

	if (rect.left == CW_USEDEFAULT)
	{
		DWORD	val = GetMessagePos();
		POINTS	pos = MAKEPOINTS(val);

		GetWindowRect(&rect);
		int cx = ::GetSystemMetrics(SM_CXFULLSCREEN), cy = ::GetSystemMetrics(SM_CYFULLSCREEN);
		int xsize = rect.right - rect.left, ysize = rect.bottom - rect.top;
		int x = pos.x - xsize / 2, y = pos.y - ysize;

		if (x + xsize > cx)
			x = cx - xsize;
		if (y + ysize > cy)
			y = cy - ysize;

		MoveWindow(x > 0 ? x : 0, y > 0 ? y : 0, xsize, ysize, FALSE);
	}
	else
		MoveWindow(rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top, FALSE);

	typedef char MaxBuf[MAX_PATH];
	typedef char MaxHead[MAX_NAMEBUF];
	tmpAbsenceStr = new MaxBuf[cfg->AbsenceMax];
	tmpAbsenceHead = new MaxHead[cfg->AbsenceMax];

	SetData();
	return	TRUE;
}

BOOL TAbsenceDlg::EvNcDestroy(void)
{
	delete [] tmpAbsenceHead;
	delete [] tmpAbsenceStr;
	return	TRUE;
}

BOOL TAbsenceDlg::EvCommand(WORD wNotifyCode, WORD wID, LPARAM hWndCtl)
{
	switch (wID)
	{
	case IDOK:
		GetData();
		cfg->WriteRegistry(CFG_ABSENCE);
		cfg->AbsenceCheck = FALSE;
		::PostMessage(GetMainWnd(), WM_COMMAND, MENU_ABSENCE, 0);
		EndDialog(TRUE);
		return	TRUE;

	case SET_BUTTON:
		GetData();
		cfg->WriteRegistry(CFG_ABSENCE);
		if (cfg->AbsenceCheck)
		{
			cfg->AbsenceCheck = FALSE;
			::PostMessage(GetMainWnd(), WM_COMMAND, MENU_ABSENCE, 0);
		}
		EndDialog(FALSE);
		return	TRUE;

	case IDCANCEL:
		EndDialog(FALSE);
		return	TRUE;

	case ABSENCEINIT_BUTTON:
		SetDlgItemText(ABSENCEHEAD_EDIT, DefaultAbsenceHead[currentChoice]);
		SetDlgItemText(ABSENCE_EDIT, DefaultAbsence[currentChoice]);
		return	TRUE;

	case HIDE_ACCEL:
		::PostMessage(GetMainWnd(), WM_HIDE_CHILDWIN, 0, 0);
		return	TRUE;

	case ABSENCE_LIST:
		if (wNotifyCode == LBN_SELCHANGE)
		{
			int		index;

			if ((index = (int)SendDlgItemMessage(ABSENCE_LIST, LB_GETCURSEL, 0, 0)) != LB_ERR)
			{
				char	oldAbsenceHead[MAX_NAMEBUF];
				strcpy(oldAbsenceHead, tmpAbsenceHead[currentChoice]);
				GetDlgItemText(ABSENCEHEAD_EDIT, tmpAbsenceHead[currentChoice], MAX_NAMEBUF);
				GetDlgItemText(ABSENCE_EDIT, tmpAbsenceStr[currentChoice], MAX_PATH);
				if (strcmp(oldAbsenceHead, tmpAbsenceHead[currentChoice]))
				{
					SendDlgItemMessage(ABSENCE_LIST, LB_DELETESTRING, currentChoice, 0);
					SendDlgItemMessage(ABSENCE_LIST, LB_INSERTSTRING, currentChoice, (LPARAM)tmpAbsenceHead[currentChoice]);
					if (currentChoice == index)
					{
						SendDlgItemMessage(ABSENCE_LIST, LB_SETCURSEL, currentChoice, 0);
						return	TRUE;
					}
				}
				currentChoice = index;
				PostMessage(WM_DELAYSETTEXT, 0, 0);
			}
		}
		else if (wNotifyCode == LBN_DBLCLK)
			PostMessage(WM_COMMAND, IDOK, 0);
		return	TRUE;
	}

	return	FALSE;
}

/*
	User定義 Event CallBack
*/
BOOL TAbsenceDlg::EventUser(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_DELAYSETTEXT:
		{
			int		len = strlen(tmpAbsenceStr[currentChoice]);

			SetDlgItemText(ABSENCEHEAD_EDIT, tmpAbsenceHead[currentChoice]);
			SetDlgItemText(ABSENCE_EDIT, tmpAbsenceStr[currentChoice]);
			SendDlgItemMessage(ABSENCE_EDIT, EM_SETSEL, (WPARAM)len, (LPARAM)len);
		}
		return	TRUE;
	}

	return	FALSE;
}

void TAbsenceDlg::SetData(void)
{
	for (int cnt=0; cnt < cfg->AbsenceMax; cnt++)
	{
		strcpy(tmpAbsenceHead[cnt], cfg->AbsenceHead[cnt]);
		strcpy(tmpAbsenceStr[cnt], cfg->AbsenceStr[cnt]);
		SendDlgItemMessage(ABSENCE_LIST, LB_INSERTSTRING, (WPARAM)-1, (LPARAM)cfg->AbsenceHead[cnt]);
	}
	currentChoice = cfg->AbsenceChoice;
	PostMessage(WM_DELAYSETTEXT, 0, 0);
	SendDlgItemMessage(ABSENCE_LIST, LB_SETCURSEL, currentChoice, 0);
}

void TAbsenceDlg::GetData(void)
{
	GetDlgItemText(ABSENCEHEAD_EDIT, tmpAbsenceHead[currentChoice], MAX_NAMEBUF);
	GetDlgItemText(ABSENCE_EDIT, tmpAbsenceStr[currentChoice], MAX_PATH);
	for (int cnt=0; cnt < cfg->AbsenceMax; cnt++)
	{
		strcpy(cfg->AbsenceHead[cnt], tmpAbsenceHead[cnt]);
		strcpy(cfg->AbsenceStr[cnt], tmpAbsenceStr[cnt]);
	}
	cfg->AbsenceChoice = currentChoice;
}


/*
	ソート設定ダイアログ
*/
TSortDlg *TSortDlg::exclusiveWnd = NULL;

TSortDlg::TSortDlg(Cfg *_cfg, TWin *_parent) : TDlg(SORT_DIALOG, _parent)
{
	cfg = _cfg;
}

int TSortDlg::Exec(void)
{
	if (exclusiveWnd)
		return	exclusiveWnd->SetForegroundWindow(), FALSE;

	exclusiveWnd = this;
	return	TDlg::Exec();
}

BOOL TSortDlg::EvCommand(WORD wNotifyCode, WORD wID, LPARAM hWndCtl)
{
	switch (wID)
	{
	case IDOK: case SET_BUTTON:
		GetData();
		cfg->WriteRegistry(CFG_GENERAL);
		if (wID == IDOK)
			EndDialog(TRUE), exclusiveWnd = NULL;
		return	TRUE;

	case IDCANCEL:
		EndDialog(FALSE);
		exclusiveWnd = NULL;
		return	TRUE;
	}

	return	FALSE;
}

BOOL TSortDlg::EvCreate(LPARAM lParam)
{
	GetWindowRect(&rect);
	int cx = ::GetSystemMetrics(SM_CXFULLSCREEN), cy = ::GetSystemMetrics(SM_CYFULLSCREEN);
	int x = rect.left + 50, y = rect.top + 20;
	int xsize = rect.right - rect.left, ysize = rect.bottom - rect.top;
	if (x + xsize > cx)
		x = cx - xsize;
	if (y + ysize > cy)
		y = cy - ysize;

	MoveWindow(x, y, xsize, ysize, FALSE);
	SetData();

	return	TRUE;
}

void TSortDlg::SetData(void)
{
	SendDlgItemMessage(GROUPDISP_CHECK, BM_SETCHECK, GetItem(cfg->ColumnItems, SW_GROUP), 0);
	SendDlgItemMessage(HOST_CHECK, BM_SETCHECK, GetItem(cfg->ColumnItems, SW_HOST), 0);
	SendDlgItemMessage(LOGON_CHECK, BM_SETCHECK, GetItem(cfg->ColumnItems, SW_USER), 0);
	SendDlgItemMessage(PRIORITY_CHECK, BM_SETCHECK, GetItem(cfg->ColumnItems, SW_PRIORITY), 0);
	SendDlgItemMessage(IPADDR_CHECK, BM_SETCHECK, GetItem(cfg->ColumnItems, SW_IPADDR), 0);
	SendDlgItemMessage(GLIDLINE_CHECK, BM_SETCHECK, cfg->GlidLineCheck, 0);

	SendDlgItemMessage(GROUP_CHECK, BM_SETCHECK, !(cfg->Sort & IPMSG_NOGROUPSORTOPT), 0);
	SendDlgItemMessage(GROUPREV_CHECK, BM_SETCHECK, (cfg->Sort & IPMSG_GROUPREVSORTOPT) != 0, 0);
	SendDlgItemMessage(USER_RADIO, BM_SETCHECK, GET_MODE(cfg->Sort) == IPMSG_NAMESORT, 0);
	SendDlgItemMessage(HOST_RADIO, BM_SETCHECK, GET_MODE(cfg->Sort) == IPMSG_HOSTSORT, 0);
	SendDlgItemMessage(IPADDR_RADIO, BM_SETCHECK, GET_MODE(cfg->Sort) == IPMSG_IPADDRSORT, 0);
	SendDlgItemMessage(SUBREV_CHECK, BM_SETCHECK, (cfg->Sort & IPMSG_SUBREVSORTOPT) != 0, 0);
	SendDlgItemMessage(ICMP_CHECK, BM_SETCHECK, (cfg->Sort & IPMSG_ICMPSORTOPT) != 0, 0);
	SendDlgItemMessage(KANJI_CHECK, BM_SETCHECK, !(cfg->Sort & IPMSG_NOKANJISORTOPT), 0);
}

void TSortDlg::GetData(void)
{
	SetItem(&cfg->ColumnItems, SW_GROUP, SendDlgItemMessage(GROUPDISP_CHECK, BM_GETCHECK, 0, 0));
	SetItem(&cfg->ColumnItems, SW_HOST, SendDlgItemMessage(HOST_CHECK, BM_GETCHECK, 0, 0));
	SetItem(&cfg->ColumnItems, SW_USER, SendDlgItemMessage(LOGON_CHECK, BM_GETCHECK, 0, 0));
	SetItem(&cfg->ColumnItems, SW_PRIORITY, SendDlgItemMessage(PRIORITY_CHECK, BM_GETCHECK, 0, 0));
	SetItem(&cfg->ColumnItems, SW_IPADDR, SendDlgItemMessage(IPADDR_CHECK, BM_GETCHECK, 0, 0));
	cfg->GlidLineCheck = SendDlgItemMessage(GLIDLINE_CHECK, BM_GETCHECK, 0, 0);

	cfg->Sort = 0;

	if (SendDlgItemMessage(GROUP_CHECK, BM_GETCHECK, 0, 0) == 0)
		cfg->Sort |= IPMSG_NOGROUPSORTOPT;
	if (SendDlgItemMessage(GROUPREV_CHECK, BM_GETCHECK, 0, 0))
		cfg->Sort |= IPMSG_GROUPREVSORTOPT;
	if (SendDlgItemMessage(USER_RADIO, BM_GETCHECK, 0, 0))
		cfg->Sort |= IPMSG_NAMESORT;
	if (SendDlgItemMessage(HOST_RADIO, BM_GETCHECK, 0, 0))
		cfg->Sort |= IPMSG_HOSTSORT;
	if (SendDlgItemMessage(IPADDR_RADIO, BM_GETCHECK, 0, 0))
		cfg->Sort |= IPMSG_IPADDRSORT;
	if (SendDlgItemMessage(SUBREV_CHECK, BM_GETCHECK, 0, 0))
		cfg->Sort |= IPMSG_SUBREVSORTOPT;
	if (SendDlgItemMessage(ICMP_CHECK, BM_GETCHECK, 0, 0))
		cfg->Sort |= IPMSG_ICMPSORTOPT;
	if (SendDlgItemMessage(KANJI_CHECK, BM_GETCHECK, 0, 0) == 0)
		cfg->Sort |= IPMSG_NOKANJISORTOPT;
}

/*
	クリッカブルURL設定ダイアログ
*/
TUrlDlg::TUrlDlg(Cfg *_cfg, TWin *_parent) : TDlg(URL_DIALOG, _parent)
{
	cfg = _cfg;
	*currentProtocol = 0;
}

TUrlDlg::~TUrlDlg()
{
}

BOOL TUrlDlg::EvCreate(LPARAM lParam)
{
	GetWindowRect(&rect);
	int cx = ::GetSystemMetrics(SM_CXFULLSCREEN), cy = ::GetSystemMetrics(SM_CYFULLSCREEN);
	int x = rect.left + 10, y = rect.top + 50;
	int xsize = rect.right - rect.left, ysize = rect.bottom - rect.top;
	if (x + xsize > cx)
		x = cx - xsize;
	if (y + ysize > cy)
		y = cy - ysize;

	MoveWindow(x, y, xsize, ysize, FALSE);
	SetData();
	SendDlgItemMessage(URL_LIST, LB_SETCURSEL, 0, 0);
	EvCommand(0, URL_LIST, 0);

	return	TRUE;
}

BOOL TUrlDlg::EvNcDestroy(void)
{
	UrlObj *urlObj;

	while ((urlObj = (UrlObj *)tmpUrlList.TopObj()) != NULL)
	{
		tmpUrlList.DelObj(urlObj);
		delete urlObj;
	}
	return	TRUE;
}

BOOL TUrlDlg::EvCommand(WORD wNotifyCode, WORD wID, LPARAM hWndCtl)
{
	switch (wID)
	{
	case IDOK: case SET_BUTTON:
		GetData();
		cfg->WriteRegistry(CFG_CLICKURL);
		if (wID == IDOK)
			EndDialog(TRUE);
		return	TRUE;

	case IDCANCEL:
		EndDialog(FALSE);
		return	TRUE;

	case ADD_BUTTON:
		{
			char	protocol[MAX_LISTBUF], buf[MAX_LISTBUF];
			int		index;
			UrlObj	*obj;

			if ((index = (int)SendDlgItemMessage(URL_LIST, LB_GETCURSEL, 0, 0)) != LB_ERR && SendDlgItemMessage(URL_LIST, LB_GETTEXT, (WPARAM)index, (LPARAM)protocol) != LB_ERR && (obj = SearchUrlObj(&tmpUrlList, protocol)) != NULL)
			{
				wsprintf(buf, EXECPROGRAM_MSGSTR, protocol);
				OpenFileDlg(this).Exec(URL_EDIT, buf, OPENFILEPRG_MSGSTR);
			}
		}
		return	TRUE;

	case URL_LIST:
		{
			char	protocol[MAX_LISTBUF];
			int		index;
			UrlObj	*obj;

			if ((index = (int)SendDlgItemMessage(URL_LIST, LB_GETCURSEL, 0, 0)) != LB_ERR && SendDlgItemMessage(URL_LIST, LB_GETTEXT, (WPARAM)index, (LPARAM)protocol) != LB_ERR && (obj = SearchUrlObj(&tmpUrlList, protocol)) != NULL)
			{
				SetCurrentData();
				SetDlgItemText(URL_EDIT, obj->program);
				strncpyz(currentProtocol, protocol, sizeof(currentProtocol));
			}
		}
		return	TRUE;
	}

	return	FALSE;
}

void TUrlDlg::SetCurrentData(void)
{
	UrlObj	*obj;

	if ((obj = SearchUrlObj(&tmpUrlList, currentProtocol)) != NULL)
		GetDlgItemText(URL_EDIT, obj->program, sizeof(obj->program));
}

void TUrlDlg::SetData(void)
{
	for (UrlObj *obj = (UrlObj *)cfg->urlList.TopObj(); obj != NULL; obj = (UrlObj *)cfg->urlList.NextObj(obj))
	{
		UrlObj *tmp_obj = new UrlObj;
		strcpy(tmp_obj->protocol, obj->protocol);
		strcpy(tmp_obj->program, obj->program);
		tmpUrlList.AddObj(tmp_obj);

		SendDlgItemMessage(URL_LIST, LB_INSERTSTRING, (WPARAM)-1, (LPARAM)obj->protocol);
	}
	SendDlgItemMessage(DEFAULTURL_CHECK, BM_SETCHECK, cfg->DefaultUrl, 0);
	SendDlgItemMessage(SHELLEXEC_CHECK, BM_SETCHECK, cfg->ShellExec, 0);
}

void TUrlDlg::GetData(void)
{
	SetCurrentData();

	for (UrlObj *tmp_obj = (UrlObj *)tmpUrlList.TopObj(); tmp_obj != NULL; tmp_obj = (UrlObj *)tmpUrlList.NextObj(tmp_obj))
	{
		UrlObj *obj = SearchUrlObj(&cfg->urlList, tmp_obj->protocol);

		if (obj == NULL)
		{
			obj = new UrlObj;
			cfg->urlList.AddObj(obj);
			strcpy(obj->protocol, tmp_obj->protocol);
		}
		strcpy(obj->program, tmp_obj->program);
	}
	cfg->DefaultUrl = (int)SendDlgItemMessage(DEFAULTURL_CHECK, BM_GETCHECK, 0, 0);
	cfg->ShellExec = (int)SendDlgItemMessage(SHELLEXEC_CHECK, BM_GETCHECK, 0, 0);
}


// パスワード確認用
TPasswordDlg::TPasswordDlg(Cfg *_cfg, TWin *_parent) : TDlg(PASSWORD_DIALOG, _parent)
{
	cfg = _cfg;
	outbuf = NULL;
}

// 単なるパスワード入力用
TPasswordDlg::TPasswordDlg(char *_outbuf, TWin *_parent) : TDlg(PASSWORD_DIALOG, _parent)
{
	cfg = NULL;
	outbuf = _outbuf;
}

BOOL TPasswordDlg::EvCommand(WORD wNotifyCode, WORD wID, LPARAM hWndCtl)
{
	switch (wID)
	{
	case IDOK:
		{
			char	_buf[MAX_NAMEBUF];
			char	*buf = outbuf ? outbuf : _buf;

			GetDlgItemText(PASSWORD_EDIT, buf, MAX_NAMEBUF);
			if (cfg)
			{
				if (CheckPassword(cfg->PasswordStr, buf))
					EndDialog(TRUE);
				else
					SetDlgItemText(PASSWORD_EDIT, ""), MessageBox(CANTAUTH_MSGSTR);
			}
			else EndDialog(TRUE);
		}
		return	TRUE;

	case IDCANCEL:
		EndDialog(FALSE);
		return	TRUE;
	}
	return	FALSE;
}

BOOL TPasswordDlg::EvCreate(LPARAM lParam)
{
	GetWindowRect(&rect);
	if (parent)
		MoveWindow(rect.left +100, rect.top +100, rect.right - rect.left, rect.bottom - rect.top, FALSE);
	else
	{
		GetWindowRect(&rect);
		int xsize = rect.right - rect.left, ysize = rect.bottom - rect.top;
		int	x = (::GetSystemMetrics(SM_CXFULLSCREEN) - xsize)/2;
		int y = (::GetSystemMetrics(SM_CYFULLSCREEN) - ysize)/2;
		MoveWindow(x, y, xsize, ysize, FALSE);
	}

	return	TRUE;
}

TPasswdChangeDlg::TPasswdChangeDlg(Cfg *_cfg, TWin *_parent) : TDlg(PASSWDCHANGE_DIALOG, _parent)
{
	cfg = _cfg;
}

BOOL TPasswdChangeDlg::EvCommand(WORD wNotifyCode, WORD wID, LPARAM hWndCtl)
{
	switch (wID)
	{
	case IDOK:
		char	buf[MAX_NAMEBUF], buf2[MAX_NAMEBUF];

		GetDlgItemText(OLDPASSWORD_EDIT, buf, sizeof(buf));
		if (CheckPassword(cfg->PasswordStr, buf))
		{
			GetDlgItemText(NEWPASSWORD_EDIT, buf, sizeof(buf));
			GetDlgItemText(NEWPASSWORD_EDIT2, buf2, sizeof(buf2));
			if (strcmp(buf, buf2) == 0)
				MakePassword(buf, cfg->PasswordStr);
			else
				return	MessageBox(NOTSAMEPASS_MSGSTR), TRUE;
			cfg->WriteRegistry(CFG_GENERAL);
			EndDialog(TRUE);
		}
		else
			SetDlgItemText(PASSWORD_EDIT, ""), MessageBox(CANTAUTH_MSGSTR);
		return	TRUE;

	case IDCANCEL:
		EndDialog(FALSE);
		return	TRUE;
	}
	return	FALSE;
}

BOOL TPasswdChangeDlg::EvCreate(LPARAM lParam)
{
	GetWindowRect(&rect);
	MoveWindow(rect.left +50, rect.top +100, rect.right - rect.left, rect.bottom - rect.top, FALSE);

	if (*cfg->PasswordStr == 0)
		::EnableWindow(GetDlgItem(OLDPASSWORD_EDIT), FALSE);

	return	TRUE;
}


BOOL CheckPassword(const char *cfgPasswd, const char *inputPasswd)
{
	char	buf[MAX_NAMEBUF];

	MakePassword(inputPasswd, buf);

	return	strcmp(buf, cfgPasswd) == 0 ? TRUE : FALSE;
}

void MakePassword(const char *inputPasswd, char *outputPasswd)
{
	while (*inputPasswd)	// 可逆だぞ...DESライブラリがあればいいのに
		*outputPasswd++ = ((~*inputPasswd++) & 0x7f); //レジストリのストリング型は８ビット目を嫌う(;_;)

	*outputPasswd = 0;
}


/*
	nul文字を必ず付与する strncpy
*/
char *strncpyz(char *dest, const char *src, int num)
{
	char	*sv = dest;

	while (num-- > 0)
		if ((*dest++ = *src++) == '\0')
			return	sv;

	if (sv != dest)		// num > 0
		*(dest -1) = 0;
	return	sv;
}

/*
	大文字小文字を無視する strncmp
*/
int strncmpi(const char *str1, const char *str2, int num)
{
	for (int cnt=0; cnt < num; cnt++)
	{
		char	c1 = toupper(str1[cnt]), c2 = toupper(str2[cnt]);

		if (c1 == c2)
		{
			if (c1)
				continue;
			else
				return	0;
		}
		if (c1 > c2)
			return	1;
		else
			return	-1;
	}
	return	0;
}

/*
	URL検索ルーチン
*/
UrlObj *SearchUrlObj(TList *list, char *protocol)
{
	for (UrlObj *obj = (UrlObj *)list->TopObj(); obj != NULL; obj = (UrlObj *)list->NextObj(obj))
		if (stricmp(obj->protocol, protocol) == 0)
			return	obj;

	return	NULL;
}

/*
	ダイアログ用アイコン設定
*/
void SetDlgIcon(HWND hWnd)
{
	static HICON	oldHIcon = NULL;

	if (oldHIcon != TMainWin::GetIPMsgIcon())
	{
		oldHIcon = TMainWin::GetIPMsgIcon();
		::SetClassLong(hWnd, GCL_HICON, (LONG)oldHIcon);
	}
}

/*
	ログ記録用の HostEntry表示文字列
*/
void MakeListString(Cfg *cfg, HostSub *hostSub, THosts *hosts, char *buf)
{
	Host	*host;

	if ((host = hosts->GetHostByAddr(hostSub)) && IsSameHost(hostSub, &host->hostSub)
		/* || (host = cfg->priorityHosts.GetHostByName(hostSub)) != NULL */)
		MakeListString(cfg, host, buf);
	else {
		Host	host;

		memset(&host, 0, sizeof(host));
		host.hostSub = *hostSub;
		MakeListString(cfg, &host, buf);
	}
}

/*
	ログ記録用の HostEntry表示文字列
*/
void MakeListString(Cfg *cfg, Host *host, char *buf)
{
	char	*fmt = "%s ";

	if (*host->nickName)
	{
		if (cfg->NickNameCheck == IPMSG_NICKNAME)
			buf += wsprintf(buf, fmt, host->nickName);
		else {
			char	tmp_name[MAX_LISTBUF];
			wsprintf(tmp_name, "%s[%s]", host->nickName, host->hostSub.userName);
			buf += wsprintf(buf, fmt, tmp_name);
		}
	}
	else
		buf += wsprintf(buf, fmt, host->hostSub.userName);

	if (host->hostStatus & IPMSG_ABSENCEOPT)
		*buf++ = '*';

	buf += wsprintf(buf, "(%s%s%s", host->groupName, *host->groupName ? "/":"", host->hostSub.hostName);

	if (cfg->IPAddrCheck)
		buf += wsprintf(buf, "/%s", inet_ntoa(*(LPIN_ADDR)&host->hostSub.addr));

	strcpy(buf, ")");
}

/*
	IME 制御
*/
BOOL SetImeOpenStatus(HWND hWnd, BOOL flg)
{
	BOOL ret = FALSE;

#if defined(JAPANESE) && defined(WIN32)
	HIMC hImc;

	if ((hImc = ImmGetContext(hWnd)) != NULL)
	{
		ret = ImmSetOpenStatus(hImc, flg);
		ImmReleaseContext(hWnd, hImc);
	}
#endif
	return	ret;
}

BOOL GetImeOpenStatus(HWND hWnd)
{
	BOOL ret = FALSE;

#if defined(JAPANESE) && defined(WIN32)
	HIMC hImc;

	if ((hImc = ImmGetContext(hWnd)) != NULL)
	{
		ret = ImmGetOpenStatus(hImc);
		ImmReleaseContext(hWnd, hImc);
	}
#endif
	return	ret;
}

/*
	ホットキー設定
*/
BOOL SetHotKey(Cfg *cfg)
{
	if (cfg->HotKeyCheck)
	{
		RegisterHotKey(GetMainWnd(), WM_SENDDLG_OPEN, cfg->HotKeyModify, cfg->HotKeySend);
		RegisterHotKey(GetMainWnd(), WM_RECVDLG_OPEN, cfg->HotKeyModify, cfg->HotKeyRecv);
		RegisterHotKey(GetMainWnd(), WM_DELMISCDLG, cfg->HotKeyModify, cfg->HotKeyMisc);
	}
	else {
		UnregisterHotKey(GetMainWnd(), WM_SENDDLG_OPEN);
		UnregisterHotKey(GetMainWnd(), WM_RECVDLG_OPEN);
		UnregisterHotKey(GetMainWnd(), WM_DELMISCDLG);
	}
	return	TRUE;
}

/*
	Host1 と Host2 が同一かどうかを比較
*/
BOOL IsSameHost(HostSub *hostSub1, HostSub *hostSub2)
{
	if (stricmp(hostSub1->hostName, hostSub2->hostName))
		return	FALSE;

	return	stricmp(hostSub1->userName, hostSub2->userName) ? FALSE : TRUE;
}

/*
	RECT -> WINPOS
*/
void RectToWinPos(const RECT *rect, WINPOS *wpos)
{
	wpos->x = rect->left, wpos->y = rect->top;
	wpos->cx = rect->right - rect->left;
	wpos->cy = rect->bottom - rect->top;
}

/*
	host array class
*/
THosts::THosts(void)
{
	hostCnt = 0;
	memset(array, 0, sizeof(array));
	for (int kind=0; kind < MAX_ARRAY; kind++)
		enable[kind] = FALSE;
}

THosts::~THosts()
{
	for (int kind=0; kind < MAX_ARRAY; kind++) {
		if (array[kind])
			free(array[kind]);
	}
}


int THosts::Cmp(HostSub *hostSub1, HostSub *hostSub2, Kind kind)
{
	switch (kind) {
	case NAME: case NAME_ADDR:
		{
			int	cmp;
			if (cmp = stricmp(hostSub1->userName, hostSub2->userName))
				return	cmp;
			if ((cmp = stricmp(hostSub1->hostName, hostSub2->hostName)) || kind == NAME)
				return	cmp;
		}	// if cmp == 0 && kind == NAME_ADDR, through...
	case ADDR:
		if (hostSub1->addr > hostSub2->addr)
			return	1;
		if (hostSub1->addr < hostSub2->addr)
			return	-1;
		if (hostSub1->portNo > hostSub2->portNo)
			return	1;
		if (hostSub1->portNo < hostSub2->portNo)
			return	-1;
		return	0;
	}
	return	-1;
}

Host *THosts::Search(Kind kind, HostSub *hostSub, int *insertIndex)
{
	int	min = 0, max = hostCnt -1, cmp, tmpIndex;

	if (insertIndex == NULL)
		insertIndex = &tmpIndex;

	while (min <= max)
	{
		*insertIndex = (min + max) / 2;

		if ((cmp = Cmp(hostSub, &array[kind][*insertIndex]->hostSub, kind)) == 0)
			return	array[kind][*insertIndex];
		else if (cmp > 0)
			min = *insertIndex +1;
		else
			max = *insertIndex -1;
	}

	*insertIndex = min;
	return	NULL;
}

BOOL THosts::AddHost(Host *host)
{
	int		insertIndex[MAX_ARRAY];

// すべてのインデックス種類での確認を先に行う
	for (int kind=0; kind < MAX_ARRAY; kind++) {
		if (enable[kind] == FALSE)
			continue;
		if (Search((Kind)kind, &host->hostSub, &insertIndex[kind]) != NULL)
			return	FALSE;
	}

#define BIG_ALLOC	100
	for (kind=0; kind < MAX_ARRAY; kind++) {
		if (enable[kind] == FALSE)
			continue;
		if ((hostCnt % BIG_ALLOC) == 0)
		{
			Host	**tmpArray = (Host **)realloc(array[kind], (hostCnt + BIG_ALLOC) * sizeof(Host *));
			if (tmpArray == NULL)
				return	FALSE;
			array[kind] = tmpArray;
		}
		memmove(array[kind] + insertIndex[kind] + 1, array[kind] + insertIndex[kind], (hostCnt - insertIndex[kind]) * sizeof(Host *));
		array[kind][insertIndex[kind]] = host;
	}
	host->RefCnt(1);
	hostCnt++;
	return	TRUE;
}

BOOL THosts::DelHost(Host *host)
{
	int		insertIndex[MAX_ARRAY];

// すべてのインデックス種類での確認を先に行う
	for (int kind=0; kind < MAX_ARRAY; kind++) {
		if (enable[kind] == FALSE)
			continue;
		if (Search((Kind)kind, &host->hostSub, &insertIndex[kind]) == NULL)
			return	FALSE;
	}

	hostCnt--;

	for (kind=0; kind < MAX_ARRAY; kind++) {
		if (enable[kind] == FALSE)
			continue;
		memmove(array[kind] + insertIndex[kind], array[kind] + insertIndex[kind] + 1, (hostCnt - insertIndex[kind]) * sizeof(Host *));
	}
	host->RefCnt(-1);

	return	TRUE;
}

BOOL THosts::PriorityHostCnt(int priority, int range)
{
	int		member = 0;

	for (int cnt=0; cnt < hostCnt; cnt++)
		if (array[NAME][cnt]->priority >= priority && array[NAME][cnt]->priority < priority + range)
			member++;
	return	member;
}

TFindDlg::TFindDlg(Cfg *_cfg, TSendDlg *_parent) : TDlg(FIND_DIALOG, _parent)
{
	cfg = _cfg;
	sendDlg = _parent;
}

BOOL TFindDlg::EvCommand(WORD wNotifyCode, WORD wID, LPARAM hWndCtl)
{
	switch (wID)
	{
	case IDOK:
		int		cnt;
		GetDlgItemText(FIND_COMBO, cfg->FindStr[0], MAX_NAMEBUF);
		cfg->FindAll = SendDlgItemMessage(ALLFIND_CHECK, BM_GETCHECK, 0, 0);

		if (sendDlg->FindHost(cfg->FindStr[0], cfg->FindAll)) {
			for (cnt=1; cnt < cfg->FindMax; cnt++)
				if (stricmp(cfg->FindStr[cnt], cfg->FindStr[0]) == 0)
					break;
			memmove(cfg->FindStr[2], cfg->FindStr[1], (cnt == cfg->FindMax ? cnt-2 : cnt-1) * MAX_NAMEBUF);
			memcpy(cfg->FindStr[1], cfg->FindStr[0], MAX_NAMEBUF);
			DWORD	start, end;		// エディット部の選択状態の保存
			SendDlgItemMessage(FIND_COMBO, CB_GETEDITSEL, (WPARAM)&start, (LPARAM)&end);
			// CB_RESETCONTENT でエディット部がクリア
			// なお、DELETESTRING でも edit 同名stringの場合、同じくクリアされる
			SendDlgItemMessage(FIND_COMBO, CB_RESETCONTENT, 0, 0);
			for (cnt=1; cnt < cfg->FindMax && cfg->FindStr[cnt][0]; cnt++)
				SendDlgItemMessage(FIND_COMBO, CB_INSERTSTRING, cnt-1, (LPARAM)cfg->FindStr[cnt]);
			SetDlgItemText(FIND_COMBO, cfg->FindStr[0]);
			SendDlgItemMessage(FIND_COMBO, CB_SETEDITSEL, 0, MAKELPARAM(start, end));
		}
		cfg->WriteRegistry(CFG_FINDHIST);
		return	TRUE;

	case IDCANCEL: case CLOSE_BUTTON:
		EndDialog(FALSE);
		return	TRUE;
	}

	return	FALSE;
}

BOOL TFindDlg::EvCreate(LPARAM lParam)
{
	SendDlgItemMessage(ALLFIND_CHECK, BM_SETCHECK, cfg->FindAll, 0);

	for (int cnt=1; cnt < cfg->FindMax && cfg->FindStr[cnt][0]; cnt++)
		SendDlgItemMessage(FIND_COMBO, CB_INSERTSTRING, (WPARAM)cnt-1, (LPARAM)cfg->FindStr[cnt]);
	if (cfg->FindStr[0][0])
		SetDlgItemText(FIND_COMBO, cfg->FindStr[0]);

	if (rect.left == CW_USEDEFAULT) {
		GetWindowRect(&rect);
		rect.left += 140;
		rect.right += 140;
		rect.top -= 20;
		rect.bottom -= 20;
	}
	MoveWindow(rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top, FALSE);
	Show();
	return	TRUE;
}

/*
	ファイルダイアログ用汎用ルーチン
*/
BOOL OpenFileDlg::Exec(UINT editCtl, char *title, char *filter, char *defaultDir)
{
	char buf[MAX_PATH];

	if (parent == NULL)
		return FALSE;

	parent->GetDlgItemText(editCtl, buf, sizeof(buf));

	if (Exec(buf, title, filter, defaultDir) == FALSE)
		return	FALSE;

	parent->SetDlgItemText(editCtl, buf);
	return	TRUE;
}

BOOL OpenFileDlg::Exec(char *target, char *title, char *filter, char *defaultDir)
{
	OPENFILENAME	ofn;
	char			szDirName[MAX_BUF] = "", szFile[MAX_BUF] = "", *fname = NULL;

	if (*target && ::GetFullPathName(target, MAX_BUF, szDirName, &fname) != 0 && fname)
		*(fname -1) = 0, strncpyz(szFile, fname, MAX_PATH);
	else if (defaultDir)
		strncpyz(szDirName, defaultDir, MAX_PATH);

	memset(&ofn, 0, sizeof(ofn));
	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.hwndOwner = parent ? parent->hWnd : NULL;
	ofn.lpstrFilter = filter;
	ofn.nFilterIndex = filter ? 1 : 0;
	ofn.lpstrFile = szFile;
	ofn.nMaxFile = sizeof(szFile);
	ofn.lpstrTitle = title;
	ofn.lpstrInitialDir = szDirName;
	ofn.lpfnHook = hook;
	ofn.Flags = OFN_HIDEREADONLY|OFN_EXPLORER|(hook ? OFN_ENABLEHOOK : 0);
	if (mode == OPEN || mode == MULTI_OPEN)
		ofn.Flags |= OFN_FILEMUSTEXIST | (mode == MULTI_OPEN ? OFN_ALLOWMULTISELECT : 0);
	else
		ofn.Flags |= (mode == NODEREF_SAVE ? OFN_NODEREFERENCELINKS : 0);

	char	dirNameBak[MAX_PATH];
	::GetCurrentDirectory(sizeof(dirNameBak), dirNameBak);

	BOOL	ret = (mode == OPEN || mode == MULTI_OPEN) ? ::GetOpenFileName(&ofn) : ::GetSaveFileName(&ofn);

	::SetCurrentDirectory(dirNameBak);
	if (ret)
	{
		if (mode == MULTI_OPEN)
			memcpy(target, szFile, sizeof(szFile));
		else
			strncpyz(target, ofn.lpstrFile, MAX_PATH);

		if (defaultDir)
			strncpyz(defaultDir, ofn.lpstrFile, ofn.nFileOffset);
	}

	return	ret;
}

/*
	BrowseDirDlg用コールバック
*/
int CALLBACK BrowseDirDlgProc(HWND hWnd, UINT uMsg, LPARAM lParam, LPARAM path)
{
	switch (uMsg)
	{
	case BFFM_INITIALIZED:
		SendMessage(hWnd, BFFM_SETSELECTION, (WPARAM)TRUE, path);
		break;

	case BFFM_SELCHANGED:
		break;
	}
	return 0;
}

/*
	ディレクトリダイアログ用汎用ルーチン
*/
BOOL BrowseDirDlg(TWin *parentWin, const char *title, const char *defaultDir, char *buf)
{ 
	IMalloc			*iMalloc = NULL;
	BROWSEINFO		brInfo;
	LPITEMIDLIST	pidlBrowse;
	BOOL			ret = FALSE;

	if (!SUCCEEDED(SHGetMalloc(&iMalloc)))
		return FALSE;

	brInfo.hwndOwner = parentWin->hWnd;
	brInfo.pidlRoot = 0;
	brInfo.pszDisplayName = buf;
	brInfo.lpszTitle = title;
	brInfo.ulFlags = BIF_RETURNONLYFSDIRS;
	brInfo.lpfn = BrowseDirDlgProc;
	brInfo.lParam = (LPARAM)defaultDir;
	brInfo.iImage = 0;

	if ((pidlBrowse = SHBrowseForFolder(&brInfo)) != NULL)
	{
		ret = SHGetPathFromIDList(pidlBrowse, buf);
		iMalloc->Free(pidlBrowse);
	}

	iMalloc->Release();
	return	ret;
}

BOOL GetLastErrorMsg(char *msg, TWin *win)
{
	char	buf[MAX_BUF];
	wsprintf(buf, "%s error = %x", msg ? msg : "", GetLastError());
	return	MessageBox(win ? win->hWnd : NULL, buf, MSG_STR, MB_OK);
}

BOOL GetSockErrorMsg(char *msg, TWin *win)
{
	char	buf[MAX_BUF];
	wsprintf(buf, "%s error = %d", msg ? msg : "", WSAGetLastError());
	return	MessageBox(win ? win->hWnd : NULL, buf, MSG_STR, MB_OK);
}


/*
	パスからファイル名部分だけを取り出す
*/
BOOL PathToFname(const char *org_path, char *target_fname)
{
	char	path[MAX_BUF], *fname=NULL;

	if (::GetFullPathName(org_path, sizeof(path), path, &fname) == 0 || fname == NULL)
		return	FALSE;

	strncpyz(target_fname, fname, MAX_PATH);
	return	TRUE;
}

/*
	パスからファイル名部分だけを取り出す（強制的に名前を作る）
*/
void ForcePathToFname(const char *org_path, char *target_fname)
{
	if (PathToFname(org_path, target_fname))
		return;

	if (org_path[1] == ':')
		wsprintf(target_fname, "(%c-drive)", *org_path);
	else if (org_path[0] == '\\' && org_path[1] == '\\') {
		if (PathToFname(org_path + 2, target_fname) == FALSE)
			wsprintf(target_fname, "(root)");
	}
	else wsprintf(target_fname, "(unknown)");
}

/*
	2byte文字系でもきちんと動作させるためのルーチン
	 (*strrchr(path, "\\")=0 だと '表'などで問題を起すため)
*/
BOOL PathToDir(const char *org_path, char *target_dir)
{
	char	path[MAX_BUF], *fname=NULL;

	if (::GetFullPathName(org_path, sizeof(path), path, &fname) == 0 || fname == NULL)
		return	strncpyz(target_dir, org_path, MAX_PATH), FALSE;

	if (fname - path > 3 || path[1] != ':')
		*(fname - 1) = 0;
	else
		*fname = 0;		// C:\ の場合

	strncpyz(target_dir, path, MAX_PATH);
	return	TRUE;
}

/*
	fname にファイル名以外の要素が含まれていないことを確かめる
*/
BOOL IsSafePath(const char *fullpath, const char *fname)
{
	char	fname2[MAX_PATH];

	if (PathToFname(fullpath, fname2) == FALSE)
		return	FALSE;

	return	strcmp(fname, fname2) == 0 ? TRUE : FALSE;
}

int MakePath(char *dest, const char *dir, const char *file)
{
	BOOL	separetor = TRUE;
	int		len;

	if ((len = strlen(dir)) == 0)
		return	wsprintf(dest, "%s", file);

	if (dir[len -1] == '\\')	// 表など、2byte目が'\\'で終る文字列対策
	{
		if (len >= 2 && IsDBCSLeadByte(dir[len -2]) == FALSE)
			separetor = FALSE;
		else {
			for (u_char *p=(u_char *)dir; *p && p[1]; IsDBCSLeadByte(*p) ? p+=2 : p++)
				;
			if (*p == '\\')
				separetor = FALSE;
		}
	}
	return	wsprintf(dest, "%s%s%s", dir, separetor ? "\\" : "", file);
}

/*
	time() の代わり
	...なぜか数時間の誤差はあるらしい
	（相対時間としてしか、使用しない）
*/
time_t Time(void)
{
	SYSTEMTIME	st;
	_int64		ft;
// 1601年1月1日から1970年1月1日までの通算100ナノ秒
#define UNIXTIME_BASE	((_int64)0x019db1ded53e8000)

	::GetLocalTime(&st);
	::SystemTimeToFileTime(&st, (FILETIME *)&ft);
	return	(time_t)((ft - UNIXTIME_BASE) / 10000000);
}

/*
	ctime() の代わり
	ただし、改行なし
*/
char *Ctime(SYSTEMTIME *st)
{
	static char	buf[] = "Mon Jan 01 00:00:00 2999";
	static char *wday = "SunMonTueWedThuFriSat";
	static char *mon  = "JanFebMarAprMayJunJulAugSepOctNovDec";
	SYSTEMTIME	_st;

	if (st == NULL)
	{
		st = &_st;
		::GetLocalTime(st);
	}
	wsprintf(buf, "%.3s %.3s %02d %02d:%02d:%02d %04d", &wday[st->wDayOfWeek * 3], &mon[(st->wMonth - 1) * 3], st->wDay, st->wHour, st->wMinute, st->wSecond, st->wYear);
	return	buf;
}

/*
	16進文字列 <-> バイナリ変換ルーチン
*/
inline u_char hexchar2char(u_char ch)
{
	if (ch >= '0' && ch <= '9')
		return ch - '0';
	ch = toupper(ch);
	if (ch >= 'A' && ch <= 'Z')
		return ch - 'A' + 10;
	return 0;
}

BOOL hexstr2bin(const char *buf, BYTE *bindata, int maxlen, int *len)
{
	for (*len=0; buf[0] && buf[1] && *len < maxlen; buf+=2, (*len)++)
	{
		bindata[*len] = hexchar2char(buf[0]) << 4 | hexchar2char(buf[1]);
	}
	return	TRUE;
}

/* hexstr to little-endian binary */
BOOL hexstr2bin_bigendian(const char *buf, BYTE *bindata, int maxlen, int *len)
{
	*len = 0;
	for (int buflen = strlen(buf); buflen >= 2 && *len < maxlen; buflen-=2, (*len)++)
	{
		bindata[*len] = hexchar2char(buf[buflen-1]) | hexchar2char(buf[buflen-2]) << 4;
	}
	return	TRUE;
}

static char *hexstr = "0123456789abcdef";
void bin2hexstr(const BYTE *bindata, int len, char *buf)
{
	for (int cnt=0; cnt < len; cnt++)
	{
		*buf++ = hexstr[bindata[cnt] >> 4];
		*buf++ = hexstr[bindata[cnt] & 0x0f];
	}
	*buf = 0;
}

/* little-endian binary to hexstr */
void bin2hexstr_bigendian(const BYTE *bindata, int len, char *buf)
{
	while (len-- > 0)
	{
		*buf++ = hexstr[bindata[len] >> 4];
		*buf++ = hexstr[bindata[len] & 0x0f];
	}
	*buf = 0;
}

/*
	16進 -> long long
*/
_int64 hex2ll(char *buf)
{
	_int64	ret = 0;

	for ( ; *buf; buf++)
	{
		if (*buf >= '0' && *buf <= '9')
			ret = (ret << 4) | (*buf - '0');
		else if (toupper(*buf) >= 'A' && toupper(*buf) <= 'F')
			ret = (ret << 4) | (toupper(*buf) - 'A' + 10);
		else continue;
	}
	return	ret;
}

/*
	サイズを文字列に
*/
int MakeSizeString(char *buf, _int64 size, int flg)
{
	if (size >= 1024 * 1024)
	{
		if (flg & MSS_NOPOINT)
			return	wsprintf(buf, "%d%sMB", (int)(size / 1024 / 1024), flg & MSS_SPACE ? " " : "");
		else
			return	wsprintf(buf, "%d.%d%sMB", (int)(size / 1024 / 1024), (int)((size * 10 / 1024 / 1024) % 10), flg & MSS_SPACE ? " " : "");
	}
	else return	wsprintf(buf, "%d%sKB", (int)(ALIGN_BLOCK(size, 1024)), flg & MSS_SPACE ? " " : "");
}

/*
	strtok_r() の簡易版
*/
char *separate_token(char *buf, char separetor, char **handle)
{
	char	*_handle;

	if (handle == NULL)
		handle = &_handle;

	if (buf)
		*handle = buf;

	if (*handle == NULL || **handle == 0)
		return	NULL;

	while (**handle == separetor)
		(*handle)++;
	buf = *handle;

	if (**handle == 0)
		return	NULL;

	while (**handle && **handle != separetor)
		(*handle)++;

	if (**handle == separetor)
		*(*handle)++ = 0;

	return	buf;
}

void MakeHash(const BYTE *data, int len, char *hashStr)
{
	CBlowFish	bl((BYTE *)"ipmsg", 5);
	BYTE		*buf = new BYTE [len + 8];

	bl.Encrypt(data, buf, len);
	bin2hexstr(buf + len - 8, 8, hashStr);
	delete [] buf;
}

BOOL VerifyHash(const BYTE *data, int len, const char *orgHash)
{
	char	hash[MAX_NAMEBUF];

	MakeHash(data, len, hash);
	return	stricmp(hash, orgHash);
}

ULONG ResolveAddr(const char *_host)
{
	if (_host == NULL)
		return 0;

	ULONG	addr = ::inet_addr(_host);

	if (addr == 0xffffffff)
	{
		hostent	*ent = ::gethostbyname(_host);
		addr = ent ? *(ULONG *)ent->h_addr_list[0] : 0;
	}

	return	addr;
}

void TBroadcastList::Reset()
{
	TBroadcastObj	*obj;

	while ((obj = Top()) != NULL)
	{
		DelObj(obj);
		delete obj;
	}
}

#if 0
BOOL TBroadcastList::SetHost(const char *host)
{
	ULONG	addr = ResolveAddr(host);

	if (addr == 0 || IsExistHost(host))
		return	FALSE;

	SetHostRaw(host, addr);
	return	TRUE;
}

BOOL TBroadcastList::IsExistHost(const char *host)
{
	for (TBroadcastObj *obj=Top(); obj; obj=Next(obj))
		if (stricmp(obj->Host(), host) == 0)
			return	TRUE;

	return	FALSE;
}

#endif
