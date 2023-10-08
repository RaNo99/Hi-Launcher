TRUN RUN;

THP* HPP;
Tdbi* DBI;
#define DBdata 0
#define DBtemp 1
#define DBswap 2

MemHandle* bmph;

/***************************************/
//1:

UInt32* (*LAE) (UInt16* exc, UInt16 w);
LocalID (*GFD) (UInt32 type);
T1recent* (*LAR) ();

/***************************************/
//2:

void* (*GOP) (UInt16 id);
UInt16 (*CGV) (UInt16 id);
void (*CSV) (UInt16 id, UInt16 v);
void (*SFT) (UInt16 id, char* c, Boolean draw);
void (*HO) (UInt16 ID);
void (*SO) (UInt16 ID);
void (*SA) (UInt16* A);
void (*HA) (UInt16* A);
Boolean (*LRN) (EventPtr e);
Boolean (*KSL) (EventPtr e);
Boolean (*H1400) (EventPtr e);
void (*RHD) (MemHandle H, UInt16* C);
void (*PRF) ();
void (*TLI) (UInt16 trig, UInt16 list, Int16 ind);
void (*PC) (UInt16 w);
void (*SAP) (UInt16* P);
Boolean (*H1300) (EventPtr e);
Boolean (*H1310) (EventPtr e);
Boolean (*H1320) (EventPtr e);
Boolean (*H1330) (EventPtr e);
Boolean (*H1340) (EventPtr e);
void (*DBH) (MemHandle h, Coord x, Coord y);
void (*D6000) (Int16 i, RectangleType* r, Char** ch);
void (*D6020) (Int16 i, RectangleType* r, Char** ch);
void (*D6021) (Int16 i, RectangleType* r, Char** ch);
void (*D6022) (Int16 i, RectangleType* r, Char** ch);
DmOpenRef (*LDB) (Tdbi* dbi, UInt16 mode);
void (*DAR) (DmOpenRef db);
void (*CAR) (DmOpenRef d1, DmOpenRef d2, DmResType except);
Boolean (*H1010) (EventPtr e);

/***************************************/
