static char *logmng_id = 
	"@(#)Copyright (C) H.Shirouzu 1996-2002   logmng.cpp	Ver2.00";
/* ========================================================================
	Project  Name			: IP Messenger for Win32
	Module Name				: Log Manager
	Create					: 1996-08-18(Sun)
	Update					: 2002-11-03(Sun)
	Copyright				: H.Shirouzu
	Reference				: 
	======================================================================== */

#include "tlib.h"
#include "resource.h"
#include "ipmsg.h"
#include "msgstr.h"

LogMng::LogMng(Cfg *_cfg)
{
	cfg = _cfg;
}

BOOL LogMng::WriteSendStart()
{
	return	WriteStart();
}

BOOL LogMng::WriteSendHead(LPCSTR head)
{
	char	buf[MAX_LISTBUF];

	wsprintf(buf, " To: %s\r\n", head);
	return	Write(buf);
}

BOOL LogMng::WriteSendMsg(LPCSTR msg, ULONG command, ShareInfo *shareInfo)
{
	return	WriteMsg(msg, command, shareInfo);
}

BOOL LogMng::WriteRecvMsg(MsgBuf *msg, THosts *hosts, ShareInfo *shareInfo)
{
	if (msg->command & IPMSG_NOLOGOPT)
		return	FALSE;

	WriteStart();
	char	buf[MAX_PATH] = " From: ";

	MakeListString(cfg, &msg->hostSub, hosts, buf + strlen(buf));
	strcat(buf, "\r\n");
	Write(buf);

	return	WriteMsg(msg->msgBuf, msg->command, shareInfo);
}

BOOL LogMng::WriteStart(void)
{
	return	Write("=====================================\r\n");
}

BOOL LogMng::WriteMsg(LPCSTR msg, ULONG command, ShareInfo *shareInfo)
{
	char	buf[MAX_BUF * 2] = "  at ";

	strcat(buf, Ctime()); 
	strcat(buf, " ");

	if (command & IPMSG_BROADCASTOPT)
		strcat(buf, BROADCASTLOG_MSGSTR);

	if (command & IPMSG_AUTORETOPT)
		strcat(buf, AUTORETLOG_MSGSTR);

	if (command & IPMSG_MULTICASTOPT)
		strcat(buf, MULTICASTLOG_MSGSTR);

	if (command & IPMSG_ENCRYPTOPT)
		strcat(buf, ENCRYPT_MSGSTR);

	if (command & IPMSG_SECRETOPT)
	{
		if (command & IPMSG_PASSWORDOPT)
			strcat(buf, PASSWDLOG_MSGSTR);
		else
			strcat(buf, SECRETLOG_MSGSTR);
	}

	if (shareInfo && (command & IPMSG_FILEATTACHOPT))
	{
		strcat(buf, "\r\n  " FILEATTACH_MSGSTR " ");
		char	fname[MAX_PATH], *ptr = buf + strlen(buf);

		for (int cnt=0; cnt < shareInfo->fileCnt && ptr-buf < sizeof(buf)-MAX_PATH; cnt++)
		{
			ForcePathToFname(shareInfo->fileInfo[cnt]->Fname(), fname);
			ptr += wsprintf(ptr, "%s%s", fname, cnt+1 == shareInfo->fileCnt ? "" : ", ");
		}
	}
	strcat(buf, "\r\n-------------------------------------\r\n");

	if (Write(buf) && Write(msg) && Write("\r\n\r\n"))
		return	TRUE;
	else
		return	FALSE;
}

inline int bit_cnt(unsigned char c)
{
	for (int bit=0; c; c>>=1)
		if (c&1)
			bit++;
	return	bit;
}

inline char make_key(char *password)
{
	char	key = 0;

	while (*password)
		key += *password++;

	return	key;
}

BOOL LogMng::Write(LPCSTR str)
{
	BOOL	ret = FALSE;
	int		len = strlen(str);

	if (cfg->LogCheck == FALSE || *cfg->LogFile == 0)
		return	TRUE;

	HANDLE		fh;
	DWORD		size;

	if ((fh = ::CreateFile(cfg->LogFile, GENERIC_WRITE, FILE_SHARE_READ|FILE_SHARE_WRITE, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0)) != INVALID_HANDLE_VALUE)
	{
		::SetFilePointer(fh, 0, 0, FILE_END);
		ret = ::WriteFile(fh, str, len, &size, NULL);
		::CloseHandle(fh);
	}

	return	ret;
}

void LogMng::StrictLogFile(char *logFile)
{
	if (strstr(logFile, "\\\\") == 0 || strchr(logFile, ':') == 0)
	{
		char	path[MAX_BUF], *tmp=NULL;
		if (::GetFullPathName(logFile, sizeof(path), path, &tmp) && tmp)
			strncpyz(logFile, path, MAX_PATH);
	}
}

