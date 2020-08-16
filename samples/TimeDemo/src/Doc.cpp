/* (02-Aug-2014) [Tab/Indent: 8/8][Line/Box: 80/74]                  (Doc.cpp) *
********************************************************************************
|                                                                              |
|               Authors: Robert C. Tausworthe, David Nash, 2020                |
|                                                                              |
===============================================================================*

    Contents Description:  Implementation of the CDoc class for this application
    using the Win32++ framework.

    Programming Notes: The programming standards roughly follow those
    established by the 1997-1999 Jet Propulsion Laboratory Deep Space Network
    Planning and Preparation Subsystem project for C++ programming.

********************************************************************************

    Implementation of the CDoc class

    This particular formulation of the document class presumes that the
    document souce is a series of textual lines that are accessed and
    formatted by the methods of this class and passed to the CView object
    for display. In the present usage of this skeleton implementation,
    a document is simulated as the results of computations illustrating
    the capabilities of the CTime class, the numeric version information
    of the Windows system, and the command line parameters.

*******************************************************************************/

#include "stdafx.h"
#include "App.h"


  // local formats for displaying CTime values as strings
static const CString longDateFmt = TEXT("%d-%b-%Y [%j] (%a) %H:%M:%S %z");
static const CString shortDateFmt = TEXT("%d-%b-%Y");
static const CString simpleHMSFmt = TEXT("%I:%M:%S %p");

  // static constant initiallization: latest CDoc.cpp file compilation date
const CString CDoc::m_compiledOn = __DATE__;

/*============================================================================*/
    CDoc::
CDoc()                                                                  /*

    Construct the document object and load the document to be initially
    displayed at the start of operations.
*-----------------------------------------------------------------------------*/
{
      // create the document object, set default values, and set the
      // initial document state

    m_fileDlgFilter =
        _T("Time Demo Files (*.arc)\0")
        _T("*.arc\0")
        _T("All Files (*.*)\0")
        _T("*.*\0")
        _T("\0");

    m_docPath.Empty();
    m_docWidth   = 0;
    m_docContent.clear();
      // show initial document content for display
    NewDocument();
}

/*============================================================================*/
    int CDoc::
GetDocLength() const                                                    /*

    Return the document length, in records.
*----------------------------------------------------------------------------*/
{
    return static_cast<int>(m_docContent.size());
}

/*============================================================================*/
    CString CDoc::
GetDocOpenFileName(const CString &title) const                          /*

    Bring up the open file dialog and get the path of the file to open.
    If none is given, return an empty path. Use the title as the dialog
    window title.
*-----------------------------------------------------------------------------*/
{
    // Bring up the dialog, and  open the file
    CString str;
    DWORD dwFlags = OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT;
    CFileDialog FileDlg(TRUE, 0, 0, dwFlags, m_fileDlgFilter);
    FileDlg.SetTitle(title);
    if (FileDlg.DoModal() == IDOK)
        str = FileDlg.GetPathName();
    else
        ::MessageBox(NULL, _T("No document name was selected.\n")
            _T("The document cannot be opened."),
            _T("Information"), MB_OK | MB_ICONINFORMATION |
            MB_TASKMODAL);

    return str;
}

/*============================================================================*/
    CString CDoc::
GetDocRecord(int rcd, int left /* = 0 */, int length /* = -1 */) const  /*

    Return a CString containing the document rcd record, starting at
    the left position and continuing for length characters.
*----------------------------------------------------------------------------*/
{
    int size = GetDocLength();
    if (size == 0 || rcd >= size || rcd < 0)
        return _T("");

    CString s  = m_docContent[rcd];

      // compute length of text to extract
    if (length < 0)
        length = s.GetLength();
    length = MAX(0, length - left);
      // extract length chars after left position using base class
    return s.Mid(left, length);
}

/*============================================================================*/
    CString CDoc::
GetDocSaveFileName(const CString &title) const                          /*

    Bring up the file save dialog and get the path of file to save the
    document in. If nont is selected, return an empty path.  Use the title
    as the window title.
*-----------------------------------------------------------------------------*/
{
    CString str;
    TCHAR extbuff[10];
    DWORD dwFlags = OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT;
    CFileDialog FileDlg(FALSE, extbuff, NULL, dwFlags, m_fileDlgFilter);
    FileDlg.SetTitle(title);

    if (FileDlg.DoModal() == IDOK)
        str = FileDlg.GetPathName();
    else
        ::MessageBox(NULL, _T("No document name was selected.\n")
            _T("The current document will not be saved."),
            _T("Information"), MB_OK | MB_ICONINFORMATION |
            MB_TASKMODAL);

    return str;
}

/*============================================================================*/
    int CDoc::
GetDocWidth() const                                                     /*

    Return the document width, in characters.
*----------------------------------------------------------------------------*/
{
    return m_docWidth;
}

/*============================================================================*/
    bool CDoc::
OpenDoc(const CString &docFileName)                                   /*

    Open the document having the given doc_file_name and load its contents
    into the internal CString array.  Return true if the document was
    opened, or false if not.
*-----------------------------------------------------------------------------*/
{
      // if there is a document already open with this file name,
      // say so, and return
    if (!m_docPath.IsEmpty() && m_docPath.CompareNoCase(docFileName) == 0)
    {
        CString s;
        s.Format(_T("This file is already open:\n    %s"),
            docFileName.c_str());
        ::MessageBox(NULL, s, _T("Error"), MB_OK |
            MB_ICONEXCLAMATION | MB_TASKMODAL);
        return true;
    }

      // if the name does not exist, return
    if (docFileName.IsEmpty())
        return false;

    try
    {
        m_docContent.clear();
        CArchive ar(docFileName, CArchive::load);
        ar >> *this;
    }
    catch (const CException &e)  // catch CException events
    {     // Process the exception and  quit
        CString msg;
        CString what(e.what());
        msg.Format(_T("Error restoring the document.\n%s\n%s"),
            e.GetText(), e.GetErrorString());
        ::MessageBox(NULL, msg.c_str(), what.c_str(),
            MB_OK | MB_ICONSTOP | MB_TASKMODAL);
        return false;
    }
    catch(...) // catch everything else
    {
        CString msg = _T("Error restoring the document.\n");
        ::MessageBox(NULL, msg.c_str(), _T("Exception"),
            MB_OK | MB_ICONSTOP | MB_TASKMODAL);
        return false;
    }
      // register the open document
    m_docPath = docFileName;
    return true;
}

/*============================================================================*/
    CString CApp::
MakeAppDataPath(const CString & subpath)                                /*

        Return a string giving the path APPDATA environmental path, with the
        given subpath appended.  Create this path if it does not exist. If
        an error is encountered, throw a user exception.
*-----------------------------------------------------------------------------*/
{
    CString appdata = GetAppDataPath();

    int from, to, next;
    for (from = 0, to = subpath.GetLength(); from < to; from = ++next)
    {
        int nextbk = subpath.Find(_T("\\"), from);
        int nextfwd = subpath.Find(_T("/"), from);
        next = MAX(nextbk, nextfwd);
        if (next < 0)
            next = to;

        CString add = subpath.Mid(from, next - from);
        appdata += _T("\\") + add;
        if ((::CreateDirectory(appdata, 0) == 0) && GetLastError() != ERROR_ALREADY_EXISTS)
        {
            CString msg = appdata + _T("\nDirectory creation error.");
            throw CUserException(msg);
        }
    }
    return appdata;
}

/*============================================================================*/
    void CDoc::
NewDocument()                                                           /*

    For this TimeDemo, develop the ad hoc array of strings that contain the
    results of various tests of the CTime class and Win32++ functions.
*-----------------------------------------------------------------------------*/
{
    m_docContent.clear();
    m_docWidth = 0;
    CString s;
    s = _T("  -------------------------------------------------------");
    PushContent(s);
    s.Format(_T("    CTime functions test results"),
        8 * sizeof(size_t));
    PushContent(s);
    s = _T("  -------------------------------------------------------");
    PushContent(s);
    PushContent(_T(""));
    CTime t1;
    PushContent(_T("   1. CTime t1; (long date format)               ")
        + t1.Format(longDateFmt));

    CTime t2(t1);
    PushContent(_T("   2. CTime t2(t1); (as UTC)                     ")
        + t2.FormatGmt(longDateFmt));

    CTime t3(time_t(0));
    PushContent(_T("   3. CTime t3(time_t(0)); (short date format)   ")
        + t3.FormatGmt(shortDateFmt));
    PushContent(_T("   4. CTime t3(time_t(0)); (simple HMS format)   ")
        + t3.FormatGmt(simpleHMSFmt));

    t3 = CTime::GetCurrentTime();
    PushContent(_T("   5. CTime t3 = GetCurrentTime()                ")
        + t3.Format(longDateFmt));

    CTime t4(2014, 3, 8, 2, 37, 40);
    PushContent(_T("   6. CTime t4(2014, 3, 8, 2, 37, 40);           ")
        + t4.Format(longDateFmt));

    CTime t5(2014, 2, 9, 2, 37, 40);
    PushContent(_T("   7. CTime t5(2014, 2, 9, 2, 37, 40);           ")
        + t5.Format(longDateFmt));

    CTime t6(2014, 3, 9, 3, 37, 40);
    PushContent(_T("   8. CTime t6(2014, 3, 9, 3, 37, 40);           ")
        + t6.Format(longDateFmt));

    CTime t7(2014, 67, 3, 37, 40);
    PushContent(_T("   9. CTime t7(2014, 67, 3, 37, 40);             ")
        + t7.Format(longDateFmt));

    CTime t8(2014, 68, 3, 37, 40);
    PushContent(_T("  10. CTime t8(2014, 68, 3, 37, 40);             ")
        + t8.Format(longDateFmt));

    CString s9(_T("09-Mar-2014 3:37:40"));
    CTime t9(GetTimeFromStr(s9));  // a CString conversion
    PushContent(_T("  11. CTime t9(\"09-Mar-2014 3:37:40\")   CString  ")
        + t9.Format(longDateFmt));

    CTime t10(GetTimeFromStr(_T("09-Mar-2014 3:37:40")));  // a LPCTSTR conversion
    PushContent(_T("  12. CTime t10(\"09-Mar-2014 2:37:40\")  LPCTSTR  ")
        + t10.Format(longDateFmt));

    CTime t11(2014, 3, 6, 2, 3, 37, 40);
    PushContent(_T("  13. CTime t11(2014, 3, 6, 2, 3, 37, 40);       ")
        + t11.Format(longDateFmt));

    CTime t12(2014, 3, 6, 3, 3, 37, 40);
    PushContent(_T("  14. CTime t12(2014, 3, 6, 3, 3, 37, 40);       ")
        + t12.Format(longDateFmt));

    SYSTEMTIME st;
    t12.GetAsSystemTime(st);
    CTime t13(st);
    PushContent(_T("  15. GetAsSystemTime(t12);                      ")
        + t13.Format(longDateFmt));

    FILETIME ft;
    t12.GetAsFileTime(ft);
    CTime t14(ft);
    PushContent(_T("  16. GetAsFileTime(t12);                        ")
        + t14.Format(longDateFmt));

    WORD fatdate, fattime;
    ::FileTimeToDosDateTime(&ft, &fatdate, &fattime);
    CTime t15(fatdate, fattime);
    PushContent(_T("  17. t15(fatdate, fattime)                      ")
        + t15.Format(longDateFmt));

    int yr = t15.GetYear();
    int mo = t15.GetMonth();
    int da = t15.GetDay();
    int hr = t15.GetHour();
    int mn = t15.GetMinute();
    int sc = t15.GetSecond();
    int dw = t15.GetDayOfWeek();
    int dy = t15.GetDayOfYear();
    CString fmt = _T("%04d-%02d-%02d %02d:%02d:%02d [%03d] (%d) ");
    s.Format(fmt + _T("local"), yr, mo, da, hr, mn, sc, dy, dw);
    PushContent(_T("  18. t15, yr-mo-day hr:min:sec [doy] (wk)       ")
        + s);

    yr = t15.GetYear(false);
    mo = t15.GetMonth(false);
    da = t15.GetDay(false);
    hr = t15.GetHour(false);
    mn = t15.GetMinute(false);
    sc = t15.GetSecond(false);
    dw = t15.GetDayOfWeek(false);
    dy = t15.GetDayOfYear(false);
    s.Format(fmt + _T("UTC"), yr, mo, da, hr, mn, sc, dy, dw);
    PushContent("  19. t15, yr-mo-day hr:min:sec [doy] (wk)       "
        + s);

    CTimeSpan ts = t12 - t1;
    s = ts.Format(_T("%D days, %H:%M:%S"));
    PushContent(_T("  20. t12 - t1 =                                 ")
        + s);

    s.Format(_T("%ld total hrs, %ld min, %ld sec"),
        ts.GetTotalHours(), ts.GetTotalMinutes(), ts.GetTotalSeconds());
    PushContent(_T("  21. t12 - t1 =                                 ")
        + s);

    ts = t1 - t12;
    s = ts.Format(_T("%D days, %H:%M:%S"));
    PushContent(_T("  22. t1 - t12 =                                 ")
        + s);

    CTime t16 = t12 + ts;
    PushContent(_T("  23. t16 = t12 + (t1 - t12) =  t1               ")
        + t16.Format(longDateFmt));

    t16 -= ts;
    PushContent(_T("  24. t16 -= (t1 - t12) =  t12                   ")
        + t16.Format(longDateFmt));

    t16 += ts;
    PushContent(_T("  25. t16 += (t1 - t12) =  t1                    ")
        + t16.Format(longDateFmt));

    t16 = t1 - ts;
    PushContent(_T("  26. t16 = t1 - (t1 - t12) =  t12               ")
        + t16.Format(longDateFmt));

    ts = -ts;
    s = ts.Format(_T("%D days, %H:%M:%S"));
    PushContent(_T("  27. -(t1 - t12) =                              ")
        + s);

    PushContent(_T("  28. t8 == t7;                                  ")
        + Truth(t8 == t7));

    PushContent(_T("  29. t8 == t9;                                  ")
        + Truth(t8 == t9));

    PushContent(_T("  30. t8 != t7;                                  ")
        + Truth(t8 != t7));

    PushContent(_T("  31. t8 < t7;                                   ")
        + Truth(t8 < t7));

    PushContent(_T("  32. t8 > t7;                                   ")
        + Truth(t8 > t7));

    PushContent(_T(""));
    s.Format(_T("  -------------------------------------------------------"));
    PushContent(s);
    s = _T("    Platform values for this machine");
    PushContent(s);
    s.Format(_T("  -------------------------------------------------------"));
    PushContent(s);
    PushContent(_T(""));
    s.Format(_T("  WINVER        = 0x%x"), WINVER);
    PushContent(s);
    s.Format(_T("  WIN32_WINNT   = 0x%x"), _WIN32_WINNT);
    PushContent(s);
    s.Format(_T("  _WIN32_IE     = 0x%x"), _WIN32_IE);
    PushContent(s);
    s.Format(_T("  NTDDI_VERSION = 0x%x"), NTDDI_VERSION);
    PushContent(s);
    PushContent(_T(""));

    s = ::GetCommandLine();
    s.Format(_T("  Command line: %s"), s.c_str());
    PushContent(s);
    PushContent(_T(""));

      // Here we process the Win32++ access to command line arguments, and
      // display them among the other tests in this demo. GetCommandLineArgs
      // retrieves our command line arguments and stores them in a vector
      // of CString.
    std::vector<CString> args = GetCommandLineArgs();
      // The second argument (if any) contains this app file name.
    for (size_t i = 0; i < args.size(); i++)
    {
        s.Format(_T("  argv(%d) = %s"), i, args[i].c_str());
        PushContent(s);
    }

    PushContent(_T(""));
    s.Format(_T("  -------------------------------------------------------"));
    PushContent(s);
    s.Format(_T("    C++ data type sizes for this %d-bit Windows program."),
        8 * sizeof(size_t));
    PushContent(s);
    s.Format(_T("  -------------------------------------------------------"));
    PushContent(s);
    PushContent(_T(""));
    s.Format(_T("  char                     %lu bytes"), sizeof(char));
    PushContent(s);
    s.Format(_T("  short                    %lu bytes"), sizeof(short));
    PushContent(s);
    s.Format(_T("  int                      %lu bytes"), sizeof(int));
    PushContent(s);
    s.Format(_T("  long                     %lu bytes"), sizeof(long));
    PushContent(s);
    s.Format(_T("  LONGLONG                 %lu bytes"), sizeof(LONGLONG));
    PushContent(s);
    PushContent(_T(""));
    s.Format(_T("  UINT                     %lu bytes"), sizeof(UINT));
    PushContent(s);
    s.Format(_T("  TCHAR                    %lu bytes"), sizeof(TCHAR));
    PushContent(s);
    s.Format(_T("  WORD                     %lu bytes"), sizeof(WORD));
    PushContent(s);
    s.Format(_T("  DWORD                    %lu bytes"), sizeof(DWORD));
    PushContent(s);
    s.Format(_T("  size_t                   %lu bytes"), sizeof(size_t));
    PushContent(s);
    s.Format(_T("  LPTSTR                   %lu bytes"), sizeof(LPTSTR));
    PushContent(s);
    s.Format(_T("  double                   %lu bytes"), sizeof(double));
    PushContent(s);
    PushContent(_T(""));
    m_docPath.Empty();
}

/*============================================================================*/
    CTime CDoc::
GetTimeFromStr(LPCTSTR szTime, int nDST /* = -1 */) const               /*

    Construct a CTime as directed by the formatting CString timestr, whose
    specifications appear below. Any nonconformity between timestr  and
    these expected format standards will result in throwing an exception.
    Acceptable formats are:

        "yyyy/mo/da H:M:S"
        "da-Mon-yyyy H:M:S"
        "Month da, yyyy H:M:S"
        "yyyy+doy H:M:S"

    The year, month, day, hour, minute, and  second values must be consistent
    with the requirements given in the description of the constructor of
    CTime(year, month, day, hour, minute, second).  The date and  time
    specifications are presumed to apply to the local zone. If H:M:S is not
    present, then midnight is presumed, and  if just S is absent, zero
    seconds is the default. The first form is assumed when the given date
    string contains "/"; the second, when "-" appears; the third, when ","
    is present; and  the fourth, when "+" is found.

*---------------------------------------------------------------------------- - */
{
    assert(szTime);
    CString timestr(szTime);
    int  p1, p2, p3; // position indexes into timestr
    int len = timestr.GetLength();
    int yyyy;   // year, 4 digits
    int mo;     // month 1 - 12
    int da;     // day 1-31
    int doy;    // day of year 1 - 366
    int H;  // hour of day 0 - 23
    int M;  // minute of hour 0 - 59
    int S;  // seconds of minute 0 - 61 (leap years)

    CString Month[] =   {_T("January"), _T("February"),
                  _T("March"), _T("April"), _T("May"),
                  _T("June"), _T("July"), _T("August"),
                  _T("September"), _T("October"),
                  _T("November"), _T("December")};
    CString AbMonth[] = {_T("Jan"), _T("Feb"), _T("Mar"),
                  _T("Apr"), _T("May"), _T("Jun"),
                  _T("Jul"), _T("Aug"), _T("Sep"),
                  _T("Oct"), _T("Nov"), _T("Dec")};

    // find  H:M:S values
    p1 = MIN(timestr.Find(_T(":")), len);
    if (p1 >= 0)
    {     // the time of day is present
        p2 = timestr.ReverseFind(_T(':'));
        CString timestrLeft = timestr.Left(p1);

        if (p1 == p2) // H:M only
        {
            p2 = MAX(timestrLeft.ReverseFind(_T(' ')), 0);
            p3 = MAX(timestr.Find(_T(" "), p1), static_cast<int>(len));
            H = _ttoi(timestr.Mid(p2 + 1, p1 - p2).c_str());
            M = _ttoi(timestr.Mid(p1 + 1, p3 - p1).c_str());
            S = 0;
        }
        else // H:M:S
        {
            p3 = MAX(timestrLeft.ReverseFind(_T(' ')), static_cast<int>(0));
            H = _ttoi(timestr.Mid(p3, p1 - p3).c_str());
            M = _ttoi(timestr.Mid(p1 + 1, p2 - p1).c_str());
            p3 = MAX(timestr.Find(_T(" "), p1), static_cast<int>(len));
            S = _ttoi(timestr.Mid(p2 + 1, p3 - p2).c_str());
        }

    }
    else // no ":" present
        H = M = S = 0;

    // now handle the year, month and  day formats
    p1 = timestr.Find(_T("/"));
    if (p1 >= 0) // "yyyy/mo/da H:M:S"
    {
        p2 = timestr.Find(_T("/"), p1 + 1);
        assert(p2 <= len);  // Invalid time conversion format.

        p3   = MIN(timestr.Find(_T(" "), p2), static_cast<int>(len));
        yyyy = _ttoi(timestr.Mid(0, p1).c_str());
        mo   = _ttoi(timestr.Mid(p1 + 1, p2 - p1 - 1).c_str());
        da   = _ttoi(timestr.Mid(p2 + 1, p3 - p2 - 1).c_str());
        CTime t(yyyy, mo, da, H, M, S, nDST);
        return t;
    }

    p1 = timestr.Find(_T("-"));
    if (p1 >= 0)  // "da-Mon-yyyy H:M:D"
    {
        p2 = timestr.Find(_T("-"), p1 + 1);
        assert(p2 <= len);  // Invalid time conversion format.

        p3   = MIN(timestr.Find(_T(" "), p2), static_cast<int>(len));
        da   = _ttoi(timestr.Mid(0, p1).c_str());
        CString mon  = timestr.Mid(p1 + 1, p2 - p1 - 1);
        yyyy = _ttoi(timestr.Mid(p2 + 1, p3 - p2).c_str());
        for (mo = 0; mo < 12; mo++)
            if (mon == AbMonth[mo])
                break;
        assert(mo != 12);  // Invalid time conversion format.

        mo++;
        CTime t(yyyy, mo, da, H, M, S, nDST);
        return t;
    }

    p2 = timestr.Find(_T(", "));
    if (p2 >= 0)  // "Month da, yyyy H:M:S"
    {
        p1 = timestr.Find(_T(" "));
        assert(p1 <= p2);  // Invalid time conversion format.

        p3   = MIN(timestr.Find(_T(" "), p2 + 2), static_cast<int>(len));
        CString month = timestr.Mid(0, p1);
        da   = _ttoi(timestr.Mid(p1 + 1, p2 - p1 - 1).c_str());
        yyyy = _ttoi(timestr.Mid(p2 + 1, p3 - p2 - 1).c_str());
        for (mo = 0; mo < 12; mo++)
            if (month == Month[mo])
                break;
        assert(mo != 12);  // Invalid time conversion format.

        mo++;
        CTime t(yyyy, mo, da, H, M, S, nDST);
        return t;
    }

    p1 = timestr.Find(_T("+"));
    assert(p1 >= 0);  // Invalid time conversion format.
    if (p1 >= 0)  // "yyyy+doy H:M:S"
    {
        p2 = MIN(timestr.Find(_T(" ")), static_cast<int>(len));
        yyyy = _ttoi(timestr.Mid(0, p1).c_str());
        doy  = _ttoi(timestr.Mid(p1 + 1, p2 - p1 - 1).c_str());
        CTime t(yyyy, doy, H, M, S, nDST);
        return t;
    }
    return CTime(0);
}

/*============================================================================*/
    void  CDoc::
PushContent(const CString &s)                                            /*

    Insert the CString s at the bottom of the document content list  and
    adjust the document maximum width indicator to accommodate s, if
    necessary.
*-----------------------------------------------------------------------------*/
{
    m_docContent.push_back(s);
    m_docWidth = MAX(m_docWidth, s.GetLength());
}

/*============================================================================*/
    bool CDoc::
SaveDoc()                                                               /*

    Save current CString array of the document into the currently named
    source file path. Return true if able to do so, or false otherwise.
*-----------------------------------------------------------------------------*/
{
      // if the current document was created as a new document that has
      // not yet been saved, give it a name and then save it.
    if (m_docPath.IsEmpty())
    {
        m_docPath =
            GetDocSaveFileName(_T("Save the current document as..."));
    }
    if (m_docPath.IsEmpty())
        return false;

    try
    {
        CArchive ar(m_docPath, CArchive::store);
        ar << *this;
    }

    catch (const CException& e)
    {     // Process the exception and  quit
        CString msg;
        CString what(e.what());
        msg.Format(_T("Error while saving document:\n%s\n%s"),
            e.GetText(), e.GetErrorString());
        ::MessageBox(NULL, msg.c_str(), what.c_str(),
            MB_OK | MB_ICONSTOP | MB_TASKMODAL);
        return false;
    }
    catch(...)
    {
        CString msg = _T("Error while saving document:\n");
        ::MessageBox(NULL, msg.c_str(), _T("Exception"),
            MB_OK | MB_ICONSTOP | MB_TASKMODAL);
        return false;
    }
    return true;
}

/*============================================================================*/
    bool CDoc::
SaveDocAs()                                                             /*

    Get a new name for the document file and replace the old one with
    this.  Save the newly named document and retain it as the current
    document in memory. Return true if able to do so, false otherwise.
*-----------------------------------------------------------------------------*/
{
    CString str = GetDocSaveFileName( _T("Save file as..."));
      // check for user bail-out
    if (str.IsEmpty())
        return false;

      // register the new name and save the document under its new name
    m_docPath = str;
    return SaveDoc();
}

/*============================================================================*/
    void CDoc::
Serialize(CArchive &ar)                                                 /*

        Serialize or deserialize the CDoc document from the archive ar,
    depending on the sense of ar.IsStoring().  Throw exceptions during
    reading or writing if the process fails at any point.  Return true
    if all goes well, or false if it didn't.
*-----------------------------------------------------------------------------*/
{
        if (ar.IsStoring())
        {     // storing the document
        UINT n = static_cast<UINT>(m_docContent.size());
          // record the number of records to save
        ar << n;
          // save the records
        for (UINT i = 0; i < n; i++)
            ar << m_docContent[i];
    }
        else // recovering
        {
          // recover the number of records
        UINT n;
        ar >> n;
          // read in the individual records
        CString s;
        for (UINT i = 0; i < n; i++)
        {
            ar >> s;
            PushContent(s);
        }
    }
}
/*----------------------------------------------------------------------------*/
