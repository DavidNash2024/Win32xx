
#include "pch.h"
#include "Test.h"
#include <iostream>

#ifdef _UNICODE
#define tcout std::wcout
#else
#define tcout std::cout
#endif


void SetMyFont(CDC dc)
{
    // font is a local variable, but SelectObject keeps it in scope.
    CFont font;
    font.CreatePointFont(300, _T("Times New Roman"));
    dc.SelectObject(font);
}

//Tests:
// Pass a CDC by value to a function
// Pass a CMemDC as a CDC
// Keep an object using SelectObject object in scope.
void SelectBitmapTest(CDC dc, int cx, int cy)
{
    CBitmap bitmap;
    bitmap.CreateBitmap(cx, cy, 1, 1, 0);
    dc.SelectObject(bitmap);
    // Note the bitmap object goes out of scope here, but SelectObject makes its own copy.
}

HBITMAP CreateHBITMAPTest(int cx, int cy)
{
    CBitmap bitmap;
    bitmap.CreateBitmap(cx, cy, 1, 1, 0);
    return bitmap;
}

CBitmap CreateBitmapTest(int cx, int cy)
{
    CBitmap bitmap;
    bitmap.CreateBitmap(cx, cy, 1, 1, 0);
    return bitmap;
}

bool TestObjectReferenceCount()
{
    // Test GDI object creation.
    bool result = true;
    CGDIObject object1;
    object1.Attach(::GetStockObject(WHITE_BRUSH));    // Unmanaged but no need to delete.
    CBitmap bitmap1;
    bitmap1.CreateBitmap(1,1,1,1,0);
    CBrush brush1(RGB(255,0,0));
    CFont font1;
    font1.CreatePointFont(300, _T("MARLETT"));
    CPen pen1(PS_SOLID, 1, RGB(0, 255, 0));

    // Confirm copied objects manage the same handle.
    CGDIObject object2 = object1;
    CGDIObject object3 = object2;
    result = result && (object1.GetHandle() == object2.GetHandle());
    result = result && (object2.GetHandle() == object3.GetHandle());

    CBitmap bitmap2 = bitmap1;
    CBitmap bitmap3 = bitmap2;
    result = result && (bitmap1.GetHandle() == bitmap2.GetHandle());
    result = result && (bitmap2.GetHandle() == bitmap3.GetHandle());

    CBrush brush2 = brush1;
    CBrush brush3 = brush2;
    result = result && (brush1.GetHandle() == brush2.GetHandle());
    result = result && (brush2.GetHandle() == brush3.GetHandle());

    CFont font2 = font1;
    CFont font3 = font2;
    result = result && (font1.GetHandle() == font2.GetHandle());
    result = result && (font2.GetHandle() == font3.GetHandle());

    CPen pen2 = pen1;
    CPen pen3 = pen2;
    result = result && (pen1.GetHandle() == pen2.GetHandle());
    result = result && (pen2.GetHandle() == pen3.GetHandle());

    // Test Detach copy reference.
    object1.Detach();                  // No need to keep a copy of the stock object for deletion later.
    HGDIOBJ bitmap = bitmap1.Detach();
    HGDIOBJ brush = brush1.Detach();
    HGDIOBJ font = font1.Detach();
    HGDIOBJ pen = pen1.Detach();

    // Detaching the handle should detach from all reference counted copies of the object.
    result = result && (object3.GetHandle() == 0);
    result = result && (bitmap3.GetHandle() == 0);
    result = result && (brush3.GetHandle() == 0);
    result = result && (font3.GetHandle() == 0);
    result = result && (pen3.GetHandle() == 0);

    // Confirm the handles we detached are valid and can be deleted.
    result = result && ::DeleteObject(bitmap);
    result = result && ::DeleteObject(brush);
    result = result && ::DeleteObject(font);
    result = result && ::DeleteObject(pen);

    // true if all tests pass.
    return result;
}

bool TestCDCReferenceCount(HWND wnd)
{
    // Test device context creation.
    bool result = true;
    CDC DC1 = GetDC(wnd);
    CClientDC clientDC1(wnd);
    CRgn rgn;
    rgn.CreateRectRgn(0, 0, 20, 20);
    CClientDCEx clientExDC1(wnd, rgn, DCX_CACHE | DCX_INTERSECTRGN);
    CMemDC memDC1(clientDC1);
    CMetaFileDC metaDC1;

    // Create and CreateEnhanced actually create the metafile and enhMetafile DCs.
    CString fileName = "test";
    CString enhancedFileName = "test_enhanced";
    metaDC1.Create(fileName);
    CEnhMetaFileDC enhMetaDC1;
    enhMetaDC1.CreateEnhanced(0, enhancedFileName, 0, 0);

    // Confirm the system owns rgn now..
    result = result && (rgn.GetHandle() == 0);

    // We can make reference counted copies of all CDC types.
    // Confirm copied objects manage the same handle.
    CDC DC2 = DC1;
    CDC DC3 = DC2;
    result = result && (DC1 == DC2);
    result = result && (DC2 == DC3);

    CDC DC4;
    DC4 = DC3.GetHDC();

    CClientDC clientDC2 = clientDC1;
    CClientDC clientDC3 = clientDC2;
    result = result && (clientDC1 == clientDC2);
    result = result && (clientDC2 == clientDC3);

    DC4.Attach(clientDC3);
    assert(DC4 == clientDC3);
    assert(clientDC1 == clientDC2);
    assert(DC4 != DC3);

    CClientDCEx clientExDC2 = clientExDC1;
    CClientDCEx clientExDC3 = clientExDC2;
    result = result && (clientExDC1 == clientExDC2);
    result = result && (clientExDC2 == clientExDC3);

    CMemDC memDC2 = memDC1;
    CMemDC memDC3 = memDC2;
    result = result && (memDC1 == memDC2);
    result = result && (memDC2 == memDC3);

    CMetaFileDC metaDC2 = metaDC1;
    CMetaFileDC metaDC3 = metaDC2;
    result = result && (metaDC1 == metaDC2);
    result = result && (metaDC2 == metaDC3);

    CEnhMetaFileDC enhMetaDC2 = enhMetaDC1;
    CEnhMetaFileDC enhMetaDC3 = enhMetaDC2;
    result = result && (enhMetaDC1 == enhMetaDC2);
    result = result && (enhMetaDC2 == enhMetaDC3);

    // Detaching the DC should detach from all reference counted copies of the CDC.
    HDC hDC = DC1.Detach();
    HDC hClientDC = clientDC1.Detach();
    HDC hClientExDC = clientExDC1.Detach();
    HDC hMemDC = memDC1.Detach();
    {
        // Closing a CMetaFileDC or CEnhMetaFileDC creates the file and deletes the DC.
        CMetaFile metaFile = metaDC1.Close();
        CEnhMetaFile enhMetaFile = enhMetaDC1.CloseEnhanced();

        // Confirm all copies have the HDC removed.
        result = result && (DC1.GetHDC() == 0);
        result = result && (clientDC1.GetHDC() == 0);
        result = result && (clientExDC1.GetHDC() == 0);
        result = result && (memDC1.GetHDC() == 0);
        result = result && (metaDC1.GetHDC() == 0);
        result = result && (enhMetaDC1.GetHDC() == 0);
        result = result && (DC2.GetHDC() == 0);
        result = result && (clientDC2.GetHDC() == 0);
        result = result && (clientExDC2.GetHDC() == 0);
        result = result && (memDC2.GetHDC() == 0);
        result = result && (metaDC2.GetHDC() == 0);
        result = result && (enhMetaDC2.GetHDC() == 0);
        result = result && (DC3.GetHDC() == 0);
        result = result && (clientDC3.GetHDC() == 0);
        result = result && (clientExDC3.GetHDC() == 0);
        result = result && (memDC3.GetHDC() == 0);
        result = result && (metaDC3.GetHDC() == 0);
        result = result && (enhMetaDC3.GetHDC() == 0);

        // Confirm the handles we detached are valid and can be deleted/released.
        result = result && ::DeleteDC(hDC);
        result = result && ::ReleaseDC(wnd, hClientDC);
        result = result && ::ReleaseDC(wnd, hClientExDC);
        result = result && ::DeleteDC(hMemDC);
    }

    // Delete the meta files.
    result = result && ::DeleteFile(fileName);
    result = result && ::DeleteFile(enhancedFileName);

    return result;
}

bool TestBitmap(CBitmap bitmap, int cx, int cy)
{
    BITMAP data{};
    bool testPassed = false;

    // Is this a valid bitmap?
    if(::GetObject(bitmap, sizeof(data), &data))
    {
        // Is the bitmap size correct?
        testPassed = (data.bmWidth == cx && data.bmHeight == cy);
    }

    return testPassed;
}

void RunTests()
{
    CMemDC memDC(GetDC(HWND_DESKTOP));
    int cx = 30;
    int cy = 40;
    CString str;
    CString T = "passed";
    CString F = "***failed***";
    std::cout << "\n\n***  Testing begins     ***\n";
    for (int i = 0; i < 1000; i++)
    {
        // Set the font.
        SetMyFont(memDC);

        // The detached bitmap should be valid and the correct size.
        SelectBitmapTest(memDC, cx, cy);
        CBitmap bitmap1 = memDC.DetachBitmap();
        str = "Select Bitmap test:            " + (TestBitmap(bitmap1, cx, cy) ? T : F) + "\n";
        std::wcout << str.c_str();

        // The created bitmap should be valid and the correct size.
        CBitmap bitmap2 = CreateBitmapTest(cx, cy);
        str = "Return CBitmap by value test:  " + (TestBitmap(bitmap2, cx, cy) ? T : F) + "\n";
        tcout << str.c_str();

        // The HBITMAP returned by CreateHBITMAPTest is expected to be invalid!
        // This test passes when TestBitmap returns false!
        // This highlights the difference between returning a HBITMAP and a CBitmap.
        CBitmap bitmap3 = CreateHBITMAPTest(cx, cy);
        str = "Return HBITMAP by value test:  " + (TestBitmap(bitmap3, cx, cy) ? F : T) + "\n";
        tcout << str.c_str();

        // The series of reference count tests should all pass.
        str = "Object Reference Count test:   " + (TestObjectReferenceCount() ? T : F) + "\n";
        tcout << str.c_str();

        // The series of reference count tests should all pass.
        str = "DC Reference Count test:       " + (TestCDCReferenceCount(HWND_DESKTOP) ? T : F) + "\n";
        tcout << str.c_str();
        tcout << "Loop count = " << ToCString(i).c_str() << '\n';
    }

    tcout << "***  Testing completed  ***\n\n";
}
