/****************************************************************************
 / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / /
  / /  \ \ \  / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / /
 / / INCLUDES  / / / / / / / / / / / / / / / / / / / / / / / / / / / / / /
   \ \ \ \ \ \  / / / / / / / / / / / / / / / / / / / / / / / / / / / / / /
 / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / /
****************************************************************************/
#include    "Public.h"
#include    "Commodity.h"

/****************************************************************************
 / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / /
  / /  \ \ \ \  / / / / / / / / / / / / / / / / / / / / / / / / / / / / / /
 / / FUNCTIONS / / / / / / / / / / / / / / / / / / / / / / / / / / / / / /
   \ \ \ \ \ \  / / / / / / / / / / / / / / / / / / / / / / / / / / / / / /
 / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / /
****************************************************************************/
BOOL InitCommodity()
{
    LONG    error;
    BOOL    cont = TRUE;

    CxObj   *Filter,
            *Sender,
            *Translate;


        // attempt to create message port
    if ( ! (BrokerMP = CreateMsgPort()) )   {
        DoErrors( FATAL,
                    "Error creating `Exec' message port",
                    "low memory?|exec.library out of date?",
                    "Quit" );

        return( FALSE );
    }

        // check to see if cxpriority is inside valid range; set to 0 if not
    if (_cxpriority < -128 || _cxpriority > 127)
        _cxpriority = 0;

        // change presets in `NewBroker' structure
    NewBroker.nb_Descr = DESC;
    NewBroker.nb_Pri = _cxpriority;
    NewBroker.nb_Port = BrokerMP;

        // set up new broker
    Broker = CxBroker( &NewBroker, &error );

        // handle any errors caused by `CxBroker'
    switch( error )     {
        case    CBERR_OK:
            break;

        case    CBERR_DUP:
            cont = FALSE;
            break;

        case    CBERR_SYSERR:
            DoErrors( FATAL,
                        "Error creating commodity",
                        "probably low memory",
                        "Quit" );

            cont = FALSE;
            break;

        case    CBERR_VERSION:
            DoErrors( FATAL,
                        "Error creating commodity",
                        "problem in source code, slap nearest coder",
                        "Quit" );

            cont = FALSE;
            break;
    }

    if ( cont == FALSE )
        return( FALSE );


        // set up and attach new filter object
    Filter = CxFilter( _opthotkey );
    if ( CxObjError( Filter ) == COERR_BADFILTER ) {
        BOOL    returnerror;

        returnerror = DoErrors( WARNING,
            "The HOTKEY definition didn't make any sense",
            "use default POPKEY (alt space)|Quit and alter POPKEY tooltype",
            "Continue with default|Quit" );
        ClearCxObjError( Filter );

        if ( returnerror == TRUE ) {
            SetFilter( Filter, DEF_HOTKEY );
            if ( CxObjError( Filter ) == COERR_BADFILTER ) {
                DoErrors ( WARNING,
                    "The default POPKEY definition didn't make any sense",
                    "bad coding?|your version of commodities.library is outdated?",
                    "Quit" );
                return( FALSE );
            }
        } else
            return( FALSE );
    }

        // set up and attach new signal object
    Sender = CxSender( BrokerMP, EVT_HOTKEY );
    AttachCxObj( Filter, Sender );

        // set up and attach new translator
    Translate = CxTranslate( NULL );
    AttachCxObj( Filter, Translate );

    AttachCxObj( Broker, Filter );
    ActivateCxObj( Broker, 1 );

    if ( CxObjError( Filter ) ) {
        DoErrors( FATAL,
                    "Error creating commodity",
                    "low memory?|bad coding?",
                    "Quit" );
        ClearCxObjError( Filter );
        return( FALSE );
    }

    return( TRUE );
}
/***************************************************************************/
void CleanUpCommodity()
{
    if ( Broker != NULL )
        DeleteCxObjAll(Broker);

    if ( BrokerMP != NULL )   {
        while ( (BrokerMsg = (CxMsg *)GetMsg( BrokerMP )) )
            ReplyMsg( (struct Message *)BrokerMsg );

        DeleteMsgPort( BrokerMP );
    }
}
/***************************************************************************/
void HandleCommodityMsgs( BOOL * quit_prog )
{
    ULONG       MsgID, MsgType;


    while ( ( BrokerMsg = (CxMsg *)GetMsg(BrokerMP) ) )     {

        MsgID = CxMsgID( BrokerMsg );
        MsgType = CxMsgType( BrokerMsg );

        ReplyMsg( (struct Message *)BrokerMsg );

        switch ( MsgType )    {

            case CXM_IEVENT:

                switch ( MsgID )
                {
                    case EVT_HOTKEY:
                        MainGUISequence();
                        break;

                    default:
                        break;
                }
                break;

            case CXM_COMMAND:   // Commodities has sent a command

                switch ( MsgID )  {

                    case CXCMD_DISABLE:
                        interface = FALSE;
                        ActivateCxObj( Broker, FALSE );
                        break;

                    case CXCMD_ENABLE:
                        ActivateCxObj( Broker, TRUE );
                        break;

                        // unique signal recieved, pop up window
                    case CXCMD_UNIQUE:

                            // switch depending on prefs.
                        switch (_relaunch)  {

                            default:

                            case RELAUNCH_POP:
                                MainGUISequence();
                                break;

                            case RELAUNCH_RELOAD:
                            case RELAUNCH_QUIT:
                                * quit_prog = TRUE;
                                break;
                        }

                        break;

                    case CXCMD_KILL:
                        * quit_prog = TRUE;
                        break;

                    case CXCMD_APPEAR:
                        break;

                    case CXCMD_DISAPPEAR:
                        break;

                    default:
                        break;
                }

                break;

        default:
            break;
        }
    }
}
/***************************************************************************/
