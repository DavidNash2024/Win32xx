/* (03-Mar-2020) [Tab/Indent: 8/8][Line/Box: 80/74]                    (MRU.h) *
********************************************************************************
|                                                                              |
|               Authors: Robert C. Tausworthe, David Nash, 2020                |
|                                                                              |
===============================================================================*

     Contents Description:  Implementation of the CMRU class for this
     application using the Win32++ framework.

    This class is used to load, display, edit, and save the Most Recently Used
    (MRU) list, commonly used to contain the files used by an application.
    This class has been adapted from the Win32++ framework as an alternate to
    the MRU operations found in the CFrame class of Win32++ for those not
    wishing to use the system's registry, but prefer a separate  application
    parameter file.

    Programming Notes: The programming style roughly follows that established
    for the 1995-1999 Jet Propulsion Laboratory Deep Space Network Planning and
    Preparation Subsystem project for C++ programming.

*******************************************************************************/

#ifndef GPP_MRU_H_FILE
#define GPP_MRU_H_FILE

/*============================================================================*/
    class
CMRU : public CObject                                                   /*

    Declaration of the Most-Recently-Used (MRU) item list handler class.
**----------------------------------------------------------------------------*/
{
      // Public Declarations
    public:
          // construction/destruction
        CMRU();
        virtual ~CMRU(){}

        void    AddEntry(LPCTSTR entryName);
        void    AssignMenu(CMenu frameMenu, size_t maxMRU = 0);
        void    EmptyList();
        CString GetEntry(size_t index);
        void    RemoveEntry(LPCTSTR entryName);
        void    UpdateMenu();
        void    ValidateMRU();

    protected:
        virtual void    Serialize(CArchive& ar);

    private:
        CMRU(const CMRU&);                // Disable copy construction
        CMRU& operator = (const CMRU&);   // Disable assignment operator

        std::vector<CString> m_MRUEntries; // MRU array entries
        size_t      m_maxMRU;           // maximum MRU entries, this app
        CMenu       m_frameMenu;        // the frame's main menu

        const CString m_emptyMRUListLabel; // File MRU locator label
};
/*----------------------------------------------------------------------------*/
#endif // GPP_MRU_H_FILE


