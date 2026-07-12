
#include "stdafx.h"
#include <iostream>
#include "Test.h"
#include "resource.h"


constexpr LPCWSTR textW =   L"0123456789abcdefg";
constexpr LPCSTR  textA =    "0123456789abcdefg";
constexpr LPCTSTR textT = _T("0123456789abcdefg");

// Test concatenation of string with embedded nulls.
static bool Test_Add()
{
    bool allPassed = true;

    CStringA strA("abc\0 456", 8);
    CStringW strW(L"def\0 12345", 10);
    CString  str("xyz\0 2345", 9);

    // CString
    CString str1(strA + strW + str + 'A' + L'B' + _T('C') + "DD" + L"EE" + _T("FF"));
    allPassed = allPassed && (str1 == CString("abc\0 456def\0 12345xyz\0 2345ABCDDEEFF", 36));
    allPassed = allPassed && (str1.GetLength() == 36);

    str1.Empty();
    str1 = strA + strW + str + 'A' + L'B' + _T('C') + "DD" + L"EE" + _T("FF");
    allPassed = allPassed && (str1 == CString("abc\0 456def\0 12345xyz\0 2345ABCDDEEFF", 36));
    allPassed = allPassed && (str1.GetLength() == 36);

    str1.Empty();
    str1 += strA;
    str1 += strW;
    str1 += str;
    str1 += 'A';
    str1 += L'B';
    str1 += _T('C');
    str1 += "DD";
    str1 += L"EE";
    str1 += _T("FF");
    allPassed = allPassed && (str1 == CString("abc\0 456def\0 12345xyz\0 2345ABCDDEEFF", 36));
    allPassed = allPassed && (str1.GetLength() == 36);

    str1.Empty();

    str1 << strA << strW << str << 'A' << L'B' << _T('C') << "DD" << L"EE" << _T("FF");

    allPassed = allPassed && (str1 == CString("abc\0 456def\0 12345xyz\0 2345ABCDDEEFF", 36));
    allPassed = allPassed && (str1.GetLength() == 36);

    // CStringA
    CStringA str2(strA + 'A' + "abc");
    allPassed = allPassed && (str2 == CStringA("abc\0 456Aabc", 12));
    allPassed = allPassed && (str2.GetLength() == 12);

    str2.Empty();
    str2 = strA + 'A' + "abc";
    allPassed = allPassed && (str2 == CStringA("abc\0 456Aabc", 12));
    allPassed = allPassed && (str2.GetLength() == 12);

    str2.Empty();
    str2 += strA;
    str2 += 'A';
    str2 += "abc";
    allPassed = allPassed && (str2 == CStringA("abc\0 456Aabc", 12));
    allPassed = allPassed && (str2.GetLength() == 12);

    str2.Empty();
    str2 << strA << 'A' << "abc";
    allPassed = allPassed && (str2 == CStringA("abc\0 456Aabc", 12));
    allPassed = allPassed && (str2.GetLength() == 12);

    // CStringW
    CStringW str3(strW + L'A' + L"abc");
    allPassed = allPassed && (str3 == CStringW(L"def\0 12345Aabc", 14));
    allPassed = allPassed && (str3.GetLength() == 14);

    str3.Empty();
    str3 = strW + L'A' + L"abc";
    allPassed = allPassed && (str3 == CStringW(L"def\0 12345Aabc", 14));
    allPassed = allPassed && (str3.GetLength() == 14);

    str3.Empty();
    str3 += strW;
    str3 += L'A';
    str3 += L"abc";
    allPassed = allPassed && (str3 == CStringW(L"def\0 12345Aabc", 14));
    allPassed = allPassed && (str3.GetLength() == 14);

    str3.Empty();
    str3 << strW << L'A' << L"abc";
    allPassed = allPassed && (str3 == CStringW(L"def\0 12345Aabc", 14));
    allPassed = allPassed && (str3.GetLength() == 14);

    return allPassed;
}

static bool Test_AllocSysString()
{
    CStringW strW(textW);
    BSTR bstrW = strW.AllocSysString();

    CStringA strA(textA);
    BSTR bstrA = strA.AllocSysString();

    CString str(textT);
    BSTR bstr = str.AllocSysString();

    bool allPassed = (0 == wcscmp(bstrW, textW));
    allPassed = allPassed && (0 == wcscmp(bstrA, AtoW(textA)));
    allPassed = allPassed && (0 == wcscmp(bstrA, TtoW(textT)));

    CStringA testA(textA);
    CStringW testW(textW);
    CString test(textT);

    testA += testA;
    testW += testW;
    test += test;

    testA.SetSysString(&bstrA);
    testW.SetSysString(&bstrW);
    test.SetSysString(&bstr);

    allPassed = allPassed && (strA + strA == testA);
    allPassed = allPassed && (strW + strW == testW);
    allPassed = allPassed && (str  + str  == test);

    SysFreeString(bstrW);
    SysFreeString(bstrA);
    SysFreeString(bstr);

    return allPassed;
}

static bool Test_AppendFormat()
{
    CStringA strA(textA);
    CStringW strW(textW);
    CString str(textT);

    CStringA strTestA = strA + strA;
    CStringW strTestW = strW + strW;
    CString  strTest  = str + str;

    strA.AppendFormat("%s", textA);
    strW.AppendFormat(L"%s", textW);
    str.AppendFormat(_T("%s"), textT);

    bool allPassed = (strA == strTestA);
    allPassed = allPassed && (strW == strTestW);
    allPassed = allPassed && (str == strTest);
    return allPassed;
}

static bool Test_Assign()
{
    LPCSTR textA1 = "0123456789\0abcdefghi";
    LPCWSTR textW1 = L"0123456789\0abcdefghi";
    LPCTSTR textT1 = _T("0123456789\0abcdefghi");

    CStringA strA(textA1, 20);
    CStringW strW(textW1, 20);
    CString str(textT1, 20);;

    CStringA strTestA;
    CStringW strTestW;
    CString strTest;

    strTestA.Assign(textA1, 20);
    strTestW.Assign(textW1, 20);
    strTest.Assign(textT1, 20);

    bool allPassed = true;

    // Verify assignment.
    allPassed = allPassed && (strA == strTestA);
    allPassed = allPassed && (strW == strTestW);
    allPassed = allPassed && (str == strTest);

    // Very correct length.
    allPassed = allPassed && (strA.GetLength() == 20);
    allPassed = allPassed && (strTestA.GetLength() == 20);
    allPassed = allPassed && (strW.GetLength() == 20);
    allPassed = allPassed && (strTestW.GetLength() == 20);
    allPassed = allPassed && (str.GetLength() == 20);
    allPassed = allPassed && (strTest.GetLength() == 20);

    // Verify assignment with embedded null.
    allPassed = allPassed && (strA != CStringA(textA1));   // != passes because textA1 has an embedded null.
    allPassed = allPassed && (strW != CStringW(textW1));   // != passes because textW1 has an embedded null.
    allPassed = allPassed && (str != CString(textT1));     // != passes because textT1 has an embedded null.
    allPassed = allPassed && (strA == CStringA(textA1, 20));
    allPassed = allPassed && (strW == CStringW(textW1, 20));
    allPassed = allPassed && (str == CString(textT1,   20));

    return allPassed;
}

static bool Test_BigBuffer()
{
    bool allPassed = true;

    int big = 1000000;
    CString str1(L'A', big);
    CString str2;
    LPTSTR pstr2 = str2.GetBuffer(big);
    StrCopy(pstr2, str1.c_str(), big +1);
    str2.ReleaseBuffer();
    allPassed = allPassed && (str1 == str2);

    CString str3('A', big);
    CString str4;
    LPTSTR pstr4 = str4.GetBuffer(big);
    StrCopy(pstr4, str3.c_str(), big +1);
    str4.ReleaseBuffer();
    allPassed = allPassed && (str3 == str4);

    CStringA strA1(_T('A'), big);
    CStringA strA2;
    LPSTR pstrA2 = strA2.GetBuffer(big);
    StrCopyA(pstrA2, strA1.c_str(), big +1);
    strA2.ReleaseBuffer();
    allPassed = allPassed && (strA1 == strA2);

    CStringW strW1(L'A', big);
    CStringW strW2;
    LPWSTR pstrW2 = strW2.GetBuffer(big);
    StrCopyW(pstrW2, strW1.c_str(), big +1);
    strW2.ReleaseBuffer();
    allPassed = allPassed && (strW1 == strW2);

    return allPassed;
}

static bool Test_Buffer()
{
    CStringA strA;
    CStringW strW;
    CString str;


    int sizeA = lstrlenA(textA);
    int sizeW = lstrlenW(textW);
    int sizeT = lstrlen(textT);

    LPSTR tA = strA.GetBuffer(sizeA);
    LPWSTR tW = strW.GetBuffer(sizeW);
    LPTSTR t = str.GetBuffer(sizeT);

    StrCopyA(tA, textA, sizeA + 1);
    StrCopyW(tW, textW, sizeW + 1);
    StrCopy(t, textT, sizeT + 1);

    // Check default ReleaseBuffer size.
    strA.ReleaseBuffer();
    strW.ReleaseBuffer();
    str.ReleaseBuffer();

    bool allPassed = (0 == strA.Compare(textA));
    allPassed = allPassed && (0 == strW.Compare(textW));
    allPassed = allPassed && (0 == str.Compare(textT));

    tA = strA.GetBuffer(sizeA);
    tW = strW.GetBuffer(sizeW);
    t = str.GetBuffer(sizeT);

    StrCopyA(tA, textA, sizeA + 1);
    StrCopyW(tW, textW, sizeW + 1);
    StrCopy(t, textT, sizeT + 1);

    // Check truncated ReleaseBuffer size.
    strA.ReleaseBuffer(10);
    strW.ReleaseBuffer(10);
    str.ReleaseBuffer(10);

    allPassed = allPassed && (strA == CStringA("0123456789"));
    allPassed = allPassed && (strW == CStringW(L"0123456789"));
    allPassed = allPassed && (str == CString(_T("0123456789")));

    int extra = 5;
    tA = strA.GetBuffer(sizeA + extra);
    tW = strW.GetBuffer(sizeW + extra);
    t = str.GetBuffer(sizeT + extra);

    StrCopyA(tA, textA, sizeA +1 + extra);
    StrCopyW(tW, textW, sizeW +1 + extra);
    StrCopy(t, textT, sizeT + 1 + extra);

    // Check ReleaseBuffer with extra characters.
    strA.ReleaseBuffer(sizeA + extra);
    strW.ReleaseBuffer(sizeW + extra);
    str.ReleaseBuffer(sizeT + extra);

    allPassed = allPassed && (strA == (CStringA("0123456789abcdefg") + CStringA('\0', extra)));
    allPassed = allPassed && (strW == (CStringW(L"0123456789abcdefg") + CStringW(L'\0', extra)));
    allPassed = allPassed && (str == (CString(_T("0123456789abcdefg")) + CString(_T('\0'), extra)));

    return allPassed;
}

static bool Test_Compare()
{
    CStringA strA(textA);
    CStringW strW(textW);
    CString str(textT);

    bool allPassed = (0 == strA.Compare(textA));
    allPassed = allPassed && (0 == strW.Compare(textW));
    allPassed = allPassed && (0 == str.Compare(textT));

    allPassed = allPassed && (0 == strA.Collate(textA));
    allPassed = allPassed && (0 == strW.Collate(textW));
    allPassed = allPassed && (0 == str.Collate(textT));

    strA.MakeUpper();
    strW.MakeUpper();
    str.MakeUpper();

    allPassed = allPassed && (0 == strA.CollateNoCase(textA));
    allPassed = allPassed && (0 == strW.CollateNoCase(textW));
    allPassed = allPassed && (0 == str.CollateNoCase(textT));

    strA.MakeUpper();
    strW.MakeUpper();
    str.MakeUpper();

    strA.MakeLower();
    strW.MakeLower();
    str.MakeLower();

    strA.MakeReverse();
    strW.MakeReverse();
    str.MakeReverse();

    strA.MakeReverse();
    strW.MakeReverse();
    str.MakeReverse();

    allPassed = allPassed && (0 == strA.CompareNoCase(textA));
    allPassed = allPassed && (0 == strW.CompareNoCase(textW));
    allPassed = allPassed && (0 == str.CompareNoCase(textT));

    return allPassed;
}

static bool Test_Construct()
{
    CStringA str1("test");
    CStringW str2(L"test");
    CString  str3("test");
    CString  str4(L"test");

    CStringA str5('h');
    CStringW str6(L'h');
    CString  str7('h');
    CString  str8(L'h');

    CStringA str9('h', 10);
    CStringW strA(L'h', 10);
    CString  strB('h', 10);
    CString  strC(L'h', 10);

    CStringA str10 = ToCStringA(1);
    CStringW str11 = ToCStringW(1);
    CString  str12 = ToCString(1);

    CStringA str13 = ToCStringA(1.3);
    CStringW str14 = ToCStringW(1.3);
    CString  str15 = ToCString(1.3);

    str13 = str13 << 1;
    str14 = str14 << 1;
    str15 = str15 << 1;

    str13 = str13 << 1.1;
    str14 = str14 << 1.1;
    str15 = str15 << 1.1;

    str13 += str13;
    str14 += str14;
    str15 += str15;

    str13 = str13 << 8 << 3.14 << 'p' << "i";
    str14 = str14 << 8 << 3.14 << L'p' << L"i";
    str15 = str15 << 8 << 3.14 << _T('p') << _T("i");

    bool allPassed = (str13 == "1.311.11.311.183.14pi");
    allPassed = allPassed && (str14 == L"1.311.11.311.183.14pi");
    allPassed = allPassed && (str15 == _T("1.311.11.311.183.14pi"));

    return allPassed;
}

static bool Test_Delete()
{
    LPCWSTR text = L"Some Text";
    CString str(text);
    str.Delete(0, 0);
    str.Delete(5, 4);
    str.Insert(5, _T("Text"));
    bool allPassed = (str == WtoT(text));
    return allPassed;
}

static bool Test_Empty()
{
    CStringA strA(textA);
    CStringW strW(textW);
    CString str(textT);

    bool allPassed = !strA.IsEmpty();
    allPassed = allPassed && !strW.IsEmpty();
    allPassed = allPassed && !str.IsEmpty();

    strA.Empty();
    strW.Empty();
    str.Empty();

    allPassed = allPassed && strA.IsEmpty();
    allPassed = allPassed && strW.IsEmpty();
    allPassed = allPassed && str.IsEmpty();

    return allPassed;
}

static bool Test_Equal()
{
    CStringA strA(textA);
    CStringW strW(textW);
    CString str(textT);

    CStringA testA = strA + "xx";
    CStringW testW = strW + L"xx";
    CString testT = str + _T("xx");

    bool allPassed = !(strA == testA);
    allPassed = allPassed && !(strW == testW);
    allPassed = allPassed && !(str == testT);

    allPassed = allPassed && (strA != testA);
    allPassed = allPassed && (strW != testW);
    allPassed = allPassed && (str != testT);

    allPassed = allPassed && (strA < testA);
    allPassed = allPassed && (strA <= testA);
    allPassed = allPassed && (strW < testW);
    allPassed = allPassed && (strW <= testW);

    allPassed = allPassed && !(strA > testA);
    allPassed = allPassed && !(strA >= testA);
    allPassed = allPassed && !(strW > testW);
    allPassed = allPassed && !(strW >= testW);

    return allPassed;
}

static bool Test_Find()
{
    CStringA strA(textA);
    CStringW strW(textW);
    CString str(textT);

    bool allPassed = (10 == strA.Find("abcdef", 3));
    allPassed = allPassed && (10 == strW.Find(L"abcdef", 3));
    allPassed = allPassed && (10 == str.Find(_T("abcdef"), 3));

    allPassed = allPassed && (-1 == strA.Find("acdef", 3));
    allPassed = allPassed && (-1 == strW.Find(L"acdef", 3));
    allPassed = allPassed && (-1 == str.Find(_T("acdef"), 3));

    allPassed = allPassed && (10 == strA.FindOneOf("acf"));
    allPassed = allPassed && (10 == strW.FindOneOf(L"acf"));
    allPassed = allPassed && (10 == str.FindOneOf(_T("acf")));

    allPassed = allPassed && (-1 == strA.FindOneOf("xyz"));
    allPassed = allPassed && (-1 == strW.FindOneOf(L"xyz"));
    allPassed = allPassed && (-1 == str.FindOneOf(_T("xyz")));

    allPassed = allPassed && (10 == strA.ReverseFind("abc"));
    allPassed = allPassed && (10 == strW.ReverseFind(L"abc"));
    allPassed = allPassed && (10 == str.ReverseFind(_T("abc")));

    allPassed = allPassed && (-1 == strA.ReverseFind("acf"));
    allPassed = allPassed && (-1 == strW.ReverseFind(L"acf"));
    allPassed = allPassed && (-1 == str.ReverseFind(_T("acf")));

    return allPassed;
}
static bool Test_Format()
{
    CStringA strA;
    CStringW strW;
    CString str;

    CStringA testA;
    CStringW testW;
    CString test;

    strA.Format("int = %d, text = %s", 20, "some text");
    testA.FormatMessage("int = %1!d!, text = %2!s!", 20,  "some text");
    strW.Format(L"int = %d, text = %s", 20,  L"some text");
    testW.FormatMessage(L"int = %1!d!, text = %2!s!", 20, L"some text");
    str.Format(_T("int = %d, text = %s"), 20, "some text");
    test.FormatMessage(_T("int = %1!d!, text = %2!s!"), 20, "some text");

    bool allPassed = true;
    allPassed = allPassed && (strA == testA);
    allPassed = allPassed && (strW == testW);
    allPassed = allPassed && (str == test);

    return allPassed;
}

static bool Test_Left()
{
    CStringA strA(textA);
    CStringW strW(textW);
    CString str(textT);

    // Check Left.
    CStringA leftA = strA.Left(10);
    CStringW leftW = strW.Left(10);
    CString left = str.Left(10);

    // Check Mid with 2 arguments.
    CStringA midA = strA.Mid(10, 2);
    CStringW midW = strW.Mid(10, 2);
    CString mid = str.Mid(10, 2);

    // Check Mid with 2 argument.
    CStringA midA1 = strA.Mid(12);
    CStringW midW1 = strW.Mid(12);
    CString mid1 = str.Mid(12);

    // Check right.
    CStringA rightA = strA.Right(5);
    CStringW rightW = strW.Right(5);
    CString right = str.Right(5);

    CStringA sumA = leftA + midA + rightA;
    CStringW sumW = leftW + midW + rightW;
    CString sum = left + mid + right;

    bool allPassed = (sumA == strA);
    allPassed = allPassed && (sumW == strW);
    allPassed = allPassed && (sum == str);

    sumA = leftA + midA + midA1;
    sumW = leftW + midW + midW1;
    sum = left + mid + mid1;

    allPassed = allPassed && (sumA == strA);
    allPassed = allPassed && (sumW == strW);
    allPassed = allPassed && (sum == str);

    return allPassed;
}

static bool Test_Plus()
{
    CStringA strA;
    CStringW strW;
    CString  strT;
    bool allPassed = true;

    // Test string + and string += individual character.
    strA = strA + 'a';
    strA += '\0';
    strA = strA + '\0';
    strA += 'b';

    strW = strW + L'a';
    strW += L'\0';
    strW = strW + L'\0';
    strW += L'b';

    strT = strT + 'a';     // ansi
    strT += '\0';          // ansi
    strT = strT + L'\0';   // Unicode
    strT +=  L'b';         // Unicode

    allPassed = allPassed && (strA == CStringA("a\0\0b", 4));
    allPassed = allPassed && (strW == CStringW(L"a\0\0b", 4));
    allPassed = allPassed && (strT == CString(_T("a\0\0b"), 4));

    strA.Empty();
    strW.Empty();
    strT.Empty();

    // Test string + and string += character array.
    strA = strA + "abc";
    strA += '\0';
    strA = strA + '\0';
    strA += "def";

    strW = strW + L"abc";
    strW += L'\0';
    strW = strW + L'\0';
    strW += L"def";

    // CString operators lets us mix ansi and Unicode.
    strT = strT + "abc";    // ansi
    strT += '\0';           // ansi
    strT = strT + L'\0';    // Unicode
    strT += L"def";         // Unicode

    // Verify + and += with null character.
    allPassed = allPassed && (strA == CStringA("abc\0\0def", 8));
    allPassed = allPassed && (strW == CStringW(L"abc\0\0def", 8));
    allPassed = allPassed && (strT == CString(_T("abc\0\0def"), 8));

    strA.Empty();
    strW.Empty();
    strT.Empty();

    // Test character array + string and individual character + string.
    strA += "abc";
    strA = strA + "def";
    strA = strA + strA;
    strA = "123" + strA;
    strA = '0' + strA;

    strW += L"abc";
    strW = strW + L"def";
    strW = strW + strW;
    strW = L"123" + strW;
    strW = L'0' + strW;

    // CString operators lets us mix ansi and Unicode.
    strT += "abc";          // ansi
    strT = strT + L"def";   // Unicode
    strT = strT + strT;
    strT = L"123" + strT;
    strT = '0' + strT;

    allPassed = allPassed && (strA == "0123abcdefabcdef");
    allPassed = allPassed && (strW == L"0123abcdefabcdef");
    allPassed = allPassed && (strT == _T("0123abcdefabcdef"));

    return allPassed;
}

static bool Test_Remove()
{
    CStringA strA(textA);
    CStringW strW(textW);
    CString str(textT);

    int index = (int)strA.Find("abc");
    strA.Remove("abc");
    strA.Insert(index, "abc");
    strA.SetAt(index, 'X');

    strW.Remove(L"abc");
    strW.Insert(index, L"abc");
    strW.SetAt(index, L'X');

    str.Remove(_T("abc"));
    str.Insert(index, _T("abc"));
    str.SetAt(index, _T('X'));

    bool allPassed = (strA.Find('X') == index);
    allPassed = allPassed && (strW.Find(L'X') == index);
    allPassed = allPassed && (str.Find(_T('X')) == index);

    strA[index] = 'a';
    strW[index] = L'a';
    str[index] = _T('a');

    allPassed = allPassed && (strA == textA);
    allPassed = allPassed && (strW == textW);
    allPassed = allPassed && (str  == textT);

    allPassed = allPassed && (strA.GetAt(index) == 'a');
    allPassed = allPassed && (strW.GetAt(index) == L'a');
    allPassed = allPassed && (str.GetAt(index) == _T('a'));

    strA.Remove('a');
    strW.Remove(L'a');
    str.Remove(_T('a'));

    allPassed = allPassed && (strA.GetAt(index) == 'b');
    allPassed = allPassed && (strW.GetAt(index) == L'b');
    allPassed = allPassed && (str.GetAt(index) == _T('b'));

    return allPassed;
}

static bool Test_Span()
{
    const CStringA strA(textA);
    CStringW strW(textW);
    CString str(textT);

    bool allPassed = true;

    allPassed = allPassed && (strA.SpanIncluding("0123") == "0123");
    allPassed = allPassed && (strW.SpanIncluding(L"0123") == L"0123");
    allPassed = allPassed && (str.SpanIncluding(_T("0123")) == _T("0123"));

    allPassed = allPassed && (strA.SpanIncluding("def") == "");
    allPassed = allPassed && (strW.SpanIncluding(L"def") == L"");
    allPassed = allPassed && (str.SpanIncluding(_T("def")) == _T(""));

    allPassed = allPassed && (strA.SpanExcluding("0123") == "");
    allPassed = allPassed && (strW.SpanExcluding(L"0123") == L"");
    allPassed = allPassed && (str.SpanExcluding(_T("0123")) == _T(""));

    allPassed = allPassed && (strA.SpanExcluding("def") == "0123456789abc");
    allPassed = allPassed && (strW.SpanExcluding(L"def") == L"0123456789abc");
    allPassed = allPassed && (str.SpanExcluding(_T("def")) == _T("0123456789abc"));

    return allPassed;
}

static bool Test_Std_String()
{
    constexpr int stringLength = 10;
    const std::string strA("abcdef\0ghi", stringLength);
    const std::wstring strW(L"ABCDEF\0GHI", stringLength);

    // Convert std::string to CStringA.
    CStringA sA1 = ToCStringA(strA);
    CStringA sA2(strA);
    bool allPassed = (sA1 == sA2);

    // Convert std::wstring to CStringW.
    CStringW sW1 = ToCStringW(strW);
    CStringW sW2(strW);
    allPassed = allPassed && (sW1 == sW2);

    // Convert both std::string and std::wstring to CString with ToCString.
    CString s1A = ToCString(strA);
    CString s1W = ToCString(strW);
    allPassed = allPassed && (s1A == CString(sA1));
    allPassed = allPassed && (s1W == CString(sW1));

    // Construct CString from both std::string and std::wstring.
    CString s2A(strA);
    CString s2W(strW);
    allPassed = allPassed && (s1A == s2A);
    allPassed = allPassed && (s1W == s2W);
    allPassed = allPassed && (sA1.GetLength() == sW2.GetLength());
    allPassed = allPassed && (sA1.GetLength() == stringLength);

    // CStringA operator << only supports std::string.
    CStringA s3A;
    s3A << strA;
    allPassed = allPassed && (s3A == sA2);
    allPassed = allPassed && (s3A.GetLength() == stringLength);

    // CStringW operator << only supports std::wstring.
    CStringW s3W;
    s3W << strW;
    allPassed = allPassed && (s3W == sW2);
    allPassed = allPassed && (s3W.GetLength() == stringLength);

    // CString operator << supports both std::string and std::wstring.
    CString s3;
    s3 << strA << strW;
    allPassed = allPassed && (s3 == sA2 + sW2);
    allPassed = allPassed && (s3.GetLength() == 2 * stringLength);

    return allPassed;
}

static bool Test_Stream()
{
    bool allPassed = true;

    // Test stream of CString, integer, null char, char array, double and float.
    CString str;
    str << textT;
    str << str << 10 << '\0' << "__" << 3.13 << 6.66f;

    CStringA strA;
    strA << textA;
    strA << strA << 10 << '\0' << "__" << 3.13 << 6.66f;

    CStringW strW;
    strW << textW;
    strW << strW << 10 << L'\0' << L"__" << 3.13 << 6.66f;

    allPassed = allPassed && (strA ==   CStringA("0123456789abcdefg0123456789abcdefg10\0__3.136.66", 47));
    allPassed = allPassed && (strW ==   CStringW(L"0123456789abcdefg0123456789abcdefg10\0__3.136.66", 47));
    allPassed = allPassed && (str  ==   CString(_T("0123456789abcdefg0123456789abcdefg10\0__3.136.66"), 47));

    // Verify stream equals + 'ToCString'.
    allPassed = allPassed && (strA == (CStringA(textA) + CStringA(textA) + ToCStringA(10) + '\0'  + "__"  + ToCStringA(3.13) + ToCStringA(6.66)));
    allPassed = allPassed && (strW == (CStringW(textW) + CStringW(textW) + ToCStringW(10) + L'\0' + L"__" + ToCStringW(3.13) + ToCStringW(6.66)));
    allPassed = allPassed && (str  == (CString(textT)  + CString(textT)  + ToCString(10)  + '\0'  + L"__" + ToCString(3.13)  + ToCString(6.66)));

    // Assign from stream (also modifies CString on left of <<, hence a little cryptic).
    // Not recommended because this fails to run: "CString x = x << "test";
    CStringA strA20 = strA << 5;
    CStringW strW20 = strW << 5;
    CString  str20  = str  << 5;

    allPassed = allPassed && (strA20 == strA);
    allPassed = allPassed && (strW20 == strW);
    allPassed = allPassed && (str20  == str);

    // Construct from stream (also modifies CString on left of <<, hence a little cryptic).
    CStringA strA3145(strA << 3.14 << 5);
    CStringW strW3145(strW << 3.14 << 5);
    CString  str3145( str  << 3.14 << 5);

    // Verify construct from stream.
    allPassed = allPassed && (strA3145 == strA);
    allPassed = allPassed && (strW3145 == strW);
    allPassed = allPassed && (str3145  == str );

    // Stream other variable types.
    size_t v1 = 1;
    ULONGLONG v2 = 2;
    short v3 = 3;
    LONGLONG v4 = 4;
    INT_PTR v5 = 5;
    DWORD v6 = 6;
    strA3145 << v1 << v2 << v3 << v4 << v5 << v6;
    strW3145 << v1 << v2 << v3 << v4 << v5 << v6;
    str3145  << v1 << v2 << v3 << v4 << v5 << v6;

    strA = strA + ToCStringA(v1) + ToCStringA(v2) + ToCStringA(v3) + ToCStringA(v4) + ToCStringA(v5) + ToCStringA(v6);
    strW = strW + ToCStringW(v1) + ToCStringW(v2) + ToCStringW(v3) + ToCStringW(v4) + ToCStringW(v5) + ToCStringW(v6);
    str  = str  + ToCString(v1)  + ToCString(v2)  + ToCString(v3)  + ToCString(v4)  + ToCString(v5)  + ToCString(v6);

    // Verify stream other variable types.
    allPassed = allPassed && (strA3145 == strA);
    allPassed = allPassed && (strW3145 == strW);
    allPassed = allPassed && (str3145 == str);

    return allPassed;
}

static bool Test_Tokenize()
{
    CStringA strA("%First Second#Third");
    CStringW strW(L"%First Second#Third");
    CString str(_T("%First Second#Third"));

    CStringA resTokenA;
    CStringW resTokenW;
    CString resToken;
    int curPosA = 0;
    int curPosW = 0;
    int curPos = 0;

    // Tokens are '%', ' ', and '#'.
    resTokenA = strA.Tokenize("% #", curPosA);
    resTokenW = strW.Tokenize(L"% #", curPosW);
    resToken = str.Tokenize(_T("% #"), curPos);

    CStringA testA = resTokenA;
    CStringW testW = resTokenW;
    CString test = resToken;

    while (resTokenA != "")
    {
        resTokenA = strA.Tokenize("% #", curPosA);
        testA += resTokenA;
    };

    while (resTokenW != L"")
    {
        resTokenW = strW.Tokenize(L"% #", curPosW);
        testW += resTokenW;
    };

    while (resToken != _T(""))
    {
        resToken = str.Tokenize(_T("% #"), curPos);
        test += resToken;
    };

    bool allPassed = true;
    // Test strings contain the text without the tokens.
    allPassed = allPassed && (testA ==    "FirstSecondThird");
    allPassed = allPassed && (testW ==   L"FirstSecondThird");
    allPassed = allPassed && (test  == _T("FirstSecondThird"));

    return allPassed;
}

static bool Test_Trim()
{
    LPCSTR text = "    abcdef    ";

    CStringA strA(text);
    CStringW strW = AtoW(text).c_str();
    CString str = AtoT(text).c_str();

    CStringA strtestA = strA;
    CStringW strtestW = strW;
    CString strtest = str;

    strA.TrimLeft();
    strW.TrimLeft();
    str.TrimLeft();

    strA.TrimRight();
    strW.TrimRight();
    str.TrimRight();

    strtestA.Trim();
    strtestW.Trim();
    strtest.Trim();

    bool allPassed = (strtestA == strA);
    allPassed = (allPassed && strtestW == strW);
    allPassed = (allPassed && strtest == str);

    return allPassed;
}

static bool Test_Truncate()
{
    CStringA strA(textA);
    CStringW strW(textW);
    CString str(textT);

    strA.Truncate(8);
    strW.Truncate(8);
    str.Truncate(8);

    bool allPassed = (strA.GetLength() == 8);
    allPassed = allPassed && (strW.GetLength() == 8);
    allPassed = allPassed && (str.GetLength() == 8);

    return allPassed;
}


static bool Test_WindowText()
{
    CStringA strA("TestCString");
    CStringW strW(L"TestCString");
    CString str(_T("TestCString"));

    CStringA testA;
    CStringW testW;
    CString test;

    // The rescource.rc file must be added to the project for LoadString to work.
    testA.LoadString(IDW_MAIN);
    testW.LoadString(IDW_MAIN);
    test.LoadString(IDW_MAIN);

    bool allPassed = (strA == testA);
    allPassed = allPassed && (strW == testW);
    allPassed = allPassed && (str == test);

    strA.GetErrorString(0);
    strW.GetErrorString(0);
    str.GetErrorString(0);

    allPassed = allPassed && (strA.Find("successfully") > 0);
    allPassed = allPassed && (strW.Find(L"successfully") > 0);
    allPassed = allPassed && (str.Find(_T("successfully")) > 0);

    // Should compile and run OK.
    for (int i = 0; i < 100; i++)
    {
        strA.GetErrorString(i);
        strW.GetErrorString(i);
        str.GetErrorString(i);
    }

    return allPassed;
}

void RunTests()
{
    using std::cout;
    sizeof(TCHAR) == 1 ? cout << "Compiled for Ansi\n" : cout << "Compiled for Unicode\n\n";

    cout << "Test_Add            "; Test_Add() ?             cout << "Passed\n" : cout << "***Failed***\n";
    cout << "Test_AllocSysString "; Test_AllocSysString() ?  cout << "Passed\n" : cout << "***Failed***\n";
    cout << "Test_AppendFormat   "; Test_AppendFormat() ?    cout << "Passed\n" : cout << "***Failed***\n";
    cout << "Test_Assign         "; Test_Assign() ?          cout << "Passed\n" : cout << "***Failed***\n";
    cout << "Test_BigBuffer      "; Test_BigBuffer() ?       cout << "Passed\n" : cout << "***Failed***\n";
    cout << "Test_Buffer         "; Test_Buffer() ?          cout << "Passed\n" : cout << "***Failed***\n";
    cout << "Test_Compare        "; Test_Compare() ?         cout << "Passed\n" : cout << "***Failed***\n";
    cout << "Test_Construct      "; Test_Construct() ?       cout << "Passed\n" : cout << "***Failed***\n";
    cout << "Test_Delete         "; Test_Delete() ?          cout << "Passed\n" : cout << "***Failed***\n";
    cout << "Test_Empty          "; Test_Empty() ?           cout << "Passed\n" : cout << "***Failed***\n";
    cout << "Test_Equal          "; Test_Equal() ?           cout << "Passed\n" : cout << "***Failed***\n";
    cout << "Test_Find           "; Test_Find() ?            cout << "Passed\n" : cout << "***Failed***\n";
    cout << "Test_Format         "; Test_Format() ?          cout << "Passed\n" : cout << "***Failed***\n";
    cout << "Test_Left           "; Test_Left() ?            cout << "Passed\n" : cout << "***Failed***\n";
    cout << "Test_Plus           "; Test_Plus() ?            cout << "Passed\n" : cout << "***Failed***\n";
    cout << "Test_Remove         "; Test_Remove() ?          cout << "Passed\n" : cout << "***Failed***\n";
    cout << "Test_Span           "; Test_Span() ?            cout << "Passed\n" : cout << "***Failed***\n";
    cout << "Test_Std_String     "; Test_Std_String() ?      cout << "Passed\n" : cout << "***Failed***\n";
    cout << "Test_Stream         "; Test_Stream() ?          cout << "Passed\n" : cout << "***Failed***\n";
    cout << "Test_Tokenize       "; Test_Tokenize() ?        cout << "Passed\n" : cout << "***Failed***\n";
    cout << "Test_Trim           "; Test_Trim() ?            cout << "Passed\n" : cout << "***Failed***\n";
    cout << "Test_Truncate       "; Test_Truncate() ?        cout << "Passed\n" : cout << "***Failed***\n";
    cout << "Test_WindowText     "; Test_WindowText() ?      cout << "Passed\n" : cout << "***Failed***\n";
}

