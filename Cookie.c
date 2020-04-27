/****************************************************************************
 / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / /
  / /  \ \ \ \  / / / / / / / / / / / / / / / / / / / / / / / / / / / / / /
 / / PRE-AMBLE / / / / / / / / / / / / / / / / / / / / / / / / / / / / / /
   \ \ \ \ \ \  / / / / / / / / / / / / / / / / / / / / / / / / / / / / / /
 / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / /
*****************************************************************************

 Procedural Map
 ~~~~~~~~~~~~~~
    NB.  the intention of this diagram is merely to illustrate where the
    different procedures\functions are used, it is NOT really intended as
    a flow diagram or anything else of that ilk.

    NB/ Function names in brackets denote functions found in other source
        files.


 MainGUISequence
    |
    +- FlushDoubleClick
    +- RefreshWin
    +- SaveScreenName
    +- FindFile
    +- MainCookieSequence -------+- (SetUpScreen)
    |                            +- OpenNormalFont
    |                            +- OpenDataFile
    |                            +- RandomizeFilePos
    |                            +- FindNextCookie
    |                            +- CountCookieSize -----------+- CheckEOC
    |                            |                             +- ExpandTABs
    |                            +- GetWindowSizeCorrect
    |                            +- DoWindow
    |                            +- ReReadCookieAndPrint ------+- ExpandTABs
    |                            +- CloseDataFile
    |                            +- CloseNormalFont
    +- PopOldScreenToFront

****************************************************************************

 Calculating Window Geometry
 ~~~~~~~~~~~~~~~~~~~~~~~~~~~
A  great  deal  of  effort  has  been put into finding a correct method for
opening the correct size of window for the selected text.  As the algorithm
spreads itself naturally over many procedures it is worth summarizing here,
the technique employed.

    - Firstly, the cookie at the specified location is read line by line
        the length of each line (in pixels) being stored if it is the longest
        so far.

    - The number of lines in the cookie is noted.

    - The width of the window is thus,
            the longest line (maxlinelen) +
            the height of the leftmost border of the system windows +
            the size of the rightmost border 
            + _xpad

    - The height of the window is equal to 
            the number of lines in the cookie
            * the intrinsic height of the font (Font_Handle->tf_Ysize)
            + the upper and lower borders

    Also  we  need  to  add  the  required  tween  line  spacing  (_lspace)
    multiplied by the number of lines less one; we should add the
    values _ytop and _ybot.

It  is  worth  noting  that as we havn't actually opened the window yet, we
need to address the Screen Structure we have opened to find the size of the
border of the window once it is opened.  eg/

    EpigramsScr->WBorLeft

    EpigramsScr->WBorTop

We  need  to  take  note,  that  we can't predict at this stage (using this
method)  the  size  of the borders if we later in the programs development,
allow the addition of drag bars, sizing gadgets and so forth.

Another  important  note  to  make  which  would  perhaps  not otherwise be
obvious,  is that some fonts will still look `funny' (needless space at the
top  of  even  the  tallest  character  etc.)  despite  our  efforts to the
contrary;  this  is not something we can help and is a attribute that would
be  intrinsic  to  the font design itself.  In an attempt to remedy this we
have  allowed  the  user  the  option  to  alter the top and bottom padding
seperately (_ytop and _ybot).


Non-System Window Borders
~~~~~~~~~~~~~~~~~~~~~~~~~
It  is  possible  in  Epigrams  to  open  the window with non-system window
borders.   This  causes  additional  complexity in the source code.  In the
event  of  a re-coding, note that changes in the following functions may be
necessary.

    GetWindowSizeCorrect()  --  The contents of this function are described
in detail above.

    ReReadCookieAndPrint()  --  In  this function we work out where the pen
needs  to be placed in both x and y terms.  It should be apparent that this
will depend on the border strategy used.

****************************************************************************
 / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / /
  / /  \ \ \ \  / / / / / / / / / / / / / / / / / / / / / / / / / / / / / /
 / / INCLUDES  / / / / / / / / / / / / / / / / / / / / / / / / / / / / / /
   \ \ \ \ \ \  / / / / / / / / / / / / / / / / / / / / / / / / / / / / / /
 / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / /
****************************************************************************/
#include    "Public.h"
#include    "Cookie.h"

/****************************************************************************
 / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / /
  / /  \ \ \ \ \ \ \ \  / / / / / / / / / / / / / / / / / / / / / / / / / /
 / / MAIN GUI SEQUENCE / / / / / / / / / / / / / / / / / / / / / / / / / /
   \ \ \ \ \ \ \ \ \  / / / / / / / / / / / / / / / / / / / / / / / / / / /
 / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / /
****************************************************************************/
void MainGUISequence()
{
    FlushDoubleClick();
    RefreshWin();
    SaveScreenName();

    FindFile();
    MainCookieSequence();
    PopOldScreenToFront();
}
/***************************************************************************/
void RefreshWin()
{
        // --- CLOSE WINDOW JUST IN CASE ONE IS ALREADY OPEN
    if ( interface == TRUE )    {
        if ( _popback == TRUE ) {
            _popback = FALSE;
            GUIGoAway( NOERROR );
            _popback = TRUE;
        } else
            GUIGoAway( NOERROR );
    }
}
/***************************************************************************/
void SaveScreenName()
{
        // --- STORE ACTIVE SCREEN, IF RELEVENT, SO WE CAN POP BACK LATER
    if ( _popback == TRUE )
        ActiveScr = IntuitionBase->ActiveScreen;
}
/***************************************************************************/
void PopOldScreenToFront()
{
        // --- BRING SCREEN TO FRONT
    if ( _poptofront == TRUE )
        ScreenToFront( Scr );
}
/***************************************************************************/
void FindFile()
{
        // --- USE THIS UNTIL WE HAVE FUNCTION FOR RANDOMIZING FILES
    strcpy( file_used, _dfilename );
}
/***************************************************************************/



/****************************************************************************
 / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / /
  / /  \ \ \ \ \ \ \ \ \ \  / / / / / / / / / / / / / / / / / / / / / / / /
 / / MAIN COOKIE SEQUENCE  / / / / / / / / / / / / / / / / / / / / / / / /
   \ \ \ \ \ \ \ \ \ \ \ \  / / / / / / / / / / / / / / / / / / / / / / / /
 / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / /
****************************************************************************/
void MainCookieSequence()
{
    struct  Cookie_Info     info;


    InitializeCookieInfo( &info );


    if ( ! SetupScreen() )  {
        GUIGoAway( NOSCREEN );

        return;
    }

    if ( ! OpenNormalFont( &info ) ) {
        char    error_line_buff[MAX_FONT_NAME_SIZE + 32];
        STRPTR  error_line = error_line_buff;


        strcpy( error_line, "Can't open `");
        strcat( error_line, FilePart(_fontname) );
        strcat( error_line, "'" );

        system_state = DoErrors( WARNING,
            error_line,
            "not available on disk|low memory",
            "Continue with Default|Quit" );

        if ( system_state ) {
            strcpy( _fontname, DEF_FONT);
            _fontsize = DEF_FONT_SIZE;
        }

        return;
    }

    if ( ! OpenDataFile( &info ) ) {
        char    error_line_buff[MAX_FN_SIZE + 32];
        STRPTR  error_line = error_line_buff;


        strcpy( error_line, "Can't open `");
        strcat( error_line, FilePart(file_used) );
        strcat( error_line, "'" );

        system_state = DoErrors( WARNING,
            error_line,
            "low memory|not available on disk",
            "Continue|Quit" );

        return;
    }

    RandomizeFilePos( &info );
    if ( ! FindNextCookie( &info ) )
        return;

    CountCookieSize( &info );
    GetWindowSizeCorrect( &info );
    if ( DoWindow( &info ) )
        ReReadCookieAndPrint( &info );

    CloseDataFile( &info );
    CloseNormalFont( &info );
}
/***************************************************************************/
BOOL OpenNormalFont( struct Cookie_Info * info )
{
        // --- SET FONT ATTRIBUTES
    if ( _usescreenfont == TRUE )  {
        Font_Attr.ta_Name = Scr->Font->ta_Name;
        Font_Attr.ta_YSize = Scr->Font->ta_YSize;
    } else  {
        Font_Attr.ta_Name =  _fontname;
        Font_Attr.ta_YSize = _fontsize;
    }
    Font_Attr.ta_Style = FS_NORMAL;
    Font_Attr.ta_Flags = FPB_DISKFONT;

    if ( info->Font_Handle = OpenDiskFont( &Font_Attr ) )
        return TRUE;
    else
        return FALSE;
}
/***************************************************************************/
void CloseNormalFont( struct Cookie_Info * info )
{
    if ( info->Font_Handle )  {
        CloseFont( info->Font_Handle );
        info->Font_Handle = NULL;
    }
}
/***************************************************************************/
BOOL OpenDataFile( struct Cookie_Info * info )
{
    if ( info->data_file_handle = Open( file_used, MODE_OLDFILE ) )
        return TRUE;
    else
        return FALSE;
}
/***************************************************************************/
void CloseDataFile( struct Cookie_Info * info )
{
    Close( info->data_file_handle );
}
/***************************************************************************/
void RandomizeFilePos( struct Cookie_Info * info )
{
    ULONG   data_file_size;


        // --- FIND FILE SIZE
    Seek( info->data_file_handle, 0, OFFSET_END );
    data_file_size = Seek( info->data_file_handle, 0, OFFSET_BEGINNING );

        // --- RANDOMIZE STARTPOINT IN FILE
    byte_pos = PRNG( data_file_size, _random_mode );
}
/***************************************************************************/
BOOL FindNextCookie( struct Cookie_Info * info )
{
    char    error_line_buff[MAX_FN_SIZE + 32];
    STRPTR  error_line = error_line_buff;


    if ( FindCookieDelimeter( info->data_file_handle, MARKER_CH ) == FALSE ) {
        byte_pos = 0;
        if ( FindCookieDelimeter( info->data_file_handle, MARKER_CH ) == FALSE )

            strcpy( error_line, "No entries in `");
            strcat( error_line, FilePart(file_used) );
            strcat( error_line, "'" );

            system_state = DoErrors( WARNING,
                error_line,
                "add some cookies to the file!",
                "Continue|Quit" );

            return FALSE;
    }

        // --- GET CURRENT FILE POSITION AFTER THE SHENANAGINS IN FindCookieDelimeter();
    byte_pos = Seek( info->data_file_handle, 0, OFFSET_BEGINNING ) + 1;
    Seek( info->data_file_handle, byte_pos, OFFSET_BEGINNING );

    return TRUE;
}
/***************************************************************************/
    /*
        THIS FUNCTION NOW STORES THE COOKIE DATA IN MEMORY -- THEREBY SAVING
        TIME LATER BY READING THE COOKIE FROM MEMORY AND NOT FROM THE FILE AGAIN
    */
void CountCookieSize( struct Cookie_Info * info )
{
    struct  Node    * cookie_line;

    char    input_string[MAX_MAXLINESIZE];

    struct  RastPort    temp_rast;      // used to enable TextExtent to calculate width of string
    WORD    temp_len;                   // temporarily stores the length of
                                        // the string in pixels - is assigned to
                                        // the real length (maxlinelen),
                                        // if it is greater than the
                                        // current length

    BOOL    end_of_cookie = FALSE;


        // --- ESTABLISH RASTPORT NOW -- FOR USE WITH TextExtent()
    InitRastPort( &temp_rast );
    SetFont( &temp_rast, info->Font_Handle );

        // --- INITIALIZE VARIABLES
    info->maxlinelen = 0;
    info->numlines = 0;


        // --- LOOP THROUGH COOKIE
    do
    {
        if ( FGets( info->data_file_handle, input_string, _maxlinesize) == NULL ) // EOF probably
            end_of_cookie = TRUE;
        else
        {
            if ( CheckEOC(input_string, MARKER_CH) || (info->numlines > MAXNUMLINES) ) // OR for safety
                end_of_cookie = TRUE;
            else
            {
                    // --- REPLACE LAST CHARACTER (A NEW LINE) WITH A NULL TERMINATOR
                input_string[ strlen( input_string ) - 1 ] = '\0';

                    // --- GET RID OF ANY TAB CHARACTERS
                ExpandTABs(input_string);

                    // --- STORE VALUES IF REQUIRED
                if ( (temp_len = TextLength( &temp_rast, input_string, strlen(input_string))) > info->maxlinelen )
                    info->maxlinelen = temp_len;

                    // --- ADD COOKIE LINE TO THE LIST OF COOKIE LINES
                cookie_line = malloc( sizeof(struct Node));
                cookie_line->ln_Name = malloc( strlen(input_string) + 1);
                strcpy( cookie_line->ln_Name, input_string );
                AddTail( &info->cookie_contents, cookie_line );

                    // --- VALID DATA LINE SO INCREASE LINE COUNT
                info->numlines ++;
            }
        }
    }
    while ( end_of_cookie != TRUE );
}
/***************************************************************************/
void GetWindowSizeCorrect( struct Cookie_Info * info )
{
    UWORD   top_border = Scr->WBorTop,
            bottom_border = Scr->WBorBottom,
            left_border = Scr->WBorLeft,
            right_border = Scr->WBorRight;


    if ( _bevel != BEV_SYSTEM )
        if ((_bevel == BEV_RAISED) || (_bevel == BEV_RECESSED))
            top_border = bottom_border = left_border = right_border = 1 ;

    EpigramsWidth = (UWORD) ((_xpad * 2) + right_border + left_border + info->maxlinelen);

    EpigramsHeight = (UWORD) (    _ytop
                                + _ybot
                                + ((info->Font_Handle->tf_YSize + _lspace) * info->numlines)
                                - _lspace
                                + (top_border + bottom_border)
                             );

	EpigramsLeft = (UWORD) ((Scr->Width - EpigramsWidth) / 2);
	EpigramsTop = (UWORD) ((Scr->Height - EpigramsHeight) / 2);
}
/***************************************************************************/
BOOL DoWindow( struct Cookie_Info * info )
{
    if ( PrepareGUIEnv() == TRUE )  {
            // --- PREPARE PENS ETC.
        SetFont( EpigramsWnd->RPort, info->Font_Handle );
        SetAPen( EpigramsWnd->RPort, _pen );
        SetBPen( EpigramsWnd->RPort, _paper );
        SetDrMd( EpigramsWnd->RPort, _draw_mode );

        return TRUE;
    } else
        return FALSE;
}
/***************************************************************************/
void ReReadCookieAndPrint( struct Cookie_Info * info )
{
    struct  Node    * cookie_line;
    ULONG   ctr, offx, offy, orig_offx, orig_offy;


        // --- INITIALIZE OFFSETS
    if ( _bevel == BEV_SYSTEM )
    {
        orig_offx = offx = _xpad + EpigramsWnd->BorderLeft;
        offy = _ytop + EpigramsWnd->BorderTop;
    }
    else
    {
        if ((_bevel == BEV_RAISED) || (_bevel == BEV_RECESSED))
        {
            orig_offx = offx = _xpad + 1;
            offy = _ytop + 1;
        }
    }
    orig_offy = offy += info->Font_Handle->tf_Baseline;


        // --- SEEK TO APPROPRIATE POSITION
    Seek( info->data_file_handle, byte_pos, OFFSET_BEGINNING );

        // --- READ THROUGH COOKIE FOR THE APPROPRIATE NUMBER OF LINES
    for ( ctr = 1; ctr <= info->numlines; ctr ++ )
    {
        Move( EpigramsWnd->RPort, offx, offy);

            // --- GET NEXT COOKIE LINE FROM LIST
        cookie_line = RemHead( &info->cookie_contents );

            // --- PRINT LINE IN THE WINDOW
        Text(EpigramsWnd->RPort, cookie_line->ln_Name, strlen(cookie_line->ln_Name));

            // --- ADVANCE THE Y CO-ORD APPROPRIATELY
        offy += info->Font_Handle->tf_YSize + _lspace;

            // --- FREE MEMORY ASSOCIATED WITH NODE
        free( cookie_line->ln_Name );
        free( cookie_line );
    }
}
/***************************************************************************/


/***************************************************************************/
BOOL FindCookieDelimeter( BPTR dfile, LONG markerch )
{
    BOOL    done = FALSE;
    LONG    checkch;


    Seek( dfile, byte_pos, OFFSET_BEGINNING );

    while ( done == FALSE ) {
        checkch = FGetC(dfile);
        if ( checkch == -1 )
            return( FALSE );

        else if ( checkch == markerch )   {
            checkch = FGetC(dfile);

            if ( checkch == -1 )
                return( FALSE );
            if ( checkch == markerch )
                done = TRUE;
        }
    }

    return( TRUE );
}
/***************************************************************************/
BOOL CheckEOC( STRPTR str, LONG ch )
{
    if ( (str[0] == ch) && (str[1] == ch) )
        return( TRUE );
    else
        return( FALSE );
}
/***************************************************************************/
void ExpandTABs( STRPTR str )
{
    int     ctr, ctrb, copyctr;
    char    copystr[MAX_MAXLINESIZE];

    copyctr = 0;
    for ( ctr = 0; ctr < strlen(str); ctr ++ )      {
        if ( str[ctr] == '\t' ) {
            for ( ctrb = 0; ctrb < _tabsize; ctrb ++ )  {
                if ( strlen( str ) < _maxlinesize )  {
                    copystr[copyctr] = ' ';
                    copyctr ++;
                }
            }
        } else {
            copystr[copyctr] = str[ctr];
            copyctr ++;
        }
    }

        // --- TERMINATE COPY STRING
    copystr[copyctr] = '\0';

        // --- COPY NEW STRING BACK TO ORIGINAL STRING
    strcpy( str, copystr );
}
/***************************************************************************/



/****************************************************************************
 / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / /
  / /  \ \ \ \ \ \ \  / / / / / / / / / / / / / / / / / / / / / / / / / / /
 / / SAVE FAVOURITE  / / / / / / / / / / / / / / / / / / / / / / / / / / /
   \ \ \ \ \ \ \ \ \  / / / / / / / / / / / / / / / / / / / / / / / / / / /
 / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / /
****************************************************************************/
    // tHIS FUNCTION SAVES OUT THE CURRENT EPIGRAM INTO A FAVORITES FILE
    //   -- CALLED FROM IDCMP HANDLER IN GUI.c
void SaveFavourite()
{
    struct  Cookie_Info     info;
    struct  Node            * cookie_line;

    ULONG   ctr;
    BPTR    fave_file_handle;


    InitializeCookieInfo( &info );

        // --- OPEN SOURCE FILE
    if ( ! OpenDataFile( &info ) ) {
        system_state = DoErrors( WARNING,
            "Can't open file",
            "low memory|bad filename",
            "Continue|Quit" );

        return;
    }

        // --- OPEN TARGET FILE
    if ( ! (fave_file_handle = Open( _favourites_file, MODE_READWRITE )) ) {
        system_state = DoErrors( WARNING,
            "Can't open favourites file",
            "low memory|bad filename",
            "Continue|Quit" );

        return;
    }


        // --- JUMP TO THE APPROPRIATE POSITION OF DATA FILE
    Seek( info.data_file_handle, byte_pos, OFFSET_BEGINNING );

        // --- JUMP TO THE APPROPRIATE POSITION OF FAVE FILE
    Seek( fave_file_handle, 0, OFFSET_END );

        // --- ADD DELIMITER TO FAVES FILE
    FPuts( fave_file_handle, COOKIE_DELIMITER );

        // --- READ COOKIE
    CountCookieSize( &info );

        // --- DUMP COOKIE TO FAVE FILE
    for ( ctr = 1; ctr <= info.numlines; ctr ++ )
    {
            // --- GET NEXT LINE FROM LIST
        cookie_line = RemHead( &info.cookie_contents );

            // --- PRINT LINE IN FAVOURITES FILE
        FPuts( fave_file_handle, cookie_line->ln_Name );
    }


    CloseDataFile( &info );
    Close( fave_file_handle );
}
/***************************************************************************/
