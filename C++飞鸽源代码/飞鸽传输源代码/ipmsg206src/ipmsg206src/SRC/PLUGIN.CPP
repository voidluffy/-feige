static char *plugin_id = 
	"@(#)Copyright (C) H.Shirouzu 1996-2001   plugin.cpp	Ver1.47";
/* ========================================================================
	Project  Name			: IP Messenger for Win32
	Module Name				: Plug-in
	Create					: 1997-09-29(Mon)
	Update					: 2001-12-06(Thu)
	Copyright				: H.Shirouzu
	Reference				: 
	======================================================================== */

#include "tlib.h"
#include "resource.h"
#include "ipmsg.h"
#include "msgstr.h"

class Plugin : public TListObj {
public:
	Plugin(char *dllname);
	HINSTANCE EntryDll(char *dllName);
	BOOL ExitDll(HINSTANCE dll);
};

class PluginMng {
protected:
	TList	plugin;

public:
	PluginMng(void);
	~PluginMng();
};

Plugin::Plugin(void)
{
}

Plugin::~Plugin(void)
{
	while (plug_cnt > 0)
		ExitDll(plug_dll[plug_cnt -1]);

	if (plug_dll)
		free(plug_dll);
}

HINSTANCE Plugin::EntryDll(char *dllName)
{
	HINSTANCE	dll;
	BOOL		(*pPluginInit)(void);

	if ((dll = ::LoadLibrary(dllName)) == NULL)
		return	NULL;

	if ((pPluginInit = (BOOL (*)(void))::GetProcAddress(dll, "PluginInitialize")) != NULL && pPluginInit())
	{
		if ((plug_dll = (HINSTANCE *)realloc(plug_dll, sizeof(HINSTANCE) * (plug_cnt + 1))) != NULL)
		{
			plug_dll[plug_cnt++] = dll;
			return	dll;
		}
	}

	::FreeLibrary(dll);
	return	NULL;
}

BOOL Plugin::ExitDll(HINSTANCE dll)
{
	BOOL	(*pPlugTerm)(void);

	for (int cnt=0; cnt < plug_cnt; cnt++)
	{
		if (plug_dll[cnt] != dll)
			continue;
		if ((pPlugTerm = (BOOL (*)(void))::GetProcAddress(plug_dll[cnt], "PluginTermiante")) != NULL)
			pPlugTerm();
		::FreeLibrary(plug_dll[cnt]);
		memmove(plug_dll + cnt, plug_dll + cnt +1, (--plug_cnt - cnt) * sizeof(HINSTANCE));
		return	TRUE;
	}
	return	FALSE;
}

