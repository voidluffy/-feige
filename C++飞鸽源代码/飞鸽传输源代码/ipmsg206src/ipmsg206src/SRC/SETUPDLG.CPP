static char *setupdlg_id = 
	"@(#)Copyright (C) H.Shirouzu 1996-2002   setupdlg.cpp	Ver2.00";
/* ========================================================================
	Project  Name			: IP Messenger for Win32
	Module Name				: Setup Dialog
	Create					: 1996-06-01(Sat)
	Update					: 2002-11-03(Sun)
	Copyright				: H.Shirouzu
	Reference				: 
	======================================================================== */

#include <stdio.h>
#include "tlib.h"
#include "resource.h"
#include "ipmsg.h"
#include "msgstr.h"

TSetupDlg::TSetupDlg(Cfg *_cfg, THosts *_hosts, TWin *_parent) : TDlg(SETUP_DIALOG, _parent)
{
	cfg			= _cfg;
	hosts	= _hosts;
}

TSetupDlg::~TSetupDlg()
{
}

BOOL TSetupDlg::EvCreate(LPARAM lParam)
{
	SetDlgIcon(hWnd);

	SetData();

	if (rect.left == CW_USEDEFAULT)
	{
		GetWindowRect(&rect);
		int	cx = ::GetSystemMetrics(SM_CXFULLSCREEN), cy = ::GetSystemMetrics(SM_CYFULLSCREEN);
		int	xsize = rect.right - rect.left, ysize = rect.bottom - rect.top;
		int	x = (cx - xsize)/2, y = (cy - ysize)/2;

		if (x + xsize > cx)
			x = cx - xsize;
		if (y + ysize > cy)
			y = cy - ysize;

		MoveWindow(x < 0 ? 0 : x, y < 0 ? 0 : y, xsize, ysize, FALSE);
	}
	else
		MoveWindow(rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top, FALSE);

	SetForegroundWindow();
	return	TRUE;
}

BOOL TSetupDlg::EvCommand(WORD wNotifyCode, WORD wID, LPARAM hWndCtl)
{
	switch (wID)
	{
	case IDOK: case SET_BUTTON:
		GetData();	// SET_BUTTON ‚Í‘Scfg-write ... for debug
		cfg->WriteRegistry(wID == IDOK ? (CFG_GENERAL|CFG_BROADCAST) : CFG_ALL);
		if (wID == IDOK)
			EndDialog(TRUE);
		return	TRUE;

	case ADD_BUTTON:
		{
			char	buf[MAX_PATH], buf2[MAX_PATH];
			if (GetDlgItemText(BROADCAST_EDIT, buf, sizeof(buf)) <= 0)
				return	TRUE;
			if (ResolveAddr(buf) == 0)
				return	MessageBox(CANTRESOLVE_MSGSTR), TRUE;
			for (int cnt=0; SendDlgItemMessage(BROADCAST_LIST, LB_GETTEXT, cnt, (LPARAM)buf2) != LB_ERR; cnt++)
				if (stricmp(buf, buf2) == 0)
					return	TRUE;
			SendDlgItemMessage(BROADCAST_LIST, LB_ADDSTRING, 0, (LPARAM)buf);
			SetDlgItemText(BROADCAST_EDIT, "");
		}
		return	TRUE;

	case DEL_BUTTON:
		{
			char	buf[MAX_PATH];
			int		index;

			while ((int)SendDlgItemMessage(BROADCAST_LIST, LB_GETSELCOUNT, 0, 0) > 0)
			{
				if (SendDlgItemMessage(BROADCAST_LIST, LB_GETSELITEMS, 1, (LPARAM)&index) != 1)
					break;
				SendDlgItemMessage(BROADCAST_LIST, LB_GETTEXT, (WPARAM)index, (LPARAM)buf);
				SetDlgItemText(BROADCAST_EDIT, buf);
				if (SendDlgItemMessage(BROADCAST_LIST, LB_DELETESTRING, (WPARAM)index, (LPARAM)buf) == LB_ERR)
					break;
			}
		}
		return	TRUE;

	case LOG_BUTTON:
		TLogDlg(cfg, this).Exec();
		return	TRUE;

	case PASSWORD_BUTTON:
		TPasswdChangeDlg(cfg, this).Exec();
		return	TRUE;

	case URL_BUTTON:
		TUrlDlg(cfg, this).Exec();
		return	TRUE;

	case IDCANCEL: case IDNO:
		EndDialog(FALSE);
		return	TRUE;
	}

	return	FALSE;
}

void TSetupDlg::SetData(void)
{
	SetDlgItemText(GROUP_COMBO, cfg->GroupNameStr);
	SetDlgItemText(NICKNAME_EDIT, cfg->NickNameStr);

	SendDlgItemMessage(OPEN_CHECK, BM_SETCHECK, cfg->OpenCheck, 0);
	SendDlgItemMessage(NOPOPUP_CHECK, BM_SETCHECK, cfg->NoPopupCheck, 0);
	SendDlgItemMessage(NOBEEP_CHECK, BM_SETCHECK, cfg->NoBeep, 0);
	SendDlgItemMessage(QUOTE_CHECK, BM_SETCHECK, cfg->QuoteCheck, 0);
	SendDlgItemMessage(SECRET_CHECK, BM_SETCHECK, cfg->SecretCheck, 0);

	for (TBroadcastObj *obj=cfg->broadcastList.Top(); obj; obj=cfg->broadcastList.Next(obj))
		SendDlgItemMessage(BROADCAST_LIST, LB_ADDSTRING, 0, (LPARAM)obj->Host());

	for (int cnt=0; cnt < hosts->HostCnt(); cnt++)
	{
		Host	*host = hosts->GetHost(cnt);

		if (*host->groupName && SendDlgItemMessage(GROUP_COMBO, CB_FINDSTRING, (WPARAM)-1, (LPARAM)host->groupName) == CB_ERR)
			SendDlgItemMessage(GROUP_COMBO, CB_ADDSTRING, (WPARAM)0, (LPARAM)host->groupName);
	}
	SendDlgItemMessage(DIALUP_CHECK, BM_SETCHECK, cfg->DialUpCheck, 0);
}

void TSetupDlg::GetData(void)
{
	char	buf[MAX_PATH];

	GetDlgItemText(NICKNAME_EDIT, cfg->NickNameStr, sizeof(cfg->NickNameStr));
	GetDlgItemText(GROUP_COMBO, cfg->GroupNameStr, sizeof(cfg->GroupNameStr));

	cfg->OpenCheck = (int)SendDlgItemMessage(OPEN_CHECK, BM_GETCHECK, 0, 0);
	cfg->NoPopupCheck = (int)SendDlgItemMessage(NOPOPUP_CHECK, BM_GETCHECK, 0, 0);
	cfg->NoBeep = (int)SendDlgItemMessage(NOBEEP_CHECK, BM_GETCHECK, 0, 0);
	cfg->QuoteCheck = (int)SendDlgItemMessage(QUOTE_CHECK, BM_GETCHECK, 0, 0);
	cfg->SecretCheck = (int)SendDlgItemMessage(SECRET_CHECK, BM_GETCHECK, 0, 0);

	cfg->broadcastList.Reset();

	for (int cnt=0; SendDlgItemMessage(BROADCAST_LIST, LB_GETTEXT, cnt, (LPARAM)buf) != LB_ERR; cnt++)
		cfg->broadcastList.SetHostRaw(buf, ResolveAddr(buf));

	cfg->DialUpCheck = (int)SendDlgItemMessage(DIALUP_CHECK, BM_GETCHECK, 0, 0);
}

