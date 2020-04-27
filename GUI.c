/****************************************************************************
 / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / /
  / /  \ \ \  / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / /
 / / INCLUDES  / / / / / / / / / / / / / / / / / / / / / / / / / / / / / /
   \ \ \ \ \ \  / / / / / / / / / / / / / / / / / / / / / / / / / / / / / /
 / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / /
****************************************************************************/
#include    "Public.h"
#include    "GUI.h"

/****************************************************************************
 / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / /
  / /  \ \ \ \  / / / / / / / / / / / / / / / / / / / / / / / / / / / / / /
 / / FUNCTIONS / / / / / / / / / / / / / / / / / / / / / / / / / / / / / /
   \ \ \ \ \ \  / / / / / / / / / / / / / / / / / / / / / / / / / / / / / /
 / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / /
****************************************************************************/
BOOL PrepareGUIEnv( void )
{
    BOOL    success = TRUE;


    if ( OpenEpigramsWindow() == FALSE )    {
        GUIGoAway( NOWINDOW );
        success = FALSE;
    }

    DrawBackground();
    DrawBevelBoxes();

    interface = TRUE;

    return ( TRUE );
}
/***************************************************************************/
void GUIGoAway( int error )
{
    switch (error)  {
        case NOSCREEN : system_state = DoErrors ( WARNING,
                            "error opening screen",
                            "low memory?",
                            "Continue|Quit" );
                        break;
        case NOWINDOW : system_state = DoErrors ( WARNING,
                            "error opening window",
                            "low memory?",
                            "Continue|Quit" );
                    break;
        default:    break;
    }

        // VERY IMPORTANT! free bitmap
    if ( bg_bm )    {
        WaitBlit();
        FreeBitMap( bg_bm );
        bg_bm = NULL;
    }

    CloseEpigramsWindow();
    CloseDownScreen();

    interface = FALSE;

        // pop back to screen
    if ( _popback == TRUE )
        ScreenDepth( ActiveScr, SDEPTH_TOFRONT, NULL );
}
/***************************************************************************/
void DrawBevelBoxes( void )
{
    if ( (_bevel == BEV_SYSTEM) || (_bevel == BEV_NONE) )
        return;

    if ( _bevel == BEV_NORMAL )  {
        DrawBevelBox( EpigramsWnd->RPort, 0, 0, EpigramsWidth, EpigramsHeight, GT_VisualInfo, VisualInfo, TAG_DONE );
        DrawBevelBox( EpigramsWnd->RPort, 3, 1, EpigramsWidth - 6 , EpigramsHeight - 2, GT_VisualInfo, VisualInfo, GTBB_Recessed, TAG_DONE );
    }

    if ( _bevel == BEV_COMPOUND )    {
        DrawBevelBox( EpigramsWnd->RPort, 0, 0, EpigramsWidth, EpigramsHeight, GT_VisualInfo, VisualInfo, TAG_DONE );
        DrawBevelBox( EpigramsWnd->RPort, 3, 1, EpigramsWidth - 6 , EpigramsHeight - 2, GT_VisualInfo, VisualInfo, GTBB_Recessed, TAG_DONE );
        DrawBevelBox( EpigramsWnd->RPort, 4, 2, EpigramsWidth - 8, EpigramsHeight - 4, GT_VisualInfo, VisualInfo, TAG_DONE );
    }

    if ( _bevel == BEV_RAISED )
        DrawBevelBox( EpigramsWnd->RPort, 0, 0, EpigramsWidth, EpigramsHeight, GT_VisualInfo, VisualInfo, TAG_DONE );

    if ( _bevel == BEV_RECESSED )
        DrawBevelBox( EpigramsWnd->RPort, 0, 0, EpigramsWidth, EpigramsHeight, GT_VisualInfo, VisualInfo, GTBB_Recessed, TAG_DONE );

    if ( _bevel == BEV_DOUBLE )  {
        DrawBevelBox( EpigramsWnd->RPort, 0, 0, EpigramsWidth, EpigramsHeight, GT_VisualInfo, VisualInfo, GTBB_Recessed, TAG_DONE );
        DrawBevelBox( EpigramsWnd->RPort, 2, 1, EpigramsWidth - 4 , EpigramsHeight - 2, GT_VisualInfo, VisualInfo, TAG_DONE );
    }
}
/***************************************************************************/
BOOL SetupScreen( void )
{
    if ( ! ( Scr = LockPubScreen( PubScreenName )))
        return( FALSE );

    if ( ! ( VisualInfo = GetVisualInfo( Scr, TAG_DONE )))
        return( FALSE );

    return( TRUE );
}
/***************************************************************************/
void CloseDownScreen( void )
{
    if ( VisualInfo ) {
        FreeVisualInfo( VisualInfo );
        VisualInfo = NULL;
    }
}
/***************************************************************************/
BOOL OpenEpigramsWindow( void )
{
    if ( _bevel == BEV_SYSTEM )  {
        EpigramsWnd = OpenWindowTags( NULL,
                // WINDOW DIMENSIONS
                    WA_Left,            EpigramsLeft,
                    WA_Top,             EpigramsTop,
                    WA_Width,           EpigramsWidth,
                    WA_Height,          EpigramsHeight,
                    WA_MinWidth,        EpigramsWidth,
                    WA_MinHeight,       EpigramsHeight,
                    WA_MaxWidth,        EpigramsWidth,
                    WA_MaxHeight,       EpigramsHeight,

                // WINDOW FLAGS
                    WA_SmartRefresh,    TRUE,
                    WA_Activate,        TRUE,
                    WA_RMBTrap,         TRUE,

                // OTHER PROPERTIES
                    WA_Title,           EpigramsWdt,
                    WA_ScreenTitle,     title_bar,
                    WA_PubScreen,       Scr,
                    WA_IDCMP,           IDCMP_MOUSEBUTTONS  |
                                        IDCMP_RAWKEY        |
                                        IDCMP_REFRESHWINDOW |
                                        IDCMP_ACTIVEWINDOW  |
                                        IDCMP_INACTIVEWINDOW,
                    TAG_DONE );
    } else  {
        EpigramsWnd = OpenWindowTags( NULL,
                // WINDOW DIMENSIONS
                    WA_Left,            EpigramsLeft,
                    WA_Top,             EpigramsTop,
                    WA_Width,           EpigramsWidth,
                    WA_Height,          EpigramsHeight,
                    WA_MinWidth,        EpigramsWidth,
                    WA_MinHeight,       EpigramsHeight,
                    WA_MaxWidth,        EpigramsWidth,
                    WA_MaxHeight,       EpigramsHeight,

                // WINDOW FLAGS
                    WA_SmartRefresh,    TRUE,
                    WA_Activate,        TRUE,
                    WA_RMBTrap,         TRUE,
                    WA_Borderless,      TRUE,

                // OTHER PROPERTIES
                    WA_Title,           EpigramsWdt,
                    WA_ScreenTitle,     title_bar,
                    WA_PubScreen,       Scr,
                    WA_IDCMP,           IDCMP_MOUSEBUTTONS  |
                                        IDCMP_RAWKEY        |
                                        IDCMP_REFRESHWINDOW |
                                        IDCMP_ACTIVEWINDOW  |
                                        IDCMP_INACTIVEWINDOW,
                    TAG_DONE );
    }

        // dump screen lock now -- we don't need it
    if ( Scr ) {
        UnlockPubScreen( NULL, Scr );
        Scr = NULL;
    }

    if ( ! EpigramsWnd )
        return( FALSE );
    else
        return( TRUE );
}
/***************************************************************************/
void CloseEpigramsWindow( void )
{
    if ( EpigramsWnd ) {
        CloseWindow( EpigramsWnd );
        EpigramsWnd = NULL;
    }
}
/***************************************************************************/
void HandleGUIMsgs()
{
    struct  IntuiMessage     *IMsg;
    ULONG   MsgClass, MsgCode, seconds, micros;


        // DO FOR ALL MESSAGES WAITING AT PORT
    while ( IMsg = GT_GetIMsg( EpigramsWnd->UserPort ) )
    {

            // STORE ALL RELEVANT INFO FROM MESSAGE
        MsgClass = IMsg->Class;
        MsgCode = IMsg->Code;
        seconds = IMsg->Seconds;
        micros = IMsg->Micros;

            // DISPOSE MESSAGE
        GT_ReplyIMsg( IMsg );

            // APPROPRIATE ACTION
        switch( MsgClass )
        {
            case    IDCMP_REFRESHWINDOW:
                GT_BeginRefresh( EpigramsWnd );
                GT_EndRefresh( EpigramsWnd, TRUE );
                break;

            case    IDCMP_RAWKEY:
                switch( MsgCode )
                {
                        // escape key
                    case    ESCAPE_KEY:
                        if (_noescape == FALSE) {
                            interface = FALSE;
                        }
                        break;

                    case    RETURN_KEY:
                        MainGUISequence();
                        break;

                    case    HELP_KEY:
                        About();
                        break;

                    case    F1_KEY:
                        SaveFavourite();
                        break;
                }
                break;

            case    IDCMP_MOUSEBUTTONS:
                switch ( MsgCode )
                {
                    case SELECTDOWN :
                            // CHECK FOR VALID DOUBLE CLICK
                        if ( DoubleClick(lastsecs, lastmicros, seconds, micros) == TRUE )   {
                            interface = FALSE;
                        }
                        else
                        {
                                // STORE TIME OF FIRST CLICK
                            lastsecs = seconds;
                            lastmicros = micros;
                        }
                        break;

                    case MENUDOWN:
                        MainGUISequence();
                }

                break;

            case    IDCMP_ACTIVEWINDOW:
                active = TRUE;
                break;

            case    IDCMP_INACTIVEWINDOW:
                active = FALSE;
                FlushDoubleClick();
                break;

            default:
                break;
        }
    }
}
/***************************************************************************/
