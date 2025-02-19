///////////////////////////////////////
// StaticImage.cpp
//

#include "stdafx.h"
#include "StaticImage.h"

void CStaticImage::DpiScaleImage()
{
    m_patternImage.LoadBitmap(IDB_BITMAP1);
    m_patternImage = DpiScaleUpBitmap(m_patternImage);
    SetBitmap(m_patternImage);
}

void CStaticImage::OnAttach()
{
    DpiScaleImage();
}

LRESULT CStaticImage::OnAfterDpiChange(UINT msg, WPARAM wparam, LPARAM lparam)
{
    DpiScaleImage();
    return FinalWindowProc(msg, wparam, lparam);
}

LRESULT CStaticImage::WndProc(UINT msg, WPARAM wparam, LPARAM lparam)
{
    switch (msg)
    {
    case WM_DPICHANGED_AFTERPARENT:  return OnAfterDpiChange(msg, wparam, lparam);
    }

    return WndProcDefault(msg, wparam, lparam);
}
