/****************************************************************************
 / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / /
  / /  \ \ \  / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / /
 / / INCLUDES  / / / / / / / / / / / / / / / / / / / / / / / / / / / / / /
   \ \ \ \ \ \  / / / / / / / / / / / / / / / / / / / / / / / / / / / / / /
 / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / /
****************************************************************************/
#include    "Public.h"
#include    "Backdrop.h"

/****************************************************************************
 / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / /
  / /  \ \ \ \  / / / / / / / / / / / / / / / / / / / / / / / / / / / / / /
 / / FUNCTIONS / / / / / / / / / / / / / / / / / / / / / / / / / / / / / /
   \ \ \ \ \ \  / / / / / / / / / / / / / / / / / / / / / / / / / / / / / /
 / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / /
****************************************************************************/
BOOL InitBackground( void )
{
    BOOL    return_val = TRUE;


        /* no harm in doing this as the `freebackground()' checks to
            see if `dt_object' already exists
            -- enables us to reload preferences safely. */
    CleanUpBackground();

        // check to see if background option is switched on
    if ( _background[0] != '\0' )   {

        dt_object = (Object *) NewDTObject( _background,
                        DTA_SourceType       ,DTST_FILE,
                        DTA_GroupID          ,GID_PICTURE,
                        PDTA_Remap           ,FALSE,
                        PDTA_Screen          ,Scr,
                        PDTA_FreeSourceBitMap,TRUE,
                        OBP_Precision        ,PRECISION_ICON,
                        TAG_DONE);

        if ( !dt_object )   {
            return_val = DoErrors( WARNING,
                      "I can't open specified background file",
                      "bad file name|datatype not recognised",
                      "Continue without background|Quit" );

            bg_bm = NULL;
        }
    }

    return( return_val );
}
/***************************************************************************/
void CleanUpBackground( void )
{
        // free memory associated with background
    if ( dt_object )
        DisposeDTObject( (Object *) dt_object );
}
/***************************************************************************/
void DrawBackground( void )
{
    struct  BitMap       *work;
    struct  BitMapHeader *header;
    UWORD   x,y,xs,ys;


    if ( ! dt_object )  {
        SetRast( EpigramsWnd->RPort, _bg_colour );

    } else if ( DoDTMethod( dt_object, NULL, NULL, DTM_PROCLAYOUT, NULL, 1 ) )    {

        GetDTAttrs( dt_object, PDTA_BitMapHeader, &header, TAG_DONE );

        if ( header )   {

            GetDTAttrs( dt_object, PDTA_BitMap, &work, TAG_DONE );

            if ( work )  {
                x = EpigramsWnd->LeftEdge;
                y = EpigramsWnd->TopEdge;
                xs = EpigramsWnd->Width;
                ys = EpigramsWnd->Height;
                if( bg_bm = (struct BitMap *)AllocBitMap(xs,ys,work->Depth,0,0) )   {
                    TileBitMap(work,header->bmh_Width,header->bmh_Height,xs,ys,bg_bm);
                    BltBitMapRastPort(bg_bm, 0, 0, EpigramsWnd->RPort, 0, 0, EpigramsWnd->Width + 1, EpigramsWnd->Width + 1, 0xc0);
                } else  {
                    system_state = DoErrors( WARNING,
                        "Couldn't provide a bitmap for the background picture",
                        "low memory?",
                        "Continue without background|Quit" );
                }
            } else
                system_state = DoErrors( WARNING,
                    "I didn't understand the bitmap in the background file",
                    "corrupt file,unknown version of picture format,memory corrupted since program started",
                    "Continue without background|Quit" );
        } else
            system_state = DoErrors( WARNING,
                "I didn't understand the bitmap header in the background file",
                "corrupt file,unknown version of picture format,memory corrupted since program started",
                "Continue without background|Quit" );

    } else {
        system_state = DoErrors( WARNING,
            "I tried to perform a `DoDTMethod' function but couldn't",
            "I don't know!",
            "Continue without background|Quit" );

        bg_bm = NULL;
    }

    RefreshWindowFrame( EpigramsWnd );
}
/***************************************************************************/
void TileBitMap(struct BitMap *Src, WORD SrcSizeX, WORD SrcSizeY, WORD DstSizeX, WORD DstSizeY , struct BitMap *Dst)
{
    WORD PosX, PosY;
    WORD SizeX, SizeY;


    for ( PosX = 0, SizeX = MIN(SrcSizeX,DstSizeX); PosX < DstSizeX; PosX += MIN( SizeX,DstSizeX-PosX ), SizeX = MIN( SizeX,DstSizeX-PosX ) )
        for ( PosY = 0, SizeY = MIN(SrcSizeY,DstSizeY); PosY < DstSizeY; PosY += MIN( SizeY, DstSizeY - PosY ), SizeY = MIN( SizeY, DstSizeY - PosY ) )
            BltBitMap( Src, 0, 0, Dst, PosX, PosY, SizeX, SizeY , 0xC0, 0xff, NULL );
}

/***************************************************************************/
