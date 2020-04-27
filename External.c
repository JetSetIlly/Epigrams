/****************************************************************************
 / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / /
  / /  \ \ \  / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / /
 / / INCLUDES  / / / / / / / / / / / / / / / / / / / / / / / / / / / / / /
   \ \ \ \ \ \  / / / / / / / / / / / / / / / / / / / / / / / / / / / / / /
 / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / /
****************************************************************************/
#include    "Public.h"
#include    "External.h"

/****************************************************************************
 / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / /
  / /  \ \ \  / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / /
 / / FUNCTIONS / / / / / / / / / / / / / / / / / / / / / / / / / / / / / /
   \ \ \ \ \ \  / / / / / / / / / / / / / / / / / / / / / / / / / / / / / /
 / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / /
****************************************************************************/
BOOL ExternalPercent()
{
    ULONG   randval;


    randval = rand() % 100;

    if ( randval < _extp )
        return( TRUE );
    else
        return( FALSE );
}
/***************************************************************************/
void HandleExternal()
{
    BPTR        outhandle;

    if ( outhandle = Open( _externalout, MODE_NEWFILE ))    {
        FPuts( outhandle, "%%\n" );
        Seek( outhandle, 0, OFFSET_END );

        if ( Execute( _external, NULL, outhandle ) == FALSE ) {
            Close( outhandle );
            system_state = DoErrors( WARNING,
                "Can't locate\load external program",
                "restart `epigrams' with corrected tool type",
                "Continue|Quit" );
        } else  {
            Close( outhandle );
//            ReadDataFile( _externalout );
        }
        DeleteFile( _externalout );
    } else
        system_state = DoErrors ( FATAL,
            "Error creating temporary file for external program",
            "coders fault!|don't use `external' option until next version|restart with normal data file",
            "Quit" );
}
/***************************************************************************/
