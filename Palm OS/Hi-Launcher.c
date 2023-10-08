#define use_HPMoveGSI

#include "PalmTypes.h"
#include "Rect.h"
#include "Form.h"
#include "MemoryMgr.h"
#include "StringMgr.h"
#include "FeatureMgr.h"
#include "NotifyMgr.h"
#include "ExgMgr.h"
#include "HsPhone.h"

//#include "SonySilkLib.h"
#include "rns_hp.h"
#include "Hi-Launcher.h"

/***************************************/

void LACL2 (matchInfoType* m) {
LACL (m);
}

char* A (char* c) {
return (char*)StrCopy (MemPtrNew (StrLen (c)+ 1), c);
}

void* M0 (UInt32 size) {
void* p= MemPtrNew (size);
MemSet (p, size, 0);
return p;
}

UInt16* LAP () {
UInt16* P= MemPtrNew (_PC*2);
UInt16 rSiz= _PC*2;
MemSet (P, _PC*2, 0);
PrefGetAppPreferences (crea, 0, P, &rSiz, true);
return P;
}

Int16 GP (UInt16 p) {
Int16 P[_PC];
UInt16 rSiz= _PC*2;
MemSet (&P, _PC*2, 0);
PrefGetAppPreferences (crea, 0, &P, &rSiz, true);
return (P[p]);
}

void SP (UInt16 p, Int16 v) {
Int16* P= LAP ();
P[p]= v;
PrefSetAppPreferences (crea, 0, 0, P, _PC*2, true);
MemPtrFree (P);
}

T1redial* LAD (RedialExclusions ex) {
T1redial* R= M0 (sizeof(T1redial)* _RC);
DmOpenRef db;
UInt16 count= 0;
db= DmOpenDatabaseByTypeCreator ('call', 'HsPh', dmModeReadOnly);
if (db) {
	Int16 i;
	Int16 num= DmNumRecords (db);
	for (i= 0; ((i<num) && (count<_RC)); i++) {
		UInt16 attr;
		MemHandle h= DmQueryRecord (db, i);
		char* c; char* n; char* nn; char* nnp;
		UInt16 nnlen;
		UInt16 j;
		PhoneCallLogRecord* r;
		DmRecordInfo (db, i, &attr, 0, 0);
		attr&= dmRecAttrCategoryMask;
		if ((attr==1) && (ex.bits.incoming)) continue;
		if ((attr==2) && (ex.bits.missed)) continue;
		r= MemHandleLock (h);
		if (attr==3) {
			if (((r->duration) && (ex.bits.outgoing)) || ((!r->duration) && (ex.bits.cancelled))) {
				goto nextloop;
			}
		}
		c= (char*)&(r[1]);
		n= c+ StrLen (c)+ 1;
		nnlen= StrLen (n);
		if (nnlen==0) goto nextloop;
		nn= M0 (nnlen+ 1);
		nnp= nn;
		for (j=0; j<nnlen; j++) {
			if (((n[j]>='0') && (n[j]<='9')) || (n[j]=='*') || (n[j]=='#')) {
				*nnp= n[j];
				nnp++;
			}
		}
		*nnp= 0;
		nnlen= StrLen (nn);
		if (nnlen>6) nnp= nn+(nnlen-6);
		R[count].nicenumber= A (nnp);
		MemPtrFree (nn);
		for (j=0; j<count; j++) {
			if (!StrCompare (R[j].nicenumber, R[count].nicenumber)) {
				MemPtrFree (R[count].nicenumber);
				goto nextloop;
			}
		}
		R[count].name= A (c[0]?c:n);
		R[count].number= A (n);
		count++;
		nextloop:
		MemHandleUnlock (h);
	}
	DmCloseDatabase (db);
	for (i=0; i<count; i++) {
		MemPtrFree (R[i].nicenumber);
	}
}
return R;
}

IndexedColorType MC (IndexedColorType cb, IndexedColorType cf) {
RGBColorType ub;
RGBColorType uf;
RGBColorType rgb;
Int16 mi, dif;
WinIndexToRGB (cb, &ub);
WinIndexToRGB (cf, &uf);
mi= ub.r- uf.r;
dif= mi/ 2;
rgb.r= uf.r+ dif;
mi= ub.g- uf.g;
dif= mi/ 2;
rgb.g= uf.g+ dif;
mi= ub.b- uf.b;
dif= mi/ 2;
rgb.b= uf.b+ dif;
return WinRGBToIndex (&rgb);
}

void WC (char* c, RectangleType* r) {
UInt16 len= StrLen (c);
UInt16 w= FntCharsWidth (c, len)+ 1;
if (w>r->extent.x) w= r->extent.x;
WinDrawTruncChars (c, len, r->topLeft.x+ ((r->extent.x- w)/2), r->topLeft.y+ ((r->extent.y- 11)/2), w);
}

char* GC (UInt32 creator, Tcid cid) {
MemHandle h= DmGetResource (creator, cid);
char* d;
if (h==0) return A ("");
d= A (MemHandleLock (h));
MemHandleUnlock (h);
DmReleaseResource (h);
return d;
}

Boolean DD () {
UInt32 u;
FtrGet ('psys', 24, &u);
if (u<4) return false;
WinScreenGetAttribute (winScreenDensity, &u);
return (u==kDensityDouble);
}

/***************************************/

void hede1 (UInt16 cardNo, LocalID dbID) {
DmOpenRef db= DmOpenDBNoOverlay (cardNo, dbID, dmModeReadOnly);
UInt16 ind= DmFindResource (db, 'code', 3, 0);
MemHandle h= DmGetResourceIndex (db, ind);
UInt32 siz= MemHandleSize (h);
void* p= MemPtrNew (siz);
void* o= MemHandleLock (h);
MemMove (p, o, siz);
MemHandleUnlock (h);
DmReleaseResource (h);
DmCloseDatabase (db);
MemPtrSetOwner (p, 0);
FtrSet (crea, _Fhandle, (UInt32)p);
SysNotifyRegister (cardNo, dbID, 'hede', p, HEDENOTIFYPRIORITY, 0);
}

void hede0 (UInt16 cardNo, LocalID dbID) {
void* f= 0;
SysNotifyUnregister (cardNo, dbID, 'hede', HEDENOTIFYPRIORITY);
if (FtrGet (crea, _Fhandle, (UInt32*)&f)) f= 0;
if (f!=0) {
	MemPtrFree (f);
	FtrSet (crea, _Fhandle, 0);
}
}

//postponed action
void POST () {
char* c= 0;
ExgSocketType s;
if (FtrGet (crea, _Furl, (UInt32*)&c)) return;
if (!c) return;
MemSet (&s, sizeof(ExgSocketType), 0);
s.name= c;
FtrUnregister (crea, _Furl);
if (ExgRequest (&s)) FrmAlert (2008);
MemPtrFree (c);
}

//register notifications
void RN () {
if (OS (0x05000000)) {
	UInt16* pre= LAP ();
	Boolean reg= ((pre[_Pb]!=0) || (pre[_Ps]!=0) || (pre[_Pe1x]!=0) || (pre[_Prb]!=0) || (pre[_Prs]!=0) || (pre[_Pre1x]!=0));
	UInt32 value;
	Boolean treo;
	UInt16 cardNo;
	LocalID dbID;
	if (pre[_Pdisabled]!=0) reg= false;
	MemPtrFree (pre);
	treo= ((!FtrGet ('hsEx', 0, &value)) && (value>=0x3523040));
	FtrSet (crea, _Ftreo, treo);
	SysCurAppDatabase (&cardNo, &dbID);
	hede0 (cardNo, dbID);
	if (reg) {
		hede1 (cardNo, dbID);
		SysNotifyRegister (cardNo, dbID, 'hapl', 0, 32, 0);
		SysNotifyRegister (cardNo, dbID, 'hapq', 0, 32, 0);
	} else {
		SysNotifyUnregister (cardNo, dbID, 'hapl', 32);
		SysNotifyUnregister (cardNo, dbID, 'hapq', 32);
	}
}
}

void SetImmediateAlarm (TAlarms alarm) {
UInt16 cardNo;
LocalID dbID;
SysCurAppDatabase (&cardNo, &dbID);
AlmSetAlarm (cardNo, dbID, alarm, TimGetSeconds()+1, true);
}

void PressHotSyncButton () {
UInt16 formid= FrmGetActiveFormID ();
if (formid==1000) {
	FormPtr F= FrmGetActiveForm ();
// 	UInt16 i;
// 	UInt16 noo= FrmGetNumberOfObjects (F);
// 	for (i=0; i<noo; i++) {
// 		FormObjectKind kind= FrmGetObjectType (F, i);
// 	}
	UInt16 obid= FrmGetObjectIndex (F, 1002);
	if (obid!=frmInvalidObjectId) {
		CtlHitControl (FrmGetObjectPtr (F, obid));
	}
}
}

void PA (SysAlarmTriggeredParamType* a) {
a->purgeAlarm= true;
switch (a->ref) {
case _AafterBoot:
	RAOS ();
	break;
case _Alasthotsync:
	PressHotSyncButton ();
	break;
}
}

/***************************************/

LocalID _1GFD (UInt32 type) {
DmSearchStateType ss;
UInt16 card;
LocalID id;
Err e= DmGetNextDatabaseByTypeCreator (true, &ss, type, crea, false, &card, &id);
if (e!=0) return 0;
return id;
}

UInt32* _1LAE (UInt16* exc, UInt16 w) {
UInt16 rSiz= 0;
UInt32* R;
PrefGetAppPreferences (crea, w, 0, &rSiz, true);
*exc= rSiz/4;
if (rSiz==0) return 0;
R= MemPtrNew (rSiz);
PrefGetAppPreferences (crea, w, R, &rSiz, true);
return R;
}

T1recent* _1LAR () {
T1recent* R= M0 (sizeof(T1recent)* _RC);
UInt16 rSiz= sizeof(T1recent)* _RC;
PrefGetAppPreferences (crea, 0, R, &rSiz, false);
if (rSiz!=sizeof(T1recent)* _RC)
	MemSet (R, sizeof(T1recent)* _RC, 0);
return R;
}

/***************************************/

UInt32 PilotMain (UInt16 cmd, void* cmdPBP, UInt16 launchFlags) {
switch (cmd) {
case sysAppLaunchCmdNotify:
	return PN (cmdPBP);
case 0x8000: case 0x8001: case 0x8002:
	AM (0, 0, 0, 0, 0);
	return 0;
case 0x8010: case 0x8011: case 0x8012:
	GRA ();
	return 0;
case 0x8020:
	POST ();
	return 0;
case sysAppLaunchCmdNormalLaunch:
	RN ();
	EL ();
	return 0;
case sysAppLaunchCmdCardLaunch:
	FtrSet (crea, _Fcardlaunch, 1);
	return 0;
case sysAppLaunchCmdSystemReset:
	SetImmediateAlarm (_AafterBoot);
	//nobreak;
case sysAppLaunchCmdSyncNotify:
	RN ();
	return 0;
case sysAppLaunchCmdAlarmTriggered:
	PA (cmdPBP);
	return 0;
}
return 0;
}
