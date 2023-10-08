//HOOKS THE SYSTEM AND DECIDES WHETHER TO POP UP THE MENU:

#define ALLOW_ACCESS_TO_INTERNALS_OF_CONTROLS

#include "HsPhoneMiscTypes.h"
#include "HsPhoneMisc.h"
#include "Hs.h"
#include "PalmTypes.h"
#include "Rect.h"
#include "Standalone.h"
#include "Form.h"
#include "NotifyMgr.h"
#include "KeyMgr.h"

#include "rns_hp.h"
#include "Hi-Launcher.h"

STANDALONE_CODE_RESOURCE_ID (3);

/***************************************/

void EE (EventType* e);

/***************************************/

//process notification
Err PN (SysNotifyParamType* n) {
if (n->notifyType=='hede')
	EE ((void*)(n->notifyDetailsP));
return 0;
}

/***************************************/

UInt16 BS16 (UInt16 n) {
return ((n>>8) | (n<<8));
//return ((((unsigned int) n) << 8) & 0xFF00) | ((((unsigned int) n) >> 8) & 0x00FF);
}

Int16 GP (UInt16 p) {
Int16 P[_PC];
UInt16 rSiz= _PC*2;
MemSet (&P, _PC*2, 0);
PrefGetAppPreferences (crea, 0, &P, &rSiz, true);
return (P[p]);
}

Boolean KD (Boolean hs) {
UInt32 U3[3];
//UInt32 kcs= KeyCurrentState ();
//return ((kcs!=0) && (kcs!=0x100));
Boolean kcs= ((KeyCurrentState () & (~0x100))!=0);
if ((!hs) || (kcs)) return kcs;
HsKeyCurrentStateExt ((UInt32*)&U3);
kcs= (U3[1] || U3[2]);
return kcs;
}

void RA () {
LocalID runvis; //officially running and actually visible
UInt16 card;
SysCurAppDatabase (&card, &runvis);

/*
UInt16 card;
UInt32 type;
DmOpenRef db= 0;
do {
	db= DmNextOpenResDatabase (db);
	if (db==0) return;
	DmOpenDatabaseInfo (db, &runvis, 0, 0, &card, 0);
	DmDatabaseInfo (card, runvis, 0, 0, 0, 0, 0, 0, 0, 0, 0, &type, 0);
} while ((type!='appl') && (type!='panl'));
*/

FtrSet (crea, _Frun, runvis);
FtrSet (crea, _Fvis, runvis);
FtrSet (crea, _Fkyo, 0);
}

void AR () {
UInt32 id= 0;
UInt32 card= 0;
UInt32 code= 0;
FtrGet (crea, _Fid, &id);
FtrGet (crea, _Fcard, &card);
FtrGet (crea, _Fcode, &code);
FtrUnregister (crea, _Fid);
FtrUnregister (crea, _Fcard);
FtrUnregister (crea, _Fcode);
if (id!=0) {
	SysUIAppSwitch (card, id, code, 0);
}
}

UInt16 PLF () {//twin
UInt16 n= 0;
Err err= SysLibFind ("GSMLibrary.lib", &n);
if (err) {
	err= SysLibFind ("PhoneLib.prc", &n);
	if (err) {
		err= SysLibLoad ('libr', 'GSM!', &n);
		if (err) {
			err= SysLibLoad ('libr', 'HsCL', &n);
			if (err) {
				HsGetPhoneLibrary (&n);
			}
		}
	}
}
return n;
}

Boolean OK () {
UInt32 u;
if (!FtrGet (crea, _Fon, &u)) return false;
if ((!FtrGet (crea, _Ftreo, &u)) && (u!=0)) {
	UInt16 refnum;
	HsAttrGet (0x88, 0, &u);//keyguard
	if (u) return false;
	refnum= PLF ();
	PhnLibGetPhoneCallStatus (refnum, &u);
	if (u & (phnCall1StatusIncoming | phnCall2StatusIncoming)) return false;
}
if (PrefGetPreference (prefDeviceLocked)!=0) {
	if ((!FtrGet ('TlLk', 12345, &u)) && (u)) {//TealLock locked
		if (SysUIBusy (false, false)) {
			return false;
		}
	} else {
		return false;
	}
}
return true;
}

Boolean ISOFF () {
UInt32 u;
if ((!FtrGet (crea, _Ftreo, &u)) && (u!=0)) {
	HsAttrGet (0x82, 0, &u);//screenon
	return !u;
}
return false;
}

/*
KEY:
2	- trigger! (5-Way Navigator)
1	- trigger!
0	- nie trafiony - sprawdz inne triggery lub jesli to
	  ostatni - przepusc event do systemu
-1	- przepusc event do systemu
-2	- usun event
*/
Int16 LNC1 (UInt16 key, UInt16 kc, UInt16 mod, UInt16 af, UInt16 ap) {
UInt32 u;
UInt32 bb, ss, hh;
if (FtrGet (crea, _Fb+af, &bb)!=0) {
	bb= GP (_Pb+ap);
	FtrSet (crea, _Fb+af, bb);
}
if (FtrGet (crea, _Fs+af, &ss)!=0) {
	ss= GP (_Ps+ap);
	FtrSet (crea, _Fs+af, ss);
}
if (FtrGet (crea, _Fhold+af, &hh)!=0) {
	hh= GP (_Phold+ap);
	FtrSet (crea, _Fhold+af, hh);
}
if (bb==0x0136) { //v.3.3 => v.3.4 - 5way center na T650&up dawniej w HL byl lapany przez 0x0136 a teraz przez 0x013D, wiec jak ktos ma stare ustawienie, to przeksztalcamy to na nowe (ale nie zapisujemy, bo sie nie da latwo):
	UInt32 value;
	Boolean treo600= ((!FtrGet (sysFtrCreator, sysFtrNumOEMDeviceID, &value)) && (value=='H101'));
	if (!treo600) { //if not T600
		bb= 0x013D;
		FtrSet (crea, _Fb+af, bb);
	}
}
if ((((key==0x0136) && (kc!=0x0136)) || (key==0x0128)) && (!FtrGet (crea, _FignoreCenterUp, &u))) {
	FtrUnregister (crea, _FignoreCenterUp);
	return -2;
}
if (key==bb) {
	if (!OK ()) {
		if ((!hh) && (ISOFF ())) {
			if (key==0x0208) return -1;
			EvtEnqueueKey (0x0208, 0x0208, 0x0908);
			return -2;
		}
		return false;
	}
	if (key==0x160D) {
		UInt32 ok_kc;
		UInt16 kc2= BS16 (kc);
		if (FtrGet (crea, _Fkc+af, &ok_kc)!=0) {
			ok_kc= GP (_Pkc+ap);
			ok_kc&= 0xFFFF;
			FtrSet (crea, _Fkc+af, ok_kc);
		}
/*{
char c[9]; StrIToH (&c, mod);
char d[9]; StrIToH (&d, 0);
RectangleType r= {0, 0, 160, 22};
RectangleType r2= {0, 0, 160, 23};
WinDrawRectangle (&r2, 0);
SysTaskDelay (20);
WinEraseRectangle (&r, 0);
WinDrawChars (&c, 8, 0, 0);
WinDrawChars (&d, 8, 0, 11);
SysTaskDelay (200);
WinEraseRectangle (&r, 0);
}*/
		if (kc2!=ok_kc) return false;
		if (mod & 0x0828 & 0x0080) {
			FtrSet (crea, _Fignore1, 0x160D);
		}
	}
	if (key==0x0503) {
		UInt32 ok_kc;
		UInt16 kc2= BS16 (kc);
		UInt16 ok_kc2;
		if (FtrGet (crea, _Fkc+af, &ok_kc)!=0) {
			ok_kc= GP (_Pkc+ap);
			ok_kc&= 0x1F00;
			FtrSet (crea, _Fkc+af, ok_kc);
		}
		ok_kc2= ok_kc;
		if (kc2 & ok_kc2) { //normal, use kc2:
			UInt32 dummy;
			if (hh!=0) return false;
			if (kc2 & 0x001F) {
				if (kc2==0x1010)
					return false;
				return -2;
			}
			if ((!FtrGet (crea, _Fignore5way, &dummy)) && (dummy & kc2)) {
				FtrUnregister (crea, _Fignore5way);
				return false;
			}
			return true;
		}
		if (kc & ok_kc2) { //hold, use kc:
			if (!(mod & 0x40)) return false;
			if (hh==0) {
				FtrSet (crea, _Fignore5way, kc);
				return false;
			}
			return 2;
		}
		return false;
	}
	if (mod & 0x0040) return false;
	/* VAR */ {
		UInt32 delay;
		UInt16 delay16;
		UInt32 del;
		UInt32 treo;
		if (FtrGet (crea, _Fdelay, &delay)!=0) {
			delay= GP (_Pdelay);
			FtrSet (crea, _Fdelay, delay);
		}
		delay16= delay;
		del= (36+ ((Int16)delay16*4))* SysTicksPerSecond ()/ 100;
		u= TimGetTicks ();
		if (FtrGet (crea, _Ftreo, &treo)) treo= 0;
		while (KD (treo)) {
			UInt32 ticks= TimGetTicks ()- u;
			if (ticks>=del) {
				//did press-and-hold
				if (FtrGet (crea, _Fsametriggers, &u)!=0) {
					u= GP (_Pb) == GP (_Prb);
					FtrSet (crea, _Fsametriggers, u);
				}
				if (!u) return true;
				return hh;
			}
		}
		//did press (not hold)
		return !hh;
	}
	return true;
}
if (key==ss) {
	UInt32 delay;
	UInt16 delay16;
	UInt32 del;
	UInt32 ticks;
	if (!OK ()) return false;
	if (FtrGet (crea, _Fdelay, &delay)!=0) {
		delay= GP (_Pdelay);
		FtrSet (crea, _Fdelay, delay);
	}
	delay16= delay;
	del= (32+ ((Int16)delay16*4))* SysTicksPerSecond ()/ 100;
	FtrGet (crea, _Fpd, &u);
	ticks= TimGetTicks ()- u;
	if (ticks<del) {
		//did tap (not hold)
		return !hh;
	}
	//did tap-and-hold
	if (FtrGet (crea, _Fsametriggers, &u)!=0) {
		u= GP (_Ps) == GP (_Prs);
		FtrSet (crea, _Fsametriggers, u);
	}
	if (!u) return true;
	return hh;
}
return false;
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

//STROKE:
Boolean LNC2 (PointType *p1, PointType *p2, UInt16 af, UInt16 ap) {
PointType e1, e2;
UInt32 bases;
if (FtrGet (crea, _Fe1+af, (UInt32*)&e1)) {
	e1.x= GP (_Pe1x+ap);
	if (e1.x==0) return false;
	e1.y= GP (_Pe1y+ap);
	e2.x= GP (_Pe2x+ap);
	e2.y= GP (_Pe2y+ap);
	bases= GP (_Pbases+ap);
	FtrSet (crea, _Fe1+af, *(UInt32*)&e1);
	FtrSet (crea, _Fe2+af, *(UInt32*)&e2);
	FtrSet (crea, _Fbases+af, bases);
} else {
	FtrGet (crea, _Fe2+af, (UInt32*)&e2);
	FtrGet (crea, _Fbases+af, &bases);
}
if (bases!=0) {
	Int16 SW;
	Int16 SH;
	SWSH (&SW, &SH);
	if (bases & 1) {
		e1.y= SH- e1.y;
		e2.y= SH- e2.y;
	}
	if (bases & 2) {
		e1.x= SW- e1.x;
		e2.x= SW- e2.x;
	}
}
{
RT r1= {{e1.x-16, e1.y-16}, {32, 32}};
RT r2= {{e2.x-16, e2.y-16}, {32, 32}};
if ((RctPtInRectangle (p1->x, p1->y, &r1)) && (RctPtInRectangle (p2->x, p2->y, &r2))) {
	return OK ();
}
return false;
}
}

void CallAddToRecent (UInt16 cardNo, LocalID dbID) {
UInt32 result;
SysNotifyParamType notify;
SysNotifyAppLaunchOrQuitType a;
LocalID hil;
DmSearchStateType ss;
UInt16 card;
Err err= DmGetNextDatabaseByTypeCreator (true, &ss, 'appl', crea, false, &card, &hil);
if ((err!=0) || (hil==0)) return;
notify.notifyType= 'hapq';
notify.notifyDetailsP= &a;
a.cardNo= cardNo;
a.dbID= dbID;
SysAppLaunch (card, hil, 0, sysAppLaunchCmdNotify, (void*)&notify, &result);
}

//event (dependent endianness)
void EE (EventType* e) {
UInt16 lc= 0x8002;
switch (e->eType) {
case 0x0200: { //bs penUpEvent
	PointType p1, p2;
	p1.x= BS16 (e->data.penUp.start.x);
	p1.y= BS16 (e->data.penUp.start.y);
	p2.x= BS16 (e->data.penUp.end.x);
	p2.y= BS16 (e->data.penUp.end.y);
	if (LNC2 (&p1, &p2, 0, 0)) {
		FtrSet (crea, _Ftrig, 0);
		goto popup;
	}
	if (LNC2 (&p1, &p2, 5, 7)) {
		lc= 0x8012;
		goto popup;
	}
	}
	return;
case 0x1600: { //bs appStopEvent
	SysAppInfoPtr ai1, ai2;
	SysGetAppInfo (&ai1, &ai2);
	if (ai1 && (ai1->launchFlags & sysAppLaunchFlagUIApp)) { //sysAppLaunchFlagUIApp sysAppLaunchFlagSubCall sysAppLaunchFlagNewGlobals
		UInt16 cardNo;
		LocalID dbID;
		if (!DmOpenDatabaseInfo (ai1->dbP, &dbID, 0, 0, &cardNo, 0)) {
			UInt32 type, creator;
			DmDatabaseInfo (cardNo, dbID, 0, 0, 0, 0, 0, 0, 0, 0, 0, &type, &creator);
			if (type=='panl') {
				type= 'appl';
				creator= 'pref';
			}
			if (type=='appl') {
				UInt32 lastCreator;
				if (FtrGet (crea, _FlastCreator, &lastCreator) || (lastCreator!=creator)) {
					CallAddToRecent (cardNo, dbID);
				}
			}
		}
	}
	}
	return;
case 0x0100: //bs penDownEvent
	FtrSet (crea, _Fpd, TimGetTicks ());
	//nobreak
default:
	return;
case 0x0400: //bs keyDownEvent
}

/*{
char c[9]; StrIToH (&c, BS16(e->data.keyDown.chr));
char d[9]; StrIToH (&d, BS16(e->data.keyDown.modifiers));
RectangleType r= {0, 0, 160, 22};
RectangleType r2= {0, 0, 160, 23};
WinDrawRectangle (&r2, 0);
SysTaskDelay (10);
WinEraseRectangle (&r, 0);
WinDrawChars (&c, 8, 0, 0);
WinDrawChars (&d, 8, 0, 11);
SysTaskDelay (90);
WinEraseRectangle (&r, 0);
}*/

/* VAR*/ {
	UInt16 key= BS16 (e->data.keyDown.chr);
	UInt16 cod= BS16 (e->data.keyDown.keyCode);
	UInt16 mod= BS16 (e->data.keyDown.modifiers);
	UInt32 dummy= 0;
	Int16 in;
	if (key==0) return;
	if (!(mod & 0x0008)) return;
	if (key==0x012D) return;
	if ((key==0x0208) && (cod==0x0207)) key= 0x0207;
	FtrGet (crea, _Fnorepeat, &dummy);
	if (dummy==key) {
		if (mod & 0x0040) goto ignore;
	} else if (dummy!=0) FtrUnregister (crea, _Fnorepeat);
	if ((!FtrGet (crea, _Fignore1, &dummy)) && (dummy==key)) {
		FtrUnregister (crea, _Fignore1);
		return;
	}
	
	in= LNC1 (key, e->data.keyDown.keyCode, mod, 0, 0); //whether Menu trigger
	if (in==-1) return;
	if (in==-2) goto ignore;
	if (in==0) {
		in= LNC1 (key, e->data.keyDown.keyCode, mod, 5, 7); //whether Recent App. trigger
/*{
char c[9]; StrIToH (&c, key);
char d[9]; StrIToH (&d, mod);
RectangleType r= {0, 0, 160, 22};
RectangleType r2= {0, 0, 160, 23};
WinDrawRectangle (&r2, 0);
SysTaskDelay (20);
WinEraseRectangle (&r, 0);
WinDrawChars (&c, 8, 0, 0);
WinDrawChars (&d, 8, 0, 11);
SysTaskDelay (200);
WinEraseRectangle (&r, 0);
}*/
		if ((in==-1) || (in==0)) return;
		if (in==-2) goto ignore;
		if (in==2) {
			while (KD (false));
			EvtFlushKeyQueue ();
			FtrUnregister (crea, _Fignore5way);
		} else FtrSet (crea, _Fnorepeat, key);
		lc= 0x8012;
	} else {
		if (key==0x013D) {
			FtrSet (crea, _Ftrig, 0x0136);
		} else {
			FtrSet (crea, _Ftrig, key);
		}
	}
	if (key==0x013D) {
		FtrSet (crea, _FignoreCenterUp, 1);
	}
}

popup:
/* VAR */ {
	LocalID hil;
	DmSearchStateType ss;
	UInt16 card;
	UInt32 u;
	Err err= DmGetNextDatabaseByTypeCreator (true, &ss, 'appl', crea, false, &card, &hil);
	if ((err!=0) || (hil==0)) return;
	RA ();
	SysAppLaunch (card, hil, 0, lc, (void*)e, &u);
	AR ();
	if (!FtrGet (crea, _Fnoignore, &u)) {
		FtrUnregister (crea, _Fnoignore);
		return;
	}
	ignore:
	// if (!FtrGet (crea, _Fcalled, &u)) {
	// 	FtrUnregister (crea, _Fcalled);
	// 	if ((!FtrGet (crea, _Ftreo, &u)) && (u!=0)) {
	// 		UInt16 refnum= PLF ();
	// 		PhnLibGetPhoneCallStatus (refnum, &u);
	// 		if (u & (phnVoiceCall1Active | phnVoiceCall2Active)) return;
	// 	}
	// }
	e->eType= 0; //nilEvent
}
}

