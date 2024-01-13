//---------------------------------------------------------------------------
#ifndef pollserversH
#define pollserversH
//---------------------------------------------------------------------------
#endif

typedef void TPollCallBack(char * s);

int PollServer(char * ip, TPollCallBack proc);


