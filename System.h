
// ----- functions, structures etc. ------
#include <exec/types.h>
#include <exec/lists.h>
#include <exec/ports.h>
#include <exec/libraries.h>
#include <exec/memory.h>
#include <exec/semaphores.h>
#include <exec/resident.h>
#include <exec/tasks.h>
#include <exec/execbase.h>

#include <dos/dos.h>

#include <intuition/intuition.h>
#include <intuition/intuitionbase.h>
#include <intuition/classes.h>
#include <intuition/classusr.h>
#include <intuition/imageclass.h>
#include <intuition/gadgetclass.h>

#include <libraries/diskfont.h>
#include <libraries/gadtools.h>
#include <libraries/commodities.h>
#include <libraries/reqtools.h>

#include <datatypes/datatypesclass.h>
#include <datatypes/datatypes.h>
#include <datatypes/pictureclass.h>

#include <graphics/displayinfo.h>
#include <graphics/gfxbase.h>

#include <rexx/errors.h>
#include <rexx/storage.h>

#include <workbench/icon.h>
#include <workbench/startup.h>
#include <workbench/workbench.h>

// ----- prototypes -----
#include <clib/macros.h>
#include <clib/alib_protos.h>
#include <clib/alib_stdio_protos.h>

#include <proto/exec.h>
#include <proto/dos.h>
#include <proto/icon.h>
#include <proto/intuition.h>
#include <proto/diskfont.h>
#include <proto/datatypes.h>
#include <proto/gadtools.h>
#include <proto/graphics.h>
#include <proto/commodities.h>
#include <proto/icon.h>
#include <proto/utility.h>
#include <proto/reqtools.h>


// ----- ANSI stuff -----
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>
