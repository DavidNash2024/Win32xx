/* (06-Sep-2016) [Tab/Indent: 8/8][Line/Box: 80/74]              (RichView.h) *
********************************************************************************

    Contents Description: Declaration of the CRichView class using the
    Win32++ Windows interface classes.

    Programming Notes: The programming style roughly follows that established
    for the 1995-1999 Jet Propulsion Laboratory Deep Space Network Planning and
    Preparation Subsystem project for C++ programming.

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
CRichView : public CRichEdit                                                /*

    This class is the customized class derived from the CRichEdit class
    for the PrintPreview sample program utilizing a rich edit window.
*-----------------------------------------------------------------------------*/
{
    public:
        CRichView() {}
        virtual ~CRichView() {}

        UINT    GetPageBreaks(CDC& dcPrinter);
        BOOL    GetNewFont() const;
        void    PrintDC(UINT, CDC&, CDC&);
        BOOL    ReadFile(LPCTSTR filePath);
        void    SetFontDefaults();
        void    SetDocName(LPCTSTR &filePath) { m_docPath = filePath;}
        void    PrintPages(CPrintDialog& printDlg);
        void    WordWrap(WordWrapType setting);
        BOOL    WriteFile(LPCTSTR filePath);

    protected:
        virtual void    OnAttach();
        virtual void    PreCreate(CREATESTRUCT& cs);

    private:
        CRichView(const CRichView&);                // Disable copy construction
        CRichView& operator = (const CRichView&);   // Disable assignment operator

          // static callback functions
        static  DWORD CALLBACK RVStreamInCallback(DWORD cookie, LPBYTE pbBuff,
                    LONG cb, LONG *pcb);
        static  DWORD CALLBACK RVStreamOutCallback(DWORD cookie, LPBYTE pbBuff,
                    LONG cb, LONG *pcb);

        CFont   m_font;
        CString m_docPath;
        std::vector<int> m_pageBreaks;  // page starting position list
};
/*----------------------------------------------------------------------------*/
#endif // RICHVIEW_H
