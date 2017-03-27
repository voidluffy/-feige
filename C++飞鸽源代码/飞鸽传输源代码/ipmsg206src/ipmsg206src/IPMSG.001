# Microsoft Developer Studio Generated NMAKE File, Format Version 4.10
# ** 編集しないでください **

# TARGTYPE "Win32 (x86) Application" 0x0101

!IF "$(CFG)" == ""
CFG=ipmsg - Win32 Debug
!MESSAGE 構成が指定されていません。ﾃﾞﾌｫﾙﾄの ipmsg - Win32 Debug を設定します。
!ENDIF 

!IF "$(CFG)" != "ipmsg - Win32 English" && "$(CFG)" != "ipmsg - Win32 Japanese"\
 && "$(CFG)" != "ipmsg - Win32 Debug" && "$(CFG)" != "ipmsg - Win32 Installer"\
 && "$(CFG)" != "ipmsg - Win32 InstallerDbg" && "$(CFG)" !=\
 "ipmsg - Win32 InstallerEng"
!MESSAGE 指定された ﾋﾞﾙﾄﾞ ﾓｰﾄﾞ "$(CFG)" は正しくありません。
!MESSAGE ｺﾏﾝﾄﾞ ﾗｲﾝ上でﾏｸﾛ 'CFG' を定義することによって
!MESSAGE NMAKE 実行時にﾋﾞﾙﾄﾞ ﾓｰﾄﾞを指定できます。例えば:
!MESSAGE 
!MESSAGE NMAKE /f "ipmsg.mak" CFG="ipmsg - Win32 Debug"
!MESSAGE 
!MESSAGE 選択可能なﾋﾞﾙﾄﾞ ﾓｰﾄﾞ:
!MESSAGE 
!MESSAGE "ipmsg - Win32 English" ("Win32 (x86) Application" 用)
!MESSAGE "ipmsg - Win32 Japanese" ("Win32 (x86) Application" 用)
!MESSAGE "ipmsg - Win32 Debug" ("Win32 (x86) Application" 用)
!MESSAGE "ipmsg - Win32 Installer" ("Win32 (x86) Application" 用)
!MESSAGE "ipmsg - Win32 InstallerDbg" ("Win32 (x86) Application" 用)
!MESSAGE "ipmsg - Win32 InstallerEng" ("Win32 (x86) Application" 用)
!MESSAGE 
!ERROR 無効な構成が指定されています。
!ENDIF 

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 
################################################################################
# Begin Project
# PROP Target_Last_Scanned "ipmsg - Win32 InstallerEng"
MTL=mktyplib.exe
RSC=rc.exe
CPP=cl.exe

!IF  "$(CFG)" == "ipmsg - Win32 English"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "ipmsg__0"
# PROP BASE Intermediate_Dir "ipmsg__0"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Win32ENG"
# PROP Intermediate_Dir "obj/English"
# PROP Target_Dir ""
OUTDIR=.\Win32ENG
INTDIR=.\obj/English

ALL : "$(OUTDIR)\ipmsg.exe"

CLEAN : 
	-@erase "$(INTDIR)\blowfish.obj"
	-@erase "$(INTDIR)\cfg.obj"
	-@erase "$(INTDIR)\Ipmsg.obj"
	-@erase "$(INTDIR)\ipmsgeng.res"
	-@erase "$(INTDIR)\Logdlg.obj"
	-@erase "$(INTDIR)\Logmng.obj"
	-@erase "$(INTDIR)\Mainwin.obj"
	-@erase "$(INTDIR)\Miscdlg.obj"
	-@erase "$(INTDIR)\Msgmng.obj"
	-@erase "$(INTDIR)\Recvdlg.obj"
	-@erase "$(INTDIR)\Senddlg.obj"
	-@erase "$(INTDIR)\Setupdlg.obj"
	-@erase "$(INTDIR)\share.obj"
	-@erase "$(INTDIR)\Tapp.obj"
	-@erase "$(INTDIR)\Tdlg.obj"
	-@erase "$(INTDIR)\Tlist.obj"
	-@erase "$(INTDIR)\Tregist.obj"
	-@erase "$(INTDIR)\Twin.obj"
	-@erase "$(OUTDIR)\ipmsg.exe"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

"$(INTDIR)" :
    if not exist "$(INTDIR)/$(NULL)" mkdir "$(INTDIR)"

# ADD BASE CPP /nologo /W3 /O1 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /YX /c
# ADD CPP /nologo /W3 /O1 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "ENGLISH" /YX /c
CPP_PROJ=/nologo /ML /W3 /O1 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "ENGLISH"\
 /Fp"$(INTDIR)/ipmsg.pch" /YX /Fo"$(INTDIR)/" /c 
CPP_OBJS=.\obj/English/
CPP_SBRS=.\.
# ADD BASE MTL /nologo /D "NDEBUG" /win32
# ADD MTL /nologo /D "NDEBUG" /win32
MTL_PROJ=/nologo /D "NDEBUG" /win32 
# ADD BASE RSC /l 0x411 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
RSC_PROJ=/l 0x409 /fo"$(INTDIR)/ipmsgeng.res" /d "NDEBUG" 
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/ipmsg.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib winmm.lib Comctl32.lib /nologo /subsystem:windows /machine:I386
# SUBTRACT BASE LINK32 /nodefaultlib
# ADD LINK32 kernel32.lib user32.lib gdi32.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib winmm.lib Comctl32.lib wsock32.lib /nologo /subsystem:windows /machine:I386
# SUBTRACT LINK32 /nodefaultlib
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib comdlg32.lib advapi32.lib\
 shell32.lib ole32.lib oleaut32.lib uuid.lib winmm.lib Comctl32.lib wsock32.lib\
 /nologo /subsystem:windows /incremental:no /pdb:"$(OUTDIR)/ipmsg.pdb"\
 /machine:I386 /out:"$(OUTDIR)/ipmsg.exe" 
LINK32_OBJS= \
	"$(INTDIR)\blowfish.obj" \
	"$(INTDIR)\cfg.obj" \
	"$(INTDIR)\Ipmsg.obj" \
	"$(INTDIR)\ipmsgeng.res" \
	"$(INTDIR)\Logdlg.obj" \
	"$(INTDIR)\Logmng.obj" \
	"$(INTDIR)\Mainwin.obj" \
	"$(INTDIR)\Miscdlg.obj" \
	"$(INTDIR)\Msgmng.obj" \
	"$(INTDIR)\Recvdlg.obj" \
	"$(INTDIR)\Senddlg.obj" \
	"$(INTDIR)\Setupdlg.obj" \
	"$(INTDIR)\share.obj" \
	"$(INTDIR)\Tapp.obj" \
	"$(INTDIR)\Tdlg.obj" \
	"$(INTDIR)\Tlist.obj" \
	"$(INTDIR)\Tregist.obj" \
	"$(INTDIR)\Twin.obj"

"$(OUTDIR)\ipmsg.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "ipmsg - Win32 Japanese"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "ipmsg___"
# PROP BASE Intermediate_Dir "ipmsg___"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Win32JPN"
# PROP Intermediate_Dir "obj/japanese"
# PROP Target_Dir ""
OUTDIR=.\Win32JPN
INTDIR=.\obj/japanese

ALL : "$(OUTDIR)\ipmsg.exe"

CLEAN : 
	-@erase "$(INTDIR)\blowfish.obj"
	-@erase "$(INTDIR)\cfg.obj"
	-@erase "$(INTDIR)\Ipmsg.obj"
	-@erase "$(INTDIR)\ipmsgjpn.res"
	-@erase "$(INTDIR)\Logdlg.obj"
	-@erase "$(INTDIR)\Logmng.obj"
	-@erase "$(INTDIR)\Mainwin.obj"
	-@erase "$(INTDIR)\Miscdlg.obj"
	-@erase "$(INTDIR)\Msgmng.obj"
	-@erase "$(INTDIR)\Recvdlg.obj"
	-@erase "$(INTDIR)\Senddlg.obj"
	-@erase "$(INTDIR)\Setupdlg.obj"
	-@erase "$(INTDIR)\share.obj"
	-@erase "$(INTDIR)\Tapp.obj"
	-@erase "$(INTDIR)\Tdlg.obj"
	-@erase "$(INTDIR)\Tlist.obj"
	-@erase "$(INTDIR)\Tregist.obj"
	-@erase "$(INTDIR)\Twin.obj"
	-@erase "$(OUTDIR)\ipmsg.exe"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

"$(INTDIR)" :
    if not exist "$(INTDIR)/$(NULL)" mkdir "$(INTDIR)"

# ADD BASE CPP /nologo /W3 /O1 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /YX /c
# ADD CPP /nologo /W3 /O1 /Op /Oy /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "JAPANESE" /YX /c
# SUBTRACT CPP /Fr
CPP_PROJ=/nologo /ML /W3 /O1 /Op /Oy /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D\
 "JAPANESE" /Fp"$(INTDIR)/ipmsg.pch" /YX /Fo"$(INTDIR)/" /c 
CPP_OBJS=.\obj/japanese/
CPP_SBRS=.\.
# ADD BASE MTL /nologo /D "NDEBUG" /win32
# ADD MTL /nologo /D "NDEBUG" /win32
MTL_PROJ=/nologo /D "NDEBUG" /win32 
# ADD BASE RSC /l 0x411 /d "NDEBUG"
# ADD RSC /l 0x411 /d "NDEBUG"
RSC_PROJ=/l 0x411 /fo"$(INTDIR)/ipmsgjpn.res" /d "NDEBUG" 
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/ipmsg.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib winmm.lib Comctl32.lib /nologo /subsystem:windows /machine:I386
# SUBTRACT BASE LINK32 /nodefaultlib
# ADD LINK32 kernel32.lib user32.lib gdi32.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib winmm.lib Comctl32.lib imm32.lib wsock32.lib /nologo /subsystem:windows /machine:I386
# SUBTRACT LINK32 /nodefaultlib
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib comdlg32.lib advapi32.lib\
 shell32.lib ole32.lib oleaut32.lib uuid.lib winmm.lib Comctl32.lib imm32.lib\
 wsock32.lib /nologo /subsystem:windows /incremental:no\
 /pdb:"$(OUTDIR)/ipmsg.pdb" /machine:I386 /out:"$(OUTDIR)/ipmsg.exe" 
LINK32_OBJS= \
	"$(INTDIR)\blowfish.obj" \
	"$(INTDIR)\cfg.obj" \
	"$(INTDIR)\Ipmsg.obj" \
	"$(INTDIR)\ipmsgjpn.res" \
	"$(INTDIR)\Logdlg.obj" \
	"$(INTDIR)\Logmng.obj" \
	"$(INTDIR)\Mainwin.obj" \
	"$(INTDIR)\Miscdlg.obj" \
	"$(INTDIR)\Msgmng.obj" \
	"$(INTDIR)\Recvdlg.obj" \
	"$(INTDIR)\Senddlg.obj" \
	"$(INTDIR)\Setupdlg.obj" \
	"$(INTDIR)\share.obj" \
	"$(INTDIR)\Tapp.obj" \
	"$(INTDIR)\Tdlg.obj" \
	"$(INTDIR)\Tlist.obj" \
	"$(INTDIR)\Tregist.obj" \
	"$(INTDIR)\Twin.obj"

"$(OUTDIR)\ipmsg.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "ipmsg - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "ipmsg__0"
# PROP BASE Intermediate_Dir "ipmsg__0"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "debug"
# PROP Intermediate_Dir "obj/debug"
# PROP Target_Dir ""
OUTDIR=.\debug
INTDIR=.\obj/debug

ALL : "$(OUTDIR)\ipmsg.exe"

CLEAN : 
	-@erase "$(INTDIR)\blowfish.obj"
	-@erase "$(INTDIR)\cfg.obj"
	-@erase "$(INTDIR)\Ipmsg.obj"
	-@erase "$(INTDIR)\ipmsgjpn.res"
	-@erase "$(INTDIR)\Logdlg.obj"
	-@erase "$(INTDIR)\Logmng.obj"
	-@erase "$(INTDIR)\Mainwin.obj"
	-@erase "$(INTDIR)\Miscdlg.obj"
	-@erase "$(INTDIR)\Msgmng.obj"
	-@erase "$(INTDIR)\Recvdlg.obj"
	-@erase "$(INTDIR)\Senddlg.obj"
	-@erase "$(INTDIR)\Setupdlg.obj"
	-@erase "$(INTDIR)\share.obj"
	-@erase "$(INTDIR)\Tapp.obj"
	-@erase "$(INTDIR)\Tdlg.obj"
	-@erase "$(INTDIR)\Tlist.obj"
	-@erase "$(INTDIR)\Tregist.obj"
	-@erase "$(INTDIR)\Twin.obj"
	-@erase "$(INTDIR)\vc40.idb"
	-@erase "$(INTDIR)\vc40.pdb"
	-@erase "$(OUTDIR)\ipmsg.exe"
	-@erase "$(OUTDIR)\ipmsg.map"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

"$(INTDIR)" :
    if not exist "$(INTDIR)/$(NULL)" mkdir "$(INTDIR)"

# ADD BASE CPP /nologo /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "JAPANESE" /YX /c
# ADD CPP /nologo /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "JAPANESE" /YX /c
CPP_PROJ=/nologo /MLd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS"\
 /D "JAPANESE" /Fp"$(INTDIR)/ipmsg.pch" /YX /Fo"$(INTDIR)/" /Fd"$(INTDIR)/" /c 
CPP_OBJS=.\obj/debug/
CPP_SBRS=.\.
# ADD BASE MTL /nologo /D "_DEBUG" /win32
# ADD MTL /nologo /D "_DEBUG" /win32
MTL_PROJ=/nologo /D "_DEBUG" /win32 
# ADD BASE RSC /l 0x411 /d "_DEBUG"
# ADD RSC /l 0x411 /d "_DEBUG"
RSC_PROJ=/l 0x411 /fo"$(INTDIR)/ipmsgjpn.res" /d "_DEBUG" 
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/ipmsg.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib winmm.lib Comctl32.lib /nologo /subsystem:windows /map /debug /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib winmm.lib Comctl32.lib imm32.lib wsock32.lib /nologo /subsystem:windows /profile /map:"debug/ipmsg.map" /debug /machine:I386
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib\
 advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib winmm.lib Comctl32.lib\
 imm32.lib wsock32.lib /nologo /subsystem:windows /profile\
 /map:"debug/ipmsg.map" /debug /machine:I386 /out:"$(OUTDIR)/ipmsg.exe" 
LINK32_OBJS= \
	"$(INTDIR)\blowfish.obj" \
	"$(INTDIR)\cfg.obj" \
	"$(INTDIR)\Ipmsg.obj" \
	"$(INTDIR)\ipmsgjpn.res" \
	"$(INTDIR)\Logdlg.obj" \
	"$(INTDIR)\Logmng.obj" \
	"$(INTDIR)\Mainwin.obj" \
	"$(INTDIR)\Miscdlg.obj" \
	"$(INTDIR)\Msgmng.obj" \
	"$(INTDIR)\Recvdlg.obj" \
	"$(INTDIR)\Senddlg.obj" \
	"$(INTDIR)\Setupdlg.obj" \
	"$(INTDIR)\share.obj" \
	"$(INTDIR)\Tapp.obj" \
	"$(INTDIR)\Tdlg.obj" \
	"$(INTDIR)\Tlist.obj" \
	"$(INTDIR)\Tregist.obj" \
	"$(INTDIR)\Twin.obj"

"$(OUTDIR)\ipmsg.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "ipmsg - Win32 Installer"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "ipmsg___"
# PROP BASE Intermediate_Dir "ipmsg___"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Win32JPN"
# PROP Intermediate_Dir "obj/install"
# PROP Target_Dir ""
OUTDIR=.\Win32JPN
INTDIR=.\obj/install

ALL : "$(OUTDIR)\setup.exe"

CLEAN : 
	-@erase "$(INTDIR)\install.obj"
	-@erase "$(INTDIR)\instjpn.res"
	-@erase "$(INTDIR)\Tapp.obj"
	-@erase "$(INTDIR)\Tdlg.obj"
	-@erase "$(INTDIR)\Tregist.obj"
	-@erase "$(INTDIR)\Twin.obj"
	-@erase "$(OUTDIR)\setup.exe"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

"$(INTDIR)" :
    if not exist "$(INTDIR)/$(NULL)" mkdir "$(INTDIR)"

# ADD BASE CPP /nologo /W3 /O1 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "JAPANESE" /YX /c
# ADD CPP /nologo /W3 /O1 /D "WIN32" /D "_WINDOWS" /D "JAPANESE" /YX /c
CPP_PROJ=/nologo /ML /W3 /O1 /D "WIN32" /D "_WINDOWS" /D "JAPANESE"\
 /Fp"$(INTDIR)/ipmsg.pch" /YX /Fo"$(INTDIR)/" /c 
CPP_OBJS=.\obj/install/
CPP_SBRS=.\.
# ADD BASE MTL /nologo /D "NDEBUG" /win32
# ADD MTL /nologo /D "NDEBUG" /win32
MTL_PROJ=/nologo /D "NDEBUG" /win32 
# ADD BASE RSC /l 0x411 /d "NDEBUG"
# ADD RSC /l 0x411 /d "NDEBUG"
RSC_PROJ=/l 0x411 /fo"$(INTDIR)/instjpn.res" /d "NDEBUG" 
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/ipmsg.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib winmm.lib Comctl32.lib imm32.lib /nologo /subsystem:windows /machine:I386
# SUBTRACT BASE LINK32 /nodefaultlib
# ADD LINK32 kernel32.lib user32.lib gdi32.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib Comctl32.lib /nologo /subsystem:windows /machine:I386 /out:"Win32JPN/setup.exe"
# SUBTRACT LINK32 /nodefaultlib
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib comdlg32.lib advapi32.lib\
 shell32.lib ole32.lib oleaut32.lib uuid.lib Comctl32.lib /nologo\
 /subsystem:windows /incremental:no /pdb:"$(OUTDIR)/setup.pdb" /machine:I386\
 /out:"$(OUTDIR)/setup.exe" 
LINK32_OBJS= \
	"$(INTDIR)\install.obj" \
	"$(INTDIR)\instjpn.res" \
	"$(INTDIR)\Tapp.obj" \
	"$(INTDIR)\Tdlg.obj" \
	"$(INTDIR)\Tregist.obj" \
	"$(INTDIR)\Twin.obj"

"$(OUTDIR)\setup.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "ipmsg - Win32 InstallerDbg"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "ipmsg___"
# PROP BASE Intermediate_Dir "ipmsg___"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "debug"
# PROP Intermediate_Dir "obj/installDBG"
# PROP Target_Dir ""
OUTDIR=.\debug
INTDIR=.\obj/installDBG

ALL : "$(OUTDIR)\setup.exe"

CLEAN : 
	-@erase "$(OUTDIR)\setup.exe"
	-@erase "$(OUTDIR)\setup.ilk"
	-@erase "$(OUTDIR)\setup.pdb"
	-@erase ".\obj\debug\install.obj"
	-@erase ".\obj\debug\install.res"
	-@erase ".\obj\debug\Tapp.obj"
	-@erase ".\obj\debug\Tdlg.obj"
	-@erase ".\obj\debug\Tregist.obj"
	-@erase ".\obj\debug\Twin.obj"
	-@erase ".\obj\debug\vc40.idb"
	-@erase ".\obj\debug\vc40.pdb"
	-@erase ".\obj\installDBG\setup.map"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

"$(INTDIR)" :
    if not exist "$(INTDIR)/$(NULL)" mkdir "$(INTDIR)"

# ADD BASE CPP /nologo /W3 /O1 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "JAPANESE" /YX /c
# ADD CPP /nologo /MLd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "JAPANESE" /Fp"obj/debug/ipmsg.pch" /YX /Fo"obj/debug/" /Fd"obj/debug/" /c
CPP_PROJ=/nologo /MLd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS"\
 /D "JAPANESE" /Fp"obj/debug/ipmsg.pch" /YX /Fo"obj/debug/" /Fd"obj/debug/" /c 
CPP_OBJS=.\obj/debug/
CPP_SBRS=.\.
# ADD BASE MTL /nologo /D "NDEBUG" /win32
# ADD MTL /nologo /D "NDEBUG" /win32
MTL_PROJ=/nologo /D "NDEBUG" /win32 
# ADD BASE RSC /l 0x411 /d "NDEBUG"
# ADD RSC /l 0x411 /fo"obj/debug/install.res" /d "NDEBUG"
RSC_PROJ=/l 0x411 /fo"obj/debug/install.res" /d "NDEBUG" 
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/ipmsg.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib Comctl32.lib /nologo /subsystem:windows /machine:I386 /out:"Win32JPN/install.exe"
# SUBTRACT BASE LINK32 /nodefaultlib
# ADD LINK32 kernel32.lib user32.lib gdi32.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib Comctl32.lib /nologo /subsystem:windows /incremental:yes /map /debug /machine:I386 /out:"debug/setup.exe"
# SUBTRACT LINK32 /nodefaultlib
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib comdlg32.lib advapi32.lib\
 shell32.lib ole32.lib oleaut32.lib uuid.lib Comctl32.lib /nologo\
 /subsystem:windows /incremental:yes /pdb:"$(OUTDIR)/setup.pdb"\
 /map:"$(INTDIR)/setup.map" /debug /machine:I386 /out:"$(OUTDIR)/setup.exe" 
LINK32_OBJS= \
	".\obj\debug\install.obj" \
	".\obj\debug\install.res" \
	".\obj\debug\Tapp.obj" \
	".\obj\debug\Tdlg.obj" \
	".\obj\debug\Tregist.obj" \
	".\obj\debug\Twin.obj"

"$(OUTDIR)\setup.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "ipmsg - Win32 InstallerEng"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "ipmsg___"
# PROP BASE Intermediate_Dir "ipmsg___"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Win32Eng"
# PROP Intermediate_Dir "obj/installEng"
# PROP Target_Dir ""
OUTDIR=.\Win32Eng
INTDIR=.\obj/installEng

ALL : "$(OUTDIR)\setup.exe"

CLEAN : 
	-@erase "$(INTDIR)\install.obj"
	-@erase "$(INTDIR)\insteng.res"
	-@erase "$(INTDIR)\Tapp.obj"
	-@erase "$(INTDIR)\Tdlg.obj"
	-@erase "$(INTDIR)\Tregist.obj"
	-@erase "$(INTDIR)\Twin.obj"
	-@erase "$(OUTDIR)\setup.exe"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

"$(INTDIR)" :
    if not exist "$(INTDIR)/$(NULL)" mkdir "$(INTDIR)"

# ADD BASE CPP /nologo /W3 /O1 /D "WIN32" /D "_WINDOWS" /D "JAPANESE" /YX /c
# ADD CPP /nologo /W3 /O1 /D "WIN32" /D "_WINDOWS" /D "ENGLISH" /YX /c
CPP_PROJ=/nologo /ML /W3 /O1 /D "WIN32" /D "_WINDOWS" /D "ENGLISH"\
 /Fp"$(INTDIR)/ipmsg.pch" /YX /Fo"$(INTDIR)/" /c 
CPP_OBJS=.\obj/installEng/
CPP_SBRS=.\.
# ADD BASE MTL /nologo /D "NDEBUG" /win32
# ADD MTL /nologo /D "NDEBUG" /win32
MTL_PROJ=/nologo /D "NDEBUG" /win32 
# ADD BASE RSC /l 0x411 /d "NDEBUG"
# ADD RSC /l 0x411 /d "NDEBUG"
RSC_PROJ=/l 0x411 /fo"$(INTDIR)/insteng.res" /d "NDEBUG" 
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/ipmsg.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib Comctl32.lib /nologo /subsystem:windows /machine:I386 /out:"Win32JPN/setup.exe"
# SUBTRACT BASE LINK32 /nodefaultlib
# ADD LINK32 kernel32.lib user32.lib gdi32.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib Comctl32.lib /nologo /subsystem:windows /machine:I386 /out:"Win32Eng/setup.exe"
# SUBTRACT LINK32 /nodefaultlib
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib comdlg32.lib advapi32.lib\
 shell32.lib ole32.lib oleaut32.lib uuid.lib Comctl32.lib /nologo\
 /subsystem:windows /incremental:no /pdb:"$(OUTDIR)/setup.pdb" /machine:I386\
 /out:"$(OUTDIR)/setup.exe" 
LINK32_OBJS= \
	"$(INTDIR)\install.obj" \
	"$(INTDIR)\insteng.res" \
	"$(INTDIR)\Tapp.obj" \
	"$(INTDIR)\Tdlg.obj" \
	"$(INTDIR)\Tregist.obj" \
	"$(INTDIR)\Twin.obj"

"$(OUTDIR)\setup.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ENDIF 

.c{$(CPP_OBJS)}.obj:
   $(CPP) $(CPP_PROJ) $<  

.cpp{$(CPP_OBJS)}.obj:
   $(CPP) $(CPP_PROJ) $<  

.cxx{$(CPP_OBJS)}.obj:
   $(CPP) $(CPP_PROJ) $<  

.c{$(CPP_SBRS)}.sbr:
   $(CPP) $(CPP_PROJ) $<  

.cpp{$(CPP_SBRS)}.sbr:
   $(CPP) $(CPP_PROJ) $<  

.cxx{$(CPP_SBRS)}.sbr:
   $(CPP) $(CPP_PROJ) $<  

################################################################################
# Begin Target

# Name "ipmsg - Win32 English"
# Name "ipmsg - Win32 Japanese"
# Name "ipmsg - Win32 Debug"
# Name "ipmsg - Win32 Installer"
# Name "ipmsg - Win32 InstallerDbg"
# Name "ipmsg - Win32 InstallerEng"

!IF  "$(CFG)" == "ipmsg - Win32 English"

!ELSEIF  "$(CFG)" == "ipmsg - Win32 Japanese"

!ELSEIF  "$(CFG)" == "ipmsg - Win32 Debug"

!ELSEIF  "$(CFG)" == "ipmsg - Win32 Installer"

!ELSEIF  "$(CFG)" == "ipmsg - Win32 InstallerDbg"

!ELSEIF  "$(CFG)" == "ipmsg - Win32 InstallerEng"

!ENDIF 

################################################################################
# Begin Source File

SOURCE=.\Src\Ipmsg.cpp
DEP_CPP_IPMSG=\
	".\Src\ipmsg.h"\
	".\Src\tlib.h"\
	

!IF  "$(CFG)" == "ipmsg - Win32 English"


"$(INTDIR)\Ipmsg.obj" : $(SOURCE) $(DEP_CPP_IPMSG) "$(INTDIR)"
   $(CPP) /nologo /ML /W3 /O1 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "ENGLISH"\
 /Fp"$(INTDIR)/ipmsg.pch" /YX /Fo"$(INTDIR)/" /c $(SOURCE)


!ELSEIF  "$(CFG)" == "ipmsg - Win32 Japanese"


"$(INTDIR)\Ipmsg.obj" : $(SOURCE) $(DEP_CPP_IPMSG) "$(INTDIR)"
   $(CPP) /nologo /ML /W3 /O1 /Op /Oy /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D\
 "JAPANESE" /Fp"$(INTDIR)/ipmsg.pch" /YX /Fo"$(INTDIR)/" /c $(SOURCE)


!ELSEIF  "$(CFG)" == "ipmsg - Win32 Debug"


"$(INTDIR)\Ipmsg.obj" : $(SOURCE) $(DEP_CPP_IPMSG) "$(INTDIR)"
   $(CPP) /nologo /MLd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS"\
 /D "JAPANESE" /Fp"$(INTDIR)/ipmsg.pch" /YX /Fo"$(INTDIR)/" /Fd"$(INTDIR)/" /c\
 $(SOURCE)


!ELSEIF  "$(CFG)" == "ipmsg - Win32 Installer"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "ipmsg - Win32 InstallerDbg"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "ipmsg - Win32 InstallerEng"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Src\Logdlg.cpp
DEP_CPP_LOGDL=\
	".\Src\ipmsg.h"\
	".\Src\msgstr.h"\
	".\Src\tlib.h"\
	

!IF  "$(CFG)" == "ipmsg - Win32 English"


"$(INTDIR)\Logdlg.obj" : $(SOURCE) $(DEP_CPP_LOGDL) "$(INTDIR)"
   $(CPP) /nologo /ML /W3 /O1 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "ENGLISH"\
 /Fp"$(INTDIR)/ipmsg.pch" /YX /Fo"$(INTDIR)/" /c $(SOURCE)


!ELSEIF  "$(CFG)" == "ipmsg - Win32 Japanese"


"$(INTDIR)\Logdlg.obj" : $(SOURCE) $(DEP_CPP_LOGDL) "$(INTDIR)"
   $(CPP) /nologo /ML /W3 /O1 /Op /Oy /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D\
 "JAPANESE" /Fp"$(INTDIR)/ipmsg.pch" /YX /Fo"$(INTDIR)/" /c $(SOURCE)


!ELSEIF  "$(CFG)" == "ipmsg - Win32 Debug"


"$(INTDIR)\Logdlg.obj" : $(SOURCE) $(DEP_CPP_LOGDL) "$(INTDIR)"
   $(CPP) /nologo /MLd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS"\
 /D "JAPANESE" /Fp"$(INTDIR)/ipmsg.pch" /YX /Fo"$(INTDIR)/" /Fd"$(INTDIR)/" /c\
 $(SOURCE)


!ELSEIF  "$(CFG)" == "ipmsg - Win32 Installer"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "ipmsg - Win32 InstallerDbg"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "ipmsg - Win32 InstallerEng"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Src\Logmng.cpp
DEP_CPP_LOGMN=\
	".\Src\ipmsg.h"\
	".\Src\msgstr.h"\
	".\Src\tlib.h"\
	

!IF  "$(CFG)" == "ipmsg - Win32 English"


"$(INTDIR)\Logmng.obj" : $(SOURCE) $(DEP_CPP_LOGMN) "$(INTDIR)"
   $(CPP) /nologo /ML /W3 /O1 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "ENGLISH"\
 /Fp"$(INTDIR)/ipmsg.pch" /YX /Fo"$(INTDIR)/" /c $(SOURCE)


!ELSEIF  "$(CFG)" == "ipmsg - Win32 Japanese"


"$(INTDIR)\Logmng.obj" : $(SOURCE) $(DEP_CPP_LOGMN) "$(INTDIR)"
   $(CPP) /nologo /ML /W3 /O1 /Op /Oy /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D\
 "JAPANESE" /Fp"$(INTDIR)/ipmsg.pch" /YX /Fo"$(INTDIR)/" /c $(SOURCE)


!ELSEIF  "$(CFG)" == "ipmsg - Win32 Debug"


"$(INTDIR)\Logmng.obj" : $(SOURCE) $(DEP_CPP_LOGMN) "$(INTDIR)"
   $(CPP) /nologo /MLd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS"\
 /D "JAPANESE" /Fp"$(INTDIR)/ipmsg.pch" /YX /Fo"$(INTDIR)/" /Fd"$(INTDIR)/" /c\
 $(SOURCE)


!ELSEIF  "$(CFG)" == "ipmsg - Win32 Installer"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "ipmsg - Win32 InstallerDbg"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "ipmsg - Win32 InstallerEng"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Src\Mainwin.cpp
DEP_CPP_MAINW=\
	".\src\blowfish.h"\
	".\Src\ipmsg.h"\
	".\Src\msgstr.h"\
	".\Src\tlib.h"\
	

!IF  "$(CFG)" == "ipmsg - Win32 English"


"$(INTDIR)\Mainwin.obj" : $(SOURCE) $(DEP_CPP_MAINW) "$(INTDIR)"
   $(CPP) /nologo /ML /W3 /O1 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "ENGLISH"\
 /Fp"$(INTDIR)/ipmsg.pch" /YX /Fo"$(INTDIR)/" /c $(SOURCE)


!ELSEIF  "$(CFG)" == "ipmsg - Win32 Japanese"


"$(INTDIR)\Mainwin.obj" : $(SOURCE) $(DEP_CPP_MAINW) "$(INTDIR)"
   $(CPP) /nologo /ML /W3 /O1 /Op /Oy /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D\
 "JAPANESE" /Fp"$(INTDIR)/ipmsg.pch" /YX /Fo"$(INTDIR)/" /c $(SOURCE)


!ELSEIF  "$(CFG)" == "ipmsg - Win32 Debug"


"$(INTDIR)\Mainwin.obj" : $(SOURCE) $(DEP_CPP_MAINW) "$(INTDIR)"
   $(CPP) /nologo /MLd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS"\
 /D "JAPANESE" /Fp"$(INTDIR)/ipmsg.pch" /YX /Fo"$(INTDIR)/" /Fd"$(INTDIR)/" /c\
 $(SOURCE)


!ELSEIF  "$(CFG)" == "ipmsg - Win32 Installer"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "ipmsg - Win32 InstallerDbg"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "ipmsg - Win32 InstallerEng"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Src\Msgmng.cpp
DEP_CPP_MSGMN=\
	".\Src\ipmsg.h"\
	".\Src\msgstr.h"\
	".\Src\tlib.h"\
	

!IF  "$(CFG)" == "ipmsg - Win32 English"


"$(INTDIR)\Msgmng.obj" : $(SOURCE) $(DEP_CPP_MSGMN) "$(INTDIR)"
   $(CPP) /nologo /ML /W3 /O1 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "ENGLISH"\
 /Fp"$(INTDIR)/ipmsg.pch" /YX /Fo"$(INTDIR)/" /c $(SOURCE)


!ELSEIF  "$(CFG)" == "ipmsg - Win32 Japanese"


"$(INTDIR)\Msgmng.obj" : $(SOURCE) $(DEP_CPP_MSGMN) "$(INTDIR)"
   $(CPP) /nologo /ML /W3 /O1 /Op /Oy /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D\
 "JAPANESE" /Fp"$(INTDIR)/ipmsg.pch" /YX /Fo"$(INTDIR)/" /c $(SOURCE)


!ELSEIF  "$(CFG)" == "ipmsg - Win32 Debug"


"$(INTDIR)\Msgmng.obj" : $(SOURCE) $(DEP_CPP_MSGMN) "$(INTDIR)"
   $(CPP) /nologo /MLd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS"\
 /D "JAPANESE" /Fp"$(INTDIR)/ipmsg.pch" /YX /Fo"$(INTDIR)/" /Fd"$(INTDIR)/" /c\
 $(SOURCE)


!ELSEIF  "$(CFG)" == "ipmsg - Win32 Installer"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "ipmsg - Win32 InstallerDbg"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "ipmsg - Win32 InstallerEng"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Src\Recvdlg.cpp
DEP_CPP_RECVD=\
	".\src\blowfish.h"\
	".\Src\ipmsg.h"\
	".\Src\msgstr.h"\
	".\Src\tlib.h"\
	

!IF  "$(CFG)" == "ipmsg - Win32 English"


"$(INTDIR)\Recvdlg.obj" : $(SOURCE) $(DEP_CPP_RECVD) "$(INTDIR)"
   $(CPP) /nologo /ML /W3 /O1 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "ENGLISH"\
 /Fp"$(INTDIR)/ipmsg.pch" /YX /Fo"$(INTDIR)/" /c $(SOURCE)


!ELSEIF  "$(CFG)" == "ipmsg - Win32 Japanese"


"$(INTDIR)\Recvdlg.obj" : $(SOURCE) $(DEP_CPP_RECVD) "$(INTDIR)"
   $(CPP) /nologo /ML /W3 /O1 /Op /Oy /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D\
 "JAPANESE" /Fp"$(INTDIR)/ipmsg.pch" /YX /Fo"$(INTDIR)/" /c $(SOURCE)


!ELSEIF  "$(CFG)" == "ipmsg - Win32 Debug"


"$(INTDIR)\Recvdlg.obj" : $(SOURCE) $(DEP_CPP_RECVD) "$(INTDIR)"
   $(CPP) /nologo /MLd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS"\
 /D "JAPANESE" /Fp"$(INTDIR)/ipmsg.pch" /YX /Fo"$(INTDIR)/" /Fd"$(INTDIR)/" /c\
 $(SOURCE)


!ELSEIF  "$(CFG)" == "ipmsg - Win32 Installer"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "ipmsg - Win32 InstallerDbg"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "ipmsg - Win32 InstallerEng"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Src\Senddlg.cpp
DEP_CPP_SENDD=\
	".\src\blowfish.h"\
	".\Src\ipmsg.h"\
	".\Src\msgstr.h"\
	".\Src\tlib.h"\
	

!IF  "$(CFG)" == "ipmsg - Win32 English"


"$(INTDIR)\Senddlg.obj" : $(SOURCE) $(DEP_CPP_SENDD) "$(INTDIR)"
   $(CPP) /nologo /ML /W3 /O1 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "ENGLISH"\
 /Fp"$(INTDIR)/ipmsg.pch" /YX /Fo"$(INTDIR)/" /c $(SOURCE)


!ELSEIF  "$(CFG)" == "ipmsg - Win32 Japanese"


"$(INTDIR)\Senddlg.obj" : $(SOURCE) $(DEP_CPP_SENDD) "$(INTDIR)"
   $(CPP) /nologo /ML /W3 /O1 /Op /Oy /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D\
 "JAPANESE" /Fp"$(INTDIR)/ipmsg.pch" /YX /Fo"$(INTDIR)/" /c $(SOURCE)


!ELSEIF  "$(CFG)" == "ipmsg - Win32 Debug"


"$(INTDIR)\Senddlg.obj" : $(SOURCE) $(DEP_CPP_SENDD) "$(INTDIR)"
   $(CPP) /nologo /MLd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS"\
 /D "JAPANESE" /Fp"$(INTDIR)/ipmsg.pch" /YX /Fo"$(INTDIR)/" /Fd"$(INTDIR)/" /c\
 $(SOURCE)


!ELSEIF  "$(CFG)" == "ipmsg - Win32 Installer"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "ipmsg - Win32 InstallerDbg"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "ipmsg - Win32 InstallerEng"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Src\Setupdlg.cpp
DEP_CPP_SETUP=\
	".\Src\ipmsg.h"\
	".\Src\msgstr.h"\
	".\Src\tlib.h"\
	

!IF  "$(CFG)" == "ipmsg - Win32 English"


"$(INTDIR)\Setupdlg.obj" : $(SOURCE) $(DEP_CPP_SETUP) "$(INTDIR)"
   $(CPP) /nologo /ML /W3 /O1 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "ENGLISH"\
 /Fp"$(INTDIR)/ipmsg.pch" /YX /Fo"$(INTDIR)/" /c $(SOURCE)


!ELSEIF  "$(CFG)" == "ipmsg - Win32 Japanese"


"$(INTDIR)\Setupdlg.obj" : $(SOURCE) $(DEP_CPP_SETUP) "$(INTDIR)"
   $(CPP) /nologo /ML /W3 /O1 /Op /Oy /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D\
 "JAPANESE" /Fp"$(INTDIR)/ipmsg.pch" /YX /Fo"$(INTDIR)/" /c $(SOURCE)


!ELSEIF  "$(CFG)" == "ipmsg - Win32 Debug"


"$(INTDIR)\Setupdlg.obj" : $(SOURCE) $(DEP_CPP_SETUP) "$(INTDIR)"
   $(CPP) /nologo /MLd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS"\
 /D "JAPANESE" /Fp"$(INTDIR)/ipmsg.pch" /YX /Fo"$(INTDIR)/" /Fd"$(INTDIR)/" /c\
 $(SOURCE)


!ELSEIF  "$(CFG)" == "ipmsg - Win32 Installer"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "ipmsg - Win32 InstallerDbg"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "ipmsg - Win32 InstallerEng"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Src\Tapp.cpp
DEP_CPP_TAPP_=\
	".\Src\tlib.h"\
	

!IF  "$(CFG)" == "ipmsg - Win32 English"


"$(INTDIR)\Tapp.obj" : $(SOURCE) $(DEP_CPP_TAPP_) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "ipmsg - Win32 Japanese"


"$(INTDIR)\Tapp.obj" : $(SOURCE) $(DEP_CPP_TAPP_) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "ipmsg - Win32 Debug"


"$(INTDIR)\Tapp.obj" : $(SOURCE) $(DEP_CPP_TAPP_) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "ipmsg - Win32 Installer"


"$(INTDIR)\Tapp.obj" : $(SOURCE) $(DEP_CPP_TAPP_) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "ipmsg - Win32 InstallerDbg"


".\obj\debug\Tapp.obj" : $(SOURCE) $(DEP_CPP_TAPP_) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "ipmsg - Win32 InstallerEng"


"$(INTDIR)\Tapp.obj" : $(SOURCE) $(DEP_CPP_TAPP_) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Src\Tdlg.cpp
DEP_CPP_TDLG_=\
	".\Src\tlib.h"\
	

!IF  "$(CFG)" == "ipmsg - Win32 English"


"$(INTDIR)\Tdlg.obj" : $(SOURCE) $(DEP_CPP_TDLG_) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "ipmsg - Win32 Japanese"


"$(INTDIR)\Tdlg.obj" : $(SOURCE) $(DEP_CPP_TDLG_) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "ipmsg - Win32 Debug"


"$(INTDIR)\Tdlg.obj" : $(SOURCE) $(DEP_CPP_TDLG_) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "ipmsg - Win32 Installer"


"$(INTDIR)\Tdlg.obj" : $(SOURCE) $(DEP_CPP_TDLG_) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "ipmsg - Win32 InstallerDbg"


".\obj\debug\Tdlg.obj" : $(SOURCE) $(DEP_CPP_TDLG_) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "ipmsg - Win32 InstallerEng"


"$(INTDIR)\Tdlg.obj" : $(SOURCE) $(DEP_CPP_TDLG_) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Src\Tlist.cpp
DEP_CPP_TLIST=\
	".\Src\tlib.h"\
	

!IF  "$(CFG)" == "ipmsg - Win32 English"


"$(INTDIR)\Tlist.obj" : $(SOURCE) $(DEP_CPP_TLIST) "$(INTDIR)"
   $(CPP) /nologo /ML /W3 /O1 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "ENGLISH"\
 /Fp"$(INTDIR)/ipmsg.pch" /YX /Fo"$(INTDIR)/" /c $(SOURCE)


!ELSEIF  "$(CFG)" == "ipmsg - Win32 Japanese"


"$(INTDIR)\Tlist.obj" : $(SOURCE) $(DEP_CPP_TLIST) "$(INTDIR)"
   $(CPP) /nologo /ML /W3 /O1 /Op /Oy /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D\
 "JAPANESE" /Fp"$(INTDIR)/ipmsg.pch" /YX /Fo"$(INTDIR)/" /c $(SOURCE)


!ELSEIF  "$(CFG)" == "ipmsg - Win32 Debug"


"$(INTDIR)\Tlist.obj" : $(SOURCE) $(DEP_CPP_TLIST) "$(INTDIR)"
   $(CPP) /nologo /MLd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS"\
 /D "JAPANESE" /Fp"$(INTDIR)/ipmsg.pch" /YX /Fo"$(INTDIR)/" /Fd"$(INTDIR)/" /c\
 $(SOURCE)


!ELSEIF  "$(CFG)" == "ipmsg - Win32 Installer"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "ipmsg - Win32 InstallerDbg"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "ipmsg - Win32 InstallerEng"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Src\Tregist.cpp
DEP_CPP_TREGI=\
	".\Src\tlib.h"\
	

!IF  "$(CFG)" == "ipmsg - Win32 English"


"$(INTDIR)\Tregist.obj" : $(SOURCE) $(DEP_CPP_TREGI) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "ipmsg - Win32 Japanese"


"$(INTDIR)\Tregist.obj" : $(SOURCE) $(DEP_CPP_TREGI) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "ipmsg - Win32 Debug"


"$(INTDIR)\Tregist.obj" : $(SOURCE) $(DEP_CPP_TREGI) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "ipmsg - Win32 Installer"


"$(INTDIR)\Tregist.obj" : $(SOURCE) $(DEP_CPP_TREGI) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "ipmsg - Win32 InstallerDbg"


".\obj\debug\Tregist.obj" : $(SOURCE) $(DEP_CPP_TREGI) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "ipmsg - Win32 InstallerEng"


"$(INTDIR)\Tregist.obj" : $(SOURCE) $(DEP_CPP_TREGI) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Src\Twin.cpp
DEP_CPP_TWIN_=\
	".\Src\tlib.h"\
	

!IF  "$(CFG)" == "ipmsg - Win32 English"


"$(INTDIR)\Twin.obj" : $(SOURCE) $(DEP_CPP_TWIN_) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "ipmsg - Win32 Japanese"


"$(INTDIR)\Twin.obj" : $(SOURCE) $(DEP_CPP_TWIN_) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "ipmsg - Win32 Debug"


"$(INTDIR)\Twin.obj" : $(SOURCE) $(DEP_CPP_TWIN_) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "ipmsg - Win32 Installer"


"$(INTDIR)\Twin.obj" : $(SOURCE) $(DEP_CPP_TWIN_) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "ipmsg - Win32 InstallerDbg"


".\obj\debug\Twin.obj" : $(SOURCE) $(DEP_CPP_TWIN_) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "ipmsg - Win32 InstallerEng"


"$(INTDIR)\Twin.obj" : $(SOURCE) $(DEP_CPP_TWIN_) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Src\Miscdlg.cpp
DEP_CPP_MISCD=\
	".\src\blowfish.h"\
	".\Src\ipmsg.h"\
	".\Src\msgstr.h"\
	".\Src\tlib.h"\
	

!IF  "$(CFG)" == "ipmsg - Win32 English"


"$(INTDIR)\Miscdlg.obj" : $(SOURCE) $(DEP_CPP_MISCD) "$(INTDIR)"
   $(CPP) /nologo /ML /W3 /O1 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "ENGLISH"\
 /Fp"$(INTDIR)/ipmsg.pch" /YX /Fo"$(INTDIR)/" /c $(SOURCE)


!ELSEIF  "$(CFG)" == "ipmsg - Win32 Japanese"


"$(INTDIR)\Miscdlg.obj" : $(SOURCE) $(DEP_CPP_MISCD) "$(INTDIR)"
   $(CPP) /nologo /ML /W3 /O1 /Op /Oy /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D\
 "JAPANESE" /Fp"$(INTDIR)/ipmsg.pch" /YX /Fo"$(INTDIR)/" /c $(SOURCE)


!ELSEIF  "$(CFG)" == "ipmsg - Win32 Debug"


"$(INTDIR)\Miscdlg.obj" : $(SOURCE) $(DEP_CPP_MISCD) "$(INTDIR)"
   $(CPP) /nologo /MLd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS"\
 /D "JAPANESE" /Fp"$(INTDIR)/ipmsg.pch" /YX /Fo"$(INTDIR)/" /Fd"$(INTDIR)/" /c\
 $(SOURCE)


!ELSEIF  "$(CFG)" == "ipmsg - Win32 Installer"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "ipmsg - Win32 InstallerDbg"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "ipmsg - Win32 InstallerEng"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Src\ipmsgjpn.rc

!IF  "$(CFG)" == "ipmsg - Win32 English"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "ipmsg - Win32 Japanese"

DEP_RSC_IPMSGJ=\
	".\src\absence.ico"\
	".\src\file.ico"\
	".\src\fileabs.ico"\
	".\src\ipmsg.ico"\
	".\src\ipmsgrev.ico"\
	".\src\v1.ico"\
	".\src\v1abs.ico"\
	

"$(INTDIR)\ipmsgjpn.res" : $(SOURCE) $(DEP_RSC_IPMSGJ) "$(INTDIR)"
   $(RSC) /l 0x411 /fo"$(INTDIR)/ipmsgjpn.res" /i "Src" /d "NDEBUG" $(SOURCE)


!ELSEIF  "$(CFG)" == "ipmsg - Win32 Debug"

DEP_RSC_IPMSGJ=\
	".\src\absence.ico"\
	".\src\file.ico"\
	".\src\fileabs.ico"\
	".\src\ipmsg.ico"\
	".\src\ipmsgrev.ico"\
	".\src\v1.ico"\
	".\src\v1abs.ico"\
	

"$(INTDIR)\ipmsgjpn.res" : $(SOURCE) $(DEP_RSC_IPMSGJ) "$(INTDIR)"
   $(RSC) /l 0x411 /fo"$(INTDIR)/ipmsgjpn.res" /i "Src" /d "_DEBUG" $(SOURCE)


!ELSEIF  "$(CFG)" == "ipmsg - Win32 Installer"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "ipmsg - Win32 InstallerDbg"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "ipmsg - Win32 InstallerEng"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Src\ipmsgeng.rc

!IF  "$(CFG)" == "ipmsg - Win32 English"

# PROP Exclude_From_Build 0
DEP_RSC_IPMSGE=\
	".\src\absence.ico"\
	".\src\file.ico"\
	".\src\fileabs.ico"\
	".\src\ipmsg.ico"\
	".\src\ipmsgrev.ico"\
	".\src\v1.ico"\
	".\src\v1abs.ico"\
	

"$(INTDIR)\ipmsgeng.res" : $(SOURCE) $(DEP_RSC_IPMSGE) "$(INTDIR)"
   $(RSC) /l 0x409 /fo"$(INTDIR)/ipmsgeng.res" /i "Src" /d "NDEBUG" $(SOURCE)


!ELSEIF  "$(CFG)" == "ipmsg - Win32 Japanese"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "ipmsg - Win32 Debug"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "ipmsg - Win32 Installer"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "ipmsg - Win32 InstallerDbg"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "ipmsg - Win32 InstallerEng"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Src\cfg.cpp
DEP_CPP_CFG_C=\
	".\src\blowfish.h"\
	".\Src\ipmsg.h"\
	".\Src\msgstr.h"\
	".\Src\tlib.h"\
	

!IF  "$(CFG)" == "ipmsg - Win32 English"


"$(INTDIR)\cfg.obj" : $(SOURCE) $(DEP_CPP_CFG_C) "$(INTDIR)"
   $(CPP) /nologo /ML /W3 /O1 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "ENGLISH"\
 /Fp"$(INTDIR)/ipmsg.pch" /YX /Fo"$(INTDIR)/" /c $(SOURCE)


!ELSEIF  "$(CFG)" == "ipmsg - Win32 Japanese"


"$(INTDIR)\cfg.obj" : $(SOURCE) $(DEP_CPP_CFG_C) "$(INTDIR)"
   $(CPP) /nologo /ML /W3 /O1 /Op /Oy /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D\
 "JAPANESE" /Fp"$(INTDIR)/ipmsg.pch" /YX /Fo"$(INTDIR)/" /c $(SOURCE)


!ELSEIF  "$(CFG)" == "ipmsg - Win32 Debug"


"$(INTDIR)\cfg.obj" : $(SOURCE) $(DEP_CPP_CFG_C) "$(INTDIR)"
   $(CPP) /nologo /MLd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS"\
 /D "JAPANESE" /Fp"$(INTDIR)/ipmsg.pch" /YX /Fo"$(INTDIR)/" /Fd"$(INTDIR)/" /c\
 $(SOURCE)


!ELSEIF  "$(CFG)" == "ipmsg - Win32 Installer"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "ipmsg - Win32 InstallerDbg"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "ipmsg - Win32 InstallerEng"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Src\plugin.cpp
DEP_CPP_PLUGI=\
	".\Src\ipmsg.h"\
	".\Src\msgstr.h"\
	".\Src\tlib.h"\
	

!IF  "$(CFG)" == "ipmsg - Win32 English"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "ipmsg - Win32 Japanese"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "ipmsg - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "ipmsg - Win32 Installer"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "ipmsg - Win32 InstallerDbg"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "ipmsg - Win32 InstallerEng"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Src\Install\install.cpp
DEP_CPP_INSTA=\
	".\Src\Install\install.h"\
	".\Src\tlib.h"\
	

!IF  "$(CFG)" == "ipmsg - Win32 English"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "ipmsg - Win32 Japanese"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "ipmsg - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "ipmsg - Win32 Installer"


"$(INTDIR)\install.obj" : $(SOURCE) $(DEP_CPP_INSTA) "$(INTDIR)"
   $(CPP) /nologo /ML /W3 /O1 /D "WIN32" /D "_WINDOWS" /D "JAPANESE"\
 /Fp"$(INTDIR)/ipmsg.pch" /YX /Fo"$(INTDIR)/" /c $(SOURCE)


!ELSEIF  "$(CFG)" == "ipmsg - Win32 InstallerDbg"


".\obj\debug\install.obj" : $(SOURCE) $(DEP_CPP_INSTA) "$(INTDIR)"
   $(CPP) /nologo /MLd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS"\
 /D "JAPANESE" /Fp"obj/debug/ipmsg.pch" /YX /Fo"obj/debug/" /Fd"obj/debug/" /c\
 $(SOURCE)


!ELSEIF  "$(CFG)" == "ipmsg - Win32 InstallerEng"


"$(INTDIR)\install.obj" : $(SOURCE) $(DEP_CPP_INSTA) "$(INTDIR)"
   $(CPP) /nologo /ML /W3 /O1 /D "WIN32" /D "_WINDOWS" /D "ENGLISH"\
 /Fp"$(INTDIR)/ipmsg.pch" /YX /Fo"$(INTDIR)/" /c $(SOURCE)


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Src\Install\insteng.rc

!IF  "$(CFG)" == "ipmsg - Win32 English"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "ipmsg - Win32 Japanese"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "ipmsg - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "ipmsg - Win32 Installer"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "ipmsg - Win32 InstallerDbg"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "ipmsg - Win32 InstallerEng"

DEP_RSC_INSTE=\
	".\src\install\setup.ico"\
	

"$(INTDIR)\insteng.res" : $(SOURCE) $(DEP_RSC_INSTE) "$(INTDIR)"
   $(RSC) /l 0x411 /fo"$(INTDIR)/insteng.res" /i "Src\Install" /d "NDEBUG"\
 $(SOURCE)


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Src\Install\instjpn.rc

!IF  "$(CFG)" == "ipmsg - Win32 English"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "ipmsg - Win32 Japanese"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "ipmsg - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "ipmsg - Win32 Installer"

# PROP Exclude_From_Build 0
DEP_RSC_INSTJ=\
	".\src\install\setup.ico"\
	

"$(INTDIR)\instjpn.res" : $(SOURCE) $(DEP_RSC_INSTJ) "$(INTDIR)"
   $(RSC) /l 0x411 /fo"$(INTDIR)/instjpn.res" /i "Src\Install" /d "NDEBUG"\
 $(SOURCE)


!ELSEIF  "$(CFG)" == "ipmsg - Win32 InstallerDbg"

# PROP Exclude_From_Build 0
DEP_RSC_INSTJ=\
	".\src\install\setup.ico"\
	

".\obj\debug\install.res" : $(SOURCE) $(DEP_RSC_INSTJ) "$(INTDIR)"
   $(RSC) /l 0x411 /fo"obj/debug/install.res" /i "Src\Install" /d "NDEBUG"\
 $(SOURCE)


!ELSEIF  "$(CFG)" == "ipmsg - Win32 InstallerEng"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\src\share.cpp
DEP_CPP_SHARE=\
	".\Src\ipmsg.h"\
	".\Src\msgstr.h"\
	".\Src\tlib.h"\
	

!IF  "$(CFG)" == "ipmsg - Win32 English"


"$(INTDIR)\share.obj" : $(SOURCE) $(DEP_CPP_SHARE) "$(INTDIR)"
   $(CPP) /nologo /ML /W3 /O1 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "ENGLISH"\
 /Fp"$(INTDIR)/ipmsg.pch" /YX /Fo"$(INTDIR)/" /c $(SOURCE)


!ELSEIF  "$(CFG)" == "ipmsg - Win32 Japanese"


"$(INTDIR)\share.obj" : $(SOURCE) $(DEP_CPP_SHARE) "$(INTDIR)"
   $(CPP) /nologo /ML /W3 /O1 /Op /Oy /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D\
 "JAPANESE" /Fp"$(INTDIR)/ipmsg.pch" /YX /Fo"$(INTDIR)/" /c $(SOURCE)


!ELSEIF  "$(CFG)" == "ipmsg - Win32 Debug"


"$(INTDIR)\share.obj" : $(SOURCE) $(DEP_CPP_SHARE) "$(INTDIR)"
   $(CPP) /nologo /MLd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS"\
 /D "JAPANESE" /Fp"$(INTDIR)/ipmsg.pch" /YX /Fo"$(INTDIR)/" /Fd"$(INTDIR)/" /c\
 $(SOURCE)


!ELSEIF  "$(CFG)" == "ipmsg - Win32 Installer"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "ipmsg - Win32 InstallerDbg"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "ipmsg - Win32 InstallerEng"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\src\blowfish.cpp
DEP_CPP_BLOWF=\
	".\src\blowfish.h"\
	".\src\blowfish.h2"\
	

!IF  "$(CFG)" == "ipmsg - Win32 English"


"$(INTDIR)\blowfish.obj" : $(SOURCE) $(DEP_CPP_BLOWF) "$(INTDIR)"
   $(CPP) /nologo /ML /W3 /O1 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "ENGLISH"\
 /Fp"$(INTDIR)/ipmsg.pch" /YX /Fo"$(INTDIR)/" /c $(SOURCE)


!ELSEIF  "$(CFG)" == "ipmsg - Win32 Japanese"


"$(INTDIR)\blowfish.obj" : $(SOURCE) $(DEP_CPP_BLOWF) "$(INTDIR)"
   $(CPP) /nologo /ML /W3 /O1 /Op /Oy /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D\
 "JAPANESE" /Fp"$(INTDIR)/ipmsg.pch" /YX /Fo"$(INTDIR)/" /c $(SOURCE)


!ELSEIF  "$(CFG)" == "ipmsg - Win32 Debug"


"$(INTDIR)\blowfish.obj" : $(SOURCE) $(DEP_CPP_BLOWF) "$(INTDIR)"
   $(CPP) /nologo /MLd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS"\
 /D "JAPANESE" /Fp"$(INTDIR)/ipmsg.pch" /YX /Fo"$(INTDIR)/" /Fd"$(INTDIR)/" /c\
 $(SOURCE)


!ELSEIF  "$(CFG)" == "ipmsg - Win32 Installer"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "ipmsg - Win32 InstallerDbg"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "ipmsg - Win32 InstallerEng"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\src\v1abs.ico

!IF  "$(CFG)" == "ipmsg - Win32 English"

!ELSEIF  "$(CFG)" == "ipmsg - Win32 Japanese"

!ELSEIF  "$(CFG)" == "ipmsg - Win32 Debug"

!ELSEIF  "$(CFG)" == "ipmsg - Win32 Installer"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "ipmsg - Win32 InstallerDbg"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "ipmsg - Win32 InstallerEng"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\src\v1.ico

!IF  "$(CFG)" == "ipmsg - Win32 English"

!ELSEIF  "$(CFG)" == "ipmsg - Win32 Japanese"

!ELSEIF  "$(CFG)" == "ipmsg - Win32 Debug"

!ELSEIF  "$(CFG)" == "ipmsg - Win32 Installer"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "ipmsg - Win32 InstallerDbg"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "ipmsg - Win32 InstallerEng"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# End Target
# End Project
################################################################################
