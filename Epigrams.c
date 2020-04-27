/****************************************************************************
 / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / /
  / /  \ \ \ \ \ \ \ \ \ \ \ \ \ \ \ \ \ \ \ \ \  / / / / / / / / / / / / /
 / / DEFINE WE NEED FOR OUR COMPILATION PROCEDURE  / / / / / / / / / / / /
   \ \ \ \ \ \ \ \ \ \ \ \ \ \ \ \ \ \ \ \ \ \ \ \  / / / / / / / / / / / /
 / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / /
****************************************************************************/
#define ROOT_FILE    1      // identify this file as the `root-file'

/****************************************************************************
 / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / /
  / /  \ \ \ \  / / / / / / / / / / / / / / / / / / / / / / / / / / / / / /
 / / INCLUDES  / / / / / / / / / / / / / / / / / / / / / / / / / / / / / /
   \ \ \ \ \ \  / / / / / / / / / / / / / / / / / / / / / / / / / / / / / /
 / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / /
****************************************************************************/
#include    "Public.h"
#include    "Epigrams.h"

/****************************************************************************
 / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / /
  / /  \ \ \ \ \ \ \ \ \ \ \  / / / / / / / / / / / / / / / / / / / / / / /
 / / INITIALIZATION FUNCTIONS  / / / / / / / / / / / / / / / / / / / / / /
   \ \ \ \ \ \ \ \ \ \ \ \ \ \  / / / / / / / / / / / / / / / / / / / / / /
 / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / /
****************************************************************************/
void main( int argc, char ** argv )
{
        // prepare title_bar for screen and commodities
    strcpy( title_bar, NAME );
    strcat( title_bar, " " );
    strcat( title_bar, VERSION);
    strcat( title_bar, ", " );
    strcat( title_bar, COPYRIGHT );
    strcat( title_bar, " " );
    strcat( title_bar, AUTHOR);


    if ( ReadToolTypes( argc, argv ) )  {
        SeedPRNG();

        if ( InitRexxPort() )
            if ( InitCommodity() )
                if ( InitBackground() )
                    MainSeq();

        CleanUp();
    }

    exit(0);
}
/***************************************************************************/
void CleanUp()
{
    CleanUpBackground();
    CleanUpCommodity();
    CleanUpRexxPort();
}
/***************************************************************************/
void MainSeq()
{
        // run `hotkey activation' once if `_startup' is active
    if ( _startup != STARTUP_NEVER )    {
        BOOL    tempstore = FALSE;
        if ( (HandleOnceADay() == TRUE) || (_startup == STARTUP_ALWAYS) )    {

                // disable `poptofront' temporarily so bootscreens etc. work correctly
            if ( _poptofront == TRUE )  {
                _poptofront = FALSE;
                tempstore = TRUE;
            }
                // run main sequence once
            MainGUISequence();

                // if the user pressed quit in respose to a requester
                // then return immediately
            if ( ! system_state )
                return;

                // turn `poptofront' back on if appropriate
            if ( tempstore = TRUE )
                _poptofront = TRUE;
        }
    }

        // sit in `eternal' loop until a CX_KILL message is sent
    while ( (WaitForMessages() == TRUE) )
        ;
}
/***************************************************************************/
BOOL ReadToolTypes( int number, STRPTR values[] )
{
    BOOL        return_val = TRUE;

    UBYTE       ** ToolArray;

    char        TempTool_buff[255];
    STRPTR      TempTool = TempTool_buff;


        // parse tooltypes/CLI instructions
    ToolArray = ArgArrayInit( number, values );

        // look for tooltypes
    if ( TempTool = FindToolType(ToolArray, "CX_POPKEY") ) {
        if (strlen(TempTool) < 40)
            strcpy(_opthotkey, TempTool);
    }

    if ( TempTool = FindToolType(ToolArray, "CX_PRIORITY") )
        _cxpriority = atol(TempTool);

    if ( TempTool = FindToolType(ToolArray, "DATAFILE") )   {
        if (strlen(TempTool) < MAX_FN_SIZE )
            strcpy(_dfilename, TempTool);

        HandlePossibleDir();
    }

    if ( TempTool = FindToolType(ToolArray, "FAVEFILE") )   {
        if (strlen(TempTool) < MAX_FN_SIZE )
            strcpy(_favourites_file, TempTool);
    }

    if ( TempTool = FindToolType(ToolArray, "EXTERNAL") )   {
        if (strlen(TempTool) < MAX_EXTERNAL_NAME ) {
            strcpy( _external, TempTool );
            _externalquery = TRUE;
        }
    }

    if ( TempTool = FindToolType(ToolArray, "EXTERNAL%") )  {
        if ( (_extp >= 0) && (_extp <= 100 ) )
            _extp = atoi(TempTool);
    }

    if ( TempTool = FindToolType(ToolArray, "FONT") )   {
        if ( stricmp(TempTool, "SCREEN") == 0 )  {
            _usescreenfont = TRUE;
        } else {
            if (strlen(TempTool) < MAX_FONT_NAME_SIZE)     {
                _usescreenfont = FALSE;
                strcpy(_fontname, TempTool);
                ShortenString(TempTool, strlen(TempTool) - 5);
                if ( stricmp( TempTool, ".font" ) != 0 )
                    strcat(_fontname, ".font");
            }
        }
    }

    if ( TempTool = FindToolType(ToolArray, "FONTSIZE") )
        _fontsize = atoi(TempTool);

    if ( TempTool = FindToolType(ToolArray, "BGCOLOUR") )
        _bg_colour= atoi(TempTool);

    if ( TempTool = FindToolType(ToolArray, "BGPATTERN") ) {
        if (strlen(TempTool) < MAX_BG_NAME_SIZE)
            strcpy(_background, TempTool);
    }

    if ( TempTool = FindToolType(ToolArray, "BEVEL") )  {
        if ( stricmp(TempTool, "NORMAL") == 0 )
            _bevel = BEV_NORMAL;
        if ( stricmp(TempTool, "COMPOUND") == 0 )
            _bevel = BEV_COMPOUND;
        if ( stricmp(TempTool, "RAISED") == 0 )
            _bevel = BEV_RAISED;
        if ( stricmp(TempTool, "RECESSED") == 0 )
            _bevel = BEV_RECESSED;
        if ( stricmp(TempTool, "DOUBLE") == 0 )
            _bevel = BEV_DOUBLE;
        if ( stricmp(TempTool, "NONE") == 0 )
            _bevel = BEV_NONE;
    }

    if ( TempTool = FindToolType(ToolArray, "PEN") )
        _pen = atoi(TempTool);

    if ( TempTool = FindToolType(ToolArray, "PAPER") )
        _paper = atoi(TempTool);

    if ( TempTool = FindToolType(ToolArray, "DRAWMODE") )   {
        if ( stricmp(TempTool, "JAM1") == 0 )
            _draw_mode = JAM1;
        if ( stricmp(TempTool, "JAM2") == 0 )
            _draw_mode = JAM2;
        if ( stricmp(TempTool, "COMPLEMENT") == 0 )
            _draw_mode = COMPLEMENT;
        if ( stricmp(TempTool, "INVERSVID") == 0 )
            _draw_mode = INVERSVID;
    }

    if ( TempTool = FindToolType(ToolArray, "XPAD") )
        _xpad = atoi(TempTool);

    if ( TempTool = FindToolType(ToolArray, "YTOP") )
        _ytop = atoi(TempTool);

    if ( TempTool = FindToolType(ToolArray, "YBOT") )
        _ybot = atoi(TempTool);

    if ( TempTool = FindToolType(ToolArray, "LSPACE") )
        _lspace = atoi(TempTool);

    if ( TempTool = FindToolType(ToolArray, "TABSIZE") )
        _tabsize = atoi(TempTool);

    if ( TempTool = FindToolType(ToolArray, "MAXLINESIZE") )    {
        _maxlinesize = atoi(TempTool);

        if ( _maxlinesize > MAX_MAXLINESIZE )
            _maxlinesize = DEF_MAXLINESIZE;
    }

    if ( TempTool = FindToolType(ToolArray, "POPONSTARTUP") )   {
        if ( stricmp(TempTool, "NEVER") == 0 )
            _startup = STARTUP_NEVER;

        if ( stricmp(TempTool, "ALWAYS") == 0 )
            _startup = STARTUP_ALWAYS;

        if ( stricmp(TempTool, "ONCEADAY") == 0 )
            _startup = STARTUP_ONCEADAY;
    }

    if ( TempTool = FindToolType(ToolArray, "POPTOFRONT") )
        _poptofront = TRUE;

    if ( TempTool = FindToolType(ToolArray, "POPBACK") )
        _popback = TRUE;

    if ( TempTool = FindToolType(ToolArray, "RELAUNCH") )   {
        if ( stricmp(TempTool, "POP") == 0 )
            _relaunch = RELAUNCH_POP;

        if ( stricmp(TempTool, "QUIT") == 0 )
            _relaunch = RELAUNCH_QUIT;

        if ( stricmp(TempTool, "RELOAD") == 0 )
            _relaunch = RELAUNCH_RELOAD;
    }

    if ( TempTool = FindToolType(ToolArray, "RANDOMMODE") )   {
        if ( stricmp(TempTool, "BASIC") == 0 )
            _random_mode = RAND_BASIC;

        else if ( stricmp(TempTool, "MODIFIER") == 0 )
            _random_mode = RAND_MODIFIER;

        else if ( stricmp(TempTool, "MD5") == 0 )
            _random_mode = RAND_MD5;

        else    {
            return_val = DoErrors( WARNING,
                      "RANDOMMODE specified, doesn't exist",
                      "mis-spelt tooltype|docs don't tally with executable?",
                      "Continue with Basic PRNG|Quit" );
        }
    }

    if ( TempTool = FindToolType(ToolArray, "MD5DIGEST") )   {
        if (strlen(TempTool) < MAX_FN_SIZE )
            strcpy(_md5digestname, TempTool);
    }

    if ( TempTool = FindToolType(ToolArray, "NOESCAPE") )
        _noescape = TRUE;

    if ( TempTool = FindToolType(ToolArray, "NOREXX") )
        _norexx = TRUE;

    if ( TempTool = FindToolType(ToolArray, "TEST") )
        _test = TRUE;


        // dispense with tool array
    ArgArrayDone();


    return( return_val );
}
/***************************************************************************/
BOOL HandleOnceADay( void )
{
    BOOL                return_val = TRUE;

    char                filename_buffer[128];
    STRPTR              filename = filename_buffer;
    BPTR                filehandle;

    char                date_buff[128];
    unsigned int        lastdate;
    unsigned int        time[2];
    unsigned int        thisdate;

    struct   ClockData  lastdata;
    struct   ClockData  thisdata;


        // only enter if user wants a once a day epigram
    if ( _startup == STARTUP_ONCEADAY ) {

            // get timer values
        if ( timer( time ) == 0 ) {

                // save off seconds to another variable
            thisdate = time[0];
                // convert date to sensible values
            Amiga2Date( thisdate, &thisdata );

                // prepare filename for ENV:
            strcpy( filename, "ENV:" );
            strcat( filename, ONCEADAY_DATEDATA_NAME );

                /* attempt to open filename;
                    if unsucessful then we know that it hasn't been
                    started today or any day */
            if ( filehandle = Open( filename, MODE_OLDFILE ) )  {
                Seek( filehandle, 0, OFFSET_BEGINNING );
                Read( filehandle, date_buff, 128 );
                lastdate = atoi( date_buff );
                Amiga2Date( lastdate, &lastdata );

                    // comparing dates
                if ( thisdata.year == lastdata.year )
                    if ( thisdata.month == lastdata.month )
                        if ( thisdata.mday == lastdata.mday )
                                return_val = FALSE;
            }
            Close( filehandle );

                // write date data to ENV:
            if ( filehandle = Open( filename, MODE_NEWFILE ) )  {
                int     length;

                length = sprintf( date_buff, "%d", thisdate );
                Seek( filehandle, 0, OFFSET_BEGINNING );
                Write( filehandle, date_buff, length );
            }
            Close( filehandle );

                // prepare filename for ENVARC:
            strcpy( filename, "ENVARC:" );
            strcat( filename, ONCEADAY_DATEDATA_NAME );

                // write date data to ENVARC:
            if ( filehandle = Open( filename, MODE_NEWFILE ) )  {
                int     length;

                length = sprintf( date_buff, "%d", thisdate );
                Seek( filehandle, 0, OFFSET_BEGINNING );
                Write( filehandle, date_buff, length );
            }
            Close( filehandle );
        }
    }

    return( return_val );
}

/****************************************************************************
 / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / /
  / /  \ \ \ \ \ \ \ \  / / / / / / / / / / / / / / / / / / / / / / / / / /
 / / MAIN WAIT FUNCTION  / / / / / / / / / / / / / / / / / / / / / / / / /
   \ \ \ \ \ \ \ \ \ \ \  / / / / / / / / / / / / / / / / / / / / / / / / /
 / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / /
****************************************************************************/
BOOL WaitForMessages( void )
{
    ULONG       CxMask      = NULL,
                RexxMask    = NULL,
                GUIMask     = NULL,
                MaskResult;

    BOOL        quit_prog = FALSE;


    CxMask = 1L << BrokerMP->mp_SigBit;
    if ( EpigramsRxPort )
        RexxMask = 1L << EpigramsRxPort->mp_SigBit;
    if ( interface == TRUE )
        GUIMask = 1L << EpigramsWnd->UserPort->mp_SigBit;

    MaskResult = Wait( SIGBREAKF_CTRL_C    |
                       CxMask              |
                       RexxMask            |
                       GUIMask              );

    if ( MaskResult & SIGBREAKF_CTRL_C )    quit_prog = TRUE;
    if ( MaskResult & CxMask )              HandleCommodityMsgs( &quit_prog );
    if ( MaskResult & RexxMask )            HandleRexxMsgs( &quit_prog );
    if ( MaskResult & GUIMask )             HandleGUIMsgs();


        // set local quit flag if gloabl quit flag is set
    if ( system_state == FALSE )   quit_prog = TRUE;
        // make sure we close the window before we exit
    if ( quit_prog == TRUE )     interface = FALSE;
        // close window if interface is set to FALSE
    if ( interface == FALSE )   GUIGoAway(0);

        // return relevent boolean depending on the execution state
    if ( quit_prog == TRUE )     return( FALSE );
    else    return( TRUE );
}

/****************************************************************************
 / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / /
  / /  \ \ \ \ \ \ \ \  / / / / / / / / / / / / / / / / / / / / / / / / / /
 / / SUPPORT FUNCTIONS / / / / / / / / / / / / / / / / / / / / / / / / / /
   \ \ \ \ \ \ \ \ \ \  / / / / / / / / / / / / / / / / / / / / / / / / / /
 / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / /
****************************************************************************/
    // clears timings for mouse clicks
void FlushDoubleClick( void )
{
    lastsecs = 0;
    lastmicros = 0;
}
/***************************************************************************/
void HandlePossibleDir()
{
    BPTR                    lock = NULL;
    struct FileInfoBlock    fib;



    if ( lock = Open( _dfilename, MODE_OLDFILE ) )
    {
        if ( !ExamineFH( lock, &fib ) )
        {
            Close( lock );
            puts("can't examine file");
            return;
        }

        if ( fib.fib_DirEntryType > 0 )
            puts("file is a directory");
        else if ( fib.fib_DirEntryType < 0 )
            puts("file is a normal");

        Close( lock );
    } else
        puts("can't lock file");
}
/***************************************************************************/
