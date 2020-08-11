/////////////////////////////
// MyWebControl.h
//

#ifndef __MYCONTROL_H
#define __MYCONTROL_H

///////////////////////////////////////
// Declaration of teh CWebControl class
//
class CWebControl : public CWebBrowser
{
public:
    CWebControl();

protected:
    virtual void OnInitialUpdate();
};


#endif // __MYCONTROL_H

