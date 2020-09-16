/////////////////////////////
// main.cpp
//

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
    virtual BOOL InitInstance()
    {
        m_metaView.Create();    // throws a CWinException on failure
        return TRUE;
    }

private:
    CMetaView m_metaView;
};

// For VS2019
// int WINAPI WinMain (_In_ HINSTANCE, _In_opt_ HINSTANCE, _In_ LPSTR, _In_ int)
int APIENTRY WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
    try
    {
        CApp theApp;

        return theApp.Run();
    }

    // catch all unhandled CException types
    catch (const CException &e)
    {
        // Display the exception and quit
        MessageBox(NULL, e.GetText(), AtoT(e.what()), MB_ICONERROR);

        return -1;
    }
}




