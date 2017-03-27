static char *cfg_id = 
	"@(#)Copyright (C) H.Shirouzu 1996-2004   cfg.cpp	Ver2.05";
/* ========================================================================
	Project  Name			: IP Messenger for Win32
	Module Name				: Configuration
	Create					: 1996-09-27(Sat)
	Update					: 2004-01-01(Thu)
	Copyright				: H.Shirouzu
	Reference				: 
	======================================================================== */

#include <stdio.h>
#include "tlib.h"
#include "resource.h"
#include "ipmsg.h"
#include "msgstr.h"
#include "blowfish.h"

#define IPMSG_DEFAULT_LISTGETMSEC	3000
#define IPMSG_DEFAULT_RETRYMSEC		1500
#define IPMSG_DEFAULT_RETRYMAX		3
#define IPMSG_DEFAULT_RECVMAX_NT	100
#define IPMSG_DEFAULT_RECVMAX_95	30
#define IPMSG_DEFAULT_ABSENCEMAX	8
#define IPMSG_DEFAULT_FINDMAX		12
#define IPMSG_DEFAULT_DELAY			500
#define IPMSG_DEFAULT_UPDATETIME	10
#define IPMSG_DEFAULT_KEEPHOSTTIME	(3600 * 24 * 180)	// 180日間
#define IPMSG_DEFAULT_QUOTE			">"
#define IPMSG_DEFAULT_ENCRYPTNUM	20

#define IPMSG_DEFAULT_VIEWMAX		(8 * 1024 * 1024)	// 8MB
#define IPMSG_DEFAULT_TRANSMAX		(64 * 1024)			// 64KB
#define IPMSG_DEFAULT_TCPBUFMAX		(256 * 1024)		// 256KB

#define IPMSG_DEFAULT_NICKNAMEWIDTH	97
#define IPMSG_DEFAULT_ABSENCEWIDTH	16
#define IPMSG_DEFAULT_GROUPWIDTH	88
#define IPMSG_DEFAULT_HOSTWIDTH		58
#define IPMSG_DEFAULT_IPADDRWIDTH	110
#define IPMSG_DEFAULT_USERWIDTH		90
#define IPMSG_DEFAULT_PRIORITYWIDTH	21

#define NOBEEP_STR			"NoBeep"
#define LISTGET_STR			"ListGet"
#define LISTGETMSEC_STR		"ListGetMSec"
#define RETRYMSEC_STR		"RetryMSec2"
#define RETRYMAX_STR		"RetryMax"
#define RECVMAXNT_STR		"RecvMaxNT"
#define RECVMAX95_STR		"RecvMax95"
#define NOERASE_STR			"NoErase"
#define DEBUG_STR			"Debug"
#define BROADCAST_STR		"BroadCast"
#define CLICKABLEURL_STR	"ClickableUrl"
#define NOPOPUPCHECK_STR	"NoPopup"
#define OPENCHECK_STR		"OpenCheck"
#define ABSENCESAVE_STR		"AbsenceSave"
#define ABSENCECHECK_STR	"AbsenceCheck"
#define ABSENCEMAX_STR		"AbsenceMax"
#define ABSENCESTR_STR		"AbsenceStr"
#define ABSENCEHEAD_STR		"AbsenceHead"
#define FINDMAX_STR			"FindMax2"
#define FINDALL_STR			"FindAll"
#define FINDSTR_STR			"FindStr"
#define PASSWORD_STR		"PasswordStr"
#define PASSWDLOGCHECK_STR	"PasswdLogCheck"
#define DELAYTIME_STR		"DelayTime"
#define QUOTECHECK_STR		"QuoteCheck"
#define SECRETCHECK_STR		"SecretCheck"
#define IPADDRCHECK_STR		"IPAddrCheck"
#define ONECLICKPOPUP_STR	"OneClickPopup"
#define ABNORMALBTN_STR		"AbnormalButton"
#define DIALUPCHECK_STR		"DialUpCheck"
#define NICKNAMECHECK_STR	"NickNameCheck"
#define NICKNAMESTR_STR		"NickNameStr"
#define GROUPNAMESTR_STR	"GroupNameStr"
#define ABSENCENONPOPUP_STR	"AbsenceNonPopup"
#define LANG_STR			"Lang"
#define SORT_STR			"Sort"
#define UPDATETIME_STR		"UpdateTime"
#define KEEPHOSTTIME_STR	"KeepHostTime"
#define MSGMINIMIZE_STR		"MsgMinimize"
#define DEFAULTURL_STR		"DefaultUrl"
#define SHELLEXEC_STR		"ShellExec"
#define EXTENDENTRY_STR		"ExtendEntry"
#define QUOTESTR_STR		"QuoteStr"
#define CONTROLIME_STR		"ControlIME"
#define HOTKEY_STR			"HotKey"
#define HOTKEYCHECK_STR		"HotKeyCheck"
#define HOTKEYMODIFY_STR	"HotKeyModify"
#define HOTKEYSEND_STR		"HotKeySend"
#define HOTKEYRECV_STR		"HotKeyRecv"
#define HOTKEYMISC_STR		"HotKeyMisc"
#define ALLOWSENDLIST_STR	"AllowSendList"
#define FILETRANSOPT_STR	"FileTransOpt"
#define RESOLVEOPT_STR		"ResolveOpt"
#define ENCRYPTNUM_STR		"EncryptNum"

#define VIEWMAX_STR			"ViewMax"
#define TRANSMAX_STR		"TransMax"
#define TCPBUFMAX_STR		"TcpbufMax"

#define LOGCHECK_STR		"LogCheck"
#define LOGFILE_STR			"LogFile"
#define SOUNDFILE_STR		"SoundFile"
#define ICON_STR			"Icon"
#define REVICON_STR			"RevIcon"
#define LASTOPEN_STR		"lastOpen"
#define LASTSAVE_STR		"lastSave"

#define WINSIZE_STR			"WindowSize"

#define SENDPRIORITY_STR	"SendPriority"
#define SENDNICKNAME_STR	"SendNickName"
#define SENDABSENCE_STR		"SendAbsence"
#define SENDUSERNAME_STR	"SendUserName"
#define SENDGROUPNAME_STR	"SendGroupName"
#define SENDHOSTNAME_STR	"SendHostName"
#define SENDIPADDR_STR		"SendIPAddr"
#define SENDORDER_STR		"SendOrder"

#define SENDXDIFF_STR		"SendXdiff"
#define SENDYDIFF_STR		"SendYdiff"
#define SENDMIDYDIFF_STR	"SendMidYdiff"
#define SENDSAVEPOS_STR		"SendSavePos"
#define SENDXPOS_STR		"SendXpos"
#define SENDYPOS_STR		"SendYpos"

#define RECVXDIFF_STR		"RecvXdiff"
#define RECVYDIFF_STR		"RecvYdiff"
#define RECVSAVEPOS_STR		"RecvSavePos"
#define RECVXPOS_STR		"RecvXpos"
#define RECVYPOS_STR		"RecvYpos"

#define FONT_STR			"Fonts"
#define SENDEDITFONT_STR	"SendEdit"
#define SENDLISTFONT_STR	"SendListView"
#define RECVHEADFONT_STR	"RecvHead"
#define RECVEDITFONT_STR	"RecvEdit"

#define HEIGHT_STR			"Height"
#define WIDTH_STR			"Width"
#define ESCAPEMENT_STR		"Escapement"
#define ORIENTATION_STR		"Orientation"
#define WEIGHT_STR			"Weight"
#define ITALIC_STR			"Italic"
#define UNDERLINE_STR		"UnderLine"
#define STRIKEOUT_STR		"StrikeOut"
#define CHARSET_STR			"CharSet"
#define OUTPRECISION_STR	"OutPrecision"
#define CLIPPRECISION_STR	"ClipPrecision"
#define QUALITY_STR			"Quality"
#define PITCHANDFAMILY_STR	"PitchAndFamily"
#define FACENAME_STR		"FaceName"

#define HOSTINFO_STR		"HostInfo"
#define USERNAME_STR		"UserName"
#define HOSTNAME_STR		"HostName"
#define IPADDR_STR			"IPAddr"
#define PORTNO_STR			"PortNo"
#define NICKNAME_STR		"NickName"
#define GROUPNAME_STR		"GroupName"
#define PRIORITY_STR		"Priority"
#define PRIORITYMAX_STR		"PriorityMax"
#define PRIORITYSOUND_STR	"PrioritySound"
#define PRIORITYREJECT_STR	"PriorityReject"
#define GLIDLINE_STR		"GlidLine"
#define COLUMNITEMS_STR		"ColumnItems"

#define CRYPT_STR			"Crypt"
#define PRIVBLOB_STR		"PrivBlob"
#define PRIVTYPE_STR		"PrivEncryptType"
#define PRIVSEED_STR		"PrivEncryptSeed"
#define PRIVSEEDLEN_STR		"PrivEncryptSeedLen"

char	*DefaultUrlProtocol[] = { "HTTP", "HTTPS", "FTP", "FILE", "TELNET", NULL };
char	*DefaultAbsence[IPMSG_DEFAULT_ABSENCEMAX] = { DEFABSENCE1_MSGSTR, DEFABSENCE2_MSGSTR, DEFABSENCE3_MSGSTR, DEFABSENCE4_MSGSTR, DEFABSENCE5_MSGSTR, DEFABSENCE6_MSGSTR, DEFABSENCE7_MSGSTR, DEFABSENCE8_MSGSTR };
char	*DefaultAbsenceHead[IPMSG_DEFAULT_ABSENCEMAX] = { DEFABSENCEHEAD1_MSGSTR, DEFABSENCEHEAD2_MSGSTR, DEFABSENCEHEAD3_MSGSTR, DEFABSENCEHEAD4_MSGSTR, DEFABSENCEHEAD5_MSGSTR, DEFABSENCEHEAD6_MSGSTR, DEFABSENCEHEAD7_MSGSTR, DEFABSENCEHEAD8_MSGSTR };

Cfg::Cfg(ULONG _nicAddr, int _portNo)
{
	nicAddr = _nicAddr;
	portNo = _portNo;
	AbsenceHead = NULL;
	AbsenceStr = NULL;
	FindStr = NULL;
	privBlob = NULL;
}

Cfg::~Cfg()
{
	delete [] privBlob;
	delete [] FindStr;
	delete [] AbsenceHead;
	delete [] AbsenceStr;
}

BOOL Cfg::ReadRegistry(void)
{
	char	buf[MAX_LISTBUF], val[MAX_LISTBUF];

	GetRegName(buf, nicAddr, portNo);
	TRegistry	reg(HS_TOOLS, buf);

	if (reg.GetInt(NOPOPUPCHECK_STR, &NoPopupCheck) != TRUE)
	{
		GetRegName(buf, 0, portNo);
		reg.ChangeApp(HS_TOOLS, buf);
		if (reg.GetInt(NOPOPUPCHECK_STR, &NoPopupCheck) != TRUE)
			reg.ChangeApp(HS_TOOLS, IPMSG_MSGSTR);
	}

	NoBeep = FALSE;
	ListGet = FALSE;
	HotKeyCheck = FALSE;
	HotKeyModify = MOD_ALT|MOD_CONTROL;
	HotKeySend = 'S';
	HotKeyRecv = 'R';
	HotKeyMisc = 'D';
	ListGetMSec = IPMSG_DEFAULT_LISTGETMSEC;
	RetryMSec = IPMSG_DEFAULT_RETRYMSEC;
	RetryMax = IPMSG_DEFAULT_RETRYMAX;
	RecvMax = IsWinNT() ? IPMSG_DEFAULT_RECVMAX_NT : IPMSG_DEFAULT_RECVMAX_95;
	NoErase = FALSE;
	Debug = FALSE;
	NoPopupCheck = FALSE;
	OpenCheck = TRUE;
	AbsenceSave = FALSE;
	AbsenceCheck = FALSE;
	AbsenceMax = IPMSG_DEFAULT_ABSENCEMAX;
	AbsenceChoice = 0;
	FindMax = IPMSG_DEFAULT_FINDMAX;
	FindAll = FALSE;
	DelayTime = IPMSG_DEFAULT_DELAY;
	QuoteCheck = TRUE;
	SecretCheck = FALSE;
	IPAddrCheck = FALSE;
	OneClickPopup = FALSE;
	AbnormalButton = FALSE;
	DialUpCheck = FALSE;
	NickNameCheck = IPMSG_NICKNAME;
	*NickNameStr = 0;
	*GroupNameStr = 0;
	AbsenceNonPopup = TRUE;
	Sort = IPMSG_NAMESORT;
	UpdateTime = IPMSG_DEFAULT_UPDATETIME;
	KeepHostTime = IPMSG_DEFAULT_KEEPHOSTTIME;
	MsgMinimize = FALSE;
	DefaultUrl = TRUE;
	ShellExec = FALSE;
	ExtendEntry = TRUE;
	ControlIME = FALSE;
	GlidLineCheck = FALSE;
	PriorityMax = DEFAULT_PRIORITYMAX;
	PriorityReject = 0;
	AllowSendList = TRUE;
	fileTransOpt = 0;
	ResolveOpt = 0;
	EncryptNum = IPMSG_DEFAULT_ENCRYPTNUM;
	ViewMax = IPMSG_DEFAULT_VIEWMAX;
	TransMax = IPMSG_DEFAULT_TRANSMAX;
	TcpbufMax = IPMSG_DEFAULT_TCPBUFMAX;
	privBlobLen = 0;
	hPrivKey = 0;
	hSmallPrivKey = 0;
	// CryptProtectData is available only Win2K/XP
	privEncryptType = IsWin2K() ? PRIV_BLOB_DPAPI : PRIV_BLOB_RAW;
	privEncryptSeed = NULL;
	privEncryptSeedLen = 0;

	strncpyz(QuoteStr, IPMSG_DEFAULT_QUOTE, sizeof(QuoteStr));

	SendWidth[SW_NICKNAME] = IPMSG_DEFAULT_NICKNAMEWIDTH;
	SendWidth[SW_ABSENCE] = IPMSG_DEFAULT_ABSENCEWIDTH;
	SendWidth[SW_GROUP] = IPMSG_DEFAULT_GROUPWIDTH;
	SendWidth[SW_HOST] = IPMSG_DEFAULT_HOSTWIDTH;
	SendWidth[SW_IPADDR] = IPMSG_DEFAULT_IPADDRWIDTH;
	SendWidth[SW_USER] = IPMSG_DEFAULT_USERWIDTH;
	SendWidth[SW_PRIORITY] = IPMSG_DEFAULT_PRIORITYWIDTH;

	SetItem(&ColumnItems, SW_NICKNAME, TRUE);
	SetItem(&ColumnItems, SW_GROUP, TRUE);
	SetItem(&ColumnItems, SW_HOST, TRUE);

	for (int cnt=0; cnt < MAX_SENDWIDTH; cnt++)
		SendOrder[cnt] = cnt;

	SendXdiff		= 0;
	SendYdiff		= 0;
	SendMidYdiff	= 0;
	SendSavePos		= 0;
	SendXpos		= 0;
	SendYpos		= 0;

	RecvXdiff		= 0;
	RecvYdiff		= 0;
	RecvSavePos		= 0;
	RecvXpos		= 0;
	RecvYpos		= 0;

	LogCheck = FALSE;
	PasswdLogCheck = FALSE;
	strncpyz(LogFile, "ipmsg.log", sizeof(LogFile));
	*SoundFile = 0;
	*IconFile = 0;
	*RevIconFile = 0;
	*lastSaveDir = *lastOpenDir = 0;
	memset(&SendEditFont, 0, sizeof(SendEditFont));
	memset(&SendListFont, 0, sizeof(SendListFont));
	memset(&RecvHeadFont, 0, sizeof(RecvHeadFont));
	memset(&RecvEditFont, 0, sizeof(RecvEditFont));

	reg.GetInt(NOBEEP_STR, &NoBeep);
	reg.GetInt(LISTGET_STR, &ListGet);
	reg.GetInt(LISTGETMSEC_STR, (int *)&ListGetMSec);
	reg.GetInt(RETRYMSEC_STR, (int *)&RetryMSec);
	reg.GetInt(RETRYMAX_STR, (int *)&RetryMax);
	reg.GetInt(IsWinNT() ? RECVMAXNT_STR : RECVMAX95_STR, (int *)&RecvMax);
	reg.GetInt(NOERASE_STR, &NoErase);
	reg.GetInt(DEBUG_STR, &Debug);
	reg.GetInt(NOPOPUPCHECK_STR, &NoPopupCheck);
	reg.GetInt(OPENCHECK_STR, &OpenCheck);
	reg.GetInt(ABSENCESAVE_STR, &AbsenceSave);
	if (AbsenceSave)
		reg.GetInt(ABSENCECHECK_STR, &AbsenceCheck);
	reg.GetInt(ABSENCEMAX_STR, &AbsenceMax);
	reg.GetInt(ALLOWSENDLIST_STR, &AllowSendList);
	reg.GetInt(FILETRANSOPT_STR, &fileTransOpt);
	reg.GetInt(RESOLVEOPT_STR, &ResolveOpt);
	reg.GetInt(ENCRYPTNUM_STR, &EncryptNum);

// for File Transfer
	reg.GetInt(VIEWMAX_STR, &ViewMax);
	if (ViewMax < 1024 * 1024)	// 1MB 以下の MapFileOfView は認めない
		ViewMax = IPMSG_DEFAULT_VIEWMAX;
	reg.GetInt(TRANSMAX_STR, &TransMax);
	if (TransMax < 8 * 1024)	// 8KB 以下の file transfer buf は認めない
		TransMax = IPMSG_DEFAULT_TRANSMAX;
	reg.GetInt(TCPBUFMAX_STR, &TcpbufMax);
	if (TcpbufMax < 8 * 1024)	// 8KB 以下の sockbuf は認めない
		TcpbufMax = IPMSG_DEFAULT_TCPBUFMAX;

// for Absence Message
	typedef char MaxBuf[MAX_PATH];
	AbsenceStr = new MaxBuf[AbsenceMax];
	memset(AbsenceStr, 0, MAX_PATH * AbsenceMax);
	reg.GetStr(ABSENCESTR_STR, AbsenceStr[0], sizeof(AbsenceStr[0]));

	typedef char MaxHead[MAX_NAMEBUF];
	AbsenceHead = new MaxHead[AbsenceMax];
	memset(AbsenceHead, 0, MAX_NAMEBUF * AbsenceMax);

	if (reg.CreateKey(ABSENCESTR_STR))
	{
		for (int cnt=0; cnt < AbsenceMax; cnt++)
		{
			char	key[MAX_PATH];

			wsprintf(key, "%s%d", ABSENCESTR_STR, cnt);
			if (reg.GetStr(key, AbsenceStr[cnt], sizeof(AbsenceStr[cnt])) != TRUE)
				strncpyz(AbsenceStr[cnt], DefaultAbsence[cnt < IPMSG_DEFAULT_ABSENCEMAX ? cnt : 0], MAX_PATH);

			wsprintf(key, "%s%d", ABSENCEHEAD_STR, cnt);
			if (reg.GetStr(key, AbsenceHead[cnt], sizeof(AbsenceHead[cnt])) != TRUE)
				strcpy(AbsenceHead[cnt], DefaultAbsenceHead[cnt < IPMSG_DEFAULT_ABSENCEMAX ? cnt : 0]);
		}
		reg.CloseKey();
	}

// Find User
	typedef char MaxFind[MAX_NAMEBUF];
	if (reg.CreateKey(FINDSTR_STR))
	{
		reg.GetInt(FINDMAX_STR, &FindMax);
		reg.GetInt(FINDALL_STR, &FindAll);
		FindStr = new MaxFind[FindMax];
		for (int cnt=0; cnt < FindMax; cnt++)
		{
			char	key[MAX_PATH];
			wsprintf(key, "%d", cnt);
			if (reg.GetStr(key, FindStr[cnt], sizeof(FindStr[cnt])) != TRUE)
				FindStr[cnt][0] = '\0';
		}
		reg.CloseKey();
	}
	else {
		FindStr = new MaxFind[FindMax];
		memset(FindStr, 0, MAX_NAMEBUF * FindMax);
	}

	*PasswordStr = 0;
	reg.GetStr(PASSWORD_STR, PasswordStr, sizeof(PasswordStr));
	reg.GetInt(PASSWDLOGCHECK_STR, &PasswdLogCheck);
	reg.GetInt(DELAYTIME_STR, &DelayTime);
	reg.GetInt(QUOTECHECK_STR, &QuoteCheck);
	reg.GetInt(SECRETCHECK_STR, &SecretCheck);
	reg.GetInt(IPADDRCHECK_STR, &IPAddrCheck);
	reg.GetInt(ONECLICKPOPUP_STR, &OneClickPopup);
	reg.GetInt(ABNORMALBTN_STR, &AbnormalButton);
	reg.GetInt(DIALUPCHECK_STR, &DialUpCheck);
	reg.GetInt(NICKNAMECHECK_STR, &NickNameCheck);
	NickNameCheck = NickNameCheck == IPMSG_NICKNAME;
	reg.GetInt(ABSENCENONPOPUP_STR, &AbsenceNonPopup);
	reg.GetStr(NICKNAMESTR_STR, NickNameStr, sizeof(NickNameStr));
	reg.GetStr(GROUPNAMESTR_STR, GroupNameStr, sizeof(GroupNameStr));
	reg.GetStr(LANG_STR, Lang, sizeof(Lang));
	reg.GetLong(SORT_STR, (long *)&Sort);
//	reg.GetInt(UPDATETIME_STR, &UpdateTime);
	reg.GetInt(KEEPHOSTTIME_STR, &KeepHostTime);
	reg.GetInt(MSGMINIMIZE_STR, &MsgMinimize);
	reg.GetInt(DEFAULTURL_STR, &DefaultUrl);
	reg.GetInt(SHELLEXEC_STR, &ShellExec);
	reg.GetInt(EXTENDENTRY_STR, &ExtendEntry);
	reg.GetInt(CONTROLIME_STR, &ControlIME);
	reg.GetInt(GLIDLINE_STR, &GlidLineCheck);
	reg.GetInt(COLUMNITEMS_STR, (int *)&ColumnItems);
	reg.GetStr(QUOTESTR_STR, QuoteStr, sizeof(QuoteStr));
	if (reg.CreateKey(HOTKEY_STR))
	{
		reg.GetInt(HOTKEYCHECK_STR, &HotKeyCheck);
		reg.GetInt(HOTKEYMODIFY_STR, &HotKeyModify);
		reg.GetInt(HOTKEYSEND_STR, &HotKeySend);
		reg.GetInt(HOTKEYRECV_STR, &HotKeyRecv);
		reg.GetInt(HOTKEYMISC_STR, &HotKeyMisc);
		reg.CloseKey();
	}
	reg.GetInt(LOGCHECK_STR, &LogCheck);
	reg.GetStr(LOGFILE_STR, LogFile, sizeof(LogFile));
	if (LogCheck)
		LogMng::StrictLogFile(LogFile);

	reg.GetStr(SOUNDFILE_STR, SoundFile, sizeof(SoundFile));
	reg.GetStr(ICON_STR, IconFile, sizeof(IconFile));
	reg.GetStr(REVICON_STR, RevIconFile, sizeof(RevIconFile));
	reg.GetStr(LASTOPEN_STR, lastOpenDir, sizeof(lastOpenDir));
	reg.GetStr(LASTSAVE_STR, lastSaveDir, sizeof(lastSaveDir));

// Send/Recv Window Location
	if (reg.CreateKey(WINSIZE_STR))
	{
		reg.GetInt(SENDNICKNAME_STR, &SendWidth[SW_NICKNAME]);
		reg.GetInt(SENDUSERNAME_STR, &SendWidth[SW_USER]);
		reg.GetInt(SENDABSENCE_STR, &SendWidth[SW_ABSENCE]);
		reg.GetInt(SENDPRIORITY_STR, &SendWidth[SW_PRIORITY]);
		reg.GetInt(SENDGROUPNAME_STR, &SendWidth[SW_GROUP]);
		reg.GetInt(SENDHOSTNAME_STR, &SendWidth[SW_HOST]);
		reg.GetInt(SENDIPADDR_STR, &SendWidth[SW_IPADDR]);
		if (reg.CreateKey(SENDORDER_STR))
		{
			for (int cnt=0; cnt < MAX_SENDWIDTH; cnt++)
			{
				wsprintf(buf, "%d", cnt);
				reg.GetInt(buf, &SendOrder[cnt]);
			}
			reg.CloseKey();
		}
		for (cnt=0; cnt < MAX_SENDWIDTH; cnt++)
		{
			if (SendOrder[cnt] >= MAX_SENDWIDTH || SendOrder[cnt] < 0)
				break;
			for (int cnt2=cnt+1; cnt2 < MAX_SENDWIDTH; cnt2++)
				if (SendOrder[cnt] == SendOrder[cnt2])
					break;
			if (cnt2 != MAX_SENDWIDTH)
				break;
		}
		if (cnt != MAX_SENDWIDTH)
			for (cnt=0; cnt < MAX_SENDWIDTH; cnt++)
				SendOrder[cnt] = cnt;

		reg.GetInt(SENDXDIFF_STR, &SendXdiff);
		reg.GetInt(SENDYDIFF_STR, &SendYdiff);
		reg.GetInt(SENDMIDYDIFF_STR, &SendMidYdiff);
		reg.GetInt(SENDSAVEPOS_STR, &SendSavePos);
		reg.GetInt(SENDXPOS_STR, &SendXpos);
		reg.GetInt(SENDYPOS_STR, &SendYpos);

		reg.GetInt(RECVXDIFF_STR, &RecvXdiff);
		reg.GetInt(RECVYDIFF_STR, &RecvYdiff);
		reg.GetInt(RECVSAVEPOS_STR, &RecvSavePos);
		reg.GetInt(RECVXPOS_STR, &RecvXpos);
		reg.GetInt(RECVYPOS_STR, &RecvYpos);
		reg.CloseKey();
	}

// Send/Recv Window Font
	if (reg.CreateKey(FONT_STR))
	{
		ReadFontRegistry(&reg, SENDEDITFONT_STR, &SendEditFont);
		ReadFontRegistry(&reg, SENDLISTFONT_STR, &SendListFont);
		ReadFontRegistry(&reg, RECVHEADFONT_STR, &RecvHeadFont);
		ReadFontRegistry(&reg, RECVEDITFONT_STR, &RecvEditFont);
		reg.CloseKey();
	}

// Broadcast Data
	if (reg.CreateKey(BROADCAST_STR))
	{
		int		cnt = 0;
		while (1)
		{
			wsprintf(buf, "%d", cnt++);
			if (reg.GetStr(buf, val, sizeof(val)) != TRUE)
				break;
			broadcastList.SetHostRaw(val, (ResolveOpt & RS_REALTIME) ? 0 : ResolveAddr(val));
		}
		reg.CloseKey();
	}

// Clickable URL
	for (cnt=0; DefaultUrlProtocol[cnt] != NULL; cnt++)
	{
		UrlObj *obj = new UrlObj;
		strncpyz(obj->protocol, DefaultUrlProtocol[cnt], sizeof(obj->protocol));
		*obj->program = 0;
		urlList.AddObj(obj);
	}

	if (reg.CreateKey(CLICKABLEURL_STR))
	{
		for (cnt = 0; reg.EnumValue(cnt, buf, sizeof(buf)) || reg.EnumKey(cnt, buf, sizeof(buf)); cnt++)
		{
			for (UrlObj *obj = (UrlObj *)urlList.TopObj(); obj != NULL; obj = (UrlObj *)urlList.NextObj(obj))
				if (stricmp(obj->protocol, buf) == 0)
					break;

			if (obj == NULL)
			{
				obj = new UrlObj;
				urlList.AddObj(obj);
			}
			strncpyz(obj->protocol, buf, sizeof(obj->protocol));
			reg.GetStr(buf, obj->program, sizeof(obj->program));
		}
		reg.CloseKey();
	}

// User Sort Priority
	if (reg.CreateKey(PRIORITY_STR))
	{
		reg.GetInt(PRIORITYMAX_STR, &PriorityMax);
		reg.GetInt(PRIORITYREJECT_STR, &PriorityReject);
#if 0
		PrioritySound = new char *[PriorityMax];
		for (int cnt=0; cnt < PriorityMax; cnt++)
		{
			wsprintf(buf, "%d", cnt);

			if (reg.CreateKey(buf))
			{
				if (reg.GetStr(PRIORITYSOUND_STR, buf, sizeof(buf)) && *buf != '\0')
					PrioritySound[cnt] = strdup(buf);
				reg.CloseKey();
			}
		}
#endif
		reg.CloseKey();
	}

	if (reg.CreateKey(HOSTINFO_STR))
	{
		int		cnt = 0, priority;
		Host	*host;
		time_t	default_time = Time() - KeepHostTime / 2;	// 90 日
		BOOL	rewriteFlg = FALSE;

		priorityHosts.Enable(THosts::NAME, TRUE);
		fileHosts.Enable(THosts::NAME_ADDR, TRUE);
		for (cnt = 0; reg.EnumValue(cnt, buf, sizeof(buf)) || reg.EnumKey(cnt, buf, sizeof(buf)); cnt++)
		{
			if (reg.OpenKey(buf) != TRUE)
				break;
			if (reg.GetInt(PRIORITY_STR, &priority) != TRUE || priority == DEFAULT_PRIORITY)
				rewriteFlg = TRUE;
			else {
				host = new Host;
				host->priority = priority;
				reg.GetStr(USERNAME_STR, host->hostSub.userName, MAX_NAMEBUF);
				reg.GetStr(HOSTNAME_STR, host->hostSub.hostName, MAX_NAMEBUF);
				reg.GetLong(IPADDR_STR, (long *)&host->hostSub.addr);
				reg.GetInt(PORTNO_STR, &host->hostSub.portNo);
				reg.GetStr(NICKNAME_STR, host->nickName, MAX_NAMEBUF);
				reg.GetStr(GROUPNAME_STR, host->groupName, MAX_NAMEBUF);
				if (reg.GetLong(UPDATETIME_STR, &host->updateTime) != TRUE)
					host->updateTime = default_time;
				priorityHosts.AddHost(host);
			}
			reg.CloseKey();
		}
		reg.CloseKey();
		if (rewriteFlg)
			WriteRegistry(CFG_HOSTINFO|CFG_DELHOST);
	}

// private/public key
	if (reg.CreateKey(CRYPT_STR))
	{
		BYTE	key[MAX_BUF];		// 実際には 592byte
		privEncryptSeedLen = privBlobLen = sizeof(key);

		if (reg.GetByte(PRIVBLOB_STR, key, &privBlobLen))
		{
			privBlob = new BYTE [privBlobLen];
			memcpy(privBlob, key, privBlobLen);
		}
		if (reg.GetByte(PRIVSEED_STR, key, &privEncryptSeedLen))
		{
			privEncryptSeed = new BYTE [privEncryptSeedLen];
			memcpy(privEncryptSeed, key, privEncryptSeedLen);
		}
		reg.GetInt(PRIVTYPE_STR, &privEncryptType);
		reg.CloseKey();
	}

	if (reg.OpenKey(DEBUG_STR))	// 旧バージョンレジストリ削除
	{
		reg.CloseKey();
		reg.DeleteChildTree();
	}
	if (reg.GetStr(NULL, buf, sizeof(buf)) != TRUE || strcmp(buf, GetVerionStr()))
	{
		reg.CloseKey();		// close top appreg
		WriteRegistry();
	}

	return	TRUE;
}

BOOL Cfg::ReadFontRegistry(TRegistry *reg, char *key, LOGFONT *font)
{
	if (reg->CreateKey(key))
	{
		int		tmp = 0;

		reg->GetInt(HEIGHT_STR, &tmp);			font->lfHeight = tmp;
		reg->GetInt(WIDTH_STR, &tmp);			font->lfWidth = tmp;
		reg->GetInt(ESCAPEMENT_STR, &tmp);		font->lfEscapement = tmp;
		reg->GetInt(ORIENTATION_STR, &tmp);		font->lfOrientation = tmp;
		reg->GetInt(WEIGHT_STR, &tmp);			font->lfWeight = tmp;
		reg->GetInt(ITALIC_STR, &tmp);			font->lfItalic = (BYTE)tmp;
		reg->GetInt(UNDERLINE_STR, &tmp);		font->lfUnderline = (BYTE)tmp;
		reg->GetInt(STRIKEOUT_STR, &tmp);		font->lfStrikeOut = (BYTE)tmp;
		reg->GetInt(CHARSET_STR, &tmp);			font->lfCharSet = (BYTE)tmp;
		reg->GetInt(OUTPRECISION_STR, &tmp);	font->lfOutPrecision = (BYTE)tmp;
		reg->GetInt(CLIPPRECISION_STR, &tmp);	font->lfClipPrecision = (BYTE)tmp;
		reg->GetInt(QUALITY_STR, &tmp);			font->lfQuality = (BYTE)tmp;
		reg->GetInt(PITCHANDFAMILY_STR, &tmp);	font->lfPitchAndFamily = (BYTE)tmp;
		reg->GetStr(FACENAME_STR, font->lfFaceName, sizeof(font->lfFaceName));
		reg->CloseKey();
	}
	return	TRUE;
}


BOOL Cfg::WriteRegistry(int ctl_flg)
{
	char	buf[MAX_LISTBUF], val[MAX_LISTBUF];

	GetRegName(buf, nicAddr, portNo);
	TRegistry	reg(HS_TOOLS, buf);

	if (ctl_flg & CFG_GENERAL)
	{
		reg.SetStr(NULL, GetVerionStr());
		reg.SetInt(NOBEEP_STR, NoBeep);
		reg.SetInt(LISTGET_STR, ListGet);
		reg.SetInt(LISTGETMSEC_STR, (int)ListGetMSec);
		reg.SetInt(RETRYMSEC_STR, (int)RetryMSec);
		reg.SetInt(RETRYMAX_STR, (int)RetryMax);
		reg.SetInt(IsWinNT() ? RECVMAXNT_STR : RECVMAX95_STR, (int)RecvMax);
		reg.SetInt(NOERASE_STR, NoErase);
		reg.SetInt(DEBUG_STR, Debug);
		reg.SetInt(NOPOPUPCHECK_STR, NoPopupCheck);
		reg.SetInt(OPENCHECK_STR, OpenCheck);
		reg.SetInt(ALLOWSENDLIST_STR, AllowSendList);
		reg.SetInt(FILETRANSOPT_STR, fileTransOpt);
		reg.SetInt(RESOLVEOPT_STR, ResolveOpt);
		reg.SetInt(ENCRYPTNUM_STR, EncryptNum);
		reg.SetInt(VIEWMAX_STR, ViewMax);
		reg.SetInt(TRANSMAX_STR, TransMax);
		reg.SetInt(TCPBUFMAX_STR, TcpbufMax);
	}

	if (ctl_flg & CFG_ABSENCE)
	{
		reg.SetInt(ABSENCESAVE_STR, AbsenceSave);
		reg.SetInt(ABSENCECHECK_STR, AbsenceCheck);
		reg.SetInt(ABSENCEMAX_STR, AbsenceMax);
		if (reg.CreateKey(ABSENCESTR_STR))
		{
			for (int cnt=0; cnt < AbsenceMax; cnt++)
			{
				char	key[MAX_PATH];
				wsprintf(key, "%s%d", ABSENCESTR_STR, cnt);
				reg.SetStr(key, AbsenceStr[cnt]);
				wsprintf(key, "%s%d", ABSENCEHEAD_STR, cnt);
				reg.SetStr(key, AbsenceHead[cnt]);
			}
		}
		reg.CloseKey();
	}

	if (ctl_flg & CFG_GENERAL)
	{
		reg.SetStr(PASSWORD_STR, PasswordStr);
		reg.SetInt(PASSWDLOGCHECK_STR, PasswdLogCheck);
		reg.SetInt(DELAYTIME_STR, DelayTime);
		reg.SetInt(QUOTECHECK_STR, QuoteCheck);
		reg.SetInt(SECRETCHECK_STR, SecretCheck);
		reg.SetInt(IPADDRCHECK_STR, IPAddrCheck);
		reg.SetInt(ONECLICKPOPUP_STR, OneClickPopup);
		reg.SetInt(ABNORMALBTN_STR, AbnormalButton);
		reg.SetInt(DIALUPCHECK_STR, DialUpCheck);
		reg.SetInt(NICKNAMECHECK_STR, NickNameCheck);
		reg.SetInt(ABSENCENONPOPUP_STR, AbsenceNonPopup);
		reg.SetStr(NICKNAMESTR_STR, NickNameStr);
		reg.SetStr(GROUPNAMESTR_STR, GroupNameStr);
		reg.SetStr(LANG_STR, Lang);
		reg.SetLong(SORT_STR, Sort);
		reg.SetInt(UPDATETIME_STR, UpdateTime);
		reg.SetInt(KEEPHOSTTIME_STR, KeepHostTime);
		reg.SetInt(MSGMINIMIZE_STR, MsgMinimize);
		reg.SetInt(EXTENDENTRY_STR, ExtendEntry);
		reg.SetInt(CONTROLIME_STR, ControlIME);
		reg.SetInt(GLIDLINE_STR, GlidLineCheck);
		reg.SetInt(COLUMNITEMS_STR, ColumnItems);
		reg.SetStr(QUOTESTR_STR, QuoteStr);
		if (reg.CreateKey(HOTKEY_STR))
		{
			reg.SetInt(HOTKEYCHECK_STR, HotKeyCheck);
			reg.SetInt(HOTKEYMODIFY_STR, HotKeyModify);
			reg.SetInt(HOTKEYSEND_STR, HotKeySend);
			reg.SetInt(HOTKEYRECV_STR, HotKeyRecv);
			reg.SetInt(HOTKEYMISC_STR, HotKeyMisc);
			reg.CloseKey();
		}
		reg.SetInt(LOGCHECK_STR, LogCheck);
		reg.SetStr(LOGFILE_STR, LogFile);
		reg.SetStr(SOUNDFILE_STR, SoundFile);
		reg.SetStr(ICON_STR, IconFile);
		reg.SetStr(REVICON_STR, RevIconFile);
		reg.SetStr(LASTOPEN_STR, lastOpenDir);
		reg.SetStr(LASTSAVE_STR, lastSaveDir);
	}

	if ((ctl_flg & CFG_WINSIZE) && reg.CreateKey(WINSIZE_STR))
	{
		reg.SetInt(SENDNICKNAME_STR, SendWidth[SW_NICKNAME]);
		reg.SetInt(SENDUSERNAME_STR, SendWidth[SW_USER]);
		reg.SetInt(SENDABSENCE_STR, SendWidth[SW_ABSENCE]);
		reg.SetInt(SENDPRIORITY_STR, SendWidth[SW_PRIORITY]);
		reg.SetInt(SENDGROUPNAME_STR, SendWidth[SW_GROUP]);
		reg.SetInt(SENDHOSTNAME_STR, SendWidth[SW_HOST]);
		reg.SetInt(SENDIPADDR_STR, SendWidth[SW_IPADDR]);

		if (reg.CreateKey(SENDORDER_STR))
		{
			for (int cnt=0; cnt < MAX_SENDWIDTH; cnt++)
			{
				wsprintf(buf, "%d", cnt);
				reg.SetInt(buf, SendOrder[cnt]);
			}
			reg.CloseKey();
		}

		reg.SetInt(SENDXDIFF_STR, SendXdiff);
		reg.SetInt(SENDYDIFF_STR, SendYdiff);
		reg.SetInt(SENDMIDYDIFF_STR, SendMidYdiff);
		reg.SetInt(SENDSAVEPOS_STR, SendSavePos);
		reg.SetInt(SENDXPOS_STR, SendXpos);
		reg.SetInt(SENDYPOS_STR, SendYpos);

		reg.SetInt(RECVXDIFF_STR, RecvXdiff);
		reg.SetInt(RECVYDIFF_STR, RecvYdiff);
		reg.SetInt(RECVSAVEPOS_STR, RecvSavePos);
		reg.SetInt(RECVXPOS_STR, RecvXpos);
		reg.SetInt(RECVYPOS_STR, RecvYpos);

		reg.CloseKey();
	}

	if ((ctl_flg & CFG_FONT) && reg.CreateKey(FONT_STR))
	{
		WriteFontRegistry(&reg, SENDEDITFONT_STR, &SendEditFont);
		WriteFontRegistry(&reg, SENDLISTFONT_STR, &SendListFont);
		WriteFontRegistry(&reg, RECVHEADFONT_STR, &RecvHeadFont);
		WriteFontRegistry(&reg, RECVEDITFONT_STR, &RecvEditFont);
		reg.CloseKey();
	}

	if ((ctl_flg & CFG_BROADCAST) && reg.CreateKey(BROADCAST_STR))
	{
		int		cnt = 0;
		for (TBroadcastObj *obj=broadcastList.Top(); obj; obj=broadcastList.Next(obj))
		{
			wsprintf(buf, "%d", cnt++);
			reg.SetStr(buf, obj->Host());
		}
		while (1)
		{
			wsprintf(buf, "%d", cnt++);
			if (reg.GetStr(buf, val, sizeof(val)) != TRUE)
				break;
			if (reg.DeleteValue(buf) != TRUE)
				break;
		}
		reg.CloseKey();
	}

	if (ctl_flg & CFG_CLICKURL)
	{
		reg.SetInt(DEFAULTURL_STR, DefaultUrl);
		reg.SetInt(SHELLEXEC_STR, ShellExec);
		if (reg.CreateKey(CLICKABLEURL_STR))
		{

			for (UrlObj *obj = (UrlObj *)urlList.TopObj(); obj != NULL; obj = (UrlObj *)urlList.NextObj(obj))
				reg.SetStr(obj->protocol, obj->program);
			reg.CloseKey();
		}
	}

	if ((ctl_flg & CFG_PRIORITY) && reg.CreateKey(PRIORITY_STR))
	{
		reg.SetInt(PRIORITYMAX_STR, PriorityMax);
		reg.SetInt(PRIORITYREJECT_STR, PriorityReject);
#if 0
		for (int cnt=0; cnt < PriorityMax; cnt++)
		{
			wsprintf(buf, "%d", cnt);

			if (reg.CreateKey(buf))
			{
				reg.SetStr(PRIORITYSOUND_STR, PrioritySound[cnt] ? PrioritySound[cnt] : "");
				reg.CloseKey();
			}
		}
#endif
		reg.CloseKey();
	}

	if ((ctl_flg & CFG_HOSTINFO) && reg.CreateKey(HOSTINFO_STR))
	{
		int cnt;
		time_t	now_time = Time();

		if (ctl_flg & CFG_DELHOST)
			reg.DeleteChildTree();

		for (cnt=0; cnt < priorityHosts.HostCnt(); cnt++)
		{
			Host	*host = priorityHosts.GetHost(cnt);
			wsprintf(buf, "%s:%s", host->hostSub.userName, host->hostSub.hostName);
			if (host->updateTime + KeepHostTime < now_time || host->priority == DEFAULT_PRIORITY)
			{
				if (ctl_flg & CFG_DELCHLDHOST)
					reg.DeleteChildTree(buf);
			}
			else if (reg.CreateKey(buf))
			{
				reg.SetStr(USERNAME_STR, host->hostSub.userName);
				reg.SetStr(HOSTNAME_STR, host->hostSub.hostName);
				reg.SetLong(IPADDR_STR, host->hostSub.addr);
				reg.SetInt(PORTNO_STR, host->hostSub.portNo);
				reg.SetStr(NICKNAME_STR, host->nickName);
				reg.SetStr(GROUPNAME_STR, host->groupName);
				reg.SetInt(PRIORITY_STR, host->priority);
				reg.SetLong(UPDATETIME_STR, host->updateTime);
				reg.CloseKey();
			}
		}
		reg.CloseKey();
	}

	if ((ctl_flg & CFG_FINDHIST) && reg.CreateKey(FINDSTR_STR))
	{
		reg.SetInt(FINDMAX_STR, FindMax);
		reg.SetInt(FINDALL_STR, FindAll);
		for (int cnt=0; cnt < FindMax; cnt++)
		{
			char	key[MAX_PATH];
			wsprintf(key, "%d", cnt);
			reg.SetStr(key, FindStr[cnt]);
		}
		reg.CloseKey();
	}

// private/public key
	if ((ctl_flg & CFG_CRYPT) && privBlob && reg.CreateKey(CRYPT_STR))
	{
		reg.SetByte(PRIVBLOB_STR, privBlob, privBlobLen);
		if (privEncryptSeed)
			reg.SetByte(PRIVSEED_STR, privEncryptSeed, privEncryptSeedLen);
		reg.SetInt(PRIVTYPE_STR, privEncryptType);
		reg.CloseKey();
	}

	return	TRUE;
}


BOOL Cfg::WriteFontRegistry(TRegistry *reg, char *key, LOGFONT *font)
{
	if (reg->CreateKey(key))
	{
		reg->SetInt(HEIGHT_STR, (int)font->lfHeight);
		reg->SetInt(WIDTH_STR, (int)font->lfWidth);
		reg->SetInt(ESCAPEMENT_STR, (int)font->lfEscapement);
		reg->SetInt(ORIENTATION_STR, (int)font->lfOrientation);
		reg->SetInt(WEIGHT_STR, (int)font->lfWeight);
		reg->SetInt(ITALIC_STR, (int)font->lfItalic);
		reg->SetInt(UNDERLINE_STR, (int)font->lfUnderline);
		reg->SetInt(STRIKEOUT_STR, (int)font->lfStrikeOut);
		reg->SetInt(CHARSET_STR, (int)font->lfCharSet);
		reg->SetInt(OUTPRECISION_STR, (int)font->lfOutPrecision);
		reg->SetInt(CLIPPRECISION_STR, (int)font->lfClipPrecision);
		reg->SetInt(QUALITY_STR, (int)font->lfQuality);
		reg->SetInt(PITCHANDFAMILY_STR, (int)font->lfPitchAndFamily);
		reg->SetStr(FACENAME_STR, font->lfFaceName);
		reg->CloseKey();
	}
	return	TRUE;
}

void Cfg::GetRegName(char *buf, ULONG nic_addr, int port_no)
{
	buf += wsprintf(buf, "%s", IPMSG_MSGSTR);
	if (port_no != IPMSG_DEFAULT_PORT)
		buf += wsprintf(buf, "%d", port_no);
	if (nic_addr)
		buf += wsprintf(buf, "_%s", inet_ntoa(*(in_addr *)&nic_addr));
}

