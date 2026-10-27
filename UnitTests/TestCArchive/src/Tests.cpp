#include "pch.h"             // Include the entire Win32++ library.
#include <iostream>
#include "Tests.h"

using namespace std;

bool DoTests()
{
    const CString fileName(L"ArchiveTest.dat");

    // A set of values we store and retrieve from the archive.
    constexpr BYTE byteTest = 0xF;
    constexpr WORD wordTest = 0xFFFF;
    constexpr LONG longTest = 2049L;
    constexpr LONGLONG llTest = 9220000000000000000LL;
    constexpr ULONGLONG ullTest = 18400000000000000000ULL;
    constexpr DWORD dwordTest = 0xFFFFFFFF;
    constexpr float floatTest = 3.1415927f;
    constexpr double doubleTest = 3.14159265359;
    constexpr int intTest = -42;
    constexpr short shortTest = 257;
    constexpr char charTest = 'H';
    constexpr wchar_t wcharTest = L'\u03B8'; //'θ'
    constexpr unsigned unsignedTest = 87654321;
    constexpr bool boolTest = true;

    const CStringA stringATest("ANSI string with \0 an embedded null", 35);
    const CStringW stringWTest(L"αβγδεζηθικλμνξοπρςστυφχψωϊϋ with \0 an embedded null", 52);
    const CString stringTest("ANSI string converted to TCHAR with \0 an embedded null", 55);
    constexpr POINT pointTest = {12, 15};
    constexpr RECT rectTest = {32, 65, 48, 0};
    constexpr SIZE sizeTest = {348, 26};

    Data data = { pointTest, rectTest, sizeTest };
    ArchiveObject archiveTest(&data, sizeof(data));

    // Store values in the file archive.
    {
        CArchive arWrite(fileName, CArchive::store);

        arWrite << byteTest;
        arWrite << wordTest;
        arWrite << longTest;
        arWrite << llTest;
        arWrite << ullTest;
        arWrite << dwordTest;
        arWrite << floatTest;
        arWrite << doubleTest;
        arWrite << intTest;
        arWrite << shortTest;
        arWrite << charTest;
        arWrite << wcharTest;
        arWrite << unsignedTest;
        arWrite << boolTest;
        arWrite << stringATest;
        arWrite << stringWTest;
        arWrite << stringTest;
        arWrite << pointTest;
        arWrite << rectTest;
        arWrite << sizeTest;
        arWrite << archiveTest;
    }

    bool pass = true;

    // Retrieve the values from a file archive.
    {
        CArchive arRead(fileName, CArchive::load);

        BYTE byteRead;
        WORD wordRead;
        LONG longRead;
        LONGLONG llRead;
        ULONGLONG ullRead;
        DWORD dwordRead;
        float floatRead;
        double doubleRead;
        int intRead;
        short shortRead;
        char charRead;
        wchar_t wcharRead;
        unsigned unsignedRead;
        bool boolRead;
        CStringA stringARead;
        CStringW stringWRead;
        CString stringRead;
        POINT pointRead;
        RECT rectRead;
        SIZE sizeRead;
        Data dataRead = {};
        ArchiveObject archiveRead(&dataRead, sizeof(dataRead));

        arRead >> byteRead;
        arRead >> wordRead;
        arRead >> longRead;
        arRead >> llRead;
        arRead >> ullRead;
        arRead >> dwordRead;
        arRead >> floatRead;
        arRead >> doubleRead;
        arRead >> intRead;
        arRead >> shortRead;
        arRead >> charRead;
        arRead >> wcharRead;
        arRead >> unsignedRead;
        arRead >> boolRead;
        arRead >> stringARead;
        arRead >> stringWRead;
        arRead >> stringRead;
        arRead >> pointRead;
        arRead >> rectRead;
        arRead >> sizeRead;
        arRead >> archiveRead;

        (byteRead == byteTest) ? cout << "Passed: byte test\n" : cout << "*** Failed: *** byte test\n";
        (wordRead == wordTest) ? cout << "Passed: word test\n" : cout << "*** Failed: *** word test\n";
        (longRead == longTest) ? cout << "Passed: long test\n" : cout << "*** Failed: *** long test\n";
        (llRead == llTest) ? cout << "Passed: longlong test\n" : cout << "*** Failed: *** longlong test\n";
        (ullRead == ullTest) ? cout << "Passed: ulonglong test\n" : cout << "*** Failed: *** ulonglong test\n";
        (dwordRead == dwordTest) ? cout << "Passed: dword test\n" : cout << "*** Failed: *** dword test\n";
        (floatRead == floatTest) ? cout << "Passed: float test\n" : cout << "*** Failed: *** float test\n";
        (intRead == intTest) ? cout << "Passed: int test\n" : cout << "*** Failed: *** int test\n";
        (shortRead == shortTest) ? cout << "Passed: short test\n" : cout << "*** Failed: *** short test\n";
        (charRead == charTest) ? cout << "Passed: char test\n" : cout << "*** Failed: *** char test\n";
        (wcharRead == wcharTest) ? cout << "Passed: wchar test\n" : cout << "*** Failed: *** wchar test\n";
        (unsignedRead == unsignedTest) ? cout << "Passed: unsigned test\n" : cout << "*** Failed: *** unsigned test\n";
        (stringARead == stringATest) ? cout << "Passed: CStringA test\n" : cout << "*** Failed: *** CStringA test\n";
        (stringWRead == stringWTest) ? cout << "Passed: CStringW test\n" : cout << "*** Failed: *** CStringW test\n";
        (stringRead == stringTest) ? cout << "Passed: CString test\n" : cout << "*** Failed: *** CString test\n";
        (CPoint(pointRead) == pointTest) ? cout << "Passed: POINT test\n" : cout << "*** Failed: *** POINT test\n";
        (CRect(rectRead) == rectTest) ? cout << "Passed: RECT test\n" : cout << "*** Failed: *** RECT test\n";
        (CSize(sizeRead) == sizeTest) ? cout << "Passed: SIZE test\n" : cout << "*** Failed: *** SIZE test\n";
        constexpr Data dataTest = { pointTest, rectTest, sizeTest };
        (memcmp(&dataRead, &dataTest, sizeof(Data)) == 0) ? cout << "Passed: ArchiveObject test\n" : cout << "*** Failed: *** ArchiveObject test\n";

        pass = (byteRead == byteTest);
        pass = pass && (wordRead == wordTest);
        pass = pass && (longRead == longTest);
        pass = pass && (llRead == llTest);
        pass = pass && (ullRead == ullTest);
        pass = pass && (dwordRead == dwordTest);
        pass = pass && (floatRead == floatTest);
        pass = pass && (intRead == intTest);
        pass = pass && (shortRead == shortTest);
        pass = pass && (charRead == charTest);
        pass = pass && (wcharRead == wcharTest);
        pass = pass && (unsignedRead == unsignedTest);
        pass = pass && (stringARead == stringATest);
        pass = pass && (stringWRead == stringWTest);
        pass = pass && (stringRead == stringTest);
        pass = pass && (CPoint(pointRead) == pointTest);
        pass = pass && (CRect(rectRead) == rectTest);
        pass = pass && (memcmp(&dataRead, &dataTest, sizeof(Data)) == 0);
    }

    return pass;
}