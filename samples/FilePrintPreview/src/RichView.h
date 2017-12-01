/* (06-Sep-2016) [Tab/Indent: 8/8][Line/Box: 80/74]              (RichView.h) *
********************************************************************************

    Declaration of the CRichView class.

********************************************************************************

    Acknowledgment. This class was adapted from that in the PrintPreview 
    sample program appearing in the Win32++ framework sample folder, created 
    by David Nash and published under the permissions granted in that work.
    The adaptation here implements the CRichView class as self-contained
    package that implements reading, writing, print, and print preview
    functions. This work has been developed under the co-authorship of 
    Robert C. Tausworthe and David Nash, and released under the copyright 
    provisions of the Win32++ framework software, copyright (c) David Nash,
    2005-2017. The former author acknowledges and thanks the latter for his 
    patient direction and inspiration in the development of the classes of 
    these classes.

*******************************************************************************/

#ifndef RICHVIEW_H
#define RICHVIEW_H

  // word wrap options
enum WordWrapType
{
    WRAP_NONE    = 0,
    WRAP_WINDOW  = 1,
    WRAP_PRINTER = 2
};

/*============================================================================*/
    class 
CRichView : public CRichEdit                                /*

    This class is the customized class derived from the CRichEdit class
    for the PrintPreview sample program utilizing a rich edit window.
*-----------------------------------------------------------------------------*/
{
    public:
        CRichView();
        virtual ~CRichView();

        virtual UINT    GetPageBreaks(CDC& dcPrinter);
        virtual BOOL    GetNewFont();
        virtual void    PrintDC(UINT, CDC&, CDC&);
        virtual BOOL    ReadFile(LPCTSTR szFileName);
        virtual void    SetFontDefaults();
            void    SetDocName(LPCTSTR &szFileName)
                    { m_sDocPath = szFileName;}
        virtual void    PrintPages(CPrintDialog& PrintDlg);
            void    WordWrap(WordWrapType setting);
        virtual BOOL    WriteFile(LPCTSTR szFileName);
    
    protected:
        virtual void    OnAttach();
        virtual void    PreCreate(CREATESTRUCT& cs);

          // RichView data
        std::vector<int> m_PageBreaks;  // page starting position list

    private:
          // static callback functions
        static  DWORD CALLBACK RVStreamInCallback(DWORD dwCookie,
                LPBYTE pbBuff, LONG cb, LONG *pcb);
        static  DWORD CALLBACK RVStreamOutCallback(DWORD dwCookie,
                LPBYTE pbBuff, LONG cb, LONG *pcb);
        
        CFont   m_Font;
        CString m_sDocPath;
};
/*----------------------------------------------------------------------------*/
#endif // RICHVIEW_H
