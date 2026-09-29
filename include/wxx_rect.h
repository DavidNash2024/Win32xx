// Win32++   Version 10.4.0
// Release Date: TBA
//
//      David Nash
//      email: dnash@bigpond.net.au
//      url: https://sourceforge.net/projects/win32-framework
//           https://github.com/DavidNash2024/Win32xx
//
//
// Copyright (c) 2005-2026  David Nash
//
// Permission is hereby granted, free of charge, to
// any person obtaining a copy of this software and
// associated documentation files (the "Software"),
// to deal in the Software without restriction, including
// without limitation the rights to use, copy, modify,
// merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom
// the Software is furnished to do so, subject to the
// following conditions:
//
// The above copyright notice and this permission notice
// shall be included in all copies or substantial portions
// of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF
// ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED
// TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A
// PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT
// SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR
// ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
// ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE
// OR OTHER DEALINGS IN THE SOFTWARE.
//
////////////////////////////////////////////////////////


///////////////////////////////////////////////////////
// wxx_rect.h
//  Definitions of the CSize, CPoint and CRect classes.

#pragma once

namespace Win32xx
{
    // Forward declarations
    class CRect;
    class CSize;


    ////////////////////////////////////////////////////////////////
    // The CPoint class can be used in place of the POINT structure.
    class CPoint : public POINT
    {
    public:
        constexpr CPoint() noexcept : POINT{ 0, 0 } {};
        constexpr CPoint(int x, int y) noexcept : POINT{ x, y } {};
        constexpr CPoint(POINT pt) noexcept : POINT(pt) {};
        constexpr CPoint(POINTS pts) noexcept : POINT{ pts.x, pts.y } {};
        constexpr CPoint(SIZE sz) noexcept : POINT{ sz.cx, sz.cy } {};
        constexpr CPoint(LPARAM dwPos) noexcept
            : POINT{ GET_X_LPARAM(dwPos), GET_Y_LPARAM(dwPos) } {};

        void Offset(int dx, int dy) noexcept;
        void Offset(POINT pt) noexcept;
        void Offset(SIZE sz) noexcept;
        void SetPoint(int px, int py) noexcept;

        // Operators
        operator LPPOINT() noexcept;
        bool operator==(CPoint pt) const noexcept;
        bool operator!=(CPoint pt) const noexcept;
        void operator+=(SIZE sz) noexcept;
        void operator-=(SIZE sz) noexcept;
        void operator+=(POINT pt) noexcept;
        void operator-=(POINT pt) noexcept;

        // Operators returning CPoint
        CPoint operator-() const noexcept;
        CPoint operator+(SIZE sz) const noexcept;
        CPoint operator-(SIZE sz) const noexcept;
        CPoint operator+(POINT pt) const noexcept;
        CPoint operator-(POINT pt) const noexcept;

        // Operators returning CRect
        CRect operator+(LPCRECT prc) const noexcept;
        CRect operator-(LPCRECT prc) const noexcept;
    };


    //////////////////////////////////////////////////////////////
    // The CRect class can be used in place of the RECT structure.
    class CRect : public RECT
    {
    public:
        constexpr CRect() noexcept : RECT{ 0, 0, 0, 0 } {};
        constexpr CRect(int l, int t, int r, int b) noexcept
            : RECT{ l, t, r, b } {}
        constexpr CRect(RECT rc) noexcept : RECT(rc) {}
        constexpr CRect(POINT pt, SIZE sz) noexcept
            : CRect(pt.x, pt.y, pt.x + sz.cx, pt.y + sz.cy) {}
        constexpr CRect(POINT topLeft, POINT bottomRight) noexcept
            : CRect(topLeft.x, topLeft.y, bottomRight.x, bottomRight.y) {}

        BOOL CopyRect(LPCRECT prc) noexcept;
        BOOL DeflateRect(int x, int y) noexcept;
        BOOL DeflateRect(SIZE size) noexcept;
        void DeflateRect(LPCRECT prc) noexcept;
        void DeflateRect(int l, int t, int r, int b) noexcept;
        BOOL EqualRect(LPRECT prc) const noexcept;
        BOOL InflateRect(int dx, int dy) noexcept;
        BOOL InflateRect(SIZE sz) noexcept;
        void InflateRect(LPCRECT prc) noexcept;
        void InflateRect(int l, int t, int r, int b) noexcept;
        BOOL IntersectRect(LPCRECT prc1, LPCRECT prc2) noexcept;
        BOOL IsRectEmpty() const noexcept;
        BOOL IsRectNull() const noexcept;
        CRect MulDiv(int mult, int div) const noexcept;
        void NormalizeRect() noexcept;
        BOOL OffsetRect(int dx, int dy) noexcept;
        BOOL OffsetRect(POINT pt) noexcept;
        BOOL OffsetRect(SIZE size) noexcept;
        BOOL PtInRect(POINT pt) const noexcept;
        BOOL SetRect(int l, int t, int r, int b) noexcept;
        BOOL SetRect(POINT topLeft, POINT bottomRight) noexcept;
        BOOL SetRectEmpty() noexcept;
        BOOL SubtractRect(LPCRECT prc1, LPCRECT prc2) noexcept;
        BOOL UnionRect(LPCRECT prc1, LPCRECT prc2) noexcept;

        // Reposition rectangle
        void MoveToX(int x) noexcept;
        void MoveToY(int y) noexcept;
        void MoveToXY(int x, int y) noexcept;
        void MoveToXY(POINT pt) noexcept;

        // Attributes
        int Height() const noexcept;
        int Width() const noexcept;
        CSize Size() const noexcept;
        CPoint CenterPoint() const noexcept;
        CPoint TopLeft() const noexcept;
        CPoint BottomRight() const noexcept;

        // operators
        operator LPRECT() noexcept;
        operator LPCRECT() const noexcept;
        bool operator==(CRect rc) const noexcept;
        bool operator!=(CRect rc) const noexcept;
        void operator+=(POINT pt) noexcept;
        void operator+=(SIZE size) noexcept;
        void operator+=(LPCRECT prc) noexcept;
        void operator-=(LPCRECT prc) noexcept;
        void operator-=(POINT pt) noexcept;
        void operator-=(SIZE sz) noexcept;
        void operator&=(RECT rc) noexcept;
        void operator|=(RECT rc) noexcept;

        // Operators returning CRect
        CRect operator+(POINT pt) const noexcept;
        CRect operator-(POINT pt) const noexcept;
        CRect operator+(SIZE sz) const noexcept;
        CRect operator-(SIZE sz) const noexcept;
        CRect operator+(LPRECT prc) const noexcept;
        CRect operator-(LPRECT prc) const noexcept;
        CRect operator&(RECT rc) const noexcept;
        CRect operator|(RECT rc) const noexcept;
    };


    //////////////////////////////////////////////////////////////
    // The CSize class can be used in place of the SIZE structure.
    class CSize : public SIZE
    {
    public:
        constexpr CSize() noexcept : SIZE{ 0, 0 } {};
        constexpr CSize(int px, int py) noexcept : SIZE{ px, py } {};
        constexpr CSize(SIZE sz) noexcept : SIZE(sz) {};
        constexpr CSize(POINT pt) noexcept : SIZE{ pt.x, pt.y } {};
        constexpr CSize(DWORD dw) noexcept : SIZE{ GET_X_LPARAM(dw), GET_Y_LPARAM(dw) } {};
        void SetSize(int sx, int sy) noexcept;

        // Operators
        operator LPSIZE() noexcept;
        bool operator==(CSize sz) const noexcept;
        bool operator!=(CSize sz) const noexcept;
        void operator+=(SIZE sz) noexcept;
        void operator-=(SIZE sz) noexcept;

        // Operators returning CSize
        CSize operator-() const noexcept;
        CSize operator+(SIZE sz) const noexcept;
        CSize operator-(SIZE sz) const noexcept;

        // Operators returning CPoint
        CPoint operator+(POINT point) const noexcept;
        CPoint operator-(POINT point) const noexcept;

        // Operators returning CRect
        CRect operator+(LPCRECT prc) const noexcept;
        CRect operator-(LPCRECT prc) const noexcept;
    };


    ////////////////////////////////////
    // Definitions for the CPoint class.
    //

    // Moves the CPoint by the specified offsets.
    inline void CPoint::Offset(int dx, int dy) noexcept
    {
        x += dx;
        y += dy;
    }

    // Moves the CPoint by the specified offsets.
    inline void CPoint::Offset(POINT pt) noexcept
    {
        x += pt.x;
        y += pt.y;
    }

    // Moves the CPoint by the specified offsets.
    inline void CPoint::Offset(SIZE sz) noexcept
    {
        x += sz.cx;
        y += sz.cy;
    }

    // Sets the coordinates of the CPoint.
    inline void CPoint::SetPoint(int px, int py) noexcept
    {
        x = px;
        y = py;
    }

    // Returns a pointer to the POINT associated with this object.
    inline CPoint::operator LPPOINT() noexcept
    {
        return this;
    }

    // Returns true if the co-ordinates of the source point and the CPoint are
    // equal.
    inline bool CPoint::operator==(CPoint pt) const noexcept
    {
        return ((x == pt.x) && (y == pt.y));
    }

    // Returns true if the co-ordinates of the source point and the CPoint are
    // not equal.
    inline bool CPoint::operator!=(CPoint pt) const noexcept
    {
        return ((x != pt.x) || (y != pt.y));
    }

    // Adds the specified SIZE to the point.
    inline void CPoint::operator+=(SIZE sz) noexcept
    {
        x += sz.cx;
        y += sz.cy;
    }

    // Subtracts the specified SIZE from this point.
    inline void CPoint::operator-=(SIZE sz) noexcept
    {
        x -= sz.cx;
        y -= sz.cy;
    }

    // Adds the specified POINT to this point.
    inline void CPoint::operator+=(POINT pt) noexcept
    {
        x += pt.x;
        y += pt.y;
    }

    // Subtracts the specified POINT from this point.
    inline void CPoint::operator-=(POINT pt) noexcept
    {
        x -= pt.x;
        y -= pt.y;
    }

    // Returns the unary minus (additive inverse).
    inline CPoint CPoint::operator-() const noexcept
    {
        return CPoint(-x, -y);
    }

    // Adds the specified SIZE and returns the value.
    inline CPoint CPoint::operator+(SIZE sz) const noexcept
    {
        return CPoint(x + sz.cx, y + sz.cy);
    }

    // Subtracts the specified SIZE and returns the value.
    inline CPoint CPoint::operator-(SIZE sz) const noexcept
    {
        return CPoint(x - sz.cx, y - sz.cy);
    }

    // Adds the specified POINT and returns the value.
    inline CPoint CPoint::operator+(POINT pt) const noexcept
    {
        return CPoint(x + pt.x, y + pt.y);
    }

    // Subtracts the specified POINT and returns the value.
    inline CPoint CPoint::operator-(POINT pt) const noexcept
    {
        return CPoint(x - pt.x, y - pt.y);
    }

    // Adds the specified RECT and returns the value.
    inline CRect CPoint::operator+(LPCRECT prc) const noexcept
    {
        return CRect(*prc) + *this;
    }

    // Subtracts the specified RECT and returns the value.
    inline CRect CPoint::operator-(LPCRECT prc) const noexcept
    {
        return CRect(*prc) - *this;
    }


    ///////////////////////////////////
    // Definitions for the CRect class.
    //

    // Copies the coordinates of the source rectangle to the CRect.
    inline BOOL CRect::CopyRect(LPCRECT prc) noexcept
    {
        return ::CopyRect(this, prc);
    }

    // Decreases the width and height of the CRect.
    inline BOOL CRect::DeflateRect(int x, int y) noexcept
    {
        return ::InflateRect(this, -x, -y);
    }

    // Decreases the width and height of the CRect.
    inline BOOL CRect::DeflateRect(SIZE size) noexcept
    {
        return ::InflateRect(this, -size.cx, -size.cy);
    }

    // Decreases the width and height of the CRect.
    inline void CRect::DeflateRect(LPCRECT prc) noexcept
    {
        left += prc->left;
        top += prc->top;
        right -= prc->right;
        bottom -= prc->bottom;
    }

    // Decreases the width and height of the CRect.
    inline void CRect::DeflateRect(int l, int t, int r, int b) noexcept
    {
        left += l;
        top += t;
        right -= r;
        bottom -= b;
    }

    // Determines whether the source rectangle and the CRect are equal by
    // comparing the coordinates of their upper-left and lower-right corners.
    inline BOOL CRect::EqualRect(LPRECT prc) const noexcept
    {
        return ::EqualRect(prc, this);
    }

    // Increases the width and height of the CRect.
    inline BOOL CRect::InflateRect(int dx, int dy) noexcept
    {
        return ::InflateRect(this, dx, dy);
    }

    // Increases the width and height of the CRect.
    inline BOOL CRect::InflateRect(SIZE sz) noexcept
    {
        return ::InflateRect(this, sz.cx, sz.cy);
    }

    // Increases the width and height of the CRect.
    inline void CRect::InflateRect(LPCRECT prc) noexcept
    {
        left -= prc->left;
        top -= prc->top;
        right += prc->right;
        bottom += prc->bottom;
    }

    // Increases the width and height of the CRect.
    inline void CRect::InflateRect(int l, int t, int r, int b) noexcept
    {
        left -= l;
        top -= t;
        right += r;
        bottom += b;
    }

    // Calculates the intersection of two source rectangles and places the
    // coordinates of the intersection rectangle into the CRect.
    inline BOOL CRect::IntersectRect(LPCRECT prc1, LPCRECT prc2) noexcept
    {
        return ::IntersectRect(this, prc1, prc2);
    }

    // Determines whether the CRect is empty.
    inline BOOL CRect::IsRectEmpty() const noexcept
    {
        return ::IsRectEmpty(this);
    }

    // Determines whether the CRect is null.
    inline BOOL CRect::IsRectNull() const noexcept
    {
        return (left == 0 && right == 0 && top == 0 && bottom == 0);
    }

    // Multiplies the CRect values by mult, and then divides the result by div.
    inline CRect CRect::MulDiv(int mult, int div) const noexcept
    {
        return CRect((left * mult) / div, (top * mult) / div,
        (right * mult) / div, (bottom * mult) / div);
    }

    // Normalizes CRect so that both the height and width are positive.
    inline void CRect::NormalizeRect() noexcept
    {
        int temp;
        if (left > right)
        {
            temp = left;
            left = right;
            right = temp;
        }

        if (top > bottom)
        {
            temp = top;
            top = bottom;
            bottom = temp;
        }
    }

    // Moves the CRect by the specified offsets.
    inline BOOL CRect::OffsetRect(int dx, int dy) noexcept
    {
        return ::OffsetRect(this, dx, dy);
    }

    // Moves the CRect by the specified offsets.
    inline BOOL CRect::OffsetRect(POINT pt) noexcept
    {
        return ::OffsetRect(this, pt.x, pt.y);
    }

    // Moves the CRect by the specified offsets.
    inline BOOL CRect::OffsetRect(SIZE size) noexcept
    {
        return ::OffsetRect(this, size.cx, size.cy);
    }

    // Determines whether the specified point lies within the CRect.
    inline BOOL CRect::PtInRect(POINT pt) const noexcept
    {
        return ::PtInRect(this, pt);
    }

    // Sets the coordinates of the CRect.
    inline BOOL CRect::SetRect(int l, int t, int r, int b) noexcept
    {
        return ::SetRect(this, l, t, r, b);
    }

    // Sets the coordinates of the CRect.
    inline BOOL CRect::SetRect(POINT topLeft, POINT bottomRight) noexcept
    {
        return ::SetRect(this, topLeft.x, topLeft.y, bottomRight.x, bottomRight.y);
    }

    // Sets all the coordinates of the CRect to zero.
    inline BOOL CRect::SetRectEmpty() noexcept
    {
        return ::SetRectEmpty(this);
    }

    // Sets the coordinates of the CRect to those formed by subtracting one
    // rectangle from another.
    inline BOOL CRect::SubtractRect(LPCRECT prc1, LPCRECT prc2) noexcept
    {
        return ::SubtractRect(this, prc1, prc2);
    }

    // Creates the union of two rectangles.
    inline BOOL CRect::UnionRect(LPCRECT prc1, LPCRECT prc2) noexcept
    {
        return ::UnionRect(this, prc1, prc2);
    }

    // Moves the CRect to the specified left position.
    inline void CRect::MoveToX(int x) noexcept
    {
        right = Width() + x;
        left = x;
    }

    // Moves the CRect to the specified top position.
    inline void CRect::MoveToY(int y) noexcept
    {
        bottom = Height() + y;
        top = y;
    }

    // Moves to CRect to the specified left and top positions.
    inline void CRect::MoveToXY(int x, int y) noexcept
    {
        MoveToX(x);
        MoveToY(y);
    }

    // Moves to CRect to the specified left and top positions.
    inline void CRect::MoveToXY(POINT pt) noexcept
    {
        MoveToX(pt.x);
        MoveToY(pt.y);
    }

    // Returns the height of the CRect.
    inline int CRect::Height() const noexcept
    {
        return bottom - top;
    }

    // Returns the width of the CRect.
    inline int CRect::Width() const noexcept
    {
        return right - left;
    }

    // Returns the size (width and height) of the CRect.
    inline CSize CRect::Size() const noexcept
    {
        return CSize(Width(), Height());
    }

    // Returns the point at the center of the CRect.
    inline CPoint CRect::CenterPoint() const noexcept
    {
        return CPoint((left + right) / 2, (top + bottom) / 2);
    }

    // Returns the top left point of the CRect.
    inline CPoint CRect::TopLeft() const noexcept
    {
        return CPoint(left, top);
    }

    // Returns the bottom right point of the CRect.
    inline CPoint CRect::BottomRight() const noexcept
    {
        return CPoint(right, bottom);
    }

    // Returns a pointer to the RECT associated with this CRect.
    inline CRect::operator LPRECT() noexcept
    {
        return this;
    }

    // Returns a const pointer to the RECT associated with this CRect.
    inline CRect::operator LPCRECT() const noexcept
    {
        return this;
    }

    // Returns true if the co-ordinates of the source rectangle and the CRect
    // are equal.
    inline bool CRect::operator==(CRect rc) const noexcept
    {
        return (::EqualRect(this, &rc) != 0);
    }

    // Returns true if the co-ordinates of the source rectangle and the CRect
    // are not equal.
    inline bool CRect::operator!=(CRect rc) const noexcept
    {
        return (::EqualRect(this, &rc) == 0);
    }

    // Adds the specified value to the CRect.
    inline void CRect::operator+=(POINT pt) noexcept
    {
        ::OffsetRect(this, pt.x, pt.y);
    }

    // Adds the specified value to the CRect.
    inline void CRect::operator+=(SIZE size) noexcept
    {
        ::OffsetRect(this, size.cx, size.cy);
    }

    // Adds the specified value to the CRect.
    inline void CRect::operator+=(LPCRECT prc) noexcept
    {
        ::InflateRect(this, prc->right - prc->left, prc->bottom - prc->top);
    }

    // Subtracts the specified value from the CRect.
    inline void CRect::operator-=(LPCRECT prc) noexcept
    {
        ::InflateRect(this, prc->left - prc->right, prc->top - prc->bottom);
    }

    // Subtracts the specified value from the CRect.
    inline void CRect::operator-=(POINT pt) noexcept
    {
        ::OffsetRect(this, -pt.x, -pt.y);
    }

    // Subtracts the specified value from the CRect.
    inline void CRect::operator-=(SIZE sz) noexcept
    {
        ::OffsetRect(this, -sz.cx, -sz.cy);
    }

    // Determines the intersection with the specified RECT.
    inline void CRect::operator&=(RECT rc) noexcept
    {
        ::IntersectRect(this, this, &rc);
    }

    // Determines the union with the specified RECT.
    inline void CRect::operator|=(RECT rc) noexcept
    {
        ::UnionRect(this, this, &rc);
    }

    // Offsets the CRect and returns the result.
    inline CRect CRect::operator+(POINT pt) const noexcept
    {
        CRect rc(*this);
        ::OffsetRect(&rc, pt.x, pt.y);
        return rc;
    }

    // Offsets the CRect and returns the result.
    inline CRect CRect::operator-(POINT pt) const noexcept
    {
        CRect rc(*this);
        ::OffsetRect(&rc, -pt.x, -pt.y);
        return rc;
    }

    // Offsets the CRect and returns the result.
    inline CRect CRect::operator+(SIZE sz) const noexcept
    {
        CRect rc(*this);
        ::OffsetRect(&rc, sz.cx, sz.cy);
        return rc;
    }

    // Offsets the CRect and returns the result.
    inline CRect CRect::operator-(SIZE sz) const noexcept
    {
        CRect rc(*this);
        ::OffsetRect(&rc, -sz.cx, -sz.cy);
        return rc;
    }

    // Offsets the CRect and returns the result.
    inline CRect CRect::operator+(LPRECT prc) const noexcept
    {
        CRect rc1(*this);
        rc1.InflateRect(prc);
        return rc1;
    }

    // Offsets the CRect and returns the result.
    inline CRect CRect::operator-(LPRECT prc) const noexcept
    {
        CRect rc1(*this);
        rc1.DeflateRect(prc);
        return rc1;
    }

    // Returns the intersection with the specified RECT.
    inline CRect CRect::operator&(RECT rc) const noexcept
    {
        CRect rc1;
        ::IntersectRect(&rc1, this, &rc);
        return rc1;
    }

    // Returns the union with the specified RECT.
    inline CRect CRect::operator|(RECT rc) const noexcept
    {
        CRect rc1;
        ::UnionRect(&rc1, this, &rc);
        return rc1;
    }


    ///////////////////////////////////
    // Definitions for the CSize class.
    //

    // Sets the coordinates of the CSize.
    inline void CSize::SetSize(int sx, int sy) noexcept
    {
        cx = sx;
        cy = sy;
    }

    // Returns the pointer to the SIZE associated with this object.
    inline CSize::operator LPSIZE() noexcept
    {
        return this;
    }

    // Returns true if the co-ordinates of the source size and the CSize are
    // equal.
    inline bool CSize::operator==(CSize sz) const noexcept
    {
        return (cx == sz.cx && cy == sz.cy);
    }

    // Returns true if the co-ordinates of the source size and the CSize are
    // not equal.
    inline bool CSize::operator!=(CSize sz) const noexcept
    {
        return (cx != sz.cx || cy != sz.cy);
    }

    // Adds the specified SIZE.
    inline void CSize::operator+=(SIZE sz) noexcept
    {
        cx += sz.cx;
        cy += sz.cy;
    }

    // Subtracts the specified SIZE.
    inline void CSize::operator-=(SIZE sz) noexcept
    {
        cx -= sz.cx;
        cy -= sz.cy;
    }

    // Returns the unary minus (additive inverse).
    inline CSize CSize::operator-() const noexcept
    {
        return CSize(-cx, -cy);
    }

    // Adds the specified SIZE and returns the value.
    inline CSize CSize::operator+(SIZE sz) const noexcept
    {
        return CSize(cx + sz.cx, cy + sz.cy);
    }

    // Subtracts the specified SIZE and returns the value.
    inline CSize CSize::operator-(SIZE sz) const noexcept
    {
        return CSize(cx - sz.cx, cy - sz.cy);
    }

    // Adds the specified POINT and returns the value.
    inline CPoint CSize::operator+(POINT pt) const noexcept
    {
        return CPoint(cx + pt.x, cy + pt.y);
    }

    // Subtracts the specified POINT and returns the value.
    inline CPoint CSize::operator-(POINT pt) const noexcept
    {
        return CPoint(cx - pt.x, cy - pt.y);
    }

    // Adds the specified RECT and returns the value.
    inline CRect CSize::operator+(LPCRECT prc) const noexcept
    {
        return CRect(*prc) + *this;
    }

    // Subtracts the specified RECT and returns the value.
    inline CRect CSize::operator-(LPCRECT prc) const noexcept
    {
        return CRect(*prc) - *this;
    }

    ////////////////////
    // Global Functions.
    //

    // Returns a CPoint holding the current cursor position.
    inline CPoint GetCursorPos() noexcept
    {
        CPoint pt;
        if (!::GetCursorPos(&pt))
        {
            TRACE("GetCursorPos failed\n");
            pt.x = 0;
            pt.y = 0;
        }
        return pt;
    }

} // namespace Win32xx

