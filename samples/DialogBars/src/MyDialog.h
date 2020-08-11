/////////////////////////////
// MyDialog.h
//

#ifndef MYDIALOG_H
#define MYDIALOG_H


#include "ProgressBar.h"
#include "ScrollBar.h"
#include "Slider.h"

/////////////////////////////////////
// Declaration of the CMyDialog class
//
class CMyDialog : public CDialog
{
public:
    CMyDialog(UINT resID);
    virtual ~CMyDialog();

    void SetProgress(int pos);
    void SetScroll(int pos);
    void SetSlider(int pos);
    void SetStatic(BOOL isSlider, int pos);

protected:
    virtual void OnDestroy();
    virtual BOOL OnInitDialog();
    virtual void OnOK();

private:
    CMyProgressBar m_progressBar;
    CMyScrollBar m_scrollBar;
    CMySlider m_slider;
};

#endif //MYDIALOG_H
