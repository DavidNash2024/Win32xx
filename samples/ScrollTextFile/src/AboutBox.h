/* (10-08-2016) [Tab/Indent: 4/4][Line/Box: 80/74]                (AboutBox.h) *
********************************************************************************
|                                                                              |
|                         robert.c.tausworthe@ieee.org                         |
|                                                                              |
===============================================================================*

    Contents Description: Declaration of the basic AboutBox dialog for a typical 
    application using the Win32++ Windows interface classes, Copyright (c) 
    2005-2017 David Nash, under permissions granted therein.

     Programming Notes:
        The programming standards roughly follow those established by the 
    1997-1999 Jet Propulsion Laboratory Network Planning and Preparation 
    Subsystem project for C++ programming.

*******************************************************************************/

#ifndef ABOUT_BOX_H_DEFINED
#define ABOUT_BOX_H_DEFINED

/*============================================================================*/
        class
AboutBox        : public CDialog                                            /*

*-----------------------------------------------------------------------------*/
{
      // public declarations
    public:
        AboutBox();
        ~AboutBox();
        
            void    SetAboutBoxInfo(LPCTSTR info) { m_sAboutBoxInfo = info;}

      // protected declarations
    protected:
        virtual INT_PTR DialogProc(UINT, WPARAM, LPARAM);

      // private declarations
    private:
        CString     m_sAboutBoxInfo; // about the program, credits, etc.
};
/*----------------------------------------------------------------------------*/
#endif // ABOUT_BOX_H_DEFINED

