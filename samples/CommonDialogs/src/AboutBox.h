/* (01-Nov-2016) [Tab/Indent: 8/8][Line/Box: 80/74]               (AboutBox.h) *
********************************************************************************
|                                                                              |
|                    Authors: Robert Tausworthe, David Nash                    |
|                                                                              |
===============================================================================*

    Contents Description: Declaration of the AboutBox class for the 
    CommonDialogs sample application using the Win32++ Windows interface
    classes.

    Programming Notes: The programming standards roughly follow those 
    established by the 1997-1999 Jet Propulsion Laboratory Deep Space Network
    Planning and Preparation Subsystem project for C++ programming.

*******************************************************************************/

#ifndef ABOUTBOX_H
#define ABOUTBOX_H

/*============================================================================*/
    class
AboutBox : public CDialog                                                   /*

*-----------------------------------------------------------------------------*/
{
    public:
        AboutBox() : CDialog(IDW_ABOUT) {}
        virtual ~AboutBox(){}
        void SetStatus(const CString& about) { m_about = about; }

    protected:
        virtual BOOL OnInitDialog();

    private:
        CString m_about;
};
/*----------------------------------------------------------------------------*/
#endif  // ABOUTBOX_H
