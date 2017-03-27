static char *mainwin_id = 
	"@(#)Copyright (C) H.Shirouzu 1996-2004   mainwin.cpp	Ver2.06";
/* ========================================================================
	Project  Name			: IP Messenger for Win32
	Module Name				: Main Window
	Create					: 1996-06-01(Sat)
	Update					: 2004-09-08(Wed)
	Copyright				: H.Shirouzu
	Reference				: 
	======================================================================== */

#include "tlib.h"
#include "resource.h"
#include "ipmsg.h"
#include "msgstr.h"
#include "blowfish.h"

HICON		TMainWin::hMainIcon = NULL;
HICON		TMainWin::hRevIcon = NULL;
TMainWin	*TMainWin::mainWin = NULL;
static HWND	hMainWnd = NULL;

/*
	MainWindow の初期化
*/
TMainWin::TMainWin(ULONG nicAddr, int _portNo, TWin *_parent) : TWin(_parent)
{
	hosts.Enable(THosts::NAME, TRUE);
	hosts.Enable(THosts::ADDR, TRUE);
	cfg = new Cfg(nicAddr, portNo = _portNo);
	if ((msgMng = new MsgMng(nicAddr, portNo, cfg))->GetStatus() == FALSE)
	{
		::ExitProcess(0xffffffff);
		return;
	}
	cfg->ReadRegistry();
	setupDlg = new TSetupDlg(cfg, &hosts);
	aboutDlg = new TAboutDlg();
	absenceDlg = new TAbsenceDlg(cfg, this);
	logmng = new LogMng(cfg);
	ansList = new TRecycleList(MAX_ANSLIST, sizeof(AddrObj));
	shareMng = new ShareMng(cfg);
	shareStatDlg = new TShareStatDlg(shareMng, cfg);
	refreshStartTime = 0;
	entryStartTime = IPMSG_GETLIST_FINISH;

	memset(packetLog, 0, sizeof(packetLog));
	packetLogCnt = 0;

	entryTimerStatus = 0;
	reverseTimerStatus = 0;
	ansTimerID = 0;
	terminateFlg = FALSE;
	activeToggle = TRUE;

	InitIcon();

	hAccel = ::LoadAccelerators(TApp::hI, (LPCSTR)IPMSG_ACCEL);
}

/*
	MainWindow 終了
*/
TMainWin::~TMainWin()
{
#if 0	// 無駄
	delete msgMng;
	delete cfg;
	delete ansList;
#endif
}

/*
	子Window および socketの終了と所有Memoryの開放
*/
void TMainWin::Terminate(void)
{
	if (terminateFlg)
		return;
	terminateFlg = TRUE;

	if (msgMng->GetStatus() != TRUE)
		return;

	ExitHost();
	DeleteListDlg(&sendList);
	DeleteListDlg(&recvList);
	DeleteListDlg(&msgList);
	delete aboutDlg;
	delete shareStatDlg;
	delete absenceDlg;
	delete setupDlg;

	if (IsNewShell())
		TaskBar(NIM_DELETE);

	time_t	now_time = Time();
	for (int cnt=0; cnt < hosts.HostCnt(); cnt++)
		hosts.GetHost(cnt)->updateTime = now_time;
	cfg->WriteRegistry(CFG_HOSTINFO|CFG_DELCHLDHOST);

	msgMng->CloseSocket();
	if (cfg->hPrivKey)
		pCryptDestroyKey(cfg->hPrivKey);
	if (cfg->hSmallPrivKey)
		pCryptDestroyKey(cfg->hSmallPrivKey);

#if 0	// 無駄
	delete logmng;

	Host *host;
	while (hosts.HostCnt() > 0)
	{
		hosts.DelHost(host = hosts.GetHost(0));
//		delete host;
	}

	AddrObj *brObj;
	while ((brObj = (AddrObj *)cfg->DialUpList.TopObj()) != NULL)
	{
		cfg->DialUpList.DelObj(brObj);
		delete brObj;
	}
#endif
}

/*
	MainWindow 生成時の CallBack
	New Shell なら、TaskTray に icon登録、そうでないなら icon化
	Packet 受信開始、Entry Packetの送出
*/
BOOL TMainWin::EvCreate(LPARAM lParam)
{
	hMainWnd = hWnd;
	mainWin = this;

	if (msgMng->GetStatus() != TRUE)
		return	TRUE;

	if (IsNewShell())
	{
		Show(SW_HIDE);
		while (TaskBar(NIM_ADD, hMainIcon, IPMSG_MSGSTR) != TRUE)
			Sleep(1000);	// for logon script
	}
	else
		Show(SW_MINIMIZE);
	TaskBarCreateMsg= ::RegisterWindowMessage("TaskbarCreated");

	SetIcon(cfg->AbsenceCheck == FALSE ? hMainIcon : hRevIcon);
	SetCaption();
	if (SetupCryptAPI() == FALSE)
		MessageBox("CryptoAPI can't be used. Setup New version IE");

	msgMng->AsyncSelectRegist(hWnd);
	SetHotKey(cfg);

	if (msgMng->GetStatus())
		EntryHost();

	return	TRUE;
}

/*
	Task Managerなどからの終了要求
*/
BOOL TMainWin::EvClose(void)
{
	Terminate();
	::PostQuitMessage(0);
	return	TRUE;
}

/*
	Timer Callback
	NonPopup受信時の icon反転
	ListGet Modeにおける、HostList要求/受信処理
*/
BOOL TMainWin::EvTimer(WPARAM timerID, TIMERPROC proc)
{
	if (terminateFlg)
	{
		::KillTimer(hWnd, timerID);
		return	TRUE;
	}

	switch (timerID)
	{
	case IPMSG_REVERSEICON:
		ReverseIcon(FALSE);
		return	TRUE;

	case IPMSG_LISTGET_TIMER:
		::KillTimer(hWnd, IPMSG_LISTGET_TIMER);
		entryTimerStatus = 0;
		if (entryStartTime != IPMSG_GETLIST_FINISH)
		{
			entryStartTime = Time();
			if (::SetTimer(hWnd, IPMSG_LISTGETRETRY_TIMER, cfg->ListGetMSec, NULL))
				entryTimerStatus = IPMSG_LISTGETRETRY_TIMER;
			BroadcastEntry(IPMSG_BR_ISGETLIST2 | IPMSG_RETRYOPT);
		}
		return	TRUE;

	case IPMSG_LISTGETRETRY_TIMER:
		::KillTimer(hWnd, IPMSG_LISTGETRETRY_TIMER);
		entryTimerStatus = 0;
		if (entryStartTime != IPMSG_GETLIST_FINISH)
		{
			entryStartTime = IPMSG_GETLIST_FINISH;
			if (cfg->ListGet)
				BroadcastEntry(IPMSG_BR_ENTRY);
		}
		return	TRUE;

	case IPMSG_ANS_TIMER:
		::KillTimer(hWnd, IPMSG_ANS_TIMER);
		ansTimerID = 0;
		ExecuteAnsQueue();
		return	TRUE;

	case IPMSG_ENTRY_TIMER:
		::KillTimer(hWnd, IPMSG_ENTRY_TIMER);

		for (TBroadcastObj *brobj=cfg->broadcastList.Top(); brobj; brobj=cfg->broadcastList.Next(brobj))
			BroadcastEntrySub(brobj->Addr(), htons(portNo), IPMSG_BR_ENTRY);

		for (AddrObj *obj = (AddrObj *)cfg->DialUpList.TopObj(); obj != NULL; obj = (AddrObj *)cfg->DialUpList.NextObj(obj))
			BroadcastEntrySub(obj->addr, obj->portNo, IPMSG_BR_ENTRY);
		return	TRUE;
	}

	return	FALSE;
}

/*
	WM_COMMAND CallBack
*/
BOOL TMainWin::EvCommand(WORD wNotifyCode, WORD wID, LPARAM hwndCtl)
{
	switch (wID)
	{
	case MENU_SETUP:
		MiscDlgOpen(setupDlg);
		return	TRUE;

	case MENU_LOGOPEN:
		LogOpen();
		return	TRUE;

	case MENU_ABOUT:
		MiscDlgOpen(aboutDlg);
		return	TRUE;

	case MENU_SHARE:
		MiscDlgOpen(shareStatDlg);
		return	TRUE;

	case MENU_ACTIVEWIN:
		ActiveChildWindow(activeToggle = TRUE);
		return	TRUE;

	case MENU_ABSENCE:
		cfg->AbsenceCheck = !cfg->AbsenceCheck;
		BroadcastEntry(IPMSG_BR_ABSENCE); 
		SetIcon(cfg->AbsenceCheck == FALSE ? hMainIcon : hRevIcon);
		return	TRUE;

	case MENU_ABSENCEEX:
		MiscDlgOpen(absenceDlg);
		return	TRUE;

	case HIDE_ACCEL:
		PostMessage(WM_HIDE_CHILDWIN, 0, 0);
		return	TRUE;

	case MISC_ACCEL:
	case MENU_DELMISCDLG:
		DeleteListDlg(&msgList);
		return	TRUE;

	case MENU_EXIT:
	case IDCANCEL:
		Terminate();
		::PostQuitMessage(0);
		return	TRUE;

	default:
		if (wID >= (UINT)MENU_ABSENCE_START && wID < (UINT)MENU_ABSENCE_START + cfg->AbsenceMax)
		{
			cfg->AbsenceChoice = wID - MENU_ABSENCE_START;
			cfg->AbsenceCheck = FALSE;
			EvCommand(0, MENU_ABSENCE, 0);
			return	TRUE;
		}
	}
	return	FALSE;
}

/*
	System Menu Callback
*/
BOOL TMainWin::EvSysCommand(WPARAM uCmdType, POINTS pos)
{
	switch (uCmdType)
	{
	case SC_RESTORE: case SC_MAXIMIZE:
		if (!IsNewShell())
			SendDlgOpen();
		return	TRUE;

	case MENU_SETUP:
	case MENU_LOGOPEN:
	case MENU_ABOUT:
	case MENU_SHARE:
	case MENU_ACTIVEWIN:
	case MENU_DELMISCDLG:
	case MENU_ABSENCE:
	case MENU_ABSENCEEX:
	case MENU_EXIT:
		return	EvCommand(0, uCmdType, 0);

	default:
		if (uCmdType >= MENU_ABSENCE_START && (int)uCmdType < MENU_ABSENCE_START + cfg->AbsenceMax)
			return	EvCommand(0, uCmdType, 0);
		break;
	}
	return	FALSE;
}

/*
	Logout時などの終了通知 CallBack
*/
BOOL TMainWin::EvEndSession(BOOL nSession, BOOL nLogOut)
{
	if (nSession)
		Terminate();
	return	TRUE;
}

/*
	iconを通常Windowに戻してよいかどうかの問い合わせ CallBack
*/
BOOL TMainWin::EvQueryOpen(void)
{
	if (!IsNewShell())
		SendDlgOpen();
	return	TRUE;
}

BOOL TMainWin::EvHotKey(int hotKey)
{
	switch (hotKey)
	{
	case WM_SENDDLG_OPEN:
	case WM_DELMISCDLG:
	case WM_RECVDLG_OPEN:
		return	PostMessage(hotKey, 0, 0), TRUE;
	}
	return	FALSE;
}

/*
	MouseButton Event CallBack
*/
BOOL TMainWin::EventButton(UINT uMsg, int nHitTest, POINTS pos)
{
	switch (uMsg)
	{
//	case WM_RBUTTONDOWN:
//	case WM_NCRBUTTONDOWN:
	case WM_RBUTTONUP:
	case WM_NCRBUTTONUP:
		Popup(MAIN_MENU);
		return	TRUE;

	case WM_LBUTTONDBLCLK:
	case WM_NCLBUTTONDBLCLK:
		if (cfg->OneClickPopup == FALSE)
			SendDlgOpen();
		return	TRUE;

	case WM_LBUTTONDOWN:
	case WM_NCLBUTTONDOWN:
		SetForegroundWindow();

		BOOL ctl_on = (GetAsyncKeyState(VK_CONTROL) & 0x8000) ? TRUE : FALSE;
		BOOL shift_on = (GetAsyncKeyState(VK_SHIFT) & 0x8000) ? TRUE : FALSE;
		BOOL menu_on = (GetAsyncKeyState(VK_MENU) & 0x8000) ? TRUE : FALSE;

//		if (ctl_on && !menu_on)
//			return	PostMessage(WM_COMMAND, MENU_ABSENCE, 0), TRUE;
		if (shift_on && !menu_on)
			return	PostMessage(WM_COMMAND, MENU_ACTIVEWIN, 0), TRUE;

		if (!IsNewShell() && cfg->MsgMinimize == FALSE)
			ActiveListDlg(&msgList);

		for (TSendDlg *dlg = (TSendDlg *)sendList.TopObj(); dlg != NULL; dlg = (TSendDlg *)sendList.NextObj(dlg))
			if (dlg->IsSending())
				dlg->SetForegroundWindow();	// 再送信確認ダイアログを前に

		if (PopupCheck())
			return	TRUE;

		if (cfg->OneClickPopup)
			PostMessage(WM_SENDDLG_OPEN, 0, 0);
		return	FALSE;
	}
	return	FALSE;
}


/*
	Menu Event CallBack
*/
BOOL TMainWin::EventInitMenu(UINT uMsg, HMENU hMenu, UINT uPos, BOOL fSystemMenu)
{
	switch (uMsg)
	{
	case WM_INITMENU:
		if (IsNewShell() != TRUE && GetMenuItemID(hMenu, 0) != MENU_DELMISCDLG)
		{
			HMENU	hTargetMenu;
			char	buf[MAX_LISTBUF];

			GetSystemMenu(hWnd, TRUE);		// 初期化
			hTargetMenu = GetSystemMenu(hWnd, FALSE);
			DeleteMenu(hTargetMenu, SC_MAXIMIZE, MF_BYCOMMAND);
			DeleteMenu(hTargetMenu, SC_MINIMIZE, MF_BYCOMMAND);
			DeleteMenu(hTargetMenu, SC_SIZE, MF_BYCOMMAND);
			DeleteMenu(hTargetMenu, SC_TASKLIST, MF_BYCOMMAND);

			HMENU	hMainMenu	= LoadMenu(TApp::hI, (LPCSTR)MAIN_MENU);
			HMENU	hSubMenu	= GetSubMenu(hMainMenu, 0);
			int		maxItem = GetMenuItemCount(hSubMenu);
			for (int cnt=0; cnt < maxItem -2; cnt++)
			{
				if (GetMenuString(hSubMenu, cnt, buf, sizeof(buf), MF_BYPOSITION) > 0)
					::AppendMenu(hTargetMenu, MF_STRING, GetMenuItemID(hSubMenu, cnt), buf);
				else
					::AppendMenu(hTargetMenu, MF_SEPARATOR, NULL, NULL);
			}
			AddAbsenceMenu(hTargetMenu, 0);
			DestroyMenu(hMainMenu);
		}
		return	TRUE;
	}
	return	FALSE;
}

BOOL TMainWin::AddAbsenceMenu(HMENU hTargetMenu, int insertOffset)
{
	char	buf[MAX_LISTBUF];
	HMENU	hSubMenu = ::CreateMenu();
	UINT	index = ::GetMenuItemCount(hTargetMenu) - insertOffset;

	if (hSubMenu == NULL)
		return	FALSE;

	for (int cnt=cfg->AbsenceMax -1; cnt >= 0; cnt--)
		::AppendMenu(hSubMenu, MF_STRING, MENU_ABSENCE_START + cnt, cfg->AbsenceHead[cnt]);
	::AppendMenu(hSubMenu, MF_SEPARATOR, 0, 0);
	::AppendMenu(hSubMenu, MF_STRING, MENU_ABSENCEEX, ABSENCESET_MSGSTR);
	::InsertMenu(hTargetMenu, index++, MF_BYPOSITION|MF_POPUP, (UINT)hSubMenu, ABSENCEMENU_MSGSTR);

	if (cfg->AbsenceCheck)
	{
		wsprintf(buf, "%s(%s)", ABSENCELIFT_MSGSTR, cfg->AbsenceHead[cfg->AbsenceChoice]);
		::InsertMenu(hTargetMenu, index, MF_BYPOSITION|MF_STRING, MENU_ABSENCE, buf);
	}
	return	TRUE;
}

/*
	User定義 Event CallBack
*/
BOOL TMainWin::EventUser(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_SENDDLG_OPEN:
		SendDlgOpen((HWND)wParam, (MsgBuf *)lParam);
		return	TRUE;

	case WM_SENDDLG_EXIT:
		SendDlgExit((TSendDlg *)lParam);
		return	TRUE;

	case WM_SENDDLG_HIDE:
		SendDlgHide((TSendDlg *)lParam);
		return	TRUE;

	case WM_RECVDLG_OPEN:
		PopupCheck();
		return	TRUE;

	case WM_RECVDLG_EXIT:
		RecvDlgExit((TRecvDlg *)lParam);
		return	TRUE;

	case WM_IPMSG_NOTIFY:	// TaskTray
		PostMessage((UINT)lParam, 0, 0);
		return	TRUE;

	case WM_HIDE_CHILDWIN:
		ActiveChildWindow(activeToggle = !activeToggle);
		return	TRUE;

	case WM_UDPEVENT:
		UdpEvent(lParam);
		return	TRUE;

	case WM_TCPEVENT:
		TcpEvent(wParam, lParam);
		return	TRUE;

	case WM_REFRESH_HOST:
		RefreshHost((BOOL)wParam);
		return	TRUE;

	case WM_MSGDLG_EXIT:
		MsgDlgExit((TMsgDlg *)lParam);
		return	TRUE;

	case WM_DELMISCDLG:
		EvCommand(0, MENU_DELMISCDLG, 0);
		return	TRUE;

	case WM_IPMSG_INITICON:
		InitIcon();
		SetIcon(cfg->AbsenceCheck == FALSE ? hMainIcon : hRevIcon);
		return	TRUE;

	default:
		if (uMsg == TaskBarCreateMsg)
		{
			TaskBar(NIM_ADD, hMainIcon, IPMSG_MSGSTR);
			return	TRUE;
		}
	}
	return	FALSE;
}

/*
	参加 Message Packet送出
*/
void TMainWin::EntryHost(void)
{
	time_t	now_time = Time();

	if (entryStartTime + (time_t)cfg->ListGetMSec / 1000 > now_time)
		return;
	entryStartTime = now_time;

	if (cfg->ListGet)
	{
		if (::SetTimer(hWnd, IPMSG_LISTGET_TIMER, cfg->ListGetMSec, NULL))
			entryTimerStatus = IPMSG_LISTGET_TIMER;
		BroadcastEntry(IPMSG_BR_ISGETLIST2);
	}
	else
		BroadcastEntry(IPMSG_BR_ENTRY);
}

/*
	脱出 Message Packet送出
*/
void TMainWin::ExitHost(void)
{
	BroadcastEntry(IPMSG_BR_EXIT);
	Sleep(100);
	BroadcastEntry(IPMSG_BR_EXIT);
}

/*
	重複 Packet調査（簡易...もしくは手抜きとも言う(^^;）
*/				
BOOL TMainWin::IsLastPacket(MsgBuf *msg)
{
	for (int cnt=0; cnt < MAX_PACKETLOG; cnt++)
	{
		if (packetLog[cnt].addr == msg->hostSub.addr && packetLog[cnt].port == msg->hostSub.portNo && packetLog[cnt].no == msg->packetNo && msg->packetNo != IPMSG_DEFAULT_PORT)
		return	TRUE;
	}
	return	FALSE;
}

/*
	UDP Packet 受信処理
*/
BOOL TMainWin::UdpEvent(LPARAM lParam)
{
	MsgBuf	msg;

	if (WSAGETSELECTERROR(lParam) || msgMng->Recv(&msg) != TRUE)
		return	FALSE;

	if (cfg->PriorityReject)
	{
		Host *host = cfg->priorityHosts.GetHostByName(&msg.hostSub);
		if (host && (host->priority < 0 || cfg->PriorityReject >= 2 && host->priority == 0) && IsSameHost(&msg.hostSub, msgMng->GetLocalHost()) == FALSE)
		{
			ULONG	cmd = GET_MODE(msg.command);
			if (cmd != IPMSG_BR_EXIT && cmd != IPMSG_NOOPERATION)
				BroadcastEntrySub(&msg.hostSub, IPMSG_BR_EXIT);
			if (GET_MODE(msg.command) != IPMSG_ANSENTRY)
				return	FALSE;
		}
	}

	switch (GET_MODE(msg.command))
	{
	case IPMSG_BR_ENTRY:
		MsgBrEntry(&msg);
		return	TRUE;

	case IPMSG_BR_EXIT:
		MsgBrExit(&msg);
		return	TRUE;

	case IPMSG_ANSENTRY:
		MsgAnsEntry(&msg);
		return	TRUE;

	case IPMSG_BR_ABSENCE:
		MsgBrAbsence(&msg);
		return	TRUE;

	case IPMSG_SENDMSG:
		MsgSendMsg(&msg);
		break;

	case IPMSG_RECVMSG:
		MsgRecvMsg(&msg);
		break;

	case IPMSG_READMSG:
	case IPMSG_ANSREADMSG:
		MsgReadMsg(&msg);
		break;

	case IPMSG_BR_ISGETLIST2:
		MsgBrIsGetList(&msg);
		return	TRUE;

	case IPMSG_OKGETLIST:
		MsgOkGetList(&msg);
		return	TRUE;

	case IPMSG_GETLIST:
		MsgGetList(&msg);
		return	TRUE;

	case IPMSG_ANSLIST:
		MsgAnsList(&msg);
		return	TRUE;

	case IPMSG_GETINFO:
		MsgGetInfo(&msg);
		break;

	case IPMSG_SENDINFO:
		MsgSendInfo(&msg);
		break;

	case IPMSG_GETPUBKEY:
		MsgGetPubKey(&msg);
		break;

	case IPMSG_ANSPUBKEY:
		MsgAnsPubKey(&msg);
		break;

	case IPMSG_GETABSENCEINFO:
		MsgGetAbsenceInfo(&msg);
		break;

	case IPMSG_SENDABSENCEINFO:
		MsgSendAbsenceInfo(&msg);
		break;

	case IPMSG_RELEASEFILES:
		MsgReleaseFiles(&msg);
		break;
	}

	packetLog[packetLogCnt].no = msg.packetNo;
	packetLog[packetLogCnt].addr = msg.hostSub.addr;
	packetLog[packetLogCnt].port = msg.hostSub.portNo;
	packetLogCnt = (packetLogCnt + 1) % MAX_PACKETLOG;

	return	TRUE;
}

/*
	TCP Packet 受信処理
*/
inline SendFileObj *TMainWin::FindSendFileObj(SOCKET sd)
{
	for (SendFileObj *obj = (SendFileObj *)sendFileList.TopObj(); obj; obj = (SendFileObj *)sendFileList.NextObj(obj))
		if (obj->conInfo->sd == sd)
			return	obj;
	return	NULL;
}

BOOL TMainWin::TcpEvent(SOCKET sd, LPARAM lParam)
{
	if (WSAGETSELECTERROR(lParam))
		return	FALSE;

	switch (LOWORD(lParam)) {
	case FD_ACCEPT:
		{
			ConnectInfo	tmpInfo, *info;
			if (msgMng->Accept(hWnd, &tmpInfo))
			{
				if (CheckConnectInfo(&tmpInfo))
				{
					info = new ConnectInfo(tmpInfo);
					connectList.AddObj(info);
				}
				else ::closesocket(tmpInfo.sd);
			}
		}
		break;

	case FD_READ:
		StartSendFile(sd);
		break;

	case FD_CLOSE:
		{
			SendFileObj *obj;
			if ((obj = FindSendFileObj(sd)) != NULL)
				EndSendFile(obj);
			else
				::closesocket(sd);
		}
		break;
	}
	return	TRUE;
}

BOOL TMainWin::CheckConnectInfo(ConnectInfo *conInfo)
{
	for (ShareInfo *info=shareMng->Top(); info; info=shareMng->Next(info))
	{
		for (int cnt=0; cnt < info->hostCnt; cnt++)
			if (info->host[cnt]->hostSub.addr == conInfo->addr/* && info->host[cnt]->hostSub.portNo == conInfo->port*/)
				return	conInfo->port = info->host[cnt]->hostSub.portNo, TRUE;
	}
	return	FALSE;
}

/*
	ファイル送受信開始処理
*/
BOOL TMainWin::StartSendFile(SOCKET sd)
{
	ConnectInfo 	*conInfo;
	AcceptFileInfo	fileInfo;

	for (conInfo=(ConnectInfo *)connectList.TopObj(); conInfo && conInfo->sd != sd; conInfo=(ConnectInfo *)connectList.NextObj(conInfo))
		;
	if (conInfo == NULL)
		return	::closesocket(sd), FALSE;
	else {
		msgMng->ConnectDone(hWnd, conInfo);	// 非同期メッセージの抑制

// すでに read 要求がきているので、固まる事は無い...はず
// 一度の recv で読めない場合、エラーにしてしまう（手抜き）
		char	buf[MAX_PATH];
		int		size;
		if ((size = ::recv(conInfo->sd, buf, sizeof(buf) -1, 0)) > 0)
			buf[size] = 0;
		if (size <= 0 || shareMng->GetAcceptableFileInfo(conInfo, buf, &fileInfo) == FALSE)
		{
			connectList.DelObj(conInfo);
			::closesocket(conInfo->sd);
			delete conInfo;
			return	FALSE;
		}
	}

	SendFileObj	*obj = new SendFileObj;
	memset(obj, 0, sizeof(SendFileObj));
	obj->conInfo = conInfo;
	obj->hFile = INVALID_HANDLE_VALUE;
	obj->fileInfo = fileInfo.fileInfo;
	obj->offset = fileInfo.offset;
	obj->packetNo = fileInfo.packetNo;
	obj->host = fileInfo.host;
	obj->command = GET_MODE(fileInfo.command);
	obj->conInfo->startTick = obj->conInfo->lastTick = ::GetTickCount();
	obj->attachTime = fileInfo.attachTime;
	connectList.DelObj(conInfo);
	sendFileList.AddObj(obj);

	if (GetFileInfomation(obj->fileInfo->Fname(), &obj->fdata) == FALSE)
		return	EndSendFile(obj), FALSE;

	obj->isDir = (obj->fdata.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) ? TRUE : FALSE;
	obj->status = obj->isDir || obj->command == IPMSG_GETDIRFILES ? FS_DIRFILESTART : FS_TRANSFILE;

	if (*obj->fdata.cFileName == 0)
		ForcePathToFname(obj->fileInfo->Fname(), obj->fdata.cFileName);

	BOOL	ret;
	if (obj->isDir)
	{
		ret = obj->command == IPMSG_GETDIRFILES ? TRUE : FALSE;
		obj->hDir = (HANDLE *)malloc((MAX_PATH/2) * sizeof(HANDLE));
	}
	else {
		if ((cfg->fileTransOpt & FT_STRICTDATE) && *(_int64 *)&obj->fdata.ftLastWriteTime > *(_int64 *)&obj->attachTime)
			ret = FALSE, obj->status = FS_COMPLETE;		// 共有情報から消去
		else if (obj->command == IPMSG_GETDIRFILES)
			ret = TRUE;
		else
			ret = OpenSendFile(obj->fileInfo->Fname(), obj);
	}
	if (ret == FALSE)
		return	EndSendFile(obj), FALSE;

	DWORD	id;	// 使わず（95系で error になるのを防ぐだけ）
	obj->hThread = (HANDLE)~0;	// 微妙な領域を避ける
	// thread 内では MT 対応が必要な crt は使わず
	if ((obj->hThread = ::CreateThread(NULL, 0, SendFileThread, obj, 0, &id)) == NULL)
		return	EndSendFile(obj), FALSE;

	return	TRUE;
}

BOOL TMainWin::OpenSendFile(const char *fname, SendFileObj *obj)
{
	DWORD	lowSize, highSize, viewSize;

	if ((obj->hFile = ::CreateFile(fname, GENERIC_READ, FILE_SHARE_READ|FILE_SHARE_WRITE, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0)) != INVALID_HANDLE_VALUE)
	{
		lowSize = ::GetFileSize(obj->hFile, &highSize);
		if ((obj->fileSize = (_int64)highSize << 32 | lowSize) == 0)
			return	TRUE;
		obj->hMap = ::CreateFileMapping(obj->hFile, 0, PAGE_READONLY, highSize, lowSize, 0);
		viewSize = (int)(obj->fileSize > cfg->ViewMax ? cfg->ViewMax : obj->fileSize);
		highSize = (int)(obj->offset >> 32);
		lowSize = (int)((obj->offset / cfg->ViewMax) * cfg->ViewMax);
		obj->mapAddr = (char *)::MapViewOfFile(obj->hMap, FILE_MAP_READ, highSize, lowSize, viewSize);
		if (obj->mapAddr && IsBadReadPtr(obj->mapAddr, 1))
		{
			CloseSendFile(obj);
			return	FALSE;
		}
	}
	return	obj->mapAddr ? TRUE : FALSE;
}

BOOL TMainWin::CloseSendFile(SendFileObj *obj)
{
	if (obj == NULL)
		return	FALSE;

	::UnmapViewOfFile(obj->mapAddr);obj->mapAddr= NULL;
	::CloseHandle(obj->hMap);		obj->hMap	= NULL;
	::CloseHandle(obj->hFile);		obj->hFile	= INVALID_HANDLE_VALUE;
	obj->offset = 0;

	return	TRUE;
}

DWORD WINAPI TMainWin::SendFileThread(void *_sendFileObj)
{
	SendFileObj	*obj = (SendFileObj *)_sendFileObj;
	fd_set		fds;
	fd_set		*rfds = NULL, *wfds = &fds;
	timeval		tv;
	int			sock_ret;
	BOOL		ret = FALSE, completeWait = FALSE;
	BOOL		(TMainWin::*SendFileFunc)(SendFileObj *obj) =
				obj->command == IPMSG_GETDIRFILES ? TMainWin::SendDirFile : TMainWin::SendFile;

	FD_ZERO(&fds);
	FD_SET(obj->conInfo->sd, &fds);

	for (int waitCnt=0; waitCnt < 180 && obj->hThread != NULL; waitCnt++)
	{
		tv.tv_sec = 1, tv.tv_usec = 0;

		if ((sock_ret = ::select(obj->conInfo->sd + 1, rfds, wfds, NULL, &tv)) > 0)
		{
			waitCnt = 0;

			if (completeWait)
			{	// dummy read により、相手側の socket クローズによる EOF を検出
				if (::recv(obj->conInfo->sd, (char *)&ret, sizeof(ret), 0) >= 0)
					ret = TRUE;
				break;
			}
			else if ((mainWin->*SendFileFunc)(obj) != TRUE)
				break;
			else if (obj->status == FS_COMPLETE)
			{
				completeWait = TRUE, rfds = &fds, wfds = NULL;
				// 過去β7以前の互換性のため
				if (obj->fileSize == 0) { ret = TRUE; break; }
			}
		}
		else if (sock_ret == 0) {
			FD_ZERO(&fds);
			FD_SET(obj->conInfo->sd, &fds);
		}
		else if (sock_ret == SOCKET_ERROR) {
			break;
		}
	}

	if (obj->isDir)
	{
		mainWin->CloseSendFile(obj);
		while (--obj->dirCnt >= 0)
			::FindClose(obj->hDir[obj->dirCnt]);
	}

	obj->status = ret ? FS_COMPLETE : FS_ERROR;
	mainWin->PostMessage(WM_TCPEVENT, obj->conInfo->sd, FD_CLOSE);
	::ExitThread(0);
	return	0;
}

int MakeDirHeader(SendFileObj *obj, BOOL find)
{
	int				len;
	WIN32_FIND_DATA	*dat = &obj->fdata;
	DWORD			attr = dat->dwFileAttributes, ipmsg_attr;

	ipmsg_attr = (find == FALSE ? IPMSG_FILE_RETPARENT : (attr & FILE_ATTRIBUTE_DIRECTORY)
			? IPMSG_FILE_DIR : IPMSG_FILE_REGULAR) |
		(attr & FILE_ATTRIBUTE_READONLY ? IPMSG_FILE_RONLYOPT : 0) |
		(attr & FILE_ATTRIBUTE_HIDDEN ? IPMSG_FILE_HIDDENOPT : 0) |
		(attr & FILE_ATTRIBUTE_SYSTEM ? IPMSG_FILE_SYSTEMOPT : 0);

	if (find)
		len = wsprintf(obj->header, "0000:%s:%x%08x:%x:%x=%x:%x=%x:", dat->cFileName,
				dat->nFileSizeHigh, dat->nFileSizeLow, ipmsg_attr,
				IPMSG_FILE_MTIME, FileTime2UnixTime(&dat->ftLastWriteTime),
				IPMSG_FILE_CREATETIME, FileTime2UnixTime(&dat->ftCreationTime));
	else if (*(_int64 *)&dat->ftLastWriteTime)
		len = wsprintf(obj->header, "0000:.:0:%x:%x=%x:%x=%x:", ipmsg_attr,
				IPMSG_FILE_MTIME, FileTime2UnixTime(&dat->ftLastWriteTime),
				IPMSG_FILE_CREATETIME, FileTime2UnixTime(&dat->ftCreationTime));
	else
		len = wsprintf(obj->header, "0000:.:0:%x:", ipmsg_attr);

	obj->header[wsprintf(obj->header, "%04x", len)] = ':';

	return	len;
}

/*
	ファイル送受信
*/
BOOL TMainWin::SendDirFile(SendFileObj *obj)
{
	BOOL	find = FALSE;

	if (obj->status == FS_OPENINFO)
	{
		char	buf[MAX_BUF];
		if (obj->dirCnt == 0)
			strncpyz(buf, obj->fileInfo->Fname(), MAX_PATH);
		else if (MakePath(buf, obj->path, *obj->fdata.cAlternateFileName ? obj->fdata.cAlternateFileName : obj->fdata.cFileName) >= MAX_PATH)
			return	FALSE;
		strncpyz(obj->path, buf, MAX_PATH);
		obj->dirCnt++;
		obj->status = FS_FIRSTINFO;
	}

	if (obj->status == FS_FIRSTINFO || obj->status == FS_NEXTINFO)
	{
		if (obj->status == FS_FIRSTINFO)
		{
			char	buf[MAX_BUF];
			MakePath(buf, obj->path, "*");
			find = (obj->hDir[obj->dirCnt -1] = ::FindFirstFile(buf, &obj->fdata)) == INVALID_HANDLE_VALUE ? FALSE : TRUE;
		}
		else find = ::FindNextFile(obj->hDir[obj->dirCnt -1], &obj->fdata);

		while (find && (strcmp(obj->fdata.cFileName, ".") == 0 || strcmp(obj->fdata.cFileName, "..") == 0))
			find = ::FindNextFile(obj->hDir[obj->dirCnt -1], &obj->fdata);
		obj->status = FS_MAKEINFO;
	}

	if (obj->status == FS_DIRFILESTART || obj->status == FS_MAKEINFO)
	{
		if (obj->status == FS_DIRFILESTART)
			find = TRUE;
		if (find && (obj->dirCnt > 0 || obj->isDir == FALSE) && (obj->fdata.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) == 0)
		{
			char	buf[MAX_BUF];
			int		len = obj->isDir ? MakePath(buf, obj->path, *obj->fdata.cAlternateFileName ? obj->fdata.cAlternateFileName : obj->fdata.cFileName) : wsprintf(buf, "%s", obj->fileInfo->Fname());
			BOOL	modifyCheck = (cfg->fileTransOpt & FT_STRICTDATE) && *(_int64 *)&obj->fdata.ftLastWriteTime > *(_int64 *)&obj->attachTime;
			if (len >= MAX_PATH || modifyCheck || OpenSendFile(buf, obj) == FALSE)
			{
				len = strlen(obj->fdata.cFileName);
				strncpyz(obj->fdata.cFileName + len, " (Can't open)", MAX_PATH - len);
				obj->fdata.nFileSizeHigh = obj->fdata.nFileSizeLow = 0;
			}
		}
		if (find == FALSE && obj->isDir)
			GetFileInfomation(obj->path, &obj->fdata);

		obj->headerOffset = 0;
		obj->headerLen = MakeDirHeader(obj, find);
		if (find == FALSE)
		{
			if (--obj->dirCnt >= 0 && obj->isDir)
			{
				::FindClose(obj->hDir[obj->dirCnt]);
				if (PathToDir(obj->path, obj->path) != TRUE && obj->dirCnt > 0)
					return	FALSE;
			}
			if (obj->dirCnt <= 0)
				obj->dirCnt--;	// 終了
		}
		obj->status = FS_TRANSINFO;
	}

	if (obj->status == FS_TRANSINFO)
	{
		int	size = ::send(obj->conInfo->sd, obj->header + obj->headerOffset, obj->headerLen - obj->headerOffset, 0);
		if (size < 0)
			return	FALSE;
		else {
			if ((obj->headerOffset += size) < obj->headerLen)
				return	TRUE;
			obj->status = obj->dirCnt < 0 ? FS_COMPLETE : find == FALSE ? FS_NEXTINFO : (obj->fdata.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) ? FS_OPENINFO : FS_TRANSFILE;
		}
	}

	if (obj->status == FS_TRANSFILE)
	{
		if (obj->mapAddr && SendFile(obj) != TRUE)
		{
			CloseSendFile(obj);
			return	FALSE;
		}
		else if (obj->mapAddr == NULL || obj->status == FS_ENDFILE)
		{
			CloseSendFile(obj);
			obj->status = obj->isDir ? FS_NEXTINFO : FS_MAKEINFO;
		}
	}
	return	TRUE;
}

/*
	ファイル送受信
*/
BOOL TMainWin::SendFile(SendFileObj *obj)
{
	if (obj == NULL || obj->hFile == INVALID_HANDLE_VALUE)
		return	FALSE;

	int		size = 0;
	_int64	remain = obj->fileSize - obj->offset;
	int		transMax = cfg->TransMax - (int)(obj->offset % cfg->TransMax);

	if (remain > 0 && (size = ::send(obj->conInfo->sd, obj->mapAddr + (obj->offset % cfg->ViewMax), (int)(remain > transMax ? transMax : remain), 0)) < 0)
		return	FALSE;

	obj->offset += size;

	if (obj->offset == obj->fileSize)
		obj->status = obj->command == IPMSG_GETDIRFILES ? FS_ENDFILE : FS_COMPLETE;
	else if ((obj->offset % cfg->ViewMax) == 0)	// 再マップの必要
	{
		::UnmapViewOfFile(obj->mapAddr);
		remain = obj->fileSize - obj->offset;
		obj->mapAddr = (char *)::MapViewOfFile(obj->hMap, FILE_MAP_READ, (int)(obj->offset >> 32), (int)obj->offset, (int)(remain > cfg->ViewMax ? cfg->ViewMax : remain));
	}

	obj->conInfo->lastTick = ::GetTickCount();

	return	TRUE;
}

BOOL TMainWin::EndSendFile(SendFileObj *obj)
{
	if (obj == NULL)
		return	FALSE;

	if (obj->hThread)
	{
		HANDLE	hThread = obj->hThread;
		obj->hThread = 0;	// 中断の合図
		::WaitForSingleObject(hThread, INFINITE);
		::CloseHandle(hThread);
	}
	if (::closesocket(obj->conInfo->sd) != 0)
		obj->status = FS_ERROR;	// error 扱いにする

	CloseSendFile(obj);

	if (obj->isDir)
		free(obj->hDir);

	shareMng->EndHostShare(obj->packetNo, &obj->host->hostSub, obj->fileInfo, obj->status == FS_COMPLETE ? TRUE : FALSE);
	sendFileList.DelObj(obj);
	delete obj->conInfo;
	delete obj;
	return	TRUE;
}

/*
	Entry Packet受信処理
*/
void TMainWin::MsgBrEntry(MsgBuf *msg)
{
	AddrObj *obj = (AddrObj *)ansList->GetObj(FREE_LIST);
	if (obj) {
		obj->addr = msg->hostSub.addr;
		obj->portNo = msg->hostSub.portNo;
		ansList->PutObj(USED_LIST, obj);
	}
	if (obj == NULL || SetAnswerQueue(obj) != TRUE)
		msgMng->Send(&msg->hostSub, IPMSG_ANSENTRY|HostStatus(), GetNickNameEx(), cfg->GroupNameStr);

	AddHost(&msg->hostSub, msg->command, msg->msgBuf, msg->msgBuf + msg->exOffset);
}

BOOL TMainWin::SetAnswerQueue(AddrObj *obj)
{
	if (ansTimerID)
		return	TRUE;

	int		hostCnt = hosts.HostCnt(), spawn;

	if (hostCnt < 50 || ((msgMng->GetLocalHost()->addr ^ obj->addr) << 8) == 0)
		spawn = 1023 & rand();
	else if (hostCnt < 300)
		spawn = 2047 & rand();
	else
		spawn = 4095 & rand();

	if ((ansTimerID = ::SetTimer(hWnd, IPMSG_ANS_TIMER, spawn, NULL)) == 0)
		return	FALSE;

	return	TRUE;
}

void TMainWin::ExecuteAnsQueue(void)
{
	AddrObj *obj;

	while ((obj = (AddrObj *)ansList->GetObj(USED_LIST)) != NULL) {
		msgMng->Send(obj->addr, obj->portNo, IPMSG_ANSENTRY|HostStatus(), GetNickNameEx(), cfg->GroupNameStr);
		ansList->PutObj(FREE_LIST, obj);
	}
}

/*
	Exit Packet受信処理
*/
void TMainWin::MsgBrExit(MsgBuf *msg)
{
	Host *host = cfg->priorityHosts.GetHostByName(&msg->hostSub);
	if (host != NULL)
		host->updateTime = Time();
	DelHost(&msg->hostSub);

	for (ShareInfo *info=shareMng->Top(),*next; info; info = next)
	{
		next = shareMng->Next(info);
		shareMng->EndHostShare(info->packetNo, &msg->hostSub);
	}
}

/*
	AnsEntry Packet受信処理
*/
void TMainWin::MsgAnsEntry(MsgBuf *msg)
{
	AddHost(&msg->hostSub, msg->command, msg->msgBuf, msg->msgBuf + msg->exOffset);
}

/*
	Absence Packet受信処理
*/
void TMainWin::MsgBrAbsence(MsgBuf *msg)
{
	AddHost(&msg->hostSub, msg->command, msg->msgBuf, msg->msgBuf + msg->exOffset);
}

/*
	Send Packet受信処理
*/
void TMainWin::MsgSendMsg(MsgBuf *msg)
{
	TRecvDlg	*recvDlg;

	if (TRecvDlg::GetCreateCnt() >= cfg->RecvMax)
		return;

	for (recvDlg = (TRecvDlg *)recvList.TopObj(); recvDlg != NULL; recvDlg = (TRecvDlg *)recvList.NextObj(recvDlg))
		if (recvDlg->IsSamePacket(msg))
			break;

	if (recvDlg != NULL || IsLastPacket(msg))
	{
		if ((msg->command & IPMSG_SENDCHECKOPT) != 0 && (msg->command & (IPMSG_BROADCASTOPT | IPMSG_AUTORETOPT)) == 0)
			msgMng->Send(&msg->hostSub, IPMSG_RECVMSG, msg->packetNo);
		return;
	}

	if ((msg->command & IPMSG_BROADCASTOPT) == 0 && (msg->command & IPMSG_AUTORETOPT) == 0)
	{
		if ((msg->command & IPMSG_SENDCHECKOPT) != 0)
			msgMng->Send(&msg->hostSub, IPMSG_RECVMSG, msg->packetNo);

		if (cfg->AbsenceCheck && *cfg->AbsenceStr[cfg->AbsenceChoice])
			msgMng->Send(&msg->hostSub, IPMSG_SENDMSG|IPMSG_AUTORETOPT, cfg->AbsenceStr[cfg->AbsenceChoice]);
		if ((msg->command & IPMSG_NOADDLISTOPT) == 0 && hosts.GetHostByAddr(&msg->hostSub) == NULL)
			BroadcastEntrySub(&msg->hostSub, IPMSG_BR_ENTRY);
	}
	RecvDlgOpen(msg);
}

/*
	Recv Packet受信処理
*/
void TMainWin::MsgRecvMsg(MsgBuf *msg)
{
	MsgInfoSub(msg);
}

/*
	Read Packet受信処理
*/
void TMainWin::MsgReadMsg(MsgBuf *msg)
{
	MsgInfoSub(msg);
}

/*
	HostList 送出可能問合せ Packet受信処理
*/
void TMainWin::MsgBrIsGetList(MsgBuf *msg)
{
	if (cfg->AllowSendList
		&& (entryStartTime + ((ULONG)cfg->ListGetMSec / 1000) < (ULONG)Time())
			&& (cfg->ListGet == FALSE || (IPMSG_RETRYOPT & msg->command)))
		msgMng->Send(&msg->hostSub, IPMSG_OKGETLIST);
}

/*
	HostList 送出可能通知 Packet受信処理
*/
void TMainWin::MsgOkGetList(MsgBuf *msg)
{
	if (entryStartTime != IPMSG_GETLIST_FINISH)
		msgMng->Send(&msg->hostSub, IPMSG_GETLIST);
}

/*
	HostList 送出要求 Packet受信処理
*/
void TMainWin::MsgGetList(MsgBuf *msg)
{
	if (cfg->AllowSendList)
		SendHostList(msg);
}

/*
	HostList 送出 Packet受信処理
*/
void TMainWin::MsgAnsList(MsgBuf *msg)
{
	if (entryStartTime != IPMSG_GETLIST_FINISH)
		AddHostList(msg);
}

/*
	Version Information 要求 Packet受信処理
*/
void TMainWin::MsgGetInfo(MsgBuf *msg)
{
	char	buf[MAX_LISTBUF];

	wsprintf(buf, WIN32EDITION_MSGSTR, GetVerionStr());
	msgMng->Send(&msg->hostSub, IPMSG_SENDINFO, buf);
}

/*
	Version Information 通知 Packet受信処理
*/
void TMainWin::MsgSendInfo(MsgBuf *msg)
{
	MsgInfoSub(msg);
}

/*
	Public Key 要求 Packet受信処理
*/
void TMainWin::MsgGetPubKey(MsgBuf *msg)
{
	int		capa = strtoul(msg->msgBuf, 0, 16);

	if ((capa &= (cfg->pubKey.Capa()|cfg->smallPubKey.Capa())) == 0)
		return;

	PubKey	*pubKey = (capa & IPMSG_RSA_1024) ? &cfg->pubKey : &cfg->smallPubKey;

	char	buf[MAX_BUF];
	wsprintf(buf, "%X:%X-", cfg->pubKey.Capa()|cfg->smallPubKey.Capa(), pubKey->Exponent());
	bin2hexstr_bigendian(pubKey->Key(), pubKey->KeyLen(), buf + strlen(buf));
	msgMng->Send(&msg->hostSub, IPMSG_ANSPUBKEY, buf);
}

/*
	Public Key 送信 Packet受信処理
*/
void TMainWin::MsgAnsPubKey(MsgBuf *msg)
{
	if (cfg->pubKey.Key() == NULL && cfg->smallPubKey.Key() == NULL)
		return;

	BYTE	key[MAX_BUF];
	int		key_len, e, capa;
	char	*capa_hex, *e_hex, *key_hex, *p;

	if ((capa_hex = separate_token(msg->msgBuf, ':', &p)) == NULL)
		return;
	if ((e_hex = separate_token(NULL, '-', &p)) == NULL)
		return;
	if ((key_hex = separate_token(NULL, ':', &p)) == NULL)
		return;

	capa = strtoul(capa_hex, 0, 16);
	e = strtoul(e_hex, 0, 16);
	hexstr2bin_bigendian(key_hex, key, sizeof(key), &key_len);

	for (TSendDlg *sendDlg = (TSendDlg *)sendList.TopObj(); sendDlg != NULL; sendDlg = (TSendDlg *)sendList.NextObj(sendDlg))
		if (sendDlg->SendPubKeyNotify(&msg->hostSub, key, key_len, e, capa))
			break;
}

/*
	Information 通知処理
*/
void TMainWin::MsgInfoSub(MsgBuf *msg)
{
	int		cmd = GET_MODE(msg->command);

	if (cmd == IPMSG_READMSG)
	{
		if (GET_OPT(msg->command) & IPMSG_READCHECKOPT)
			msgMng->Send(&msg->hostSub, IPMSG_ANSREADMSG, msg->packetNo);
	}
	else {
		long	packet_no = (cmd == IPMSG_RECVMSG || cmd == IPMSG_ANSREADMSG) ? atol(msg->msgBuf) : 0;

		if (cmd == IPMSG_ANSREADMSG) {
			for (TRecvDlg *recvDlg = (TRecvDlg *)recvList.TopObj(); recvDlg != NULL; recvDlg = (TRecvDlg *)recvList.NextObj(recvDlg))
				if (recvDlg->SendFinishNotify(&msg->hostSub, packet_no))
					break;
			return;
		}
		for (TSendDlg *sendDlg = (TSendDlg *)sendList.TopObj(); sendDlg != NULL; sendDlg = (TSendDlg *)sendList.NextObj(sendDlg))
			if (sendDlg->SendFinishNotify(&msg->hostSub, packet_no))
				break;
		if (sendDlg == NULL)
			return;
	}
	if (IsLastPacket(msg))		//重複チェック
		return;

	char	title[MAX_LISTBUF], *msg_text = msg->msgBuf;
	int		show_mode = cfg->MsgMinimize ? SW_MINIMIZE : SW_SHOW;
	MakeListString(cfg, &msg->hostSub, &hosts, title);
	separate_token(title, '(');

	switch (cmd)
	{
	case IPMSG_READMSG:
		if (cfg->OpenCheck == FALSE)
			return;
		{
			char	*str = Ctime(), *p;
			str = separate_token(str, ' ', &p);
			str = separate_token(NULL, 0, &p);
			*strrchr(str, ':') = 0;
			wsprintf(msg_text, "%s\r\n(%s)", OPENFIN_MSGSTR, str);
		}
		break;

	case IPMSG_SENDINFO:
	case IPMSG_SENDABSENCEINFO:
		show_mode = SW_SHOW;
		break;

	default:
		return;
	}

	if (cmd == IPMSG_SENDABSENCEINFO) {	//将来的には TMsgDlgで処理
		static int msg_cnt = 0;	// TMsgDlg 化した後は TMsgDlg::createCnt に移行
		if (msg_cnt >= cfg->RecvMax)
			return;
		msg_cnt++;
		MessageBox(msg_text, title);
		msg_cnt--;
	}
	else {
		if (TMsgDlg::GetCreateCnt() >= cfg->RecvMax * 4)
			return;
		TMsgDlg	*msgDlg = new TMsgDlg(IsNewShell() ? this : 0);
		msgDlg->Create(msg_text, title, show_mode);
		if (cmd == IPMSG_SENDINFO || cmd == IPMSG_SENDABSENCEINFO)
			ActiveDlg(msgDlg);
		msgList.AddObj(msgDlg);
	}
}

/*
	不在通知 Information 要求 Packet受信処理
*/
void TMainWin::MsgGetAbsenceInfo(MsgBuf *msg)
{
	msgMng->Send(&msg->hostSub, IPMSG_SENDABSENCEINFO, cfg->AbsenceCheck ? cfg->AbsenceStr[cfg->AbsenceChoice] : NOTABSENCE_MSGSTR);
}

/*
	不在通知 Information 通知 Packet受信処理
*/
void TMainWin::MsgSendAbsenceInfo(MsgBuf *msg)
{
	MsgInfoSub(msg);
}

/*
	添付ファイル破棄通知 Packet受信処理
*/
void TMainWin::MsgReleaseFiles(MsgBuf *msg)
{
	int	packet_no = atoi(msg->msgBuf);

	shareMng->EndHostShare(packet_no, &msg->hostSub);
}

/*
	送信Dialog生成。ただし、同一の迎撃送信Dialogが開いている場合は、
	そのDialogを Activeにするのみ。
*/
BOOL TMainWin::SendDlgOpen(HWND hRecvWnd, MsgBuf *msg)
{
	TSendDlg *sendDlg;

	if (hRecvWnd)
	{
		for (sendDlg = (TSendDlg *)sendList.TopObj(); sendDlg != NULL; sendDlg = (TSendDlg *)sendList.NextObj(sendDlg))
			if (sendDlg->GetRecvWnd() == hRecvWnd && sendDlg->IsSending() != TRUE)
				return	ActiveDlg(sendDlg), TRUE;
	}

	if ((sendDlg = new TSendDlg(msgMng, shareMng, &hosts, cfg, logmng, hRecvWnd, msg)) == NULL)
		return	FALSE;

	sendList.AddObj(sendDlg);
	sendDlg->Create(), sendDlg->Show();
	ControlIME(sendDlg, TRUE);

// test
	if (hosts.HostCnt() == 0 && cfg->ListGet == FALSE)
		BroadcastEntrySub(inet_addr("127.0.0.1"), htons(portNo), IPMSG_BR_ENTRY);

	return	TRUE;
}

/*
	送信Dialog Hide通知(WM_SENDDLG_HIDE)処理。
	伝えてきた送信Dialogに対応する、受信Dialogを破棄
*/
void TMainWin::SendDlgHide(TSendDlg *sendDlg)
{
	ControlIME(sendDlg, FALSE);

	if (sendDlg->GetRecvWnd() && cfg->NoErase == FALSE)
	{
		TRecvDlg *recvDlg;

		for (recvDlg = (TRecvDlg *)recvList.TopObj(); recvDlg != NULL; recvDlg = (TRecvDlg *)recvList.NextObj(recvDlg))
		{
			if (recvDlg->hWnd == sendDlg->GetRecvWnd() && recvDlg->IsClosable())
			{
				recvDlg->EvCommand(0, IDCANCEL, 0);
				break;
			}
		}
	}
}

/*
	送信Dialog Exit通知(WM_SENDDLG_EXIT)処理。
	伝えてきた送信Dialog および対応する受信Dialogの破棄
*/
void TMainWin::SendDlgExit(TSendDlg *sendDlg)
{
	if (sendDlg->IsSending() == FALSE)	// 送信中の場合は HIDE で実行済み
		ControlIME(sendDlg, FALSE);
	sendList.DelObj(sendDlg);
	delete sendDlg;
}

/*
	受信Dialogを生成
*/
BOOL TMainWin::RecvDlgOpen(MsgBuf *msg)
{
	TRecvDlg *recvDlg;

	if ((recvDlg = new TRecvDlg(msgMng, msg, &hosts, cfg, logmng)) == NULL)
		return	FALSE;
	if (recvDlg->Status() == FALSE)		// 暗号文の復号に失敗した
	{
		delete recvDlg;
		return	FALSE;
	}

	recvList.AddObj(recvDlg);

	if (cfg->NoBeep == FALSE)
	{
		char	*soundFile = cfg->SoundFile;
#if 0
		Host	*host = hosts.GetHostByAddr(&msg->hostSub);

		if (host)
		{
			int priorityLevel = (host->priority - DEFAULT_PRIORITY) / PRIORITY_OFFSET;

			if (priorityLevel >= 0 && priorityLevel < cfg->PriorityMax)
			{
				if (cfg->PrioritySound[priorityLevel])
					soundFile = cfg->PrioritySound[priorityLevel];
			}
		}
#endif
		if (*soundFile == '\0' || PlaySound(soundFile, 0, SND_FILENAME|SND_ASYNC) != TRUE)
			if (MessageBeep(MB_OK) != TRUE)
				MessageBeep((UINT)~0);
	}

	if (cfg->NoPopupCheck || (cfg->AbsenceNonPopup && cfg->AbsenceCheck))
	{
		if (reverseTimerStatus == 0)
		{
			ReverseIcon(TRUE);
			if (::SetTimer(hWnd, IPMSG_REVERSEICON, IPMSG_TIMERINTERVAL, NULL))
				reverseTimerStatus = IPMSG_REVERSEICON;
		}
	}
	else {
		recvDlg->Create();
		recvDlg->Show();
		recvDlg->SetForceForegroundWindow();
	}

	return	TRUE;
}

/*
	受信Dialogを破棄
*/
void TMainWin::RecvDlgExit(TRecvDlg *recvDlg)
{
	recvList.DelObj(recvDlg);
	delete recvDlg;
}

/*
	確認Dialogを破棄
*/
void TMainWin::MsgDlgExit(TMsgDlg *msgDlg)
{
	msgList.DelObj(msgDlg);
	delete msgDlg;
}

/*
	Setup/About/Absence Dialogを生成
*/
void TMainWin::MiscDlgOpen(TDlg *dlg)
{
	if (dlg->hWnd == NULL)
		dlg->Create(), dlg->Show();
	else
		ActiveDlg(dlg);
}

/*
	TaskTrayに指定iconを登録
*/
BOOL TMainWin::TaskBar(int nimMode, HICON hSetIcon, LPCSTR tip)
{
	NOTIFYICONDATA	tn;

	memset(&tn, 0, sizeof(tn));
	tn.cbSize = sizeof(tn);
	tn.hWnd = hWnd;
	tn.uID = IPMSG_DEFAULT_PORT;		// test
	tn.uFlags = NIF_MESSAGE|(hSetIcon ? NIF_ICON : 0)|(tip ? NIF_TIP : 0);
	tn.uCallbackMessage = WM_IPMSG_NOTIFY;
	tn.hIcon = hSetIcon;
	if (tip)
		strcpy(tn.szTip, tip);

	return	::Shell_NotifyIcon(nimMode, &tn);
}

/*
	MainWindow or TaskTray Icon を clickした時の Popup Menu生成
*/
void TMainWin::Popup(UINT resId)
{
	HMENU	hMenu = ::LoadMenu(TApp::hI, (LPCSTR)resId);
	HMENU	hSubMenu = ::GetSubMenu(hMenu, 0);	//かならず、最初の項目に定義
	DWORD	val = ::GetMessagePos();
	POINTS	pos = MAKEPOINTS(val);

	if (hMenu == NULL || hSubMenu == NULL)
		return;

	ShareCntInfo	info;
	shareMng->GetShareCntInfo(&info);

	char	buf[MAX_LISTBUF] = DOWNLOAD_MSGSTR;
	if (info.packetCnt)
		wsprintf(buf + strlen(buf), "(%d/%d)", info.fileCnt, info.transferCnt);
	::InsertMenu(hSubMenu, 0, MF_BYPOSITION|MF_SEPARATOR, NULL, NULL);
	::InsertMenu(hSubMenu, 0, MF_BYPOSITION|MF_STRING, MENU_SHARE, buf);

	AddAbsenceMenu(hSubMenu, 2);

	SetForegroundWindow();		//とっても大事！

	::TrackPopupMenu(hSubMenu, TPM_RIGHTBUTTON, pos.x, pos.y, 0, hWnd, NULL);

	::DestroyMenu(hMenu);
}

/*
	NonPopup状態になっていた受信Dialogがあれば、出現させる
*/
BOOL TMainWin::PopupCheck(void)
{
	BOOL		result = FALSE; 
	TRecvDlg	*recvDlg;

	for (recvDlg = (TRecvDlg *)recvList.TopObj(); recvDlg != NULL; recvDlg = (TRecvDlg *)recvList.NextObj(recvDlg))
	{
		if (recvDlg->hWnd == 0)
		{
			recvDlg->Create(), recvDlg->Show();
			result = TRUE;
		}
	}

	if (result)
	{
		::KillTimer(hWnd, IPMSG_REVERSEICON);
		reverseTimerStatus = 0;
		SetIcon(cfg->AbsenceCheck == FALSE ? hMainIcon : hRevIcon);
	}

	return	result;
}

/*
	全Windowを前面にもってくる。hide == TRUEの場合、全Windowを hideに
*/
void TMainWin::ActiveChildWindow(BOOL active)
{
	ActiveDlg(aboutDlg, active);
	ActiveDlg(setupDlg, active);
	ActiveDlg(absenceDlg, active);
	ActiveDlg(shareStatDlg, active);
	ActiveListDlg(&recvList, active);
	ActiveListDlg(&sendList, active);
	ActiveListDlg(&msgList, active);

	if (active == FALSE)
		SetForegroundWindow();
}

/*
	HostDataのcopy
*/
inline void TMainWin::SetHostData(Host *destHost, HostSub *hostSub, ULONG command, time_t update_time, char *nickName, char *groupName, int priority)
{
	destHost->hostStatus = GET_OPT(command);
	destHost->hostSub = *hostSub;
	destHost->updateTime = update_time;
	destHost->priority = priority;
	strncpyz(destHost->nickName, nickName, sizeof(destHost->nickName));
	strncpyz(destHost->groupName, groupName, sizeof(destHost->groupName));
}

/*
	Host追加処理
*/
void TMainWin::AddHost(HostSub *hostSub, ULONG command, char *nickName, char *groupName)
{
	Host	*host, *tmp_host, *priorityHost;
	time_t	now_time = Time();
	int		priority = DEFAULT_PRIORITY;

	if (GET_MODE(command) == IPMSG_BR_ENTRY && (command & IPMSG_DIALUPOPT) && !IsSameHost(hostSub, msgMng->GetLocalHost()))
	{
		for (AddrObj *obj = (AddrObj *)cfg->DialUpList.TopObj(); obj != NULL; obj = (AddrObj *)cfg->DialUpList.NextObj(obj))
			if (obj->addr == hostSub->addr && obj->portNo == hostSub->portNo)
				break;

		if (obj == NULL)
		{
			obj = new AddrObj;
			obj->addr	= hostSub->addr;
			obj->portNo	= hostSub->portNo;
			cfg->DialUpList.AddObj(obj);
		}
	}

	if ((priorityHost = cfg->priorityHosts.GetHostByName(hostSub)) != NULL)
	{
		priority = priorityHost->priority;
//		command |= priorityHost->hostStatus & IPMSG_ENCRYPTOPT;
	}

	if ((host = hosts.GetHostByName(hostSub)) != NULL)
	{
		if (host->hostSub.addr != hostSub->addr || host->hostSub.portNo != hostSub->portNo)
		{
			if ((tmp_host = hosts.GetHostByAddr(hostSub)) != NULL)
			{
				for (TSendDlg *sendDlg = (TSendDlg *)sendList.TopObj(); sendDlg != NULL; sendDlg = (TSendDlg *)sendList.NextObj(sendDlg))
					sendDlg->DelHost(tmp_host);
				hosts.DelHost(tmp_host);
				if (tmp_host->RefCnt() == 0)
					delete tmp_host;
			}
			hosts.DelHost(host);
			host->hostSub.addr = hostSub->addr;
			host->hostSub.portNo = hostSub->portNo;
			hosts.AddHost(host);
		}
		if (((command ^ host->hostStatus) & (IPMSG_ABSENCEOPT|IPMSG_FILEATTACHOPT|IPMSG_ENCRYPTOPT)) || strcmp(host->nickName, nickName) || strcmp(host->groupName, groupName))
		{
			SetHostData(host, hostSub, command, now_time, nickName, groupName, priority);
			for (TSendDlg *sendDlg = (TSendDlg *)sendList.TopObj(); sendDlg != NULL; sendDlg = (TSendDlg *)sendList.NextObj(sendDlg))
				sendDlg->ModifyHost(host);
		}
		else {
			host->hostStatus = GET_OPT(command);
			host->updateTime = now_time;
		}
		return;
	}

	if ((host = hosts.GetHostByAddr(hostSub)) != NULL)
	{
		for (TSendDlg *sendDlg = (TSendDlg *)sendList.TopObj(); sendDlg != NULL; sendDlg = (TSendDlg *)sendList.NextObj(sendDlg))
			sendDlg->DelHost(host);
		hosts.DelHost(host);
		if (host->RefCnt() == 0)
			delete host;
	}

	if ((host = priorityHost) == NULL)
		host = new Host;

	SetHostData(host, hostSub, command, now_time, nickName, groupName, priority);
	hosts.AddHost(host);
//	if (priorityHost == NULL)
//		cfg->priorityHosts.AddHost(host);

	SetCaption();

	for (TSendDlg *sendDlg = (TSendDlg *)sendList.TopObj(); sendDlg != NULL; sendDlg = (TSendDlg *)sendList.NextObj(sendDlg))
		sendDlg->AddHost(host);
}

/*
	全Hostの削除処理
*/
void TMainWin::DelAllHost(void)
{
	for (TSendDlg *sendDlg = (TSendDlg *)sendList.TopObj(); sendDlg != NULL; sendDlg = (TSendDlg *)sendList.NextObj(sendDlg))
		sendDlg->DelAllHost();

	while (hosts.HostCnt() > 0)
		DelHostSub(hosts.GetHost(0));
}

/*
	特定Hostの削除処理
*/
void TMainWin::DelHost(HostSub *hostSub)
{
	Host *host;

	if ((host = hosts.GetHostByAddr(hostSub)) != NULL)
		DelHostSub(host);
}

/*
	特定Hostの削除処理Sub
*/
void TMainWin::DelHostSub(Host *host)
{
	for (TSendDlg *sendDlg = (TSendDlg *)sendList.TopObj(); sendDlg != NULL; sendDlg = (TSendDlg *)sendList.NextObj(sendDlg))
		sendDlg->DelHost(host);

	for (AddrObj *obj = (AddrObj *)cfg->DialUpList.TopObj(); obj != NULL; obj = (AddrObj *)cfg->DialUpList.NextObj(obj))
	{
		if (obj->addr == host->hostSub.addr && obj->portNo == host->hostSub.portNo)
		{
			cfg->DialUpList.DelObj(obj);
			delete obj;
			break;
		}
	}
	hosts.DelHost(host);
	if (host->RefCnt() == 0)
		delete host;
	else if (host->pubKey.Key())
		host->pubKey.UnSet();
	SetCaption();
}

/*
	更新Button用処理。所有HostListの削除、および再Entry Packet送出。
	なお、unRemoveFlg == TRUEの場合は、削除処理を行わない
*/
void TMainWin::RefreshHost(BOOL unRemoveFlg)
{
	time_t	now_time = Time();

	if (cfg->ListGet && entryTimerStatus != 0 || refreshStartTime + IPMSG_ENTRYMINSEC >= now_time)
		return;

	if (unRemoveFlg != TRUE)
	{
		if (cfg->UpdateTime == 0 || refreshStartTime + cfg->UpdateTime < now_time) {
			DelAllHost();
		}
		else {
			for (int cnt=0; cnt < hosts.HostCnt(); )
			{
				if (hosts.GetHost(cnt)->updateTime + cfg->UpdateTime < now_time)
					DelHostSub(hosts.GetHost(cnt));
				else
					cnt++;
			}
		}
	}
	refreshStartTime = now_time;
	EntryHost();
}

/*
	Main Window or TaskTray用 Captionの設定
*/
void TMainWin::SetCaption(void)
{
	char	buf[MAX_LISTBUF];

	wsprintf(buf, "IPMsg(%d)", hosts.HostCnt());

	if (IsNewShell())
		TaskBar(NIM_MODIFY, NULL, buf);
	else
		::SetWindowText(hWnd, buf);
}

/*
	IPMSG_ENTRY/IPMSG_EXIT/IPMSG_ABSENCEパケット送出
*/
void TMainWin::BroadcastEntry(ULONG mode)
{
	for (TBroadcastObj *brobj=cfg->broadcastList.Top(); brobj; brobj=cfg->broadcastList.Next(brobj))
		BroadcastEntrySub(brobj->Addr(cfg->ResolveOpt & RS_REALTIME), htons(portNo), IPMSG_NOOPERATION);
	BroadcastEntrySub((ULONG)~0, htons(portNo), IPMSG_NOOPERATION);

	this->Sleep(cfg->DelayTime);

	UINT host_status = mode | HostStatus();

	msgMng->Send((ULONG)~0, htons(portNo), host_status, GetNickNameEx(), cfg->GroupNameStr);	// local network broadcast

	for (brobj=cfg->broadcastList.Top(); brobj; brobj=cfg->broadcastList.Next(brobj))
		BroadcastEntrySub(brobj->Addr(), htons(portNo), host_status);

	for (AddrObj *obj = (AddrObj *)cfg->DialUpList.TopObj(); obj != NULL; obj = (AddrObj *)cfg->DialUpList.NextObj(obj))
		BroadcastEntrySub(obj->addr, obj->portNo, host_status);

	if (mode == IPMSG_BR_ENTRY && cfg->ExtendEntry)
		::SetTimer(hWnd, IPMSG_ENTRY_TIMER, IPMSG_ENTRYMINSEC * 1000, NULL);
}

/*
	IPMSG_ENTRY/IPMSG_EXIT/IPMSG_ABSENCEパケット送出Sub
*/
void TMainWin::BroadcastEntrySub(ULONG addr, int port_no, ULONG mode)
{
	if (addr == 0)
		return;

	HostSub	hostSub;

	hostSub.addr 	= addr;
	hostSub.portNo	= port_no;

	BroadcastEntrySub(&hostSub, mode);
}

void TMainWin::BroadcastEntrySub(HostSub *hostSub, ULONG mode)
{
	if (mode == IPMSG_NOOPERATION)
		msgMng->Send(hostSub, mode);
	else
		msgMng->Send(hostSub, mode | (cfg->DialUpCheck ? IPMSG_DIALUPOPT : 0) | HostStatus(), GetNickNameEx(),  cfg->GroupNameStr);
}

/*
	icon 反転処理。startFlg == TRUEの場合、逆iconに Resetされる
*/
void TMainWin::ReverseIcon(BOOL startFlg)
{
	static	cnt = 0;

	if (startFlg)
		cnt = 0;

	SetIcon(cnt++ % 2 ? hMainIcon : hRevIcon);
}

/*
	指定iconを MainWindow or TaskTray にセット
*/
void TMainWin::SetIcon(HICON hSetIcon)
{
	if (IsNewShell())
		TaskBar(NIM_MODIFY, hSetIcon);
	else {
		::SetClassLong(hWnd, GCL_HICON, (LONG)hSetIcon);
		::FlashWindow(hWnd, FALSE);
	}
}

/*
	HostList送出処理
	手抜き ... デリミタやデータがない場合に特殊文字('\a','\b')を使用
*/
void TMainWin::SendHostList(MsgBuf *msg)
{
	int		start_no, len, total_len = 0, host_cnt = 0;
	char	*buf = new char [MAX_UDPBUF];
	char	tmp[MAX_BUF];

	if ((start_no = atoi(msg->msgBuf)) < 0)
		start_no = 0;

	total_len = wsprintf(buf, "%5d%c%5d%c", 0, HOSTLIST_SEPARATOR, 0, HOSTLIST_SEPARATOR);

	for (int cnt=start_no; cnt < hosts.HostCnt(); cnt++)
	{
		Host	*host = hosts.GetHost(cnt);
		if ((len = wsprintf(tmp, "%s%c%s%c%ld%c%s%c%d%c%s%c%s%c",
			host->hostSub.userName, HOSTLIST_SEPARATOR,
			host->hostSub.hostName, HOSTLIST_SEPARATOR,
			host->hostStatus, HOSTLIST_SEPARATOR,
			inet_ntoa(*(LPIN_ADDR)&host->hostSub.addr), HOSTLIST_SEPARATOR,
			host->hostSub.portNo, HOSTLIST_SEPARATOR,
			*host->nickName ? host->nickName : HOSTLIST_DUMMY, HOSTLIST_SEPARATOR,
			*host->groupName ? host->groupName : HOSTLIST_DUMMY, HOSTLIST_SEPARATOR
			)) + total_len +80 >= MAX_UDPBUF)	// +80 : ipmsg protocol header reserve
		{
			break;
		}
		memcpy(buf + total_len, tmp, len +1);
		total_len += len;
		host_cnt++;
	}
	len = wsprintf(tmp, "%5d%c%5d", start_no + host_cnt == hosts.HostCnt() ? 0 : start_no + host_cnt, HOSTLIST_SEPARATOR, host_cnt);
	memcpy(buf, tmp, len);
	msgMng->Send(&msg->hostSub, IPMSG_ANSLIST, buf);
	delete [] buf;
}

/*
	HostList受信処理
*/
void TMainWin::AddHostList(MsgBuf *msg)
{
	char	*tok, *nickName, *groupName, *p;
	HostSub	hostSub;
	ULONG	host_status;
	int		total_num, continue_cnt;

	if ((tok = separate_token(msg->msgBuf, HOSTLIST_SEPARATOR, &p)) == NULL)
		return;
	continue_cnt = atoi(tok);

	if ((tok = separate_token(NULL, HOSTLIST_SEPARATOR, &p)) == NULL)
		return;
	total_num = atoi(tok);

	for (int host_cnt=0; (tok = separate_token(NULL, HOSTLIST_SEPARATOR, &p)) != NULL; host_cnt++)
	{
		nickName = groupName = NULL;
		strncpyz(hostSub.userName, tok, sizeof(hostSub.userName));

		if ((tok = separate_token(NULL, HOSTLIST_SEPARATOR, &p)) == NULL)
			break;
		strncpyz(hostSub.hostName, tok, sizeof(hostSub.hostName));

		if ((tok = separate_token(NULL, HOSTLIST_SEPARATOR, &p)) == NULL)
			break;
		host_status = GET_OPT(atol(tok));

		if ((tok = separate_token(NULL, HOSTLIST_SEPARATOR, &p)) == NULL)
			break;
		hostSub.addr = inet_addr(tok);

		if ((tok = separate_token(NULL, HOSTLIST_SEPARATOR, &p)) == NULL)
			break;
		hostSub.portNo = atoi(tok);

		if ((nickName = separate_token(NULL, HOSTLIST_SEPARATOR, &p)) == NULL)
			break;
		if (strcmp(nickName, HOSTLIST_DUMMY) == 0)
			nickName = "";

		if ((groupName = separate_token(NULL, HOSTLIST_SEPARATOR, &p)) == NULL)
			break;
		if (strcmp(groupName, HOSTLIST_DUMMY) == 0)
			groupName = "";

		AddHost(&hostSub, IPMSG_BR_ENTRY|host_status, nickName, groupName);
	}

	if (continue_cnt || host_cnt < total_num)
	{
		msgMng->Send(&msg->hostSub, IPMSG_GETLIST, abs(continue_cnt - (total_num - host_cnt)));
		if (::SetTimer(hWnd, IPMSG_LISTGETRETRY_TIMER, cfg->ListGetMSec, NULL))
			entryTimerStatus = IPMSG_LISTGETRETRY_TIMER;
	}
	else {
		entryStartTime = IPMSG_GETLIST_FINISH;
		if (cfg->ListGet)
			BroadcastEntry(IPMSG_BR_ENTRY);
	}
}

/*
	Logを開く
*/
void TMainWin::LogOpen(void)
{
	SHELLEXECUTEINFO	shellExecInfo;

	memset(&shellExecInfo, 0, sizeof(shellExecInfo));
	shellExecInfo.cbSize = sizeof(SHELLEXECUTEINFO);
	shellExecInfo.fMask = SEE_MASK_NOCLOSEPROCESS;
	shellExecInfo.hwnd = NULL;
	shellExecInfo.lpFile = cfg->LogFile;
	shellExecInfo.nShow = SW_SHOWNORMAL;

	if (!ShellExecuteEx(&shellExecInfo) || (int)shellExecInfo.hInstApp <= WINEXEC_ERR_MAX)
	{
		switch ((int)shellExecInfo.hInstApp)
		{
		case SE_ERR_NOASSOC: case SE_ERR_ASSOCINCOMPLETE:
			MessageBox(FILEEXTERR_MSGSTR);
			break;
		default:
			MessageBox(CANTOPENLOG_MSGSTR);
			break;
		}
	}
//	WaitForSingleObject(shellExecInfo.hProcess, INFINITE);
}

/*
	自Hostの Status (dialup mode / absence mode)
*/
ULONG TMainWin::HostStatus(void)
{
	return	(cfg->DialUpCheck ? IPMSG_DIALUPOPT : 0) | (cfg->AbsenceCheck ? IPMSG_ABSENCEOPT : 0) | (msgMng->IsAvailableTCP() ? IPMSG_FILEATTACHOPT : 0) | (cfg->pubKey.Key() || cfg->smallPubKey.Key() ? IPMSG_ENCRYPTOPT : 0);
}

/*
	IPMSG用 icon初期化
*/
void  TMainWin::InitIcon(void)
{
	if (*cfg->IconFile == 0 || (hMainIcon = ::ExtractIcon(TApp::hI, cfg->IconFile, 0)) == NULL)
		hMainIcon = ::LoadIcon(TApp::hI, (LPCSTR)IPMSG_ICON);
	if (*cfg->RevIconFile == 0 || (hRevIcon = ::ExtractIcon(TApp::hI, cfg->RevIconFile, 0)) == NULL)
		hRevIcon = ::LoadIcon(TApp::hI, (LPCSTR)RECV_ICON);
}

/*
	MainWindow iconを教えてあげる。
	この routineは static member function であり、SendDlgなどから呼ばれる。
*/
HICON TMainWin::GetIPMsgIcon(void)
{
	return	hMainIcon;
}

/*
	ListDlg を Active or Hideに（単なる簡易ルーチン）
*/
void TMainWin::ActiveListDlg(TList *list, BOOL active)
{
	for (TListDlg *dlg = (TListDlg *)list->TopObj(); dlg != NULL; dlg = (TListDlg *)list->NextObj(dlg))
		ActiveDlg(dlg, active);
}

/*
	ListDlg を Delete する（単なる簡易ルーチン）
*/
void TMainWin::DeleteListDlg(TList *list)
{
	TListDlg *dlg;

	while ((dlg = (TListDlg *)list->TopObj()) != NULL)
	{
		list->DelObj(dlg);
		delete dlg;
	}
}

/*
	Dlg を Active or Hideに
*/
void TMainWin::ActiveDlg(TDlg *dlg, BOOL active)
{
	if (dlg->hWnd != 0)
		active ? dlg->Show(), dlg->SetForegroundWindow() : dlg->Show(SW_HIDE);
}

/*
	Extend NickName
*/
char *TMainWin::GetNickNameEx(void)
{
	static char buf[MAX_LISTBUF];

	if (cfg->AbsenceCheck && *cfg->AbsenceHead[cfg->AbsenceChoice])
		wsprintf(buf, "%s[%s]", *cfg->NickNameStr ? cfg->NickNameStr : msgMng->GetLocalHost()->userName, cfg->AbsenceHead[cfg->AbsenceChoice]);
	else
		strcpy(buf, *cfg->NickNameStr ? cfg->NickNameStr : msgMng->GetLocalHost()->userName);

	return	buf;
}

/* VC4 で CryptoAPI を使用可能にする */
BOOL (WINAPI *pCryptAcquireContext)(HCRYPTPROV *, LPCSTR, LPCSTR, DWORD, DWORD);
BOOL (WINAPI *pCryptDestroyKey)(HCRYPTKEY);
BOOL (WINAPI *pCryptGetKeyParam)(HCRYPTKEY, DWORD, BYTE *, DWORD *, DWORD);
BOOL (WINAPI *pCryptSetKeyParam)(HCRYPTKEY, DWORD, BYTE *, DWORD);
BOOL (WINAPI *pCryptExportKey)(HCRYPTKEY, HCRYPTKEY, DWORD, DWORD, BYTE *, DWORD *);
BOOL (WINAPI *pCryptGetUserKey)(HCRYPTPROV, DWORD, HCRYPTKEY *);
BOOL (WINAPI *pCryptEncrypt)(HCRYPTKEY, HCRYPTHASH, BOOL, DWORD, BYTE *, DWORD *, DWORD);
BOOL (WINAPI *pCryptGenKey)(HCRYPTPROV, ALG_ID, DWORD, HCRYPTKEY *);
BOOL (WINAPI *pCryptGenRandom)(HCRYPTPROV, DWORD, BYTE *);
BOOL (WINAPI *pCryptImportKey)(HCRYPTPROV, CONST BYTE *, DWORD, HCRYPTKEY, DWORD, HCRYPTKEY *);
BOOL (WINAPI *pCryptDecrypt)(HCRYPTKEY, HCRYPTHASH, BOOL, DWORD, BYTE *, DWORD *);
BOOL (WINAPI *pCryptReleaseContext)(HCRYPTPROV, DWORD);

BOOL (WINAPI *pCryptCreateHash)(HCRYPTPROV, ALG_ID, HCRYPTKEY, DWORD, HCRYPTHASH *);
BOOL (WINAPI *pCryptHashData)(HCRYPTHASH, BYTE *, DWORD, DWORD);
BOOL (WINAPI *pCryptSignHash)(HCRYPTHASH, DWORD, LPCSTR, DWORD, BYTE *, DWORD *);
BOOL (WINAPI *pCryptVerifySignature)(HCRYPTHASH, CONST BYTE *, DWORD, HCRYPTKEY, LPCSTR, DWORD);
BOOL (WINAPI *pCryptProtectData)(DATA_BLOB* pDataIn, LPCWSTR szDataDescr, DATA_BLOB* pOptionalEntropy, PVOID pvReserved, CRYPTPROTECT_PROMPTSTRUCT* pPromptStruct, DWORD dwFlags, DATA_BLOB* pDataOut);
BOOL (WINAPI *pCryptUnprotectData)(DATA_BLOB* pDataIn, LPWSTR* ppszDataDescr, DATA_BLOB* pOptionalEntropy, PVOID pvReserved, CRYPTPROTECT_PROMPTSTRUCT* pPromptStruct, DWORD dwFlags, DATA_BLOB* pDataOut);


BOOL TMainWin::SetupCryptAPI(void)
{
	HINSTANCE	advdll;

	cfg->hCsp = cfg->hSmallCsp = NULL;

	if ((advdll = ::LoadLibrary("advapi32.dll")) == NULL)
		return	GetLastErrorMsg("advapi32"), FALSE;

	pCryptAcquireContext = (BOOL (WINAPI *)(HCRYPTPROV *, LPCSTR, LPCSTR, DWORD, DWORD))::GetProcAddress(advdll, "CryptAcquireContextA");
	pCryptDestroyKey = (BOOL (WINAPI *)(HCRYPTKEY))::GetProcAddress(advdll, "CryptDestroyKey");
	pCryptGetKeyParam = (BOOL (WINAPI *)(HCRYPTKEY, DWORD, BYTE *, DWORD *, DWORD))::GetProcAddress(advdll, "CryptGetKeyParam");
	pCryptSetKeyParam = (BOOL (WINAPI *)(HCRYPTKEY, DWORD, BYTE *, DWORD))::GetProcAddress(advdll, "CryptSetKeyParam");
	pCryptExportKey = (BOOL (WINAPI *)(HCRYPTKEY, HCRYPTKEY, DWORD, DWORD, BYTE *, DWORD *))::GetProcAddress(advdll, "CryptExportKey");
	pCryptGetUserKey = (BOOL (WINAPI *)(HCRYPTPROV, DWORD, HCRYPTKEY *))::GetProcAddress(advdll, "CryptGetUserKey");
	pCryptEncrypt = (BOOL (WINAPI *)(HCRYPTKEY, HCRYPTHASH, BOOL, DWORD, BYTE *, DWORD *, DWORD))::GetProcAddress(advdll, "CryptEncrypt");
	pCryptGenKey = (BOOL (WINAPI *)(HCRYPTPROV, ALG_ID, DWORD, HCRYPTKEY *))::GetProcAddress(advdll, "CryptGenKey");
	pCryptGenRandom = (BOOL (WINAPI *)(HCRYPTPROV, DWORD, BYTE *))::GetProcAddress(advdll, "CryptGenRandom");
	pCryptImportKey = (BOOL (WINAPI *)(HCRYPTPROV, CONST BYTE *, DWORD, HCRYPTKEY, DWORD, HCRYPTKEY *))::GetProcAddress(advdll, "CryptImportKey");
	pCryptDecrypt = (BOOL (WINAPI *)(HCRYPTKEY, HCRYPTHASH, BOOL, DWORD, BYTE *, DWORD *))::GetProcAddress(advdll, "CryptDecrypt");
	pCryptCreateHash = (BOOL (WINAPI *)(HCRYPTPROV, ALG_ID, HCRYPTKEY, DWORD, HCRYPTHASH *))::GetProcAddress(advdll, "CryptCreateHash");
	pCryptHashData = (BOOL (WINAPI *)(HCRYPTHASH, BYTE *, DWORD, DWORD))::GetProcAddress(advdll, "CryptHashData");
	pCryptSignHash = (BOOL (WINAPI *)(HCRYPTHASH, DWORD, LPCSTR, DWORD, BYTE *, DWORD *))::GetProcAddress(advdll, "CryptSignHashA");
	pCryptVerifySignature = (BOOL (WINAPI *)(HCRYPTHASH, CONST BYTE *, DWORD, HCRYPTKEY, LPCSTR, DWORD))::GetProcAddress(advdll, "CryptVerifySignatureA");
	pCryptReleaseContext = (BOOL (WINAPI *)(HCRYPTPROV, DWORD))::GetProcAddress(advdll, "CryptReleaseContext");

	HINSTANCE	cryptdll;
	if (cryptdll = ::LoadLibrary("crypt32.dll"))
	{
		pCryptProtectData = (BOOL (WINAPI *)(DATA_BLOB* pDataIn, LPCWSTR szDataDescr, DATA_BLOB* pOptionalEntropy, PVOID pvReserved, CRYPTPROTECT_PROMPTSTRUCT* pPromptStruct, DWORD dwFlags, DATA_BLOB* pDataOut))::GetProcAddress(cryptdll, "CryptProtectData");
		pCryptUnprotectData = (BOOL (WINAPI *)(DATA_BLOB* pDataIn, LPWSTR* ppszDataDescr, DATA_BLOB* pOptionalEntropy, PVOID pvReserved, CRYPTPROTECT_PROMPTSTRUCT* pPromptStruct, DWORD dwFlags, DATA_BLOB* pDataOut))::GetProcAddress(cryptdll, "CryptUnprotectData");
	}
	if (pCryptProtectData == NULL && cfg->privEncryptType == PRIV_BLOB_DPAPI)
		cfg->privEncryptType = PRIV_BLOB_RAW;

	if (pCryptAcquireContext == NULL)
		return	GetLastErrorMsg("CryptAcquireContext"), FALSE;

	SetupCryptAPICore();

// 起動直後に、まれにコケる環境があるらしいので、わずかにリトライ...
#define MAX_RETRY	3
	int	cnt = 0;
	while (cfg->hCsp && cfg->pubKey.Key() == NULL || cfg->hSmallCsp && cfg->smallPubKey.Key() == NULL)
	{
		if (++cnt > MAX_RETRY)
			break;
		if (cfg->hCsp)
			pCryptReleaseContext(cfg->hCsp, 0), cfg->hCsp = NULL;
		if (cfg->hSmallCsp)
			pCryptReleaseContext(cfg->hSmallCsp, 0), cfg->hSmallCsp = NULL;
		::Sleep(1000);
		SetupCryptAPICore(cnt == MAX_RETRY ? KEY_DIAG : 0);
	}
	if (cnt > MAX_RETRY || cfg->pubKey.Key() == NULL && cfg->smallPubKey.Key() == NULL)
	{
		if (MessageBox("RSA failed. Create New RSA key?", "msg", MB_OKCANCEL) == IDOK)
			SetupCryptAPICore(KEY_REBUILD|KEY_DIAG);
	}

#if 0
	if (cfg->pubKey.Key())
	{
		char	hash[MAX_NAMEBUF];
		MakeHash(cfg->pubKey.Key(), cfg->pubKey.KeyLen(), hash);
		int	len = strlen(msgMng->GetLocalHost()->userName);
		wsprintf(msgMng->GetLocalHost()->userName + min(len, 32), "@%s", hash);
	}
#endif

	return	cfg->pubKey.Key() || cfg->smallPubKey.Key();
}

BOOL TMainWin::SetupCryptAPICore(int ctl_flg)
{
	BYTE	data[MAX_BUF];
	int		len = sizeof(data);

// RSA 鍵の生成
	SetupRSAKey(1024, ctl_flg);
	SetupRSAKey(512, ctl_flg);

// Self Check 1024bit
	if (cfg->pubKey.Key()) {
		BOOL		ret = FALSE;
		HCRYPTKEY	hExKey = 0;

		cfg->pubKey.KeyBlob(data, sizeof(data), &len);
		if (pCryptImportKey(cfg->hCsp, data, len, 0, 0, &hExKey)) {
			len = 128/8;
			if (pCryptEncrypt(hExKey, 0, TRUE, 0, data, (DWORD *)&len, MAX_BUF))
				ret = TRUE;
			else if (ctl_flg & KEY_DIAG) GetLastErrorMsg("CryptEncrypt test1024");
			pCryptDestroyKey(hExKey);
		}
		else if (ctl_flg & KEY_DIAG) GetLastErrorMsg("CryptImportKey test1024");

		if (ret) {
			ret = FALSE;
			if (pCryptDecrypt(cfg->hPrivKey, 0, TRUE, 0, (BYTE *)data, (DWORD *)&len))
				ret = TRUE;
			else if (ctl_flg & KEY_DIAG) GetLastErrorMsg("CryptDecrypt test1024");
		}

		if (ret == FALSE)
			cfg->pubKey.UnSet();
	}

// Self Check 512bit
	if (cfg->smallPubKey.Key()) {
		BOOL		ret = FALSE;
		BYTE		tmp[MAX_BUF];
		DWORD		tmplen = MAX_BUF / 2;
		HCRYPTKEY	hKey = 0, hExKey = 0;

		cfg->smallPubKey.KeyBlob(data, sizeof(data), &len);
		if (pCryptImportKey(cfg->hSmallCsp, data, len, 0, 0, &hExKey)) {
			if (pCryptGenKey(cfg->hSmallCsp, CALG_RC2, CRYPT_EXPORTABLE, &hKey)) {
				pCryptExportKey(hKey, hExKey, SIMPLEBLOB, 0, NULL, (DWORD *)&len);
				if (pCryptExportKey(hKey, hExKey, SIMPLEBLOB, 0, data, (DWORD *)&len)) {
					if (pCryptEncrypt(hKey, 0, TRUE, 0, tmp, &tmplen, MAX_BUF))
						ret = TRUE;
					else if (ctl_flg & KEY_DIAG) GetLastErrorMsg("CryptEncrypt test512");
				}
				else if (ctl_flg & KEY_DIAG) GetLastErrorMsg("CryptExportKey test512");
				pCryptDestroyKey(hKey);
			}
			else if (ctl_flg & KEY_DIAG) GetLastErrorMsg("CryptGenKey test512");
			pCryptDestroyKey(hExKey);
		}
		else if (ctl_flg & KEY_DIAG) GetLastErrorMsg("CryptImportKey test512");

		if (ret) {
			ret = FALSE;
			if (pCryptImportKey(cfg->hSmallCsp, data, len, cfg->hSmallPrivKey, 0, &hKey)) {
				if (pCryptDecrypt(hKey, 0, TRUE, 0, (BYTE *)tmp, (DWORD *)&tmplen))
					ret = TRUE;
				else if (ctl_flg & KEY_DIAG) GetLastErrorMsg("CryptDecrypt test512");
				pCryptDestroyKey(hKey);
			}
			else if (ctl_flg & KEY_DIAG) GetLastErrorMsg("CryptImportKey test512");
		}

		if (ret == FALSE)
			cfg->smallPubKey.UnSet();
	}

	if (cfg->pubKey.Key() == NULL && cfg->hPrivKey)
		pCryptDestroyKey(cfg->hPrivKey), cfg->hPrivKey = NULL;
	if (cfg->smallPubKey.Key() == NULL && cfg->hSmallPrivKey)
		pCryptDestroyKey(cfg->hSmallPrivKey), cfg->hSmallPrivKey = NULL;

	return	cfg->pubKey.Key() || cfg->smallPubKey.Key();
}

BOOL TMainWin::SetupRSAKey(int bitLen, int ctl_flg)
{
	BOOL		isSmall = bitLen == 512 ? TRUE : FALSE;
	BYTE		data[MAX_BUF];
	char		*contName = (char *)data;
	int			len = sizeof(data);
	HCRYPTPROV&	hCsp = isSmall ? cfg->hSmallCsp : cfg->hCsp;
	HCRYPTKEY&	hPrivKey = isSmall ? cfg->hSmallPrivKey : cfg->hPrivKey;
	PubKey&		pubKey = isSmall ? cfg->smallPubKey : cfg->pubKey;
	const char	*csp_name = isSmall ? MS_DEF_PROV : MS_ENHANCED_PROV;

	int	cap = isSmall ? IPMSG_RSA_512|IPMSG_RC2_40 : IPMSG_RSA_1024|IPMSG_BLOWFISH_128;
	int	SmallAcqFlags[] = { CRYPT_MACHINE_KEYSET, 0, CRYPT_NEWKEYSET|CRYPT_MACHINE_KEYSET, CRYPT_NEWKEYSET, -1 };
	int	BigAcqFlags[] = { CRYPT_MACHINE_KEYSET, CRYPT_NEWKEYSET|CRYPT_MACHINE_KEYSET, -1 };
	int	*AcqFlgs = isSmall ? SmallAcqFlags : BigAcqFlags;

	wsprintf(contName, "ipmsg.rsa%d.%s", bitLen, msgMng->GetLocalHost()->userName);

// rebuld 時には、事前に公開鍵を消去
	if ((ctl_flg & KEY_REBUILD) && hCsp && pubKey.Key() == NULL)
	{
		pCryptReleaseContext(hCsp, 0), hCsp = NULL;
		if (!pCryptAcquireContext(&hCsp, contName, csp_name, PROV_RSA_FULL, CRYPT_DELETEKEYSET|CRYPT_MACHINE_KEYSET))
			if (ctl_flg & KEY_DIAG) GetLastErrorMsg("CryptAcquireContext(destroy)");
		pCryptAcquireContext(&hCsp, contName, csp_name, PROV_RSA_FULL, CRYPT_DELETEKEYSET);
	}

// open key cotainer
	for (int i=0; AcqFlgs[i] != -1; i++)
	{
		hCsp = NULL;
		if (pCryptAcquireContext(&hCsp, contName, csp_name, PROV_RSA_FULL, AcqFlgs[i]))
			break;
	}
	if (hCsp == NULL)
	{
		if (isSmall && (ctl_flg & KEY_DIAG))
			GetLastErrorMsg("CryptAcquireContext");
		return	FALSE;
	}

// プライベート鍵をimport (only 1024bit)
	if (!isSmall && cfg->privBlob)
	{
		if (LoadPrivBlob(data, &len) && !pCryptImportKey(hCsp, data, len, 0, CRYPT_EXPORTABLE, &hPrivKey))
		{	// import is fail...
			if (ctl_flg & KEY_DIAG)
				GetLastErrorMsg("CryptImportKey(blob)");
			// コケた場合、再Acquireしないと副作用が残ることがある...
			pCryptReleaseContext(hCsp, 0), hCsp = NULL;
			pCryptAcquireContext(&hCsp, contName, csp_name, PROV_RSA_FULL, CRYPT_MACHINE_KEYSET);
		}
		if (hPrivKey == NULL)
		{
			if (cfg->privEncryptType == PRIV_BLOB_USER && cfg->privEncryptSeed)
			{
				TaskBar(NIM_DELETE);
				::ExitProcess(0xffffffff);		// パスワードが一致しない場合、即終了
			}
			delete [] cfg->privBlob;
			cfg->privBlob = NULL;
		}
	}

// 初回 or 512bit 鍵の場合は、hCsp からプライベート鍵ハンドルを取得
	if (hPrivKey == NULL)
	{
		if (!pCryptGetUserKey(hCsp, AT_KEYEXCHANGE, &hPrivKey)
		&& !pCryptGenKey(hCsp, CALG_RSA_KEYX, CRYPT_EXPORTABLE, &hPrivKey))
			if (ctl_flg & KEY_DIAG)
				GetLastErrorMsg("CryptGenKey");
	}

// 公開鍵を export
	if (pCryptExportKey(hPrivKey, 0, PUBLICKEYBLOB, 0, data, (DWORD *)&len))
		pubKey.SetByBlob(data, cap);
	else if (ctl_flg & KEY_DIAG) GetLastErrorMsg("CryptExportKey");

// プライベート鍵を保存
	if (!isSmall && cfg->privBlob == NULL && hPrivKey)
	{
		len = sizeof(data);
		if (pCryptExportKey(hPrivKey, 0, PRIVATEKEYBLOB, 0, data, (DWORD *)&len))
			StorePrivBlob(data, len);
	}
	return	TRUE;
}

BOOL TMainWin::LoadPrivBlob(BYTE *rawBlob, int *rawBlobLen)
{
	if (cfg->privBlob == NULL)
		return	FALSE;

	BYTE	key[MAX_BUF];

	if (cfg->privEncryptType == PRIV_BLOB_RAW)
	{
		memcpy(rawBlob, cfg->privBlob, *rawBlobLen = cfg->privBlobLen);
		return	TRUE;
	}
	else if (cfg->privEncryptType == PRIV_BLOB_USER)
	{
		if (cfg->privEncryptSeed == NULL)
			return	FALSE;
		while (1)
		{
			TPasswordDlg	dlg((char *)key, this);
			if (dlg.Exec() == FALSE)
				return	FALSE;
			CBlowFish	bl(key, strlen((char *)key));
			if (bl.Decrypt(cfg->privEncryptSeed, key, cfg->privEncryptSeedLen) == PRIV_SEED_LEN && memcmp(key, PRIV_SEED_HEADER, PRIV_SEED_HEADER_LEN) == 0)
				break;
		}
	}
	else if (cfg->privEncryptType == PRIV_BLOB_DPAPI)
	{
		if (cfg->privEncryptSeed == NULL)
			return	FALSE;
		DATA_BLOB	in = { cfg->privEncryptSeedLen, cfg->privEncryptSeed }, out;
		if (pCryptUnprotectData(&in, 0, 0, 0, 0, CRYPTPROTECT_LOCAL_MACHINE|CRYPTPROTECT_UI_FORBIDDEN, &out) == FALSE)
			return	FALSE;
		memcpy(key, out.pbData, out.cbData);
		::LocalFree(out.pbData);
		if (out.cbData != PRIV_SEED_LEN)
			return	FALSE;
	}
	else return	FALSE;

	CBlowFish	bl(key + PRIV_SEED_HEADER_LEN, 128/8);
	return (*rawBlobLen = bl.Decrypt(cfg->privBlob, rawBlob, cfg->privBlobLen)) != 0;
}

BOOL TMainWin::StorePrivBlob(BYTE *rawBlob, int rawBlobLen)
{
	delete	cfg->privBlob;
	cfg->privBlob = NULL;
	delete	cfg->privEncryptSeed;
	cfg->privEncryptSeed = NULL;
	cfg->privBlobLen = cfg->privEncryptSeedLen = 0;

	BYTE	data[MAX_BUF], *encodeBlob = data;

	if (cfg->privEncryptType == PRIV_BLOB_RAW)
	{
		encodeBlob = rawBlob;
		cfg->privBlobLen = rawBlobLen;
	}
	else {
		BYTE	seed[PRIV_SEED_LEN], *seedCore = seed + PRIV_SEED_HEADER_LEN;
		// seed の作成
		memcpy(seed, PRIV_SEED_HEADER, PRIV_SEED_HEADER_LEN);
		pCryptGenRandom(cfg->hCsp ? cfg->hCsp : cfg->hSmallCsp, 128/8, seedCore);

		if (cfg->privEncryptType == PRIV_BLOB_USER)
		{
			TPasswordDlg	dlg((char *)data, this);
			if (dlg.Exec() == FALSE)
				return	FALSE;
			// seed の暗号化
			CBlowFish	bl(data, strlen((char *)data));
			cfg->privEncryptSeedLen = bl.Encrypt(seed, data, PRIV_SEED_LEN);
			cfg->privEncryptSeed = new BYTE [cfg->privEncryptSeedLen];
			memcpy(cfg->privEncryptSeed, data, cfg->privEncryptSeedLen);
		}
		else if (cfg->privEncryptType == PRIV_BLOB_DPAPI)
		{
			// seed の暗号化
			DATA_BLOB in = { PRIV_SEED_LEN, seed }, out;
			if (!pCryptProtectData(&in, L"ipmsg", 0, 0, 0, CRYPTPROTECT_LOCAL_MACHINE|CRYPTPROTECT_UI_FORBIDDEN, &out))
				return	FALSE;
			cfg->privEncryptSeed = new BYTE [cfg->privEncryptSeedLen = out.cbData];
			memcpy(cfg->privEncryptSeed, out.pbData, out.cbData);
			::LocalFree(out.pbData);
		}
		else return	FALSE;
		// seed による、暗号化blob の作成
		CBlowFish	bl(seedCore, 128/8);
		cfg->privBlobLen = bl.Encrypt(rawBlob, encodeBlob, rawBlobLen);
	}

	cfg->privBlob = new BYTE [cfg->privBlobLen];
	memcpy(cfg->privBlob, encodeBlob, cfg->privBlobLen);
	cfg->WriteRegistry(CFG_CRYPT);
	return	TRUE;
}

void TMainWin::ControlIME(TWin *win, BOOL open)
{
	HWND	targetWnd = (win && win->hWnd) ? win->GetDlgItem(SEND_EDIT) : hWnd;

	if (win && win->hWnd && open == FALSE)
		::SetFocus(targetWnd);		// ATOK残像 暫定対策...

	if (cfg->ControlIME == FALSE)
		return;

	if (open == FALSE)
	{
		for (TSendDlg *sendDlg = (TSendDlg *)sendList.TopObj(); sendDlg != NULL; sendDlg = (TSendDlg *)sendList.NextObj(sendDlg))
			if (sendDlg != win && sendDlg->IsSending() != TRUE)
				return;
	}

//	SetImeOpenStatus(targetWnd, open);
	if (GetImeOpenStatus(targetWnd) != open)
		SetImeOpenStatus(targetWnd, open);
}

/*
	MainWindow を教えてあげる。
*/
HWND GetMainWnd(void)
{
	return	hMainWnd;
}

/*
	Version文字列
*/
char *GetVerionStr(void)
{
	return	strstr(mainwin_id, "Ver");
}


