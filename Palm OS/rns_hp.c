Boolean HPLand () {
if (g_CollapseAPIVersion>=pinAPIVersion1_1)
	return (SysGetOrientation ()%2==0);
if (g_CollapseAPIVersion<=sonyVersion3) {
	UInt16 dummy;
	VskGetState (g_sonySilkRefNum, vskStateResizeDirection, &dummy);
	return (dummy & vskResizeHorizontally);
}
return false;
}

Coord HPGIH (THP* P, UInt16 SH) {
Coord	H_init= P->H_last;
if ((P->H_next==hp_first) || (H_init==0)) {
	switch (P->H_first) {
	case hp_standard: return 160;
	case hp_keep: return SH;
	case hp_plus: return 0x7FFF;
	}
}
return H_init;
}

Coord HPGIW (THP* P, UInt16 SW) {
Coord	W_init= P->W_last;
if ((P->W_next==hp_first) || (W_init==0)) {
	switch (P->W_first) {
	case hp_standard: return 160;
	case hp_keep: return SW;
	case hp_plus: return 0x7FFF;
	}
}
return W_init;
}

void HPLoad (FormPtr frmP, THP* P) {
UInt16 SW;
UInt16 SH;
Coord H_init;
Coord H_max;
Coord W_init;
Coord W_max;
if (P->H_next==hp_first) P->H_last= 0;
if (g_CollapseAPIVersion<=0) return;
if ((P->H_first==hp_standard) && (P->W_first==hp_standard)) return;

WinGetDisplayExtent (&SW, &SH);

H_init= HPGIH (P, SH);
H_max= 0x7FFF;
if (P->H_first==hp_standard) H_max= 160;

W_init= HPGIW (P, SW);
W_max= 0x7FFF;
if (P->W_first==hp_standard) W_max= 160;

FrmSetDIAPolicyAttr (frmP, frmDIAPolicyCustom);
WinSetConstraintsSize (WinGetWindowHandle (frmP), 160, H_init, H_max, 160, W_init, W_max);
}

void HPEnter (THP* P) {
UInt16 SW;
UInt16 SH;
Boolean land;
Coord H_init;
Coord W_init;
Boolean clos= false;
WinGetDisplayExtent (&SW, &SH);

land= HPLand ();

H_init= HPGIH (P, SH);
W_init= HPGIW (P, SW);

if ((W_init>160) && land) clos= true;
if ((H_init>160) && (!land)) clos= true;

if (g_CollapseAPIVersion>0) {
	PINSetInputAreaState (clos);
}
if (g_CollapseAPIVersion==sonyVersion1) {
	Boolean size;
	SilkLibEnableResize (g_sonySilkRefNum);
	SilkLibResizeDispWin (g_sonySilkRefNum, clos);
	if (land) size= (P->W_first!=hp_standard); else
		size= (P->H_first!=hp_standard);
	if (size) SilkLibEnableResize (g_sonySilkRefNum);
	else SilkLibDisableResize (g_sonySilkRefNum);
//	if (P->H_last!=0) HPCB (0);
	if ((P->H_last!=SH) || (P->W_last!=SW)) HPCB (0);
	P->H_last= SH;
	P->W_last= SW;
}
if (g_CollapseAPIVersion<=sonyVersion2) {
	Boolean size;
	UInt16 m= 1;
	if (g_CollapseAPIVersion<=sonyVersion3) m= 3;
	VskSetState (g_sonySilkRefNum, vskStateEnable, m);
	VskSetState (g_sonySilkRefNum, vskStateResize, clos);
	if (land) size= (P->W_first!=hp_standard); else
		size= (P->H_first!=hp_standard);
	VskSetState (g_sonySilkRefNum, vskStateEnable, size*m);
//	if (P->H_last!=0) HPCB (0);
	if ((P->H_last!=SH) || (P->W_last!=SW)) HPCB (0);
	P->H_last= SH;
	P->W_last= SW;
}
}

Boolean HPChanged (FormPtr frmP, THP* P) {
UInt16 SW;
UInt16 SH;
Boolean result;
Boolean land;
WinGetDisplayExtent (&SW, &SH);

result= ((P->H_last!=SH) || (P->W_last!=SW));

land= HPLand ();
if (g_CollapseAPIVersion>0) {
	Boolean size;
	Boolean rota= false;
	if (SH==SW) rota= true; else
		if (land) rota= (P->H_first!=hp_standard);
		else rota= (P->W_first!=hp_standard);
	SysSetOrientationTriggerState (rota);
	if (land) size= (P->W_first!=hp_standard); else
		size= (P->H_first!=hp_standard);
	PINSetInputTriggerState (!size);
}
P->H_last= SH;
P->W_last= SW;
return result;
}

#ifdef use_HPMoveGSI
void HPMoveGSI (FormType* frmP, Int16 x, Int16 y) {
UInt16 numObjectsOnForm;
UInt16 index;
RectangleType bounds;
numObjectsOnForm= FrmGetNumberOfObjects (frmP);
for (index= numObjectsOnForm- 1; index>=0; index--) {
	if (frmGraffitiStateObj==FrmGetObjectType (frmP, index)) {
		FrmGetObjectBounds (frmP, index, &bounds);
		bounds.topLeft.x= x;
		bounds.topLeft.y= y;
		FrmSetObjectBounds (frmP, index, &bounds);
		break;
	}
}
}
#endif

Err HPCB (SysNotifyParamType* notifyParamsP) {
EventType eventToAdd;
MemSet (&eventToAdd, sizeof(EventType), 0);
eventToAdd.eType= (eventsEnum)winDisplayChangedEvent;
EvtAddUniqueEventToQueue (&eventToAdd, 0, 1);
return 0;
}

void HP1 () {
Err error= errNone;
UInt16 cardNo;
LocalID dbID;
UInt32 version;
error= SysCurAppDatabase (&cardNo, &dbID);
error= FtrGet ('pins', 1, &version);
if (!error) {
	g_CollapseAPIVersion= version;
	error= SysNotifyRegister (cardNo, dbID, 'scrs', HPCB, 0, 0);
	return;
}
if ((error= SysLibFind ("Sony Silk Library", &g_sonySilkRefNum))) {
	if (error==(0x0500 | 10)) {
		error= SysLibLoad ('libr', 'SlSi', &g_sonySilkRefNum);
		g_CollapseAPIVersion= 0;
	}
}
if (error) return;
if (error= FtrGet ('SoNy', 0x0102, &version)) {
	if (SilkLibOpen (g_sonySilkRefNum)== errNone) { 
		SilkLibEnableResize (g_sonySilkRefNum);
		g_CollapseAPIVersion= sonyVersion1;
	} else g_CollapseAPIVersion= 0;
} else {
	if (VskOpen (g_sonySilkRefNum)== errNone) { 
		if (VskGetAPIVersion (g_sonySilkRefNum)>= 0x00000003) {
			VskSetState (g_sonySilkRefNum, vskStateEnable, (vskResizeVertically|vskResizeHorizontally));
			g_CollapseAPIVersion= sonyVersion3;
		} else {
			VskSetState (g_sonySilkRefNum, vskStateEnable, vskResizeVertically);
			g_CollapseAPIVersion= sonyVersion2;
		}
	} else g_CollapseAPIVersion= 0;
} 
SysNotifyRegister (cardNo, dbID, 'scrd', HPCB, 0, 0);
}

void HP0 () {
UInt16 cardNo;
LocalID dbID;
SysCurAppDatabase (&cardNo, &dbID);
if (g_CollapseAPIVersion>0) {
	SysNotifyUnregister (cardNo, dbID, 'scrs', 0);
} else if (g_CollapseAPIVersion<0) {
	if (g_CollapseAPIVersion==sonyVersion1)
		SilkLibResizeDispWin (g_sonySilkRefNum, silkResizeNormal);
	SilkLibDisableResize (g_sonySilkRefNum);
	SilkLibClose (g_sonySilkRefNum);
	SysNotifyUnregister (cardNo, dbID, 'scrd', 0);
}
}

