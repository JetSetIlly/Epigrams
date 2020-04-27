/****************************************************************************
 / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / /
  / /  \ \ \  / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / /
 / / INCLUDES  / / / / / / / / / / / / / / / / / / / / / / / / / / / / / /
   \ \ \ \ \ \  / / / / / / / / / / / / / / / / / / / / / / / / / / / / / /
 / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / /
****************************************************************************/
#include    "Public.h"
#include    "Support.h"

/****************************************************************************
 / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / /
  / /  \ \ \ \ \ \ \  / / / / / / / / / / / / / / / / / / / / / / / / / / /
 / / USEFUL FUNCTIONS  / / / / / / / / / / / / / / / / / / / / / / / / / /
   \ \ \ \ \ \ \ \ \ \  / / / / / / / / / / / / / / / / / / / / / / / / / /
 / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / /
****************************************************************************/
void ShortenString( STRPTR str, int amount )
{
    int     ctr;

    if ( amount > strlen(str) )
        return;

    for ( ctr = 0; ctr < strlen(str); ctr ++ )
        str[ctr] = str[amount + ctr];
}
/***************************************************************************/
void DelLeadingSpaces( STRPTR tmpstr )
{
    int     ctrb, ctr = 0;

    while ((tmpstr[ctr] == ' ') || (tmpstr[ctr] == '\t'))
        for (ctrb=ctr; ctrb < strlen(tmpstr); ctrb++)
            tmpstr[ctrb] = tmpstr[ctrb+1];
}

/****************************************************************************
 / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / /
  / /  \ \ \ \ \ \  / / / / / / / / / / / / / / / / / / / / / / / / / / / /
 / / RANDOM NUMBERS  / / / / / / / / / / / / / / / / / / / / / / / / / / /
   \ \ \ \ \ \ \ \ \  / / / / / / / / / / / / / / / / / / / / / / / / / / /
 / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / /
****************************************************************************/
void SeedPRNG( void )
{
    srand( time(NULL)^rand() );
}
/***************************************************************************/
ULONG PRNG( ULONG base, BYTE PRNG_type )
{
    ULONG       randnum, modifier;


    switch ( PRNG_type )    {
        case RAND_BASIC:
                        randnum = rand() % base;
                        break;
        case RAND_MODIFIER:
                        randnum = rand() % base;

                        modifier = rand() % (base % 10);
                        if ( rand() & 1L )
                            modifier *= -1;
                        randnum += modifier;
                        break;
        case RAND_MD5:
                        load_seed();
                        randnum = md5_rnd(0, base);
                        save_seed();
                        break;
    };

    return (randnum);
}

/****************************************************************************
 / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / /
  / /  \ \ \ \  / / / / / / / / / / / / / / / / / / / / / / / / / / / / / /
 / / DO-ERRORS / / / / / / / / / / / / / / / / / / / / / / / / / / / / / /
   \ \ \ \ \ \  / / / / / / / / / / / / / / / / / / / / / / / / / / / / / /
 / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / /
****************************************************************************/
BOOL DoErrors( BOOL errortype, STRPTR error, STRPTR solution_orig, STRPTR buttons )
{
    char    text[255];
    char    solution_copy[255];
    BOOL    return_val = TRUE;
    int     ctr, ctrb;
    struct  ReqToolsBase *ReqToolsBase;     // Base pointer for REQTools library



        // copy solution so we don't trash original string
    strcpy(solution_copy, solution_orig);

    if ( errortype == WARNING )
        strcpy( text, "WARNING: " );
    else
        strcpy( text, "FATAL: " );

        // add error info. onto text buffer
    strcat( text, error );

        // prepare solution lines
    strcat( text, "\n\n--- " );

        // build up solution lines
    do {
        if ( solution_copy[0] == '|' )
            strcat( text, "\n     " );
        else
            strncat( text, solution_copy, 1 );

            // shorten string
        for ( ctrb = 0; ctrb < strlen(solution_copy); ctrb ++ )
            solution_copy[ctrb] = solution_copy[1 + ctrb];
    } while ( solution_copy[0] != '\0' );


    if ( ReqToolsBase = (struct ReqToolsBase *) OpenLibrary (REQTOOLSNAME, REQTOOLSVERSION) ) {
        struct TagItem tags[] = {
            RTEZ_ReqTitle, (ULONG) NAME,
            TAG_END };

        ULONG   what;


        what = rtEZRequest( text,
                     buttons,
                     NULL,
                     (struct TagItem *)tags,
                     NULL );

        switch ( what ) {
                // the default OR/ leftmost button
            case 1:
                return_val = TRUE;
                break;

                // the rightmost button when more than one button
                // should be QUIT by convention
            case 0:
                return_val = FALSE;
                break;
        }

        CloseLibrary ((struct Library *)ReqToolsBase);
    } else
        puts("text");


    return( return_val );
}

/****************************************************************************
 / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / /
  / /  \ \ \ \ \ \  / / / / / / / / / / / / / / / / / / / / / / / / / / / /
 / / DISPLAY ABOUT / / / / / / / / / / / / / / / / / / / / / / / / / / / /
   \ \ \ \ \ \ \ \  / / / / / / / / / / / / / / / / / / / / / / / / / / / /
 / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / /
****************************************************************************/
void About( void )
{
    char    info[511];
    char    byte_txt[16];

        // prepare info block
    strcpy( info, NAME );
    strcat( info, " ");
    strcat( info, VERSION );
    strcat( info, "\n" );
    strcat( info, COPYRIGHT );
    strcat( info, " ");
    strcat( info, AUTHOR );
    strcat( info, "\n" );
    strcat( info, ABOUT_TEXT );

    if ( _norexx == FALSE ) {
        strcat( info, "\n-----\nARexx Port: " );
        strcat( info, PORT_NAME );
    }

        /* the condition is included simply for future versions of Epigrams
            where it may be possible to open up the about requester without
            an epigram window being open */
    if ( interface == TRUE )    {
        strcat( info, "\n-----\nData File Used - " );
        strcat( info, FilePart(file_used) );
        strcat( info, "\n Byte Pos - " );
        sprintf( byte_txt, "%ld", byte_pos );
        strcat( info, byte_txt );
    }

    strcat( info, "\n-----\nPRNG method - " );
    switch( _random_mode )  {
        case RAND_BASIC:
            strcat( info, "Basic" );
            break;
        case RAND_MODIFIER:
            strcat( info, "Basic + Modifier" );
            break;
        case RAND_MD5:
            strcat( info, "MD5 Hashing" );
            strcat( info, "\n-----\nPRNG code -- ©1997 Rich Skrenta\nMD5 -- ©1993 Ron Rivest");
            break;
        default:
            strcat( info, "Unknown" );
    }

    if ( (ReqToolsBase = (struct ReqToolsBase *) OpenLibrary (REQTOOLSNAME, REQTOOLSVERSION)) ) {

            // dont foget to alter embedded string and screen title
        rtEZRequestTags ( info,
                     "A Marvellous Program",
                     NULL,
                     NULL,
                     RTEZ_ReqTitle, (ULONG) "About " NAME,
                     RTEZ_Flags, EZREQF_CENTERTEXT,
                     RT_Window, EpigramsWnd,
                     RT_LockWindow, TRUE,
                     TAG_END );

        CloseLibrary ((struct Library *)ReqToolsBase);
    }
}


/*
void About( void )
{
    struct  Window *     AboutWnd = NULL;
    struct  IntuiMessage     *IMsg;
    ULONG   MsgClass, MsgID, MsgType;
    BOOL    waiting_for_close = TRUE;





        // SET UP SCREEN
    if ( ! SetupScreen )
        return;             // ---- SILENTLY

        // OPEN WINDOW
    AboutWnd = OpenWindowTags ( NULL,
                // WINDOW DIMENSIONS
                    WA_Left,            0,
                    WA_Top,             11,
                    WA_Width,           200,
                    WA_Height,          200,
                    WA_MinWidth,        200,
                    WA_MinHeight,       200,
                    WA_MaxWidth,        200,
                    WA_MaxHeight,       200,

                // WINDOW FLAGS
                    WA_SmartRefresh,    TRUE,
                    WA_Activate,        TRUE,
                    WA_RMBTrap,         TRUE,
                    WA_DragBar,         TRUE,
                    WA_CloseGadget,     TRUE,
                    WA_DepthGadget,     TRUE,

                // OTHER PROPERTIES
                    WA_Title,           "About Epigrams",
                    WA_ScreenTitle,     title_bar,
                    WA_PubScreen,       Scr,
                    WA_IDCMP,           IDCMP_MOUSEBUTTONS  |
                                        IDCMP_REFRESHWINDOW |
                                        IDCMP_ACTIVEWINDOW  |
                                        IDCMP_INACTIVEWINDOW|
                                        IDCMP_CLOSEWINDOW,
                    TAG_DONE );

        // DUMP SCREEN LOCK NOW -- WE DON'T NEED IT
    if ( Scr ) {
        UnlockPubScreen( NULL, Scr );
        Scr = NULL;
    }

        // PUT IMAGE IN WINDOW
    DrawImage( AboutWnd->RPort, &Logo, 10, 15 );

        // DO IDCMP CHECKING
    while ( waiting_for_close ) {
        while ( IMsg = GT_GetIMsg( AboutWnd->UserPort ) )    {

            MsgClass = IMsg->Class;
            GT_ReplyIMsg( IMsg );

            switch( MsgClass )  {
                case    IDCMP_REFRESHWINDOW:
                    GT_BeginRefresh( AboutWnd );
                    GT_EndRefresh( AboutWnd, TRUE );
                    break;

                case    IDCMP_ACTIVEWINDOW:
                    break;

                case    IDCMP_INACTIVEWINDOW:
                    break;

                case    IDCMP_CLOSEWINDOW:
                    waiting_for_close = FALSE;
                    break;

                default:
                    break;
            }
        }
    }

    CloseWindow( AboutWnd );
    AboutWnd = NULL;
}
*/
/***************************************************************************/
