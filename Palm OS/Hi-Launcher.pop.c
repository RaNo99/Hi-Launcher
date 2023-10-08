//MENU SHOULD POP UP:

#define use_HPMoveGSI

#define ALLOW_ACCESS_TO_INTERNALS_OF_BITMAPS
#define ALLOW_ACCESS_TO_INTERNALS_OF_FIELDS

#include "Hs.h"
#include "HsPhonePower.h"
#include "HsPhoneLibrary.h"
#include "PalmTypes.h"
#include "Rect.h"
#include "NotifyMgr.h"
#include "FeatureMgr.h"
#include "Bitmap.h"
#include "SonyCLIE.h"
//#include "SonySilkLib.h"
//#include "SonyHighResLib.h"
//#include "rns_dd.h"
#include "VFSmgr.h"
#include "ExgLib.h"
#include "Form.h"
#include "HelperServiceClass.h"
#include "pmkeylib/PmKeyLibCommon.h"
#include "pmkeylib/PmKeyLib.h"
#include "PalmOneFeatures.h"

#include "rns_hp.h"
#include "Hi-Launcher.h"

/***************************************/

void LC (TPOP* POP, Int16 ind);
Boolean LNCH (UInt32 creator, UInt32 type);
/*UInt32 DIV (UInt32 a, UInt32 b);
UInt32 MUL (UInt32 a, UInt32 b);*/

/***************************************/

UInt16 HR () {
SonySysFtrSysInfoP sonySysFtrSysInfoP;
UInt16 refNum;
UInt32 width;
UInt32 height;
UInt32 depth;
Boolean enableColor;
if (FtrGet ('SoNy', 1, (UInt32*)&sonySysFtrSysInfoP))
	return 0xFFFF;
if (!(sonySysFtrSysInfoP->libr & 1))
	return 0xFFFF;
if (SysLibFind ("Sony HR Library", &refNum))
	return 0xFFFF;
HRWinScreenMode (refNum, winScreenModeGet, &width, &height, &depth, &enableColor);
if (width>=320) return refNum;
return 0xFFFF;
}

Boolean BROP (Boolean set, Boolean value) {
UInt16 beam_ir_ref;
Boolean beamPref= value;
UInt16 beamLen = sizeof(beamPref);
if (SysLibFind ("IrDA Library", &beam_ir_ref)) return false;
ExgLibControl (beam_ir_ref, (0x8000 | (1+set)), &beamPref, &beamLen);
return beamPref;
}

Boolean GBRS () {
return BROP (false, false);
}

void SBRS (Boolean s) {
BROP (true, s);
}

Boolean GBLS () {
UInt32 treo= 0;
FtrGet (crea, _Ftreo, &treo);
if (treo) {
	UInt16 old;
	Boolean color= true;
	HsLightMode (false, &old);
	WinScreenMode (winScreenModeGetSupportedDepths, 0, 0, 0, &color);
	if (color) {//tryb nocny przyciemnia a nie w³¹cza œwiat³o
		old= !old;
	}
	return old;
} else {
	Boolean old;
	HwrDisplayAttributes (false, 0x12, &old);
	return old;
}
}

Boolean SBLS () {//returns true if non-treo
UInt32 treo= 0;
FtrGet (crea, _Ftreo, &treo);
if (treo) {
	UInt16 old;
	HsLightMode (false, &old);
	old= !old;
	HsLightMode (true, &old);
}
return !treo;
}

Boolean T650 () {
UInt32 value;
return ((!FtrGet ('hsEx', 0, &value)) && (value>=0x5300000));
}

Boolean GKLS () {
if (T650 ()) {
	return !(HsCurrentLightCircumstance () & hsLightCircumstanceKeylightOffFlagBit);
}
return false;
}

Boolean SKLS (Boolean s) {
if (T650 ()) {
	HsLightCircumstance (!s, hsLightCircumstanceKeylightOff);
	return true;
} else {
	UInt32 treo= 0;
	FtrGet (crea, _Ftreo, &treo);
	if (treo) {
		FrmAlert (2016);
	} else {
		FrmAlert (2013);
	}
	return false;
}
}

Boolean GAOS () {
UInt16 old= SysSetAutoOffTime (0);
SysSetAutoOffTime (old);
SP (_PautooffD, !old);
return (!!old);
}

void SAOS (TPOP* POP) {
UInt16 old= SysSetAutoOffTime (0);
if (old==0) {
	UInt16 new= POP->autooffV;
	if (new==0) new= 2* 60;
	SysSetAutoOffTime (new);
} else {
	SP (_PautooffV, old);
}
SP (_PautooffD, !!old);
}

void RAOS () {
if (GP (_PautooffD)) {
	UInt16 old= SysSetAutoOffTime (0);
	if (old!=0) {
		SP (_PautooffV, old);
	}
}
}

Boolean GSSS () {
UInt32 old= PrefGetPreference (prefSysSoundVolume);
return (old!=0);
}

void SSSS (TPOP* POP) {
UInt16 old= PrefGetPreference (prefSysSoundVolume);
UInt16 new= 0;
if (old==0) {
	new= POP->soundV;
	if (new==0) new= 64;
} else {
	SP (_PsoundV, old);
}
new*=16;
SndSetDefaultVolume (0, &new, 0);//not allowed on OS5? works :-)
new/=16;
PrefSetPreference (prefSysSoundVolume, new);
if (new!=0) SndPlaySystemSound (7);
}

Boolean GBTS () {
UInt16 cardNo; 
LocalID dbID;
DmSearchStateType ss;
Boolean b= false;
UInt32 result;
Err e= DmGetNextDatabaseByTypeCreator (true, &ss, 0, 'BTTG', true, &cardNo, &dbID);
if (e) return false;
SysAppLaunch (cardNo, dbID, 0, 0x8000+3, &b, &result);
return b;
}

Boolean SBTS (TPOP* POP) {
UInt16 cardNo;
LocalID dbID;
DmSearchStateType ss;
UInt32 result;
Err e= DmGetNextDatabaseByTypeCreator (true, &ss, 0, 'BTTG', true, &cardNo, &dbID);
if (e) {
	FrmAlert (2005);
	return false;
}
SysAppLaunch (cardNo, dbID, 0, 0x8000+2, 0, &result);
return true;
}

Boolean NLF (UInt16* n) {
UInt32 val;
Err err= FtrGet ('netl', 0, &val);
if ((err) || (!val)) return false;
err= SysLibFind ("Net.lib", n);
return (!err);
}

Boolean GNWS (UInt16 N, Boolean isN) {
UInt16 n= N;
Err err;
if ((!isN) && (!NLF (&n))) return false;
err= NetLibOpenIfCloseWait (n);
if ((!err) || (err==(0x1200 | 1))) {
	NetMasterPBType pb;
	MemSet (&pb, sizeof(NetMasterPBType), 0);
	err= NetLibMaster (n, netMasterInterfaceInfo, &pb, 0);
	NetLibClose (n, false);
	if (err!=0) return false;
	return (pb.param.interfaceInfo.driverUp && pb.param.interfaceInfo.ifUp);
}
return false;
}

void SNWS (TPOP* POP) {
UInt16 n;
if (!NLF (&n)) return;
if (GNWS (n, true)) {
	NetLibFinishCloseWait (n);
	if (GNWS (n, true)) {
		UInt16 i;
		if (FrmAlert (2007)) return;
		for (i= 0; i<100; i++) {
			UInt32 cr;
			UInt16 inst;
			if (NetLibIFGet (n, i, &cr, &inst)) break;
			NetLibIFDown (n, cr, inst, SysTicksPerSecond ()* 5);
		}
		NetLibFinishCloseWait (n);
	}
} else {
	UInt16 dum;
	Err err= NetLibOpen (n, &dum);
	Boolean imm= false;
	if ((!err) || (err==(0x1200 | 1)))
		if (dum) imm= true;
	NetLibClose (n, imm);
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

Boolean GRDS () {
UInt32 treo= 0;
FtrGet (crea, _Ftreo, &treo);
if (treo) {
	UInt16 gLibRef= PLF ();
	PhnPowerType power= phnPowerOff;
	PhnLibOpen (gLibRef);
	power= PhnLibModulePowered (gLibRef);
	PhnLibClose (gLibRef);
	return !!power;
}
return false;
}

Boolean SRDS (TPOP* POP) {
UInt32 treo= 0;
FtrGet (crea, _Ftreo, &treo);
if (treo) {
	UInt16 gLibRef= PLF ();
	PhnPowerType power= phnPowerOff;
	PhnLibOpen (gLibRef);
	power= PhnLibModulePowered (gLibRef);
	if (power==0) {
	        PhnLibSetModulePower (gLibRef, 1);
	}
	if (power==1) {
	        PhnLibSetModulePower (gLibRef, 0);
	}
	PhnLibClose (gLibRef);
	return true;
}
FrmAlert (2013);
return false;
}

UInt32 GetAppCreator (DmOpenRef db) {
UInt16 cardNo;
LocalID dbID;
if (!DmOpenDatabaseInfo (db, &dbID, 0, 0, &cardNo, 0)) {
	UInt32 type, creator;
	DmDatabaseInfo (cardNo, dbID, 0, 0, 0, 0, 0, 0, 0, 0, 0, &type, &creator);
	if (type=='panl') {
		return 'pref';
	}
	if (type=='appl') {
		return creator;
	}
}
return 0;
}

UInt32 GetCurrentUIAppCreator () { //todo: if ARM UI app is running, and something pops-up over it (e.g. RNS:: HBX) then the popup is mistakenly returned
DmOpenRef db= 0;
Boolean skippedHL= false;
SysAppInfoPtr ai1, ai2;
SysGetAppInfo (&ai1, &ai2);
if (ai1 && (ai1->launchFlags & sysAppLaunchFlagUIApp)) { //sysAppLaunchFlagUIApp sysAppLaunchFlagSubCall sysAppLaunchFlagNewGlobals
	UInt32 creator= GetAppCreator (ai1->dbP);
	if (creator) return creator;
}
while (db= DmNextOpenResDatabase (db)) {
	UInt32 creator= GetAppCreator (db);
	if (creator) {
		if ((creator!=crea) || skippedHL) {
			return creator;
		}
		skippedHL= true;
	}
};
return 0;
}

/*UInt32* ODBS (UInt16* _odbsc) {
UInt32* odbs= MemPtrNew (16*sizeof(UInt32));
UInt16 odbsc= 0;
DmOpenRef db= 0;
Boolean force= false;
do {
	LocalID id;
	UInt16 mode;
	UInt16 card;
	UInt32 type;
	UInt32 creator;
	db= DmNextOpenResDatabase (db);
	if (!db) break;
	if (DmOpenDatabaseInfo (db, &id, 0, &mode, &card, 0)) break;
	if (mode & dmModeLeaveOpen)
		continue;
	DmDatabaseInfo (card, id, 0, 0, 0, 0, 0, 0, 0, 0, 0, &type, &creator);
	if (type=='panl') odbs[odbsc++]= 'pref';
	if (type!='appl') continue;
	if (!force) {
		force= true;
		continue;
	}
	odbs[odbsc++]= creator;
} while (odbsc<16);
*_odbsc= odbsc;
return odbs;
}*/

UInt16 CW (Boolean small, FontID font, char* c) {
FontID old;
UInt16 w;
if (small) font= (font*5)+ 2;
old= FntSetFont (font);
w= FntCharsWidth (c, StrLen (c));
FntSetFont (old);
if (small) w>>= 1;
return w;
}

UInt32 GetRecentApp () {
UInt32 currentCreator= GetCurrentUIAppCreator ();
T1recent* RR= _1LAR ();
UInt32 creator= 0;
Int16 i;
for (i= 0; i<_RC; i++) {
	if ((RR[i].creator==0) || (RR[i].creator==currentCreator)) {
		continue;
	}
	creator= RR[i].creator;
	break;
}
MemPtrFree (RR);
return creator;
}

void GRA () {
UInt32 creator;
SndPlaySystemSound (7);
creator= GetRecentApp ();
if (creator) {
	LNCH (creator, 'appl');
} else {
	FrmAlert (2002);
}
}

Boolean KD () { //consider converting this one to the same version as KD(...) in Hi-Launcher.pop.2.c
//UInt32 kcs= KeyCurrentState ();
//return ((kcs!=0) && (kcs!=0x100));
return ((KeyCurrentState () & (~0x100))!=0);
}

void WaitNoKeyDown () {
while (KD ());
EvtFlushKeyQueue ();
}

UInt16 WaitAlert (UInt16 id) {
WaitNoKeyDown ();
return FrmAlert (id);
}

/***************************************/

Boolean IM_ismenu (TPOP* POP) {
LocalID id= _1GFD ('Data');
UInt32 f;
POP->lastfocus= 0xFFFF;
if (id==0) {
	nomenu:
	f= 0;
	FtrGet (crea, _Fset, &f);
	if ((f==0) && (!WaitAlert (2000))) LC (POP, -2);
	return false;
}
POP->db= DmOpenDBNoOverlay (0, id, dmModeReadOnly);
POP->m= DmGet1Resource ('tMEN', POP->mid);
if (POP->m==0) goto nomenu;
POP->M= MemHandleLock (POP->m);
POP->cnt= POP->M->data.header.count;
if (POP->cnt==0) goto nomenu;
if (POP->M->data.header.version>MENUVERSION) {
	WaitAlert (2014);
	return false;
}
if (POP->M->data.header.version<MENUVERSION) {
	major:
	if (!WaitAlert (2003)) {
		UInt32 f= 0;
		FtrGet (crea, _Fset, &f);
		FtrSet (crea, _Fupdate, 1);
		if (f==0) LC (POP, -2);
	}
	return false;
}
id= _1GFD ('Swap');
if (id==0) goto major;
POP->swp= DmOpenDBNoOverlay (0, id, dmModeReadOnly);
return true;
}

UInt16 IM_getmaxcw (TPOP* POP, UInt16 hr) {
UInt32 depth;
UInt16* P;
UInt16 c_g, c_r, c_d;
Int16 i;
UInt16 maxcw;
POP->hires= hr;
WinGetDisplayExtent (&POP->sw, &POP->sh);
WinScreenMode (winScreenModeGet, 0, 0, &depth, 0);
POP->color= depth>>3;
FtrGet (sysFtrCreator, sysFtrNumROMVersion, &(POP->OS));
POP->b_d= SysTicksPerSecond ()* (20+ (SysRandom (0)%80));
P= LAP ();
POP->fc= 255- P[_Pfc];
POP->f.word= P[_Pf];
POP->hsn= P[_Phsn]+ 14;
if ((hr==0xFFFF) && (POP->hsn<11)) POP->hsn= 11;
POP->small= (POP->hsn<11);
POP->si= ((!P[_Psi]) && (!POP->M->data.header.noicons));
POP->asi= (!P[_Poie]);
POP->ani= P[_Pani];
POP->anis= P[_Panis]+ 4;
POP->whbg= P[_Pwhbg];
POP->rev= P[_Prev];
POP->und= (!P[_Pund]);
POP->autohide= P[_Pautohide]*SysTicksPerSecond ();
POP->autooffV= P[_PautooffV];
POP->soundV= P[_PsoundV];
POP->chpal= P[_Pchpal];
POP->via= P[_Pvia];
POP->highlight= P[_Phighlight];
POP->doubletp= P[_Pdoubletp];
POP->excluderedial.word= P[_Pexcluderedial];
MemPtrFree (P);

if (POP->mid==0) {
	UInt32 treo= 0;
	if (POP->chpal) {
		RGBColorType* WP;
		POP->P= MemHandleNew (256* sizeof (RGBColorType));
		WP= MemHandleLock (POP->P);
		if (WinPalette (0, 0, 256, WP) || WinPalette (2, 0, 256, 0)) {
			MemHandleFree (POP->P);
			POP->P= 0;
		} else MemHandleUnlock (POP->P);
	}
	FtrGet ('QCMT', 2, &(POP->kyoceraOkOff));
	FtrGet (crea, _Ftreo, &treo);
	if (treo) {
		SysNotifyParamType not= {sysNotifyExternalDialogOpenedEvent, crea, 0, 0, 0};
		SysNotifyBroadcast (&not);
	}
}

c_g= POP->M->data.header.count_g;
if (c_g!=0) POP->gg= M0 (c_g* sizeof(char*));

c_r= POP->M->data.header.count_r;//recent
if (c_r!=0) {
	UInt32 currentCreator= GetCurrentUIAppCreator ();
	Int16 ri= POP->ri;
	POP->MRU_RA= _1LAR ();
	POP->MRU_RS= M0 (c_r*sizeof(T1recent*));
	for (i= 0; i<c_r; i++) {
		UInt32 creator;
		UInt16 ind;
		dobegin:
		if (ri>=_RC) goto fillend;
		creator= POP->MRU_RA[ri++].creator;
		if ((creator==0) || (creator==currentCreator)) goto dobegin;
		ind= i;
		if (POP->rev) ind= c_r- i- 1;
		POP->MRU_RS[ind]= (T1recent*)&(POP->MRU_RA[ri- 1]);
	}
	fillend:
	POP->riu= ri;
}

c_d= POP->M->data.header.count_d;//redial
if (c_d!=0) {
	Int16 di= POP->di;
	UInt16 ind;
	POP->MRU_DA= LAD (POP->excluderedial);
	POP->MRU_DS= M0 (c_d*sizeof(T1redial));
	for (i= 0; i<c_d; i++) {
		dobegin2:
		if (di>=_RC) goto fillend2;
		if (POP->MRU_DA[di++].number==0) goto dobegin2;
		ind= i;
		if (POP->rev) ind= c_d- i- 1;
		POP->MRU_DS[ind]= POP->MRU_DA[di- 1];
	}
	fillend2:
	POP->diu= di;
}

if (POP->small)
	maxcw= POP->M->data.header.maxcw2; else
	maxcw= POP->M->data.header.maxcw;

if (c_r!=0) {
	for (i= 0; i<POP->cnt; i++) if ((POP->M[i+1].type==_recent) && (POP->M[i+1].fcc==0)) {
		char* c= S_RECENT;
		UInt16 w;
		if (POP->MRU_RS[POP->M[i+1].data.body.index]!=0)
			c= (char*)&(POP->MRU_RS[POP->M[i+1].data.body.index]->name);
		w= CW (POP->small, POP->M[i+1].font, c);
		if (w>maxcw) maxcw= w;
	}
}
if (c_d!=0) {
	for (i= 0; i<POP->cnt; i++) if ((POP->M[i+1].type==_redial) && (POP->M[i+1].fcc==0)) {
		char* c= S_REDIAL;
		UInt16 w;
		if (POP->MRU_DS[POP->M[i+1].data.body.index].name!=0)
			c= POP->MRU_DS[POP->M[i+1].data.body.index].name;
		w= CW (POP->small, POP->M[i+1].font, c);
		if (w>maxcw) maxcw= w;
	}
}
if (c_g!=0) for (i= 0; i<POP->cnt; i++) if ((POP->M[i+1].type>=_0_status) && (POP->M[i+1].type<_x_status)) {
	char* oc= GC ('tSTR', POP->M[i+1].cid);
	char* nc= MemPtrNew (128);
	char* c1= 0;
	char* c2= 0;
	char* c3= 0;
	char* c4= 0;
	char* d1= 0;
	char* d2= 0;
	char* d3= 0;
	char* d4= 0;
	char* d= nc;
	char* c;
	UInt16 w;
	POP->gg[POP->M[i+1].data.body.index]= nc;
	switch (POP->M[i+1].type) {
	case _battery: {
		UInt8 p;
		UInt16 v= SysBatteryInfo (false, 0, 0, 0, 0, 0, &p);
		c1= StrStr (oc, "^P");
		c2= StrStr (oc, "^V");
		if (c1!=0) {
			d1= MemPtrNew (8);
			StrIToA (d1, p);
		}
		if (c2!=0) {
			NumberFormatType nf= PrefGetPreference (prefNumberFormat);
			UInt16 kp;
			d2= MemPtrNew (8);
			StrIToA (d2, v);
			kp= StrLen (d2)- 2;
			MemMove (&(d2[kp+ 1]), &(d2[kp]), 3);
			if (nf%3==0) d2[kp]= '.';
				else d2[kp]= ',';
		}
		break; }
	case _memory: {
		UInt32 free, max, total, used;
		UInt16 id= MemHeapID (0, 1);
		MemHeapFreeBytes (id, &free, &max);
		total= MemHeapSize (id);
		used= total- free;
		c1= StrStr (oc, "^T");
		c2= StrStr (oc, "^F");
		c3= StrStr (oc, "^U");
		c4= StrStr (oc, "^P");
		if (c1!=0) {
			d1= MemPtrNew (8);
			StrIToA (d1, total>>10);
		}
		if (c2!=0) {
			d2= MemPtrNew (8);
			StrIToA (d2, free>>10);
		}
		if (c3!=0) {
			d3= MemPtrNew (8);
			StrIToA (d3, used>>10);
		}
		if (c4!=0) {
			UInt32 f32= free>>17;
			UInt16 f16= f32;
			UInt32 t32= total>>19;
			UInt16 t16= t32;
			d4= MemPtrNew (8);
			StrIToA (d4, (f16* 25)/t16);
		}
		break; }
	case _datetime: {
		DateTimeType dt;
		c1= StrStr (oc, "^T");
		c2= StrStr (oc, "^D");
		c3= StrStr (oc, "^W");
		TimSecondsToDateTime (TimGetSeconds (), &dt);
		if (c1!=0) {
			TimeFormatType tf= PrefGetPreference (prefTimeFormat);
			d1= MemPtrNew (32);
			TimeToAscii (dt.hour, dt.minute, tf, d1);
		}
		if (c2!=0) {
			DateFormatType tf= PrefGetPreference (prefLongDateFormat);
			d2= MemPtrNew (64);
			DateToAscii (dt.month, dt.day, dt.year, tf, d2);
		}
		if (c3!=0) {
			d3= MemPtrNew (64);
			DateTemplateToAscii ("^1l", dt.month, dt.day, dt.year, d3, 64);
		}
		break; }
	}
	for (c= oc; *c!=0; c++) {
		if (c==c1) {
			c++;
			StrCopy (d, d1);
			d+= StrLen (d1);
		} else if (c==c2) {
			c++;
			StrCopy (d, d2);
			d+= StrLen (d2);
		} else if (c==c3) {
			c++;
			StrCopy (d, d3);
			d+= StrLen (d3);
		} else if (c==c4) {
			c++;
			StrCopy (d, d4);
			d+= StrLen (d4);
		} else {
			*d= *c;
			d++;
		}
	}
	*d= 0;
	if (d1!=0) MemPtrFree (d1);
	if (d2!=0) MemPtrFree (d2);
	if (d3!=0) MemPtrFree (d3);
	if (d4!=0) MemPtrFree (d4);
	w= CW (POP->small, POP->M[i+1].font, nc);
	MemPtrFree (oc);
	if (w>maxcw) maxcw= w;
}
return maxcw;
}

Boolean IM_windows (TPOP* POP, UInt16 hr, UInt16 maxcw) {
Int16 sh= hstyle_s*POP->M->data.header.count_s;
Int16 ch;
UInt16 iconw= (16*POP->si);
UInt16 _w= POP->f.bits.width;
UInt16 _s= POP->f.bits.shadowWidth;
UInt16 _tot= (2* _w)+ _s;
Err e;
WinHandle winH;
RectangleType R= {0, 0, POP->sw, POP->sh};
if (hr!=0xFFFF) sh/=2;
if (POP->small)
	ch= hstyle_c2*POP->M->data.header.count_c;
else	ch= hstyle_c*POP->M->data.header.count_c;

POP->oh= (POP->hsn*POP->M->data.header.count_n)+ (ch)+ (sh);
if ((POP->small) && (POP->si)) iconw= 9;
POP->ow= 8+ maxcw+ iconw- (4*POP->small);
POP->x= POP->x+ _w;
POP->y= POP->sh- POP->y- POP->oh- _w- _s;
if (POP->mid!=0) POP->y+= POP->oh;
if (POP->y+ POP->oh+_w+_s>POP->sh) {
	POP->y= POP->sh- POP->oh- _w- _s;
}
if (POP->x+ POP->ow+_w+_s>POP->sw) {
	if (POP->ow+ _tot<POP->sw) POP->x= POP->sw- POP->ow- _w- _s; else
		POP->x= _w;
}
POP->yy= MemPtrNew (sizeof(UInt16)* POP->cnt);
POP->hh= MemPtrNew (sizeof(UInt16)* POP->cnt);
POP->last= -1;
POP->ret= -1;
POP->hi= -1;
if (POP->highlight && POP->M->data.header.canhilite)
	POP->hi= POP->M->data.header.defaultitem;
/* VAR*/ {
	UInt32 u= 0;
	FtrGet (crea, _Fbeep, &u);
	if (TimGetTicks ()- u> SysTicksPerSecond ()/5)
		SndPlaySystemSound (7);
}
switch (hr) {
case 0xFFFF:
	winH= WinCreateOffscreenWindow (POP->ow, POP->oh, screenFormat, &e);
	break;
case 0xFFFE:
	winH= WinCreateOffscreenWindow (POP->ow, POP->oh, nativeFormat, &e);
	if (winH==0) break;
	POP->B= WinGetBitmap (winH);
	BmpSetDensity (POP->B, kDensityLow);
	break;
default:
	winH= HRWinCreateOffscreenWindow (hr, 2*POP->ow, 2*POP->oh, screenFormat, &e);
}
if (winH==0) {
	WaitAlert (2010);
	POP->exit= true;
	return false;
}
POP->W= winH;
POP->oldaw= WinGetActiveWindow ();
POP->olddw= WinGetDrawWindow ();
POP->S= WinCreateWindow (&R, 0, true, true, &e);
WinSetActiveWindow (POP->S);
return true;
}

//init menu
Boolean IM (TPOP* POP) {
UInt16 hr;
UInt16 maxcw;
if (!IM_ismenu (POP)) return false;
if (DD ()) {
	hr= 0xFFFE;
	POP->oldcs= WinGetCoordinateSystem ();
	if (POP->oldcs!=72)
		WinSetCoordinateSystem (72);
} else hr= HR ();
/*
hr:	0..0xFFFD	- HR
	0xFFFE	- DD
	0xFFFF	- std
*/
maxcw= IM_getmaxcw (POP, hr);
return IM_windows (POP, hr, maxcw);
}

void WCh (char* c, RectangleType* r, TPOP* POP){
UInt16 hires= POP->hires;
UInt16 len;
UInt16 w;
if ((POP->small) || (hires==0xFFFF)) {
	WC (c, r);
	return;
}
len= StrLen (c);
w= FntCharsWidth (c, len);
if (hires==0xFFFE) {
	w*= 2;
	w++;
	if (w>r->extent.x) w= r->extent.x;
	BmpSetDensity (POP->B, kDensityDouble);
	WinSetCoordinateSystem (144);
	WinDrawTruncChars (c, len, r->topLeft.x+ ((r->extent.x- w)/2), r->topLeft.y+ ((r->extent.y- 22)/2), w);
	BmpSetDensity (POP->B, kDensityLow);
	WinSetCoordinateSystem (72);
} else {
	if (w>r->extent.x) w= r->extent.x;
	HRWinDrawTruncChars (hires, c, len, r->topLeft.x+ ((r->extent.x- w)/2), r->topLeft.y+ ((r->extent.y- 22)/2), w);
}
}

void WCs (char* c, Int16 xs, Int16 x, Int16 ys, Int16 y, TPOP* POP, UInt16 si) {
UInt16 len= StrLen (c);
Int16 ox= (2*x)+ xs;
Int16 oy= (2*y)+ ys;
Int16 plus= 20;
UInt16 hiresx= POP->hires;
if (POP->small) hiresx= 0xFFFF;
switch (hiresx) {
case 0xFFFE:
	BmpSetDensity (POP->B, kDensityDouble);
	WinSetCoordinateSystem (144);
	WinDrawChars (c, len, ox, oy);
	BmpSetDensity (POP->B, kDensityLow);
	WinSetCoordinateSystem (72);
	goto underlineit;
case 0xFFFF:
	ox= xs+ x;
	oy= ys+ y;
	if (POP->small) {
		oy--;
		plus= 12;
	} else plus= 10;
	WinDrawChars (c, len, ox, oy);
	goto underlineit;
}
HRWinDrawChars (hiresx, c, len, ox, oy);
underlineit:
if ((POP->und) && (si!=0)) {
	Int16 ix= FntCharsWidth (c, si- 1);
	Int16 iw= FntCharWidth (c[si- 1]);
	if (hiresx==0xFFFE) {
		ix*= 2;
		iw*= 2;
	}
	if (hiresx>=0xFFFE) iw--;
	WinDrawLine (ox+ ix- 1, oy+ plus, ox+ ix+ iw, oy+ plus);
}
}

void HDB (UInt16 id, Int16 sx, Int16 x, Int16 sy, Int16 y, Boolean hrdd) {
MemHandle bmph;
BitmapPtr bmp;
if (hrdd) {
	id+= 300;
	x*= 2;
	y*= 2;
}
bmph= DmGetResource ('Tbmp', id);
if (bmph==0) return;
bmp= MemHandleLock (bmph);
WinDrawBitmap (bmp, x+sx, y+sy);
MemHandleUnlock (bmph);
DmReleaseResource (bmph);
}

//offscreen draw
void OD (TPOP* POP) { //draw items
Int16 hstyles[6]= {POP->hsn, hstyle_c, hstyle_s, POP->hsn<<1, hstyle_c<<1, hstyle_s};
WinHandle oldh= WinSetDrawWindow (POP->W);
UInt16 y= 0;
FontID oldf= FntGetFont ();
UnderlineModeType oldu= WinSetUnderlineMode (0);

Boolean sony= (POP->hires<0xFFFE);
Boolean hrdd= (POP->hires!=0xFFFF);
Boolean small= (POP->small);
Boolean hrdd2= (hrdd && (!small));
UInt16 i;
Int16 aw= POP->ow;

#ifdef trial
UInt32 fx= 10012;
FtrGet (crea, _Ftrial3+ fx- 10012, &fx);
if (fx>14) goto afterdraw;
#endif

if (!POP->color) {
	WinEraseWindow ();
}

if (hrdd) aw*= 2;

for (i= 0; i<POP->cnt; i++) {
	Titem* M= &(POP->M[i+ 1]);
	Int16 ah= (small && (M->hstyle==1))?(hstyle_c2<<1):(hstyles[M->hstyle+ (3*hrdd)]);
	RectangleType r= {0, y, aw, ah};
	Int16 type= M->type;
	IndexedColorType cb= M->color[0];
	IndexedColorType cf= M->color[1];
	IndexedColorType cl= M->color[2];
	FontID font= M->font;
	IndexedColorType oldb, oldc, oldt;
	char* c;
	char* sc;
	Boolean dsi;
	Boolean cbx= false;
	if (M->data.body.invisible) continue;
	POP->yy[i]= y;
	POP->hh[i]= ah;
	y+= ah;
	if (small) font= (font*5)+ 2; else
		if (sony) font+= 8;
	FntSetFont (font);
	if (POP->color) {
		oldc= WinSetForeColor (cf);
		oldb= WinSetBackColor (cb);
		oldt= WinSetTextColor (cf);
		if (type!=_splitter) WinEraseRectangle (&r, 0);
	}
	sc= c= GC ('tSTR', M->cid);
	dsi= ((POP->si) && (!M->noicon));
	switch (type) {
	case _application: case _desk: case _panel: {
		MemHandle bmph;
		BitmapPtr bmp;
		if (!dsi) break;
		if (hrdd2) {
			Coord width;
			UInt16 bmpo;
			if (!M->b1000) {
				braki:
				dsi= POP->asi;
				break;
			}
			bmph= DmGetResource (M->data.body.creator, 1000);
			if (bmph==0) goto braki;
			bmp= MemHandleLock (bmph);
			width= bmp->width;
			if (POP->OS>=0x04003000) BmpGetDimensions (bmp, &width, 0, 0);
			bmpo= (38- width)/ 2;
			if (POP->OS>=0x05000000) {
				WinDrawOperation odo= WinSetDrawMode (winOverlay);
				WinPaintBitmap (bmp, r.topLeft.x+ bmpo, r.topLeft.y+ (r.extent.y/2)- 11);
				WinSetDrawMode (odo);
			} else {
				WinDrawBitmap (bmp, r.topLeft.x+ bmpo, r.topLeft.y+ (r.extent.y/2)- 11);
			}
		} else {
			UInt16 bmpo;
			if (!M->b1001) goto braki;
			bmph= DmGetResource (M->data.body.creator, 1001);
			if (bmph==0) goto braki;
			bmp= MemHandleLock (bmph);
			bmpo= (20- bmp->width)/2;
			if (POP->OS>=0x05000000) {
				WinDrawOperation odo= WinSetDrawMode (winOverlay);
				WinPaintBitmap (bmp, r.topLeft.x+ bmpo, r.topLeft.y+ (r.extent.y/2)- 4);
				WinSetDrawMode (odo);
			} else {
				WinDrawBitmap (bmp, r.topLeft.x+ bmpo, r.topLeft.y+ (r.extent.y/2)- 4);
			}
		}
		dsi= false;
		MemHandleUnlock (bmph);
		DmReleaseResource (bmph);
		}
		break;
	case _submenu: {
		UInt16 ow2= POP->ow;
		if (small) ow2<<= 1;
		HDB (8000+ 0xA0+ 0, r.topLeft.x, ow2- 10, r.topLeft.y+ (r.extent.y/2)+1, -5, hrdd2);
		}
		break;
	case _recent:
		if (c[0]==0) {
			sc= S_RECENT;
			if (POP->MRU_RS[M->data.body.index]!=0)
				sc= (char*)(&POP->MRU_RS[M->data.body.index]->name);
		}
		break;
	case _redial:
		if (c[0]==0) {
			sc= S_REDIAL;
			if (POP->MRU_DS[M->data.body.index].name!=0)
				sc= POP->MRU_DS[M->data.body.index].name;
		}
		break;
	case _beamreceive:
		if (GBRS ()) cbx= true;
		break;
	case _backlight:
		if (GBLS ()) cbx= true;
		break;
	case _keylights:
		if (GKLS ()) cbx= true;
		break;
	case _autooff:
		if (GAOS ()) cbx= true;
		break;
	case _sound:
		if (GSSS ()) cbx= true;
		break;
	case _bluetooth:
		if (GBTS ()) cbx= true;
		break;
	case _network:
		if (GNWS (0, false)) cbx= true;
		break;
	case _radio:
		if (GRDS ()) cbx= true;
		break;
	case _battery: case _memory: case _datetime:
		sc= POP->gg[M->data.body.index];
		break;
	case _splitter: {
		UInt16 tc, bc;
		Boolean ps, ns;
		UInt16 x, y;
		if (POP->color) {
			RectangleType r3= r;
			tc= M->data.body.tc;
			bc= M->data.body.bc;
			r3.extent.y/= 2;
			WinSetBackColor (tc);
			WinEraseRectangle (&r3, 0);
			r3.topLeft.y+= r3.extent.y;
			r3.extent.y= r.extent.y- r3.extent.y;
			WinSetBackColor (bc);
			WinEraseRectangle (&r3, 0);
		}
		y= r.topLeft.y+ (r.extent.y/2);
		x= r.topLeft.x+ r.extent.x- 1;
		if (POP->color) WinSetForeColor (cl);
		WinDrawLine (r.topLeft.x, y, x, y);
		if (POP->color)
			WinSetForeColor (M->data.body.tl);
		WinDrawLine (r.topLeft.x, y- 1, x, y- 1);
		if (POP->color)
			WinSetForeColor (M->data.body.bl);
		WinDrawLine (r.topLeft.x, y+ 1, x, y+ 1);
		if (c[0]!=0) {
			UInt16 clen= StrLen (c);
			char* e= MemPtrNew (clen+ 3);
			if ((POP->color) && (ps) && (ns))
				WinSetBackColor (bc);
			e[0]= ' ';
			StrCopy (&e[1], c);
			e[clen+ 1]= ' ';
			e[clen+ 2]= 0;
			WCh (e, &r, POP);
			MemPtrFree (e);
		}
		sc= 0;
		}
		//nobreak:
	case _untyped:
		dsi= false;
	}
	if (sc) WCs (sc, r.topLeft.x, 4+ (16*POP->si), r.topLeft.y+ (r.extent.y/2)+1, -6, POP, M->shortcut_i);
	if (cbx) {
		UInt16 ow2= POP->ow;
		if (small) ow2<<= 1;
		HDB (8000+ 0xA0+ 1, r.topLeft.x, ow2- 13, r.topLeft.y+ (r.extent.y/2)+1, -5, hrdd2);
	}
	if (dsi) HDB (8000+ type, r.topLeft.x, 5, r.topLeft.y+ (r.extent.y/2)+1, -5, hrdd2);	
	if (POP->color) {
		WinSetForeColor (oldc);
		WinSetBackColor (oldb);
		WinSetTextColor (oldt);
	}
	MemPtrFree (c);
}
#ifdef trial
afterdraw:
#endif
WinSetUnderlineMode (oldu);
FntSetFont (oldf);
WinSetDrawWindow (oldh);
}

void DR (TPOP* POP, RectangleType* r, Int16 x, Int16 y) {
RectangleType R;
switch (POP->hires) {
case 0xFFFF:
	WinCopyRectangle (POP->W, 0, r, x, y, winPaint);
	return;
case 0xFFFE:
	BmpSetDensity (POP->B, kDensityDouble);
	WinCopyRectangle (POP->W, 0, r, x, y, winPaint);
//	BmpSetDensity (POP->B, kDensityLow);
	return;
default:
	R= *r;
	R.topLeft.x*= 2;
	R.topLeft.y*= 2;
	R.extent.x*= 2;
	R.extent.y*= 2;
	HRWinCopyRectangle (POP->hires, POP->W, 0, &R, 2*x, 2*y, winPaint);
}
}

void DR2 (TPOP* POP, RectangleType* r, Int16 x, Int16 y) {
RectangleType R;
switch (POP->hires) {
case 0xFFFF:
	WinCopyRectangle (POP->Wb, 0, r, x, y, winOverlay);
	return;
case 0xFFFE:
	R= *r;
	R.topLeft.x/= 2;
	R.topLeft.y/= 2;
	R.extent.x/= 2;
	R.extent.y/= 2;
	BmpSetDensity (POP->Bb, kDensityDouble);
	WinCopyRectangle (POP->Wb, 0, &R, x, y, winOverlay);
//	BmpSetDensity (POP->Bb, kDensityLow);
	return;
default:
	HRWinCopyRectangle (POP->hires, POP->Wb, 0, r, 2*x, 2*y, winOverlay);
}
}

Boolean Versa (char* email) {
SysNotifyParamType param;
HelperNotifyEventType details;
HelperNotifyExecuteType execute;
HelperServiceEMailDetailsType emailDetails;
Char subj[3]; //= {0};
Char msg[30]; //= {0};
Char name[10]; //= {0};
subj[0]= 0;
msg[0]= 0;
name[0]= 0;
param.notifyType = 'hlpr';
param.broadcaster = crea;
param.notifyDetailsP = (void*)&details;
param.handled = false;
details.version = 1;
details.actionCode = 3;
details.data.executeP = &execute;
execute.serviceClassID = 'mail';
execute.helperAppID = 0;
execute.dataP = email;
execute.displayedName = name;
execute.err = errNone;
emailDetails.version = 1;
emailDetails.cc = subj;
emailDetails.subject = subj;
emailDetails.message = msg;
execute.detailsP = &emailDetails;
SysNotifyBroadcast(&param);
return param.handled;
}

void ExL () {
char* c;
ExgSocketType s;
char* colon;
FtrGet (crea, _Furl, (UInt32*)&c);
MemSet (&s, sizeof(ExgSocketType), 0);
s.name= c;
colon= StrChr (c, ':');
if (colon) {
	Boolean postponed= false;
	UInt16 len= colon- c;
	char* t= MemPtrNew (len+ 1);
	UInt32 creator;
	MemMove (t, c, len);
	t[len]= 0;
	if (!ExgGetDefaultApplication (&creator, 0xFFFC, t)) {
		UInt32 id;
		UInt32 run;
		if ((creator=='asc3') && (!StrCompare (t, "mailto"))) {
			MemPtrFree (t);
			Versa (colon+ 1);
			goto freeall;
		}
		FtrGet (crea, _Frun, &id);
		DmDatabaseInfo (0, id, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, &run);
		if (creator==run) {
			if (creator=='BLZ5') {
				FormPtr frmP= FrmGetActiveForm ();
				if (FrmGetFormId (frmP)==25100) {
					UInt16 fldI= FrmGetObjectIndex (frmP, 1038);
					UInt16 ctlI= FrmGetObjectIndex (frmP, 1040);
					if ((fldI!=frmInvalidObjectId) && (ctlI!=frmInvalidObjectId)) {
		 				FieldPtr fldP= FrmGetObjectPtr (frmP, fldI);
		 				Boolean hidden= !fldP->attr.usable;
						char* fldT= FldGetTextPtr (fldP);
						RectangleType R;
		 				if (hidden) {
		 					RectangleType R0= {0,0,0,0};
							WinGetClip (&R);
							WinSetClip (&R0);
		 				}
						if (fldT) {
							FldSetSelection (fldP, 0, StrLen (fldT));
						}
						FldInsert (fldP, c, StrLen (c));
		 				if (hidden) {
							WinSetClip (&R);
		 				}
						CtlHitControl (FrmGetObjectPtr (frmP, ctlI));
						goto freeall;
					}
				}
			}
			LNCH (crea, 'appl');
			FtrSet (crea, _Fcode, 0x8020);
			MemPtrSetOwner (c, 0);
			postponed= true;
		}
	}
	MemPtrFree (t);
	if (postponed) return;
}
if (ExgRequest (&s)) FrmAlert (2008);
freeall:
FtrUnregister (crea, _Furl);
MemPtrFree (c);
}

/*
void ExL () {
char* c;
ExgSocketType s;
char* colon;
FtrGet (crea, _Furl, (UInt32*)&c);
MemSet (&s, sizeof(ExgSocketType), 0);
s.name= c;
colon= StrChr (c, ':');
if (colon) {
	Boolean postponed= false;
	UInt16 len= colon- c;
	char* t= MemPtrNew (len+ 1);
	UInt32 creator;
	MemMove (t, c, len);
	t[len]= 0;
	if (!ExgGetDefaultApplication (&creator, 0xFFFC, t)) {
		UInt32 id;
		UInt32 run;
		iccreator:
		FtrGet (crea, _Frun, &id);
		DmDatabaseInfo (0, id, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, &run);
		if (creator==run) {
			LNCH (crea);
			FtrSet (crea, _Fcode, 0x8020);
			MemPtrSetOwner (c, 0);
			postponed= true;
		}
	}
	MemPtrFree (t);
	if (postponed) return;
}
FtrUnregister (crea, _Furl);
if (ExgRequest (&s)) FrmAlert (2008);
MemPtrFree (c);
}
*/

void HeL () {
char* c;
char* d;
HelperNotifyExecuteType* execute;
HelperNotifyEventType* event;
SysNotifyParamType note;
FtrGet (crea, _Fdial, (UInt32*)&c);
FtrGet (crea, _Fdial_name, (UInt32*)&d);
FtrUnregister (crea, _Fdial);
FtrUnregister (crea, _Fdial_name);

execute= MemPtrNew (sizeof(HelperNotifyExecuteType));
event= MemPtrNew (sizeof(HelperNotifyEventType));
MemSet (execute, sizeof(HelperNotifyExecuteType), 0);
MemSet (event, sizeof(HelperNotifyEventType), 0);
MemSet (&note, sizeof(SysNotifyParamType), 0);
execute->dataP= c;
execute->displayedName= d;
execute->serviceClassID= 'voic';
event->version= 1;
event->actionCode= 3;
event->data.executeP= execute;
note.notifyType= 'hlpr';
note.notifyDetailsP= (void*)event;
SysNotifyBroadcastDeferred (&note, sizeof(HelperNotifyEventType));
//MemPtrFree (c); system will free
//MemPtrFree (d); system will free
}

void RUNDA (UInt16 card, LocalID id) {
FormActiveStateType formState;
DmOpenRef db= DmOpenDatabase (card, id, dmModeReadOnly);
MemHandle h= DmGet1Resource ('code', 1000);
MemPtr p= MemHandleLock (h);
FormPtr oldform= FrmGetActiveForm ();
UInt16 w= FrmGetActiveFormID ();
WinHandle awin= WinGetActiveWindow ();
WinHandle dwin= WinGetDrawWindow ();
FrmSaveActiveState (&formState);
((void (*)(void))p)();
FrmRestoreActiveState (&formState);
if (w!=0) FrmSetActiveForm (oldform);
if (awin!=0) WinSetActiveWindow (awin);
if (dwin!=0) WinSetDrawWindow (dwin);
MemHandleUnlock (h);
DmReleaseResource (h);
DmCloseDatabase (db);
}

void DaL () {
UInt16 card;
LocalID id;
UInt32 creator;
DmSearchStateType ss;
Err e;
FtrGet (crea, _Fdacreator, (UInt32*)&creator);
e= DmGetNextDatabaseByTypeCreator (true, &ss, 'DAcc', creator, true, &card, &id);
if ((e!=0) || (id==0)) {
	FrmAlert (2004);
} else {
	RUNDA (card, id);
}
FtrUnregister (crea, _Fdacreator);
}

//uninit menu
void UM (TPOP* POP) {
if (POP->gg!=0) {
	UInt16 i;
	for (i= 0; i<POP->M->data.header.count_g; i++) if (POP->gg[i]!=0) MemPtrFree (POP->gg[i]);
	MemPtrFree (POP->gg);
}
if (POP->MRU_RA!=0) {
	MemPtrFree (POP->MRU_RA);
	if (POP->MRU_RS!=0) MemPtrFree (POP->MRU_RS);
}
if (POP->MRU_DA!=0) {
	UInt16 i;
	for (i=0; i<POP->M->data.header.count_d; i++) {
		if (POP->MRU_DA[i].name!=0) MemPtrFree (POP->MRU_DA[i].name);
		if (POP->MRU_DA[i].number!=0) MemPtrFree (POP->MRU_DA[i].number);
	}
	MemPtrFree (POP->MRU_DA);
	if (POP->MRU_DS!=0) MemPtrFree (POP->MRU_DS);
}
if (POP->yy!=0) {
	MemPtrFree (POP->hh);
	MemPtrFree (POP->yy);
}

if (POP->oldw!=0) {
	Coord w, h;
	WinGetDisplayExtent (&w, &h);
	if ((w==POP->sw) && (h==POP->sh))
		WinRestoreBits (POP->oldw, POP->R.topLeft.x, POP->R.topLeft.y);
}
if (POP->olddw!=0)
	WinSetDrawWindow (POP->olddw);
if (POP->oldaw!=0) {
	WinSetActiveWindow (POP->oldaw);
	WinDeleteWindow (POP->S, false);
}
if (POP->W!=0) WinDeleteWindow (POP->W, false);
if (POP->Wb!=0) WinDeleteWindow (POP->Wb, false);
if ((POP->hires==0xFFFE) && (POP->oldcs!=72))
	WinSetCoordinateSystem (POP->oldcs);
if (POP->swp!=0) DmCloseDatabase (POP->swp);
if (POP->m!=0) {
	MemHandleUnlock (POP->m);
	DmReleaseResource (POP->m);
}
if (POP->db!=0) DmCloseDatabase (POP->db);

if (POP->P!=0) {
	RGBColorType* WP= MemHandleLock (POP->P);
	WinPalette (1, 0, 256, WP);
	MemHandleFree (POP->P);
}

if (POP->kyoceraOkOff!=0)
	FtrSet ('QCMT', 2, POP->kyoceraOkOff);

if (POP->lastfocus!=0xFFFF) {
	FldGrabFocus (FrmGetObjectPtr (POP->F, POP->lastfocus));
	FrmSetFocus (POP->F, POP->lastfocus);
}
if (POP->ipe) InsPtEnable (true);
if (POP->gsie) GsiEnable (true);
if (POP->attne) AttnIndicatorEnable (true);
if (POP->mid==0) {
	UInt32 treo= 0;
	UInt32 postpone= 0;
	FtrGet (crea, _Ftreo, &treo);
	FtrGet (crea, _Fpostpone, &postpone);
	if (postpone) {
		FtrSet (crea, _Fpostpone, 0);
		switch (postpone) {
		case _attention:
			AttnListOpen ();
			break;
		case _url:
			ExL ();
			break;
		case _dial:
			HeL ();
			break;
		case _desk:
			DaL ();
			break;
		}
	}
	FtrUnregister (crea, _Fon);
	FtrUnregister (crea, _Ftrig);
	FtrUnregister (crea, _Frun);
	FtrUnregister (crea, _Fvis);
	FtrUnregister (crea, _Fkyo);
	if (treo) {
		SysNotifyParamType not= {sysNotifyExternalDialogClosedEvent, crea, 0, 0, 0};
		SysNotifyBroadcast (&not);
	}
// 	if (POP->hs) {
// 		PhnLibRegister (POP->hs, crea, 0);
// 	}
}
}

Int16 II (TPOP* POP, Int16 x, Int16 y) {
UInt16 i;
x-= POP->x;
y-= POP->y;
if ((x<0) || (x>POP->ow)) return -1;
for (i= 0; i<POP->cnt; i++) {
	Int16 yy, hh;
	Ttype type= POP->M[i+1].type;
	if ((POP->M[i+1].data.body.invisible) || (type==_splitter) || ((type>=_0_status) && (type<_x_status))) continue;
	yy= POP->yy[i];
	hh= POP->hh[i];
	if (POP->hires!=0xFFFF) {
		yy/= 2;
		hh/= 2;
	}
	if ((y>=yy) && (y<yy+ hh)) return i;
}
return -1;
}

RectangleType GR (TPOP* POP, Int16 ind) {
Int16 yy= POP->yy[ind];
Int16 hh= POP->hh[ind];
if (POP->hires!=0xFFFF) {
	if (yy%2!=0) {
		yy++;
		hh--;
	}
	yy/= 2;
	hh/= 2;
}
{
RectangleType r= {POP->x, POP->y+ yy, POP->ow, hh};
return r;
}
}

void HI (TPOP* POP, Int16 new) {
Int16 old= POP->last;
if (old!=new) {
	UInt16 round= 5- (2*POP->small);
	if (old!=-1) {
		RectangleType r= GR (POP, old);
		WinInvertRectangle (&r, round);
	}
	if (new!=-1) {
		RectangleType r= GR (POP, new);
		WinInvertRectangle (&r, round);
		if (POP->actick) {//=== if not (pierwszy highlight w tym submenu) {
			SndPlaySystemSound (7);
			FtrSet (crea, _Fbeep, TimGetTicks ());
		}
	}
}
POP->actick= TimGetTicks ();
POP->last= new;
}

void IB (TPOP* POP) {
WinHandle winH;
Err e;
Boolean hrdd;
WinHandle oldh;
UInt8 oldf, oldb;
Boolean colors;
if (POP->hires==0xFFFF) return;
if (POP->mid!=0) return;
if (POP->ow<16+b_w) return;
switch (POP->hires) {
case 0xFFFF:
	winH= WinCreateOffscreenWindow (b_c*b_w, b_h, screenFormat, &e);
	break;
case 0xFFFE:
	winH= WinCreateOffscreenWindow (b_c*b_w, b_h, nativeFormat, &e);
	POP->Bb= WinGetBitmap (winH);
	BmpSetDensity (POP->Bb, kDensityLow);
	break;
default:
	winH= HRWinCreateOffscreenWindow (POP->hires, b_c*2*b_w, 2*b_h, screenFormat, &e);
}
POP->Wb= winH;
hrdd= (POP->hires!=0xFFFF);
oldh= WinSetDrawWindow (winH);
colors= ((POP->color>1) && (POP->OS>=0x05000000));
if (colors) {
	oldf= WinSetForeColor (0xA1);
	oldb= WinSetBackColor (255);
}
HDB (8600, 0, 0, 0, 0, hrdd);
if (colors) {
	WinSetBackColor (oldb);
	WinSetForeColor (oldf);
}
WinSetDrawWindow (oldh);
POP->b_t= POP->oh;
POP->b_a= 1;
POP->b_x= 8+ (SysRandom (0)% (POP->ow- 16- b_w));
}

void SB (TPOP* POP) {
UInt16 x;
Int16 vc;
if (!POP->b_a) return;
if (POP->b_t+ b_h<0) POP->b_m= 2;
if (POP->b_m!=1) return;
if (POP->hi!=-1) return;
/* VAR */ {
RectangleType R1= {POP->b_x, POP->b_t, b_w, b_h};
DR (POP, &R1, POP->b_x+ POP->x, POP->b_t+ POP->y);
}
x= POP->b_i* b_w;
POP->b_i++;
POP->b_i%= b_c;
POP->b_t--;
if (POP->b_t%5==0) POP->b_x+= POP->b_v;
vc= (SysRandom (0)%40)- 1;
if (vc<2) POP->b_v= vc;
/* VAR */ {
RectangleType R;
RectangleType R1= {x, 0, b_w, b_h};
RectangleType R2= {POP->x, POP->y, POP->ow, POP->oh};
WinGetClip (&R);
WinSetClip (&R2);
DR2 (POP, &R1, POP->b_x+ POP->x, POP->b_t+ POP->y);
WinSetClip (&R);
}
}

Err IP (UInt32 totalBytes, UInt32 offset, void* userDataP) {
Int32 ftr;
Int16 last;
UInt16 tb= totalBytes>>11;
UInt16 of= offset>>11;
Int16 cur= (13* of)/ tb;
FtrGet (crea, _Fprogres, &ftr);
last= ftr;
if (cur!=last) {
	Int16 i;
	for (i= last+ 1; i<=cur; i++)
		HDB (8601, 13+ (i* 9), 0, 74, 0, 0);
	FtrSet (crea, _Fprogres, cur);
}
return 0;
}

//screen draw
Boolean VD (TPOP* POP) {
UInt16 e;
UInt16 x= POP->x;
UInt16 y= POP->y;
UInt16 _w= POP->f.bits.width;
UInt16 _s= POP->f.bits.shadowWidth;
RectangleType R= {x-_w, y-_w, POP->ow+(2*_w)+_s, POP->oh+(2*_w)+_s};
RectangleType R1= {0, 0, POP->ow, POP->oh};
RectangleType R2= {x, y, POP->ow, POP->oh};
IndexedColorType oldc;
UInt16 del;
POP->ipe= InsPtEnabled ();
if (POP->ipe) InsPtEnable (false);
POP->gsie= GsiEnabled ();
if (POP->gsie) GsiEnable (false);
POP->attne= ((POP->OS>=0x04003000) && (AttnIndicatorEnabled ()));
if (POP->attne) AttnIndicatorEnable (false);
POP->F= FrmGetActiveForm ();
if (POP->F) {
	POP->lastfocus= FrmGetFocus (POP->F);
	if (POP->lastfocus!=0xFFFF) {
		FormObjectKind kind= FrmGetObjectType (POP->F, POP->lastfocus);
		if ((kind!=frmFieldObj) || (POP->oldaw != FrmGetWindowHandle (POP->F))) {
			POP->lastfocus= 0xFFFF;
		} else FrmSetFocus (POP->F, 0xFFFF);
	}
}
POP->oldw= WinSaveBits (&R, &e);

if (POP->oldw==0) {
#ifdef STOPREASONMORE
POP->stopreason= 1002;
#endif
	FrmAlert (2010);
	POP->exit= true;
	POP->stop= true;
	return false;
}

POP->R= R;
if (POP->color) oldc= WinSetForeColor (POP->fc);
if (POP->whbg) {
	UInt16 cor= POP->f.bits.cornerDiam;
	WinEraseRectangle (&R, cor+ 1);
}
WinDrawRectangleFrame (POP->f.word, &R2);
if (POP->color) WinSetForeColor (oldc);

if (POP->ani==0) DR (POP, &R1, x, y);

del= SysTicksPerSecond ()/50;
if (POP->anis<256) del/= 2;

switch (POP->ani) {
case 1: {
	UInt16 pxs= POP->anis+ 1;
	UInt16 tw= (POP->ow+pxs-1)/pxs;
	UInt16 th= (POP->oh+pxs-1)/pxs;
	UInt16 all= th* tw;
	UInt16 cnt= all;
	UInt8* alr= M0 (all);
	UInt16 del2= all/32;
	if (POP->hires==0xFFFE) BmpSetDensity (POP->B, kDensityDouble);
	while (cnt!=0) {
		UInt32 sta= TimGetTicks ();
		UInt16 i;
		for (i=0; i<del2; i++) {
			UInt16 v;
			if (cnt==0) break;
			do v= SysRandom (0)%all; while (alr[v]!=0);
			alr[v]= 1;
			cnt--;
			{
			RectangleType R= {v%tw*pxs, v/tw*pxs, pxs, pxs};
			DR (POP, &R, x+ (v%tw*pxs), y+ (v/tw*pxs));
			}
		}
		while (TimGetTicks ()- sta<del);
	}
	MemPtrFree (alr);
	}
	break;
case 2:
	switch (POP->anis) {
	case 256: {
                UInt16 i;
		for (i= 0; i<(POP->ow+3)/4; i++) {
			UInt32 sta= TimGetTicks ();
			RectangleType R= {i*4, 0, 4, POP->oh};
			DR (POP, &R, x+(i*4), y);
			while (TimGetTicks ()- sta<del);
		}
		}
		break;
	case 257: {
		Int16 i;
		for (i= POP->ow/4; i>=0; i--) {
			UInt32 sta= TimGetTicks ();
			RectangleType R= {i*4, 0, 4, POP->oh};
			DR (POP, &R, x+(i*4), y);
			while (TimGetTicks ()- sta<del);
		}
		}
		break;
	case 258: {
		Int16 i;
		for (i= POP->oh/4; i>=0; i--) {
			UInt32 sta= TimGetTicks ();
			RectangleType R= {0, i*4, POP->ow, 4};
			DR (POP, &R, x, y+(i*4));
			while (TimGetTicks ()- sta<del);
		}
		}
		break;
	case 259: {
		UInt16 i;
		for (i= 0; i<(POP->oh+3)/4; i++) {
			UInt32 sta= TimGetTicks ();
			RectangleType R= {0, i*4, POP->ow, 4};
			DR (POP, &R, x, y+(i*4));
			while (TimGetTicks ()- sta<del);
		}
		}
		break;
	case 260: {
		Int16 cen= POP->ow/2;
		Int16 i;
		for (i= POP->ow/4; i>=0; i--) {
			UInt32 sta= TimGetTicks ();
			RectangleType R= {cen+(i*2), 0, 2, POP->oh};
			RectangleType R2= {cen-(i*2)-2, 0, 2, POP->oh};
			DR (POP, &R, x+cen+(i*2), y);
			DR (POP, &R2, x+cen-(i*2)-2, y);
			while (TimGetTicks ()- sta<del);
		}
		}
		break;
	case 261: {
		Int16 cen= POP->oh/2;
		Int16 i;
		for (i= POP->oh/4; i>=0; i--) {
			UInt32 sta= TimGetTicks ();
			RectangleType R= {0, cen+(i*2), POP->ow, 2};
			RectangleType R2= {0, cen-(i*2)-2, POP->ow, 2};
			DR (POP, &R, x, y+cen+(i*2));
			DR (POP, &R2, x, y+cen-(i*2)-2);
			while (TimGetTicks ()- sta<del);
		}
		}
		break;
	case 262: {
		Int16 cen= POP->ow/2;
		UInt16 i;
		for (i= 0; i<(POP->ow+3)/4; i++) {
			UInt32 sta= TimGetTicks ();
			RectangleType R= {cen+(i*2), 0, 2, POP->oh};
			RectangleType R2= {cen-(i*2)-2, 0, 2, POP->oh};
			DR (POP, &R, x+cen+(i*2), y);
			DR (POP, &R2, x+cen-(i*2)-2, y);
			while (TimGetTicks ()- sta<del);
		}
		}
		break;
	case 263: {
		Int16 cen= POP->oh/2;
		UInt16 i;
		for (i= 0; i<(POP->oh+3)/4; i++) {
			UInt32 sta= TimGetTicks ();
			RectangleType R= {0, cen+(i*2), POP->ow, 2};
			RectangleType R2= {0, cen-(i*2)-2, POP->ow, 2};
			DR (POP, &R, x, y+cen+(i*2));
			DR (POP, &R2, x, y+cen-(i*2)-2);
			while (TimGetTicks ()- sta<del);
		}
		}
		break;
	}
	break;
case 3:
	switch (POP->anis) {
	case 256: {
		Int16 i;
		for (i= POP->ow/4; i>=0; i--) {
			UInt32 sta= TimGetTicks ();
			RectangleType R= {(i*4), 0, POP->ow- (i*4), POP->oh};
			DR (POP, &R, x, y);
			while (TimGetTicks ()- sta<del);	
		}
		}
		break;
	case 257: {
		Int16 i;
		for (i= POP->ow/4; i>=0; i--) {
			UInt32 sta= TimGetTicks ();
			RectangleType R= {0, 0, POP->ow- (i*4), POP->oh};
			DR (POP, &R, x+ (i*4), y);
			while (TimGetTicks ()- sta<del);	
		}
		}
		break;
	case 258: {
		Int16 i;
		for (i= POP->oh/4; i>=0; i--) {
			UInt32 sta= TimGetTicks ();
			RectangleType R= {0, 0, POP->ow, POP->oh- (i*4)};
			DR (POP, &R, x, y+ (i*4));
			while (TimGetTicks ()- sta<del);	
		}
		}
		break;
	case 259: {   	
		Int16 i;
		for (i= POP->oh/4; i>=0; i--) {
			UInt32 sta= TimGetTicks ();
			RectangleType R= {0, (i*4), POP->ow, POP->oh- (i*4)};
			DR (POP, &R, x, y);
			while (TimGetTicks ()- sta<del);
		}
		}
		break;
	}
	break;
}

if (POP->color) WinSetForeColor (POP->fc);
WinDrawRectangleFrame (POP->f.word, &R2);
if (POP->color) WinSetForeColor (oldc);
POP->popuptick= TimGetTicks ();
return true;
}

Boolean LNCH (UInt32 creator, UInt32 type) {
UInt16 card;
LocalID id;
DmSearchStateType ss;
matchInfoType m;
Err e= DmGetNextDatabaseByTypeCreator (true, &ss, type, creator, true, &card, &id);
if ((e==0) && (id!=0)) {
	if (type=='panl') {
		FtrSet (crea, _Fcode, sysAppLaunchCmdPanelCalledFromApp);
	}
	goto launch;
}
if (type!='appl') {
	if ((e!=0) || (id==0)) {
		/*if (type=='panl') */FrmAlert (2015);
		return false;
	}
	goto launch;
}
m.path= 0;
m.creator= creator;
m.icon1000= 0;
m.icon1001= 0;
LACL2 (&m);
if (m.path==0) {
	FrmAlert (2001);
	return false;
}
/* VAR */ {
	WinHandle oldw= WinSetDrawWindow (WinGetDisplayWindow ());
	RectangleType R= {17, 70, 125, 19};
	RectangleType R1= {19, 72, 121, 14};
	RectangleType R2= {20, 73, 120, 12};
	WinHandle w= WinSaveBits (&R, &e);
	WinEraseRectangle (&R1, 5);
	WinDrawRectangleFrame (0x0402, &R2);
	FtrSet (crea, _Fprogres, 0);
	e= VFSImportDatabaseFromFileCustom (m.vol, m.path, &card, &id, &IP, 0);
	if (e==0) {
		MemHandle h;
		void* hilvfs;
		VFSRunningApp* pVra= MemPtrNew (sizeof(VFSRunningApp));
		StrCopy ((char*)pVra, m.path);
		pVra->volRefNum= m.vol;
		pVra->cardNo= card;
		pVra->dbID= id;
		pVra->creator= m.creator;
		pVra->type= 'appl';
		pVra->vfsState= vsWaiting;
		PrefSetAppPreferences (crea, 1, 0, pVra, sizeof(VFSRunningApp), false);
		MemPtrFree (pVra);
		h= DmGetResource ('tPRC', 0);
		hilvfs= MemHandleLock (h);
		DmCreateDatabaseFromImage (hilvfs);
		MemHandleUnlock (h);
		DmReleaseResource (h);
		DmGetNextDatabaseByTypeCreator (true, &ss, 'appl', 'Rhiv', true, &card, &id);
	}
	MemPtrFree (m.path);
	FtrUnregister (crea, _Fprogres);
	WinRestoreBits (w, 17, 70);
	WinSetDrawWindow (oldw);
	SndPlaySystemSound (7);
	if (e==0x0201) {
		FrmAlert (2011);
	}
}
launch:
/* VAR */ {
	UInt32 run= 0; UInt32 vis= 0; UInt32 kyo= 0;
	FtrGet (crea, _Frun, &run);
	FtrGet (crea, _Fvis, &vis);
	FtrGet (crea, _Fkyo, &kyo);
	if (id==vis) return true;
	if ((id==run) && (kyo!=0))
		EvtEnqueueKey (0x010B, 0, 8); else {
		FtrSet (crea, _Fcard, card);
		FtrSet (crea, _Fid, id);
	}
}
return true;
}

void LC (TPOP* POP, Int16 ind) {
UInt32 creator;
Ttype type;
UInt16 e8= 0;
POP->stoponnil= false;
if (ind==-1) return;
if (ind==-2) {
	creator= crea;
	goto launch;
}
if (ind==-3) {
	creator= GetRecentApp ();
	if (creator) {
		goto launch;
	} else {
		FrmAlert (2002);
		goto afteralert2;
	}
}
if (ind<0) type= -ind; else type= POP->M[ind+ 1].type;
if (type!=_submenu) {//pwcppj1 always ignores block
	UInt32 u= 0;
	if (POP->pass) {
		HI (POP, ind);
		SysTaskDelay (SysTicksPerSecond()/10);
	}
	FtrGet (crea, _Fbeep, &u);
#ifdef trial
	{
	UInt16 _a= 7;
	UInt16 _b= 7;
	UInt32 ADD= 0;
	UInt16 add;
	char* c;
	FtrGet (crea, (_Ftrial1+ 7)- _b, &ADD);
	add= ADD;
	if (TimGetTicks ()- u> SysTicksPerSecond ()/_a)
		SndPlaySystemSound (_b);
	c= MemPtrNew (2048);
	SysCopyStringResource (c, _b+ (2300- 7)+ add);
	add++;
	FtrSet (crea, (_Ftrial1- 7)+ _a, add%didyoukwow);
	if (!FrmCustomAlert (_a+ (2200- 7),c,"",""))
		goto freeit;
	FrmHelp (4000);
	freeit:
	WinSetActiveWindow (POP->S);
	POP->stay= true;
	MemPtrFree (c);
	}
#else
	if (TimGetTicks ()- u> SysTicksPerSecond ()/5)
		SndPlaySystemSound (7);
#endif
}
switch (type) {
case _submenu:
	if (POP->pass==POP->M[ind+ 1].data.body.mid) {
		SndPlaySystemSound (7);
		return;
	}
	if (POP->pass) HI (POP, ind);
	/* VAR */ {
		UInt16 _w= POP->f.bits.width;
		UInt16 _s= POP->f.bits.shadowWidth;
		UInt16 _r= 0;//recent
		UInt16 _d= 0;//redial
		Int16 yy= POP->yy[ind];
		Int16 Y= 0;
		Int16 X;
		if (POP->hires!=0xFFFF) yy/= 2;
		if (POP->M->data.header.count_r==1) _r= 1;//recent
		if (POP->M->data.header.count_d==1) _d= 1;//recent
		if (POP->b_m==1) POP->b_m= 3;
		X= POP->x+ POP->ow+ _w+ _s;
		if (!POP->M[ind+ 1].data.body.invisible) {
			Y= POP->sh- POP->y- yy- _w- _s;
		}
		POP->exit= AM (POP->M[ind+ 1].data.body.mid, X, Y, POP->riu- _r, POP->diu- _d);
		if (POP->exit) {
			POP->stop= true;
#ifdef STOPREASON
			POP->stopreason= 1;
#endif
			return;
		}
		if (POP->highlight) {
			if (POP->M[ind+ 1].data.body.invisible) {
				POP->actick= 0; //prevent HI() from calling SndPlaySystemSound
				HI (POP, POP->hi);
			} else {
				POP->hi= ind;
			}
		} else {
			if (POP->M[ind+ 1].data.body.invisible) {
				POP->ret= POP->hi;
			} else {
				POP->ret= ind;
			}
			POP->hi= -1;
			HI (POP, -1);
			if (POP->b_m==3) POP->b_m= 1;
		}
	}
	return;
case _application:
case _panel:
	creator= POP->M[ind+ 1].data.body.creator;
	launch:
	if (LNCH (creator, type==_panel?'panl':'appl')) break;
	afteralert:
	POP->hi= ind;
	afteralert2:
	WinSetActiveWindow (POP->S);
	POP->stay= true;
	return;
case _desk:
	creator= POP->M[ind+ 1].data.body.creator;
	FtrSet (crea, _Fdacreator, creator);
	FtrSet (crea, _Fpostpone, _desk);
	break;
/*case _panel:       //popup panel - does not work :/
	{
		DmSearchStateType ss;
		UInt16 cardNo;
		LocalID dbID;
		Err e= DmGetNextDatabaseByTypeCreator (true, &ss, 'panl', POP->M[ind+ 1].data.body.creator, true, &cardNo, &dbID);
		if ((e!=0) || (dbID==0)) {
			FrmAlert (2015);
		} else {
			UInt32 result;
			SysAppLaunch (cardNo, dbID, 0, sysAppLaunchCmdPanelCalledFromApp, 0, &result);
		}
	}
	break;*/
case _url:
	if (POP->OS>=0x04003000) {
		FtrSet (crea, _Furl, (UInt32)GC ('tURL', POP->M[ind+1].data.body.uid));
		FtrSet (crea, _Fpostpone, _url);
	} else {
		FrmAlert (2009);
		goto afteralert;
	}
	break;
case _dial:
	FtrSet (crea, _Fdial, (UInt32)GC ('tURL', POP->M[ind+1].data.body.uid));
	FtrSet (crea, _Fdial_name, (UInt32)GC ('tSTR', POP->M[ind+1].cid));
	FtrSet (crea, _Fpostpone, _dial);
	break;
case _redial: {
	UInt16 indor= POP->M[ind+ 1].data.body.index;
	if (POP->MRU_DS[indor].number==0) {
		UInt32 treo= 0;
		FtrGet (crea, _Ftreo, &treo);
		if (treo) {
			FrmAlert (2012);
		} else {
			FrmAlert (2013);
		}
		goto afteralert;
	}
	FtrSet (crea, _Fdial, (UInt32)A(POP->MRU_DS[indor].number));
	FtrSet (crea, _Fdial_name, (UInt32)A(POP->MRU_DS[indor].name));
	FtrSet (crea, _Fpostpone, _dial);
	break;
	}
case _recent: {
	UInt16 indor= POP->M[ind+ 1].data.body.index;
	if (POP->MRU_RS[indor]==0) {
		FrmAlert (2002);
		goto afteralert;
	}
	creator= POP->MRU_RS[indor]->creator;
	goto launch;
	}
case _home:
	creator= PrefGetPreference (prefLauncherAppCreator);
	goto launch;
case _find:
	e8= 266;
	break;
case _backlight:
	if (SBLS (POP)) {
		e8= 0x0113;
	}
	break;
case _keylights:
	if (!SKLS (!GKLS ())) {
		goto afteralert;
	}
	break;
case _beamreceive:
	SBRS (!GBRS ());
	break;
case _autooff:
	SAOS (POP);
	break;
case _sound:
	SSSS (POP);
	break;
case _bluetooth:
	if (!SBTS (POP)) {
		goto afteralert;
	}
	break;
case _network:
	SNWS (POP);
	WinSetActiveWindow (POP->S);
	POP->stay= true;
	break;
case _radio:
	if (!SRDS (POP)) {
		goto afteralert;
	}
	break;
case _brightness: {
	UInt32 treo= 0;
	FtrGet (crea, _Ftreo, &treo);
	if (treo) {
		e8= 0x0113;
	} else {
		e8= 0x0122;
	}
	break;
	}
case _contrast:
	e8= 0x0123;
	break;
case _hotsync:
	e8= 0x0209;
	break;
case _modemhotsync:
	e8= 0x020A;
	break;
case _lasthotsync:
	SetImmediateAlarm (_Alasthotsync);
	creator= 'sync';
	goto launch;
case _off:
	SysTaskDelay (10);
	e8= 0x011E;
	break;
case _offandlock:
	e8= 0x0112;
	break;
case _clock:
	e8= 0x0500;
	break;
case _softreset:
	SysReset ();
	break;
case _attention:
	if (POP->OS>=0x04003000) {
		FtrSet (crea, _Fpostpone, _attention);
	} else {
		FrmAlert (2009);
		goto afteralert;
	}
	break;
case _graffiti:
	SysGraffitiReferenceDialog (0xFF);
	break;
case _abc:
	e8= 272;
	break;
case _123:
	e8= 273;
	break;
case _toolbar:
	e8= 0x0106;
	break;
case _menu:
	e8= 261;
	break;
default:
	FrmAlert (2006);
	goto afteralert;
}
if (e8!=0) {
	UInt32 trig= 0;
	FtrGet (crea, _Ftrig, &trig);
	if (e8==trig) FtrSet (crea, _Fignore1, trig);
	EvtEnqueueKey (e8, 0, 8);
}
POP->stop= true;
#ifdef STOPREASON
POP->stopreason= 2;
#endif
POP->exit= 1;
}

Boolean LSA (TPOP* POP, char c) {
UInt16 i;
if ((c>='A') && (c<='Z')) c-= ('A'- 'a');
for (i= 0; i<POP->cnt; i++) {
	if (POP->M[i+ 1].shortcut==c) {
		Boolean vis= !POP->M[i+ 1].data.body.invisible;
		UInt32 s= TimGetTicks ();
		if (vis) {
			HI (POP, i);
		} else {
			HI (POP, -1);
		}
		LC (POP, i);
		if (vis) while (TimGetTicks ()- s < SysTicksPerSecond ()/8);
		return true;
	}
}
return false;
}

Boolean HE1 (EventType* e, TPOP* POP) {
switch (e->eType) {
case nilEvent: {
	UInt32 now= TimGetTicks ();
	if ((!POP->b_m) && (now> POP->actick+ POP->b_d)) POP->b_m= 1;
	SB (POP);
	if (!KD ()) POP->wasnil= true;
	if (POP->autohide && (now> POP->actick+ POP->autohide)) {
		SndPlaySystemSound (7);
		POP->exit= 1;
		POP->stop= true;
#ifdef STOPREASON
		POP->stopreason= 3;
#endif
		return true;
	}
	if (POP->stoponnil) {
		POP->stoponnil= false;
		SndPlaySystemSound (7);
		POP->stop= true;
#ifdef STOPREASON
		POP->stopreason= 4;
#endif
		return true;
	}
	}
	return false;
case penDownEvent: {
	RectangleType RB= {POP->b_x+ POP->x, POP->b_t+ POP->y, b_w/2, b_h/2};
	if ((POP->b_m==1) && (RctPtInRectangle (e->screenX, e->screenY, &RB))) {
		FrmAlert (2100);
		WinSetActiveWindow (POP->S);
		POP->stay= true;
		return true;
	}
	if (POP->b_m==1) POP->b_m= 3;
	POP->hi= -1;
	HI (POP, II (POP, e->screenX, e->screenY));
	POP->noted= true;
	return false;
	}
case penMoveEvent:
	HI (POP, II (POP, e->screenX, e->screenY));
	return false;
case penUpEvent:
	if ((POP->noted) && (EvtKeyQueueEmpty ()))
		POP->stoponnil= true;
	POP->wasnoted= POP->noted;
	POP->noted= false;
	return false;
case keyDownEvent: {
	WChar c= e->data.keyDown.chr;
	UInt32 trig= 0;
	POP->stoponnil= false;
	if (!(e->data.keyDown.modifiers & 8)) {
		if (LSA (POP, c)) return true;
		if ((POP->M->data.header.canhilite) && ((c=='\n') || (c==' ') || (c==0x001D))) {
			launchc:
			LC (POP, POP->hi);
			return true;
		}
		if (c==0x001B) return false;
		if (c==0x001E) goto goup;
		if (c==0x001F) goto godown;
		SndPlaySystemSound (7);
		POP->stop= true;
#ifdef STOPREASON
		POP->stopreason= 5;
#endif
		return true;
	}
	if (c==0x013D) return true; //totally ignored
	if (((c==0x0136) || (c==0x0128)) && (!FtrGet (crea, _FignoreCenterUp, &trig))) {
		FtrUnregister (crea, _FignoreCenterUp);
		return true;
	}
	if (c==0x0132) c= 0x000B;
	if (c==0x0133) c= 0x000C;
	if (POP->b_m==1) POP->b_m= 3;
	FtrGet (crea, _Ftrig, &trig);
// {
// 	char a[32]; char b[32];
// 	StrIToH (a, trig);
// 	StrIToH (b, c);
// 	FrmCustomAlert (10021, a, "\n^-trigger, pressed-v\n", b);
// }
	switch (c) { //more important than double-trigger-press action:
	case 0x000B: case 0x1700: case 0x0132: case 0x010C: case 0x012E: case 0x161B: goup:
		if ((c==trig) && (e->data.keyDown.modifiers & 0x0040) && (TimGetTicks ()- POP->popuptick < SysTicksPerSecond()/2)) return true;
		if (!POP->M->data.header.canhilite)
			return true;
		if (POP->ret!=-1) {
			POP->hi= POP->ret;
			POP->ret= -1;
		}
		do if (--POP->hi<=-1) POP->hi= POP->cnt- 1; while ((POP->M[POP->hi+1].data.body.invisible) || (POP->M[POP->hi+1].type==_splitter) || ((POP->M[POP->hi+1].type>=_0_status) && (POP->M[POP->hi+1].type<_x_status)));
		HI (POP, POP->hi);
		return true;
	case 0x000C: case 0x1701: case 0x0133: case 0x103: case 0x012F: case 0x161C: godown:
		if ((c==trig) && (e->data.keyDown.modifiers & 0x0040) && (TimGetTicks ()- POP->popuptick < SysTicksPerSecond()/2)) return true;
		if (!POP->M->data.header.canhilite)
			return true;
		if (POP->ret!=-1) {
			POP->hi= POP->ret;
			POP->ret= -1;
		}
		do if (++POP->hi==POP->cnt) POP->hi= 0; while ((POP->M[POP->hi+1].data.body.invisible) || (POP->M[POP->hi+1].type==_splitter) || ((POP->M[POP->hi+1].type>=_0_status) && (POP->M[POP->hi+1].type<_x_status)));
		HI (POP, POP->hi);
		return true;
	}
	if (c==trig) {
		if (e->data.keyDown.modifiers & 0x0040) return true;
		WaitNoKeyDown ();
		if ((POP->doubletp==1) && (POP->hi!=-1)) goto launchc;
		if (POP->doubletp==2) {
			LC (POP, -_home);
			return true;
		}
		SndPlaySystemSound (7);
		if (POP->doubletp==4) {
			LC (POP, -3);
			return true;
		}
		if (POP->doubletp==3) {
			if (c==0x0208) {
				LC (POP, -_off);
				return true;
			} else {
				FtrSet (crea, _Fnoignore, 1);
			}
		}
		POP->exit= 1;
		POP->stop= true;
		return true;
	}
	switch (c) {
	case 0x160D:
		if (e->data.keyDown.keyCode!=0xE006)
			break;
		//nobreak;
	case 0x012D: case 0x010D: case 0x0383: case 0x0101: //ignored->false:
		return false;
	case 0x1A0D: case 0x1705: case 0x0317: //ignored->true:
		return true;
	case 0x0503: {
		UInt16 kc= e->data.keyDown.keyCode;
		if (kc & 0x001F)
			return true;
		if ((kc & 0x1F00) && (!POP->wasnil))
			return true;
		if (kc & 0x1800) //((kc & 0x1000) || (kc & 0x0800))
			goto lanc;
		if (kc & 0x0400)
			goto popstop;
		return true;
		}
	case 0x1706: case 0x1608: case 0x0310:
	case 0x0135: case 0x1801: case 0x1709:
	case 0x0107: case 0x000D: case 0x161F:
	case 0x0136:
	lanc:
		if (POP->hi!=-1) goto launchc;
		//nobreak, must be last!
	}
	if (e->data.keyDown.modifiers & 64) return true;
	popstop:
	POP->stop= true;
#ifdef STOPREASON
	POP->stopreason= 6;
{
char c[10];
char d[10];
char f[10];
StrIToH ((char*)&c, e->data.keyDown.chr);
StrIToH ((char*)&d, e->data.keyDown.keyCode);
StrIToH ((char*)&f, e->data.keyDown.modifiers);
StrCat ((char*)&c, "\n");
StrCat ((char*)&d, "\n");
FrmCustomAlert (10021,(char*)&c,(char*)&d,(char*)&f);
}
#endif
	SndPlaySystemSound (7);
	switch (c) {
	case 0x160D:
		if (e->data.keyDown.keyCode!=0x0105)
			return true;
	case 0x0105:
		LC (POP, -2);
		return true;
	case 0x1707:
	case 0x1706: case 0x1608: case 0x0310: 
	case 0x0503: case 0x0134: case 0x1800: 
	case 0x1708: case 0x0130: case 0x0131: 
	case 0x000D: case 0x1A03: case 0x0135: 
	case 0x1709: case 0x161F: case 0x0136: //buttons that do nothing when no selection (?)
		return true;
/* a niech se zmienia zak³adki/kategorie w launcherze :-)
	case 0x0108: {
		UInt32 creator;
		UInt32 vis= 0;
		FtrGet (crea, _Fvis, &vis);
		DmDatabaseInfo (0, vis, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, &creator);
		if (creator==PrefGetPreference (prefLauncherAppCreator)) {
			POP->exit= 1;
			return true;
		}
		}
		//break;
*/
	}
/*{
char u[9]; StrIToA (&u, c);
char v[9]; StrIToA (&v, trig);
FrmCustomAlert (10021,&u,"\n",&v);
}*/
	EvtAddEventToQueue (e);
	return true;
	}
}
return false;
}

void HE2 (EventType* e, TPOP* POP) {
UInt16 x, y;
switch (e->eType) {
case penUpEvent:
	POP->pass= 0;
	if (FtrGet (crea, _Fpass, &(POP->pass)))
		POP->pass= 0;
	if ((!POP->wasnoted) && (!POP->pass)) return;
	FtrUnregister (crea, _Fpass);
	x= e->screenX;//data.penUp.end.x;
	y= e->screenY;//data.penUp.end.y;
	if (RctPtInRectangle (x, y, &POP->R)) {
		if (POP->M->data.header.canhilite) {
			Int16 ind= II (POP, x, y);
			LC (POP, ind);
			POP->pass= 0;
			if (POP->b_m==3) POP->b_m= 1;
		} else goto behind;
	} else {
		UInt16 bc;
		UInt16 i;
		PenBtnInfoType* b= (PenBtnInfoType*)EvtGetPenBtnList (&bc);
		for (i= 0; i<bc; i++) {
			if (RctPtInRectangle (x, y, &(b[i].boundsR))) return;
		}
		if ((POP->mid!=0) && (POP->via)) {
			FtrSet (crea, _Fpass, POP->mid);
			EvtAddEventToQueue (e);
#ifdef STOPREASONMORE
POP->stopreason= 1001;
#endif
			POP->stop= true;
			return;
		}
		behind:
		SndPlaySystemSound (7);
		POP->stop= true;
#ifdef STOPREASON
		POP->stopreason= 7;
#endif
	}
	return;
case winEnterEvent:
	if (POP->oldw==0) {
		if (VD (POP)) {
			IB (POP);
			HI (POP, POP->hi);
		}
	} else if (!POP->highlight) {
		POP->stay= false;
		POP->hi= -1;
		HI (POP, -1);
	}
	return;
case winDisplayChangedEvent:
	{
	Coord nw, nh;
	WinGetDisplayExtent (&nw, &nh);
	if ((nw==POP->sw) && (nh==POP->sh)) return;
	POP->exit= 1;
	POP->stop= true;
#ifdef STOPREASON
	POP->stopreason= 8;
#endif
	return;
	}
case appStopEvent:
	EvtAddUniqueEventToQueue (e, 0, true);
	return;
}
}

//activate menu
UInt8 AM (Tmid mid, Int16 x, Int16 y, Int16 ri, Int16 di) {//recent
TPOP* POP;
EventType e;
UInt32 tps;
UInt8 exit;
FtrSet (crea, _Fon, 1);
#ifdef trial
{
	UInt32 f= 0;
	UInt32 typ= 'ownr'- 1274;
	Boolean isf= FtrGet (crea, _Ftrial2, &f); //minuty-1
	UInt32 p= 0;
	UInt16 rSiz= 4;
	UInt32 n;
	UInt32 alt;
	Boolean nie2;
	typ-= 61;
	isf= !isf;
	if (PrefGetAppPreferences (typ+ 1274+ 61, 255, &p, &rSiz, true)==noPreferenceFound) goto inc;//minuty
	if (isf) {
		if (p!=f+1) goto inc;
	} else FtrSet (crea, _Ftrial2, p- 1);
	f= p;
	n= TimGetSeconds ();
	alt= 2202- 782;
	if (n<f- (1*60*60)) goto inc;
	f+= 918000;//(255*60*60);
	alt--;
	nie2= false;
	if (f<n) {
		WaitAlert (alt+ 782+ 1); //2202
		FtrUnregister (crea, _Fon);
		return 0;
		inc:
		f= 0;
		FtrGet (crea, _Ftrial3, &f);
		f++;
		nie2= true;
		FtrSet (crea, _Ftrial3, f);
	}
	if (!((f>=n) || nie2)) goto inc;
}
#endif
POP= M0 (sizeof(TPOP));
POP->ri= ri;
POP->di= di;
POP->mid= mid;
POP->x= x;
POP->y= y;
tps= SysTicksPerSecond ()/20;
if (IM (POP)) {
	OD (POP);
	do {
		els:
		EvtGetEvent (&e, tps);
		if (!HE1 (&e, POP))
		if (!SysHandleEvent (&e))
			HE2 (&e, POP);
		if (POP->stay==true) {
			POP->stay= false;
			goto els;
		}
#ifdef STOPREASONMORE
if (e.eType==appStopEvent) POP->stopreason= 1000;
#endif
	} while ((e.eType!=appStopEvent) && (!POP->stop));
}
#ifdef STOPREASON
{
char c[9]; StrIToA ((char*)&c, POP->stopreason);
FrmCustomAlert (10021,"stop reason code:","\n",(char*)&c);
}
#endif
exit= POP->exit;
if (exit==0) exit= (e.eType==appStopEvent);
UM (POP);
MemPtrFree (POP);

return exit;
}

/***************************************/

void VFSDel () {//twin//Uninstall Manager
VFSRunningApp vra;
UInt16 vraSize= 0;
Int16 prefResult= PrefGetAppPreferences (crea, 1, &vra, &vraSize, false);
if ((prefResult==noPreferenceFound) || (vraSize!=sizeof(VFSRunningApp))) return;
PrefGetAppPreferences (crea, 1, &vra, &vraSize, false);
if (vra.vfsState==vsDeleteMe) {
	UInt16 cardNo;
	LocalID dbID;
	DmSearchStateType searchState;
	Err err= DmGetNextDatabaseByTypeCreator (true, &searchState, 'appl', 'Rhiv', true, &cardNo, &dbID);
	if (err==0) {
		UInt32 type= 'Rhiv';
		DmSetDatabaseInfo (cardNo, dbID, 0, 0, 0, 0, 0, 0, 0, 0, 0, &type, 0); //Uninstall Manager
		DmDeleteDatabase (cardNo, dbID);
	}
	PrefSetAppPreferences (crea, 1, 0, 0, 0, false);
}
}

//appswitch
void AS (SysNotifyAppLaunchOrQuitType* a, Boolean launch) {
char name[32];
UInt32 type, creator, avoid;
UInt32 lastCreator;
UInt16 attr;
if ((!launch) && (!FtrGet (crea, _FignoreLocalDB, &avoid)) && (avoid==a->dbID)) {
	FtrUnregister (crea, _FignoreLocalDB);
	return;
}
//DmDatabaseInfo (a->cardNo, a->dbID, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
DmDatabaseInfo (a->cardNo, a->dbID, (char*)&name, &attr, 0, 0, 0, 0, 0, 0, 0, &type, &creator);
if ((type!='appl') || (creator=='Rhiv') || (creator=='Rhbv') || (creator=='McPv')) {
	return;
}
if (launch) {
	VFSDel ();
}
if (FtrGet (crea, _FlastCreator, &lastCreator) || (lastCreator!=creator) || (lastCreator==crea) || (creator==crea)) {
	Boolean addtorecent= true;
	FtrSet (crea, _FlastCreator, creator);
	if ((attr & 0x0100) && (!GP (_Phidden))) {
		addtorecent= false;
	}
	if (addtorecent) {
		UInt16 ec;
		UInt32* E= _1LAE (&ec, 1);
		if (E) {
			UInt16 i;
			for (i= 0; i<ec; i++) if (E[i]==creator) {
				addtorecent= false;
				break;
			}
			MemPtrFree (E);
		}
	}
	if (addtorecent) {
		T1recent* RR= _1LAR ();
		if (RR[0].creator!=creator) {
			UInt16 i;
			DmOpenRef db;
			i= 1;
			do {
				if (RR[i].creator==creator) break;
			} while (++i<_RC- 1);
			MemMove (&(RR[1]), &(RR[0]), i* sizeof(T1recent));
			RR[0].creator= creator;
			db= DmOpenDatabase (a->cardNo, a->dbID, dmModeReadOnly);
			if (db) {
				MemHandle h= DmGet1Resource ('tAIN', 1000);
				if (h!=0) {
					StrCopy ((char*)RR, MemHandleLock (h));
					MemHandleUnlock (h);
					DmReleaseResource (h);
				} else StrCopy ((char*)RR, (char*)&name);
				DmCloseDatabase (db);
			}
			PrefSetAppPreferences (crea, 0, 0, RR, sizeof(T1recent)*_RC, false);
		}
		MemPtrFree (RR);
	}
	if (launch) {
		UInt16 cardNo;
		LocalID dbID;
		UInt16 ec;
		UInt32* E;
		UInt16 i;
		SysCurAppDatabase (&cardNo, &dbID);
		hede0 (cardNo, dbID);
		E= _1LAE (&ec, 2);
		for (i= 0; i<ec; i++) if (E[i]==creator) goto skipreg;
		hede1 (cardNo, dbID);
		skipreg:
		if (E!=0) MemPtrFree (E);
	}
}

/*{
UInt16 cardNo;
LocalID dbID;
RectangleType ree= {0, 0, 400, 400};
IndexedColorType obc= WinSetBackColor (0xFF);
IndexedColorType otc= WinSetTextColor (0);
//SysCurAppDatabase (&cardNo, &dbID);
DmDatabaseInfo (a->cardNo, a->dbID, (char*)&name, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
WinEraseRectangle (&ree, 0);
WinDrawChars (name, StrLen(name), 0, 0);
WinSetTextColor (otc);
WinSetBackColor (obc);
SysTaskDelay (100);
WinEraseRectangle (&ree, 0);
SysTaskDelay (10);
}*/

}

//process notification
UInt16 PN (SysNotifyParamType* n) {
Boolean launch= false;
switch (n->notifyType) {
case 'hapl':
	launch= true;
	//nobreak
case 'hapq':
	AS ((void*)(n->notifyDetailsP), launch);
	//return 0;
}
return 0;
}

/*
UInt32 DIV (UInt32 a, UInt32 b) {
return a/ b;
}

UInt32 MUL (UInt32 a, UInt32 b) {
return a* b;
}
*/
