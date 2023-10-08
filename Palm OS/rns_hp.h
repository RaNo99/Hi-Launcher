#define sonyVersion1		-1
#define sonyVersion2		-2
#define sonyVersion3		-3

/*
#define pinAPIVersion1_0		0x01000000
#define pinAPIVersion1_1		0x01103000
#define pinAPIVersion2_0		0x02003000

#define winDisplayChangedEvent 0x4101
#define pinMaxConstraintSize 0x7FFF

#define frmDIAPolicyStayOpen 0
#define frmDIAPolicyCustom 1

#define sysOrientationTriggerDisabled 0
#define sysOrientationTriggerEnabled 1

typedef enum {
pinInputTriggerEnabled,
pinInputTriggerDisabled,
pinInputTriggerNone
} PINInputTriggerStateType;

typedef enum {
pinInputAreaOpen,
pinInputAreaClosed,
pinInputAreaNone,
reserved1,
reserved2,
pinInputAreaUser,
pinInputAreaReset = 0xFFFF
} PINInputAreaStateType;

typedef struct {
UInt32 version;
PointType startPt;
PointType endPt;
} SysNotifyPenStrokeType;

typedef struct {
UInt32 notifyType;
UInt32 broadcaster;
SysNotifyPenStrokeType* notifyDetailsP; //void*
void* userDataP;
Boolean handled;
UInt8 reserved2;
} SysNotifyParamType;

Err FrmSetDIAPolicyAttr (FormPtr formP, UInt16 diaPolicy)
PINS_TRAP(14);

Err PINSetInputAreaState (UInt16 state)
PINS_TRAP(0);

Err PINSetInputTriggerState (UInt16 state)
PINS_TRAP(2);

Err SysSetOrientationTriggerState (UInt16 triggerState) PINS_TRAP(22);  */

typedef enum {
hp_standard,	//not sizeable, always 320
hp_keep,	//sizeable, keep from previous form
hp_plus	//sizeable, auto extend to >320
} hp_first_enum;
//form displayed for the first time

typedef enum {
hp_last,	//size from previous usage of this form
hp_first	//use initial size (hp_first_enum)
} hp_next_enum;
//form displayed again

typedef struct {
hp_first_enum H_first;	//vertical (height, y)
hp_first_enum W_first;	//landscape (width, x)
hp_next_enum H_next;	//vertical (height, y)
hp_next_enum W_next;	//landscape (width, x)
Coord H_last;
Coord W_last;
}  THP;

void HPLoad (FormPtr frmP, THP* P);
void HPEnter (THP* P);
Boolean HPChanged (FormPtr frmP, THP* P);
#ifdef use_HPMoveGSI
void HPMoveGSI (FormType* frmP, Int16 x, Int16 y);
#endif
void HP1 ();
void HP0 ();
Err HPCB (SysNotifyParamType* notifyParamsP);

