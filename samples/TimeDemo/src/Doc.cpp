/* (06-May-2024) [Tab/Indent: 8/8][Line/Box: 80/74]                  (Doc.cpp) *
********************************************************************************
|                                                                              |
|               Authors: Robert C. Tausworthe, David Nash, 2024                |
|                                                                              |
===============================================================================*

    Contents Description:  Implementation of the CDoc class for this application
    using the Win32++ framework. This particular formulation of the document
    class presumes that the document source is a series of textual lines that
    are accessed and formatted by the methods of this class and passed to the
    CView object for display. In the present usage of this skeleton implementation,
    a document is simulated as the results of computations illustrating the
    capabilities of the CTime class, the numeric version information of the
    Windows system, and the command line parameters.

    Programming Notes: The programming style roughly follows that established
    for the 1995-1999 Jet Propulsion Laboratory Deep Space Network Planning and
    Preparation Subsystem project for C++ programming.

*******************************************************************************/

#include "stdafx.h"
#include "App.h"

  // local formats for displaying CTime values as strings
static const CString longDateFmt = TEXT("%d-%b-%Y [%j] (%a) %H:%M:%S %Z");
static const CString shortDateFmt = TEXT("%d-%b-%Y");
static const CString simpleHMSFmt = TEXT("%I:%M:%S %p");

  // static constant initialization: latest CDoc.cpp file compilation date
const CString CDoc::m_compiledOn = __DATE__;

/*============================================================================*/
    CDoc::
CDoc()                                                                      /*

    Construct the document object and load the document to be initially
    displayed at the start of operations.
*-----------------------------------------------------------------------------*/
{
      // create the document object, set default values, and set the
      // initial document state

    m_fileDlgFilter =
        L"Time Demo Files (*.arc)\0"
        L"*.arc\0"
        L"All Files (*.*)\0"
        L"*.*\0"
        L"\0";

    m_docPath.Empty();
    m_docWidth   = 0;
    m_docContent.clear();
      // show initial document content for display
    NewDocument();
}

/*============================================================================*/
    int CDoc::
GetDocLength() const                                                        /*

    Return the document length, in records.
*----------------------------------------------------------------------------*/
{
    return static_cast<int>(m_docContent.size());
}

/*============================================================================*/
    CString CDoc::
GetDocOpenFileName(const CString &title) const                              /*

    Bring up the open file dialog and get the path of the file to open.
    If none is given, return an empty path. Use the title as the dialog
    window title.
*-----------------------------------------------------------------------------*/
{
    // Bring up the dialog, and  open the file
    CString str;
    DWORD dwFlags = OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT;
    CFileDialog FileDlg(TRUE, nullptr, nullptr, dwFlags, m_fileDlgFilter);
    FileDlg.SetTitle(title);
    if (FileDlg.DoModal() == IDOK)
        str = FileDlg.GetPathName();
    else
        ::MessageBox(nullptr, L"No document name was selected.\n"
            L"The document cannot be opened.",
            L"Information", MB_OK | MB_ICONINFORMATION |
            MB_TASKMODAL);
    return str;
}

/*============================================================================*/
    CString CDoc::
GetDocRecord(int rcd, int left /* = 0 */, int length /* = -1 */) const      /*

    Return a CString containing the document rcd record, starting at
    the left position and continuing for length characters.
*----------------------------------------------------------------------------*/
{
    int size = GetDocLength();
    if (size == 0 || rcd >= size || rcd < 0)
        return L"";

    CString s  = m_docContent[rcd];
      // compute length of text to extract
    if (length < 0)
        length = s.GetLength();
    length = std::max(0, length - left);
      // extract length chars after left position using base class
    return s.Mid(left, length);
}

/*============================================================================*/
    CString CDoc::
GetDocSaveFileName(const CString &title) const                              /*

    Bring up the file save dialog and get the path of file to save the
    document in. If none is selected, return an empty path.  Use the title
    as the window title.
*-----------------------------------------------------------------------------*/
{
    CString str;
    TCHAR extbuff[10];
    DWORD dwFlags = OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT;
    CFileDialog FileDlg(FALSE, extbuff, nullptr, dwFlags, m_fileDlgFilter);
    FileDlg.SetTitle(title);
    if (FileDlg.DoModal() == IDOK)
        str = FileDlg.GetPathName();
    else
        ::MessageBox(nullptr, L"No document name was selected.\n"
            L"The current document will not be saved.",
            L"Information", MB_OK | MB_ICONINFORMATION |
            MB_TASKMODAL);
    return str;
}

/*============================================================================*/
    int CDoc::
GetDocWidth() const                                                         /*

    Return the document width, in characters.
*----------------------------------------------------------------------------*/
{
    return m_docWidth;
}

/*============================================================================*/
    bool CDoc::
OpenDoc(const CString &docFileName)                                         /*

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
        s.Format(L"This file is already open:\n    %s",
            docFileName.c_str());
        ::MessageBox(nullptr, s, L"Error", MB_OK |
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
        msg.Format(L"Error restoring the document.\n%s\n%s",
            e.GetText(), e.GetErrorString());
        ::MessageBox(nullptr, msg.c_str(), what.c_str(),
            MB_OK | MB_ICONSTOP | MB_TASKMODAL);
        return false;
    }
    catch(...) // catch everything else
    {
        CString msg = L"Error restoring the document.\n";
        ::MessageBox(nullptr, msg.c_str(), L"Exception",
            MB_OK | MB_ICONSTOP | MB_TASKMODAL);
        return false;
    }
      // register the open document
    m_docPath = docFileName;
    return true;
}

/*============================================================================*/
    CString CApp::
MakeAppDataPath(const CString & subpath)                                    /*

        Return a string giving the path APPDATA environmental path, with the
        given subpath appended.  Create this path if it does not exist. If
        an error is encountered, throw a user exception.
*-----------------------------------------------------------------------------*/
{
    CString appDataPath = GetAppDataPath();

    if (!appDataPath.IsEmpty())
    {
        appDataPath += L"\\" + subpath;
        ::SHCreateDirectory(nullptr, appDataPath);

        DWORD attributes = GetFileAttributes(appDataPath);
        if ((attributes == INVALID_FILE_ATTRIBUTES) || !(attributes & FILE_ATTRIBUTE_DIRECTORY))
            throw CFileException(appDataPath, L"Failed to access app directory");
    }
    else
        appDataPath = L".";

    return appDataPath;
}

/*============================================================================*/
    void CDoc::
NewDocument()                                                               /*

    For this TimeDemo, develop the ad hoc array of strings that contain the
    results of various tests of the CTime class and Win32++ functions.
*-----------------------------------------------------------------------------*/
{
    m_docContent.clear();
    m_docWidth = 0;
    CString s;
    s = L"  -------------------------------------------------------";
    PushContent(s);
    s.Format(L"    CTime functions test results",
        8 * sizeof(size_t));
    PushContent(s);
    s = L"  -------------------------------------------------------";
    PushContent(s);
    PushContent(L"");
    CTime t1;
    PushContent(L"   1. CTime t1; (long date format)               "
        + t1.Format(longDateFmt));

    CTime t2(t1);
    PushContent(L"   2. CTime t2(t1); (as UTC)                     "
        + t2.FormatGmt(longDateFmt));

    CTime t3(time_t(0));
    PushContent(L"   3. CTime t3(time_t(0)); (short date format)   "
        + t3.FormatGmt(shortDateFmt));
    PushContent(L"   4. CTime t3(time_t(0)); (simple HMS format)   "
        + t3.FormatGmt(simpleHMSFmt));

    t3 = t3.GetCurrentTime();
    PushContent(L"   5. CTime t3 = t3.GetCurrentTime()             "
        + t3.Format(longDateFmt));

    CTime t4(2024, 3, 9, 3, 37, 40);
    PushContent(L"   6. CTime t4(2024, 3, 9, 3, 37, 40);           "
        + t4.Format(longDateFmt));

    CTime t5(2024, 3, 9, 3, 37, 40);
    PushContent(L"   7. CTime t5(2024, 3, 9, 3, 37, 40);           "
        + t5.Format(longDateFmt));

    CTime t6(2024, 3, 9, 3, 37, 40);
    PushContent(L"   8. CTime t6(2024, 3, 9, 3, 37, 40);           "
        + t6.Format(longDateFmt));

    CString s9(L"09-Mar-2024 3:37:40");
    CTime t9(GetTimeFromStr(s9));  // a CString conversion
    PushContent(L"   9. CTime t9(\"09-Mar-2024 3:37:40\")   CString  "
        + t9.Format(longDateFmt));

    CTime t10(GetTimeFromStr(L"09-Mar-2024 3:37:40"));  // a LPCWSTR conversion
    PushContent(L"  10. CTime t10(\"09-Mar-2024 3:37:40\")  LPCWSTR  "
        + t10.Format(longDateFmt));

    SYSTEMTIME st;
    t10.GetAsSystemTime(st);
    CTime t11(st);
    PushContent(L"  11. GetAsSystemTime(t10);                      "
        + t11.Format(longDateFmt));

    FILETIME ft;
    t10.GetAsFileTime(ft);
    CTime t12(ft, 0);
    PushContent(L"  12. GetAsFileTime(t10);                        "
        + t12.Format(longDateFmt));

    WORD fatdate, fattime;
    ::FileTimeToDosDateTime(&ft, &fatdate, &fattime);
    CTime t13(fatdate, fattime, 0);
    PushContent(L"  13. CTime t13(fatdate, fattime)                "
        + t13.Format(longDateFmt));

    int yr = t13.GetYear();
    int mo = t13.GetMonth();
    int da = t13.GetDay();
    int hr = t13.GetHour();
    int mn = t13.GetMinute();
    int sc = t13.GetSecond();
    int dw = t13.GetDayOfWeek();
    int dy = t13.GetDayOfYear();
    CString fmt = L"%04d-%02d-%02d %02d:%02d:%02d [%03d] (%d) ";
    s.Format(fmt + L"local", yr, mo, da, hr, mn, sc, dy, dw);
    PushContent(L"  14. t13, yr-mo-day hr:min:sec [doy] (wk)       "
        + s);

    yr = t13.GetYear(false);
    mo = t13.GetMonth(false);
    da = t13.GetDay(false);
    hr = t13.GetHour(false);
    mn = t13.GetMinute(false);
    sc = t13.GetSecond(false);
    dw = t13.GetDayOfWeek(false);
    dy = t13.GetDayOfYear(false);
    s.Format(fmt + L"UTC", yr, mo, da, hr, mn, sc, dy, dw);
    PushContent("  15. t13, yr-mo-day hr:min:sec [doy] (wk)       "
        + s);

    CTimeSpan ts = t10 - t1;
    s = ts.Format(L"%D days, %H:%M:%S");
    PushContent(L"  16. t10 - t1 =                                 "
        + s);

    s.Format(L"%ld total hrs, %ld min, %ld sec",
        ts.GetTotalHours(), ts.GetTotalMinutes(), ts.GetTotalSeconds());
    PushContent(L"  17. t10 - t1 =                                 "
        + s);

    ts = t1 - t10;
    s = ts.Format(L"%D days, %H:%M:%S");
    PushContent(L"  18. t1 - t10 =                                 "
        + s);

    CTime t16 = t10 + (t1 - t10);
    PushContent(L"  19. t16 = t10 + (t1 - t10) =  t1               "
        + t16.Format(longDateFmt));

    t16 -= (t1 - t10);
    PushContent(L"  20. t16 -= (t1 - t10) =  t10                   "
        + t16.Format(longDateFmt));

    t16 += (t1 - t10);
    PushContent(L"  21. t16 += (t1 - t10) =  t1                    "
        + t16.Format(longDateFmt));

    t16 = t1 - (t1 - t10);
    PushContent(L"  22. t16 = t1 - (t1 - t10) =  t10               "
        + t16.Format(longDateFmt));

    ts = -(t1 - t10);
    s = ts.Format(L"%D days, %H:%M:%S");
    PushContent(L"  23. -(t1 - t10) =                              "
        + s);

    PushContent(L"  24. t5 == t6;                                  "
        + Truth(t5 == t6));

    PushContent(L"  25. t5 != t6;                                  "
        + Truth(t5 != t6));

    PushContent(L"  26. t5 < t6;                                   "
        + Truth(t5 < t6));

    PushContent(L"  27. t5 > t6;                                   "
        + Truth(t5 > t6));

    PushContent(L"");
    s.Format(L"  -------------------------------------------------------");
    PushContent(s);
    s = L"    Platform values for this machine";
    PushContent(s);
    s.Format(L"  -------------------------------------------------------");
    PushContent(s);
    PushContent(L"");
    s.Format(L"  WINVER        = 0x%x", WINVER);
    PushContent(s);

#ifdef WIN32_WINNT
    s.Format(L"  WIN32_WINNT   = 0x%x", _WIN32_WINNT);
    PushContent(s);
#endif

    s.Format(L"  _WIN32_IE     = 0x%x", _WIN32_IE);
    PushContent(s);

#ifdef NTDDI_VERSION
    s.Format(L"  NTDDI_VERSION = 0x%x", NTDDI_VERSION);
    PushContent(s);
#endif

    PushContent(L"");

    s = ::GetCommandLine();
    s.Format(L"  Command line: %s", s.c_str());
    PushContent(s);
    PushContent(L"");

      // Here we process the Win32++ access to command line arguments, and
      // display them among the other tests in this demo. GetCommandLineArgs
      // retrieves our command line arguments and stores them in a vector
      // of CString.
    std::vector<CString> args = GetCommandLineArgs();
      // The second argument (if any) contains this app file name.
    for (size_t i = 0; i < args.size(); i++)
    {
        s.Format(L"  argv(%d) = %s", i, args[i].c_str());
        PushContent(s);
    }

    PushContent(L"");
    s.Format(L"  -------------------------------------------------------");
    PushContent(s);
    s.Format(L"    C++ data type sizes for this %d-bit Windows program.",
        8 * sizeof(size_t));
    PushContent(s);
    s.Format(L"  -------------------------------------------------------");
    PushContent(s);
    PushContent(L"");
    s.Format(L"  char                     %lu bytes", sizeof(char));
    PushContent(s);
    s.Format(L"  short                    %lu bytes", sizeof(short));
    PushContent(s);
    s.Format(L"  int                      %lu bytes", sizeof(int));
    PushContent(s);
    s.Format(L"  long                     %lu bytes", sizeof(long));
    PushContent(s);
    s.Format(L"  LONGLONG                 %lu bytes", sizeof(LONGLONG));
    PushContent(s);
    PushContent(L"");
    s.Format(L"  UINT                     %lu bytes", sizeof(UINT));
    PushContent(s);
    s.Format(L"  TCHAR                    %lu bytes", sizeof(TCHAR));
    PushContent(s);
    s.Format(L"  WORD                     %lu bytes", sizeof(WORD));
    PushContent(s);
    s.Format(L"  DWORD                    %lu bytes", sizeof(DWORD));
    PushContent(s);
    s.Format(L"  size_t                   %lu bytes", sizeof(size_t));
    PushContent(s);
    s.Format(L"  LPWSTR                   %lu bytes", sizeof(LPWSTR));
    PushContent(s);
    s.Format(L"  double                   %lu bytes", sizeof(double));
    PushContent(s);
    PushContent(L"");
    m_docPath.Empty();
}

/*============================================================================*/
    CTime CDoc::
GetTimeFromStr(LPCWSTR szTime, int nDST /* = -1 */) const                   /*

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

    CString Month[] =   {L"January", L"February",
                  L"March", L"April", L"May",
                  L"June", L"July", L"August",
                  L"September", L"October",
                  L"November", L"December"};
    CString AbMonth[] = {L"Jan", L"Feb", L"Mar",
                  L"Apr", L"May", L"Jun",
                  L"Jul", L"Aug", L"Sep",
                  L"Oct", L"Nov", L"Dec"};

    // find  H:M:S values
    p1 = std::min(timestr.Find(L':'), len);
    if (p1 >= 0)
    {     // the time of day is present
        p2 = timestr.ReverseFind(L':');
        CString timestrLeft = timestr.Left(p1);

        if (p1 == p2) // H:M only
        {
            p2 = std::max(timestrLeft.ReverseFind(L' '), 0);
            p3 = std::max(timestr.Find(L" ", p1), len);
            H = _ttoi(timestr.Mid(p2 + 1, p1 - p2).c_str());
            M = _ttoi(timestr.Mid(p1 + 1, p3 - p1).c_str());
            S = 0;
        }
        else // H:M:S
        {
            p3 = std::max(timestrLeft.ReverseFind(L' '), 0);
            H = _ttoi(timestr.Mid(p3, p1 - p3).c_str());
            M = _ttoi(timestr.Mid(p1 + 1, p2 - p1).c_str());
            p3 = std::max(timestr.Find(L' ', p1), len);
            S = _ttoi(timestr.Mid(p2 + 1, p3 - p2).c_str());
        }

    }
    else // no ":" present
        H = M = S = 0;

    // now handle the year, month and  day formats
    p1 = timestr.Find(L'/');
    if (p1 >= 0) // "yyyy/mo/da H:M:S"
    {
        p2 = timestr.Find(L'/', p1 + 1);
        assert(p2 <= len);  // Invalid time conversion format.

        p3   = std::min(timestr.Find(L' ', p2), len);
        yyyy = _ttoi(timestr.Mid(0, p1).c_str());
        mo   = _ttoi(timestr.Mid(p1 + 1, p2 - p1 - 1).c_str());
        da   = _ttoi(timestr.Mid(p2 + 1, p3 - p2 - 1).c_str());
        CTime t(yyyy, mo, da, H, M, S, nDST);
        return t;
    }

    p1 = timestr.Find(L'-');
    if (p1 >= 0)  // "da-Mon-yyyy H:M:D"
    {
        p2 = timestr.Find(L'-', p1 + 1);
        assert(p2 <= len);  // Invalid time conversion format.

        p3   = std::min(timestr.Find(L' ', p2), len);
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

    p2 = timestr.Find(L", ");
    if (p2 >= 0)  // "Month da, yyyy H:M:S"
    {
        p1 = timestr.Find(L' ');
        assert(p1 <= p2);  // Invalid time conversion format.

        p3   = std::min(timestr.Find(L' ', p2 + 2), len);
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

    p1 = timestr.Find(L'+');
    assert(p1 >= 0);  // Invalid time conversion format.
    if (p1 >= 0)  // "yyyy+doy H:M:S"
    {
        p2 = std::min(timestr.Find(L' '), len);
        yyyy = _ttoi(timestr.Mid(0, p1).c_str());
        doy  = _ttoi(timestr.Mid(p1 + 1, p2 - p1 - 1).c_str());
        CTime t(yyyy, doy, H, M, S, nDST);
        return t;
    }
    return CTime(0);
}

/*============================================================================*/
    void  CDoc::
PushContent(const CString &s)                                               /*

    Insert the CString s at the bottom of the document content list  and
    adjust the document maximum width indicator to accommodate s, if
    necessary.
*-----------------------------------------------------------------------------*/
{
    m_docContent.push_back(s);
    m_docWidth = std::max(m_docWidth, s.GetLength());
}

/*============================================================================*/
    bool CDoc::
SaveDoc()                                                                   /*

    Save current CString array of the document into the currently named
    source file path. Return true if able to do so, or false otherwise.
*-----------------------------------------------------------------------------*/
{
      // if the current document was created as a new document that has
      // not yet been saved, give it a name and then save it.
    if (m_docPath.IsEmpty())
    {
        m_docPath =
            GetDocSaveFileName(L"Save the current document as...");
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
        msg.Format(L"Error while saving document:\n%s\n%s",
            e.GetText(), e.GetErrorString());
        ::MessageBox(nullptr, msg.c_str(), what.c_str(),
            MB_OK | MB_ICONSTOP | MB_TASKMODAL);
        return false;
    }
    catch(...)
    {
        CString msg = L"Error while saving document:\n";
        ::MessageBox(nullptr, msg.c_str(), L"Exception",
            MB_OK | MB_ICONSTOP | MB_TASKMODAL);
        return false;
    }
    return true;
}

/*============================================================================*/
    bool CDoc::
SaveDocAs()                                                                 /*

    Get a new name for the document file and replace the old one with
    this.  Save the newly named document and retain it as the current
    document in memory. Return true if able to do so, false otherwise.
*-----------------------------------------------------------------------------*/
{
    CString str = GetDocSaveFileName(L"Save file as...");
      // check for user bail-out
    if (str.IsEmpty())
        return false;

      // register the new name and save the document under its new name
    m_docPath = str;
    return SaveDoc();
}

/*============================================================================*/
    void CDoc::
Serialize(CArchive &ar)                                                     /*

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
