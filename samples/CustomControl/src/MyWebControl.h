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

protected:
    virtual void OnInitialUpdate();
};


#endif // __MYCONTROL_H

