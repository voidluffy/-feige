static char *msgmng_id = 
	"@(#)Copyright (C) H.Shirouzu 1996-2002   msgmng.cpp	Ver2.00";
/* ========================================================================
	Project  Name			: IP Messenger for Win32
	Module Name				: Message Manager
	Create					: 1996-06-01(Sat)
	Update					: 2002-11-03(Sun)
	Copyright				: H.Shirouzu
	Reference				: 
	======================================================================== */

#include "tlib.h"
#include <stdio.h>
#include "resource.h"
#include "ipmsg.h"
#include "msgstr.h"

MsgMng::MsgMng(ULONG nicAddr, int portNo, Cfg *_cfg)
{
	status = FALSE;
	packetNo = Time();
	udp_sd = tcp_sd = INVALID_SOCKET;
	hAsyncWnd = 0;
	local.addr = nicAddr;
	local.portNo = htons(portNo);
	cfg = _cfg;

	if (WSockInit(cfg ? TRUE : FALSE) == FALSE)
		return;

	DWORD	size = sizeof(local.hostName);
	if (::GetComputerName(local.hostName, &size) == FALSE)
	{
		GetLastErrorMsg("GetComputerName()");
		return;
	}

	if (nicAddr == INADDR_ANY)
	{
		char	host[MAX_BUF];
		if (::gethostname(host, sizeof(host)) == -1)
			strcpy(host, local.hostName);

		hostent	*ent = ::gethostbyname(host);
		if (ent)
			local.addr = *(ULONG *)ent->h_addr_list[0];
	}

	size = sizeof(local.userName);
	if (::GetUserName(local.userName, &size) != TRUE)
		strncpyz(local.userName, NO_NAME, sizeof(local.userName));

	status = TRUE;
}

MsgMng::~MsgMng()
{
	WSockTerm();
}

BOOL MsgMng::WSockInit(BOOL recv_flg)
{
	WSADATA		wsaData;
	if (::WSAStartup(0x0101, &wsaData) != 0)
		return	GetSockErrorMsg("WSAStart()"), FALSE;

	if ((udp_sd = ::socket(AF_INET, SOCK_DGRAM, 0)) == INVALID_SOCKET)
		return	GetSockErrorMsg("Please setup TCP/IP(controlpanel->network)\r\n"), FALSE;

	if (recv_flg != TRUE)
		return	TRUE;

	if ((tcp_sd = ::socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET)
		return	GetSockErrorMsg("Please setup2 TCP/IP(controlpanel->network)\r\n"), FALSE;

	struct sockaddr_in	addr;
	memset(&addr, 0, sizeof(addr));
	addr.sin_family			= AF_INET;
	addr.sin_addr.s_addr	= local.addr;
	addr.sin_port			= local.portNo;

	if (::bind(udp_sd, (LPSOCKADDR)&addr, sizeof(addr)) != 0)
		return	GetSockErrorMsg("bind()"), FALSE;

	if (::bind(tcp_sd, (LPSOCKADDR)&addr, sizeof(addr)) != 0)
	{
		::closesocket(tcp_sd);
		tcp_sd = INVALID_SOCKET;
		GetSockErrorMsg("bind(tcp) error. Can't support file attach");
	}

	BOOL	flg = TRUE;	// Non Block
	if (::ioctlsocket(udp_sd, FIONBIO, (unsigned long *)&flg) != 0)
		return	GetSockErrorMsg("ioctlsocket(nonblock)"), FALSE;

	if (IsAvailableTCP() && ::ioctlsocket(tcp_sd, FIONBIO, (unsigned long *)&flg) != 0)
		return	GetSockErrorMsg("ioctlsocket tcp(nonblock)"), FALSE;

	flg = TRUE;			// allow broadcast
	if (::setsockopt(udp_sd, SOL_SOCKET, SO_BROADCAST, (char *)&flg, sizeof(flg)) != 0)
		return	GetSockErrorMsg("setsockopt(broadcast)"), FALSE;

	int	buf_size = MAX_SOCKBUF, buf_minsize = MAX_SOCKBUF / 2;		// UDP ÉoÉbÉtÉ@ê›íË
	if (::setsockopt(udp_sd, SOL_SOCKET, SO_SNDBUF, (char *)&buf_size, sizeof(int)) != 0
	&&	::setsockopt(udp_sd, SOL_SOCKET, SO_SNDBUF, (char *)&buf_minsize, sizeof(int)) != 0)
		GetSockErrorMsg("setsockopt(sendbuf)");

	buf_size = MAX_SOCKBUF, buf_minsize = MAX_SOCKBUF / 2;
	if (::setsockopt(udp_sd, SOL_SOCKET, SO_RCVBUF, (char *)&buf_size, sizeof(int)) != 0
	&&	::setsockopt(udp_sd, SOL_SOCKET, SO_RCVBUF, (char *)&buf_minsize, sizeof(int)) != 0)
		GetSockErrorMsg("setsockopt(recvbuf)");

	flg = TRUE;	// REUSE ADDR
	if (IsAvailableTCP() && ::setsockopt(tcp_sd, SOL_SOCKET, SO_REUSEADDR, (char *)&flg, sizeof(flg)) != 0)
		GetSockErrorMsg("setsockopt tcp(reuseaddr)");

	if (IsAvailableTCP() && ::listen(tcp_sd, 5) != 0)
		return	FALSE;

	return	TRUE;
}

void MsgMng::WSockTerm(void)
{
	CloseSocket();
	if (IsNewShell())	// Ç»Ç∫Ç© NT3.51 Ç≈ÇÕîöéÄ...
		WSACleanup();
}

void MsgMng::CloseSocket(void)
{
	if (udp_sd != INVALID_SOCKET)
	{
		::closesocket(udp_sd);
		udp_sd = INVALID_SOCKET;
	}
	if (tcp_sd != INVALID_SOCKET)
	{
		::closesocket(tcp_sd);
		tcp_sd = INVALID_SOCKET;
	}
}

BOOL MsgMng::WSockReset(void)
{
	WSockTerm();
	return	WSockInit(TRUE);
}

BOOL MsgMng::Send(HostSub *hostSub, ULONG command, int val)
{
	char	buf[MAX_NAMEBUF];

	wsprintf(buf, "%d", val);
	return	Send(hostSub->addr, hostSub->portNo, command, buf);
}

BOOL MsgMng::Send(HostSub *hostSub, ULONG command, const char *message, const char *exMsg)
{
	return	Send(hostSub->addr, hostSub->portNo, command, message, exMsg);
}

BOOL MsgMng::Send(ULONG host, int port_no, ULONG command, const char *message, const char *exMsg)
{
	char	buf[MAX_UDPBUF];
	int		trans_len;

	MakeMsg(buf, command, message, exMsg, &trans_len);
	return	UdpSend(host, port_no, buf, trans_len);
}

BOOL MsgMng::AsyncSelectRegist(HWND hWnd)
{
	if (hAsyncWnd == 0)
		hAsyncWnd = hWnd;

	if (::WSAAsyncSelect(udp_sd, hWnd, WM_UDPEVENT, FD_READ) == SOCKET_ERROR)
		return	FALSE;

	if (::WSAAsyncSelect(tcp_sd, hWnd, WM_TCPEVENT, FD_ACCEPT|FD_CLOSE) == SOCKET_ERROR)
		return	FALSE;

	return	TRUE;
}

BOOL MsgMng::Recv(MsgBuf *msg)
{
	RecvBuf		buf;

	if (UdpRecv(&buf) != TRUE || buf.size == 0)
		return	FALSE;

	return	ResolveMsg(&buf, msg);
}

ULONG MsgMng::MakeMsg(char *buf, int _packetNo, ULONG command, const char *msg, const char *exMsg, int *packet_len)
{
	int		len, ex_len = exMsg ? strlen(exMsg) + 1 : 0, max_len = MAX_UDPBUF;

	if (packet_len == NULL)
		packet_len = &len;
	*packet_len = wsprintf(buf, "%d:%ld:%s:%s:%ld:", IPMSG_VERSION, _packetNo, local.userName, local.hostName, command);

	if (ex_len + *packet_len + 1 >= MAX_UDPBUF)
		ex_len = 0;
	max_len -= ex_len;

	if (msg != NULL)	// sprintfÇÕégÇÌÇ»Ç¢
		*packet_len += LocalNewLineToUnix(msg, buf + *packet_len, max_len - *packet_len);

	(*packet_len)++;

	if (ex_len)
	{
		memcpy(buf + *packet_len, exMsg, ex_len);
		*packet_len += ex_len;
	}

	return	_packetNo;
}

int MsgMng::LocalNewLineToUnix(const char *src, char *dest, int maxlen)
{
	int		len = 0;

	maxlen--;	// \0 äiî[ï™

	while (*src != '\0' && len < maxlen)
		if ((dest[len] = *src++) != '\r')
			len++;
	dest[len] = 0;

	return	len;
}

int MsgMng::UnixNewLineToLocal(const char *src, char *dest, int maxlen)
{
	int		len = 0;
	char	*tmpbuf = NULL;

	if (src == dest)
		tmpbuf = strdup(src), src = tmpbuf;

	maxlen--;	// \0 äiî[ï™

	while (*src != '\0' && len < maxlen)
	{
		if ((dest[len] = *src++) == '\n')
		{
			dest[len++] = '\r';
			if (len < maxlen)
				dest[len] = '\n';
		}
		len++;
	}
	dest[len] = 0;
	if (tmpbuf)
		free(tmpbuf);

	return	len;
}


BOOL MsgMng::ResolveMsg(RecvBuf *buf, MsgBuf *msg)
{
	char	*exStr = NULL, *tok, *p;
	int		len;

	if (buf->size > (len = strlen(buf->msgBuf)) +1)
		exStr = buf->msgBuf + len +1;

	msg->hostSub.addr	= buf->addr.sin_addr.s_addr;
	msg->hostSub.portNo	= buf->addr.sin_port;

	if ((tok = separate_token(buf->msgBuf, ':', &p)) == NULL)
		return	FALSE;
	if ((msg->version = atoi(tok)) != IPMSG_VERSION)
		return	FALSE;

	if ((tok = separate_token(NULL, ':', &p)) == NULL)
		return	FALSE;
	msg->packetNo = atol(tok);

	if ((tok = separate_token(NULL, ':', &p)) == NULL)
		return	FALSE;
	strncpyz(msg->hostSub.userName, tok, sizeof(msg->hostSub.userName));

	if ((tok = separate_token(NULL, ':', &p)) == NULL)
		return	FALSE;
	strncpyz(msg->hostSub.hostName, tok, sizeof(msg->hostSub.hostName));

	if ((tok = separate_token(NULL, ':', &p)) == NULL)
		return	FALSE;
	msg->command = atol(tok);

	int		cnt = 0, ex_len;
	*msg->msgBuf = 0;
	if ((tok = separate_token(NULL, 0, &p)) != NULL) // â¸çsÇUNIXå`éÆÇ©ÇÁDOSå`éÆÇ…ïœä∑
	{
		while (*tok != '\0' && cnt < MAX_UDPBUF -1)
		{
			if ((msg->msgBuf[cnt++] = *tok++) == '\n')
			{
				msg->msgBuf[cnt-1] = '\r';
				if (cnt < MAX_UDPBUF -1)
					msg->msgBuf[cnt++] = '\n';
			}
		}
		msg->msgBuf[cnt] = '\0';
	}
	msg->exOffset = cnt;

	if (exStr && (ex_len = strlen(exStr) + 1) < MAX_UDPBUF -1)
	{
		if (++msg->exOffset + ex_len >= MAX_UDPBUF)
			msg->msgBuf[(msg->exOffset = MAX_UDPBUF - ex_len) -1] = '\0'; // exStr óDêÊ
		memcpy(msg->msgBuf + msg->exOffset, exStr, ex_len);
	}

	return	TRUE;
}


BOOL MsgMng::UdpSend(ULONG host_addr, int port_no, const char *buf)
{
	return	UdpSend(host_addr, port_no, buf, strlen(buf) +1);
}

BOOL MsgMng::UdpSend(ULONG host_addr, int port_no, const char *buf, int len)
{
	struct sockaddr_in	addr;

	memset(&addr, 0, sizeof(addr));
	addr.sin_family			= AF_INET;
	addr.sin_port			= port_no;
	addr.sin_addr.s_addr	= host_addr;

	if (::sendto(udp_sd, buf, len, 0, (LPSOCKADDR)&addr, sizeof(addr)) == SOCKET_ERROR)
	{
		switch (WSAGetLastError()) {
		case WSAENETDOWN:
			break;
		case WSAEHOSTUNREACH:
			static	BOOL	done;
			if (done == FALSE) {
				done = TRUE;
//				MessageBox(0, HOSTUNREACH_MSGSTR, inet_ntoa(*(LPIN_ADDR)&host_addr), MB_OK);
			}
			return	FALSE;
		default:
			return	FALSE;
		}

		if (WSockReset() != TRUE)
			return	FALSE;

		if (hAsyncWnd && AsyncSelectRegist(hAsyncWnd) != TRUE)
			return	FALSE;

		if (::sendto(udp_sd, buf, len, 0, (LPSOCKADDR)&addr, sizeof(addr)) == SOCKET_ERROR)
			return	FALSE;
	}

	return	TRUE;
}


BOOL MsgMng::UdpRecv(RecvBuf *buf)
{
	buf->addrSize = sizeof(buf->addr);

	if ((buf->size = ::recvfrom(udp_sd, buf->msgBuf, sizeof(buf->msgBuf) -1, 0, (LPSOCKADDR)&buf->addr, &buf->addrSize)) == SOCKET_ERROR)
		return	FALSE;
	buf->msgBuf[buf->size] = 0;

	return	TRUE;
}

BOOL MsgMng::Accept(HWND hWnd, ConnectInfo *info)
{
	struct sockaddr_in	addr;
	int		size = sizeof(addr), flg=TRUE;
	if ((info->sd = ::accept(tcp_sd, (LPSOCKADDR)&addr, &size)) == INVALID_SOCKET)
		return	FALSE;
	::setsockopt(info->sd, SOL_SOCKET, TCP_NODELAY, (char *)&flg, sizeof(flg));

	info->addr = addr.sin_addr.s_addr;
	info->port = addr.sin_port;
	info->server = info->complete = TRUE;

	for (int buf_size=cfg->TcpbufMax; buf_size > 0; buf_size /= 2)
		if (::setsockopt(info->sd, SOL_SOCKET, SO_SNDBUF, (char *)&buf_size, sizeof(buf_size)) == 0)
			break;

	if (AsyncSelectConnect(hWnd, info))
	{
		info->startTick = info->lastTick = ::GetTickCount();
		return	TRUE;
	}

	::closesocket(info->sd);
	return	FALSE;
}

BOOL MsgMng::Connect(HWND hWnd, ConnectInfo *info)
{
	info->server = FALSE;
	if ((info->sd = ::socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET)
		return	FALSE;

	BOOL	flg = TRUE;	// Non Block
	if (::ioctlsocket(info->sd, FIONBIO, (unsigned long *)&flg) != 0)
		return	FALSE;
	::setsockopt(info->sd, SOL_SOCKET, TCP_NODELAY, (char *)&flg, sizeof(flg));

	for (int buf_size=cfg->TcpbufMax; buf_size > 0; buf_size /= 2)
		if (::setsockopt(info->sd, SOL_SOCKET, SO_RCVBUF, (char *)&buf_size, sizeof(buf_size)) == 0)
			break;

	if (AsyncSelectConnect(hWnd, info))
	{
		struct sockaddr_in	addr;
		memset(&addr, 0, sizeof(addr));
		addr.sin_family			= AF_INET;
		addr.sin_port			= info->port;
		addr.sin_addr.s_addr	= info->addr;
		if ((info->complete = (::connect(info->sd, (LPSOCKADDR)&addr, sizeof(addr)) == 0)) || WSAGetLastError() == WSAEWOULDBLOCK)
		{
			info->startTick = info->lastTick = ::GetTickCount();
			return	TRUE;
		}
	}
	::closesocket(info->sd);
	return	FALSE;
}

BOOL MsgMng::AsyncSelectConnect(HWND hWnd, ConnectInfo *info)
{
	if (::WSAAsyncSelect(info->sd, hWnd, WM_TCPEVENT, (info->server ? FD_READ : FD_CONNECT)|FD_CLOSE) == SOCKET_ERROR)
		return	FALSE;
	return	TRUE;
}

/*
	îÒìØä˙ånÇÃó}êß
*/
BOOL MsgMng::ConnectDone(HWND hWnd, ConnectInfo *info)
{
	::WSAAsyncSelect(info->sd, hWnd, 0, 0);	// îÒìØä˙ÉÅÉbÉZÅ[ÉWÇÃó}êß
	BOOL	flg = FALSE;
	::ioctlsocket(info->sd, FIONBIO, (unsigned long *)&flg);
	return	TRUE;
}

