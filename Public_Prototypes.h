
// ------------- prototypes etc. -------------

    // ---- Background.c
void    DrawBackground( void );
BOOL    InitBackground( void );
void    CleanUpBackground( void );
void    TileBitMap(struct BitMap *,WORD, WORD, WORD, WORD, struct BitMap * );


    // ---- Commodity.c
BOOL    InitCommodity( void );
void    CleanUpCommodity( void );
void    HandleCommodityMsgs( BOOL * );


    // ---- Cookie.c
void    MainGUISequence( void );
void    SaveFavourite( void );


    // ---- External.c
BOOL    ExternalPercent( void );
void    HandleExternal( void );


    // ---- Epigrams.c
void    CleanUp( void );
void    MainSeq( void );
BOOL    ReadToolTypes( int, STRPTR [] );
BOOL    HandleOnceADay( void );
BOOL    WaitForMessages( void );
void    FlushDoubleClick( void );
void    HandlePossibleDir( void );


    // ---- GUI.c
BOOL    PrepareGUIEnv( void );
void    GUIGoAway( int );
enum GUIGOAWAY_OPTS {
    NOERROR, NOSCREEN, NOWINDOW
    };
void    DrawBevelBoxes( void );
BOOL    SetupScreen( void );
void    CloseDownScreen( void );
BOOL    HandleEpigramsIDCMP( void );
BOOL    OpenEpigramsWindow( void );
void    CloseEpigramsWindow( void );
void    HandleGUIMsgs( void );


    // ---- Rexx.c
BOOL    InitRexxPort( void );
void    CleanUpRexxPort( void );
void    HandleRexxMsgs( BOOL * );


    // ---- Support.c
void    ShortenString( STRPTR, int );
void    DelLeadingSpaces( STRPTR );
void    SeedPRNG( void );
ULONG   PRNG( ULONG, BYTE );
BOOL    DoErrors( BOOL, STRPTR, STRPTR, STRPTR );
    #define     WARNING     TRUE
    #define     FATAL       FALSE
void    About( void );
