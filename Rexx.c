/****************************************************************************
 / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / /
  / /  \ \ \  / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / /
 / / INCLUDES  / / / / / / / / / / / / / / / / / / / / / / / / / / / / / /
   \ \ \ \ \ \  / / / / / / / / / / / / / / / / / / / / / / / / / / / / / /
 / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / /
****************************************************************************/
#include    "Public.h"
#include    "Rexx.h"

/****************************************************************************
 / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / /
  / /  \ \ \ \  / / / / / / / / / / / / / / / / / / / / / / / / / / / / / /
 / / FUNCTIONS / / / / / / / / / / / / / / / / / / / / / / / / / / / / / /
   \ \ \ \ \ \  / / / / / / / / / / / / / / / / / / / / / / / / / / / / / /
 / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / /
****************************************************************************/
BOOL InitRexxPort()
{
    BOOL    return_val = TRUE;
    struct  MsgPort *test_port;


    if ( _norexx == TRUE )
        return( TRUE );                 // go away because we don't need this shit

    EpigramsRxPort = CreateMsgPort();
    if ( ! EpigramsRxPort ) {
        EpigramsRxPort = NULL;

        DoErrors( WARNING,
            "I can't prepare Rexx Port",
            "bad name specified (slap coder)|REXXMast not loaded?",
            "Continue without Rexx Port|Quit" );

        return( FALSE );
    }

    EpigramsRxPort->mp_Node.ln_Name = PORT_NAME;
    EpigramsRxPort->mp_Node.ln_Pri  = 0;
    AddPort( EpigramsRxPort );

    return( TRUE );
}
/***************************************************************************/
void CleanUpRexxPort()
{
    if ( EpigramsRxPort ) {
        struct RexxMsg *msg;

        Forbid();
        RemPort( EpigramsRxPort );

        while ((msg = (struct RexxMsg *)GetMsg(EpigramsRxPort)) != NULL) {
            msg->rm_Result1 = RC_FATAL;
            msg->rm_Result2 = NULL;
            ReplyMsg(msg);
        }

        Permit();
        DeleteMsgPort(EpigramsRxPort);
        EpigramsRxPort = NULL;
    }
}
/***************************************************************************/
void HandleRexxMsgs( BOOL * quit_prog )
{
    struct  RexxMsg * msg;


    while ( (msg = (struct RexxMsg *)GetMsg(EpigramsRxPort)) != NULL ) {
        msg->rm_Result1 = RC_OK;
        msg->rm_Result2 = NULL;

        if ( (msg->rm_Action & RXCODEMASK) == RXCOMM )  {

                // CAN'T SWITCH FOR STRINGS :(
            if ( strcmp( ARG0(msg), "POPUP" ) == 0 )
                MainGUISequence();

            else if ( strcmp( ARG0(msg), "POPOFF" ) == 0 )
                interface = FALSE;

            else if ( strcmp( ARG0(msg), "QUIT" ) == 0 )
                * quit_prog = TRUE;
        }
        ReplyMsg( msg );
    }
}
/***************************************************************************/
