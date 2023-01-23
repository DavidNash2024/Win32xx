/////////////////////////////
// main.cpp
//

// VS6 requires these macros to be defined.
#if defined (_MSC_VER) && (_MSC_VER <= 1200)
  #define WINVER          0x0410
  #define _WIN32_WINDOWS  0x0410
  #define _WIN32_IE       0x0401
#endif

#include "wxx_wincore.h"
#include "MetaView.h"


////////////////////////////////////////////////////////////////
// CApp manages the application. It initializes the Win32++
// framework when it is constructed, and creates the main frame
// window when it runs.
class CApp : public CWinApp
{
public:
    CApp() {};
    virtual ~CApp() {}

protected:
    virtual BOOL InitInstance()
    {
        m_metaView.Create();    // throws a CWinException on failure
        return TRUE;
    }

private:
    CApp(const CApp&);                // Disable copy construction
    CApp& operator = (const CApp&);   // Disable assignment operator

    CMetaView m_metaView;
};


#if defined (_MSC_VER) && (_MSC_VER >= 1920) // >= VS2019
  int WINAPI WinMain (__in HINSTANCE, __in_opt HINSTANCE, __in LPSTR, __in int)
#else
  int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
#endif
{
    try
    {
        // Initialize the Win32++ framework.
        CApp theApp;

        // Run the application's message loop.
        return theApp.Run();
    }

    // Catch all unhandled CException types.
    catch (const CException &e)
    {
        // Display the exception and quit.
        ::MessageBox(0, e.GetText(), AtoT(e.what()), MB_ICONERROR);

        return -1;
    }
}




