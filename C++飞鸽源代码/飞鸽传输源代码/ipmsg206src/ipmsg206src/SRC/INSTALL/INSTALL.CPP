static char *install_id = 
	"@(#)Copyright (C) H.Shirouzu 1998-2004   install.cpp	Ver2.06";
/* ========================================================================
	Project  Name			: Installer for IPMSG32
	Module Name				: Installer Application Class
	Create					: 1998-06-14(Sun)
	Update					: 2004-02-10(Tue)
	Copyright				: H.Shirouzu
	Reference				: 
	======================================================================== */

#include "../tlib.h"
#include "instrc.h"
#include "install.h"

char	*SetupFiles [] = { INSTALL_EXENAME, README_NAME, OPENLAB_NAME, NULL };

/*
	WinMain
*/
int WINAPI WinMain(HINSTANCE hI, HINSTANCE, LPSTR cmdLine, int nCmdShow)
{
	TInstApp	app(hI, cmdLine, nCmdShow);

	return	app.Run();
}

/*
	インストールアプリケーションクラス
*/
TInstApp::TInstApp(HINSTANCE _hI, LPSTR _cmdLine, int _nCmdShow) : TApp(_hI, _cmdLine, _nCmdShow)
{
}

TInstApp::~TInstApp()
{
}

void TInstApp::InitWindow(void)
{
	InitCommonControls();
	TDlg *maindlg = new TInstDlg(cmdLine);
	mainWnd = maindlg;
	maindlg->Create();
}


/*
	メインダイアログクラス
*/
TInstDlg::TInstDlg(char *cmdLine) : TDlg(INSTALL_DIALOG), staticText(this)
{
	cfg.mode = SETUP_MODE;
	cfg.portNo = IPMSG_DEFAULT_PORT;
	cfg.startupLink	= TRUE;
	cfg.programLink	= TRUE;
	cfg.desktopLink	= TRUE;
	cfg.delPubkey	= TRUE;

	if (cmdLine)
	{
		for (char *tok = strtok(cmdLine, " \t\n"); tok; tok = strtok(NULL, " \t\n"))
		{
			if (strcmp(tok, UNINSTALL_CMDLINE) == 0)
				cfg.mode = UNINSTALL_MODE;
			else if (*tok >= '0' && *tok <= '9' && atoi(tok) > 0)
				cfg.portNo = atoi(tok);
		}
	}
}

TInstDlg::~TInstDlg()
{
}

/*
	メインダイアログ用 WM_INITDIALOG 処理ルーチン
*/
BOOL TInstDlg::EvCreate(LPARAM lParam)
{
	if (IsNewShell() != TRUE)
	{
		MessageBox(NOTNEWSHELL_MSGSTR);
		::PostQuitMessage(0);
		return	TRUE;
	}

	if (cfg.portNo != IPMSG_DEFAULT_PORT)
	{
		char	buf[100];
		::GetWindowText(hWnd, buf, sizeof(buf));
		::wsprintf(buf + strlen(buf), " (PortNo = %d)", cfg.portNo);
		SetWindowText(buf);
	}

	GetWindowRect(&rect);
	int		cx = ::GetSystemMetrics(SM_CXFULLSCREEN), cy = ::GetSystemMetrics(SM_CYFULLSCREEN);
	int		xsize = rect.right - rect.left, ysize = rect.bottom - rect.top;

	::SetClassLong(hWnd, GCL_HICON, (LONG)::LoadIcon(TApp::hI, (LPCSTR)SETUP_ICON));
	MoveWindow((cx - xsize)/2, (cy - ysize)/2, xsize, ysize, TRUE);
	Show();

// プロパティシートの生成
	staticText.CreateByWnd(GetDlgItem(INSTALL_STATIC));
	propertySheet = new TInstSheet(&staticText, &cfg);

// 現在ディレクトリ設定
	char	buf[MAX_PATH], setupDir[MAX_PATH], resetupDir[MAX_PATH];

	::GetModuleFileName(NULL, resetupDir, sizeof(resetupDir));
	GetParentDir(resetupDir, resetupDir);
	SetDlgItemText(RESETUP_EDIT, resetupDir);

// Program Filesのパス取り出し
	TRegistry	reg(HKEY_LOCAL_MACHINE);
	if (reg.OpenKey(REGSTR_PATH_SETUP))
	{
		if (reg.GetStr(REGSTR_PROGRAMFILES, buf, sizeof(buf)))
			MakePath(setupDir, buf, IPMSG_STR);
		reg.CloseKey();
	}

// 既にセットアップされている場合は、セットアップディレクトリを読み出す
	if (reg.OpenKey(REGSTR_PATH_APPPATHS))
	{
		if (reg.OpenKey(IPMSG_EXENAME))
		{
			reg.GetStr(REGSTR_PATH, setupDir, sizeof(setupDir));
			reg.CloseKey();
		}
		reg.CloseKey();
	}
	SetDlgItemText(FILE_EDIT, setupDir);

	if (cfg.mode != UNINSTALL_MODE && (strcmp(setupDir, resetupDir) == 0 || GetDriveTypeEx(resetupDir) == DRIVE_REMOTE))
		cfg.mode = RESETUP_MODE;
	::SendMessage(GetDlgItem(cfg.mode == SETUP_MODE ? SETUP_RADIO : cfg.mode == RESETUP_MODE ? RESETUP_RADIO : UNINSTALL_RADIO), BM_SETCHECK, 1, 0);
	ChangeMode();

	return	TRUE;
}

/*
	メインダイアログ用 WM_COMMAND 処理ルーチン
*/
BOOL TInstDlg::EvCommand(WORD wNotifyCode, WORD wID, LPARAM hwndCtl)
{
	switch (wID)
	{
	case IDOK:
		propertySheet->GetData();
		if (cfg.mode == UNINSTALL_MODE)
			UnInstall();
		else
			Install();
		return	TRUE;

	case IDCANCEL:
		::PostQuitMessage(0);
		return	TRUE;

	case FILE_BUTTON:
		BrowseDirDlg(this, FILE_EDIT, "Select Install Directory");
		return	TRUE;

	case SETUP_RADIO:
	case RESETUP_RADIO:
	case UNINSTALL_RADIO:
		if (wNotifyCode == BN_CLICKED)
			ChangeMode();
		return	TRUE;
	}
	return	FALSE;
}

void TInstDlg::ChangeMode(void)
{
	cfg.mode = SendDlgItemMessage(SETUP_RADIO, BM_GETCHECK, 0, 0) ? SETUP_MODE : SendDlgItemMessage(RESETUP_RADIO, BM_GETCHECK, 0, 0) ? RESETUP_MODE : UNINSTALL_MODE;
	::EnableWindow(GetDlgItem(FILE_EDIT), cfg.mode == SETUP_MODE);
	::EnableWindow(GetDlgItem(RESETUP_EDIT), cfg.mode == RESETUP_MODE);
	propertySheet->Paste();
}

BOOL TInstDlg::Install(void)
{
	char	buf[MAX_PATH], setupDir[MAX_PATH], setupPath[MAX_PATH];

// 現在、起動中の ipmsg を終了
	if (TerminateIPMsg() != TRUE)
		return	FALSE;

// インストールパス設定
	GetDlgItemText(cfg.mode == SETUP_MODE ? FILE_EDIT : RESETUP_EDIT, setupDir, sizeof(setupDir));
	CreateDirectory(setupDir, NULL);
	DWORD	attr = GetFileAttributes(setupDir);
	if (attr == 0xffffffff || (attr & FILE_ATTRIBUTE_DIRECTORY) == 0)
		return	MessageBox(NOTCREATEDIR_MSGSTR), FALSE;
	MakePath(setupPath, setupDir, IPMSG_EXENAME);

	if (MessageBox(START_MSGSTR, INSTALL_STR, MB_OKCANCEL|MB_ICONINFORMATION) != IDOK)
		return	FALSE;

// ファイルコピー
	if (cfg.mode == SETUP_MODE)
	{
		char			installPath[MAX_PATH], orgDir[MAX_PATH];
		WIN32_FIND_DATA	data;
		HANDLE			fh;
		BOOL			copy = TRUE;

		GetDlgItemText(RESETUP_EDIT, orgDir, sizeof(orgDir));
		MakePath(buf, orgDir, IPMSG_EXENAME);
		if ((fh = ::FindFirstFile(buf, &data)) != INVALID_HANDLE_VALUE)
		{
			if (data.nFileSizeLow < MAX_WRAPPER_IPMSGSIZE)
				MakePath(buf, orgDir, RESOLVE_WRAPPER_IPMSG);
			::FindClose(fh);
		}
		if ((fh = ::FindFirstFile(setupPath, &data)) != INVALID_HANDLE_VALUE)
		{
			if (data.nFileSizeLow < MAX_WRAPPER_IPMSGSIZE)
				copy = FALSE;
			::FindClose(fh);
		}
		if (copy && ::CopyFile(buf, setupPath, FALSE) == FALSE)
			return	MessageBox(setupPath, NOTCREATEFILE_MSGSTR), FALSE;

		for (int cnt=0; SetupFiles[cnt] != NULL; cnt++)
		{
			MakePath(buf, orgDir, SetupFiles[cnt]);
			MakePath(installPath, setupDir, SetupFiles[cnt]);
			if (::CopyFile(buf, installPath, FALSE) == FALSE)
				return	MessageBox(installPath, NOTCREATEFILE_MSGSTR), FALSE;
		}
	}

// スタートアップ＆デスクトップに登録
	TRegistry	reg(HKEY_CURRENT_USER);
	if (reg.OpenKey(REGSTR_SHELLFOLDERS))
	{
		char	*regStr[]	= { REGSTR_STARTUP, REGSTR_PROGRAMS, REGSTR_DESKTOP, NULL };
		BOOL	execFlg[]	= { cfg.startupLink, cfg.programLink, cfg.desktopLink };

		for (int cnt=0; regStr[cnt] != NULL; cnt++)
		{
			if (reg.GetStr(regStr[cnt], buf, sizeof(buf)))
			{
				if (cnt != 0 || RemoveSameLink(buf, buf) != TRUE)
					::wsprintf(buf + strlen(buf), "\\%s", IPMSG_SHORTCUT_NAME);
				if (execFlg[cnt])
					SymLink(setupPath, buf);
				else
					DeleteLink(buf);
			}
		}
		reg.CloseKey();
	}

// レジストリにアプリケーション情報を登録
	reg.ChangeTopKey(HKEY_LOCAL_MACHINE);
	if (reg.OpenKey(REGSTR_PATH_APPPATHS))
	{
		if (reg.CreateKey(IPMSG_EXENAME))
		{
			reg.SetStr(NULL, setupPath);
			reg.SetStr(REGSTR_PATH, setupDir);
			reg.CloseKey();
		}
		reg.CloseKey();
	}

// レジストリにアンインストール情報を登録
	if (reg.OpenKey(REGSTR_PATH_UNINSTALL))
	{
		if (reg.CreateKey(IPMSG_NAME))
		{
			MakePath(buf, setupDir, INSTALL_EXENAME);
			strcat(buf, " /r");
			reg.SetStr(REGSTR_VAL_UNINSTALLER_DISPLAYNAME, IPMSG_FULLNAME);
			reg.SetStr(REGSTR_VAL_UNINSTALLER_COMMANDLINE, buf);
			reg.CloseKey();
		}
		reg.CloseKey();
	}

// コピーしたアプリケーションを起動
	if (MessageBox(SETUPCOMPLETE_MSGSTR, INSTALL_STR, MB_OKCANCEL|MB_ICONINFORMATION) == IDOK)
	{
		::SetCurrentDirectory(setupDir);
		::WinExec(setupPath, SW_SHOW);
	}
	if (cfg.mode == SETUP_MODE)
		::ShellExecute(NULL, NULL, setupDir, 0, 0, SW_SHOW);

	::PostQuitMessage(0);
	return	TRUE;
}

BOOL TInstDlg::UnInstall(void)
{
// 現在、起動中の ipmsg を終了
	if (TerminateIPMsg() != TRUE)
		return	FALSE;

	if (MessageBox(START_MSGSTR, UNINSTALL_STR, MB_OKCANCEL|MB_ICONINFORMATION) != IDOK)
		return	FALSE;

// スタートアップ＆デスクトップから削除
	TRegistry	reg(HKEY_CURRENT_USER);
	if (reg.OpenKey(REGSTR_SHELLFOLDERS))
	{
		char	buf[MAX_PATH];
		char	*regStr[]	= { REGSTR_STARTUP, REGSTR_PROGRAMS, REGSTR_DESKTOP, NULL };

		for (int cnt=0; regStr[cnt] != NULL; cnt++)
		{
			if (reg.GetStr(regStr[cnt], buf, sizeof(buf)))
			{
				if (cnt == 0)
					RemoveSameLink(buf);
				::wsprintf(buf + strlen(buf), "\\%s", IPMSG_SHORTCUT_NAME);
				DeleteLink(buf);
			}
		}
		reg.CloseKey();
	}

// レジストリからユーザー設定情報を削除
	if (reg.ChangeApp(HSTOOLS_STR))
		reg.DeleteChildTree(REGSTR_IPMSG);

// レジストリからアプリケーション情報を削除
	char	setupDir[MAX_PATH];		// セットアップディレクトリ情報を保存
	GetDlgItemText(RESETUP_EDIT, setupDir, sizeof(setupDir));

	reg.ChangeTopKey(HKEY_LOCAL_MACHINE);
	if (reg.OpenKey(REGSTR_PATH_APPPATHS))
	{
		if (reg.OpenKey(IPMSG_EXENAME))
		{
			reg.GetStr(REGSTR_PATH, setupDir, sizeof(setupDir));
			reg.CloseKey();
		}
		reg.DeleteKey(IPMSG_EXENAME);
		reg.CloseKey();
	}

// レジストリからアンインストール情報を削除
	if (reg.OpenKey(REGSTR_PATH_UNINSTALL))
	{
		reg.DeleteKey(IPMSG_NAME);
		reg.CloseKey();
	}

// 終了メッセージ
	MessageBox(UNINSTCOMPLETE_MSGSTR);

// インストールディレクトリを開く
	if (GetDriveTypeEx(setupDir) != DRIVE_REMOTE)
		::ShellExecute(NULL, NULL, setupDir, 0, 0, SW_SHOW);

// 公開鍵の削除
#ifndef MS_DEF_PROV
typedef unsigned long HCRYPTPROV;
#define MS_DEF_PROV				"Microsoft Base Cryptographic Provider v1.0"
#define MS_ENHANCED_PROV		"Microsoft Enhanced Cryptographic Provider v1.0"
#define CRYPT_DELETEKEYSET      0x00000010
#define CRYPT_MACHINE_KEYSET    0x00000020
#define PROV_RSA_FULL			1
#endif
	BOOL		(WINAPI *pCryptAcquireContext)(HCRYPTPROV *, LPCSTR, LPCSTR, DWORD, DWORD);
	HINSTANCE	advdll;

	if (cfg.delPubkey && (advdll = ::LoadLibrary("advapi32.dll")) && (pCryptAcquireContext = (BOOL (WINAPI *)(HCRYPTPROV *, LPCSTR, LPCSTR, DWORD, DWORD))::GetProcAddress(advdll, "CryptAcquireContextA")))
	{
		HCRYPTPROV	csp = NULL;
		char		contName[MAX_PATH], userName[MAX_PATH];
		DWORD		size = sizeof(userName);

		::GetUserName(userName, &size);

		::wsprintf(contName, "ipmsg.rsa1024.%s", userName);
		pCryptAcquireContext(&csp, contName, MS_ENHANCED_PROV, PROV_RSA_FULL, CRYPT_DELETEKEYSET|CRYPT_MACHINE_KEYSET);
		pCryptAcquireContext(&csp, contName, MS_ENHANCED_PROV, PROV_RSA_FULL, CRYPT_DELETEKEYSET);

		::wsprintf(contName, "ipmsg.rsa512.%s", userName);
		pCryptAcquireContext(&csp, contName, MS_DEF_PROV, PROV_RSA_FULL, CRYPT_DELETEKEYSET|CRYPT_MACHINE_KEYSET);
		pCryptAcquireContext(&csp, contName, MS_DEF_PROV, PROV_RSA_FULL, CRYPT_DELETEKEYSET);
	}

	::PostQuitMessage(0);
	return	TRUE;
}

/*
	同じ内容を持つショートカットを削除（スタートアップへの重複登録よけ）
*/
BOOL TInstDlg::RemoveSameLink(const char *dir, char *remove_path)
{
	char			path[MAX_PATH], dest[MAX_PATH], arg[MAX_PATH];
	HANDLE			fh;
	WIN32_FIND_DATA	data;
	BOOL			ret = FALSE;

	::wsprintf(path, "%s\\*.*", dir);
	if ((fh = ::FindFirstFile(path, &data)) == INVALID_HANDLE_VALUE)
		return	FALSE;

	do {
		::wsprintf(path, "%s\\%s", dir, data.cFileName);
		if (ReadLink(path, dest, arg) && *arg == 0)
		{
			int		dest_len = strlen(dest), ipmsg_len = strlen(IPMSG_EXENAME);
			if (dest_len > ipmsg_len && strncmpi(dest + dest_len - ipmsg_len, IPMSG_EXENAME, ipmsg_len) == 0) {
				ret = ::DeleteFile(path);
				if (remove_path != NULL)
					strcpy(remove_path, path);
			}
		}

	} while (::FindNextFile(fh, &data));

	::FindClose(fh);
	return	ret;
}


/*
	立ち上がっている IPMSG を終了
*/
BOOL TInstDlg::TerminateIPMsg(void)
{
	BOOL	existFlg = FALSE;

	::EnumWindows(TerminateIPMsgProc, (LPARAM)&existFlg);
	if (existFlg)
	{
		if (MessageBox(TERMINATE_MSGSTR, "", MB_OKCANCEL) == IDCANCEL)
			return	FALSE;
		::EnumWindows(TerminateIPMsgProc, NULL);
	}
	return	TRUE;
}

/*
	lParam == NULL ...	全 IPMSG を終了
	lParam != NULL ...	lParam を BOOL * とみなし、IPMSG proccess が存在する
						場合は、そこにTRUE を代入する。
*/
BOOL CALLBACK TerminateIPMsgProc(HWND hWnd, LPARAM lParam)
{
	char	buf[100];

	if (::GetClassName(hWnd, buf, sizeof(buf)) != 0)
	{
		if (strncmpi(IPMSG_CLASS, buf, strlen(IPMSG_CLASS)) == 0)
		{
			if (lParam)
				*(BOOL *)lParam = TRUE;		// existFlg;
			else
				::SendMessage(hWnd, WM_CLOSE, 0, 0);
		}
	}
	return	TRUE;
}


TInstSheet::TInstSheet(TWin *_parent, InstallCfg *_cfg) : TDlg(INSTALL_SHEET, _parent)
{
	cfg = _cfg;
}

void TInstSheet::GetData(void)
{
	if (resId == UNINSTALL_SHEET) {
		cfg->delPubkey = SendDlgItemMessage(DELPUBKEY_CHECK, BM_GETCHECK, 0, 0);
	}
	else {
		cfg->startupLink = SendDlgItemMessage(STARTUP_CHECK, BM_GETCHECK, 0, 0);
		cfg->programLink = SendDlgItemMessage(PROGRAM_CHECK, BM_GETCHECK, 0, 0);
		cfg->desktopLink = SendDlgItemMessage(DESKTOP_CHECK, BM_GETCHECK, 0, 0);
	}
}

void TInstSheet::PutData(void)
{
	if (resId == UNINSTALL_SHEET) {
		SendDlgItemMessage(DELPUBKEY_CHECK, BM_SETCHECK, cfg->delPubkey, 0);
	}
	else {
		SendDlgItemMessage(STARTUP_CHECK, BM_SETCHECK, cfg->startupLink, 0);
		SendDlgItemMessage(PROGRAM_CHECK, BM_SETCHECK, cfg->programLink, 0);
		SendDlgItemMessage(DESKTOP_CHECK, BM_SETCHECK, cfg->desktopLink, 0);
	}
}

void TInstSheet::Paste(void)
{
	if (hWnd) {
		if ((resId == UNINSTALL_SHEET) == (cfg->mode == UNINSTALL_MODE))
			return;
		GetData();
		Destroy();
	}
	resId = cfg->mode == UNINSTALL_MODE ? UNINSTALL_SHEET : INSTALL_SHEET;

	Create();
	PutData();
}

BOOL TInstSheet::EvCommand(WORD wNotifyCode, WORD wID, LPARAM hwndCtl)
{
	switch (wID)
	{
	case IDOK:	case IDCANCEL:
		{
			TWin	*top = parent;
			while (top->GetParent())
				top = top->GetParent();
			top->EvCommand(wNotifyCode, wID, hwndCtl);
		}
		return	TRUE;
	}
	return	FALSE;
}

BOOL TInstSheet::EvCreate(LPARAM lParam)
{
	Show();
	return	TRUE;
}

/*
	ディレクトリダイアログ用汎用ルーチン
*/
void BrowseDirDlg(TWin *parentWin, UINT editCtl, char *title)
{ 
	IMalloc			*iMalloc = NULL;
	BROWSEINFO		brInfo;
	LPITEMIDLIST	pidlBrowse;
	char			fileBuf[MAX_PATH];

	parentWin->GetDlgItemText(editCtl, fileBuf, sizeof(fileBuf));
	if (!SUCCEEDED(SHGetMalloc(&iMalloc)))
		return;

	TBrowseDirDlg	dirDlg(fileBuf);
	brInfo.hwndOwner = parentWin->hWnd;
	brInfo.pidlRoot = 0;
	brInfo.pszDisplayName = fileBuf;
	brInfo.lpszTitle = title;
	brInfo.ulFlags = BIF_RETURNONLYFSDIRS;
	brInfo.lpfn = BrowseDirDlg_Proc;
	brInfo.lParam = (LPARAM)&dirDlg;
	brInfo.iImage = 0;

	do {
		if ((pidlBrowse = ::SHBrowseForFolder(&brInfo)) != NULL)
		{
			if (::SHGetPathFromIDList(pidlBrowse, fileBuf))
				::SetDlgItemText(parentWin->hWnd, editCtl, fileBuf);
			iMalloc->Free(pidlBrowse);
			break;
		}
	} while (dirDlg.IsDirty());

	iMalloc->Release();
}

/*
	BrowseDirDlg用コールバック
*/
int CALLBACK BrowseDirDlg_Proc(HWND hWnd, UINT uMsg, LPARAM lParam, LPARAM data)
{
	switch (uMsg)
	{
	case BFFM_INITIALIZED:
		((TBrowseDirDlg *)data)->CreateByWnd(hWnd);
		break;

	case BFFM_SELCHANGED:
		if (((TBrowseDirDlg *)data)->hWnd)
			((TBrowseDirDlg *)data)->SetFileBuf(lParam);
		break;
	}
	return 0;
}

/*
	BrowseDlg用サブクラス生成
*/
BOOL TBrowseDirDlg::CreateByWnd(HWND _hWnd)
{
	BOOL	ret = TSubClass::CreateByWnd(_hWnd);
	dirtyFlg = FALSE;

// ディレクトリ設定
	DWORD	attr = GetFileAttributes(fileBuf);
	if (attr == 0xffffffff || (attr & FILE_ATTRIBUTE_DIRECTORY) == 0)
		GetParentDir(fileBuf, fileBuf);
	SendMessage(BFFM_SETSELECTION, TRUE, (LPARAM)fileBuf);
	SetWindowText(IPMSG_FULLNAME);

// ボタン作成
	RECT	tmp_rect;
	::GetWindowRect(GetDlgItem(IDOK), &tmp_rect);
	POINT	pt = { tmp_rect.left, tmp_rect.top };
	::ScreenToClient(hWnd, &pt);
	int		cx = (pt.x - 30) / 2, cy = tmp_rect.bottom - tmp_rect.top;

	::CreateWindow(BUTTON_CLASS, MKDIR_STR, WS_CHILD|WS_VISIBLE|BS_PUSHBUTTON, 10, pt.y, cx, cy, hWnd, (HMENU)MKDIR_BUTTON, TApp::hI, NULL);
	::CreateWindow(BUTTON_CLASS, RMDIR_STR, WS_CHILD|WS_VISIBLE|BS_PUSHBUTTON, 18 + cx, pt.y, cx, cy, hWnd, (HMENU)RMDIR_BUTTON, TApp::hI, NULL);

	HFONT	hDlgFont = (HFONT)SendDlgItemMessage(IDOK, WM_GETFONT, 0, 0L);
	if (hDlgFont)
	{
		SendDlgItemMessage(MKDIR_BUTTON, WM_SETFONT, (UINT)hDlgFont, 0L);
		SendDlgItemMessage(RMDIR_BUTTON, WM_SETFONT, (UINT)hDlgFont, 0L);
	}

	return	ret;
}

/*
	BrowseDlg用 WM_COMMAND 処理
*/
BOOL TBrowseDirDlg::EvCommand(WORD wNotifyCode, WORD wID, LPARAM hwndCtl)
{
	switch (wID)
	{
	case MKDIR_BUTTON:
		{
			char		dirBuf[MAX_PATH], path[MAX_PATH];
			TInputDlg	dlg(dirBuf, this);
			if (dlg.Exec() == FALSE)
				return	TRUE;
			MakePath(path, fileBuf, dirBuf);
			if (::CreateDirectory(path, NULL))
			{
				strcpy(fileBuf, path);
				dirtyFlg = TRUE;
				PostMessage(WM_CLOSE, 0, 0);
			}
		}
		return	TRUE;

	case RMDIR_BUTTON:
		if (::RemoveDirectory(fileBuf))
		{
			GetParentDir(fileBuf, fileBuf);
			dirtyFlg = TRUE;
			PostMessage(WM_CLOSE, 0, 0);
		}
		return	TRUE;
	}
	return	FALSE;
}

BOOL TBrowseDirDlg::SetFileBuf(LPARAM list)
{
	return	::SHGetPathFromIDList((LPITEMIDLIST)list, fileBuf);
}

/*
	一行入力
*/
BOOL TInputDlg::EvCommand(WORD wNotifyCode, WORD wID, LPARAM hwndCtl)
{
	switch (wID)
	{
	case IDOK:
		GetDlgItemText(INPUT_EDIT, dirBuf, MAX_PATH);
		EndDialog(TRUE);
		return	TRUE;

	case IDCANCEL:
		EndDialog(FALSE);
		return	TRUE;
	}
	return	FALSE;
}

/*
	親ディレクトリ取得（必ずフルパスであること。UNC対応）
*/
BOOL GetParentDir(const char *srcfile, char *dir)
{
	char	path[MAX_BUF], *fname=NULL;

	if (::GetFullPathName(srcfile, sizeof(path), path, &fname) == 0 || fname == NULL)
		return	strcpy(dir, srcfile), FALSE;

	if (fname - path > 3 || path[1] != ':')
		*(fname - 1) = 0;
	else
		*fname = 0;		// C:\ の場合

	strcpy(dir, path);
	return	TRUE;
}

/*
	ディレクトリとファイルの連結
*/
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
	ファイルの保存されているドライブ識別
*/
UINT GetDriveTypeEx(const char *file)
{
	if (file == NULL)
		return	GetDriveType(NULL);

	if (IsUncFile(file))
		return	DRIVE_REMOTE;

	char	buf[MAX_PATH];
	int		len = strlen(file), len2;

	strcpy(buf, file);
	do {
		len2 = len;
		GetParentDir(buf, buf);
		len = strlen(buf);
	} while (len != len2);

	return	GetDriveType(buf);
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
	リンク
	あらかじめ、CoInitialize(NULL); を実行しておくこと
*/
BOOL SymLink(LPCSTR src, LPSTR dest, LPCSTR arg)
{
	IShellLink		*shellLink;
	IPersistFile	*persistFile;
	WORD			wbuf[MAX_PATH];
	BOOL			ret = FALSE;
	char			buf[MAX_PATH];

	if (SUCCEEDED(CoCreateInstance(CLSID_ShellLink, NULL, CLSCTX_INPROC_SERVER, IID_IShellLink, (void **)&shellLink)))
	{
		shellLink->SetPath(src);
		shellLink->SetArguments(arg);
		GetParentDir(src, buf);
		shellLink->SetWorkingDirectory(buf);
		if (SUCCEEDED(shellLink->QueryInterface(IID_IPersistFile, (void **)&persistFile)))
		{
			MultiByteToWideChar(CP_ACP, 0, dest, -1, wbuf, MAX_PATH);
			if (SUCCEEDED(persistFile->Save(wbuf, TRUE)))
			{
				ret = TRUE;
				GetParentDir(dest, buf);
				::SHChangeNotify(SHCNE_UPDATEDIR, SHCNF_PATH|SHCNF_FLUSH, buf, NULL);
			}
			persistFile->Release();
		}
		shellLink->Release();
	}
	return	ret;
}

/*
	リンクの解決
	あらかじめ、CoInitialize(NULL); を実行しておくこと
*/
BOOL ReadLink(LPCSTR src, LPSTR dest, LPSTR arg)
{
	IShellLink		*shellLink;
	IPersistFile	*persistFile;
	WORD			wbuf[MAX_PATH];
	BOOL			ret = FALSE;

	if (SUCCEEDED(CoCreateInstance(CLSID_ShellLink, NULL, CLSCTX_INPROC_SERVER, IID_IShellLink, (void **)&shellLink)))
	{
		if (SUCCEEDED(shellLink->QueryInterface(IID_IPersistFile, (void **)&persistFile)))
		{
			::MultiByteToWideChar(CP_ACP, 0, src, -1, wbuf, MAX_PATH);
			if (SUCCEEDED(persistFile->Load(wbuf, STGM_READ)))
			{
				if (SUCCEEDED(shellLink->GetPath(dest, MAX_PATH, NULL, SLGP_SHORTPATH)))
				{
					shellLink->GetArguments(arg, MAX_PATH);
					ret = TRUE;
				}
			}
			persistFile->Release();
		}
		shellLink->Release();
	}
	return	ret;
}

/*
	リンクファイル削除
*/
BOOL DeleteLink(LPCSTR path)
{
	char	dir[MAX_PATH];

	if (::DeleteFile(path) != TRUE)
		return	FALSE;

	GetParentDir(path, dir);
	::SHChangeNotify(SHCNE_UPDATEDIR, SHCNF_PATH|SHCNF_FLUSH, dir, NULL);

	return	TRUE;
}

