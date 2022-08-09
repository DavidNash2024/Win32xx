/* (24-Jul-2015) [Tab/Indent: 8/8][Line/Box: 80/74]               (MyButton.h) *
********************************************************************************
|                                                                              |
|                      Author: Robert C. Tausworthe, 2020                      |
|                                                                              |
===============================================================================*

    Contents Description: The CMyButton class declaration. This class is
    derived from the CButton class found in the Win32++ Windows interface
    classes. It permits individual settings of the parameters of owner-drawn
    button controls on dialog forms.

    Programming Notes: The programming style roughly follows that established
    by the 1995-1999 Jet Propulsion Laboratory Deep Space Network Planning and
    Preparation Subsystem project for C++ programming.

    Acknowledgement: The author would like to thank and acknowledge the advice,
    critical review, insight, and assistance provided by David Nash in the
    development of this work.

*******************************************************************************/

#ifndef CMyButton_H
#define CMyButton_H

/******************************************************************************/
    class
CMyButton : public CButton                                                  /*

    This class extends the CButton class to permit owner-drawn buttons.
*-----------------------------------------------------------------------------*/
{
    public:
        CMyButton() {}
        virtual ~CMyButton() {}

        virtual void DrawItem(LPDRAWITEMSTRUCT);

    protected:

    private:
        CMyButton(const CMyButton&);                // Disable copy construction
        CMyButton& operator = (const CMyButton&);   // Disable assignment operator
};

/*----------------------------------------------------------------------------*/
#endif  // CMyButton_H
