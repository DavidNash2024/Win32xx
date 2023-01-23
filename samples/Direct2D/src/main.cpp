/////////////////////////////
// main.cpp
//

#include "stdafx.h"
#include "D2DApp.h"


#if defined (_MSC_VER) && (_MSC_VER >= 1920) // >= VS2019
  int WINAPI WinMain (__in HINSTANCE, __in_opt HINSTANCE, __in LPSTR, __in int)
#else
  int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
#endif
{
    CD2DApp app;
    app.Run();

    return 0;
}