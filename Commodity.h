
// structures for Comm.c

struct  NewBroker   NewBroker =     {
    NB_VERSION,                             // nb_Version - Version of the NewBroker structure -- AmigaDOS internal
    NAME,                                   // nb_Name - Name Commodities uses to identify this commodity
    title_bar,                              // nb_Title - Title of commodity that appears in CX_Exchange
    DESC,                                   // nb_Descr - Description of the commodity
    NBU_UNIQUE | NBU_NOTIFY,                // nb_Unique - Tells CX not to launch another commodity with same name
    0,                                      // nb_Flags - Tells CX if this commodity has a window
    0,                                      // nb_Pri - This commodity's priority
    0,                                      // nb_Port - MsgPort CX talks to
    0                                       // nb_ReservedChannel - reserved for later use
};
