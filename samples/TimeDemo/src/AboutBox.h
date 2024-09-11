/* (06-May-2024) [Tab/Indent: 8/8][Line/Box: 80/74]                (AboutBox.h) *
********************************************************************************
|                                                                              |
|               Authors: Robert C. Tausworthe, David Nash, 2020                |
|                                                                              |
===============================================================================*

    Contents Description: Declaration of the AboutBox dialog for this
    application using the Win32++ framework.

    Programming Notes: The programming style roughly follows that established
    for the 1995-1999 Jet Propulsion Laboratory Deep Space Network Planning and
    Preparation Subsystem project for C++ programming.

*******************************************************************************/

#ifndef ABOUT_BOX_H_DEFINED
#define ABOUT_BOX_H_DEFINED

/*============================================================================*/
    class
AboutBox        : public CDialog                                        /*

*-----------------------------------------------------------------------------*/
{
    public:
        AboutBox();
        virtual ~AboutBox() override {}

        void SetStatus(const CString& status) { m_status = status; }

    protected:
        virtual BOOL OnInitDialog() override;

    private:
        AboutBox(const AboutBox&) = delete;
        AboutBox& operator=(const AboutBox&) = delete;

        CString m_status;

};
/*----------------------------------------------------------------------------*/
#endif // ABOUT_BOX_H_DEFINED

