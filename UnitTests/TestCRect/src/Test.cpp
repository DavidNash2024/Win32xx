
#include "stdafx.h"
#include <iostream>
#include "Test.h"


bool CSizeConstruct()
{
    bool pass = true;
    CSize size1;
    CSize size2(20, 30);
    CSize size3(size2);
    CPoint pt(40, 50);
    CSize size4(pt);
    DWORD dw = MAKEWPARAM(50, 60);
    CSize size5(dw);

    pass = pass && ((size1.cx == 0) && (size1.cy == 0));
    pass = pass && ((size2.cx == 20) && (size2.cy == 30));
    pass = pass && ((size3.cx == 20) && (size3.cy == 30));
    pass = pass && ((size4.cx == 40) && (size4.cy == 50));
    pass = pass && ((size5.cx == 50) && (size5.cy == 60));

    return pass;
}

bool CSizeOperations()
{
    bool pass = true;

    CSize size1(20, 20);
    CSize size2(30, 30);
    size2.SetSize(40, 50);

    pass = pass && ((size1.cx == 20) && (size1.cy == 20));
    pass = pass && ((size2.cx == 40) && (size2.cy == 50));

    pass = pass && !(size1 == size2);
    pass = pass && (size1 != size2);

    LPSIZE pSize = (LPSIZE)&size1;
    SIZE size = *pSize;
    pass = pass && ((size.cx == 20) && (size.cy == 20));

    size1 += size;
    size1 -= size;
    pass = pass && ((size1.cx == 20) && (size1.cy == 20));

    size1 = -size1;
    pass = pass && ((size1.cx == -20) && (size1.cy == -20));
    size1 = -size1;
    pass = pass && ((size1.cx == 20) && (size1.cy == 20));
    size1 = size1 + size2;
    pass = pass && ((size1.cx == 60) && (size1.cy == 70));
    size1 = size1 - size2;;
    pass = pass && ((size1.cx == 20) && (size1.cy == 20));

    return pass;
}

bool CPointConstruct()
{
    bool pass = true;

    CPoint point1;
    CPoint point2(30, 30);
    POINT point = {40, 50};
    CPoint point3(point);
    POINTS points = {50, 60};
    CPoint point4(points);
    CSize size(70, 80);
    CPoint point5(size);
    LPARAM lparam = MAKELPARAM(90, 95);
    CPoint point6(lparam);

    pass = pass && ((point1.x == 0) && (point1.y == 0));
    pass = pass && ((point2.x == 30) && (point2.y == 30));
    pass = pass && ((point3.x == 40) && (point3.y == 50));
    pass = pass && ((point4.x == 50) && (point4.y == 60));
    pass = pass && ((point5.x == 70) && (point5.y == 80));
    pass = pass && ((point6.x == 90) && (point6.y == 95));

    return pass;
}

bool CPointOperations()
{
    bool pass = true;

    CPoint point1(20, 30);
    CPoint point2(30, 40);

    pass = pass && (point1 == point1);
    pass = pass && (point1 != point2);
    point1 += point2;
    pass = pass && (point1 == CPoint(50, 70));
    point1 -= point2;
    pass = pass && (point1 == CPoint(20, 30));
    CSize size(10, 15);
    point1 += size;
    pass = pass && (point1 == CPoint(30, 45));
    point1 -= size;
    pass = pass && (point1 == CPoint(20, 30));
    pass = pass && (-point1 == CPoint(-20, -30));
    CPoint point3 = point1 + point2;
    pass = pass && (point3 == CPoint(50, 70));
    point3 = point1 - point2;
    pass = pass && (point3 == CPoint(-10, -10));
    point3 = point1 + size;
    pass = pass && (point3 == CPoint(30, 45));
    point3 = point1 - size;
    pass = pass && (point3 == CPoint(10, 15));

    return pass;
}

bool CRectConstruct()
{
    CRect rect1;
    CRect rect2(10, 20, 30, 40);

    bool pass = true;
    pass = pass && ((rect1.left == 0) && (rect1.top == 0) && (rect1.right == 0) && (rect1.bottom == 0));
    pass = pass && ((rect2.left == 10) && (rect2.top == 20) && (rect2.right == 30) && (rect2.bottom == 40));
    pass = pass && (rect1 == CRect(0, 0, 0, 0));
    pass = pass && (rect2 == CRect(10, 20, 30, 40));
    CRect rect3(rect2);
    pass = pass && (rect3 == rect2);
    CSize size(50, 60);
    CPoint point(70, 80);
    CRect rect4(point, size);
    pass = pass && (rect4 == CRect(70, 80, 120, 140));
    CRect rect5(point, point);
    pass = pass && (rect5 == CRect(70, 80, 70, 80));

    return pass;
}

bool CRectInflate()
{
    bool pass = true;

    CRect rect1(2, 4, 6, 8);
    CRect rect2;
    CopyRect(rect2, &rect1);
    pass = pass && ((rect1 == rect2) && (rect2 == CRect(2, 4, 6, 8)));
    rect2.InflateRect(2, 2);
    pass = pass && (rect2 == CRect(0, 2, 8, 10));
    rect2.DeflateRect(2, 2);
    pass = pass && (rect2 == CRect(2, 4, 6, 8));
    CSize size(2, 2);
    rect2.InflateRect(size);
    pass = pass && (rect2 == CRect(0, 2, 8, 10));
    rect2.DeflateRect(size);
    pass = pass && (rect2 == CRect(2, 4, 6, 8));
    rect2.InflateRect(2, 2, 2, 2);
    pass = pass && (rect2 == CRect(0, 2, 8, 10));
    rect2.DeflateRect(2, 2, 2, 2);
    pass = pass && (rect2 == CRect(2, 4, 6, 8));
    rect2.InflateRect(CRect(2, 2, 2, 2));
    pass = pass && (rect2 == CRect(0, 2, 8, 10));
    rect2.DeflateRect(CRect(2, 2, 2, 2));
    pass = pass && (rect2 == CRect(2, 4, 6, 8));

    return pass;
}

bool CRectOperations()
{
    bool pass = true;
    CRect rect1(10, 20, 10, 20);
    pass = pass && !!rect1.IsRectEmpty();
    pass = pass && !rect1.IsRectNull();
    CRect rect2;
    pass = pass && !!rect2.IsRectNull();
    CRect rect3 = rect1.MulDiv(4, 2);
    pass = pass && (rect3 == CRect(20, 40, 20, 40));
    CRect rect4(50, 60, 0, 0);
    rect4.NormalizeRect();
    pass = pass && (rect4 == CRect(0, 0, 50, 60));
    rect4.OffsetRect(20, 10);
    pass = pass && (rect4 == CRect(20, 10, 70, 70));
    CPoint point(-20, -10);
    rect4.OffsetRect(point);
    pass = pass && (rect4 == CRect(0, 0, 50, 60));
    CSize size(20, 10);
    rect4.OffsetRect(size);
    pass = pass && (rect4 == CRect(20, 10, 70, 70));
    rect4.SetRect(5, 10, 20, 30);
    pass = pass && (rect4 == CRect(5, 10, 20, 30));
    CPoint point1(10, 20);
    CPoint point2(30, 40);
    rect4.SetRect(point1, point2);
    pass = pass && (rect4 == CRect(10, 20, 30, 40));
    rect4.SetRectEmpty();
    pass = pass && (rect4 == CRect(0, 0, 0, 0));
    CRect rect5(10, 10, 100, 100);
    CRect rect6(50, 10, 150, 150);
    rect4.SubtractRect(&rect5, &rect6);
    pass = pass && (rect4 == CRect(10, 10, 50, 100));
    rect4.UnionRect(&rect5, &rect6);
    pass = pass && (rect4 == CRect(10, 10, 150, 150));

    return pass;
}

bool CRectReposition()
{
    bool pass = true;

    CRect rect1(20, 20, 80, 80);
    rect1.MoveToX(10);
    pass = pass && (rect1 == CRect(10, 20, 70, 80));
    rect1.MoveToY(10);
    pass = pass && (rect1 == CRect(10, 10, 70, 70));
    rect1.MoveToXY(30, 30);
    pass = pass && (rect1 == CRect(30, 30, 90, 90));

    return pass;
}

bool CRectAttributes()
{
    bool pass = true;

    CRect rect1(10, 30, 90, 100);
    pass = pass && (rect1.Width() == 80);
    pass = pass && (rect1.Height() == 70);
    pass = pass && (rect1.Size() == CSize(80, 70));
    pass = pass && (rect1.CenterPoint() == CPoint(50, 65));
    pass = pass && (rect1.TopLeft() == CPoint(10, 30));
    pass = pass && (rect1.BottomRight() == CPoint(90, 100));

    return pass;
}

void RunTests()
{
    using std::cout;

    cout << ("*** CPoint tests start ***\n");
    cout << ("CPoint constructions test   "); CPointConstruct() ?  cout << ("Passed\n") : cout << ("Failed\n");
    cout << ("CPoint operations test      "); CPointOperations() ? cout << ("Passed\n") : cout << ("Failed\n");
    cout << ("*** CPoint tests end ***\n\n");

    cout << ("*** CRect tests start ***\n" );
    cout << ("CRect constructions test    "); CRectConstruct() ?   cout << ("Passed\n") : cout << ("Failed\n");
    cout << ("CRect inflate test          "); CRectInflate() ?     cout << ("Passed\n") : cout << ("Failed\n");
    cout << ("CRect operations test       "); CRectOperations() ?  cout << ("Passed\n") : cout << ("Failed\n");
    cout << ("CRect reposition test       "); CRectReposition() ?  cout << ("Passed\n") : cout << ("Failed\n");
    cout << ("CRect Attributes test       "); CRectAttributes() ?  cout << ("Passed\n") : cout << ("Failed\n");
    cout << ("*** CRect tests end ***\n\n" );

    cout << ("*** CSize tests start ***\n" );
    cout << ("CSize constructions test    "); CSizeConstruct() ?   cout << ("Passed\n") : cout << ("Failed\n");
    cout << ("CSize operations test       "); CSizeOperations() ?  cout << ("Passed\n") : cout << ("Failed\n");
    cout << ("*** CSize tests end ***\n\n ");
}


