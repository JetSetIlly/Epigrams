
/****************************************************************************
 / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / /
  / /  \ \ \ \ \  / / / / / / / / / / / / / / / / / / / / / / / / / / / / /
 / / PROTOTYPES  / / / / / / / / / / / / / / / / / / / / / / / / / / / / /
   \ \ \ \ \ \ \  / / / / / / / / / / / / / / / / / / / / / / / / / / / / /
 / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / /
****************************************************************************/
void    RefreshWin( void );
void    SaveScreenName( void );
void    PopOldScreenToFront( void );
void    FindFile( void );

void    MainCookieSequence( void );
BOOL    OpenNormalFont( struct Cookie_Info * );
void    CloseNormalFont( struct Cookie_Info * );
BOOL    OpenDataFile( struct Cookie_Info * );
void    CloseDataFile( struct Cookie_Info * );
void    RandomizeFilePos( struct Cookie_Info * );
BOOL    FindNextCookie( struct Cookie_Info * );
void    CountCookieSize( struct Cookie_Info * );
void    GetWindowSizeCorrect( struct Cookie_Info * );
BOOL    DoWindow( struct Cookie_Info * );
void    ReReadCookieAndPrint( struct Cookie_Info * );

BOOL    FindCookieDelimeter( BPTR, LONG );
BOOL    CheckEOC( STRPTR, LONG );
void    ExpandTABs( STRPTR );


/****************************************************************************
 / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / /
  / /  \ \ \ \ \  / / / / / / / / / / / / / / / / / / / / / / / / / / / / /
 / / STRUCTURES  / / / / / / / / / / / / / / / / / / / / / / / / / / / / /
   \ \ \ \ \ \ \  / / / / / / / / / / / / / / / / / / / / / / / / / / / / /
 / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / /
****************************************************************************/
struct Cookie_Info {
    struct  TextFont *  Font_Handle;
    BPTR                data_file_handle;

    struct  List        cookie_contents;

        // VARIABLES TO KEEP TRACK OF THE SELECTED COOKIE
    ULONG   maxlinelen,         // LENGTH IN PIXELS OF THE LONGEST LINE IN THE COOKIE
            numlines;           // THE NUMBER OF LINES IN THE COOKIE
};


void InitializeCookieInfo( struct Cookie_Info * info )
{
    NewList( &info->cookie_contents );
    info->numlines = info->maxlinelen = 0;
}
