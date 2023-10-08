#define crea 'Rhil'
#include "Hi-Launcher.strings.h"

#define HEDENOTIFYPRIORITY 16
#define MENUVERSION 3

//define STOPREASON
//define STOPREASONMORE

/***************************************/

void HwrDisplayAttributes (Boolean set, UInt8 kind, void* val)
SYS_TRAP(sysTrapHwrDisplayAttributes);

/***************************************/

SysAppInfoPtr SysGetAppInfo (SysAppInfoPtr *rootAppPP, SysAppInfoPtr *actionCodeAppPP) SYS_TRAP(sysTrapSysGetAppInfo);

#define GOI(_id) (FrmGetObjectIndex (RUN.CFP, _id))

/***************************************/

typedef enum {
_EshowKeyboard = sysEventFirstUserEvent,
} TEvents;

typedef enum {
_AafterBoot,
_Alasthotsync
} TAlarms;

typedef enum {
//menu trigger:
_Fb= 0,
_Fs,
_Fhold,
_Fe1,
_Fe2,
//recent app trigger:
_Frb,
_Frs,
_Frhold,
_Fre1,
_Fre2,
//other:
_Fpd,
_Fon,
_Ftrig,
_Frun,
_Fvis,
_Fkyo,
_Fid,
_Fcard,
_Fbeep,
_Ftrial1,
_Ftrial2_old,
_Ftrial3,
_Fset,
_Fupdate,
_Frefresh,
_Fprogres,
_Ftrial2,
_Fdisabled,
//menu trigger prefs:
_Fkc,
_Fbases,
_Funused1,
_Funused2,
_Funused3,
//recent app trigger prefs:
_Frkc,
_Frbases,
_Freserved1,
_Freserved2,
_Freserved3,
//other:
_Fignore1,
_Fignore5way,
_Fcardlaunch,
_Fdelay,
_Fpass,
_Fhandle,
_Ftreo,
_Fpostpone,
_Furl,
_Fdacreator,
_Fnorepeat,
_Fdial,
_Fdial_name,
_Fsametriggers,
_Fcode,
_Fnoignore,
_FignoreCenterUp,
_FlastCreator,
_FignoreLocalDB, //DO NOT MOVE!!!! - Bike Or Design crashed on exit
} Tfeatures;

typedef enum {
_Pb= 0,				//	trigger: hard button
_Ps,				//	trigger: silk icon
_Pe1x,				//	trigger: stroke: start: x
_Pe1y,				//	trigger: stroke: start: y
_Pe2x,				//	trigger: stroke: end: x
_Pe2y,                          //	trigger: stroke: end: y
_Phold,				//	trigger: press&hold

_Prb,                           //	recent app trigger: hard button
_Prs,                           //	recent app trigger: silk icon
_Pre1x,                         //	recent app trigger: stroke: start: x
_Pre1y,                         //	recent app trigger: stroke: start: y
_Pre2x,                         //	recent app trigger: stroke: end: x
_Pre2y,                         //	recent app trigger: stroke: end: y
_Prhold,                        //	recent app trigger: press&hold

_Pf,				//S	frame style
_Pfc,				//S	frame color
_Phsn,				//S	item height
_Psi,				//S	show icons
_Pani,				//	animation style
_Panis,				//	animation properties
_Pwhbg,				//S	white background
_Prev,				//S	reverse MRU order
_Pautohide,			//	autohide menu after
_Pfirst,			//	first launch?
_Poie,				//S	show icons only if exist
_Pund,				//S	underline hotkeys
_PautooffV,			//	last auto off delay (as opposed to 0)
_PsoundV,			//	last sound volume (as opposed to 0)
_Pchpal,			//	force default color palette

_Pkc,				//	trigger: properties: keycode
_Pbases,			//	trigger: properties: screen base
_Punused2,
_Punused3,
_Punused4,
_Punused5,
_Punused6,

_Prkc,				//	recent app trigger: properties: keycode
_Prbases,			//	recent app trigger: properties: screen base
_Preserved2,
_Preserved3,
_Preserved4,
_Preserved5,
_Preserved6,

_Pdelay,			//	press&hold delay
_Pvia,				//	tap through submenus
_Phidden,			//	show hidden apps in lists
_Phighlight,			//S	persistent selection/highlight
_Pdisabled,			//	Hi-Launcher is turned off
_Pdoubletp,			//	double trigger press action
_Pexcluderedial,		//	connection types excluded from redial
_PautooffD,			//	true if auto off was disabled (set to 0) by Hi-Launcher
_PC				//	count (must be last)
} Tprefs;

typedef enum {
_SPf,				//S	frame style
_SPfc,				//S	frame color
_SPhsn,				//S	item height
_SPsi,				//S	show icons
_SPwhbg,			//S	white background
_SPrev,				//S	reverse MRU order
_SPoie,				//S	show icons only if exist
_SPund,				//S	underline hotkeys
_SPhighlight,			//S	persistent selection/highlight
_SPversion,			//M	Menu format version

_SPC				//	count (must be last)
} TsavedPrefs;

#define _RC 32

#define RT RectangleType

#define maxbcount 0xB0
#define m_caption 63

#define m_card 8
#define m_path 512

#define b_w 6
#define b_h 8
#define b_c 4

#define Tmid Int16 //menu resource id
#define Tcid Int16 //caption resource id

#define hstyle_c 11
#define hstyle_c2 8
#define hstyle_s 3

#define didyoukwow 13

#define VFS_MAX_PATH 255

#define fieldwidth 62

typedef union {
	struct {
		UInt16 outgoing : 2;
		UInt16 cancelled : 2;
		UInt16 incoming : 2;
		UInt16 missed : 2;
		UInt16 unused : 8;
	} bits;
	UInt16 word;
} RedialExclusions;

typedef struct {
	UInt8 unused1;
	UInt8 roaming;
	DateType date;
	UInt8 hour;
	UInt8 minute;
	UInt16 unused2;
	UInt16 duration;
} PhoneCallLogRecord;

typedef enum {
	vsDeleteMe= 0,
	vsRunning,
	vsWaiting
} VFSState;

typedef struct {
	char path[VFS_MAX_PATH+ 1];
	UInt16 volRefNum;
	UInt16 cardNo;
	LocalID dbID;
	UInt32 creator;
	UInt32 type; //my
	VFSState vfsState;
} VFSRunningApp;

typedef struct {
Char name[dmDBNameLength];
BitmapPtr icon1000;
BitmapPtr icon1001;
char* path; // <-
UInt16 vol; // <-
UInt32 creator; // ->
} matchInfoType;

typedef struct {
char name[32];
UInt32 creator;
UInt32 special; //0 (unused)
} T1recent;

typedef struct {
char* name;
char* number;
char* nicenumber;//for comparison
} T1redial;

typedef enum {
_untyped_v0= 0,
_submenu_v0,
_application_v0,
_special_v0, //used in some cases only
_splitter_v0,
_recent_v0,
_home_v0,
_find_v0,
_beamreceive_v0,
_backlight_v0,
_brightness_v0,
_contrast_v0,
_hotsync_v0,
_off_v0,
_offandlock_v0,
_clock_v0,
_abc_v0,
_123_v0,
_toolbar_v0,
_menu_v0,
_autooff_v0,
_sound_v0,
_unknown_v0
} Ttype_v0;

typedef enum {
_untyped_v2= 0,

_0_main_v2= 0x20,
_submenu_v2= _0_main_v2,
_application_v2,
_card_v2,
_desk_v2,
_special_v2,
_toggle_v2,
_status_v2,
_splitter_v2,
_url_v2,
_dial_v2,
_x_main_v2,

_0_special_v2= 0x40,
_recent_v2= _0_special_v2,
_home_v2,
_find_v2,
_menu_v2,
_toolbar_v2,
_brightness_v2,
_contrast_v2,
_abc_v2,
_123_v2,
_off_v2,
_offandlock_v2,
_hotsync_v2,
_modemhotsync_v2,
_clock_v2,
_softreset_v2,
_attention_v2,
_graffiti_v2,
_redial_v2,
_x_special_v2,

_0_toggle_v2= 0x60,
_backlight_v2= _0_toggle_v2,
_beamreceive_v2,
_autooff_v2,
_sound_v2,
_bluetooth_v2,
_network_v2,
_radio_v2,
_x_toggle_v2,

_0_status_v2= 0x70,
_battery_v2= _0_status_v2, //^P - percent, ^V - volt
_memory_v2, //^T - total, ^F - free, ^U - used
_datetime_v2, //^T - time, ^D - date, ^W - day of week
_x_status_v2
} Ttype_v2;

typedef enum {
_untyped= 0,

_0_main= 0x20,
_submenu= _0_main,
_application,
_card,
_desk,
_panel,
_url,
_dial,
_special,	//==
_toggle,	//====> these 3 must be one after another due to the "SOS (...)" function
_status,	//==
_splitter,
_x_main,

_0_special= 0x40,
_recent= _0_special,
_redial,
_home,
_find,
_menu,
_toolbar,
_brightness,
_contrast,
_abc,
_123,
_off,
_offandlock,
_hotsync,
_modemhotsync,
_lasthotsync,
_clock,
_softreset,
_attention,
_graffiti,
_x_special,

_0_toggle= 0x60,
_backlight= _0_toggle,
_beamreceive,
_bluetooth,
_network,
_radio,
_autooff,
_sound,
_keylights,
_x_toggle,

_0_status= 0x70,
_battery= _0_status, //^P - percent, ^V - volt
_memory, //^T - total, ^F - free, ^U - used
_datetime, //^T - time, ^D - date, ^W - day of week
_x_status
} Ttype;

typedef IndexedColorType TColors[4];
#define dc0 0xD2 //background
#define dc1 0xD6 //text
#define dc2 0xD6 //line

typedef struct {
	union {
		struct {
			UInt16 count;
			UInt16 count_n;
			UInt16 count_c;
			UInt16 count_s;
			UInt16 count_r; //recent count
			UInt16 maxcw;
			UInt16 version;
			UInt16 count_g;
			Boolean canhilite;
			Boolean noicons;
			Int16 defaultitem;
			UInt16 maxcw2;
			UInt16 count_d; //redial count
			UInt32 reserved6;
		} header;
		struct {
			UInt32 creator; //_application
			Tmid mid; //_submenu
			UInt16 index; //_recent, _status, _redial
			IndexedColorType tl;
			IndexedColorType bl;
			IndexedColorType tc;
			IndexedColorType bc;
			Tcid uid; //_url
			Boolean invisible;
			Boolean unused0;
		} body;
	} data;
	Ttype type;
	UInt8 font;
	UInt8 hstyle;
	TColors color;
	UInt8 noicon;
	Tcid cid;	//caption resource id
	char fcc;	//first caption char (or 0)
	Boolean b1000;
	Boolean b1001;
	char shortcut;
	UInt8 shortcut_i;	//index of the char (or 0)
	UInt8 other;
	UInt32 unused1;
	UInt32 unused2;
	UInt32 unused3;
	UInt32 unused4;
	UInt32 unused5;
} Titem;

/***************************************/

char* A (char* c);
void* M0 (UInt32 size);
UInt16* LAP ();
Int16 GP (UInt16 p);
void SP (UInt16 p, Int16 v);
T1redial* LAD ();
UInt32* _1LAE (UInt16* exc, UInt16 w);
LocalID _1GFD (UInt32 type);
T1recent* _1LAR ();
IndexedColorType MC (IndexedColorType cb, IndexedColorType cf);
void WC (char* c, RectangleType* r);
char* GC (UInt32 creator, Tcid cid);
void RN ();
void SetImmediateAlarm (TAlarms alarm);
Boolean DD ();
void hede1 (UInt16 cardNo, LocalID dbID);
void hede0 (UInt16 cardNo, LocalID dbID);

/***************************************/

UInt8 AM (Tmid mid, Int16 x, Int16 y, Int16 ri, Int16 di);
void GRA ();
void EL ();
UInt16 PN (SysNotifyParamType* n);
void RAOS ();
Boolean OS (UInt32 atleast);

void LACL (matchInfoType* m);
void LACL2 (matchInfoType* m);

/***************************************/

typedef struct {
	TColors color;
	Boolean is;
	UInt8 font;
} Tstyle;

typedef struct {
	Tmid mid;
	Titem* items;
	char* title;
	void* prev; //Tmenu*
	Int16 trigger;
} Tmenu;

typedef struct {
	char name[dmDBNameLength];
	UInt32 creator;
} CardItemType;

typedef struct {
	Titem* item;
	char* caption;
	char* URL;
} TClip;

typedef struct {
	UInt16 CFI;
	FormPtr CFP;
	UInt16* P1;
	Boolean ME;
	UInt16 key;
	UInt16 keycode;
	PointType p1;
	PointType p2;
	Boolean baseR;
	Boolean baseB;
	FrameBitsType bits;

	UInt16 aac;
	MemHandle aah;
	SysDBListItemType* aap;
	char** aan;

	UInt16 cac;
	CardItemType* cap;
	char** can;
	Boolean car;

	UInt16 dac;
	MemHandle dah;
	SysDBListItemType* dap;
	char** dan;
	Boolean dar;

	UInt16 pac;
	MemHandle pah;
	SysDBListItemType* pap;
	char** pan;
	Boolean par;

	SysDBListItemType* Aap;
	Tmenu* M;
	DmOpenRef tmp;
	Boolean save;
	Boolean hidden;
	ListPtr L;
	UInt32 depth;
	IndexedColorType selfor;
	IndexedColorType col;
	UInt32 to;
	char* fc;
	char* fu;
	Int16 ind;
	Int16 ci;
	Int16 ui;
	Tstyle style;
	UInt16 ani;
	UInt16 anis;
	UInt32* ex;
	UInt16 exc;
	UInt32* di;
	UInt16 dic;
	UInt32* _ex;
	UInt16 _exc;
	Boolean ras;
	Boolean noted;
	UInt32 launcre;
	MemHandle _2;
	UInt16 sonySilkRefNum;
	Int32 CollapseAPIVersion;
	void (*HPEnter) (THP* P);
	Boolean (*HPChanged) (FormPtr frmP, THP* P);
	UInt16 hpp;
	UInt16 r_initDelay;
	UInt16 r_period;
	UInt16 r_doubleTapDelay;
	Boolean r_queueAhead;
	Boolean keydown;
	TClip clip;

	UInt16 amc;
	MemHandle amh;
	SysDBListItemType* amp;
	char** amn;

	Boolean reload;
	char loadname[32];
	
	Boolean DD;
} TRUN;

typedef struct {
	Tmid mid;
	DmOpenRef db;
	DmOpenRef swp;
	MemHandle m;
	Titem* M;
	UInt16 hires;
	UInt16 cnt;
	Boolean color;
	Boolean stoponnil;
	Boolean ipe;
	Boolean gsie;
	Coord sw;
	Coord sh;
	UInt16 oldcs;
	UInt16 ow;
	UInt16 oh;
	Int16 x;
	Int16 y;
	WinHandle W;
	WinHandle Wb;
	WinHandle oldw;
	BitmapType* B;
	BitmapType* Bb;
	FormType* F;
	WinHandle S;
	WinHandle oldaw;
	WinHandle olddw;
	RectangleType R;
	Boolean noted;
	Boolean stop;
	Int16* yy;
	Int16* hh;
	Int16 last;
	FrameBitsType f;
	IndexedColorType fc;
	Boolean si;
	Boolean asi;
	Boolean und;
	Int16 hsn;
	Int16 hi;
	Int16 ret;
	UInt16 ani;
	UInt16 anis;
	UInt8 exit;
	Boolean whbg;
	char** gg;

	UInt16 ri;//recent
	UInt16 riu;
	T1recent* MRU_RA;//all
	T1recent** MRU_RS;//sorted

	UInt16 di;//redial
	UInt16 diu;
	T1redial* MRU_DA;//all
	T1redial* MRU_DS;//sorted

	Boolean rev;
	Boolean b_a;
	UInt8 b_m;//0=notyet, 1=go, 2=after, 3=paused
	Boolean stay;
	UInt32 b_d;//last submenu popup tick
	UInt16 b_i;
	Int16 b_t;
	Int16 b_x;
	Int16 b_v;
	UInt32 autohide;
	UInt32 actick;//last menu user action tick
	Boolean wasnoted;
	Boolean small;
	UInt16 autooffV;
	UInt16 soundV;
	MemHandle P;
	UInt32 kyoceraOkOff;
	Boolean chpal;
	Boolean wasnil;
	UInt32 OS;
	UInt32 pass;
	Boolean via;
	Boolean highlight;
	Boolean attne;
	Boolean reserved;
	Boolean answered;
	UInt8 doubletp;
	UInt16 lastfocus;
	RedialExclusions excluderedial;
	UInt32 popuptick;//last submenu popup tick
#ifdef STOPREASON
	UInt16 stopreason;
#endif
} TPOP;

typedef struct {
	char* name;
	UInt32 creator;
	UInt32 type;
	UInt16 attr;
} Tdbi;

typedef char* pchar;

typedef struct {
	UInt16 pxs;
	UInt16 ow;
	UInt16 oh;
} Tarm1d;
