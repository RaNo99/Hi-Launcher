#define use_HPMoveGSI

#define ALLOW_ACCESS_TO_INTERNALS_OF_WINDOWS
#define ALLOW_ACCESS_TO_INTERNALS_OF_FORMS

#include "PalmTypes.h"
#include "SonyCLIE.h"
//#include "SonySilkLib.h"
#include "VFSmgr.h"
#include "FeatureMgr.h"
#include "PenInputMgr.h"
#include "StringMgr.h"
#include "Bitmap.h"
#include "SysUtils.h"
#include "UIColor.h"
#include "ExgMgr.h"
#include "Form.h"
#include "KeyMgr.h"
#include "AppLaunchCmd.h"
#include "PhoneLookup.h"
#include "SysEvtMgr.h"
#include "Menu.h"
#include "TimeMgr.h"
#include "Keyboard.h"
#include "HsNavCommon.h"
#include "FrmGlue.h"

#include "rns_hp.h"
#include "Hi-Launcher.h"
#include "Hi-Launcher.set.glob.c"

#define g_sonySilkRefNum RUN.sonySilkRefNum
#define g_CollapseAPIVersion RUN.CollapseAPIVersion
#include "rns_hp.c"

/***************************************/

Boolean H1100 (EventPtr e);

/***************************************/

Int16 CIa (UInt32 creator) {
UInt16 i;
if (creator==0) return -1;
for (i= 0; i<RUN.aac; i++)
	if (RUN.aap[i].creator==creator)
		return i;
return -1;
}

Int16 CIc (UInt32 creator) {
UInt16 i;
if (creator==0) return -1;
for (i= 0; i<RUN.cac; i++)
	if (RUN.cap[i].creator==creator)
		return i;
return -1;
}

Int16 CId (UInt32 creator) {
UInt16 i;
if (creator==0) return -1;
for (i= 0; i<RUN.dac; i++)
	if (RUN.dap[i].creator==creator)
		return i;
return -1;
}

Int16 CIp (UInt32 creator) {
UInt16 i;
if (creator==0) return -1;
for (i= 0; i<RUN.pac; i++)
	if (RUN.pap[i].creator==creator)
		return i;
return -1;
}

void LB (UInt16 s, UInt16 c) {
UInt16 i;
RUN.DD= DD ();
if (RUN.DD) s+= 300;
bmph= M0 (c* (sizeof(MemHandle)));
for (i= 0; i<c; i++) {
	bmph[i]= DmGet1Resource (bitmapRsc, s+ i);
}
}

void UB (UInt16 c) {
UInt16 i;
for (i= 0; i<c; i++) {
	if (bmph[i]!=0) {
		DmReleaseResource (bmph[i]);
	}
}
MemPtrFree (bmph);
}

Ttype_v2 T0_2 (Ttype_v0 old) {
Ttype_v0 types_v0[]= {_untyped_v2, _submenu_v2, _application_v2, _special_v2, _splitter_v2, _recent_v2, _home_v2, _find_v2, _beamreceive_v2, _backlight_v2, _brightness_v2, _contrast_v2, _hotsync_v2, _off_v2, _offandlock_v2, _clock_v2, _abc_v2, _123_v2, _toolbar_v2, _menu_v2, _autooff_v2, _sound_v2};
Ttype_v2 new= _untyped_v2;
if (old<_unknown_v0)
	new= types_v0[old];
return new;
}

Ttype T2_3 (Ttype_v2 old) {
Ttype new= old;
if ((old>=_special_v2) && (old<=_splitter_v2)) {
	new+= 3;
}
if (old==_url_v2) new= _url;
if (old==_dial_v2) new= _dial;
if ((old>=_home_v2) && (old<=_modemhotsync_v2)) {
	new++;
}
if ((old>=_clock_v2) && (old<=_graffiti_v2)) {
	new+= 2;
}
if (old==_redial_v2) new= _redial;
if (old==_bluetooth_v2) new= _bluetooth;
if (old==_network_v2) new= _network;
if (old==_radio_v2) new= _radio;
if (old==_autooff_v2) new= _autooff;
if (old==_sound_v2) new= _sound;
return new;
}

void* LR (DmResType type, DmResID resID) {
MemHandle h= DmGet1Resource (type, resID);
if (h==0) return 0;
{
UInt32 siz= MemHandleSize (h);
void* p2= MemPtrNew (siz);
void* p1= MemHandleLock (h);
MemMove (p2, p1, siz);
MemHandleUnlock (h);
DmReleaseResource (h);
return p2;
}
}

Titem* LM () {
Titem* M= LR ('tMEN', RUN.M->mid);
if (M==0) {
	empty:
	M= M0 (sizeof(Titem));
	M->color[0]= dc0;
	M->color[1]= dc1;
	M->color[2]= dc2;
} else {
	UInt16 v= M->data.header.version;
	UInt16 i;
	if (v>MENUVERSION) {
		MemPtrFree (M);
		FrmAlert (2014);
		goto empty;
	}
	for (i= 0; i<M->data.header.count; i++) {

		//live MENUVERSION conversion
		if (v==0) M[i+1].type= T0_2 (M[i+1].type);
		if (v<3) M[i+1].type= T2_3 (M[i+1].type);

		if ((M[i+1].type==_application) || (M[i+1].type==_card)) {
			if (CIa (M[i+1].data.body.creator)!=-1) {
				M[i+1].type= _application;
			} else {
				if (CIc (M[i+1].data.body.creator)!=-1) {
					M[i+1].type= _card;
				} else {
					M[i+1].data.body.creator= 0;
				}
			}
		}
		if ((M[i+1].type==_desk) && (CId (M[i+1].data.body.creator)==-1)) M[i+1].data.body.creator= 0;
		if ((M[i+1].type==_panel) && (CIp (M[i+1].data.body.creator)==-1)) M[i+1].data.body.creator= 0;
	}
}
M->data.header.version= MENUVERSION;
return M;
}

void DM () {
UInt16 ind= DmFindResource (RUN.tmp, 'tMEN', RUN.M->mid, 0);
if (ind!=0xFFFF) DmRemoveResource (RUN.tmp, ind);
}

void SM () {
UInt16 siz= (RUN.M->items->data.header.count+ 1)* sizeof(Titem);
MemHandle h= DmGet1Resource ('tMEN', RUN.M->mid);
if (h==0) h= DmNewResource (RUN.tmp, 'tMEN', RUN.M->mid, siz); else h= DmResizeResource (h, siz);
DmWrite (MemHandleLock (h), 0, RUN.M->items, siz);
MemHandleUnlock (h);
DmReleaseResource (h);
}

Tcid NC (UInt32 creator) {
UInt16 id= 1000;
UInt16 i;
MemHandle h;
for (i= 1; i<1000; i++) {
	if (DmFindResource (RUN.tmp, creator, i, 0)==0xFFFF) {
		id= i;
		break;
	}
}
h= DmNewResource (RUN.tmp, creator, id, 1);
DmSet (MemHandleLock (h), 0, 1, 0);
MemHandleUnlock (h);
DmReleaseResource (h);
return id;
}

void DC (UInt32 creator, Tcid cid) {
UInt16 ind= DmFindResource (RUN.tmp, creator, cid, 0);
if (ind!=0xFFFF) DmRemoveResource (RUN.tmp, ind);
}

void SC (UInt32 creator, Tcid cid, char* c) {
UInt16 siz= StrLen (c)+ 1;
MemHandle h= DmGet1Resource (creator, cid);
if (h==0) h= DmNewResource (RUN.tmp, creator, cid, siz); else h= DmResizeResource (h, siz);
DmWrite (MemHandleLock (h), 0, c, siz);
MemHandleUnlock (h);
DmReleaseResource (h);
}

/***************************************/

Int16 CF (char* p1, char* p2, Int32 other) {
return StrCompare (p1, p2);
}

void* CR (FileRef file, UInt32 type, UInt16 id) {
MemHandle h;
if (VFSFileDBGetResource (file, type, id, &h))
	return 0;
	{
UInt16 siz= MemHandleSize (h);
void* c= MemPtrNew (siz);
void* d= MemHandleLock (h);
if (c) MemMove (c, d, siz);
MemHandleFree (h);
return c;
}
}

void CC (FileRef file, char* tAIN, BitmapPtr* tAIB1000, BitmapPtr* tAIB1001) {
void* t= CR (file, 'tAIN', 1000);
if (t!=0) {
	StrCopy (tAIN, t);
	MemPtrFree (t);
}
if (tAIB1000!=0)
	(*tAIB1000)= CR (file, 'tAIB', 1000);
if (tAIB1001!=0)
	(*tAIB1001)= CR (file, 'tAIB', 1001);
}

void LACL (matchInfoType* m) {
UInt16 vols[m_card];
UInt16 i;
UInt32 ia;
UInt16 vol;
UInt16 volc= 0;
for (i=0; i<m_card; i++) vols[i]= 0;
if (FtrGet (sysFileCVFSMgr, 0, &ia)) return;
ia= vfsIteratorStart;
while ((ia!=vfsIteratorStop) && (volc<m_card)) {
	if (VFSVolumeEnumerate (&vol, &ia)) break;
	vols[volc++]= vol;
}
{
char* prc= ".prc";
char* path= MemPtrNew (m_path);
FileInfoType info= {0, MemPtrNew(256), 256};
MemHandle h;
UInt16 i;
if (m==0) {
	h= MemHandleNew (16);
	RUN.cac= 0;
}
for (i=0; i<volc; i++) {
	UInt16 pathlen= m_path- 256;
	FileRef dir, file;
	char* namepart;
	if (VFSGetDefaultDirectory (vols[i], prc, path, &pathlen)) continue;
	namepart= StrChr (path, 0)- 1;
	if (*namepart=='/') *namepart= 0;
	if (VFSFileOpen (vols[i], path, vfsModeRead, &dir)) continue;
	*namepart= '/';
	namepart++;
	*namepart= 0;
	ia= vfsIteratorStart;
	while (ia!=vfsIteratorStop) {
		UInt16 len;
		UInt32 type, creator;
		char nice[dmDBNameLength];
		UInt16 attr;
		UInt16 cmp= 0x0100;
		if (VFSDirEntryEnumerate (dir, &ia, &info))
			break;
		len= StrLen (info.nameP);
		if ((len<5) || (StrCaselessCompare (prc, &(info.nameP[len- 4])))) continue;
		StrCopy (namepart, info.nameP);
		if (VFSFileOpen (vols[i], path, vfsModeRead, &file)) continue;
		if ((m!=0) || RUN.hidden) cmp= 0;
		if ((!VFSFileDBInfo (file, (char*)&nice, &attr, 0, 0, 0, 0, 0, 0, 0, &type, &creator, 0)) && (type=='appl') && (!(attr & cmp))) {
			if (m==0) {
				CC (file, (char*)&nice, 0, 0);
				MemHandleResize (h, sizeof(CardItemType)* (1+ RUN.cac));
				RUN.cap= MemHandleLock (h);
				StrCopy ((char*)&(RUN.cap[RUN.cac].name), (char*)&nice);
				RUN.cap [RUN.cac].creator= creator;
				MemHandleUnlock (h);
				RUN.cac++;
			} else {
				if (creator==m->creator) {
					BitmapPtr* B1000= 0;
					BitmapPtr* B1001= 0;
					m->path= MemPtrNew (m_path);
					StrCopy (m->path, path);
					if ((UInt32)(m->icon1000)==1)
						B1000= &(m->icon1000);
					if ((UInt32)(m->icon1001)==1)
						B1001= &(m->icon1001);
					CC (file, (char*)&nice, B1000, B1001);
					StrCopy ((char*)&(m->name), (char*)&nice);
					m->vol= vols[i];
					ia= vfsIteratorStop;
				}
			}
		}
		VFSFileClose (file);
	}
	VFSFileClose (dir);
	if ((m!=0) && (m->path!=0)) break;
}
MemPtrFree (info.nameP);
MemPtrFree (path);
if (m!=0) return;
RUN.cap= MemHandleLock (h);
SysQSort (RUN.cap, RUN.cac, sizeof(CardItemType), (CmpFuncPtr)&CF, 0);
}
}

void WB (DmOpenRef d2, UInt32 creator, DmResID id, BitmapPtr p1) {
if (DmFindResource (d2, creator, id, 0)==0xFFFF) {
	UInt32 ps= MemPtrSize (p1);
	MemHandle h2= DmNewResource (d2, creator, id, ps);
	void* p2= MemHandleLock (h2);
	DmWrite (p2, 0, p1, ps);
	MemHandleUnlock (h2);
	DmReleaseResource (h2);
}
}

Boolean CB (DmOpenRef d2, UInt32 creator, DmResID id) {
MemHandle h1;
if (DmFindResource (d2, creator, id, 0)!=0xFFFF)
	return true;
h1= DmGet1Resource ('tAIB', id);
if (h1==0) return false;
{
UInt32 hs= MemHandleSize (h1);
MemHandle h2= DmNewResource (d2, creator, id, hs);
void* p1= MemHandleLock (h1);
void* p2= MemHandleLock (h2);
DmWrite (p2, 0, p1, hs);
MemHandleUnlock (h2);
MemHandleUnlock (h1);
DmReleaseResource (h2);
DmReleaseResource (h1);
return true;
}
}

Boolean CBL (UInt32 type, Titem* M, DmOpenRef dest, UInt16 i1) {
Boolean result= false;
Boolean b1000= false;
Boolean b1001= false;
UInt32 creator;
DmOpenRef src;
if (M[i1].noicon) {
	result= true;
	goto after;
}
creator= M[i1].data.body.creator;
src= DmOpenDatabaseByTypeCreator (type, creator, dmModeReadOnly);
if (src) {
	b1000= CB (dest, creator, 1000);
	b1001= CB (dest, creator, 1001);
	DmCloseDatabase (src);
	result= true;
}
after:
M[i1].b1000= b1000;
M[i1].b1001= b1001;
return result;
}

void CBC (Titem* M, DmOpenRef dest, UInt16 i1) {
Boolean b1000;
Boolean b1001;
matchInfoType m;
m.path= 0;
m.creator= M[i1].data.body.creator;
m.icon1000= (void*)1;
m.icon1001= (void*)1;
if (!M[i1].noicon) {
	LACL (&m);
	if (m.path!=0) MemPtrFree (m.path);
}
b1000= ((UInt32)(m.icon1000)>1);
b1001= ((UInt32)(m.icon1001)>1);
if (b1000) {
	WB (dest, M[i1].data.body.creator, 1000, m.icon1000);
	MemPtrFree (m.icon1000);
}
if (b1001) {
	WB (dest, M[i1].data.body.creator, 1001, m.icon1001);
	MemPtrFree (m.icon1001);
}
M[i1].b1000= b1000;
M[i1].b1001= b1001;
}

void PMx (Titem* M, DmOpenRef dest) {
UInt16 v= M->data.header.version;
Boolean canhilite= false;
Boolean noicons= true;
UInt16 c_n= 0;
UInt16 c_c= 0;
UInt16 c_s= 0;
UInt16 c_r= 0;
UInt16 c_g= 0;
UInt16 c_d= 0;
UInt16 cw= 20; //menu will never be narrower
UInt16 cw2= 10; //small menu will never be narrower
FontID oldf= FntGetFont ();
UInt16 i;
if (v<MENUVERSION) { //MENUVERSION conversion on request:
	for (i=0; i<M->data.header.count; i++) {
		Ttype type= M[i+1].type;
		if (v==0) {
			type= T0_2 (type);
		}
		type= T2_3 (type);
		M[i+1].type= type;
	}
	M->data.header.version= MENUVERSION;
}
for (i= 0; i<M->data.header.count; i++) {
	Ttype type= M[i+1].type;
	Boolean sta;
	sta= ((type>=_0_status) && (type<_x_status));
	if ((!sta) && (type!=_splitter) && (!M[i+ 1].data.body.invisible)) canhilite= true;
	if (type==_card) {
		type= _application;
		M[i+1].type= _application;
	}
	if (type==_application) {
		if (!CBL ('appl', M, dest, i+1))
			CBC (M, dest, i+1);
	}
	if (type==_desk) {
		CBL ('DAcc', M, dest, i+1);
	}
	if (type==_panel) {
		CBL ('panl', M, dest, i+1);
	}
	if (type==_recent) {
		M[i+1].data.body.index= c_r;
		c_r++;
	}
	if (type==_redial) {
		M[i+1].data.body.index= c_d;
		c_d++;
	}
	if (sta) {
		M[i+1].data.body.index= c_g;
		c_g++;
	}
	if (type==_splitter) {
		if (RUN.depth>=8) {
			IndexedColorType tc, bc, tl, bl;
			Boolean pn= (i==0);
			Boolean nn= (i==M->data.header.count- 1);
			Boolean ps= ((!pn) && (M[i].type==_splitter));
			Boolean ns= ((!nn) && (M[i+2].type==_splitter));
			if (pn) {
				tc= M[i+1].color[2];
			} else {
				if (ps) {
					tc= MC (M[i+1].color[2], M[i].color[2]);
				} else {
					tc= M[i].color[0];
				}
			}
			if (nn) {
				bc= M[i+1].color[2];
			} else {
				if (ns) {
					bc= MC (M[i+2].color[2], M[i+1].color[2]);
				} else {
					bc= M[i+2].color[0];
				}
			}
			tl= MC (tc, M[i+1].color[2]);
			bl= MC (M[i+1].color[2], bc);
			M[i+1].data.body.tl= tl;
			M[i+1].data.body.bl= bl;
			M[i+1].data.body.tc= tc;
			M[i+1].data.body.bc= bc;
		}
	} else if (!M[i+1].noicon) {
		noicons= false;
	}
	/* VAR */ {
		UInt8 hstyle= 0;
		char* c= GC ('tSTR', M[i+1].cid);
		UInt16 clen= StrLen (c);
		UInt16 w, w2;
	
		FntSetFont (M[i+1].font);
		w= FntCharsWidth (c, clen);
	
		FntSetFont ((M[i+1].font*5)+2);
		w2= FntCharsWidth (c, clen);
	
		if ((type>=_0_toggle) && (type<_x_toggle)) {
			w+= 14;
			w2+= 14;
		}
		if (type==_submenu) {
			w+= 10;
			w2+= 10;
		}
		if (w>cw) cw= w;
		if (w2>cw2) cw2= w2;
	
		if (type==_splitter) {
			if (c[0]==0) {
				c_s++;
				hstyle= 2;
			} else {
				c_c++;
				hstyle= 1;
			}
		} else if (!M[i+1].data.body.invisible) c_n++;
		M[i+1].hstyle= hstyle;
		M[i+1].fcc= *c;
		/* VAR */ {
			char s= M[i+1].shortcut;
			UInt8 si= 0;
			if (s!=0) {
				char* n= MemPtrNew (m_caption+ 1);
				char* d;
				Int32 io;
				StrToLower (n, c);
				d= StrChr (n, s);
				io= -1;
				if (d!=0) io= d- n;
				MemPtrFree (n);
				si= io+ 1;
			}
			M[i+1].shortcut_i= si;
		}
		MemPtrFree (c);
	}
}
M->data.header.count_n= c_n;
M->data.header.count_c= c_c;
M->data.header.count_s= c_s;
M->data.header.maxcw= cw;
M->data.header.maxcw2= (cw2>>1);
M->data.header.count_r= c_r;
M->data.header.count_g= c_g;
M->data.header.count_d= c_d;
M->data.header.canhilite= canhilite;
M->data.header.noicons= noicons;
FntSetFont (oldf);
}

void PM (DmOpenRef list, DmOpenRef dest) {
UInt16 rc= DmNumResources (list);
Int16 i;
for (i= 0; i<rc; i++) {
	DmResType type;
	DmResourceInfo (list, i, &type, 0, 0);
	if (type=='tMEN') {
		MemHandle h= DmGetResourceIndex (list, i);
		Titem* M= MemHandleLock (h);
		UInt32 mps= MemPtrSize (M);
		Titem* m= MemPtrNew (mps);
		MemMove (m, M, mps);
		PMx (m, dest);
		DmWrite (M, 0, m, mps);
		MemPtrFree (m);
		MemHandleUnlock (h);
		DmReleaseResource (h);
	}
}
}

void B1 () {
SO (1020);
//SO (1021);
}

void B0 () {
HO (1020);
//HO (1021);
}

void ITMP () {
DmOpenRef db;
RUN.tmp= LDB (&(DBI[DBtemp]), dmModeReadWrite);
DAR (RUN.tmp);
db= LDB (&(DBI[DBdata]), dmModeReadOnly);
CAR (db, RUN.tmp, 0);
DmCloseDatabase (db);
}

void ICAA () {
UInt16 i;
RUN.Aap= MemPtrNew ((RUN.aac+ RUN.cac)* sizeof(SysDBListItemType));
RUN.aap= MemHandleLock (RUN.aah);
MemMove (RUN.Aap, RUN.aap, RUN.aac* sizeof(SysDBListItemType));
MemHandleUnlock (RUN.aah);
for (i= 0; i<RUN.cac; i++) {
	MemMove (&(RUN.Aap[RUN.aac+ i]), &(RUN.cap[i]), sizeof(CardItemType));
	RUN.Aap[RUN.aac+ i].cardNo= _card;
}
SysQSort (RUN.Aap, RUN.aac+ RUN.cac, sizeof(SysDBListItemType), (CmpFuncPtr)&CF, 0);
}

void LME () {
DmOpenRef db, sw;
sw= LDB (&(DBI[DBswap]), dmModeReadWrite);
DAR (sw);
db= LDB (&(DBI[DBdata]), dmModeReadWrite);
PM (db, sw);
DmCloseDatabase (db);
DmCloseDatabase (sw);
}

void SD () {
if (RUN.save) {
	DmOpenRef db= LDB (&(DBI[DBdata]), dmModeReadWrite);
	DAR (db);
	CAR (RUN.tmp, db, 0);
	DmCloseDatabase (db);
}
DmCloseDatabase (RUN.tmp);
RUN.tmp= 0;
if (RUN.save) {
	LME ();
	RUN.save= false;
}
}

void FIX () {
UInt32 f= 0;
FtrGet (crea, _Fupdate, &f);
if (f==0) return;
B1 ();
FtrUnregister (crea, _Fupdate);
LME ();
B0 ();
FrmAlert (1014);
}

void LOAD () {
if (!RUN.reload) return;
RUN.reload= false;
B1 ();
LME ();
B0 ();
}

void LSM () {
LocalID id;
MemHandle h;
void* p;
B1 ();
id= GFD ('Data');
if (id) DmDeleteDatabase (0, id);
h= DmGetResource ('tMNU', 1000);
p= MemHandleLock (h);
DmCreateDatabaseFromImage (p);
MemHandleUnlock (h);
DmReleaseResource (h);
LME ();
RUN.P1[_Pb]= 0x0204;
RUN.P1[_Ps]= 0;
RUN.P1[_Pe1x]= 0;
RUN.P1[_Phold]= 0;
RUN.P1[_Pf]= 0x0201;
RUN.P1[_Prev]= 1;
RUN.P1[_Phidden]= 1;
SAP (RUN.P1);
RN ();
B0 ();
FrmAlert (1021);
}

void I1000 () {
WinScreenMode (winScreenModeGet, 0, 0, &RUN.depth, 0);
FrmDrawForm (RUN.CFP);
RUN.selfor= UIColorGetTableEntryIndex (UIObjectSelectedForeground);
if (!RUN.car) {
	RUN.P1= LAP ();
	if (RUN.P1[_Pfirst]<7) {
#ifdef trial
		FrmAlert (2203);
#endif
		RUN.P1[_Pfirst]= 8;
		SAP (RUN.P1);
	}
	if ((!GFD ('Data')) && FrmAlert (1022))
		LSM ();
	RUN.hidden= RUN.P1[_Phidden];
	MemPtrFree (RUN.P1);
	B1 ();
	LACL (0);
	RUN.car= true;
}
if (!RUN.aah) {
	B1 ();
	SysCreateDataBaseList ('appl', 0, &RUN.aac, &RUN.aah, true);
	if (!RUN.hidden) RHD (RUN.aah, &RUN.aac);
}
if (!RUN.dar) {
	B1 ();
	SysCreateDataBaseList ('DAcc', 0, &RUN.dac, &RUN.dah, true);
	if ((!RUN.hidden) && (RUN.dac!=0))
		RHD (RUN.dah, &RUN.dac);
	RUN.dar= true;
}
if (!RUN.par) {
	B1 ();
	SysCreatePanelList (&RUN.pac, &RUN.pah);
//	SysCreateDataBaseList ('panl', 0, &RUN.pac, &RUN.pah, true);
	if ((!RUN.hidden) && (RUN.pac!=0))
		RHD (RUN.pah, &RUN.pac);
	RUN.par= true;
}
if (RUN.tmp!=0) {
	B1 ();
	SD ();
}
if (bmph==0) LB (8000, maxbcount);
B0 ();
}

Boolean OS (UInt32 atleast) {
UInt32 val;
FtrGet (sysFtrCreator, sysFtrNumROMVersion, &val);
return (val>=atleast);
}

void NAV (char* c) {
ExgSocketType s;
if (!OS (0x04003000)) return;
MemSet (&s, sizeof(ExgSocketType), 0);
s.name= c;
ExgRequest (&s);
}

Boolean H1000 (EventPtr e) {
switch (e->eType) {
case frmOpenEvent:
	I1000 ();
	FIX ();
	LOAD ();
	return true;
case winExitEvent:
	FIX ();
	return false;
case ctlSelectEvent:
	switch (e->data.ctlSelect.controlID) {
	case 2000: case 2001:
		RUN.ras= (e->data.ctlSelect.controlID==2001);
		RUN.P1= LAP ();
		FrmGotoForm (1100);
		return true;
	case 2002:
		B1 ();
		ITMP ();
		RUN.ci= -1;
		RUN.ui= -1;
		FrmGotoForm (1200);
		return true;
	case 2003:
		RUN.P1= LAP ();
		FrmGotoForm (1300);
		return true;
	case 2004:
		B1 ();
		ICAA ();
		FrmGotoForm (1400);
		return true;
	}
	return false;
case menuEvent:
	switch (e->data.menu.itemID) {
	case 1000:
		FrmHelp (1000);
		return true;
	case 1001: {
		FormPtr fp= FrmInitForm (9000);
		UInt16 but= FrmDoDialog (fp);
		FrmDeleteForm (fp);
		if (but==1000) NAV ("http://www.hilauncher.com/pda.html");
		if (but==1001) NAV ("http://www.replaced.com");
		return true;
		}
#ifdef trial
	case 2000:
		FrmHelp (2202);
		return true;
#endif
	case 3000: {
		UInt16 cardNo;
		LocalID dbID;
		if (GFD ('Data') && FrmAlert (1020))
			return true;
		RUN.P1= LAP ();
		LSM ();
		MemPtrFree (RUN.P1);
		SysCurAppDatabase (&cardNo, &dbID);
		SysUIAppSwitch (cardNo, dbID, 0, 0);
		return true;
		}
	case 3001:
		RUN.P1= LAP ();
		FrmGotoForm (1010);
		return true;
	case 3002:
		if (GFD ('Data')) {
			RUN.P1= LAP ();
			FrmGotoForm (1020);
		} else FrmAlert (1027);
		return true;
	case 4000: case 4001:
		if (OS (0x05000000)) {
			SP (_Pdisabled, e->data.menu.itemID- 4000);
			RN ();
		} else FrmAlert (1023);
		return true;
	}
	return false;
}
return false;
}

void I1100 () {
UInt16 plus= (7* RUN.ras);
char* ct= S_TRIGGER1;
if (RUN.ras==1) {
	ct= S_TRIGGER2;
	RUN.CFP->helpRscId= 1101;
}
FrmSetTitle (RUN.CFP, ct);
if (RUN.P1[_Pb+ plus]!=0) {
	CSV (2001, 1);
	H1100 ((void*)0);
} else
if (RUN.P1[_Ps+ plus]!=0) {
	CSV (2002, 1);
	H1100 ((void*)1);
} else
if (RUN.P1[_Pe1x+ plus]!=0) {
	CSV (2003, 1);
	H1100 ((void*)2);
} else CSV (2000, 1);
if (RUN.P1[_Phold+ plus]) CSV (2010, 1);
RUN.baseB= (RUN.P1[_Pbases+ plus] & 1);
RUN.baseR= (RUN.P1[_Pbases+ plus] & 2);
if (RUN.baseB) CSV (2011, 1);
if (RUN.baseR) CSV (2012, 1);
FrmDrawForm (RUN.CFP);
}

Boolean S1100 () {
UInt16 plus= (7* RUN.ras);
Boolean c2001= CGV (2001);
Boolean c2002= CGV (2002);
Boolean c2003= CGV (2003);
Boolean isset= (c2001 || c2002 || c2003);
if ((!isset) && (RUN.ras==0))
	if (FrmAlert (1000)) return false;
if ((c2001 && (RUN.P1[_Pb+ plus]==0)) || (c2002 && (RUN.P1[_Ps+ plus]==0)) || (c2003 && (RUN.P1[_Pe1x+ plus]==0))) {
	FrmAlert (1001);
	return false;
}
if (isset && RUN.P1[_Pdisabled])
	if (!FrmAlert (1031)) RUN.P1[_Pdisabled]= false;
SAP (RUN.P1);
RN ();
return true;
}

void T1110 () {
//Boolean c2001= CGV (2001);
Boolean c2002= CGV (2002);
Boolean c2003= CGV (2003);
RUN.ME= c2002+ (2*c2003);
RUN.noted= false;
FrmPopupForm (1110);
}

Boolean H1100 (EventPtr e) {
UInt16 plus= (7* RUN.ras);
switch ((UInt32)e) {
case 0: goto c2001;
case 1: goto c2002;
case 2: goto c2003;
}
switch (e->eType) {
case frmOpenEvent:
	I1100 ();
	return true;
case frmCloseEvent:
	MemPtrFree (RUN.P1);
	return false;
case ctlSelectEvent:
	switch (e->data.ctlSelect.controlID) {
	case 1000:
		if (!S1100 ()) return true;
	case 1001:
		FrmGotoForm (1000);
		return true;
	case 2000:
		HO (5001);
		HO (4000);
		HO (2010);
		HO (2011);
		HO (2012);
		RUN.P1[_Pe1x+ plus]= 0;
		RUN.P1[_Pb+ plus]= 0;
		RUN.P1[_Ps+ plus]= 0;
		return true;
	case 2001: c2001:
		HO (2010);
		HO (2011);
		HO (2012);
		RUN.P1[_Pe1x+ plus]= 0;
		RUN.P1[_Ps+ plus]= 0;
		CtlSetLabel (GOP (4000), S_SELECTBUTTON);
		if (RUN.ras==1)
			CtlSetLabel (GOP (2010), S_LNC_PAH_ONLY);
			else CtlSetLabel (GOP (2010), S_POP_PAH_ONLY);
		SO (2010);
		SO (5001);
		SO (4000);
		return true;
	case 2002: c2002:
		HO (2010);
		HO (2011);
		HO (2012);
		RUN.P1[_Pe1x+ plus]= 0;
		RUN.P1[_Pb+ plus]= 0;
		CtlSetLabel (GOP (4000), S_SELECTICON);
		if (RUN.ras==1)
			CtlSetLabel (GOP (2010), S_LNC_TAH_ONLY);
			else CtlSetLabel (GOP (2010), S_POP_TAH_ONLY);
		SO (2010);
		SO (5001);
		SO (4000);
		return true;
	case 2003: c2003:
		HO (2010);
		RUN.P1[_Pb+ plus]= 0;
		RUN.P1[_Ps+ plus]= 0;
		CtlSetLabel (GOP (4000), S_SELECTSTROKE);
		SO (5001);
		SO (4000);
		SO (2011);
		SO (2012);
		return true;
	case 2010:
		RUN.P1[_Phold+ plus]= e->data.ctlSelect.on;
		return true;
	case 2011:
		RUN.baseB= e->data.ctlSelect.on;
		RUN.P1[_Pbases+ plus]= RUN.baseB+ (2* RUN.baseR);
		RUN.P1[_Pe1x+ plus]= 0;
		return true;
	case 2012:
		RUN.baseR= e->data.ctlSelect.on;
		RUN.P1[_Pbases+ plus]= RUN.baseB+ (2* RUN.baseR);
		RUN.P1[_Pe1x+ plus]= 0;
		return true;
	case 4000:
		T1110 ();
		return true;
	}
	return false;
}
return false;
}

void I1110 () {
UInt16 initDelay= SysTicksPerSecond ()* 10;
KeyRates (false, &RUN.r_initDelay, &RUN.r_period, &RUN.r_doubleTapDelay, &RUN.r_queueAhead);
KeyRates (true, &initDelay, &RUN.r_period, &RUN.r_doubleTapDelay, &RUN.r_queueAhead);
SO (2000+ (2* RUN.ME));
SO (2001+ (2* RUN.ME));
FrmDrawForm (RUN.CFP);
}

void S1110 () {
UInt16 plus= (7* RUN.ras);
switch (RUN.ME) {
case 0:
	RUN.P1[_Pb+ plus]= RUN.key;
	break;
case 1:
	RUN.P1[_Ps+ plus]= RUN.key;
	break;
case 2:
	RUN.P1[_Pe1x+ plus]= RUN.p1.x;
	RUN.P1[_Pe1y+ plus]= RUN.p1.y;
	RUN.P1[_Pe2x+ plus]= RUN.p2.x;
	RUN.P1[_Pe2y+ plus]= RUN.p2.y;
//	break;
}
RUN.P1[_Pkc+ plus]= RUN.keycode;
}

void C1110 () {
KeyRates (true, &RUN.r_initDelay, &RUN.r_period, &RUN.r_doubleTapDelay, &RUN.r_queueAhead);
RUN.CFP= FrmGetFormPtr (1100);
RUN.CFI= 1100;
}

Boolean H1110 (EventPtr e) {
switch (e->eType) {
case frmOpenEvent:
	I1110 ();
	return true;
case frmCloseEvent:
	C1110 ();
	return false;
case ctlSelectEvent:
	switch (e->data.ctlSelect.controlID) {
	case 1000:
		S1110 ();
	case 1001:
		FrmReturnToForm (1100);
		C1110 ();
		return true;
	}
	return false;
}
return false;
}

Boolean OOT (Int16 type) {
return !((type==_untyped) || (type>=_0_main && type<_x_main) || (type>=_0_special && type<_x_special) || (type>=_0_toggle && type<_x_toggle) || (type>=_0_status && type<_x_status));
}

void D2000 (Int16 i, RectangleType* r, Char** ch) { //draw items in designer
Titem* M= &(RUN.M->items[i+ 1]);
Int16 type= M->type;
IndexedColorType cb, cf, cl, oldc, oldb, oldt;
RectangleType r1;
FontID bold, oldf;
char* c;
if (OOT (type)) type= -1;
if (type<=_untyped) {
	cb= UIColorGetTableEntryIndex (UIObjectFill);
	cf= UIColorGetTableEntryIndex (UIObjectSelectedFill);
	bold= 0;
} else {
	cb= M->color[0];
	cf= M->color[1];
	cl= M->color[2];
	bold= M->font;
}
oldf= FntSetFont (bold);
oldc= WinSetForeColor (cf);
r1= *r;
r1.topLeft.x-= 2;
r1.extent.x= 70;
if (RUN.depth>=8) {
	oldb= WinSetBackColor (cb);
	oldt= WinSetTextColor (cf);
	if (type!=_splitter) WinEraseRectangle (&r1, 0);
} else WinSetForeColor (oldc);
c= GC ('tSTR', M->cid);
switch (type) {
default:
	WinDrawTruncChars (c, StrLen (c), r->topLeft.x+13, r->topLeft.y, 54);
	break;
case -1:
	WC (S_UNKNOWN, &r1);
	break;
case _untyped: case _special: case _toggle: case _status:
	WC (S_NONE, &r1);
	break;
case _submenu:
	WinDrawTruncChars (c, StrLen (c), r->topLeft.x+13, r->topLeft.y, 54);
	if (RUN.depth>=8) {
		WinSetForeColor (cb);
		WinSetBackColor (oldb);
	}
	DBH (bmph[0xA0], 76, r->topLeft.y+ 1);
	break;
case _application: case _card: case _desk: case _panel:
	if (M->data.body.creator==0) {
		WC (S_NONE, &r1);
	} else {
		WinDrawTruncChars (c, StrLen (c), r->topLeft.x+13, r->topLeft.y, 54);
	}
	break;
case _splitter: {
	UInt16 tc, bc;
	Boolean ps, ns;
	UInt16 x, y;
	if (RUN.depth>=8) {
		Boolean p0= (i==0);
		Boolean n0= (i==RUN.M->items->data.header.count- 1);
		Boolean pn= (p0 || (OOT (RUN.M->items[i].type)));
		Boolean nn= (n0 || (OOT (RUN.M->items[i+2].type)));
		RectangleType r3;
		ps= ((!pn) && (RUN.M->items[i].type==_splitter));
		ns= ((!nn) && (RUN.M->items[i+2].type==_splitter));
		if (p0) {
			tc= cl;
		} else if (pn) {
			tc= UIColorGetTableEntryIndex (UIObjectFill);
		} else {
			if (ps) {
				tc= MC (cl, RUN.M->items[i].color[2]);
			} else {
				tc= RUN.M->items[i].color[0];
			}
		}
		if (n0) {
			bc= cl;
		} else if (nn) {
			bc= UIColorGetTableEntryIndex (UIObjectFill);
		} else {
			if (ns) {
				bc= MC (RUN.M->items[i+2].color[2], cl);
			} else {
				bc= RUN.M->items[i+2].color[0];
			}
		}
		r3= r1;
		r3.extent.y/= 2;
		WinSetBackColor (tc);
		WinEraseRectangle (&r3, 0);
		r3.topLeft.y+= r3.extent.y;
		r3.extent.y= r1.extent.y- r3.extent.y;
		WinSetBackColor (bc);
		WinEraseRectangle (&r3, 0);
	}
	y= r1.topLeft.y+ (r1.extent.y/2);
	x= r1.topLeft.x+ r1.extent.x- 1;
	if (RUN.depth>=8) WinSetForeColor (cl);
	WinDrawLine (r1.topLeft.x, y, x, y);
	if (RUN.depth>=8) WinSetForeColor (MC (tc, cl));
	WinDrawLine (r1.topLeft.x, y- 1, x, y- 1);
	if (RUN.depth>=8) WinSetForeColor (MC (cl, bc));
	WinDrawLine (r1.topLeft.x, y+ 1, x, y+ 1);
	if (c[0]!=0) {
		UInt16 clen;
		char* d;
		if ((RUN.depth>=8) && (ps) && (ns))
			WinSetBackColor (MC (bc, tc));
		clen= StrLen (c);
		d= MemPtrNew (clen+ 3);
		d[0]= ' ';
		StrCopy (&d[1], c);
		d[clen+ 1]= ' ';
		d[clen+ 2]= 0;
		WC (d, &r1);
		MemPtrFree (d);
	}
	}
	break;
}

if ((type>_untyped) && (type!=_splitter) && (!M->noicon)) DBH (bmph[M->type], r->topLeft.x+ 1, r->topLeft.y+ 1);
if (RUN.depth>=8) {
	if (oldc==RUN.selfor) {
		WinSetForeColor (UIColorGetTableEntryIndex (UIObjectSelectedFill));
		RctInsetRectangle (&r1, 1);
		if (type==_submenu) r1.extent.x+= 9;
		WinDrawRectangleFrame (1, &r1);
	}
	WinSetForeColor (oldc);
	WinSetBackColor (oldb);
	WinSetTextColor (oldt);
}
FntSetFont (oldf);
MemPtrFree (c);
}

UInt16 ValueBetween (UInt16 val, UInt16 min, UInt16 max) {
return val<min?min:(val>max?max:val);
}

void R1200 () {
UInt16 SW;
UInt16 SH;
WinGetDisplayExtent (&SW, &SH);
{
RectangleType r1= {2, 2, 156, SH- 4};
UInt16 LIC;
UInt16 i;
WinSetBounds (FrmGetWindowHandle (RUN.CFP), &r1);
FrmSetObjectPosition (RUN.CFP, GOI(1000), 4, SH- 21);
FrmSetObjectPosition (RUN.CFP, GOI(1002), 4, SH- 21);
FrmSetObjectPosition (RUN.CFP, GOI(1001), 57, SH- 21);
FrmSetObjectPosition (RUN.CFP, GOI(9001), 0, SH- 26);
LIC= (SH- 52)/11;
for (i= 0; i<4; i++) FrmSetObjectPosition (RUN.CFP, GOI(3000+i), 6+ (20*i), 19+ (LIC*11));
LstSetHeight (GOP(2000), LIC);
LIC= (SH- 50)/11;
LstSetHeight (GOP(6001), ValueBetween (RUN.aac, 3, LIC));
LstSetHeight (GOP(6002), ValueBetween (RUN.cac, 3, LIC));
LstSetHeight (GOP(6003), ValueBetween (RUN.dac, 3, LIC));
LstSetHeight (GOP(6004), ValueBetween (RUN.pac, 3, LIC));
LstSetHeight (GOP(6020), LIC);
}

#ifdef trial
HPMoveGSI (RUN.CFP, 144, SH- 59);
#else
HPMoveGSI (RUN.CFP, 144, SH- 17);
#endif
}

void I1200 () {
UInt16 i;
ListPtr L;
RUN.fc= MemPtrNew (128);
RUN.fc[0]= 0;
RUN.fu= MemPtrNew (512);
RUN.fu[0]= 0;

RUN.aan= MemPtrNew (RUN.aac* sizeof(char*));
RUN.aap= MemHandleLock (RUN.aah);
for (i= 0; i<RUN.aac; i++)
	RUN.aan[i]= (char*)&(RUN.aap[i]);

if (RUN.cac!=0) {
	RUN.can= MemPtrNew (RUN.cac* sizeof(char*));
	for (i= 0; i<RUN.cac; i++)
		RUN.can[i]= (char*)&(RUN.cap[i]);
}

if (RUN.dac!=0) {
	RUN.dan= MemPtrNew (RUN.dac* sizeof(char*));
	RUN.dap= MemHandleLock (RUN.dah);
	for (i= 0; i<RUN.dac; i++)
		RUN.dan[i]= (char*)&(RUN.dap[i]);
}

if (RUN.pac!=0) {
	RUN.pan= MemPtrNew (RUN.pac* sizeof(char*));
	RUN.pap= MemHandleLock (RUN.pah);
	for (i= 0; i<RUN.pac; i++)
		RUN.pan[i]= (char*)&(RUN.pap[i]);
}

L= GOP (6001);
LstSetListChoices (L, RUN.aan, RUN.aac);
L= GOP (6002);
LstSetListChoices (L, RUN.can, RUN.cac);
L= GOP (6003);
LstSetListChoices (L, RUN.dan, RUN.dac);
L= GOP (6004);
LstSetListChoices (L, RUN.pan, RUN.pac);

RUN.M= M0 (sizeof(Tmenu));
//RUN.M->mid= 0;
RUN.M->items= LM ();
RUN.M->title= A (S_TOPMOST);
//RUN.M->prev= 0;
FrmSetTitle (RUN.CFP, RUN.M->title);
SO (1000);
SO (1001);

LstSetDrawFunction (RUN.L, &D2000);
LstSetListChoices (RUN.L, 0, RUN.M->items->data.header.count);
LstSetSelection (RUN.L, -1);

R1200 ();
FrmDrawForm (RUN.CFP);
L= GOP (6000);
LstSetDrawFunction (L, D6000);
L= GOP (6020);
LstSetDrawFunction (L, D6020);
L= GOP (6021);
LstSetDrawFunction (L, D6021);
L= GOP (6022);
LstSetDrawFunction (L, D6022);
}

MemHandle GCH (UInt32 creator, Tcid cid) {
MemHandle h= DmGet1Resource (creator, cid);
if (h==0) return 0;
{
UInt32 siz= MemHandleSize (h);
MemHandle r= MemHandleNew (siz);
char* c= MemHandleLock (h);
char* d= MemHandleLock (r);
StrCopy (d, c);
MemHandleUnlock (r);
MemHandleUnlock (h);
DmReleaseResource (h);
return r;
}
}

void SF (char* f, UInt32 creator, UInt16 field, Tcid cid) {
FieldPtr FldP= GOP (field);
MemHandle h= FldGetTextHandle (FldP);
MemHandle n= GCH (creator, cid);
UInt16 len= 0;
if (n==0) f[0]= 0; else {
	char* c= MemHandleLock (n);
	len= StrLen (c);
	StrCopy (f, c);
	MemHandleUnlock (n);
}
FldSetTextHandle (FldP, n);
if (h!=0) MemHandleFree (h);
if (field==7000) {
	FrmGlueNavObjectTakeFocus (RUN.CFP, 7000);
//	FrmSetFocus (RUN.CFP, GOI (7000));    (included in FrmGlueNavObjectTakeFocus)
	FldSetSelection (FldP, 0, len);
}
FldDrawField (FldP);
}

#define SCF(_cid) (SF(RUN.fc, 'tSTR', 7000,_cid))
#define SUF(_cid) (SF(RUN.fu, 'tURL', 7001,_cid))

void SON (Int16 ci, Ttype type) {
if (ci==-1) {
	ListPtr L= GOP (6001- _application+ type);
	CtlSetLabel (GOP (4001- _application+ type), S_NONE);
	LstSetSelection (L, -1);
} else TLI (4001- _application+ type, 6001- _application+ type, ci);
}

void SOS (Ttype group, Ttype type, Ttype base) {
//_special is the SOS_base because it is the first in the "_special,_toggle,_status" group
if (type==group) {
	ListPtr L= GOP (6020- _special+ group);
	CtlSetLabel (GOP (4020- _special+ group), S_NONE);
	LstSetSelection (L, -1);
} else TLI (4020- _special+ group, 6020- _special+ group, type- base);
}

void SI (Int16 ind) {
Titem* M;
Ttype group;
Int16 ci;

UInt16 Aalls[]= {5, 3001, 3002, 3003, 5000, 4000};
UInt16 Aallh[]= {32, 5000, 4000, 8000, 5001, 4001, 5002, 4002, 5003, 4003, 5020, 4020, 5021, 4021, 8030, 8031, 5022, 4022, 5100, 8020, 7000, 5101, 8010, 8011, 8012, 3003, 3002, 3001, 5200, 5201, 7001, 5004, 4004};
UInt16 Asubmenu[]= {2, 8010, 8000};
UInt16 Aapplication[]= {3, 8010, 5001, 4001};
UInt16 Acard[]= {3, 8010, 5002, 4002};
UInt16 Adesk[]= {3, 8010, 5003, 4003};
UInt16 Apanel[]= {3, 8010, 5004, 4004};
UInt16 Aspecial[]= {3, 8010, 5020, 4020};
UInt16 Atoggle[]= {3, 8010, 5021, 4021};
UInt16 Astatus[]= {4, 8030, 8010, 5022, 4022};
UInt16 Asplitter[]= {1, 8012};
UInt16 Aurl[]= {3, 8010, 5200, 7001};
UInt16 Adial[]= {4, 8031, 8010, 5201, 7001};
UInt16 Auntyped[]= {5, 5100, 8020, 7000, 8011, 5101};

RUN.ui= -1;
if (ind==-1) {
	RUN.ci= -1;
	HA ((UInt16*)&Aallh);
	SO (3000);
	SO (2000);
	return;
}
M= &(RUN.M->items[ind+ 1]);
group= M->type;
if (group<=_untyped) {
	ListPtr L= GOP (6000);
	RUN.ci= -1;
	CtlSetLabel (GOP (4000), S_NONE);
	LstSetSelection (L, -1);
} else {
	if (group>=_0_special && group<_x_special) {
		group= _special;
	} else
	if (group>=_0_toggle && group<_x_toggle) {
		group= _toggle;
	} else
	if (group>=_0_status && group<_x_status) {
		group= _status;
	}
	TLI (4000, 6000, group- _0_main);
	CSV (8020, M->font);
	SCF (M->cid);
	RUN.ci= ind;
}
switch (group) {
case _application:
	ci= CIa (M->data.body.creator);
	SON (ci, _application);
	break;
case _card:
	ci= CIc (M->data.body.creator);
	SON (ci, _card);
	break;
case _desk:
	ci= CId (M->data.body.creator);
	SON (ci, _desk);
	break;
case _panel:
	ci= CIp (M->data.body.creator);
	SON (ci, _panel);
	break;
case _special:
	SOS (group, M->type, _0_special);
	break;
case _toggle:
	SOS (group, M->type, _0_toggle);
	break;
case _status:
	SOS (group, M->type, _0_status);
	break;
case _url: case _dial:
	RUN.ui= ind;
	SUF (M->data.body.uid);
	break;
}
if (group!=_submenu) HA ((UInt16*)&Asubmenu);
if (group!=_application) HA ((UInt16*)&Aapplication);
if (group!=_card) HA ((UInt16*)&Acard);
if (group!=_desk) HA ((UInt16*)&Adesk);
if (group!=_panel) HA ((UInt16*)&Apanel);
if (group!=_special) HA ((UInt16*)&Aspecial);
if (group!=_toggle) HA ((UInt16*)&Atoggle);
if (group!=_status) HA ((UInt16*)&Astatus);
if (group!=_splitter) HA ((UInt16*)&Asplitter);
if (group!=_url) HA ((UInt16*)&Aurl);
if (group!=_dial) HA ((UInt16*)&Adial);
if (group==_untyped) HA ((UInt16*)&Auntyped);

if (group==_submenu) SA ((UInt16*)&Asubmenu);
if (group==_application) SA ((UInt16*)&Aapplication);
if (group==_card) SA ((UInt16*)&Acard);
if (group==_desk) SA ((UInt16*)&Adesk);
if (group==_panel) SA ((UInt16*)&Apanel);
if (group==_special) SA ((UInt16*)&Aspecial);
if (group==_toggle) SA ((UInt16*)&Atoggle);
if (group==_status) SA ((UInt16*)&Astatus);
if (group==_splitter) SA ((UInt16*)&Asplitter);
if (group==_url) SA ((UInt16*)&Aurl);
if (group==_dial) SA ((UInt16*)&Adial);
if (group!=_untyped) SA ((UInt16*)&Auntyped);

SA ((UInt16*)&Aalls);
}

void ClearItemClip (TClip* clip) {
if (clip->item) {
	MemPtrFree (clip->item);
	clip->item= 0;
}
if (clip->caption) {
	MemPtrFree (clip->caption);
	clip->caption= 0;
}
if (clip->URL) {
	MemPtrFree (clip->URL);
	clip->URL= 0;
}
}

void CI (TClip* dest) {
Titem* ori;
ClearItemClip (dest);
ori= &(RUN.M->items[RUN.ind+ 1]);
dest->item= MemPtrNew (sizeof(Titem));
MemMove (dest->item, ori, sizeof(Titem));
dest->caption= GC ('tSTR', ori->cid);
if ((ori->type==_url) || (ori->type==_dial)) {
	dest->URL= GC ('tURL', ori->data.body.uid);
}
}

void AI (TClip* source) {
Int16 ind= RUN.ind+ 1;
UInt16 c= RUN.M->items->data.header.count;
Titem* M;
if (c && (ind<=RUN.M->items->data.header.defaultitem))
	RUN.M->items->data.header.defaultitem++;
M= M0 ((c+ 2)* sizeof(Titem));
MemMove (M, RUN.M->items, (ind+ 1)* sizeof(Titem));
MemMove (&(M[ind+ 2]), &(RUN.M->items[ind+ 1]), (c- ind)* sizeof(Titem));
MemPtrFree (RUN.M->items);
RUN.M->items= M;
c++;
M->data.header.count= c;
if (source) {
	Titem* new= &(M[ind+ 1]);
	MemMove (new, source->item, sizeof(Titem));
	new->cid= NC ('tSTR');
	SC ('tSTR', new->cid, A(source->caption));
	if (source->URL) {
		new->data.body.uid= NC ('tURL');
		SC ('tURL', new->data.body.uid, A(source->URL));
	}
	new->data.body.mid= 0;
	new->shortcut= 0;
/*	M[ind+ 1]= M[ind+ 0];
	M[ind+ 1].cid= NC ('tSTR');
	SC ('tSTR', M[ind+ 1].cid, GC ('tSTR', M[ind+ 0].cid));
	if ((M[ind+ 0].type==_url) || (M[ind+ 0].type==_dial)) {
		M[ind+ 1].data.body.uid= NC ('tURL');
		SC ('tURL', M[ind+ 1].data.body.uid, GC ('tURL', M[ind+ 0].data.body.uid));
	}
	M[ind+ 1].data.body.mid= 0;
	M[ind+ 1].shortcut= 0;*/
} else {
	M[ind+ 1].cid= NC ('tSTR');
}
LstSetListChoices (RUN.L, 0, c);
LstSetSelection (RUN.L, ind);
LstDrawList (RUN.L);
SI (ind);
}

void DMx (Tmid mid) {
Titem* M= LR ('tMEN', mid);
UInt16 i;
UInt16 ind;
for (i= 1; i<M->data.header.count+ 1; i++) {
	if (M[i].type==_submenu)
		DMx (M[i].data.body.mid);
	if ((M[i].type==_url) || (M[i].type==_dial))
		DC ('tURL', M[i].data.body.uid);
	DC ('tSTR', M[i].cid);
}
MemPtrFree (M);
ind= DmFindResource (RUN.tmp, 'tMEN', mid, 0);
if (ind!=0xFFFF) DmRemoveResource (RUN.tmp, ind);
}

void DI (Int16 ind) {
UInt16 c;
if ((RUN.M->items[ind+ 1].type==_submenu) && (RUN.M->items[ind+ 1].data.body.mid!=0)) {
	if (FrmAlert (1007)) return;
	DMx (RUN.M->items[ind+ 1].data.body.mid);
}
if ((RUN.M->items[ind+ 1].type==_url) || (RUN.M->items[ind+ 1].type==_dial))
	DC ('tURL', RUN.M->items[ind+ 1].data.body.uid);
DC ('tSTR', RUN.M->items[ind+ 1].cid);
c= RUN.M->items->data.header.count;
if ((ind<RUN.M->items->data.header.defaultitem) || ((ind==c- 1) && (ind==RUN.M->items->data.header.defaultitem)))
	RUN.M->items->data.header.defaultitem--;
MemMove (&(RUN.M->items[ind+ 1]), &(RUN.M->items[ind+ 2]), (c- ind- 1)* sizeof(Titem));
c--;
RUN.M->items->data.header.count= c;
MemPtrResize (RUN.M->items, (c+ 1)* sizeof(Titem));
LstSetListChoices (RUN.L, 0, c);
if (ind==c) {
	ind--;
	LstSetSelection (RUN.L, ind);
}
LstDrawList (RUN.L);
SI (ind);
}

void MI (Int16 by) {
Int16 ind= RUN.ind;
Titem M;
if ((ind+ by<0) || (ind+ by>=RUN.M->items->data.header.count)) return;
if (ind==RUN.M->items->data.header.defaultitem)
	RUN.M->items->data.header.defaultitem+= by;
	else if (ind+by==RUN.M->items->data.header.defaultitem)
		RUN.M->items->data.header.defaultitem-= by;
M= RUN.M->items[ind+ 1];
RUN.M->items[ind+ 1]= RUN.M->items[ind+ 1+ by];
RUN.M->items[ind+ 1+ by]= M;
ind+= by;
LstSetSelection (RUN.L, ind);
LstDrawList (RUN.L);
RUN.ci= ind;
if (RUN.ui!=-1) RUN.ui= ind;
}

Tmid FM () {
UInt16 i;
for (i= 1; i<1000; i++) {
	if (DmFindResource (RUN.tmp, 'tMEN', i, 0)==0xFFFF) return i;
}
return 1000;
}

void SetNiceTitle () {
char* title;
if (*(RUN.M->title)) {
	title= RUN.M->title;
} else {
	title= S_NONAMEMENU;
}
FrmSetTitle (RUN.CFP, title);
}

void GM () {
Tmid mid;
Tmenu* M;
SM ();
mid= RUN.M->items[RUN.ind+ 1].data.body.mid;
if (mid==0) {
	mid= FM ();
	RUN.M->items[RUN.ind+ 1].data.body.mid= mid;
}
RUN.M->trigger= RUN.ind;
M= M0 (sizeof(Tmenu));
M->mid= mid;
M->title= GC ('tSTR', RUN.M->items[RUN.ind+ 1].cid);
M->prev= RUN.M;
RUN.M= M;
RUN.M->items= LM ();
SetNiceTitle ();
SO (9003);
HO (1000);
HO (1001);
SO (1002);
LstSetListChoices (RUN.L, 0, RUN.M->items->data.header.count);
LstSetSelection (RUN.L, -1);
LstSetTopItem (RUN.L, 0);
LstDrawList (RUN.L);
SI (-1);
}

void RM () {
UInt16 cnt= RUN.M->items->data.header.count;
Tmenu* M= RUN.M;
Int16 ind;
RUN.M= M->prev;
SetNiceTitle ();
SO (9003);
MemPtrFree (M->title);
MemPtrFree (M->items);
MemPtrFree (M);
if (RUN.M->prev==0) {
	HO (1002);
	SO (1000);
	SO (1001);
}
LstSetListChoices (RUN.L, 0, RUN.M->items->data.header.count);
ind= RUN.M->trigger;
LstSetSelection (RUN.L, ind);
LstDrawList (RUN.L);
SI (ind);
if (cnt==0) RUN.M->items[ind+ 1].data.body.mid= 0;
}

Boolean ACT (Ttype type) {
return !(((type>=_0_status) && (type<_x_status)) || (type==_status) || (type==_splitter));
}

Int16 FUI () {
Int16 i;
for (i= 0; i<RUN.M->items->data.header.count; i++) {
	Ttype type= RUN.M->items[i+ 1].type;
	Boolean b;
	if (OOT (type)) return i;
	if ((type==_untyped) || (type==_special) || (type==_toggle) || (type==_status)) return i;
	b= ((type==_application) || (type==_card) || (type==_desk) || (type==_panel));
	if (b && (RUN.M->items[i+ 1].data.body.creator==0)) return i;
	if ((type==_submenu) && (RUN.M->items[i+ 1].data.body.mid==0)) return i;
}
return -1;
}

Int16 FAI () {
Int16 i;
for (i= 0; i<RUN.M->items->data.header.count; i++) {
	if (ACT (RUN.M->items[i+ 1].type) && (!RUN.M->items[i+ 1].data.body.invisible)) return i;
}
return 0;
}

Boolean TTL (char* c) {
UInt16 fit= FldWordWrap (c, fieldwidth);
UInt16 len= StrLen (c);
return (len>fit);
}

Boolean S1200 () {
Int16 ui= FUI ();
UInt16 cnt;
UInt16 i;
if (ui!=-1) {
	if (FrmAlert (1005)) return false;
	do DI (ui); while ((ui= FUI ())!=-1);
}
cnt= RUN.M->items->data.header.count;
if (cnt==0) {
	if (FrmAlert (1006)) return false;
	DM ();
	return true;
}
for (i= 0; i<cnt; i++) {
	if (RUN.M->items[i+1].type==_splitter) {
		char* c= GC ('tSTR', RUN.M->items[i+1].cid);
		char d= *c;
		MemPtrFree (c);
		if (d!=0) goto ok1;
	} else if (!RUN.M->items[i+1].data.body.invisible)
		goto ok1;
}
if (FrmAlert (1011)) return false;
for (i= 0; i<cnt; i++) DI (0);
DM ();
return true;
ok1:
for (i= 0; i<cnt; i++) {
	Ttype typ= RUN.M->items[i+1].type;
	if (!ACT (typ)) {
		RUN.M->items[i+1].shortcut= 0;
		RUN.M->items[i+1].data.body.invisible= false;
	}
	if (typ==_splitter) {
		RUN.M->items[i+1].color[0]= dc0;
	} else {
		RUN.M->items[i+1].color[2]= dc2;
	}
}
if (!ACT (RUN.M->items[RUN.M->items->data.header.defaultitem+ 1].type)) RUN.M->items->data.header.defaultitem= FAI ();
SM ();
return true;
}

void PNb () {
FieldPtr FldP= GOP(7001);
char* c= FldGetTextPtr (FldP);
UInt16 len= 0;
AddrLookupParamsType params;
MemSet (&params, sizeof(AddrLookupParamsType), 0);
params.title= S_PICKNUMBER;
params.pasteButtonText= S_OK;
//params.lookupString[addrLookupStringLength];
params.field1= addrLookupSortField;
params.field2= addrLookupListPhone;//addrLookupMobile;
//params.field2Optional= false;
params.userShouldInteract= true;
params.formatStringP= "^listphone\r^listname";
if (c) len= StrLen (c);
FrmSetFocus (RUN.CFP, GOI (7001));
FldSetSelection (FldP, 0, len);
PhoneNumberLookupCustom (FldP, &params, false);
c= FldGetTextPtr (FldP);
len= 0;
if (c) {
	char* spl= StrChr (c, 0x0D);
	if (spl) {
		*spl= 0;
		spl++;
		if (*spl) {
			SFT (7000, spl, true);
			RUN.keydown= false;
		}
		SFT (7001, c, true);
	}
}
}

// void AUE (eventsEnum eType) {
// EventType e;
// MemSet (&e, sizeof(e), 0);
// e.eType= eType;
// EvtAddUniqueEventToQueue (&e, 0, false);
// }

void ShowKeyboard (FieldPtr fldP, UInt16 id) {
EventType e;
MemSet (&e, sizeof(e), 0);
e.eType= _EshowKeyboard;
e.data.fldEnter.pField= fldP;
e.data.fldEnter.fieldID= id;
EvtAddUniqueEventToQueue (&e, 0, false);
}

void ProcessMultiField (FieldPtr fldP, UInt16 fldID, char* c) {
char* enter;
Boolean anychange= false;
UInt16 ss, se;
while (enter= StrChr (c, '\n')) {
	if (!anychange) {
		FldGetSelection (fldP, &ss, &se);
		anychange= true;
	}
	if (enter-c<ss) ss--;
	if (enter-c<se) se--;
	MemMove (enter, enter+1, StrLen (enter));
}
if (anychange) {
	SFT (fldID, c, false);
	FldSetSelection (fldP, ss, se);
	if (ss==se) {
		FldSetInsPtPosition (fldP, ss);
	}
	FldDrawField (fldP);
}
}

Int16 CompareItems (Titem* i1, Titem* i2, Int32 other) {
char* c1= GC ('tSTR', i1->cid);
char* c2= GC ('tSTR', i2->cid);
Int16 result= StrCaselessCompare (c1, c2);
MemPtrFree (c1);
MemPtrFree (c2);
return result;
}

void SortItems (UInt16 first, UInt16 last) {
if (last- first>1) {
	SysInsertionSort (&(RUN.M->items[first]), last- first, sizeof(Titem), (void*)CompareItems, 0);
}
}

void SortMenu () {
if (RUN.M->items->data.header.count>1) {
	UInt16 i= 0;
	UInt16 first= 1;
	while (i++<RUN.M->items->data.header.count) {
		if (RUN.M->items[i].type==_splitter) {
			SortItems (first, i);
			first= i+ 1;
		}
	}
	SortItems (first, i);
	LstSetSelection (RUN.L, -1);
	LstDrawList (RUN.L);
	SI (-1);
}
}

Boolean H1200 (EventPtr e) {
UInt16 cid;
UInt32 ftr;
if (e->eType==winExitEvent) {
	RUN.keydown= false;
}
if (RUN.ci!=-1) {
	FormPtr F= FrmGetFormPtr (1200);
	FieldPtr fldP= FrmGetObjectPtr (F, FrmGetObjectIndex (F, 7000));
	char* c= FldGetTextPtr (fldP);
	if (c!=0) {
		if (StrCompare (c, RUN.fc)) {
			ProcessMultiField (fldP, 7000, c);
			SC ('tSTR', RUN.M->items[RUN.ci+ 1].cid, c);
			LstDrawList (RUN.L);
			if ((TTL (c)) && RUN.keydown && (OS (0x04003000))) {
				ShowKeyboard (fldP, 7000);
//				AUE (_EshowKeyboard);
			}
			StrCopy (RUN.fc, c);
		}
	}
}
if (RUN.ui!=-1) {
	FormPtr F= FrmGetFormPtr (1200);
	FieldPtr fldP= FrmGetObjectPtr (F, FrmGetObjectIndex (F, 7001));
	char* c= FldGetTextPtr (fldP);
	if (c!=0) {
		if (StrCompare (c, RUN.fu)) {
			ProcessMultiField (fldP, 7001, c);
			SC ('tURL', RUN.M->items[RUN.ui+ 1].data.body.uid, c);
			if ((TTL (c)) && RUN.keydown && (OS (0x04003000))) {
				ShowKeyboard (fldP, 7001);
//				AUE (_EshowKeyboard);
			}
			StrCopy (RUN.fu, c);
		}
	}
}
RUN.ind= LstGetSelection (RUN.L);
switch (e->eType) {
case fldEnterEvent:
	if ((TTL (FldGetTextPtr(e->data.fldEnter.pField))) && (OS (0x04003000))) {
		ShowKeyboard (e->data.fldEnter.pField, e->data.fldEnter.fieldID);
// 		AUE (_EshowKeyboard);
	}
	return false;
case winExitEvent:
	FtrGet (crea, _Frefresh, &ftr);
	if (ftr!=0) {
		LstDrawList (RUN.L);
		FtrUnregister (crea, _Frefresh);
	}
	return false;
case frmOpenEvent:
	I1200 ();
	return true;
case frmCloseEvent:
	while (RUN.M->prev!=0) RM ();
	MemPtrFree (RUN.M->title);
	MemPtrFree (RUN.M->items);
	MemPtrFree (RUN.M);
	MemPtrFree (RUN.fu);
	MemPtrFree (RUN.fc);
	if (RUN.pac) {
		MemPtrFree (RUN.pan);
		MemHandleUnlock (RUN.pah);
	}
	if (RUN.dac) {
		MemPtrFree (RUN.dan);
		MemHandleUnlock (RUN.dah);
	}
	if (RUN.cac) MemPtrFree (RUN.can);
	MemPtrFree (RUN.aan);
	MemHandleUnlock (RUN.aah);
	return false;
case menuEvent:
	if (RUN.ind==-1) {
		if (e->data.menu.itemID<2000)
			FrmAlert (1017);
		if (e->data.menu.itemID<3000)
			return false;
	}
	switch (e->data.menu.itemID) {
	case 1000:
		RUN.style.is= true;
		RUN.style.font= RUN.M->items[RUN.ind+ 1].font;
		RUN.M->items->font= RUN.style.font;
		MemMove (&(RUN.style.color), &(RUN.M->items[RUN.ind+ 1].color), sizeof(TColors));
		MemMove (&RUN.M->items->color, &(RUN.style.color), sizeof(TColors));
		return true;
	case 1001:
		if (RUN.style.is) {
			RUN.M->items[RUN.ind+ 1].font= RUN.style.font;
			RUN.M->items->font= RUN.style.font;
			MemMove (&(RUN.M->items[RUN.ind+ 1].color), &(RUN.style.color), sizeof(TColors));
			MemMove (&RUN.M->items->color, &(RUN.style.color), sizeof(TColors));
			CSV (8020, RUN.style.font);
			LstDrawList (RUN.L);
		} else {
			FrmAlert (1033);
		}
		return true;
	case 1002: {
		TClip clip;
		MemSet (&clip, sizeof(clip), 0);
		CI (&clip);
		AI (&clip);
		ClearItemClip (&clip);
		return true;
		}
	case 1003: {
		Ttype type= RUN.M->items[RUN.ind+ 1].type;
		if (type==_splitter) FrmAlert (1013);
		else if (type==_untyped) FrmAlert (1019);
		else FrmPopupForm (1210);
		return true;
		}
	case 1004:
		if (RUN.M->items[RUN.ind+ 1].type==_submenu) {
			FrmAlert (1035);
		} else {
			CI (&RUN.clip);
			DI (RUN.ind);
		}
		return true;
	case 1005:
		CI (&RUN.clip);
		return true;
	case 3000:
		if (RUN.clip.item) {
			AI (&RUN.clip);
		} else {
			FrmAlert (1036);
		}
		return true;
	case 3001:
		SortMenu ();
		return true;
	}
	return false;
case ctlSelectEvent:
	switch (e->data.ctlSelect.controlID) {
	case 1000:
		if (!S1200 ()) return true;
		RUN.save= true;
	case 1001:
		FrmGotoForm (1000);
		return true;
	case 1002:
		if (!S1200 ()) return true;
		RM ();
		return true;
	case 3000:
		AI (false);
		return true;
	case 3001:
		DI (RUN.ind);
		return true;
	case 4002:
		if (RUN.cac==0) {
			FrmAlert (1015);
			return true;
		}
		return false;
	case 4003:
		if (RUN.dac==0) {
			FrmAlert (1016);
			return true;
		}
		return false;
	case 4004:
		return (RUN.pac==0);//we never expect no prefs panels
	case 8000:
		GM ();
		return true;
	case 8010: case 8011: case 8012:
		if (RUN.depth<8) FrmAlert (1004); else
			PC (e->data.ctlSelect.controlID- 8010);
		return true;
	case 8020:
		RUN.M->items[RUN.ind+ 1].font= e->data.ctlSelect.on;
  		RUN.M->items->font= e->data.ctlSelect.on;
		LstDrawList (RUN.L);
		return true;
	case 8030:
		FrmHelp (1201);
		return true;
	case 8031:
		if (OS (0x04003000)) PNb (); else
			FrmAlert (2009);
		return true;
	}
	return false;
case ctlRepeatEvent:
	switch (e->data.ctlRepeat.controlID) {
	case 3002:
		MI (-1);
		return false;
	case 3003:
		MI (1);
		return false;
	}
	return false;
case lstSelectEvent:
	SI (e->data.lstSelect.selection);
	return false;
case popSelectEvent: {
	Int16 priorSelection= e->data.popSelect.priorSelection;
	Int16 selection= e->data.popSelect.selection;
	pchar STAP[]= {
	S_POWER ": ^P%",
	S_FREE ": ^FK",
	S_TIME ": ^T"
	};
	switch (e->data.popSelect.listID) {
	case 6000: {
		Int16 ind= RUN.ind;
		Ttype oldtype= RUN.M->items[ind+ 1].type;
		Ttype newtype= selection+ _0_main;
		if (priorSelection==selection) return false;
		if ((oldtype==_submenu) && (RUN.M->items[ind+ 1].data.body.mid!=0)) {
			if (FrmAlert (1008)) {
				LstSetSelection (e->data.popSelect.listP, priorSelection);
				return true;
			}
			DMx (RUN.M->items[ind+ 1].data.body.mid);
			RUN.M->items[RUN.ind+ 1].data.body.mid= 0;
		}
		if ((oldtype==_url) || (oldtype==_dial)) {
			DC ('tURL', RUN.M->items[ind+ 1].data.body.uid);
			RUN.M->items[ind+ 1].data.body.uid= -1;
		}
		if (oldtype==_untyped) {
			RUN.M->items[ind+ 1].font= RUN.M->items->font;
			MemMove (&(RUN.M->items[ind+ 1].color), &RUN.M->items->color, sizeof(TColors));
		}
//		if ((newtype==_splitter) || (newtype==_submenu)) RUN.M->items[ind+ 1].data.body.invisible= false;
		if (newtype==_url) {
			Tcid uid= NC ('tURL');
			RUN.M->items[ind+ 1].data.body.uid= uid;
			SC ('tURL', uid, "http://");
		}
		if (newtype==_dial) {
			Tcid uid= NC ('tURL');
			RUN.M->items[ind+ 1].data.body.uid= uid;
			SC ('tURL', uid, "");
		}
		RUN.M->items[ind+ 1].type= newtype;
		RUN.M->items[ind+ 1].data.body.creator= 0;
		SI (ind);
		LstDrawList (RUN.L);
		return true;
		}
	case 6001:
		RUN.M->items[RUN.ind+ 1].data.body.creator= RUN.aap[selection].creator;
		cid= RUN.M->items[RUN.ind+ 1].cid;
		SC ('tSTR', cid, (char*)&(RUN.aap[selection].name));
		SCF (cid);
		LstDrawList (RUN.L);
		return false;
	case 6002:
		RUN.M->items[RUN.ind+ 1].data.body.creator= RUN.cap[selection].creator;
		cid= RUN.M->items[RUN.ind+ 1].cid;
		SC ('tSTR', cid, (char*)&(RUN.cap[selection].name));
		SCF (cid);
		LstDrawList (RUN.L);
		return false;
	case 6003:
		RUN.M->items[RUN.ind+ 1].data.body.creator= RUN.dap[selection].creator;
		cid= RUN.M->items[RUN.ind+ 1].cid;
		SC ('tSTR', cid, (char*)&(RUN.dap[selection].name));
		SCF (cid);
		LstDrawList (RUN.L);
		return false;
	case 6004:
		RUN.M->items[RUN.ind+ 1].data.body.creator= RUN.pap[selection].creator;
		cid= RUN.M->items[RUN.ind+ 1].cid;
		SC ('tSTR', cid, (char*)&(RUN.pap[selection].name));
		SCF (cid);
		LstDrawList (RUN.L);
		return false;
	case 6020:
		RUN.M->items[RUN.ind+ 1].type= _0_special+ selection;
		goto comm1;
	case 6021:
		RUN.M->items[RUN.ind+ 1].type= _0_toggle+ selection;
		comm1:
		cid= RUN.M->items[RUN.ind+ 1].cid;
		SC ('tSTR', cid, LstGetSelectionText (e->data.popSelect.listP, selection));
		comm2:
		SCF (cid);
		LstDrawList (RUN.L);
		return false;
	case 6022:
		RUN.M->items[RUN.ind+ 1].type= _0_status+ selection;
		cid= RUN.M->items[RUN.ind+ 1].cid;
		SC ('tSTR', cid, STAP[selection]);
		goto comm2;
	}
	return false;
	}
case keyDownEvent:
	if (e->data.keyDown.modifiers & 0x0008)
		return KSL (e);
	if (e->data.keyDown.chr=='\n') {
		return true;
	}
	RUN.keydown= true;
	return false;
case _EshowKeyboard: /*VAR*/ {
		UInt16 ss, se;
		KeyboardType kbd= kbdAlpha;
		if ((e->data.fldEnter.fieldID==7001) && (RUN.M->items[RUN.ind+ 1].type==_dial)) {
			kbd= kbdNumbersAndPunc;
		}
		SysKeyboardDialog (kbd);
		FldGetSelection (e->data.fldEnter.pField, &ss, &se);
		FldSetScrollPosition (e->data.fldEnter.pField, ss);
	}
	return true;
case winDisplayChangedEvent:
	HPChanged (RUN.CFP, &HPP[1]);
	R1200 ();
	if (g_CollapseAPIVersion==sonyVersion1)
		WinEraseWindow ();
	FrmDrawForm (RUN.CFP);
}
return false;
}

void I1210 () {
Titem* M= &(RUN.M->items[RUN.ind+ 1]);
if (ACT (M->type)) {
	char c[2];
	char s= M->shortcut;
	if ((s>='a') && (s<='z')) s-= 0x20;
	c[0]= s; c[1]= 0;
	SFT (2000, (char*)&c, false);
	SO (5000);
	SO (2000);
	CSV (3001, RUN.ind==RUN.M->items->data.header.defaultitem);
	CSV (3002, M->data.body.invisible);
	SO (3001);
//	if (M->type!=_submenu)
		SO (3002);
}
CSV (3000, !M->noicon);
FrmDrawForm (RUN.CFP);
}

Boolean S1210 () {
if (ACT (RUN.M->items[RUN.ind+ 1].type)) {
	Boolean def= CGV (3001);
	Boolean inv= CGV (3002);
	char* c;
	char s;
	UInt16 i;
	if (def && inv) {
		FrmAlert (1032);
		return false;
	}
	c= FldGetTextPtr (GOP (2000));
	s= c[0];
	if ((s>='A') && (s<='Z')) s-= ('A'- 'a');
	if (s==0) goto ok1;
	for (i= 0; i<RUN.M->items->data.header.count; i++)
		if ((i!=RUN.ind) && (RUN.M->items[i+1].shortcut==s)) {
			if (ACT (RUN.M->items[i+1].type)) {
				char u[2];
				char t= s;
				if ((t>='a') && (t<='z')) t+= ('A'- 'a');
				u[0]= t; u[1]= 0;
				if (FrmCustomAlert (1012,(char*)&u,"",""))
					return false;
			}
			RUN.M->items[i+1].shortcut= 0;
		}
	ok1:
	RUN.M->items[RUN.ind+1].shortcut= s;
	if (RUN.ind==RUN.M->items->data.header.defaultitem) {
		if (!def) RUN.M->items->data.header.defaultitem= 0;
	} else {
		if (def) RUN.M->items->data.header.defaultitem= RUN.ind;
	}
	RUN.M->items[RUN.ind+ 1].data.body.invisible= inv;
}
RUN.M->items[RUN.ind+1].noicon= !CGV (3000);
FtrSet (crea, _Frefresh, 1);
return true;
}

Boolean H1210 (EventPtr e) {
switch (e->eType) {
case frmOpenEvent:
	I1210 ();
	return true;
case frmCloseEvent:
	RUN.CFP= FrmGetFormPtr (1200);
	RUN.CFI= 1200;
	RUN.to= SysTicksPerSecond ()/4;
	return false;
case ctlSelectEvent:
	switch (e->data.ctlSelect.controlID) {
	case 1000:
		if (!S1210 ()) return true;
	case 1001:
		FrmReturnToForm (1200);
		RUN.CFP= FrmGetFormPtr (1200);
		RUN.CFI= 1200;
		RUN.to= SysTicksPerSecond ()/4;
		return true;
	}
	return false;
}
return false;
}

Boolean AHE (EventPtr e) {
switch (e->eType) {
case winEnterEvent:
	if (e->data.winEnter.enterWindow==(void*)RUN.CFP)
		HPEnter (&HPP[RUN.hpp]);
	return false;
case winDisplayChangedEvent:
	if ((RUN.hpp==0) && (RUN.CFI!=0)) {
		if (HPChanged (RUN.CFP, &HPP[0])) {
			if (g_CollapseAPIVersion==sonyVersion1)
				WinEraseWindow ();
			FrmDrawForm (RUN.CFP);
		}
	}
	return false;
case frmLoadEvent: {
	FormEventHandlerType* han= 0;
	RUN.to= evtWaitForever;
	FtrSet (crea, _Fon, 1);
	FtrSet (crea, _Fset, 1);
	RUN.CFI= e->data.frmLoad.formID;
	RUN.CFP= FrmInitForm (RUN.CFI);
	FrmSetActiveForm (RUN.CFP);
	RUN.hpp= 0;
	switch (RUN.CFI) {
	case 1000:
		FtrUnregister (crea, _Fon);
		han= H1000;
		break;
	case 1010: case 1020:
		RUN.L= GOP (2000);
		han= H1010;
		RUN.hpp= 1;
		break;
	case 1100:
		han= H1100;
		break;
	case 1110:
		han= H1110;
		break;
	case 1200:
		RUN.L= GOP (2000);
		RUN.to= SysTicksPerSecond ()/4;
		han= H1200;
		RUN.hpp= 1;
		break;
	case 1210:
		han= H1210;
		RUN.hpp= 2;
		break;
	case 1300:
		han= H1300;
		break;
	case 1310:
		han= H1310;
		break;
	case 1320:
		han= H1320;
		break;
	case 1330:
		han= H1330;
		break;
	case 1340:
		han= H1340;
		break;
	case 1400:
		RUN.L= GOP (6001);
		han= H1400;
		RUN.hpp= 1;
		break;
	}
	if (han!=0) {
		FrmSetEventHandler (RUN.CFP, han);
		HPLoad (RUN.CFP, &HPP[RUN.hpp]);
		return true;
	}
	}
}
return false;
}

Boolean STA ();
void STO ();
#ifdef trial
void T1 ();
#endif

void EL () {
EventType e;
UInt16 err;
if (!STA ()) return;
do {
	EvtGetEvent (&e, RUN.to);
	if (!LRN (&e))
		if ((!SysHandleEvent (&e)) || (e.eType==winEnterEvent) || (e.eType==winExitEvent))//TT3
			if (!MenuHandleEvent (0, &e, &err))
				if (!AHE (&e)) FrmDispatchEvent (&e);
#ifdef trial
	if (frmOpenEvent==e.eType) T1 ();
#endif
} while (e.eType!=appStopEvent);
STO ();
}

Boolean STA () {
Tdbi DBI_data[]= {
{"Hi-Launcher DB",  crea, 'Data', 0x0009},
{"Hi-Launcher TMP", crea, 'Temp', 0x0401},
{"Hi-Launcher SWP", crea, 'Swap', 0x0001}
};
THP HPP_data[]= {
//popup:
{hp_keep, hp_keep, hp_first, hp_first, 0, 0},
//base:
{hp_keep, hp_keep, hp_first, hp_first, 0, 0},
//other:
{hp_keep, hp_keep, hp_first, hp_first, 0, 0}
};
UInt32 dummy;
UInt16 rSiz;
void (*FF)(void);
if (!OS (0x03503000)) {
	FrmAlert (1009);
	return false;
}
if (!OS (0x05000000)) {
	if (DmFindDatabase (0, "Hi-Launcher Hack")) {
		UInt32 temp;
		if (FtrGet (crea, 1000, &temp))
			FrmCustomAlert (1010, S_INACTIVE, "", "");
	} else FrmCustomAlert (1010, S_NOTEXIST, S_INSTALLAND, "");
}

if (!FtrGet (crea, _Fcardlaunch, &dummy)) {
	FtrUnregister (crea, _Fcardlaunch);
	FrmAlert (1018);
}

RUN._2= DmGet1Resource ('code', 2);
FF= MemHandleLock (RUN._2);
FF ();
LAE= _1LAE;
GFD= _1GFD;
LAR= _1LAR;

RUN.HPEnter= HPEnter;
RUN.HPChanged= HPChanged;

rSiz= 32;
if ((PrefGetAppPreferences (crea, 1, RUN.loadname, &rSiz, false)==noPreferenceFound) || (!RUN.loadname[0])) {
	StrCopy (RUN.loadname, S_MENUNAME);
}

RUN.to= evtWaitForever;

DBI= MemPtrNew (sizeof(DBI_data));
MemMove (DBI, &DBI_data, sizeof(DBI_data));
HPP= MemPtrNew (sizeof(HPP_data));
MemMove (HPP, &HPP_data, sizeof(HPP_data));

HP1 ();
FrmGotoForm (1000);
return true;
}

void STO () {
#ifdef trial
UInt32 typ= 'ownr'+ 27911;
UInt32 tim= TimGetSeconds ();
UInt16 rSiz= 4;
Int16 id= -1;
UInt32 p;
if (PrefGetAppPreferences (typ- 27911, id+ 256, &p, &rSiz, true)==noPreferenceFound)
	PrefSetAppPreferences (typ- 27911, id+ 256, 17, &tim, 4, true);
#endif
FrmCloseAllForms ();
HP0 ();
if (RUN.pah!=0) MemHandleFree (RUN.pah);
if (RUN.dah!=0) MemHandleFree (RUN.dah);
if (RUN.cap!=0) MemPtrFree (RUN.cap);
if (RUN.aah!=0) MemHandleFree (RUN.aah);
if (RUN.tmp!=0) DmCloseDatabase (RUN.tmp);
if (bmph!=0) UB (maxbcount);
FtrUnregister (crea, _Fon);
FtrUnregister (crea, _Fset);
MemHandleUnlock (RUN._2);
DmReleaseResource (RUN._2);
MemPtrFree (DBI);
MemPtrFree (HPP);
}

#ifdef trial
void T1 () {
Coord x, y;
IndexedColorType oldc;
FontID oldf;
RGBColorType r= {0, 0xFF, 0, 0};
WinGetWindowExtent (&x, &y);
x-= 40;
y-= 16;
oldc= WinSetTextColor (WinRGBToIndex (&r));
oldf= FntSetFont (1);
if (RUN.CFI!=1000)
	WinDrawChars (S_TRIAL1"  ", 6, x, y);
WinSetTextColor (oldc);
FntSetFont (oldf);
}
#endif
