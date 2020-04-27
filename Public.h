
// main header file included by all other files that need it

/****************************************************************************
 / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / /
  / /  \ \ \ \  / / / / / / / / / / / / / / / / / / / / / / / / / / / / / /
 / / INCLUDES  / / / / / / / / / / / / / / / / / / / / / / / / / / / / / /
   \ \ \ \ \ \  / / / / / / / / / / / / / / / / / / / / / / / / / / / / / /
 / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / /
****************************************************************************/
#include    "System.c"
#include    "Public_Prototypes.h"
#include    "Version.h"

/****************************************************************************
 / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / /
  / /  \ \ \ \ \ \ \ \ \ \ \ \ \ \  / / / / / / / / / / / / / / / / / / / /
 / / "COMPILER DEPENDENCY DECIDER" / / / / / / / / / / / / / / / / / / / /
   \ \ \ \ \ \ \ \ \ \ \ \ \ \ \ \  / / / / / / / / / / / / / / / / / / / /
 / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / /
****************************************************************************/
/* The preprocessing that appears below was `ripped' from SnoopDOS source
    code. It allows us to use `global' variables in multiple source files
    by defining GLOBAL depending on whether the source file being compiled
    is the MAIN file one of the `support-files'. (eg/ Comm.c)    */

#undef GLOBAL                   // undefine GLOBAL to be sure
#undef INIT                     // ditto INIT

#ifdef ROOT_FILE                // if MAIN is a valid #define. -- ie/ does it appear in the calling file? (should only happen from `Epigrams.c')
#  define GLOBAL                // define GLOBAL so it equals `nothing'. -- ie/ define the varibale
#  define INIT(x)   = x         //
#else
#  define GLOBAL    extern      // re-define GLOBAL to extern -- ie/ declare the variable
#  define INIT(x)
#endif

/****************************************************************************
 / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / /
  / /  \ \ \ \ \ \ \ \  / / / / / / / / / / / / / / / / / / / / / / / / / /
 / / PREFERENCE DEFINES  / / / / / / / / / / / / / / / / / / / / / / / / /
   \ \ \ \ \ \ \ \ \ \ \  / / / / / / / / / / / / / / / / / / / / / / / / /
 / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / /
****************************************************************************/
    // ----- define for rexx port
#define         PORT_NAME                       "epigrams"

    // ----- defines for key presses
#define         HELP_KEY                        0x5F    // Keyboard raw code for HELP key
#define         ESCAPE_KEY                      0x45    // Keyboard raw code for ESCAPE key
#define         RETURN_KEY                      0x44    // Keyboard raw code for RETURN key
#define         F1_KEY                          0x50    // Keyboard raw code for F1
                                                        //   NB. these codes are very near
                                                        // the bone of the keyboard.device

    // ------ commodity preferences
#define         DEF_HOTKEY                      "-caps alt space"
#define         EVT_HOTKEY                      1L

    // ----- data file and external preferences
#define         DEF_DATA_FILE                   "S:Epigrams.dat"
#define         DEF_EXTERNAL                    ""
#define         DEF_EXTP                        100

#define         DEF_FAVE_FILE                   "S:Epigrams_Favourites.dat"

    // ----- font preferences
#define         DEF_FONT                        "topaz.font"
#define         DEF_FONT_SIZE                   8

    // ----- default background preferences
#define         DEF_BACKGROUND                  ""
#define         DEF_BG_COLOUR                   0

    // ----- possible beveling options
enum  BEVELS {
    BEV_SYSTEM, BEV_NORMAL, BEV_COMPOUND, BEV_RAISED,
    BEV_RECESSED, BEV_DOUBLE, BEV_NONE
};

    // ----- text options
#define         DEF_PEN                         1
#define         DEF_PAPER                       0
#define         DEF_DRAWMODE                    JAM1
#define         DEF_XPAD                        15
#define         DEF_YTOP                        8
#define         DEF_YBOT                        8
#define         DEF_LSPACE                      1
#define         DEF_TAB                         4
#define         DEF_MAXLINESIZE                 80

    // ----- possible startup options
enum            STARTUP {
                    STARTUP_NEVER,
                    STARTUP_ALWAYS,
                    STARTUP_ONCEADAY
                };
#define         ONCEADAY_DATEDATA_NAME          "Epigrams.onceaday"

    // ----- possible relaunch options
enum            RELAUNCH {
                    RELAUNCH_POP,
                    RELAUNCH_QUIT,
                    RELAUNCH_RELOAD
                };

    // ----- flags for different random modes found in PRNG
#define         RAND_BASIC                      0
#define         RAND_MODIFIER                   1
#define         RAND_MD5                        2
#define         MD5_SEED_FILE                   "S:Epigrams.md5.digest"

    // ----- test file name
#define         TEST_FILE_NAME                  "RAM:Epigrams.test"

    // ----- `private' preferences
#define         MAX_FN_SIZE                     255
#define         MAX_EXTERNAL_NAME               255
#define         MAX_FONT_NAME_SIZE              255
#define         MAX_BG_NAME_SIZE                255
#define         MAX_MAXLINESIZE                 255
#define         MAXNUMLINES                     32
#define         MARKER_CH                       '%'
#define         COOKIE_DELIMITER                "%%\n"
#define         CARRIAGE_RETURN                 10

#define         DEF_CX_PRIORITY                 0

/****************************************************************************
 / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / /
  / /  \ \ \ \ \ \ \  / / / / / / / / / / / / / / / / / / / / / / / / / / /
 / / GLOBAL VARIABLES  / / / / / / / / / / / / / / / / / / / / / / / / / /
   \ \ \ \ \ \ \ \ \ \  / / / / / / / / / / / / / / / / / / / / / / / / / /
 / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / /
****************************************************************************/
// --------------------------------------
// ----- default tool types initialization

GLOBAL  char    _opthotkey[64]                  INIT(DEF_HOTKEY);
GLOBAL  LONG    _cxpriority                     INIT(DEF_CX_PRIORITY);

GLOBAL  BOOL    _uselibrary                     INIT(TRUE);

GLOBAL  char    _dfilename[MAX_FN_SIZE]         INIT(DEF_DATA_FILE);
GLOBAL  char    _favourites_file[MAX_FN_SIZE]   INIT(DEF_FAVE_FILE);

GLOBAL  char    _external[MAX_EXTERNAL_NAME]    INIT(DEF_EXTERNAL);
GLOBAL  BOOL    _externalquery                  INIT(FALSE);
GLOBAL  char    _externalout[]                  INIT("T:epigrams.external");
GLOBAL  ULONG   _extp                           INIT(DEF_EXTP);

GLOBAL  BOOL    _usescreenfont                  INIT(TRUE);
GLOBAL  char    _fontname[MAX_FONT_NAME_SIZE]   INIT(DEF_FONT);
GLOBAL  int     _fontsize                       INIT(DEF_FONT_SIZE);

GLOBAL  char    _background[MAX_BG_NAME_SIZE]   INIT(DEF_BACKGROUND);
GLOBAL  UBYTE   _bg_colour                      INIT(DEF_BG_COLOUR);
GLOBAL  BYTE    _bevel                          INIT(BEV_SYSTEM);

GLOBAL  int     _pen                            INIT(DEF_PEN);
GLOBAL  int     _paper                          INIT(DEF_PAPER);
GLOBAL  int     _draw_mode                      INIT(DEF_DRAWMODE);
GLOBAL  int     _xpad                           INIT(DEF_XPAD);
GLOBAL  int     _ytop                           INIT(DEF_YTOP);
GLOBAL  int     _ybot                           INIT(DEF_YBOT);
GLOBAL  int     _lspace                         INIT(DEF_LSPACE);
GLOBAL  int     _tabsize                        INIT(DEF_TAB);
GLOBAL  int     _maxlinesize                    INIT(DEF_MAXLINESIZE);

GLOBAL  int     _startup                        INIT(STARTUP_NEVER);
GLOBAL  BOOL    _poptofront                     INIT(FALSE);
GLOBAL  BOOL    _popback                        INIT(FALSE);
GLOBAL  int     _relaunch                       INIT(RELAUNCH_POP);

GLOBAL  ULONG   _random_mode                    INIT(RAND_BASIC);
GLOBAL  char    _md5digestname[MAX_FN_SIZE]     INIT(MD5_SEED_FILE);
GLOBAL  BOOL    _norexx                         INIT(FALSE);
GLOBAL  BOOL    _noescape                       INIT(FALSE);
GLOBAL  BOOL    _test                           INIT(FALSE);


// --------------------------------------
// ----- other globals

    // ----- keep track of whether window is open etc.
GLOBAL  BOOL                system_state        INIT(TRUE);
GLOBAL  BOOL                interface           INIT(FALSE);
GLOBAL  BOOL                active              INIT(FALSE);

    // ----- double-click information
GLOBAL  ULONG               lastsecs, lastmicros;

    // ----- structures etc. for fonts and bitmaps
GLOBAL  struct TextAttr     Font_Attr;
GLOBAL  struct BitMap *     bg_bm;
GLOBAL  Object *            dt_object           INIT(NULL);

    // ----- structures etc. for commodities etc.
GLOBAL  struct MsgPort *    BrokerMP            INIT(NULL);
GLOBAL  CxMsg  *            BrokerMsg           INIT(NULL);
GLOBAL  CxObj  *            Broker              INIT(NULL);

    // ----- rexx message port
GLOBAL  struct MsgPort *    EpigramsRxPort;

    // ----- structures etc. for screens, windows etc.
GLOBAL  struct Screen *     Scr                 INIT(NULL);
GLOBAL  struct Screen *     ActiveScr           INIT(NULL);
GLOBAL  UBYTE *             PubScreenName       INIT(NULL);
GLOBAL  APTR                VisualInfo          INIT(NULL);
GLOBAL  struct Window *     EpigramsWnd         INIT(NULL);
GLOBAL  struct IntuiMessage EpigramsMsg;
GLOBAL  UWORD               EpigramsLeft        INIT(104);
GLOBAL  UWORD               EpigramsTop         INIT(85);
GLOBAL  UWORD               EpigramsWidth       INIT(433);
GLOBAL  UWORD               EpigramsHeight      INIT(103);
GLOBAL  UBYTE *             EpigramsWdt         INIT(NULL);

    // ----- used by screen title and commodities title
GLOBAL  UBYTE               title_bar[64];

    // ----- stores the file that has been randomly selected
GLOBAL  char                file_used[MAX_FN_SIZE];
GLOBAL  ULONG               byte_pos            INIT(0);
