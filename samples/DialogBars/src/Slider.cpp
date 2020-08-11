/////////////////////////////
// Slider.cpp
//

#include "stdafx.h"
#include "Slider.h"
#include "DialogApp.h"

/////////////////////////////////
// CMySlider function definitions
//

// Called after the slider control is attached.
void CMySlider::OnInitialUpdate()
{
    // Set a tic mark every ten units for the slider control
    SetTicFreq(10);
}

// Handles messages reflected back from the dialog.
LRESULT CMySlider::OnMessageReflect(UINT msg, WPARAM wparam, LPARAM lparam)
{
    UNREFERENCED_PARAMETER(wparam);
    UNREFERENCED_PARAMETER(lparam);

    switch (msg)
    {
    case WM_HSCROLL:
        {
            // Get the slider bar position
            int pos = GetPos();

            // Get a pointer to the MyDialog object
            CMyDialog& MyDialog = GetDialogApp()->GetDialog();

            MyDialog.SetProgress(pos);     // Set the progress bar position
            MyDialog.SetScroll(pos);       // Set the scroll bar position
            MyDialog.SetStatic(TRUE, pos); // Set the static text
            break;
        }
    }

    return 0;
}

// Sets the slider control's position.
void CMySlider::SetSlider(int pos)
{
    // Set the slider position
    SetPos(pos, TRUE);
}

