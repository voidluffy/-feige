/*	@(#)Copyright (C) H.Shirouzu 1996-2004   ipmsg.h	Ver2.05 */
/* ========================================================================
	Project  Name			: IP Messenger for Win32
	Module Name				: Main Header
	Create					: 1996-06-01(Sat)
	Update					: 2004-01-01(Thu)
	Copyright				: H.Shirouzu
	Reference				: 
	======================================================================== */

#ifndef IPMSG_H
#define IPMSG_H

#include <time.h>

/*  IP Messenger Communication Protocol version 1.2 define  */
/*  macro  */
#define GET_MODE(command)	(command & 0x000000ffUL)
#define GET_OPT(command)	(command & 0xffffff00UL)

/*  header  */
#define IPMSG_VERSION			0x0001
#define IPMSG_DEFAULT_PORT		0x0979

/*  command  */
#define IPMSG_NOOPERATION		0x00000000UL

#define IPMSG_BR_ENTRY			0x00000001UL
#define IPMSG_BR_EXIT			0x00000002UL
#define IPMSG_ANSENTRY			0x00000003UL
#define IPMSG_BR_ABSENCE		0x00000004UL

#define IPMSG_BR_ISGETLIST		0x00000010UL
#define IPMSG_OKGETLIST			0x00000011UL
#define IPMSG_GETLIST			0x00000012UL
#define IPMSG_ANSLIST			0x00000013UL
#define IPMSG_BR_ISGETLIST2		0x00000018UL

#define IPMSG_SENDMSG			0x00000020UL
#define IPMSG_RECVMSG			0x00000021UL
#define IPMSG_READMSG			0x00000030UL
#define IPMSG_DELMSG			0x00000031UL
#define IPMSG_ANSREADMSG		0x00000032UL

#define IPMSG_GETINFO			0x00000040UL
#define IPMSG_SENDINFO			0x00000041UL

#define IPMSG_GETABSENCEINFO	0x00000050UL
#define IPMSG_SENDABSENCEINFO	0x00000051UL

#define IPMSG_GETFILEDATA		0x00000060UL
#define IPMSG_RELEASEFILES		0x00000061UL
#define IPMSG_GETDIRFILES		0x00000062UL

#define IPMSG_GETPUBKEY			0x00000072UL
#define IPMSG_ANSPUBKEY			0x00000073UL

/*  option for all command  */
#define IPMSG_ABSENCEOPT		0x00000100UL
#define IPMSG_SERVEROPT			0x00000200UL
#define IPMSG_DIALUPOPT			0x00010000UL
#define IPMSG_FILEATTACHOPT		0x00200000UL
#define IPMSG_ENCRYPTOPT		0x00400000UL

/*  option for send command  */
#define IPMSG_SENDCHECKOPT		0x00000100UL
#define IPMSG_SECRETOPT			0x00000200UL
#define IPMSG_BROADCASTOPT		0x00000400UL
#define IPMSG_MULTICASTOPT		0x00000800UL
#define IPMSG_NOPOPUPOPT		0x00001000UL
#define IPMSG_AUTORETOPT		0x00002000UL
#define IPMSG_RETRYOPT			0x00004000UL
#define IPMSG_PASSWORDOPT		0x00008000UL
#define IPMSG_NOLOGOPT			0x00020000UL
#define IPMSG_NEWMUTIOPT		0x00040000UL
#define IPMSG_NOADDLISTOPT		0x00080000UL
#define IPMSG_READCHECKOPT		0x00100000UL
#define IPMSG_SECRETEXOPT		(IPMSG_READCHECKOPT|IPMSG_SECRETOPT)

/* encryption flags for encrypt command */
#define IPMSG_RSA_512			0x00000001UL
#define IPMSG_RSA_1024			0x00000002UL
#define IPMSG_RSA_2048			0x00000004UL
#define IPMSG_RC2_40			0x00001000UL
#define IPMSG_RC2_128			0x00004000UL
#define IPMSG_RC2_256			0x00008000UL
#define IPMSG_BLOWFISH_128		0x00020000UL
#define IPMSG_BLOWFISH_256		0x00040000UL
#define IPMSG_SIGN_MD5			0x10000000UL

/* compatibilty for Win beta version */
#define IPMSG_RC2_40OLD			0x00000010UL	// for beta1-4 only
#define IPMSG_RC2_128OLD		0x00000040UL	// for beta1-4 only
#define IPMSG_BLOWFISH_128OLD	0x00000400UL	// for beta1-4 only
#define IPMSG_RC2_40ALL			(IPMSG_RC2_40|IPMSG_RC2_40OLD)
#define IPMSG_RC2_128ALL		(IPMSG_RC2_128|IPMSG_RC2_128OLD)
#define IPMSG_BLOWFISH_128ALL	(IPMSG_BLOWFISH_128|IPMSG_BLOWFISH_128OLD)

/* file types for fileattach command */
#define IPMSG_FILE_REGULAR		0x00000001UL
#define IPMSG_FILE_DIR			0x00000002UL
#define IPMSG_FILE_RETPARENT	0x00000003UL	// return parent directory
#define IPMSG_FILE_SYMLINK		0x00000004UL
#define IPMSG_FILE_CDEV			0x00000005UL	// for UNIX
#define IPMSG_FILE_BDEV			0x00000006UL	// for UNIX
#define IPMSG_FILE_FIFO			0x00000007UL	// for UNIX
#define IPMSG_FILE_RESFORK		0x00000010UL	// for Mac

/* file attribute options for fileattach command */
#define IPMSG_FILE_RONLYOPT		0x00000100UL
#define IPMSG_FILE_HIDDENOPT	0x00001000UL
#define IPMSG_FILE_EXHIDDENOPT	0x00002000UL	// for MacOS X
#define IPMSG_FILE_ARCHIVEOPT	0x00004000UL
#define IPMSG_FILE_SYSTEMOPT	0x00008000UL

/* extend attribute types for fileattach command */
#define IPMSG_FILE_UID			0x00000001UL
#define IPMSG_FILE_USERNAME		0x00000002UL	// uid by string
#define IPMSG_FILE_GID			0x00000003UL
#define IPMSG_FILE_GROUPNAME	0x00000004UL	// gid by string
#define IPMSG_FILE_PERM			0x00000010UL	// for UNIX
#define IPMSG_FILE_MAJORNO		0x00000011UL	// for UNIX devfile
#define IPMSG_FILE_MINORNO		0x00000012UL	// for UNIX devfile
#define IPMSG_FILE_CTIME		0x00000013UL	// for UNIX
#define IPMSG_FILE_MTIME		0x00000014UL
#define IPMSG_FILE_ATIME		0x00000015UL
#define IPMSG_FILE_CREATETIME	0x00000016UL
#define IPMSG_FILE_CREATOR		0x00000020UL	// for Mac
#define IPMSG_FILE_FILETYPE		0x00000021UL	// for Mac
#define IPMSG_FILE_FINDERINFO	0x00000022UL	// for Mac
#define IPMSG_FILE_ACL			0x00000030UL
#define IPMSG_FILE_ALIASFNAME	0x00000040UL	// alias fname
#define IPMSG_FILE_UNICODEFNAME	0x00000041UL	// UNICODE fname

#define FILELIST_SEPARATOR	'\a'
#define HOSTLIST_SEPARATOR	'\a'
#define HOSTLIST_DUMMY		"\b"

/*  end of IP Messenger Communication Protocol version 1.2 define  */


/*  IP Messenger for Windows  internal define  */
#define IPMSG_REVERSEICON			0x0100
#define IPMSG_TIMERINTERVAL			500
#define IPMSG_ENTRYMINSEC			5
#define IPMSG_GETLIST_FINISH		0

#define IPMSG_BROADCAST_TIMER		0x0101
#define IPMSG_SEND_TIMER			0x0102
#define IPMSG_LISTGET_TIMER			0x0104
#define IPMSG_LISTGETRETRY_TIMER	0x0105
#define IPMSG_ENTRY_TIMER			0x0106
#define IPMSG_DUMMY_TIMER			0x0107
#define IPMSG_RECV_TIMER			0x0108
#define IPMSG_ANS_TIMER				0x0109

#define IPMSG_NICKNAME			1
#define IPMSG_FULLNAME			2

#define IPMSG_NAMESORT			0x00000000
#define IPMSG_IPADDRSORT		0x00000001
#define IPMSG_HOSTSORT			0x00000002
#define IPMSG_NOGROUPSORTOPT	0x00000100
#define IPMSG_ICMPSORTOPT		0x00000200
#define IPMSG_NOKANJISORTOPT	0x00000400
#define IPMSG_ALLREVSORTOPT		0x00000800
#define IPMSG_GROUPREVSORTOPT	0x00001000
#define IPMSG_SUBREVSORTOPT		0x00002000

#define WM_IPMSG_NOTIFY			(WM_USER + 101)
#define WM_IPMSG_INITICON		(WM_USER + 102)
#define WM_RECVDLG_OPEN			(WM_USER + 110)
#define WM_RECVDLG_EXIT			(WM_USER + 111)
#define WM_RECVDLG_FILEBUTTON	(WM_USER + 112)
#define WM_SENDDLG_OPEN			(WM_USER + 121)
#define WM_SENDDLG_CREATE		(WM_USER + 122)
#define WM_SENDDLG_EXIT			(WM_USER + 123)
#define WM_SENDDLG_HIDE			(WM_USER + 124)
#define WM_SENDDLG_RESIZE		(WM_USER + 125)
#define WM_UDPEVENT				(WM_USER + 130)
#define WM_TCPEVENT				(WM_USER + 131)
#define WM_REFRESH_HOST			(WM_USER + 140)
#define WM_MSGDLG_EXIT			(WM_USER + 150)
#define WM_DELMISCDLG			(WM_USER + 151)
#define WM_HIDE_CHILDWIN		(WM_USER + 160)
#define WM_EDIT_DBLCLK			(WM_USER + 170)
#define WM_DELAYSETTEXT			(WM_USER + 180)

// ListView extended define for VC4
#ifndef LVM_SETEXTENDEDLISTVIEWSTYLE
#define LVM_SETEXTENDEDLISTVIEWSTYLE	(LVM_FIRST + 54)
#define LVM_GETEXTENDEDLISTVIEWSTYLE	(LVM_FIRST + 55)
#define LVM_SETCOLUMNORDERARRAY			(LVM_FIRST + 58)
#define LVM_GETCOLUMNORDERARRAY			(LVM_FIRST + 59)
#define LVS_EX_GRIDLINES				0x00000001
#define LVS_EX_HEADERDRAGDROP			0x00000010
#define LVS_EX_FULLROWSELECT			0x00000020
#define LVS_SHOWSELALWAYS				0x0008
#endif

// ListView extended define for VC4 & VC5
#ifndef LVM_SETSELECTIONMARK
#define LVM_SETSELECTIONMARK			(LVM_FIRST + 67)
#endif

// CryptoAPI for VC4
#ifndef MS_DEF_PROV
typedef unsigned long HCRYPTPROV;
typedef unsigned long HCRYPTKEY;
typedef unsigned long HCRYPTHASH;
typedef unsigned int ALG_ID;
#define ALG_TYPE_RSA			(2 << 9)
#define ALG_TYPE_BLOCK			(3 << 9)
#define ALG_CLASS_DATA_ENCRYPT	(3 << 13)
#define ALG_CLASS_HASH			(4 << 13)
#define ALG_CLASS_KEY_EXCHANGE	(5 << 13)
#define ALG_SID_RSA_ANY			0
#define ALG_TYPE_ANY			0
#define ALG_SID_RC2				2
#define ALG_SID_MD5				3
#define CALG_RSA_KEYX			(ALG_CLASS_KEY_EXCHANGE|ALG_TYPE_RSA|ALG_SID_RSA_ANY)
#define CALG_RC2				(ALG_CLASS_DATA_ENCRYPT|ALG_TYPE_BLOCK|ALG_SID_RC2)
#define CALG_MD5				(ALG_CLASS_HASH | ALG_TYPE_ANY | ALG_SID_MD5)
#define CRYPT_EXPORTABLE		0x00000001
#define PROV_RSA_FULL			1
#define MS_DEF_PROV				"Microsoft Base Cryptographic Provider v1.0"
#define MS_ENHANCED_PROV		"Microsoft Enhanced Cryptographic Provider v1.0"
#define CUR_BLOB_VERSION		0x02
#define SIMPLEBLOB				0x1
#define PUBLICKEYBLOB			0x6
#define PRIVATEKEYBLOB          0x7
#define CRYPT_NEWKEYSET			0x00000008
#define CRYPT_DELETEKEYSET      0x00000010
#define CRYPT_MACHINE_KEYSET    0x00000020
#define AT_KEYEXCHANGE			1
#define AT_SIGNATURE			2
#define KP_EFFECTIVE_KEYLEN		19	// for CryptSetKeyParam
#define NTE_BAD_KEY				0x80090003L

typedef struct _CRYPTOAPI_BLOB {
	DWORD   cbData;
	BYTE    *pbData;
} DATA_BLOB, *PDATA_BLOB;
#endif

typedef struct _CRYPTPROTECT_PROMPTSTRUCT {
	DWORD cbSize;
	DWORD dwPromptFlags;
	HWND hwndApp;
	LPCWSTR szPrompt;
} CRYPTPROTECT_PROMPTSTRUCT, *PCRYPTPROTECT_PROMPTSTRUCT;

#define CRYPTPROTECT_UI_FORBIDDEN		0x1
#define CRYPTPROTECT_LOCAL_MACHINE		0x4
#define CRYPTPROTECT_CRED_SYNC			0x8
#define CRYPTPROTECT_AUDIT				0x10
#define CRYPTPROTECT_VERIFY_PROTECTION	0x40

#define SKEY_HEADER_SIZE	12

#define PRIV_BLOB_USER			0x0001
#define PRIV_BLOB_DPAPI			0x0002
#define PRIV_BLOB_RAW			0x0003
#define PRIV_SEED_HEADER		"ipmsg:"
#define PRIV_SEED_HEADER_LEN	6
#define PRIV_SEED_LEN			(PRIV_SEED_HEADER_LEN + (128/8))	// 128bit seed

extern BOOL (WINAPI *pCryptAcquireContext)(HCRYPTPROV *, LPCSTR, LPCSTR, DWORD, DWORD);
extern BOOL (WINAPI *pCryptDestroyKey)(HCRYPTKEY);
extern BOOL (WINAPI *pCryptGetKeyParam)(HCRYPTKEY, DWORD, BYTE *, DWORD *, DWORD);
extern BOOL (WINAPI *pCryptSetKeyParam)(HCRYPTKEY, DWORD, BYTE *, DWORD);
extern BOOL (WINAPI *pCryptExportKey)(HCRYPTKEY, HCRYPTKEY, DWORD, DWORD, BYTE *, DWORD *);
extern BOOL (WINAPI *pCryptGetUserKey)(HCRYPTPROV, DWORD, HCRYPTKEY *);
extern BOOL (WINAPI *pCryptEncrypt)(HCRYPTKEY, HCRYPTHASH, BOOL, DWORD, BYTE *, DWORD *, DWORD);
extern BOOL (WINAPI *pCryptGenKey)(HCRYPTPROV, ALG_ID, DWORD, HCRYPTKEY *);
extern BOOL (WINAPI *pCryptGenRandom)(HCRYPTPROV, DWORD, BYTE *);
extern BOOL (WINAPI *pCryptImportKey)(HCRYPTPROV, CONST BYTE *, DWORD, HCRYPTKEY, DWORD, HCRYPTKEY *);
extern BOOL (WINAPI *pCryptDecrypt)(HCRYPTKEY, HCRYPTHASH, BOOL, DWORD, BYTE *, DWORD *);
extern BOOL (WINAPI *pCryptCreateHash)(HCRYPTPROV, ALG_ID, HCRYPTKEY, DWORD, HCRYPTHASH *);
extern BOOL (WINAPI *pCryptHashData)(HCRYPTHASH, BYTE *, DWORD, DWORD);
extern BOOL (WINAPI *pCryptSignHash)(HCRYPTHASH, DWORD, LPCSTR, DWORD, BYTE *, DWORD *);
extern BOOL (WINAPI *pCryptVerifySignature)(HCRYPTHASH, CONST BYTE *, DWORD, HCRYPTKEY, LPCSTR, DWORD);
extern BOOL (WINAPI *pCryptReleaseContext)(HCRYPTPROV, DWORD);
extern BOOL (WINAPI *pCryptProtectData)(DATA_BLOB* pDataIn, LPCWSTR szDataDescr, DATA_BLOB* pOptionalEntropy, PVOID pvReserved, CRYPTPROTECT_PROMPTSTRUCT* pPromptStruct, DWORD dwFlags, DATA_BLOB* pDataOut);
extern BOOL (WINAPI *pCryptUnprotectData)(DATA_BLOB* pDataIn, LPWSTR* ppszDataDescr, DATA_BLOB* pOptionalEntropy, PVOID pvReserved, CRYPTPROTECT_PROMPTSTRUCT* pPromptStruct, DWORD dwFlags, DATA_BLOB* pDataOut);

// General define
#define MAX_SOCKBUF		65536
#define MAX_UDPBUF		16384
#define MAX_CRYPTLEN	((MAX_UDPBUF - MAX_BUF) / 2)
#define MAX_BUF			1024
#define MAX_NAMEBUF		50
#define MAX_LANGBUF		10
#define MAX_LISTBUF		(MAX_NAMEBUF * 3 + 50)
#define MAX_ANSLIST		100

#define HS_TOOLS		"HSTools"
#define IP_MSG			"IPMsg"
#define NO_NAME			"no_name"
#define URL_STR			"://"
#define MAILTO_STR		"mailto:"
#define MSG_STR			"msg"

#define DEFAULT_PRIORITY	10
#define PRIORITY_OFFSET		10
#define DEFAULT_PRIORITYMAX	5

#ifdef JAPANESE
#define PROPORTIONAL_FONT	"ＭＳ Ｐゴシック"
#endif

#define ALIGN_BLOCK(size, align_size) (((size) + (align_size) -1) / (align_size))

class PubKey {
protected:
	BYTE	*key;
	int		keyLen;
	int		e;
	int		capa;

public:
	PubKey(void) { key = NULL; capa = 0; }
	~PubKey() { UnSet(); }

	void Set(BYTE *_key, int len, int _e, int _capa) {
		UnSet(); e = _e; capa = _capa; key = new BYTE [keyLen=len]; memcpy(key, _key, len);
	}
	void UnSet(void) {
		delete [] key; key = NULL; capa = 0;
	}
	const BYTE *Key(void) { return key; }
	int KeyLen(void) { return keyLen; }
	int Exponent(void) { return e; }
	BOOL KeyBlob(BYTE *blob, int maxLen, int *len) {
		if ((*len = KeyBlobLen()) > maxLen) return FALSE;
		/* PUBLICSTRUC */	blob[0] = PUBLICKEYBLOB; blob[1] = CUR_BLOB_VERSION;
					*(WORD *)(blob+2) = 0; *(ALG_ID *)(blob+4) = CALG_RSA_KEYX;
		/* RSAPUBKEY */		memcpy(blob+8, "RSA1", 4);
					*(DWORD *)(blob+12) = keyLen * 8; *(int *)(blob+16) = e;
		/* PUBKEY_DATA */	memcpy(blob+20, key, keyLen);
		return	TRUE;
	}
	int KeyBlobLen(void) { return keyLen + 8 + 12; /* PUBLICKEYSTRUC + RSAPUBKEY */ }
	void SetByBlob(BYTE *blob, int _capa) {
		UnSet();
		keyLen = *(int *)(blob+12) / 8;
		key = new BYTE [keyLen];
		memcpy(key, blob+20, keyLen);
		e = *(int *)(blob+16);
		capa = _capa;
	}
	int Capa(void) { return capa; }
};

struct HostSub {
	char	userName[MAX_NAMEBUF];
	char	hostName[MAX_NAMEBUF];
	ULONG	addr;
	int		portNo;
};

struct Host {
	HostSub	hostSub;
	char	nickName[MAX_NAMEBUF];
	char	groupName[MAX_NAMEBUF];
	ULONG	hostStatus;
	time_t	updateTime;
	int		priority;
	int		refCnt;
	PubKey	pubKey;
	int		cryptSpec;

	Host(void) { refCnt = 0; }
	~Host() { refCnt = 0; }
	int	RefCnt(int cnt=0) { return refCnt += cnt; }
};

class THosts {
public:
	enum Kind { NAME, ADDR, NAME_ADDR, MAX_ARRAY };
	BOOL enable[MAX_ARRAY];

protected:
	int		hostCnt;
	Host	**array[MAX_ARRAY];
	Host	*Search(Kind kind, HostSub *hostSub, int *insertIndex=NULL);
	int		Cmp(HostSub *hostSub1, HostSub *hostSub2, Kind kind);

public:
	THosts(void);
	~THosts();

	void	Enable(Kind kind, BOOL _enable) { enable[kind] = _enable; }
	BOOL	AddHost(Host *host);
	BOOL	DelHost(Host *host);
	int		HostCnt(void) { return hostCnt; }
	Host	*GetHost(int index, Kind kind=NAME) { return array[kind][index]; }
	Host	*GetHostByName(HostSub *hostSub) { return enable[NAME] ? Search(NAME, hostSub) : NULL; }
	Host	*GetHostByAddr(HostSub *hostSub) { return enable[ADDR] ? Search(ADDR, hostSub) : NULL; }
	Host	*GetHostByNameAddr(HostSub *hostSub) { return enable[NAME_ADDR] ? Search(NAME_ADDR, hostSub) : NULL; }
	int		PriorityHostCnt(int priority, int range=1);
};

struct AddrObj : public TListObj {
	ULONG	addr;
	int		portNo;
};

struct UrlObj : public TListObj {
	char	protocol[MAX_NAMEBUF];
	char	program[MAX_PATH];
};

ULONG ResolveAddr(const char *_host);

class TBroadcastObj : public TListObj {
	char	*host;
	ULONG	addr;
public:
	TBroadcastObj(const char *_host=NULL, ULONG _addr=0) { host=strdup(_host); addr=_addr; }
	~TBroadcastObj() { if (host) free(host); };
	const char *Host() { return	host; }
	ULONG	Addr(BOOL realTime=FALSE) { return realTime ? (addr = ResolveAddr(host)) : addr; }
};

class TBroadcastList : public TList {
public:
	TBroadcastList() {}
	~TBroadcastList() { Reset(); }
	void Reset(void);
//	BOOL SetHost(const char *host);
	void SetHostRaw(const char *host, ULONG addr=0) { TBroadcastObj *obj = new TBroadcastObj(host, addr); AddObj(obj); }
//	BOOL IsExistHost(const char *host);
	TBroadcastObj *Top() { return (TBroadcastObj *)TopObj(); }
	TBroadcastObj *Next(TBroadcastObj *obj) { return (TBroadcastObj *)NextObj(obj); }
};

enum SendWidth { SW_NICKNAME, SW_ABSENCE, SW_GROUP, SW_HOST, SW_IPADDR, SW_USER, SW_PRIORITY, MAX_SENDWIDTH };
inline BOOL GetItem(UINT columnItems, int sw) { return (columnItems & (1 << sw)) ? TRUE : FALSE; }
inline void SetItem(UINT *columnItems, int sw, BOOL on) { if (on) *columnItems |= (1 << sw); else *columnItems &= ~(1 << sw); }

#define CFG_ALL			0xffffffff
#define CFG_GENERAL		0x00000001
#define CFG_ABSENCE		0x00000002
#define CFG_WINSIZE		0x00000004
#define CFG_FONT		0x00000008
#define CFG_BROADCAST	0x00000010
#define CFG_CLICKURL	0x00000020
#define CFG_PRIORITY	0x00000040
#define CFG_FINDHIST	0x00000080
#define CFG_HOSTINFO	0x00001000
#define CFG_DELHOST		0x00002000
#define CFG_DELCHLDHOST	0x00004000
#define CFG_CRYPT		0x00008000

#define FT_STRICTDATE	0x00000001
#define RS_REALTIME		0x00000001

struct Cfg {
protected:
	BOOL	ReadFontRegistry(TRegistry *reg, char *key, LOGFONT *font);
	BOOL	WriteFontRegistry(TRegistry *reg, char *key, LOGFONT *font);

public:
	THosts	priorityHosts;
	THosts	fileHosts;
	int		PriorityMax;
	int		PriorityReject;
	char	**PrioritySound;

	ULONG	nicAddr;
	int		portNo;
	BOOL	NoPopupCheck;
	BOOL	OpenCheck;
	BOOL	NoErase;
	BOOL	NoBeep;
	BOOL	OneClickPopup;
	int		DelayTime;
	BOOL	LogCheck;
	char	LogFile[MAX_PATH];
	char	SoundFile[MAX_PATH];
	BOOL	AbsenceSave;
	BOOL	AbsenceCheck;
	int		AbsenceMax;
	int		AbsenceChoice;
	char	(*AbsenceStr)[MAX_PATH];
	char	(*AbsenceHead)[MAX_NAMEBUF];

	int		FindMax;
	char	(*FindStr)[MAX_NAMEBUF];
	BOOL	FindAll;

	HCRYPTPROV	hSmallCsp;
	PubKey		smallPubKey;
	HCRYPTKEY	hSmallPrivKey;

	HCRYPTPROV	hCsp;
	PubKey		pubKey;
	BYTE		*privBlob;
	int			privBlobLen;
	int			privEncryptType;
	BYTE		*privEncryptSeed;
	int			privEncryptSeedLen;
	HCRYPTKEY	hPrivKey;
	
	BOOL	QuoteCheck;
	BOOL	SecretCheck;
	BOOL	IPAddrCheck;
	BOOL	HotKeyCheck;
	int		HotKeyModify;
	int		HotKeySend;
	int		HotKeyRecv;
	int		HotKeyMisc;
	BOOL	ControlIME;
	int		GlidLineCheck;
	UINT	ColumnItems;
	BOOL	AllowSendList;
	int		fileTransOpt;
	int		ResolveOpt;
	int		EncryptNum;

	int		ViewMax;
	int		TransMax;
	int		TcpbufMax;

	int		NickNameCheck;
	char	NickNameStr[MAX_NAMEBUF];
	char	GroupNameStr[MAX_NAMEBUF];
	BOOL	AbnormalButton;
	BOOL	AbsenceNonPopup;
	BOOL	ListGet;
	UINT	ListGetMSec;
	UINT	RetryMSec;
	UINT	RetryMax;
	int		RecvMax;
	char	Lang[MAX_LANGBUF];
	char	IconFile[MAX_PATH];
	char	RevIconFile[MAX_PATH];
	char	lastSaveDir[MAX_PATH];
	char	lastOpenDir[MAX_PATH];
	ULONG	Sort;
	int		UpdateTime;
	int		KeepHostTime;
	BOOL	MsgMinimize;
	BOOL	DefaultUrl;
	BOOL	ShellExec;
	BOOL	ExtendEntry;
	char	QuoteStr[MAX_NAMEBUF];
	BOOL	Debug;

	int		SendWidth[MAX_SENDWIDTH];
	int		SendOrder[MAX_SENDWIDTH];

	int		SendXdiff;
	int		SendYdiff;
	int		SendMidYdiff;
	BOOL	SendSavePos;
	int		SendXpos;
	int		SendYpos;

	int		RecvXdiff;
	int		RecvYdiff;
	BOOL	RecvSavePos;
	int		RecvXpos;
	int		RecvYpos;

	LOGFONT	SendEditFont;
	LOGFONT	SendListFont;
	LOGFONT	RecvHeadFont;
	LOGFONT	RecvEditFont;

	char	PasswordStr[MAX_NAMEBUF];
	BOOL	PasswdLogCheck;
	TList	urlList;
	BOOL	DialUpCheck;
	TList	DialUpList;
	TBroadcastList	broadcastList;

	Cfg(ULONG _nicAddr, int _portNo);
	~Cfg();
	enum PART { FIND };

	BOOL	ReadRegistry(void);
	BOOL	WriteRegistry(int ctl_flg = CFG_ALL);
	void	GetRegName(char *buf, ULONG nic_addr, int port_no);
};

struct MsgBuf {
	HostSub	hostSub;
	int		version;
	int		portNo;
	ULONG	packetNo;
	ULONG	command;
	int		exOffset;			// expand message offset in msgBuf
	int		dummy;
	char	msgBuf[MAX_UDPBUF];

	// メモリコピー節約のため。
	void	Init(MsgBuf *org) {
		if (org == NULL) {
			memset(this, 0, (char *)&this->dummy - (char *)this);
			*msgBuf = 0;
			return;
		}
		memcpy(this, org, (char *)&this->dummy - (char *)this);
		strcpy(this->msgBuf, org->msgBuf);
		strcpy(this->msgBuf + exOffset, org->msgBuf + exOffset);
	}
};

struct RecvBuf {
	struct sockaddr_in	addr;
	int					addrSize;
	int					size;
	char				msgBuf[MAX_UDPBUF];
};

struct ConnectInfo : public TListObj {
	SOCKET	sd;
	ULONG	addr;
	int		port;
	BOOL	server;
	BOOL	complete;
	DWORD	startTick;
	DWORD	lastTick;
};

class MsgMng {
protected:
	SOCKET		udp_sd;
	SOCKET		tcp_sd;

	BOOL		status;
	ULONG		packetNo;
	Cfg			*cfg;

	HWND		hAsyncWnd;
	UINT		uAsyncMsg;
	UINT		lAsyncMode;
	HostSub		local;

	BOOL		WSockInit(BOOL recv_flg);
	void		WSockTerm(void);
	BOOL		WSockReset(void);

public:
	MsgMng(ULONG nicAddr, int portNo, Cfg *cfg=NULL);
	~MsgMng();

	BOOL	GetStatus(void)	{ return	status; }
	HostSub	*GetLocalHost(void) { return	&local; }
	void	CloseSocket(void);
	BOOL	IsAvailableTCP() { return tcp_sd != INVALID_SOCKET ? TRUE : FALSE; }

	BOOL	Send(HostSub *hostSub, ULONG command, int val);
	BOOL	Send(HostSub *hostSub, ULONG command, const char *message=NULL, const char *exMsg=NULL);
	BOOL	Send(ULONG host_addr, int port_no, ULONG command, const char *message=NULL, const char *exMsg=NULL);
	BOOL	AsyncSelectRegist(HWND hWnd);
	BOOL	Recv(MsgBuf *msg);
	BOOL	ResolveMsg(RecvBuf *buf, MsgBuf *msg);
	ULONG	MakePacketNo(void) { return packetNo++; }
	ULONG	MakeMsg(char *udp_msg, int packetNo, ULONG command, const char *msg, const char *exMsg=NULL, int *packet_len=NULL);
	ULONG	MakeMsg(char *udp_msg, ULONG command, const char *msg, const char *exMsg=NULL, int *packet_len=NULL) { return	MakeMsg(udp_msg, MakePacketNo(), command, msg, exMsg, packet_len); }
	BOOL	UdpSend(ULONG host_addr, int port_no, const char *udp_msg);
	BOOL	UdpSend(ULONG host_addr, int port_no, const char *udp_msg, int len);
	BOOL	UdpRecv(RecvBuf *buf);

	BOOL	Accept(HWND hWnd, ConnectInfo *info);
	BOOL	Connect(HWND hWnd, ConnectInfo *info);
	BOOL	AsyncSelectConnect(HWND hWnd, ConnectInfo *info);
	BOOL	ConnectDone(HWND hWnd, ConnectInfo *info);

	static int LocalNewLineToUnix(const char *src, char *dest, int maxlen);
	static int UnixNewLineToLocal(const char *src, char *dest, int maxlen);
};

class TAbsenceDlg : public TDlg {
protected:
	Cfg		*cfg;
	int		currentChoice;
	char	(*tmpAbsenceStr)[MAX_PATH];
	char	(*tmpAbsenceHead)[MAX_NAMEBUF];
	void	SetData(void);
	void	GetData(void);

public:
	TAbsenceDlg(Cfg *_cfg, TWin *_parent = NULL);
	virtual ~TAbsenceDlg();

	virtual BOOL	EvCreate(LPARAM lParam);
	virtual BOOL	EvNcDestroy(void);
	virtual BOOL	EvCommand(WORD wNotifyCode, WORD wID, LPARAM hwndCtl);
	virtual BOOL	EventUser(UINT uMsg, WPARAM wParam, LPARAM lParam);
};

class TSortDlg : public TDlg {
protected:
	static	TSortDlg *exclusiveWnd;
	Cfg		*cfg;
	void	SetData(void);
	void	GetData(void);

public:
	TSortDlg(Cfg *_cfg, TWin *_parent = NULL);

	virtual int		Exec(void);
	virtual BOOL	EvCommand(WORD wNotifyCode, WORD wID, LPARAM hwndCtl);
	virtual BOOL	EvCreate(LPARAM lParam);
};

class TUrlDlg : public TDlg {
protected:
	Cfg		*cfg;
	TList	tmpUrlList;
	char	currentProtocol[MAX_NAMEBUF];
	void	SetData(void);
	void	GetData(void);
	void	SetCurrentData(void);

public:
	TUrlDlg(Cfg *_cfg, TWin *_parent = NULL);
	virtual ~TUrlDlg();

	virtual BOOL	EvCreate(LPARAM lParam);
	virtual BOOL	EvNcDestroy(void);
	virtual BOOL	EvCommand(WORD wNotifyCode, WORD wID, LPARAM hwndCtl);
};

class TSendDlg;
class TFindDlg : public TDlg {
protected:
	Cfg		*cfg;
	TSendDlg	*sendDlg;

public:
	TFindDlg(Cfg *_cfg, TSendDlg *_parent);

	virtual BOOL	EvCommand(WORD wNotifyCode, WORD wID, LPARAM hwndCtl);
	virtual BOOL	EvCreate(LPARAM lParam);
};

enum SendStatus { ST_GETCRYPT=0, ST_MAKECRYPTMSG, ST_MAKEMSG, ST_SENDMSG, ST_DONE };
class SendEntry {
	Host		*host;
	SendStatus	status;
	UINT		command;
	char		*msg;
	int			msgLen;

public:
	SendEntry() { msg = NULL; host = NULL; }
	~SendEntry() { delete [] msg; if (host && host->RefCnt(-1) == 0) delete host; }
	void SetMsg(char *_msg, int len) { msg = new char[msgLen=len]; memcpy(msg, _msg, len); }
	const char *Msg(void) { return msg; }
	int MsgLen() { return msgLen; }
	void SetHost(Host *_host) { (host = _host)->RefCnt(1); }
	Host *Host(void) { return host; }
	void SetStatus(SendStatus _status) { status = _status; }
	SendStatus Status(void) { return status; }
	void SetCommand(UINT _command) { command = _command ; }
	UINT Command(void) { return command; }
};

class TEditSub : public TSubClassCtl {
protected:
	Cfg			*cfg;

public:
	TEditSub(Cfg *_cfg, TWin *_parent);

	virtual BOOL	EventButton(UINT uMsg, int nHitTest, POINTS pos);
	virtual BOOL	EventUser(UINT uMsg, WPARAM wParam, LPARAM lParam);
};

class TSeparateSub : public TSubClassCtl {
protected:
	HCURSOR	hCursor;

public:
	TSeparateSub(TWin *_parent);

	virtual BOOL	EvSetCursor(HWND cursorWnd, WORD nHitTest, WORD wMouseMsg);
	virtual BOOL	EvNcHitTest(POINTS pos, LRESULT *result);
	virtual BOOL	EventButton(UINT uMsg, int nHitTest, POINTS pos);
};

class TListViewEx : public TSubClassCtl {
protected:
	int		focus_index;

public:
	TListViewEx(TWin *_parent);

	int		GetFocusIndex(void) { return focus_index; }
	void	SetFocusIndex(int index) { focus_index = index; }

	virtual BOOL	EventFocus(UINT uMsg, HWND hFocusWnd);
	virtual BOOL	EventButton(UINT uMsg, int nHitTest, POINTS pos);
	virtual BOOL	EventUser(UINT uMsg, WPARAM wParam, LPARAM lParam);
};

class TListDlg : public TDlg, public TListObj {
public:
	TListDlg(LPSTR _name, TWin *_parent = NULL) : TDlg(_name, _parent) {}
	TListDlg(UINT	resid, TWin *_parent = NULL) : TDlg(resid, _parent) {}
};

class FileInfo : public TListObj {
	int			id;		// Win 版では、単なるシーケンシャルNOをセット
	char		*fname;
	const char	*fname_ext;	// for recv dir thread
	UINT		attr;
	_int64		size;
	time_t		mtime;
	time_t		atime;
	time_t		crtime;
	BOOL		isSelected;		// for recvdlg

public:
	FileInfo(int _id=0) { id=_id; fname_ext=fname=NULL; size=0; mtime=0; }
	FileInfo(const FileInfo& org) { fname=NULL; *this = org; }
	~FileInfo() { if (fname) free(fname); }

	int Id() { return id; }
	void SetId(int _id) { id = _id; }
	const char *Fname() { return fname ? fname : fname_ext; }
	void SetFname(const char *_fname) { if (fname) free(fname); fname = (char *)strdup(_fname); }
	void SetFnameExt(const char *_fname) { fname_ext = _fname; }
	_int64 Size() { return size; }
	void SetSize(_int64 _size) { size = _size; }
	time_t Mtime() { return mtime; }
	void SetMtime(time_t _mtime) { mtime = _mtime; }
	time_t Atime() { return atime; }
	void SetAtime(time_t _atime) { atime = _atime; }
	time_t Crtime() { return crtime; }
	void SetCrtime(time_t _crtime) { crtime = _crtime; }
	UINT Attr() { return attr; }
	void SetAttr(UINT _attr) { attr = _attr; }
	BOOL IsSelected() { return isSelected; }
	void SetSelected(BOOL _isSelected) { isSelected = _isSelected; }
	FileInfo& operator =(const FileInfo& org) { id=org.id; SetFname(org.fname); attr=org.attr; size=org.size; mtime=org.mtime; atime=org.atime; crtime=org.crtime; isSelected=org.isSelected; return *this; }
};

struct ShareInfo : public TListObj {	// いずれ class に（暫定移行中）
	int			packetNo;		// not use recvdlg
	Host		**host;			// allow host list, not use recvdlg
	int			hostCnt;		// not use recvdlg
	char		*transStat;		// not use recvdlg
	FileInfo	**fileInfo;		// allow file list
	int			fileCnt;
	FILETIME	attachTime;

public:
	ShareInfo(int packetNo=0);
	void LinkList(ShareInfo *top);
};

BOOL EncodeShareMsg(ShareInfo *info, char *buf, int bufsize);
ShareInfo *DecodeShareMsg(char *buf);
BOOL FreeDecodeShareMsg(ShareInfo *info);
BOOL FreeDecodeShareMsgFile(ShareInfo *info, int index);
BOOL SetFileButton(TDlg *dlg, int buttonID, ShareInfo *info);


struct AcceptFileInfo {
	FileInfo	*fileInfo;
	Host		*host;
	_int64		offset;
	int			packetNo;
	UINT		command;
	FILETIME	attachTime;
};

struct ShareCntInfo {
	int		hostCnt;
	int		fileCnt;
	int		dirCnt;
	int		transferCnt;
	int		doneCnt;
	int		packetCnt;
	_int64	totalSize;
};

class TShareStatDlg;
class ShareMng {
public:
	enum			{ TRANS_INIT, TRANS_BUSY, TRANS_DONE };
protected:
	TListObj		_top;	// 番兵用
	ShareInfo		*top;
	TShareStatDlg	*statDlg;
	Cfg				*cfg;
	BOOL			SetFileInfo(char *fname, FileInfo *info);
public:
	ShareMng(Cfg *_cfg);
	~ShareMng();
	ShareInfo *CreateShare(int packetNo);
	void	DestroyShare(ShareInfo *info);
	BOOL	AddFileShare(ShareInfo *info, char *fname);
	BOOL	DelFileShare(ShareInfo *info, int fileNo);
	BOOL	EndHostShare(int packetNo, HostSub *hostSub, FileInfo *fileInfo=NULL, BOOL done=TRUE);
	BOOL	AddHostShare(ShareInfo *info, SendEntry *entry, int entryNum);
	ShareInfo	*Top(void) { return top->next != top ? (ShareInfo *)top->next : NULL; }
	ShareInfo	*Next(ShareInfo *info) { return info->next != top ? (ShareInfo *)info->next : NULL; }
	ShareInfo	*Search(int packetNo);
	BOOL	GetShareCntInfo(ShareCntInfo *info, ShareInfo *shareInfo=NULL);
	BOOL	GetAcceptableFileInfo(ConnectInfo *info, char *buf, AcceptFileInfo *fileInfo);
	void	RegistShareStatDlg(TShareStatDlg *_dlg) { statDlg = _dlg; }
	static int GetFileInfoNo(ShareInfo *info, FileInfo *fileInfo);
};

class TShareDlg : public TDlg {
	ShareMng	*shareMng;
	ShareInfo	*shareInfo;
	Cfg			*cfg;
	char		lvBuf[MAX_PATH];
	BOOL		AddList(int cnt);
	BOOL		DelList(int cnt);
	TListViewEx	shareListView;

public:
	TShareDlg(ShareMng *_shareMng, ShareInfo *_shareInfo, Cfg *_cfg, TWin *_parent = NULL);
	~TShareDlg();
//	virtual int		Exec(void);
	virtual BOOL	EvCommand(WORD wNotifyCode, WORD wID, LPARAM hwndCtl);
	virtual BOOL	EvCreate(LPARAM lParam);
	virtual BOOL	EvDropFiles(HDROP hDrop);
	virtual BOOL	EvNotify(UINT ctlID, NMHDR *pNmHdr);
static BOOL FileAddDlg(TDlg *dlg, ShareMng *sharMng, ShareInfo *shareInfo, Cfg *cfg);
};

class TSaveCommonDlg : public TDlg {
protected:
	TWin		*parentWin;
	ShareInfo	*shareInfo;
	Cfg			*cfg;
	int			offset;
	BOOL		SetInfo(void);

public:
	TSaveCommonDlg(ShareInfo *_shareInfo, Cfg *_cfg, TWin *_parentWin);
	virtual int		Exec(void);
	virtual BOOL	EvCreate(LPARAM lParam);
	virtual BOOL	EvCommand(WORD wNotifyCode, WORD wID, LPARAM hwndCtl);
	virtual BOOL	EventUser(UINT uMsg, WPARAM wParam, LPARAM lParam);
	virtual void	EndDialog(int) {}	// 無視させる
};

class LogMng {
protected:
	Cfg		*cfg;
	BOOL	Write(LPCSTR str);

public:
	LogMng(Cfg *_cfg);

	BOOL	WriteSendStart(void);
	BOOL	WriteSendHead(LPCSTR head);
	BOOL	WriteSendMsg(LPCSTR msg, ULONG command, ShareInfo *shareInfo=NULL);
	BOOL	WriteRecvMsg(MsgBuf *msg, THosts *hosts, ShareInfo *shareInfo=NULL);
	BOOL	WriteStart(void);
	BOOL	WriteMsg(LPCSTR msg, ULONG command, ShareInfo *shareInfo=NULL);

static void StrictLogFile(char *path);
};

class TSendDlg : public TListDlg {
protected:
	MsgMng		*msgMng;
	Cfg			*cfg;
	LogMng		*logmng;
	HWND		hRecvWnd;
	MsgBuf		msg;
	ShareMng	*shareMng;
	ShareInfo	*shareInfo;

	THosts		*hosts;
	TFindDlg	*findDlg;
	Host		**hostArray;
	int			memberCnt;

	SendEntry	*sendEntry;
	int			sendEntryNum;
	UINT		command;
	char		*shareStr;
	char		msgBuf[MAX_UDPBUF];
	char		selectGroup[MAX_NAMEBUF];

	ULONG		packetNo;
	int			packetLen;
	UINT		timerID;
	UINT		retryCnt;

// display
	HFONT		hListFont;
	HFONT		hEditFont;
	LOGFONT		orgFont;

	RECT		orgRect;
	enum		send_item { host_item=0, member_item, refresh_item, edit_item, ok_item, secret_item, passwd_item, separate_item, file_item, max_senditem };
	WINPOS		item[max_senditem];

	int			currentMidYdiff;
	int			dividYPos;
	int			lastYPos;
	BOOL		captureMode;
	BOOL		listOperateCnt;
	BOOL		hiddenDisp;

	int			maxItems;
	UINT		ColumnItems;
	int			FullOrder[MAX_SENDWIDTH];
	int			items[MAX_SENDWIDTH];
	BOOL		lvStateEnable;
	int			sortItem;
	BOOL		sortRev;

	TEditSub		editSub;
	TListViewEx		hostListView;
	TSeparateSub	separateSub;

	void	SetFont(void);
	void	SetSize(void);
	void	SetMainMenu(HMENU hMenu);
	void	PopupContextMenu(POINTS pos);
	void	GetOrder(void);

	void	SetQuoteStr(LPSTR str, LPCSTR quoteStr);
	void	SelectHost(HostSub *hostSub, BOOL force=FALSE);
	void	DisplayMemberCnt(void);
	UINT	GetInsertIndexPoint(Host *host);
	int		CompareHosts(Host *host1, Host *host2);
	int		GroupCompare(Host *host1, Host *host2);
	int		SubCompare(Host *host1, Host *host2);
	BOOL	SendMsg(void);
	BOOL	SendMsgSub(void);
	BOOL	MakeEncryptPacket(SendEntry *entry);
	BOOL	MakeEncryptMsg(Host *host, char *msgstr, char *buf);
	BOOL	IsSendFinish(void);
	void	InitializeHeader(void);
	char	*GetListItemStr(Host *host, int item);
	void	MakeEncryptMsg(Host *host, char *msgbuf);

public:
	TSendDlg(MsgMng *_msgmng, ShareMng *_shareMng, THosts *_hosts, Cfg *cfg, LogMng *logmng, HWND _hRecvWnd = NULL, MsgBuf *msg = NULL);
	virtual ~TSendDlg();

	HWND	GetRecvWnd(void) { return	hRecvWnd; }
	void	AddHost(Host *host);
	void	ModifyHost(Host *host);
	void	DelHost(Host *host);
	void	DelAllHost(void);
	BOOL	IsSending(void);
	BOOL	SendFinishNotify(HostSub *hostSub, ULONG packet_no);
	BOOL	SendPubKeyNotify(HostSub *hostSub, BYTE *pubkey, int len, int e, int capa);
	BOOL	FindHost(char *findStr, BOOL isAllfind);

	virtual BOOL	EvCreate(LPARAM lParam);
	virtual BOOL	EvCommand(WORD wNotifyCode, WORD wID, LPARAM hwndCtl);
	virtual BOOL	EvSysCommand(WPARAM uCmdType, POINTS pos);
	virtual BOOL	EvSize(UINT fwSizeType, WORD nWidth, WORD nHeight);
	virtual BOOL	EvTimer(WPARAM timerID, TIMERPROC proc);
	virtual BOOL	EvGetMinMaxInfo(MINMAXINFO *info);
	virtual BOOL	EvMouseMove(UINT fwKeys, POINTS pos);
	virtual BOOL	EvContextMenu(HWND childWnd, POINTS pos);
	virtual BOOL	EvMeasureItem(UINT ctlID, MEASUREITEMSTRUCT *lpMis);
	virtual BOOL	EvDrawItem(UINT ctlID, DRAWITEMSTRUCT *lpDis);

	virtual BOOL	EvMenuSelect(UINT uItem, UINT fuFlag, HMENU hMenu);
	virtual BOOL	EvNotify(UINT ctlID, NMHDR *pNmHdr);
	virtual BOOL	EvDropFiles(HDROP hDrop);
	virtual BOOL	EventButton(UINT uMsg, int nHitTest, POINTS pos);
	virtual BOOL	EventInitMenu(UINT uMsg, HMENU hMenu, UINT uPos, BOOL fSystemMenu);
	virtual BOOL	EventCtlColor(UINT uMsg, HDC hDcCtl, HWND hWndCtl, HBRUSH *result);
	virtual BOOL	EventUser(UINT uMsg, WPARAM wParam, LPARAM lParam);
	virtual void	Show(int mode = SW_SHOWDEFAULT);
};

enum FileStatus { FS_DIRFILESTART, FS_OPENINFO, FS_FIRSTINFO, FS_NEXTINFO, FS_MAKEINFO, FS_TRANSINFO, FS_TRANSFILE, FS_ENDFILE, FS_COMPLETE, FS_ERROR };

struct RecvFileObj {
	ConnectInfo	*conInfo;
	FileInfo	*fileInfo;

	BOOL		isDir;
	FileInfo	curFileInfo;

	_int64		offset;
	_int64		woffset;
	char		*recvBuf;
	HANDLE		hFile;
	HANDLE		hThread;

	int			infoLen;
	int			dirCnt;

	_int64		totalTrans;
	DWORD		startTick;
	DWORD		lastTick;
	int			totalFiles;
	FileStatus	status;
	char		saveDir[MAX_PATH];
	char		path[MAX_PATH];
	char		info[MAX_BUF];	// for dirinfo buffer
};

class TRecvDlg : public TListDlg {
protected:
	static int	createCnt;
	MsgMng		*msgMng;
	MsgBuf		msg;
	THosts		*hosts;
	char		head[MAX_LISTBUF];
	Cfg			*cfg;
	LogMng		*logmng;
	BOOL		openFlg;
	SYSTEMTIME	recvTime;
	HFONT		hHeadFont;
	HFONT		hEditFont;
	LOGFONT		orgFont;
	TEditSub	editSub;
	BOOL		status;

	RecvFileObj	*fileObj;
	ShareInfo	*shareInfo;

	UINT		timerID;
	UINT		retryCnt;
	char		readMsgBuf[MAX_LISTBUF];
	ULONG		packetNo;
	int			cryptCapa;

	RECT	orgRect;
	enum	recv_item { title_item=0, head_item, head2_item, open_item, edit_item, ok_item, cancel_item, quote_item, file_item, max_recvitem };
	WINPOS	item[max_recvitem];

	void	SetFont(void);
	void	SetSize(void);
	void	SetMainMenu(HMENU hMenu);
	void	PopupContextMenu(POINTS pos);
	BOOL	TcpEvent(SOCKET sd, LPARAM lParam);
	BOOL	StartRecvFile(void);
	BOOL	ConnectRecvFile(void);
static DWORD WINAPI RecvFileThread(void *_recvDlg);
	BOOL	SaveFile(void);
	BOOL	OpenRecvFile(void);
	BOOL	RecvFile(void);
	BOOL	RecvDirFile(void);
	BOOL	CloseRecvFile(BOOL setAttr=FALSE);
	BOOL	EndRecvFile(BOOL manual_suspend=FALSE);
	void	SetTransferButtonText(void);
	BOOL	DecryptMsg(void);

public:
	TRecvDlg(MsgMng *_msgmng, MsgBuf *_msg, THosts *hosts, Cfg *cfg, LogMng *logmng);
	virtual ~TRecvDlg();

	virtual BOOL	IsClosable(void) { return openFlg && (fileObj == NULL || fileObj->conInfo == NULL); }
	virtual BOOL	IsSamePacket(MsgBuf *test_msg);
	virtual BOOL	SendFinishNotify(HostSub *hostSub, ULONG packet_no);

	virtual BOOL	EvCreate(LPARAM lParam);
	virtual BOOL	EvCommand(WORD wNotifyCode, WORD wID, LPARAM hwndCtl);
	virtual BOOL	EvSysCommand(WPARAM uCmdType, POINTS pos);
	virtual BOOL	EvSize(UINT fwSizeType, WORD nWidth, WORD nHeight);
	virtual BOOL	EvGetMinMaxInfo(MINMAXINFO *info);
	virtual BOOL	EvTimer(WPARAM timerID, TIMERPROC proc);
	virtual BOOL	EvContextMenu(HWND childWnd, POINTS pos);
	virtual BOOL	EventButton(UINT uMsg, int nHitTest, POINTS pos);
	virtual BOOL	EventUser(UINT uMsg, WPARAM wParam, LPARAM lParam);
	virtual BOOL	EventInitMenu(UINT uMsg, HMENU hMenu, UINT uPos, BOOL fSystemMenu);
	virtual BOOL	EventCtlColor(UINT uMsg, HDC hDcCtl, HWND hWndCtl, HBRUSH *result);
	virtual void	Show(int mode = SW_NORMAL);
	BOOL			Status() { return status; }
	static int		GetCreateCnt(void) { return createCnt; }
};

class TSetupDlg : public TDlg {
protected:
	Cfg		*cfg;
	THosts	*hosts;

	void	SetData(void);
	void	GetData(void);

public:
	TSetupDlg(Cfg *cfg, THosts *hosts, TWin *_parent = NULL);
	virtual ~TSetupDlg();

	virtual BOOL	EvCommand(WORD wNotifyCode, WORD wID, LPARAM hwndCtl);
	virtual BOOL	EvCreate(LPARAM lParam);
};

class TAboutDlg : public TDlg {
public:
	TAboutDlg(TWin *_parent = NULL);
	virtual BOOL	EvCommand(WORD wNotifyCode, WORD wID, LPARAM hwndCtl);
	virtual BOOL	EvCreate(LPARAM lParam);
};

class TMsgDlg : public TListDlg {
protected:
	static int	createCnt;
	int			showMode;

public:
	TMsgDlg(TWin *_parent = NULL);
	virtual ~TMsgDlg();

	virtual BOOL	Create(char *text, char *title, int _showMode = SW_SHOW);
	virtual BOOL	EvCreate(LPARAM lParam);
	virtual BOOL	EvCommand(WORD wNotifyCode, WORD wID, LPARAM hWndCtl);
	static int		GetCreateCnt(void) { return createCnt; }
};

class TLogDlg : public TDlg {
protected:
	Cfg		*cfg;
	void	SetData(void);
	void	GetData(void);

public:
	TLogDlg(Cfg *_cfg, TWin *_parent = NULL);

	BOOL	OpenSoundFileDlg(void);

	virtual BOOL	EvCommand(WORD wNotifyCode, WORD wID, LPARAM hwndCtl);
	virtual BOOL	EvCreate(LPARAM lParam);
};

class TPasswordDlg : public TDlg {
protected:
	Cfg		*cfg;
	char	*outbuf;
public:
	TPasswordDlg(Cfg *_cfg, TWin *_parent = NULL);
	TPasswordDlg(char *_outbuf, TWin *_parent = NULL);
	virtual BOOL	EvCommand(WORD wNotifyCode, WORD wID, LPARAM hwndCtl);
	virtual BOOL	EvCreate(LPARAM lParam);
};

class TPasswdChangeDlg : public TDlg {
protected:
	Cfg		*cfg;
public:
	TPasswdChangeDlg(Cfg *_cfg, TWin *_parent = NULL);
	virtual BOOL	EvCommand(WORD wNotifyCode, WORD wID, LPARAM hwndCtl);
	virtual BOOL	EvCreate(LPARAM lParam);
};

class TShareStatDlg : public TDlg {
	ShareMng	*shareMng;
	Cfg			*cfg;
	char		lvBuf[MAX_PATH];
	BOOL		SetAllList(void);
	BOOL		DelList(int cnt);
	TListViewEx	shareListView;

public:
	TShareStatDlg(ShareMng *_shareMng, Cfg *_cfg, TWin *_parent = NULL);
	~TShareStatDlg();
//	virtual int		Exec(void);
	virtual BOOL	Refresh(void);
	virtual BOOL	EvCommand(WORD wNotifyCode, WORD wID, LPARAM hwndCtl);
	virtual BOOL	EvCreate(LPARAM lParam);
	virtual BOOL	EvNotify(UINT ctlID, NMHDR *pNmHdr);
};

struct SendFileObj : public TListObj {
	ConnectInfo	*conInfo;
	FileInfo	*fileInfo;
	FILETIME	attachTime;

	UINT		command;
	BOOL		isDir;
	_int64		fileSize;
	_int64		offset;
	char		*mapAddr;
	HANDLE		hFile;
	HANDLE		hMap;
	HANDLE		hThread;

	Host		*host;
	int			packetNo;
	FileStatus	status;

	int			dirCnt;
	HANDLE		*hDir;	// FindFirst handle
	char		path[MAX_PATH];

	int			headerLen;
	int			headerOffset;
	char		header[MAX_BUF];	// for dirinfo
	WIN32_FIND_DATA	fdata;
};

class TMainWin : public TWin {
protected:
	static HICON	hMainIcon;
	static HICON	hRevIcon;
	static TMainWin	*mainWin;	// for thread proc

	TList			sendList;
	TList			recvList;
	TList			msgList;
	TList			sendFileList;
	TList			connectList;
	THosts			hosts;

	TSetupDlg		*setupDlg;
	TAboutDlg		*aboutDlg;
	TShareStatDlg	*shareStatDlg;
	TAbsenceDlg		*absenceDlg;
	MsgMng			*msgMng;
	LogMng			*logmng;
	ShareMng		*shareMng;
	Cfg				*cfg;
	TRecycleList	*ansList;

	int			portNo;
	int			memberCnt;
	time_t		refreshStartTime;
	time_t		entryStartTime;
	UINT		entryTimerStatus;
	UINT		reverseTimerStatus;
	UINT		ansTimerID;
	UINT		TaskBarCreateMsg;
	BOOL		terminateFlg;
	BOOL		activeToggle;

#define MAX_PACKETLOG	16
	struct {
		ULONG		no;
		ULONG		addr;
		int			port;
	} packetLog[MAX_PACKETLOG];
	int		packetLogCnt;

	BOOL	IsLastPacket(MsgBuf *msg);
	void	SetIcon(HICON hSetIcon);
	void	ReverseIcon(BOOL startFlg);

	void	EntryHost(void);
	void	ExitHost(void);
	void	Popup(UINT resId);
	BOOL	PopupCheck(void);
	BOOL	AddAbsenceMenu(HMENU hMenu, int insertIndex);
	void	ActiveChildWindow(BOOL hide=FALSE);
	BOOL	TaskBar(int nimMode, HICON hSetIcon = NULL, LPCSTR tip = NULL);
	BOOL	UdpEvent(LPARAM lParam);
	BOOL	TcpEvent(SOCKET sd, LPARAM lParam);

	BOOL	CheckConnectInfo(ConnectInfo *conInfo);
	inline	SendFileObj *FindSendFileObj(SOCKET sd);
	BOOL	StartSendFile(SOCKET sd);
	BOOL	OpenSendFile(const char *fname, SendFileObj *obj);
static DWORD WINAPI SendFileThread(void *_sendFileObj);
	BOOL	SendFile(SendFileObj *obj);
	BOOL	SendDirFile(SendFileObj *obj);
	BOOL	CloseSendFile(SendFileObj *obj);
	BOOL	EndSendFile(SendFileObj *obj);

	void	BroadcastEntry(ULONG mode);
	void	BroadcastEntrySub(ULONG addr, int portNo, ULONG mode);
	void	BroadcastEntrySub(HostSub *hostSub, ULONG mode);
	void	Terminate(void);

	BOOL	SendDlgOpen(HWND hRecvWnd = NULL, MsgBuf *msg = NULL);
	void	SendDlgHide(TSendDlg *sendDlg);
	void	SendDlgExit(TSendDlg *sendDlg);
	BOOL	RecvDlgOpen(MsgBuf *msg);
	void	RecvDlgExit(TRecvDlg *recvDlg);
	void	MsgDlgExit(TMsgDlg *msgDlg);
	void	MiscDlgOpen(TDlg *dlg);
	void	LogOpen(void);

	void	AddHost(HostSub *hostSub, ULONG command, char *nickName="", char *groupName="");
	inline	void SetHostData(Host *destHost, HostSub *hostSub, ULONG command, time_t now_time, char *nickName="", char *groupName="", int priority=DEFAULT_PRIORITY);
	void	DelAllHost(void);
	void	DelHost(HostSub *hostSub);
	void	DelHostSub(Host *host);
	void	RefreshHost(BOOL unRemove);
	void	SetCaption(void);
	void	SendHostList(MsgBuf *msg);
	void	AddHostList(MsgBuf *msg);
	ULONG	HostStatus(void);
	void	ActiveListDlg(TList *dlgList, BOOL active = TRUE);
	void	DeleteListDlg(TList *dlgList);
	void	ActiveDlg(TDlg *dlg, BOOL active = TRUE);
	char	*GetNickNameEx(void);
	void	InitIcon(void);
	void	ControlIME(TWin *win, BOOL on);
	BOOL	SetAnswerQueue(AddrObj *obj);
	void	ExecuteAnsQueue(void);
	BOOL	SetupCryptAPI(void);
// ctl_flg
#define		KEY_REBUILD		0x0001
#define		KEY_DIAG		0x0002
	BOOL	SetupCryptAPICore(int ctl_flg = 0);
	BOOL	SetupRSAKey(int bitLen, int ctl_flg = 0);
	BOOL	LoadPrivBlob(BYTE *rawBlob, int *rawBlobLen);
	BOOL	StorePrivBlob(BYTE *rawBlob, int rawBlobLen);

	void	MsgBrEntry(MsgBuf *msg);
	void	MsgBrExit(MsgBuf *msg);
	void	MsgAnsEntry(MsgBuf *msg);
	void	MsgBrAbsence(MsgBuf *msg);
	void	MsgSendMsg(MsgBuf *msg);
	void	MsgRecvMsg(MsgBuf *msg);
	void	MsgReadMsg(MsgBuf *msg);
	void	MsgBrIsGetList(MsgBuf *msg);
	void	MsgOkGetList(MsgBuf *msg);
	void	MsgGetList(MsgBuf *msg);
	void	MsgAnsList(MsgBuf *msg);
	void	MsgGetInfo(MsgBuf *msg);
	void	MsgSendInfo(MsgBuf *msg);
	void	MsgGetPubKey(MsgBuf *msg);
	void	MsgAnsPubKey(MsgBuf *msg);
	void	MsgGetAbsenceInfo(MsgBuf *msg);
	void	MsgSendAbsenceInfo(MsgBuf *msg);
	void	MsgReleaseFiles(MsgBuf *msg);
	void	MsgInfoSub(MsgBuf *msg);

public:
	TMainWin(ULONG _nicAddr=INADDR_ANY, int _portNo=IPMSG_DEFAULT_PORT, TWin *_parent = NULL);
	virtual ~TMainWin();

	virtual BOOL	EvCreate(LPARAM lParam);
	virtual BOOL	EvClose(void);
	virtual BOOL	EvCommand(WORD wNotifyCode, WORD wID, LPARAM hwndCtl);
	virtual BOOL	EvSysCommand(WPARAM uCmdType, POINTS pos);
	virtual BOOL	EvTimer(WPARAM timerID, TIMERPROC proc);
	virtual BOOL	EvEndSession(BOOL nSession, BOOL nLogOut);
	virtual BOOL	EvQueryOpen(void);
	virtual BOOL	EvHotKey(int hotKey);
	virtual BOOL	EventButton(UINT uMsg, int nHitTest, POINTS pos);
	virtual BOOL	EventInitMenu(UINT uMsg, HMENU hMenu, UINT uPos, BOOL fSystemMenu);
	virtual BOOL	EventUser(UINT uMsg, WPARAM wParam, LPARAM lParam);

	static	HICON	GetIPMsgIcon(void);
};

class TMsgApp : public TApp {
public:
	TMsgApp(HINSTANCE _hI, LPSTR _cmdLine, int _nCmdShow);
	virtual ~TMsgApp();

	virtual void	InitWindow(void);
};


class OpenFileDlg {
public:
	enum			Mode { OPEN, MULTI_OPEN, SAVE, NODEREF_SAVE };

protected:
	TWin			*parent;
	LPOFNHOOKPROC	hook;
	Mode			mode;

public:
	OpenFileDlg(TWin *_parent, Mode _mode=OPEN, LPOFNHOOKPROC _hook=NULL) { parent = _parent; hook = _hook; mode = _mode; }
	BOOL Exec(char *target, char *title=NULL, char *filter=NULL, char *defaultDir=NULL);
	BOOL Exec(UINT editCtl, char *title=NULL, char *filter=NULL, char *defaultDir=NULL);
};

char *strncpyz(char *dest, const char *src, int num);
int strncmpi(const char *str1, const char *str2, int num);
BOOL CheckPassword(const char *cfgPasswd, const char *inputPasswd);
void MakePassword(const char *inputPasswd, char *outputPasswd);

#define MSS_SPACE	0x00000001
#define MSS_NOPOINT	0x00000002
int MakeSizeString(char *buf, _int64 size, int flg=0);

UrlObj *SearchUrlObj(TList *list, char *protocol);
void SetDlgIcon(HWND hWnd);
void MakeListString(Cfg *cfg, HostSub *hostSub, THosts *hosts, char *buf);
void MakeListString(Cfg *cfg, Host *host, char *buf);
HWND GetMainWnd(void);
char *GetVerionStr(void);
BOOL SetImeOpenStatus(HWND hWnd, BOOL flg);
BOOL GetImeOpenStatus(HWND hWnd);
BOOL SetHotKey(Cfg *cfg);
BOOL IsSameHost(HostSub *host, HostSub *host2);
inline BOOL IsSameHostEx(HostSub *host, HostSub *host2) { return IsSameHost(host, host2) && host->addr == host2->addr && host->portNo == host2->portNo ? TRUE : FALSE; }
void RectToWinPos(const RECT *rect, WINPOS *wpos);
time_t Time(void);
char *Ctime(SYSTEMTIME *st=NULL);
BOOL GetFileInfomation(const char *path, WIN32_FIND_DATA *fdata);
BOOL BrowseDirDlg(TWin *parentWin, const char *title, const char *defaultDir, char *buf);
BOOL PathToDir(const char *org_path, char *target_dir);
BOOL PathToFname(const char *org_path, char *target_fname);
void ForcePathToFname(const char *org_path, char *target_fname);
void ConvertShareMsgEscape(char *str);
BOOL IsSafePath(const char *fullpath, const char *fname);
BOOL GetLastErrorMsg(char *msg=NULL, TWin *win=NULL);
BOOL GetSockErrorMsg(char *msg=NULL, TWin *win=NULL);
int MakePath(char *dest, const char *dir, const char *file);

_int64 hex2ll(char *buf);
void bin2hexstr(const BYTE *bin, int len, char *buf);
void bin2hexstr_bigendian(const BYTE *bin, int len, char *buf);
BOOL hexstr2bin(const char *buf, BYTE *bindata, int maxlen, int *len);
BOOL hexstr2bin_bigendian(const char *buf, BYTE *bindata, int maxlen, int *len);
char *separate_token(char *buf, char separetor, char **handle=NULL);
void MakeHash(const BYTE *data, int len, char *hashStr);
BOOL VerifyHash(const BYTE *data, int len, const char *orgHash);

// 1601年1月1日から1970年1月1日までの通算100ナノ秒
#define UNIXTIME_BASE	((_int64)0x019db1ded53e8000)

inline time_t FileTime2UnixTime(FILETIME *ft) {
	return	(time_t)((*(_int64 *)ft - UNIXTIME_BASE) / 10000000);
}
inline void UnixTime2FileTime(time_t ut, FILETIME *ft) {
	*(_int64 *)ft = (_int64)ut * 10000000 + UNIXTIME_BASE;
}

#endif
