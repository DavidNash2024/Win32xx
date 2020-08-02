/* (10-08-2016) [Tab/Indent: 4/4][Line/Box: 80/74]                (AboutBox.h) *
********************************************************************************
|                                                                              |
|                Authors: Robert Tausworthe, David Nash, 2020                  |
|                                                                              |
===============================================================================*

    Contents Description: Declaration of the AboutBox dialog for this 
    sample prpgram using the Win32++ Windows interface classes, Copyright 
    (c) 2005-2020 David Nash, under permissions granted therein.

     Programming Notes: The programming standards roughly follow those 
     established by the 1997-1999 Jet Propulsion Laboratory Network Planning 
     and Preparation Subsystem project for C++ programming.

*******************************************************************************/

#ifndef ABOUT_BOX_H_DEFINED
#define ABOUT_BOX_H_DEFINED

/*============================================================================*/
    class
AboutBox    : public CDialog                                            /*

*-----------------------------------------------------------------------------*/
{
    public:
        AboutBox();
        virtual ~AboutBox(){}
        
        void    SetAboutBoxInfo(LPCTSTR info) { m_aboutBoxInfo = info;}

    protected:
        BOOL    OnInitDialog(); 

    private:
        CString m_aboutBoxInfo; // about the program, credits, etc.
};
/*----------------------------------------------------------------------------*/
#endif // ABOUT_BOX_H_DEFINED
