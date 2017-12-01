//////////////////////////////////////////////
// MySlider.cpp

#include "stdafx.h"
#include "Slider.h"
#include "DialogApp.h"

void CMySlider::OnInitialUpdate()
{
    // Called automatically via AttachItem

    // Set a tic mark every ten units for the slider control
    SetTicFreq(10);
}

LRESULT CMySlider::OnMessageReflect(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(wParam);
    UNREFERENCED_PARAMETER(lParam);

    switch (uMsg)
    {
    case WM_HSCROLL:
        {
            // Get the slider bar position
            int nPos = GetPos();

            // Get a pointer to the MyDialog object
            CMyDialog& MyDialog = GetDialogApp().GetDialog();

            MyDialog.SetProgress(nPos);     // Set the progress bar position
            MyDialog.SetScroll(nPos);       // Set the scroll bar position
            MyDialog.SetStatic(TRUE, nPos); // Set the static text
            break;
        }
    }

    return 0;
}

void CMySlider::SetSlider(int nPos)
{
    // Set the slider position
    SetPos(nPos, TRUE);
}

