/////////////////////////////
// main.cpp
//

// VS6 requires these macros to be defined.
#define WINVER          0x0410
#define _WIN32_WINDOWS  0x0410
#define _WIN32_IE       0x0401

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
    CMetaView m_metaView;
};


// Initialize the Win32++ framework.
CApp theApp;


// For VS2019
// int WINAPI WinMain (_In_ HINSTANCE, _In_opt_ HINSTANCE, _In_ LPSTR, _In_ int)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
    try
    {
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




