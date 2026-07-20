
#include "pch.h"
#include "Test.h"
#include <iostream>


void RunTests()
{
    using std::cout;

    cout << "Construct test        "; Construct() ?        cout << ("Passed\n") : cout << ("***Failed***\n");
    cout << "GetHandle test        "; GetHandle() ?        cout << ("Passed\n") : cout << ("***Failed***\n");
    cout << "GetFileDirectory test "; GetFileDirectory() ? cout << ("Passed\n") : cout << ("***Failed***\n");
    cout << "GetFileName test      "; GetFileName() ?      cout << ("Passed\n") : cout << ("***Failed***\n");
    cout << "GetFileNameExt test   "; GetFileNameExt() ?   cout << ("Passed\n") : cout << ("***Failed***\n");
    cout << "GetFileNameWOExt test "; GetFileNameWOExt() ? cout << ("Passed\n") : cout << ("***Failed***\n");
    cout << "GetFilePath test      "; GetFilePath() ?      cout << ("Passed\n") : cout << ("***Failed***\n");
    cout << "GetFileTitle test     "; GetFileTitle() ?     cout << ("Passed\n") : cout << ("***Failed***\n");
    cout << "GetFilePosition test  "; GetFilePosition() ?  cout << ("Passed\n") : cout << ("***Failed***\n");
    cout << "GetLength test        "; GetLength() ?        cout << ("Passed\n") : cout << ("***Failed***\n");
    cout << "OperatorHandle test   "; OperatorHandle() ?   cout << ("Passed\n") : cout << ("***Failed***\n");
    cout << "SetFilePath test      "; SetFilePath() ?      cout << ("Passed\n") : cout << ("***Failed***\n");
    cout << "SetFileLenght test    "; SetFileLength() ?    cout << ("Passed\n") : cout << ("***Failed***\n");
    cout << "Close test            "; Close() ?            cout << ("Passed\n") : cout << ("***Failed***\n");
    cout << "Flush test            "; Flush() ?            cout << ("Passed\n") : cout << ("***Failed***\n");
    cout << "LockRange test        "; LockRange() ?        cout << ("Passed\n") : cout << ("***Failed***\n");
    cout << "Open test             "; Open() ?             cout << ("Passed\n") : cout << ("***Failed***\n");
    cout << "Read test             "; Read() ?             cout << ("Passed\n") : cout << ("***Failed***\n");
    cout << "Remove test           "; Remove() ?           cout << ("Passed\n") : cout << ("***Failed***\n");
    cout << "Rename test           "; Rename() ?           cout << ("Passed\n") : cout << ("***Failed***\n");
    cout << "Seek test             "; Seek() ?             cout << ("Passed\n") : cout << ("***Failed***\n");
    cout << "SeekToBegin test      "; SeekToBegin() ?      cout << ("Passed\n") : cout << ("***Failed***\n");
    cout << "SeekToEnd test        "; SeekToEnd() ?        cout << ("Passed\n") : cout << ("***Failed***\n");
    cout << "UnlockRange test      "; UnlockRange() ?      cout << ("Passed\n") : cout << ("***Failed***\n");
    cout << "Write test            "; Write() ?            cout << ("Passed\n") : cout << ("***Failed***\n");
}

bool Construct()
{
    try
    {
        CString test1("C:\\Temp\\test1");
        CString test2("C:\\Temp\\test2");
        CFile file1;
        file1.Open(test1, CFile::modeNoTruncate | CFile::modeRead | CFile::shareDenyNone);

        CFile file2(test1, CFile::modeNoTruncate | CFile::shareDenyNone);

        file1.Close();
        file2.Close();
        file2.Remove(test1);

        file1.Open(test1, CREATE_ALWAYS);
        file1.Close();
        file2.Rename(test1, test2);
        file2.Remove(test2);
    }

    catch (const CFileException& e)
    {
        CString text;
        Trace("An exception occurred\n");
        text << e.GetFilePath() << "   " << e.GetError() << "\n\n";
        Trace(text);
        return false;
    }

    return true;
}

bool GetHandle()
{
    try
    {
        CString test1("C:\\Temp\\test1");
        CFile file1(test1, CFile::modeNoTruncate | CFile::modeRead | CFile::shareDenyNone);

        if ((file1.GetHandle() == 0) || (file1.GetHandle() == INVALID_HANDLE_VALUE))
            throw CFileException(_T("GetHandle failed"));
    }

    catch (CFileException& e)
    {
        CString text;
        Trace("An exception occurred\n");
        text << e.GetFilePath() << "   " << e.GetError() << "\n\n";
        Trace(text);
        return false;
    }

    return true;
}

bool GetFileDirectory()
{
    try
    {
        CString test1("C:\\Temp\\test1");
        CFile file1(test1, CFile::modeNoTruncate | CFile::modeRead | CFile::shareDenyNone);

        CString text = file1.GetFileDirectory();
        return (text == _T("C:\\Temp"));
    }

    catch (CFileException& e)
    {
        CString text;
        Trace("An exception occurred\n");
        text << e.GetFilePath() << "   " << e.GetError() << "\n\n";
        Trace(text);
        return false;
    }

}

bool GetFileName()
{
    try
    {
        CString test1("C:\\Temp\\test1");
        CFile file1(test1, CFile::modeNoTruncate | CFile::modeRead | CFile::shareDenyNone);

        CString text = file1.GetFileName();
        bool pass = (text == _T("test1"));

        file1.Close();
        test1 = "C:\\Temp\\test1.txt";
        file1.Open(test1, CFile::modeNoTruncate | CFile::modeRead | CFile::shareDenyNone);
        text = file1.GetFileName();
        pass = pass && (text == _T("test1.txt"));
        return pass;
    }

    catch (CFileException& e)
    {
        CString text;
        Trace("An exception occurred\n");
        text << e.GetFilePath() << "   " << e.GetError() << "\n\n";
        Trace(text);
        return false;
    }
}

bool GetFileNameExt()
{
    try
    {
        CString test1("C:\\Temp\\test1");
        CFile file1(test1, CFile::modeNoTruncate | CFile::modeRead | CFile::shareDenyNone);

        CString text = file1.GetFileNameExt();
        bool pass = (text == _T(""));

        file1.Close();
        test1 = "C:\\Temp\\test1.txt";
        file1.Open(test1, CFile::modeNoTruncate | CFile::modeRead | CFile::shareDenyNone);
        text = file1.GetFileNameExt();
        pass = pass && (text == _T("txt"));
        return pass;
    }

    catch (CFileException& e)
    {
        CString text;
        Trace("An exception occurred\n");
        text << e.GetFilePath() << "   " << e.GetError() << "\n\n";
        Trace(text);
        return false;
    }
}

bool GetFileNameWOExt()
{
    try
    {
        CString test1("C:\\Temp\\test1");
        CFile file1(test1, CFile::modeNoTruncate | CFile::modeRead | CFile::shareDenyNone);

        CString text = file1.GetFileNameWOExt();
        bool pass = (text == _T("test1"));

        file1.Close();
        test1 = "C:\\Temp\\test1.txt";
        file1.Open(test1, CFile::modeNoTruncate | CFile::modeRead | CFile::shareDenyNone);
        text = file1.GetFileNameWOExt();
        pass = pass && (text == _T("test1"));
        return pass;
    }

    catch (CFileException& e)
    {
        CString text;
        Trace("An exception occurred\n");
        text << e.GetFilePath() << "   " << e.GetError() << "\n\n";
        Trace(text);
        return false;
    }
}

bool GetFilePath()
{
    try
    {
        CString test1("C:\\Temp\\test1");
        CFile file1(test1, CFile::modeNoTruncate | CFile::modeRead | CFile::shareDenyNone);

        CString text = file1.GetFilePath();
        bool pass = (text == test1);

        file1.Close();
        test1 = "C:\\Temp\\test1.txt";
        file1.Open(test1, CFile::modeNoTruncate | CFile::modeRead | CFile::shareDenyNone);
        text = file1.GetFilePath();
        pass = pass && (text == test1);
        return pass;
    }

    catch (CFileException& e)
    {
        CString text;
        Trace("An exception occurred\n");
        text << e.GetFilePath() << "   " << e.GetError() << "\n\n";
        Trace(text);
        return false;
    }
}

bool GetFileTitle()
{
    try
    {
        CString test1("C:\\Temp\\test1");
        CFile file1(test1, CFile::modeNoTruncate | CFile::modeRead | CFile::shareDenyNone);

        CString text = file1.GetFileTitle();
        bool pass = (text == _T("test1"));

        file1.Close();
        test1 = "C:\\Temp\\test1.txt";
        file1.Open(test1, CFile::modeNoTruncate | CFile::modeRead | CFile::shareDenyNone);
        text = file1.GetFileTitle();
        pass = pass && ((text == _T("test1")) || (text == _T("test1.txt")));
        return pass;
    }

    catch (CFileException& e)
    {
        CString text;
        Trace("An exception occurred\n");
        text << e.GetFilePath() << "   " << e.GetError() << "\n\n";
        Trace(text);
        return false;
    }
}

bool GetFilePosition()
{
    try
    {
        CString test1("C:\\Temp\\test1");
        CFile file1(test1, CFile::modeNoTruncate | CFile::modeRead | CFile::shareDenyNone);

        ULONGLONG pos = file1.GetPosition();
        bool pass = (pos == 0);

        return pass;
    }

    catch (CFileException& e)
    {
        CString text;
        Trace("An exception occurred\n");
        text << e.GetFilePath() << "   " << e.GetError() << "\n\n";
        Trace(text);
        return false;
    }
}

bool GetLength()
{
    try
    {
        CString test1("C:\\Temp\\test1");
        CFile file1(test1, CFile::modeNoTruncate | CFile::modeRead | CFile::shareDenyNone);

        ULONGLONG pos = file1.GetLength();
        bool pass = (pos == 0);

        return pass;
    }

    catch (CFileException& e)
    {
        CString text;
        Trace("An exception occurred\n");
        text << e.GetFilePath() << "   " << e.GetError() << "\n\n";
        Trace(text);
        return false;
    }
}

bool OperatorHandle()
{
    try
    {
        CString test1("C:\\Temp\\test1");

        HANDLE hfile = ::CreateFile(test1, GENERIC_READ, 0, 0, CREATE_ALWAYS, 0, 0);

        CFile file2(hfile);
    }

    catch (CFileException& e)
    {
        CString text;
        Trace("An exception occurred\n");
        text << e.GetFilePath() << "   " << e.GetError() << "\n\n";
        Trace(text);
        return false;
    }

    return true;
}

bool SetFilePath()
{
    bool passed;
    try
    {
        CString test1("C:\\Temp\\test1.txt");
        CFile file1;
        file1.SetFilePath(test1);

        passed = file1.GetFilePath() == test1;
        passed = passed && file1.GetFileName() == _T("test1.txt");
        passed = passed && file1.GetFileNameWOExt() == _T("test1");
        passed = passed && file1.GetFileNameExt() == _T("txt");
    }

    catch (CFileException& e)
    {
        CString text;
        Trace("An exception occurred\n");
        text << e.GetFilePath() << "   " << e.GetError() << "\n\n";
        Trace(text);
        passed = false;
    }

    return passed;
}

bool SetFileLength()
{
    bool passed;
    try
    {
        CString test1("C:\\Temp\\test1.txt");
        CFile file1(test1, CREATE_ALWAYS);
        file1.SetLength(200);

        ULONGLONG pos = file1.SeekToEnd();
        passed = (pos == 200);
    }

    catch (CFileException& e)
    {
        CString text;
        Trace("An exception occurred\n");
        text << e.GetFilePath() << "   " << e.GetError() << "\n\n";
        Trace(text);
        passed = false;
    }

    return passed;
}

bool Close()
{
    try
    {
        CString test1("C:\\Temp\\test1.txt");
        CFile file1(test1, CREATE_ALWAYS);
        file1.Close();

        // Would fail without Close
        CFile file2(test1, CREATE_ALWAYS);
    }

    catch (CFileException& e)
    {
        CString text;
        Trace("An exception occurred\n");
        text << e.GetFilePath() << "   " << e.GetError() << "\n\n";
        Trace(text);
        return false;
    }

    return true;
}

bool Flush()
{
    try
    {
        CString test1("C:\\Temp\\test1.txt");
        CFile file1(test1, CREATE_ALWAYS);
        file1.Flush();
    }

    catch (CFileException& e)
    {
        CString text;
        Trace("An exception occurred\n");
        text << e.GetFilePath() << "   " << e.GetError() << "\n\n";
        Trace(text);
        return false;
    }

    return true;
}

bool LockRange()
{
    {
        bool passed;
        try
        {
            CString test1("C:\\Temp\\test1.txt");
            CFile file1(test1, CREATE_ALWAYS);
            file1.SetLength(200);

            file1.LockRange(0, 200);
            file1.UnlockRange(0, 200);
            passed = true;
        }

        catch (CFileException& e)
        {
            CString text;
            Trace("An exception occurred\n");
            text << e.GetFilePath() << "   " << e.GetError() << "\n\n";
            Trace(text);
            passed = false;
        }

        return passed;
    }
}

bool Open()
{
    bool passed;
    try
    {
        CString test1("C:\\Temp\\test1.txt");
        CFile file1;
        file1.Open(test1, CREATE_ALWAYS);
        passed = true;
    }

    catch (CFileException& e)
    {
        CString text;
        Trace("An exception occurred\n");
        text << e.GetFilePath() << "   " << e.GetError() << "\n\n";
        Trace(text);
        passed = false;
    }

    return passed;

}

bool Read()
{
    // Write to file first
    Write();

    // Now use CArchive to read.
    CString fileName = "C:\\Temp\\test.ar";
    CArchive ar(fileName, CArchive::load);
    CString test;
    ar >> test;

    return test == _T("This is a test");
}

bool Remove()
{
    bool passed;
    try
    {
        CString test1("C:\\Temp\\test1.txt");
        CFile file1(test1, CREATE_ALWAYS);
        file1.Close();
        file1.Remove(test1);
        file1.Open(test1, CREATE_NEW);
        passed = true;
    }

    catch (CFileException& e)
    {
        CString text;
        Trace("An exception occurred\n");
        text << e.GetFilePath() << "   " << e.GetError() << "\n\n";
        Trace(text);
        passed = false;
    }

    return passed;
}

bool Rename()
{
    bool passed;
    try
    {
        CString test1("C:\\Temp\\test1.txt");
        CString test2("C:\\Temp\\test2.txt");

        CFile file1(test1, CREATE_ALWAYS);
        CFile file2(test2, CREATE_ALWAYS);
        file2.Close();
        file2.Remove(test2);
        file1.Close();
        file1.Rename(test1, test2);
        passed = (::GetFileAttributes(test2) != INVALID_FILE_ATTRIBUTES);
        file1.Remove(test2);
    }

    catch (CFileException& e)
    {
        CString text;
        Trace("An exception occurred\n");
        text << e.GetFilePath() << "   " << e.GetError() << "\n\n";
        Trace(text);
        passed = false;
    }

    return passed;
}

bool Seek()
{
    bool passed;
    try
    {
        CString test1("C:\\Temp\\test1.txt");
        CFile file1(test1, CREATE_ALWAYS);
        file1.SetLength(200);

        ULONGLONG pos = file1.Seek(100, FILE_BEGIN);
        passed = (pos == 100);
        pos = file1.Seek(50, FILE_CURRENT);
        passed = (passed && (pos == 150));

    }

    catch (CFileException& e)
    {
        CString text;
        Trace("An exception occurred\n");
        text << e.GetFilePath() << "   " << e.GetError() << "\n\n";
        Trace(text);
        return false;
    }

    return passed;
}

bool SeekToBegin()
{
    bool passed;
    try
    {
        CString test1("C:\\Temp\\test1.txt");
        CFile file1(test1, CREATE_ALWAYS);
        file1.SetLength(200);

        ULONGLONG pos = file1.Seek(100, FILE_BEGIN);
        passed = pos == 100;
        pos = file1.Seek(50, FILE_CURRENT);
        passed = (passed && (pos == 150));
        file1.SeekToBegin();
        passed = (file1.GetPosition() == 0);
    }

    catch (CFileException& e)
    {
        CString text;
        Trace("An exception occurred\n");
        text << e.GetFilePath() << "   " << e.GetError() << "\n\n";
        Trace(text);
        return false;
    }

    return passed;
}

bool SeekToEnd()
{
    bool passed;
    try
    {
        CString test1("C:\\Temp\\test1.txt");
        CFile file1(test1, CREATE_ALWAYS);
        file1.SetLength(200);

        ULONGLONG pos = file1.Seek(100, FILE_BEGIN);
        passed = pos == 100;
        pos = file1.Seek(50, FILE_CURRENT);
        passed = passed && (pos == 150);
        file1.SeekToEnd();
        passed = file1.GetPosition() == 200;
    }

    catch (CFileException& e)
    {
        CString text;
        Trace("An exception occurred\n");
        text << e.GetFilePath() << "   " << e.GetError() << "\n\n";
        Trace(text);
        return false;
    }

    return passed;
}

bool UnlockRange()
{
    {
        bool passed;
        try
        {
            CString test1("C:\\Temp\\test1.txt");
            CFile file1(test1, CREATE_ALWAYS);
            file1.SetLength(200);

            file1.LockRange(0, 200);
            file1.UnlockRange(0, 200);
            passed = true;
        }

        catch (CFileException& e)
        {
            CString text;
            Trace("An exception occurred\n");
            text << e.GetFilePath() << "   " << e.GetError() << "\n\n";
            Trace(text);
            return false;
        }

        return passed;
    }
}

bool Write()
{
    CString archive = "C:\\Temp\\test.ar";
    CArchive ar(archive, CArchive::store);

    CString test = "This is a test";
    ar << test;

    return true;
}


