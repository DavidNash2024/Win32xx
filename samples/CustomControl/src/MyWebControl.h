/////////////////////////////
// MyWebControl.h
//

#ifndef __MYCONTROL_H
#define __MYCONTROL_H


/////////////////////////////////////////////////
// CWebControl is a custom control for the dialog
// that displays a web browser.
class CWebControl : public CWebBrowser
{
public:
    CWebControl();
    virtual ~CWebControl() override = default;

protected:
    virtual void OnInitialUpdate() override;
    virtual LRESULT WndProc(UINT msg, WPARAM wparam, LPARAM lparam) override;

private:
    CWebControl(const CWebControl&) = delete;
    CWebControl& operator=(const CWebControl&) = delete;
};


#endif // __MYCONTROL_H

