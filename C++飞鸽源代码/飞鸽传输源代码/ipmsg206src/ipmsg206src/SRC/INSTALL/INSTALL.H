/* @(#)Copyright (C) H.Shirouzu 1998-2002   install.h	Ver2.00 */
/* ========================================================================
	Project  Name			: Installer for IPMSG32
	Module Name				: Main Header
	Create					: 1998-06-14(Sun)
	Update					: 2002-11-03(Sun)
	Copyright				: H.Shirouzu
	Reference				: 
	======================================================================== */

enum InstMode { SETUP_MODE, RESETUP_MODE, UNINSTALL_MODE };

struct InstallCfg {
	InstMode	mode;
	int			portNo;
	BOOL		startupLink;
	BOOL		programLink;
	BOOL		desktopLink;
	BOOL		delPubkey;
};

class TInstSheet : public TDlg
{
	InstallCfg	*cfg;

public:
	TInstSheet(TWin *_parent, InstallCfg *_cfg);

	virtual BOOL	EvCreate(LPARAM lParam);
	virtual BOOL	EvCommand(WORD wNotifyCode, WORD wID, LPARAM hwndCtl);

	void	Paste(void);
	void	GetData(void);
	void	PutData(void);
};

class TInstDlg : public TDlg
{
protected:
	TSubClassCtl	staticText;
	TInstSheet		*propertySheet;
	InstallCfg		cfg;

public:
	TInstDlg(char *cmdLine);
	virtual ~TInstDlg();

	virtual BOOL	EvCreate(LPARAM lParam);
	virtual BOOL	EvCommand(WORD wNotifyCode, WORD wID, LPARAM hwndCtl);
#if 0
	virtual BOOL	EvNcDestroy(void);
	virtual BOOL	EventUser(UINT uMsg, WPARAM wParam, LPARAM lParam);
#endif
	BOOL	Install(void);
	BOOL	UnInstall(void);
	void	ChangeMode(void);
	BOOL	TerminateIPMsg(void);
	BOOL	RemoveSameLink(const char *dir, char *remove_path=NULL);
};

class TInstApp : public TApp
{
public:
	TInstApp(HINSTANCE _hI, LPSTR _cmdLine, int _nCmdShow);
	virtual ~TInstApp();

	void InitWindow(void);
};

class TBrowseDirDlg : public TSubClass
{
protected:
	char	*fileBuf;
	BOOL	dirtyFlg;

public:
	TBrowseDirDlg(char *_fileBuf) { fileBuf = _fileBuf; }
	virtual BOOL	CreateByWnd(HWND _hWnd);
	virtual BOOL	EvCommand(WORD wNotifyCode, WORD wID, LPARAM hwndCtl);
	virtual BOOL	SetFileBuf(LPARAM list);
	BOOL	IsDirty(void) { return dirtyFlg; };
};

class TInputDlg : public TDlg
{
protected:
	char	*dirBuf;

public:
	TInputDlg(char *_dirBuf, TWin *_win) : TDlg(INPUT_DIALOG, _win) { dirBuf = _dirBuf; }
	virtual BOOL	EvCommand(WORD wNotifyCode, WORD wID, LPARAM hwndCtl);
};

#define IPMSG_CLASS				"ipmsg_class"
#define IPMSG_NAME				"IPMSG for Win32"
#define IPMSG_FULLNAME			"IP Messenger for Win"
#define IPMSG_STR				"IPMsg"
#define HSTOOLS_STR				"HSTools"
#ifdef ENGLISH
#define REGSTR_IPMSG			IPMSG_STR "Eng"
#else
#define REGSTR_IPMSG			IPMSG_STR
#endif
#define IPMSG_DEFAULT_PORT		0x0979

#define IPMSG_EXENAME			"ipmsg.exe"
#define INSTALL_EXENAME			"setup.exe"
#define README_NAME				"readme.txt"
#define OPENLAB_NAME			"openlab.txt"

#define UNINSTALL_CMDLINE		"/r"
#define IPMSG_SHORTCUT_NAME		IPMSG_NAME ".lnk"
#define UNC_PREFIX				"\\\\"
#define MAX_WRAPPER_IPMSGSIZE	50000
#define RESOLVE_WRAPPER_IPMSG	"org\\ipmsg.exe"
#define MAX_BUF					1024

#define REGSTR_SHELLFOLDERS		REGSTR_PATH_EXPLORER "\\Shell Folders"
#define REGSTR_STARTUP			"Startup"
#define REGSTR_DESKTOP			"Desktop"
#define REGSTR_PROGRAMS			"Programs"
#define REGSTR_PATH				"Path"
#define REGSTR_PROGRAMFILES		"ProgramFilesDir"

#ifdef	ENGLISH
#define MKDIR_STR				"CREATE"
#define RMDIR_STR				"DELETE"
#else
#define MKDIR_STR				"作成"
#define RMDIR_STR				"削除"
#endif
#define INSTALL_STR				"Install"
#define UNINSTALL_STR			"UnInstall"

#ifdef	ENGLISH
#define NOTNEWSHELL_MSGSTR		"This setup is only run new shell.\r\n"
#define SETUPCOMPLETE_MSGSTR	"Setup is completed.\r\n\r\nDo you want to start IPMSG?"
#define UNINSTCOMPLETE_MSGSTR	"Uninstallation is completed.\r\n"
#define TERMINATE_MSGSTR		"Do you permit to close IPMSG?"
#define NOTCREATEDIR_MSGSTR		"Can't create directory"
#define NOTCREATEFILE_MSGSTR	"Can't create file"
#define START_MSGSTR			"Do you permit to start?"
#else
#define NOTNEWSHELL_MSGSTR		"このセットアップは、NewShell専用です。\r\nNT3.51でのインストールは手動で行ってください。"
#define SETUPCOMPLETE_MSGSTR	"セットアップは完了しました。\r\nアンインストールは「アプリケーションの追加と削除」から行ってください。\r\n\r\nこのまま、IP Messenger を起動しますか？"
#define UNINSTCOMPLETE_MSGSTR	"アンインストールは完了しました。\r\nファイル削除自体は手動で行ってください。"
#define TERMINATE_MSGSTR		"あらかじめ、現在起動している\r\nIP Messenger を終了させる必要があります。\r\nよろしいですか？"
#define NOTCREATEDIR_MSGSTR		"ディレクトリが作れません"
#define NOTCREATEFILE_MSGSTR	"ファイルをコピーできません"
#define START_MSGSTR			"開始します"
#endif

// function prototype
int strncmpi(const char *str1, const char *str2, int num);
BOOL CALLBACK TerminateIPMsgProc(HWND hWnd, LPARAM lParam);
BOOL SymLink(LPCSTR src, LPSTR dest, LPCSTR arg="");
BOOL ReadLink(LPCSTR src, LPSTR dest, LPSTR arg);
BOOL DeleteLink(LPCSTR path);
void BrowseDirDlg(TWin *parentWin, UINT editCtl, char *title);
int CALLBACK BrowseDirDlg_Proc(HWND hWnd, UINT uMsg, LPARAM lParam, LPARAM data);
BOOL GetParentDir(const char *srcfile, char *dir);
int MakePath(char *dest, const char *dir, const char *file);
UINT GetDriveTypeEx(const char *file);

// inline function
inline BOOL IsUncFile(const char *path) { return strnicmp(path, UNC_PREFIX, 2) == 0; }

