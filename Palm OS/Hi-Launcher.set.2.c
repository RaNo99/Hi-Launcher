#define use_HPMoveGSI

#include "SonyCLIE.h"
//#include "SonySilkLib.h"
#include "Standalone.h"
#include "VFSmgr.h"
#include "StringMgr.h"
#include "FeatureMgr.h"
#include "PenInputMgr.h"
#include "SysEvtMgr.h"
#include "KeyMgr.h"
#include "StringMgr.h"
#include "UIControls.h"
#include "ExgMgr.h"
#include "SysUtils.h"

#include "rns_hp.h"
#include "Hi-Launcher.h"

#define g_sonySilkRefNum RUN.sonySilkRefNum
#define g_CollapseAPIVersion RUN.CollapseAPIVersion

STANDALONE_CODE_RESOURCE_ID (2);

/***************************************/

void _2FF ();
void* _2GOP (UInt16 id);
UInt16 _2CGV (UInt16 id);
void _2CSV (UInt16 id, UInt16 v);
void _2SFT (UInt16 id, char* c, Boolean draw);
void _2HO (UInt16 ID);
void _2SO (UInt16 ID);
Boolean _2LRN (EventPtr e);
void _2SA (UInt16* A);
void _2HA (UInt16* A);
Boolean _2KSL (EventPtr e);
Boolean _2H1400 (EventPtr e);
void _2RHD (MemHandle H, UInt16* C);
void _2PRF ();
void _2TLI (UInt16 trig, UInt16 list, Int16 ind);
void _2PC (UInt16 w);
void _2SAP (UInt16* P);
Boolean _2H1300 (EventPtr e);
Boolean _2H1310 (EventPtr e);
Boolean _2H1320 (EventPtr e);
Boolean _2H1330 (EventPtr e);
Boolean _2H1340 (EventPtr e);
void _2DBH (MemHandle h, Coord x, Coord y);
void _2D6000 (Int16 i, RectangleType* r, Char** ch);
void _2D6020 (Int16 i, RectangleType* r, Char** ch);
void _2D6021 (Int16 i, RectangleType* r, Char** ch);
void _2D6022 (Int16 i, RectangleType* r, Char** ch);
DmOpenRef _2LDB (Tdbi* dbi, UInt16 mode);
void _2DAR (DmOpenRef db);
void _2CAR (DmOpenRef d1, DmOpenRef d2, DmResType except);
Boolean _2H1010 (EventPtr e);

/***************************************/

void start () {
_2FF ();
}

#include "Hi-Launcher.set.glob.c"

//fill functions
void _2FF () {
GOP= _2GOP;
CGV= _2CGV;
CSV= _2CSV;
SFT= _2SFT;
HO= _2HO;
SO= _2SO;
LRN= _2LRN;
SA= _2SA;
HA= _2HA;
KSL= _2KSL;
H1400= _2H1400;
RHD= _2RHD;
PRF= _2PRF;
TLI= _2TLI;
PC= _2PC;
SAP= _2SAP;
H1300= _2H1300;
H1310= _2H1310;
H1320= _2H1320;
H1330= _2H1330;
H1340= _2H1340;
DBH= _2DBH;
D6000= _2D6000;
D6020= _2D6020;
D6021= _2D6021;
D6022= _2D6022;
LDB= _2LDB;
DAR= _2DAR;
CAR= _2CAR;
H1010= _2H1010;
}

/***************************************/

#define GOP _2GOP
#define CGV _2CGV
#define CSV _2CSV
#define SFT _2SFT
#define HO _2HO
#define SO _2SO
#define LRN _2LRN
#define SA _2SA
#define HA _2HA
#define KSL _2KSL
#define H1400 _2H1400
#define RHD _2RHD
#define PRF _2PRF
#define TLI _2TLI
#define PC _2PC
#define SAP _2SAP
#define H1300 _2H1300
#define H1310 _2H1310
#define H1320 _2H1320
#define H1330 _2H1330
#define H1340 _2H1340
#define DBH _2DBH
#define D6000 _2D6000
#define D6020 _2D6020
#define D6021 _2D6021
#define D6022 _2D6022
#define LDB _2LDB
#define DAR _2DAR
#define CAR _2CAR
#define H1010 _2H1010

/***************************************/

Int16 INC (UInt32 c) {
Int16 i;
for (i= 0; i<RUN._exc; i++)
	if (RUN._ex[i]==c) return i;
return -1;
}

void D1 (Int16 i, RectangleType* r, Char** ch) {
UInt32 creator= RUN.Aap[i].creator;
char* name= (char*)&(RUN.Aap[i]);
UInt16 bm= _application;
FontID oldf;
if (RUN.Aap[i].cardNo==_card) bm= _card;
if (creator==RUN.launcre) bm= _home;
DBH (bmph[bm], r->topLeft.x+ 1, r->topLeft.y+ 1);
WinDrawTruncChars (name, StrLen (name), r->topLeft.x+14, r->topLeft.y, 110);
oldf= FntSetFont (1);
RUN._ex= RUN.di;
RUN._exc= RUN.dic;
if (INC (creator)!=-1)
	WinDrawChar ('D', 133, r->topLeft.y);
RUN._ex= RUN.ex;
RUN._exc= RUN.exc;
if (INC (creator)!=-1)
	WinDrawChar ('E', 126, r->topLeft.y);
FntSetFont (oldf);
}

/***************************************/

void* _2GOP (UInt16 id) {
return FrmGetObjectPtr (RUN.CFP, GOI (id));
}

UInt16 _2CGV (UInt16 id) {
return CtlGetValue (GOP (id));
}

void _2CSV (UInt16 id, UInt16 v) {
CtlSetValue (GOP (id), v);
}

void _2SFT (UInt16 id, char* c, Boolean draw) {
UInt16 len= StrLen (c);
FieldPtr FldP= GOP (id);
MemHandle h= FldGetTextHandle (FldP);
MemHandle n= MemHandleNew (len+ 1);
char* p= MemHandleLock (n);
StrCopy (p, c);
MemHandleUnlock (n);
FldSetTextHandle (FldP, n);
if (h!=0) MemHandleFree (h);
FrmSetFocus (RUN.CFP, GOI (id));
FldSetSelection (FldP, 0, len);
if (draw) FldDrawField (FldP);
}

void _2HO (UInt16 ID) {
FrmHideObject (RUN.CFP, GOI (ID));
}

void _2SO (UInt16 ID) {
FrmShowObject (RUN.CFP, GOI (ID));
}

void _2SA (UInt16* A) {
UInt16 i;
for (i= 1; i<=*A; i++) SO (A[i]);
}

void _2HA (UInt16* A) {
UInt16 i;
for (i= 1; i<=*A; i++) HO (A[i]);
}

void SWSH (Int16* SW, Int16* SH) { //twin
UInt32 version;
Err error= FtrGet ('pins', 1, &version);
WinGetDisplayExtent (SW, SH);
if ((!error) && (version>=0x01103000)) {
	UInt16 GH= PINGetInputAreaState ();
	if (GH==pinInputAreaOpen) {
		UInt16 ori= SysGetOrientation ();
		UInt16 ssac= 0;
		SilkscreenAreaType* ssa= (SilkscreenAreaType*)EvtGetSilkscreenAreaList (&ssac);
		UInt16 i;
		for (i= 0; i<ssac; i++) {
			if (ssa[i].areaType=='graf') {
				if (ori%2==1) *SH= *SH+ ssa[i].bounds.extent.y; else
					*SW= *SW+ ssa[i].bounds.extent.x;
				return;
			}
		}
	}
}
}

Boolean _2LRN (EventPtr e) {
if (RUN.CFI!=1110) return false;
switch (RUN.ME) {
case 0: case 1:
	if (e->eType!=keyDownEvent) return false;
	if (!(e->data.keyDown.modifiers & 8)) return false;
	switch (e->data.keyDown.chr) {
	case 0x1705: case 0x1702: case 301: case 899: case 269: case 0x1A0D:
	case 0x0317: case 0x013D: case 276: case 286: case 257: case 0x0117:
	case 0x0126: case 0x0127: case 0x0128: case 0x0129: case 0x012A: case 0x012B: case 0x012C: //ignored
		return false;
	}
	while ((KeyCurrentState () & (~0x100))!=0);
	{
	UInt16 Aall[6]= {5, 2000, 2001, 2002, 2003, 3001};
	char c[9];
	UInt16 sid;
	char* stx= MemPtrNew (64);
	UInt16 kc= e->data.keyDown.keyCode;
	UInt16 chr= e->data.keyDown.chr;
	StrIToA ((char*)&c, e->data.keyDown.chr);
	HA ((UInt16*)&Aall);
	SO (3000);
	if (chr==0x0132) chr= 11;
	if (chr==0x0133) chr= 12;
	switch (chr) {
	case 11: case 0x010C:
		sid= 0;//Up
		break;
	case 12: case 0x0103:
		sid= 1;//Down
		break;
	case 261:
		sid= 2;//Menu
		break;
	case 262:
		sid= 3;//Command Bar
		break;
	case 264:
		sid= 4;//Home
		break;
	case 266:
		sid= 5;//Find
		break;
	case 267:
		sid= 6;//Calc/Fav
		break;
	case 270:
		sid= 7;//Ronomatic
		break;
	case 272: case 1293:
		sid= 8;//Alpha Keyboard
		break;
	case 273: case 1294:
		sid= 9;//Numeric Keyboard
		break;
	case 516:
		sid= 10;//Date Book
		break;
	case 517:
		sid= 11;//Address
		break;
	case 518:
		sid= 12;//To Do List
		break;
	case 519:
		sid= 13;//Memo Pad/Note Pad
		break;
	case 520:
		sid= 14;//Power
		break;
	case 0x1700: case 0x012E: case 0x161B:
		sid= 15;//Jog Up
		break;
	case 0x1701: case 0x012F: case 0x161C:
		sid= 16;//Jog Down
		break;
	case 0x1706: case 0x1608: case 0x0130:
	case 0x000D:
		sid= 17;//Jog Push
		break;
	case 0x1707:
		sid= 18;//Back
		break;
	case 0x170C:
		sid= 36;//Capture
		break;
	case 0x170F:
		sid= 19;//Hold
		break;
	case 0x1727:
		sid= 35;//Camera
		break;
	case 0x0122: case 0x020D:
		sid= 20;//Brightness
		break;
	case 0x0123: case 0x1602: case 0x0113:
		sid= 21;//Contrast
		break;
	case 0x0134: case 0x1800: case 0x1708:
	case 0x1B04:
		sid= 23;//Left
		break;
	case 0x0135: case 0x1801: case 0x1709:
	case 0x1B05:
		sid= 24;//Right
		break;
	case 0x0136:
		if (kc!=0x0136) { //if not T600
			chr= 0x013D;
		}
		//nobreak
	case 0x0107:
		sid= 25;//Center
		break;
	case 0x0503: //(5Way)
		if (kc & 0x001F) return false;
		if (kc & 0x0400) sid= 23; else
		if (kc & 0x0800) sid= 24; else
		if (kc & 0x1000) sid= 25; else
			return false;
		break;
	case 0x160D: //(Treo extended)
		if (kc==0x0105) sid= 2; else
		if ((kc==0xE000) || (kc==0xE001)) sid= 44; else
		if (kc==0xE006) sid= 22; else
			return false;
		break;
	case 0x160B:
		sid= 26;//Lid
		break;
	case 0x0214: case 0x1A05: case 0x170D:
		sid= 27;//Record
		break;
	case 0x0215: case 0x0216: case 0x0217: case 0x1620: case 539: case 536:
		sid= 43;//Control + button
		break;
	case 1295:
		sid= 28;//Top-left Icon
		break;
	case 1296:
		sid= 29;//Bottom-left Icon
		break;
	case 1297:
		sid= 30;//Top-right Icon
		break;
	case 1298:
		sid= 31;//Bottom-right Icon
		break;
	case 0x0209:
		sid= 32;//HotSync
		break;
	case 0x020A:
		sid= 33;//Modem HotSync
		break;
	case 0x1A03:
		sid= 34;//Escape
		break;
	case 5905:
		sid= 37;//VG Select
		break;
	case 5906: case 6913: case 5904:
		sid= 38;//VG Toggle
		break;
	case 5908:
		sid= 39;//Battery
		break;
	case 5909:
		sid= 40;//Media
		break;
	case 5913:
		sid= 41;//Wireless
		break;
	case 5907:
		sid= 42;//Volume
		break;
	case 5663:
		sid= 45;//Side
		break;
	case 6929:
		sid= 46;//Music
		break;
	case 6918:
		sid= 47;//Blue
		break;
	case 6919:
		sid= 48;//Yellow
		break;
	case 6920: case 538:
		sid= 49;//Green
		break;
	case 6921:
		sid= 50;//Red
		break;
	case 0x0131:
		sid= 51;//Back/Function
		break;
	case 0x1B10:
		sid= 52;//Bluetooth
		break;
	case 0x1609:
		sid= 53;//Alt
		break;
	case 0x0500:
		sid= 54;//Clock
		break;
	default:
		StrCopy (stx, S_KEYCODE);
		StrCat (stx, (char*)&c);
		goto afterit;
	}
	SysStringByIndex (1000, sid, stx, 64);
	afterit:
	FrmCustomAlert (1002,stx,"","");
	SO (3001);
	SO (1000);
	RUN.key= chr;
	RUN.keycode= kc;
	return true;
	}
case 2: {
	PointType p1;
	PointType p2;
	Int16 SW;
	Int16 SH;
	if (e->eType==penDownEvent) RUN.noted= true;
	if (e->eType!=penUpEvent) return false;
	if (!RUN.noted) return false;
	RUN.noted= false;
	p1= e->data.penUp.start;
	p2= e->data.penUp.end;
	if ((p1.x==p2.x) && (p1.y==p2.y)) return false;
	SWSH (&SW, &SH);
	if (RUN.baseR) {
		p1.x= SW- p1.x- 1;
		p2.x= SW- p2.x- 1;
	}
	if (RUN.baseB) {
		p1.y= SH- p1.y- 1;
		p2.y= SH- p2.y- 1;
	}
	if (p1.x==0) p1.x= 1;
	HO (2004);
	HO (2005);
	HO (3003);
	SO (3002);
	FrmAlert (1003);
	SO (3003);
	SO (1000);
/*	if ((p1.x>80) && (p1.x<160)) p1.x*= -1;
	if ((p2.x>80) && (p2.x<160)) p2.x*= -1;
	if ((p1.y>80) && (p1.y<160)) p1.y*= -1;
	if ((p2.y>80) && (p2.y<160)) p2.y*= -1;*/
	RUN.p1= p1;
	RUN.p2= p2;
	return true;
	}
}
return false;
}

void TIE (Boolean inc) {
if (inc) {
	Int16 pos= INC (RUN.Aap[RUN.ind].creator);
	MemMove (&(RUN._ex[pos]), &(RUN._ex[pos+ 1]), (RUN._exc- pos- 1)* 4);
	if (--RUN._exc==0) {
		MemPtrFree (RUN._ex);
		RUN._ex= 0;
	}
} else {
	UInt32* R= MemPtrNew ((RUN._exc+ 1)* 4);
	R[RUN._exc]= RUN.Aap[RUN.ind].creator;
	if (RUN._ex!=0) {
		MemMove (R, RUN._ex, RUN._exc* 4);
		MemPtrFree (RUN._ex);
	}
	RUN._ex= R;
	RUN._exc++;
}
}

void R1400 () {
UInt16 SW;
UInt16 SH;
WinGetDisplayExtent (&SW, &SH);
{
RectangleType r1= {2, 2, 156, SH- 4};
UInt16 LIC= (SH- 75)/11;
WinSetBounds (FrmGetWindowHandle (RUN.CFP), &r1);
FrmSetObjectPosition (RUN.CFP, GOI(1000), 4, SH- 21);
FrmSetObjectPosition (RUN.CFP, GOI(1001), 57, SH- 21);
FrmSetObjectPosition (RUN.CFP, GOI(9001), 0, SH- 26);

FrmSetObjectPosition (RUN.CFP, GOI(3001), 3, 21+ (LIC*11));
FrmSetObjectPosition (RUN.CFP, GOI(3000), 3, 35+ (LIC*11));

LstSetHeight (GOP(6001), LIC);
}
}

void I1400 () {
RUN.launcre= PrefGetPreference (prefLauncherAppCreator);
RUN.ex= LAE (&RUN.exc, 1);
RUN.di= LAE (&RUN.dic, 2);
LstSetDrawFunction (RUN.L, &D1);
LstSetListChoices (RUN.L, 0, RUN.aac+ RUN.cac);
LstSetSelection (RUN.L, -1);
R1400 ();
FrmDrawForm (RUN.CFP);
}

void S1400 () {
Int16 i, j;
T1recent* RR;
PrefSetAppPreferences (crea, 1, 0, RUN.ex, RUN.exc*4, true);
PrefSetAppPreferences (crea, 2, 0, RUN.di, RUN.dic*4, true);
RR= LAR ();
for (i= _RC-1; i>=0; i--) {
	for (j=0; j<RUN.exc; j++) {
		if (RR[i].creator==RUN.ex[j]) {
			MemMove (&(RR[i]), &(RR[i+1]), sizeof(T1recent)* (_RC- i- 1));
			MemSet (&(RR[_RC- 1]), sizeof(T1recent), 0);
			break;
		}
	}
}
PrefSetAppPreferences (crea, 0, 0, RR, sizeof(T1recent)*_RC, false);
MemPtrFree (RR);
}

Boolean _2KSL (EventPtr e) {
if (e->data.keyDown.modifiers & 0x0008) {
	WChar chr= e->data.keyDown.chr;
	if ((chr==11) || (chr==12)) {
		LstScrollList (RUN.L, chr- 11, LstGetVisibleItems (RUN.L)- 1);
		return true;
	}
}
return false;
}

Boolean KIS1400 (EventPtr e) {
char c[2];
UInt16 i;
if (e->data.keyDown.modifiers & 0x0008) return false;
c[0]= e->data.keyDown.chr;
c[1]= 0;
for (i= 0; i<RUN.aac+ RUN.cac; i++) {
	if (StrCaselessCompare ((char*)&c, (char*)&(RUN.Aap[i]))<=0) {
		LstSetSelection (RUN.L, i);
		RUN._ex= RUN.di;
		RUN._exc= RUN.dic;
		CSV (3000, INC (RUN.Aap[i].creator)!=-1);
		RUN._ex= RUN.ex;
		RUN._exc= RUN.exc;
		CSV (3001, INC (RUN.Aap[i].creator)!=-1);
		SO (3000);
		SO (3001);
		return true;
	}
}
return false;
}

Boolean KIS1010 (EventPtr e) {
char c[2];
UInt16 i;
if (e->data.keyDown.modifiers & 0x0008) return false;
c[0]= e->data.keyDown.chr;
c[1]= 0;
for (i= 0; i<RUN.amc; i++) {
	if (StrCaselessCompare ((char*)&c, (char*)&(RUN.amp[i]))<=0) {
		LstSetSelection (RUN.L, i);
		return true;
	}
}
return false;
}

Boolean _2H1400 (EventPtr e) {
UInt16 i;
RUN.ind= LstGetSelection (RUN.L);
switch (e->eType) {
case frmOpenEvent:
	I1400 ();
	return true;
case frmCloseEvent:
	MemPtrFree (RUN.Aap);
	if (RUN.ex!=0) MemPtrFree (RUN.ex);
	if (RUN.di!=0) MemPtrFree (RUN.di);
	return false;
case ctlSelectEvent:
	switch (e->data.ctlSelect.controlID) {
	case 1000:
		S1400 ();
	case 1001:
		FrmGotoForm (1000);
		return true;
	case 3000:
		RUN._ex= RUN.di;
		RUN._exc= RUN.dic;
		TIE (!e->data.ctlSelect.on);
		RUN.dic= RUN._exc;
		RUN.di= RUN._ex;
		LstDrawList (RUN.L);
		return true;
	case 3001:
		RUN._ex= RUN.ex;
		RUN._exc= RUN.exc;
		TIE (!e->data.ctlSelect.on);
		RUN.exc= RUN._exc;
		RUN.ex= RUN._ex;
		LstDrawList (RUN.L);
		return true;
	}
	return false;
case lstSelectEvent:
	i= e->data.lstSelect.selection;
	RUN._ex= RUN.di;
	RUN._exc= RUN.dic;
	CSV (3000, INC (RUN.Aap[i].creator)!=-1);
	RUN._ex= RUN.ex;
	RUN._exc= RUN.exc;
	CSV (3001, INC (RUN.Aap[i].creator)!=-1);
	if (RUN.Aap[i].creator==crea)
		HO (3000); else SO (3000);
	SO (3001);
	return false;
case keyDownEvent:
	return (KSL (e) || KIS1400 (e));
case winDisplayChangedEvent:
	(RUN.HPChanged) (RUN.CFP, &HPP[1]);
	R1400 ();
	if (g_CollapseAPIVersion==sonyVersion1)
		WinEraseWindow ();
	FrmDrawForm (RUN.CFP);
}
return false;
}

void _2RHD (MemHandle H, UInt16* C) {
UInt16 cnt= *C;
SysDBListItemType* p= MemHandleLock (H);
Int16 i;
for (i= cnt- 1; i>=0; i--) {
	UInt16 attr;
	DmDatabaseInfo (p[i].cardNo, p[i].dbID, 0, &attr, 0, 0, 0, 0, 0, 0, 0, 0, 0);
	if (attr & 0x0100) {
		cnt--;
		MemMove (&(p[i]), &(p[i+1]), sizeof(SysDBListItemType)* (cnt- i));
	}
}
if (cnt!=0) MemHandleResize (H, sizeof(SysDBListItemType)* cnt);
MemHandleUnlock (H);
*C= cnt;
}

void _2PRF () {
RectangleType r= {80, 100, 66, 34};
UInt16 w= RUN.bits.bits.width;
UInt16 u= RUN.bits.bits.cornerDiam;
UInt16 v= RUN.bits.bits.shadowWidth;
IndexedColorType oldc;
WinHandle oldw;
if (u!=0) {
	UInt16 u1= w;
	if (u1>0) u1++;
	if (u1!=0) RUN.bits.bits.cornerDiam= u1;
}
if (v!=0) {
	UInt16 v1= w;
	if (v1!=0) RUN.bits.bits.shadowWidth= v1;
}
oldw= WinSetDrawWindow (WinGetDisplayWindow ());
WinEraseRectangle (&r, 0);
RctInsetRectangle (&r, 6);
if (RUN.depth>=8) oldc= WinSetForeColor (RUN.col);
WinDrawRectangleFrame (RUN.bits.word, &r);
if (RUN.depth>=8) WinSetForeColor (oldc);
WinSetDrawWindow (oldw);
}

void _2TLI (UInt16 trig, UInt16 list, Int16 ind) {
Int16 i;
ListPtr L= GOP(list);
LstSetSelection (L, ind);
for (i=0; i<3; i++) {
	if (ind-i>=0) {
		LstSetTopItem (L, ind-i);
	}
}
CtlSetLabel (GOP(trig), LstGetSelectionText (L, ind));
}

Boolean UPC (IndexedColorType* ind) {
RGBColorType rgb;
WinIndexToRGB (*ind, &rgb);
return UIPickColor (ind, &rgb, 0, 0, 0);
}

void _2PC (UInt16 w) {
Int16 ind= RUN.ind;
IndexedColorType c= RUN.M->items[ind+ 1].color[w];
if (UPC (&c)) {
	RUN.M->items[ind+ 1].color[w]= c;
	RUN.M->items->color[w]= c;
	if (RUN.depth>8) FrmDrawForm (RUN.CFP); else
		LstDrawList (RUN.L);
} else if (RUN.depth>8) FrmDrawForm (RUN.CFP);
}

void _2SAP (UInt16* P) {
PrefSetAppPreferences (crea, 0, 0, P, _PC*2, true);
FtrUnregister (crea, _Fb);
FtrUnregister (crea, _Fs);
FtrUnregister (crea, _Fe1);
FtrUnregister (crea, _Fe2);
FtrUnregister (crea, _Fhold);
FtrUnregister (crea, _Fkc);

FtrUnregister (crea, _Frb);
FtrUnregister (crea, _Frs);
FtrUnregister (crea, _Fre1);
FtrUnregister (crea, _Fre2);
FtrUnregister (crea, _Frhold);
FtrUnregister (crea, _Frkc);

FtrUnregister (crea, _Fdelay);
FtrUnregister (crea, _Fsametriggers);
}

void I1300 () {
#ifdef trial
SO (3003);
#endif
FrmDrawForm (RUN.CFP);
}

void S1300 () {
SAP (RUN.P1);
if (RUN.hidden!=RUN.P1[_Phidden]) {
	UInt16 cardNo;
	LocalID dbID;
	SysCurAppDatabase (&cardNo, &dbID);
	SysUIAppSwitch (cardNo, dbID, 0, 0);
	RUN.hidden= RUN.P1[_Phidden];//perhaps not needed
}
}

Boolean _2H1300 (EventPtr e) {
switch (e->eType) {
case frmOpenEvent:
	I1300 ();
	return true;
case frmCloseEvent:
	MemPtrFree (RUN.P1);
	return false;
case ctlSelectEvent:
	switch (e->data.ctlSelect.controlID) {
#ifdef trial
	case 3003:
		CSV (3003, 1);
		FrmAlert (2201);
		return true;
#endif
	case 1000:
		S1300 ();
	case 1001:
		FrmGotoForm (1000);
		return true;
	case 4000:
		FrmPopupForm (1310);
		return true;
	case 4001:
		FrmPopupForm (1320);
		return true;
	case 4002:
		FrmPopupForm (1330);
		return true;
	case 4003:
		FrmPopupForm (1340);
		return true;
	}
	return false;
}
return false;
}

void I1310 () {
RUN.col= 255- RUN.P1[_Pfc];
RUN.bits.word= RUN.P1[_Pf];
{
UInt16 width= RUN.bits.bits.width;
UInt16 shadow= RUN.bits.bits.shadowWidth;
UInt16 corner= RUN.bits.bits.cornerDiam;
CSV (3000, width);
CSV (3001, shadow);
CSV (3002, corner);
CSV (3003, RUN.P1[_Pwhbg]);
FrmDrawForm (RUN.CFP);
PRF ();
}
}

void S1310 () {
RUN.P1[_Pf]= RUN.bits.word;
RUN.P1[_Pfc]= 255- RUN.col;
RUN.P1[_Pwhbg]= CGV (3003);
}

Boolean _2H1310 (EventPtr e) {
switch (e->eType) {
case frmOpenEvent:
	I1310 ();
	return true;
case frmCloseEvent:
	RUN.CFP= FrmGetFormPtr (1300);
	RUN.CFI= 1300;
	return false;
case ctlRepeatEvent:
	if (e->data.ctlRepeat.controlID==3000) {
		UInt16 v= e->data.ctlRepeat.value;
		RUN.bits.bits.width= v;
		PRF ();
	}
	return false;
case ctlSelectEvent:
	switch (e->data.ctlSelect.controlID) {
	case 1000:
		S1310 ();
	case 1001:
		FrmReturnToForm (1300);
		RUN.CFP= FrmGetFormPtr (1300);
		RUN.CFI= 1300;
		return true;
	case 8000:
		if (RUN.depth<8) FrmAlert (1004); else {
			UPC (&RUN.col);
			if (RUN.depth>8) FrmDrawForm (RUN.CFP);
			PRF ();
		}
		return true;
	case 3001:
		RUN.bits.bits.shadowWidth= e->data.ctlSelect.on;
		PRF ();
		return true;
	case 3002:
		RUN.bits.bits.cornerDiam= e->data.ctlSelect.on;
		PRF ();
		return true;
	}
	return false;
}
return false;
}

void G1320 () {
UInt16 Amosaic[]= {2, 5010, 3000};
UInt16 Afade[]= {4, 3014, 3015, 3016, 3017};
UInt16 Aslifade[]= {4, 3010, 3011, 3012, 3013};
if (RUN.ani==0) HO (5001);
if (RUN.ani!=1) HA ((UInt16*)&Amosaic);
if (RUN.ani!=2) HA ((UInt16*)&Afade);
if (RUN.ani<2) HA ((UInt16*)&Aslifade);

switch (RUN.ani) {
case 1:
	if (RUN.anis>255) RUN.anis= 4;
	CSV (3000, RUN.anis);
	break;
case 2:
	if (RUN.anis<256) RUN.anis= 256;
	FrmSetControlGroupSelection (RUN.CFP, 2, RUN.anis- 256+ 3010);
	break;
case 3:
	if ((RUN.anis<256) || (RUN.anis>259))RUN.anis= 256;
	FrmSetControlGroupSelection (RUN.CFP, 2, RUN.anis- 256+ 3010);
	break;
}

if (RUN.ani!=0) SO (5001);
if (RUN.ani>=2) SA ((UInt16*)&Aslifade);
if (RUN.ani==2) SA ((UInt16*)&Afade);
if (RUN.ani==1) SA ((UInt16*)&Amosaic);
}

void I1320 () {
RUN.ani= RUN.P1[_Pani];
RUN.anis= RUN.P1[_Panis]+ 4;
/*
0..3	- mosaic 1..4
4..9	- mosaic 8..13
256	- right
257	- left
258	- up
259	- down
260	- ih
261	- iv
262	- oh
263	- ov
*/
CSV (3001+ RUN.ani, 1);
G1320 ();
FrmDrawForm (RUN.CFP);
}

void S1320 () {
RUN.P1[_Pani]= RUN.ani;
RUN.P1[_Panis]= RUN.anis- 4;
}

Boolean _2H1320 (EventPtr e) {
switch (e->eType) {
case frmOpenEvent:
	I1320 ();
	return true;
case frmCloseEvent:
	RUN.CFP= FrmGetFormPtr (1300);
	RUN.CFI= 1300;
	return false;
case ctlSelectEvent:
	switch (e->data.ctlSelect.controlID) {
	case 1000:
		S1320 ();
	case 1001:
		FrmReturnToForm (1300);
		RUN.CFP= FrmGetFormPtr (1300);
		RUN.CFI= 1300;
		return true;
	case 3000:
		RUN.anis= e->data.ctlSelect.value;
		return true;
	case 3001: case 3002: case 3003: case 3004:
		RUN.ani= e->data.ctlSelect.controlID- 3001;
		G1320 ();
		return true;
	case 3010: case 3011: case 3012: case 3013: case 3014: case 3015: case 3016: case 3017:
		RUN.anis= e->data.ctlSelect.controlID- 3010+ 256;
		return true;
	}
	return false;
}
return false;
}

void I1330 () {
RedialExclusions re;
Boolean si= !RUN.P1[_Psi];
CSV (3000, RUN.P1[_Phsn]+ 6);
CSV (3001, si);
CSV (3002, RUN.P1[_Prev]);
CSV (3004, RUN.P1[_Poie]);
CSV (3003, !RUN.P1[_Pund]);
CSV (3006, RUN.P1[_Phighlight]);
if (si) SO (3004);
CSV (3005, RUN.P1[_Phidden]);
re.word= RUN.P1[_Pexcluderedial];
CSV (3010, !re.bits.outgoing);
CSV (3011, !re.bits.cancelled);
CSV (3012, !re.bits.incoming);
CSV (3013, !re.bits.missed);
FrmDrawForm (RUN.CFP);
}

Boolean S1330 () {
RedialExclusions re;
re.word= 0;
re.bits.outgoing= !CGV (3010);
re.bits.cancelled= !CGV (3011);
re.bits.incoming= !CGV (3012);
re.bits.missed= !CGV (3013);
if (re.bits.outgoing && re.bits.cancelled && re.bits.incoming && re.bits.missed) {
	FrmAlert (1034);
	return false;
}
RUN.P1[_Phsn]= CGV (3000)- 6;
RUN.P1[_Psi]= !CGV (3001);
RUN.P1[_Prev]= CGV (3002);
RUN.P1[_Poie]= CGV (3004);
RUN.P1[_Pund]= !CGV (3003);
RUN.P1[_Phighlight]= CGV (3006);
RUN.P1[_Phidden]= CGV (3005);
RUN.P1[_Pexcluderedial]= re.word;
return true;
}

Boolean _2H1330 (EventPtr e) {
switch (e->eType) {
case frmOpenEvent:
	I1330 ();
	return true;
case frmCloseEvent:
	RUN.CFP= FrmGetFormPtr (1300);
	RUN.CFI= 1300;
	return false;
case ctlSelectEvent:
	switch (e->data.ctlSelect.controlID) {
	case 1000:
		if (!S1330 ()) {
			return true;
		}
	case 1001:
		FrmReturnToForm (1300);
		RUN.CFP= FrmGetFormPtr (1300);
		RUN.CFI= 1300;
		return true;
	case 3001:
		if (e->data.ctlSelect.on) SO (3004);
			else HO (3004);
		return false;
	}
	return false;
}
return false;
}

void I1340 () {
UInt16 val;
CSV (3000, RUN.P1[_Pdelay]+ 6);
CSV (3004, RUN.P1[_Pchpal]);
TLI (5000, 5001, RUN.P1[_Pautohide]/ 4);
val= RUN.P1[_Pdoubletp];
if (val>4) val= 0;//number of double-trigger-press actions
TLI (5100, 5101, val);
CSV (3005, RUN.P1[_Pvia]);
FrmDrawForm (RUN.CFP);
}

void S1340 () {
RUN.P1[_Pautohide]= LstGetSelection (GOP (5001))* 4;
RUN.P1[_Pdoubletp]= LstGetSelection (GOP (5101));
RUN.P1[_Pchpal]= CGV (3004);
RUN.P1[_Pdelay]= CGV (3000)- 6;
RUN.P1[_Pvia]= CGV (3005);
}

Boolean _2H1340 (EventPtr e) {
switch (e->eType) {
case frmOpenEvent:
	I1340 ();
	return true;
case frmCloseEvent:
	RUN.CFP= FrmGetFormPtr (1300);
	RUN.CFI= 1300;
	return false;
case ctlSelectEvent:
	switch (e->data.ctlSelect.controlID) {
	case 1000:
		S1340 ();
	case 1001:
		FrmReturnToForm (1300);
		RUN.CFP= FrmGetFormPtr (1300);
		RUN.CFI= 1300;
		return true;
	}
	return false;
}
return false;
}

void _2DBH (MemHandle h, Coord x, Coord y) {
if (h) {
	BitmapPtr b= MemHandleLock (h);
	if (b) {
		if (RUN.DD) {
			BitmapPtr B= WinGetBitmap (WinGetDrawWindow ());
			UInt16 oldd= BmpGetDensity (B);
			BmpSetDensity (B, 72);
			WinDrawBitmap (b, x, y);
			BmpSetDensity (B, oldd);
		} else {
			WinDrawBitmap (b, x, y);
		}
		MemHandleUnlock (h);
	}
}
}

void _2D6000 (Int16 i, RectangleType* r, Char** ch) {
DBH (bmph[_0_main+ i], r->topLeft.x+ 1, r->topLeft.y+1);
WinDrawChars (ch[i], StrLen (ch[i]), r->topLeft.x+14, r->topLeft.y);
}

void _2D6020 (Int16 i, RectangleType* r, Char** ch) {
DBH (bmph[_0_special+ i], r->topLeft.x+ 1, r->topLeft.y+ 1);
WinDrawChars (ch[i], StrLen (ch[i]), r->topLeft.x+14, r->topLeft.y);
}

void _2D6021 (Int16 i, RectangleType* r, Char** ch) {
DBH (bmph[_0_toggle+ i], r->topLeft.x+ 1, r->topLeft.y+ 1);
WinDrawChars (ch[i], StrLen (ch[i]), r->topLeft.x+14, r->topLeft.y);
}

void _2D6022 (Int16 i, RectangleType* r, Char** ch) {
DBH (bmph[_0_status+ i], r->topLeft.x+ 1, r->topLeft.y+ 1);
WinDrawChars (ch[i], StrLen (ch[i]), r->topLeft.x+14, r->topLeft.y);
}

DmOpenRef _2LDB (Tdbi* dbi, UInt16 mode) {
LocalID id;
if (mode & 0x1000) {
	mode-= 0x1000;
	id= DmFindDatabase (0, dbi->name);
	if (id) goto afound;
} else {
	DmOpenRef db;
	DmSearchStateType ss;
	UInt16 card= 0;
	Err e= DmGetNextDatabaseByTypeCreator (true, &ss, dbi->type, dbi->creator, false, &card, &id);
	if ((e==0) && (id!=0)) {
		db= DmOpenDBNoOverlay (card, id, mode);
		if (db!=0) return db;
	}
}
DmCreateDatabase (0, dbi->name, dbi->creator, dbi->type, 1);
id= DmFindDatabase (0, dbi->name);
DmSetDatabaseInfo (0, id, 0, &(dbi->attr), 0, 0, 0, 0, 0, 0, 0, 0, 0);
afound:
return DmOpenDBNoOverlay (0, id, mode);
}

void _2DAR (DmOpenRef db) {
UInt16 rc= DmNumResources (db);
Int16 i;
for (i= rc-1; i>=0; i--)
	DmRemoveResource (db, i);
}

void _2CAR (DmOpenRef d1, DmOpenRef d2, DmResType except) {
UInt16 rc= DmNumResources (d1);
UInt16 i;
for (i= 0; i<rc; i++) {
	DmResType type;
	DmResID id;
	MemHandle h1;
	UInt32 hs;
	DmResourceInfo (d1, i, &type, &id, 0);
	if (except==type) continue;
	h1= DmGetResourceIndex (d1, i);
	hs= MemHandleSize (h1);
	if (hs!=0) {
		MemHandle h2= DmNewResource (d2, type, id, hs);
		void* p1= MemHandleLock (h1);
		void* p2= MemHandleLock (h2);
		DmWrite (p2, 0, p1, hs);
		MemHandleUnlock (h2);
		MemHandleUnlock (h1);
		DmReleaseResource (h2);
	}
	DmReleaseResource (h1);
}
}

Int16 LMs () {
Int16 ind= -1;
SysCreateDataBaseList ('Rthe', crea, &RUN.amc, &RUN.amh, true);
RUN.amn= 0;
if (RUN.amc!=0) {
	UInt16 i;
	RUN.amn= MemPtrNew (RUN.amc* sizeof(char*));
	RUN.amp= MemHandleLock (RUN.amh);
	for (i= 0; i<RUN.amc; i++) {
		RUN.amn[i]= (char*)&(RUN.amp[i]);
		if (!StrCompare (RUN.amn[i], RUN.loadname)) {
			ind= i;
		}
	}
}
LstSetListChoices (RUN.L, RUN.amn, RUN.amc);
LstSetSelection (RUN.L, -1);
return ind;
}

void UMs () {
if (RUN.amc!=0) {
	MemHandleUnlock (RUN.amh);
	MemHandleFree (RUN.amh);
	MemPtrFree (RUN.amn);
}
}       

Boolean CanOverwrite () {
LocalID id= GFD ('Data');
if (id) {
	if (FrmAlert (1028)) return false;
 	DmDeleteDatabase (0, id);
}
return true;
}

Boolean S1010 (char* c) {
Tdbi DB= {c, crea, 'Rthe', 0x0009};
Boolean proceed= true;
DmOpenRef src= LDB (&DB, dmModeReadOnly | 0x1000);
if (src) {
	MemHandle H= DmGet1Resource ('tOPT', 0);
	if (H) {
		UInt16* sps= MemHandleLock (H);
		if ((MemHandleSize (H)<(_SPC*2)) || (sps[_SPversion]<=MENUVERSION)) { //load menu version (0 or 2) or 3
			if (CanOverwrite ()) {
				RUN.P1[_Pf]= sps[_SPf];
				RUN.P1[_Pfc]= sps[_SPfc];
				RUN.P1[_Phsn]= sps[_SPhsn];
				RUN.P1[_Psi]= sps[_SPsi];
				RUN.P1[_Pwhbg]= sps[_SPwhbg];
				RUN.P1[_Prev]= sps[_SPrev];
				RUN.P1[_Poie]= sps[_SPoie];
				RUN.P1[_Pund]= sps[_SPund];
				RUN.P1[_Phighlight]= sps[_SPhighlight];
				SAP (RUN.P1);
			} else {
				proceed= false;
			}
		} else {
			FrmAlert (2014);
			proceed= false;
		}
		MemHandleUnlock (H);
		DmReleaseResource (H);
	} else {
		proceed= CanOverwrite ();
	}
	if (proceed) {
		DmOpenRef db= LDB (&(DBI[DBdata]), dmModeReadWrite);
	 	if (db) {
			CAR (src, db, 'tOPT');
			DmCloseDatabase (db);
		}
	}
	DmCloseDatabase (src);
}
if (proceed) {
	PrefSetAppPreferences (crea, 1, 0, c, StrLen(c)+1, false);
	StrCopy (RUN.loadname, c);
	RUN.reload= true;
}
return proceed;
}

Boolean S1020 () {
LocalID id;
DmOpenRef db;
char* c= FldGetTextPtr (GOP (3000));
if ((!c) || (!StrLen (c))) {
	FrmAlert (1024);
	return false;
}
id= DmFindDatabase (0, c);
if (id) {
	UInt32 type, creator;
	DmDatabaseInfo (0, id, 0, 0, 0, 0, 0, 0, 0, 0, 0, &type, &creator);
	if ((type=='Rthe') && (creator==crea)) {
		if (FrmAlert (1025)) return false;
	} else {
		FrmCustomAlert (1026, S_BYANOTHER, 0, 0);
		return false;
	}
	db= DmOpenDBNoOverlay (0, id, dmModeReadWrite);
	if (db) DAR (db);
} else {
	Tdbi DB= {c, crea, 'Rthe', 0x0009};
	db= LDB (&DB, dmModeReadWrite | 0x1000);
}
if (db) {
	MemHandle H;
	UInt16* p;
	UInt16 sps[_SPC];
	DmOpenRef src= LDB (&(DBI[DBdata]), dmModeReadOnly);
	if (src) {
		CAR (src, db, 0);
		DmCloseDatabase (src);
	}
	H= DmNewResource (db, 'tOPT', 0, _SPC*2);
	p= MemHandleLock (H);
	sps[_SPf]= RUN.P1[_Pf];
	sps[_SPfc]= RUN.P1[_Pfc];
	sps[_SPhsn]= RUN.P1[_Phsn];
	sps[_SPsi]= RUN.P1[_Psi];
	sps[_SPwhbg]= RUN.P1[_Pwhbg];
	sps[_SPrev]= RUN.P1[_Prev];
	sps[_SPoie]= RUN.P1[_Poie];
	sps[_SPund]= RUN.P1[_Pund];
	sps[_SPhighlight]= RUN.P1[_Phighlight];
	sps[_SPversion]= MENUVERSION;
	DmWrite (p, 0, &sps, _SPC*2);
	MemHandleUnlock (H);
	DmReleaseResource (H);
	DmCloseDatabase (db);
}
PrefSetAppPreferences (crea, 1, 0, c, StrLen(c)+1, false);
StrCopy (RUN.loadname, c);
return true;
}

// callback:
Err WDBD (const void* dataP, UInt32* sizeP, void* userDataP) {
Err err;
*sizeP= ExgSend ((ExgSocketPtr)userDataP, (void*)dataP, *sizeP, &err);
return err;
}

Err doSDB (UInt16 cardNo, LocalID dbID, char* nameP, char* descriptionP) {
ExgSocketType exgSocket;
Err err;
MemSet (&exgSocket, sizeof(exgSocket), 0);
exgSocket.description= descriptionP;
exgSocket.name= nameP;
err= ExgPut (&exgSocket);
if (err) {
	if (err==0x1501) FrmAlert (1029);
} else {
	err= ExgDBWrite (WDBD, &exgSocket, NULL, dbID, cardNo);
	err= ExgDisconnect (&exgSocket, err);
}
return err;
}

void SDB (char* c, Boolean send) {
LocalID id= DmFindDatabase (0, c);
DmOpenRef db= DmOpenDBNoOverlay (0, id, dmModeReadOnly);
char name[64]= {0};
if (send) StrCopy ((char*)&name, "?_send:");
StrCat ((char*)&name, c);
StrCat ((char*)&name, ".pdb");
doSDB (0, id, (char*)&name, c);
DmCloseDatabase (db);
}

void DM (char* c) {
if (FrmCustomAlert (1030, c, "", "")) return;
DmDeleteDatabase (0, DmFindDatabase (0, c));
UMs ();
LMs ();
LstDrawList (RUN.L);
}

void RM (char* c) {
UInt16 i;
LocalID dbid= DmFindDatabase (0, c);
FormPtr fp= FrmInitForm (1030);
UInt16 len= StrLen (c);
UInt16 ind= FrmGetObjectIndex (fp, 3000);
FieldPtr FldP= FrmGetObjectPtr (fp, ind);
FldInsert (FldP, c, len);
FrmSetFocus (fp, ind);
FldSetSelection (FldP, 0, len);
again:
if (FrmDoDialog (fp)==1002) {
	c= FldGetTextPtr (FldP);
	if ((!c) || (!StrLen (c))) {
		FrmAlert (1024);
		goto again;
	}
	if (DmFindDatabase (0, c)) {
		FrmAlert (1026);
		goto again;
	}
	DmSetDatabaseInfo (0, dbid, c, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
}
UMs ();
LMs ();
for (i= 0; i<RUN.amc; i++) if (!StrCompare (RUN.amn[i], c)) {
	LstSetSelection (RUN.L, i);
	break;
}
FrmDeleteForm (fp);
LstDrawList (RUN.L);
}

void R1010 () {
UInt16 SW;
UInt16 SH;
WinGetDisplayExtent (&SW, &SH);
{
RectangleType r1= {2, 2, 156, SH- 4};
UInt16 LIC= (SH- 42)/11;
WinSetBounds (FrmGetWindowHandle (RUN.CFP), &r1);
FrmSetObjectPosition (RUN.CFP, GOI(1000), 4, SH- 21);
FrmSetObjectPosition (RUN.CFP, GOI(1001), 57, SH- 21);
FrmSetObjectPosition (RUN.CFP, GOI(9001), 0, SH- 26);
LstSetHeight (GOP(2000), LIC);
}
}

void R1020 () {
UInt16 SW;
UInt16 SH;
WinGetDisplayExtent (&SW, &SH);
{
RectangleType r1= {2, 2, 156, SH- 4};
UInt16 LIC= (SH- 53)/11;
WinSetBounds (FrmGetWindowHandle (RUN.CFP), &r1);
FrmSetObjectPosition (RUN.CFP, GOI(1002), 4, SH- 21);
FrmSetObjectPosition (RUN.CFP, GOI(1001), 57, SH- 21);
FrmSetObjectPosition (RUN.CFP, GOI(9001), 0, SH- 26);
LstSetHeight (GOP(2000), LIC);
}
}

Boolean _2H1010 (EventPtr e) {
Int16 sel= LstGetSelection (RUN.L);
char* c= 0;
if (sel!=-1) c= LstGetSelectionText (RUN.L, sel);
switch (e->eType) {
case menuEvent:
	if (c==0) return false;
	switch (e->data.menu.itemID) {
	case 1000:
		SDB (c, 0);
		return true;
	case 1001:
		SDB (c, 1);
		return true;
	case 1002:
		DM (c);
		return true;
	case 1003:
		RM (c);
		return true;
	}
	return false;
case keyDownEvent:
	if (RUN.CFI==1020) return KSL (e);
	return (KSL (e) || KIS1010 (e));
case frmCloseEvent:
	UMs ();
	MemPtrFree (RUN.P1);
	return false;
case frmOpenEvent:
	LstSetSelection (RUN.L, LMs ());
	if (RUN.CFI==1020) {
		SFT (3000, RUN.loadname, false);
		R1020 ();
	} else R1010 ();
	FrmDrawForm (RUN.CFP);
	return true;
case lstSelectEvent:
	if (RUN.CFI==1020) SFT (3000, c, true);
	return true;
case ctlSelectEvent:
	switch (e->data.ctlSelect.controlID) {
	case 1000: //load:
		if (sel==-1) return false;
		if (S1010 (c)) FrmGotoForm (1000);
		return true;
	case 1001:
		FrmGotoForm (1000);
		return true;
	case 1002: //save:
		if (S1020 ()) FrmGotoForm (1000);
		return true;
	}
	return false;
case winDisplayChangedEvent:
	(RUN.HPChanged) (RUN.CFP, &HPP[1]);
	if (RUN.CFI==1020) R1020 (); else R1010 ();
	if (g_CollapseAPIVersion==sonyVersion1)
		WinEraseWindow ();
	FrmDrawForm (RUN.CFP);
}
return false;
}
