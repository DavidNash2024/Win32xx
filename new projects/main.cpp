///////////////////////////////////
// main.cpp

// To configure your project:
// 1) Add Win32++'s include directory to the project's additional include directories for C/C++
// 2) Add Win32++'s include directory to the project's additional include directories for Resources

#include "wincore.h"


// A class that inherits from CWnd. It is used to create the window.
class CView : public CWnd
{
public:
    CView() {}
    virtual ~CView() {}
    virtual LRESULT WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam)
    {
        switch (uMsg)
        {
        case WM_DESTROY:
            //End the program when window is destroyed
            ::PostQuitMessage(0);
            break;
        }

        //Use the default message handling for remaining messages
        return WndProcDefault(uMsg, wParam, lParam);
    }
};


int APIENTRY WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
    // Start Win32++
    CWinApp MyApp;
	
    // Create our view window
    CView m_View;
    m_View.Create();

    // Run the application's message loop
    return MyApp.Run();
}
