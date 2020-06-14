#include "stdafx.h"
#include "Splash.h"
#include "resource.h"


CSplash::CSplash()
{
    LoadFont();
    Create();          // Creates the splash window
    UpdateWindow();    // Calls OnDraw
}

CSplash::CSplash(LPCTSTR text) : m_text(text)
{ 
    LoadFont();
    Create();          // Creates the splash window
    UpdateWindow();    // Calls OnDraw
}

CSplash::~CSplash()
{
}

// Called during window creation.
int CSplash::OnCreate(CREATESTRUCT&)
{
    m_hIcon = (HICON)GetApp()->LoadImage(IDW_MAIN, IMAGE_ICON, 256, 256, LR_DEFAULTCOLOR | LR_SHARED | LR_DEFAULTSIZE);
    CenterWindow();
    return 0;
}

// Creates the progress bar child window.
void CSplash::CreateBar()
{
    m_progress.Create(*this);
    m_progress.SetWindowPos(NULL, CRect(50, 200, 204, 210), SWP_SHOWWINDOW);
    m_progress.SetStep(1);
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
        if (mem != NULL)
        {
            void* data = LockResource(mem);
            if (data != NULL)
            {
                DWORD len = SizeofResource(hResInstance, res);
                DWORD fonts = 0;

                m_fontHandle = AddFontMemResourceEx(
                    data,           // font resource
                    len,        // number of bytes in font resource 
                    NULL,           // Reserved. Must be 0.
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
    dcMem.CreateCompatibleBitmap(dc, 256, 256);

    DrawIconEx(dcMem, 0, 0, m_hIcon, 256, 256, 0, 0, DI_NORMAL);

    dcMem.SetTextColor(RGB(0, 255, 0));
    dcMem.SetBkMode(TRANSPARENT);
    CRect rc = GetClientRect();
    rc.top += 40;

    dcMem.CreateFont(30, 0, 0, 0, FW_BOLD, 0, 0, 0, DEFAULT_CHARSET, OUT_OUTLINE_PRECIS,
        CLIP_DEFAULT_PRECIS, ANTIALIASED_QUALITY, FF_MODERN, L"French Script MT");

    dcMem.DrawText(m_text.c_str(), m_text.GetLength(), rc, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

    dc.BitBlt(0, 0, 256, 256, dcMem, 0, 0, SRCCOPY);
}

// Sets the CREATESTRUCT struct prior to window creation.
void CSplash::PreCreate(CREATESTRUCT& cs)
{
    cs.style = WS_CLIPCHILDREN | WS_POPUP | WS_VISIBLE;

    cs.cx = 256;
    cs.cy = 256;
}

// Sets the WNDCLASS struct prior to window creation.
void CSplash::PreRegisterClass(WNDCLASS& wc)
{
    wc.lpszClassName = L"Splash Screen";
    wc.hbrBackground = (HBRUSH)GetStockObject(NULL_BRUSH);
}

