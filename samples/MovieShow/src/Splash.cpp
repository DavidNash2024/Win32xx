/////////////////////////////
// Splash.cpp
//

#include "stdafx.h"
#include "Splash.h"
#include "resource.h"

///////////////////////////////
// CSplash function definitions
//

// Constructor.
CSplash::CSplash() : m_hIcon(0), m_fontHandle(0)
{
    LoadFont();
}

// Destructor.
CSplash::~CSplash()
{
}

// Called during window creation.
int CSplash::OnCreate(CREATESTRUCT&)
{
    int xImage = 256;
    int yImage = 256;
    m_hIcon = (HICON)GetApp()->LoadImage(IDW_MAIN, IMAGE_ICON, xImage, yImage, LR_SHARED);
    CenterWindow();
    AddBar();
    return 0;
}

// Adds a progress bar child window.
void CSplash::AddBar()
{
    m_progress.Create(*this);
    m_progress.SetWindowPos(0, 50, 200, 156, 10, 0);
    m_progress.ShowWindow(SW_HIDE);
    m_progress.SetStep(1);
}

// Hides the splash screen
void CSplash::Hide()
{
    ShowWindow(SW_HIDE);
}

// Loads the font from the FRSCRIPT.TTF resource.
void CSplash::LoadFont()
{
    HINSTANCE hResInstance = GetApp()->GetResourceHandle();

    HRSRC res = FindResource(hResInstance,
        MAKEINTRESOURCE(IDF_MYFONT), L"BINARY");
    if (res)
    {
        HGLOBAL mem = LoadResource(hResInstance, res);
        if (mem != 0)
        {
            void* data = LockResource(mem);
            if (data != 0)
            {
                DWORD len = SizeofResource(hResInstance, res);
                DWORD fonts = 0;

                m_fontHandle = AddFontMemResourceEx(
                    data,           // font resource
                    len,            // number of bytes in font resource
                    0,              // Reserved. Must be 0.
                    &fonts          // number of fonts installed
                );

                if (m_fontHandle == 0)
                {
                    MessageBox(L"Font add fails", L"Error", MB_OK);
                }

            }
        }
    }
}

// Perform the drawing on the splash window
void CSplash::OnDraw(CDC& dc)
{
    CMemDC dcMem(dc);
    int xImage = 256;
    int yImage = 256;
    dcMem.CreateCompatibleBitmap(dc, xImage, yImage);
    DrawIconEx(dcMem, 0, 0, m_hIcon, xImage, yImage, 0, 0, DI_NORMAL);

    dcMem.SetTextColor(RGB(0, 255, 0));
    dcMem.SetBkMode(TRANSPARENT);
    CRect rc = GetClientRect();
    rc.top += 40;

    dcMem.CreateFont(30, 0, 0, 0, FW_BOLD, 0, 0, 0, DEFAULT_CHARSET, OUT_OUTLINE_PRECIS,
        CLIP_DEFAULT_PRECIS, ANTIALIASED_QUALITY, FF_MODERN, L"French Script MT");

    dcMem.DrawText(m_text.c_str(), m_text.GetLength(), rc, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

    dc.BitBlt(0, 0, xImage, yImage, dcMem, 0, 0, SRCCOPY);
}

// Sets the CREATESTRUCT struct prior to window creation.
void CSplash::PreCreate(CREATESTRUCT& cs)
{
    cs.style = WS_POPUP;           // Initially hidden
    cs.dwExStyle = WS_EX_TOPMOST | WS_EX_NOACTIVATE; // Topmost and hidden from taskbar

    cs.cx = 256;
    cs.cy = 256;
}

// Sets the WNDCLASS struct prior to window creation.
void CSplash::PreRegisterClass(WNDCLASS& wc)
{
    wc.lpszClassName = L"Splash Screen";
    wc.hbrBackground = (HBRUSH)GetStockObject(NULL_BRUSH);
    wc.hCursor = ::LoadCursor(0, IDC_ARROW);
}

// Removes the progress bar.
void CSplash::RemoveBar()
{
    m_progress.Destroy();
}

// Centers the splash screen with text over the parent window.
void CSplash::ShowText(LPCTSTR text, CWnd* parent)
{
    assert(text != 0);
    assert(parent != 0);
    assert(parent->IsWindow());

    m_text = text;
    int xImage = 256;
    int yImage = 256;

    CRect parentRect = parent->GetWindowRect();
    int x = parentRect.left + (parentRect.Width()  - xImage) / 2;
    int y = parentRect.top  + (parentRect.Height() - yImage) / 2;

    // Show the window on top without activating it.
    SetWindowPos(HWND_TOPMOST, x, y, xImage, yImage, SWP_NOACTIVATE | SWP_SHOWWINDOW);

    UpdateWindow();
    Invalidate();
}
