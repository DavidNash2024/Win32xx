#include <Windowsx.h>
#include <windows.h>
#include <tchar.h>

#include <dwmapi.h>
#pragma comment(lib,"Dwmapi")

// https://docs.microsoft.com/en-us/windows/desktop/dwm/customframe

HINSTANCE hInst;
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
int nWidth = 600, nHeight = 400;

#define RECTWIDTH(rc)            (rc.right - rc.left)
#define RECTHEIGHT(rc)            (rc.bottom - rc.top)

const int TOPEXTENDWIDTH = 32;
const int LEFTEXTENDWIDTH = 8;
const int RIGHTEXTENDWIDTH = 8;
const int BOTTOMEXTENDWIDTH = 8;

LRESULT HitTestNCA(HWND hWnd, WPARAM wParam, LPARAM lParam);

int APIENTRY _tWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow)
{
    hInst = hInstance;
    WNDCLASSEX wcex =
    {
        sizeof(WNDCLASSEX), CS_HREDRAW | CS_VREDRAW, WndProc, 0, 0, hInst, LoadIcon(NULL, IDI_APPLICATION),
        LoadCursor(NULL, IDC_ARROW), (HBRUSH)GetStockObject(BLACK_BRUSH), NULL, TEXT("WindowClass"), NULL,
    };
    if (!RegisterClassEx(&wcex))
        return MessageBox(NULL, L"Cannot register class !", L"Error", MB_ICONERROR | MB_OK);
    int nX = (GetSystemMetrics(SM_CXSCREEN) - nWidth) / 2, nY = (GetSystemMetrics(SM_CYSCREEN) - nHeight) / 2;
    HWND hWnd = CreateWindowEx(0, wcex.lpszClassName, TEXT("Test"), WS_OVERLAPPEDWINDOW, nX, nY, nWidth, nHeight, NULL, NULL, hInst, NULL);
    if (!hWnd)
        return MessageBox(NULL, L"Cannot create window !", L"Error", MB_ICONERROR | MB_OK);

    ShowWindow(hWnd, SW_SHOWNORMAL);
    UpdateWindow(hWnd);
    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return (int)msg.wParam;
}

LRESULT CustomCaptionProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam, bool* pfCallDWP)
{
    LRESULT lRet = 0;
    HRESULT hr = S_OK;
    bool fCallDWP = true; // Pass on to DefWindowProc?
    static HICON hIcon = NULL;

    fCallDWP = !DwmDefWindowProc(hWnd, message, wParam, lParam, &lRet);
    if (message == WM_CREATE)
    {
        RECT rcClient;
        GetWindowRect(hWnd, &rcClient);
        // Inform the application of the frame change.
        SetWindowPos(hWnd, NULL, rcClient.left, rcClient.top, RECTWIDTH(rcClient), RECTHEIGHT(rcClient), SWP_FRAMECHANGED);

        HMODULE hDLL = LoadLibrary(L"Setupapi.dll");
        if (hDLL)
        {
            hIcon = (HICON)LoadImage(hDLL, MAKEINTRESOURCE(2),    IMAGE_ICON, 32, 32, LR_DEFAULTCOLOR | LR_SHARED);
        }

        fCallDWP = true;
        lRet = 0;
    }

    // Handle window activation.
    if (message == WM_ACTIVATE)
    {
        // Extend the frame into the client area.
        MARGINS margins;
        margins.cxLeftWidth = LEFTEXTENDWIDTH;
        margins.cxRightWidth = RIGHTEXTENDWIDTH;
        margins.cyBottomHeight = BOTTOMEXTENDWIDTH;
        margins.cyTopHeight = TOPEXTENDWIDTH;
        hr = DwmExtendFrameIntoClientArea(hWnd, &margins);
        if (!SUCCEEDED(hr))
        {
            // Handle error.
        }

        fCallDWP = true;
        lRet = 0;
    }

    if (message == WM_PAINT)
    {
        PAINTSTRUCT ps;
        BITMAP bm;
        RECT rect, rectCaptionButtonBounds, rectText;

        BeginPaint(hWnd, &ps);
        if (SUCCEEDED(DwmGetWindowAttribute(hWnd, DWMWA_CAPTION_BUTTON_BOUNDS, &rectCaptionButtonBounds, sizeof(rectCaptionButtonBounds))))
        {
            GetClientRect(hWnd, &rect);
            DrawIconEx(ps.hdc, rect.right - (rectCaptionButtonBounds.right - rectCaptionButtonBounds.left) - 32, 0, hIcon, 32, 32, 0, NULL, DI_NORMAL);
            SetBkMode(ps.hdc, TRANSPARENT);
            SetTextColor(ps.hdc, RGB(255, 255, 0));
            WCHAR wsText[255] = L"Test Custom Caption";
            SetRect(&rectText, LEFTEXTENDWIDTH, 0, RECTWIDTH(rect), TOPEXTENDWIDTH);
            DrawText(ps.hdc, wsText, -1, &rectText, DT_SINGLELINE | DT_VCENTER);
        }
        EndPaint(hWnd, &ps);

        fCallDWP = true;
        lRet = 0;
    }

    // Handle the non-client size message.
    if ((message == WM_NCCALCSIZE) && (wParam == TRUE))
    {
        // Calculate new NCCALCSIZE_PARAMS based on custom NCA inset.
        /*NCCALCSIZE_PARAMS *pncsp = reinterpret_cast<NCCALCSIZE_PARAMS*>(lParam);

        pncsp->rgrc[0].left = pncsp->rgrc[0].left + 0;
        pncsp->rgrc[0].top = pncsp->rgrc[0].top + 0;
        pncsp->rgrc[0].right = pncsp->rgrc[0].right - 0;
        pncsp->rgrc[0].bottom = pncsp->rgrc[0].bottom - 0;*/

        lRet = 0;
        // No need to pass the message on to the DefWindowProc.
        fCallDWP = false;
    }

    // Handle hit testing in the NCA if not handled by DwmDefWindowProc.
    if ((message == WM_NCHITTEST) && (lRet == 0))
    {
        lRet = HitTestNCA(hWnd, wParam, lParam);

        if (lRet != HTNOWHERE)
        {
            fCallDWP = false;
        }
    }
    if (message == WM_DESTROY)
        PostQuitMessage(0);

    *pfCallDWP = fCallDWP;

    return lRet;
}

// Hit test the frame for resizing and moving.
LRESULT HitTestNCA(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
    // Get the point coordinates for the hit test.
    POINT ptMouse = { GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam) };

    // Get the window rectangle.
    RECT rcWindow;
    GetWindowRect(hWnd, &rcWindow);

    // Get the frame rectangle, adjusted for the style without a caption.
    RECT rcFrame = { 0 };
    AdjustWindowRectEx(&rcFrame, WS_OVERLAPPEDWINDOW & ~WS_CAPTION, FALSE, NULL);

    // Determine if the hit test is for resizing. Default middle (1,1).
    USHORT uRow = 1;
    USHORT uCol = 1;
    bool fOnResizeBorder = false;

    // Determine if the point is at the top or bottom of the window.
    if (ptMouse.y >= rcWindow.top && ptMouse.y < rcWindow.top + TOPEXTENDWIDTH)
    {
        fOnResizeBorder = (ptMouse.y < (rcWindow.top - rcFrame.top));
        uRow = 0;
    }
    else if (ptMouse.y < rcWindow.bottom && ptMouse.y >= rcWindow.bottom - BOTTOMEXTENDWIDTH)
    {
        uRow = 2;
    }

    // Determine if the point is at the left or right of the window.
    if (ptMouse.x >= rcWindow.left && ptMouse.x < rcWindow.left + LEFTEXTENDWIDTH)
    {
        uCol = 0; // left side
    }
    else if (ptMouse.x < rcWindow.right && ptMouse.x >= rcWindow.right - RIGHTEXTENDWIDTH)
    {
        uCol = 2; // right side
    }

    // Hit test (HTTOPLEFT, ... HTBOTTOMRIGHT)
    LRESULT hitTests[3][3] =
    {
        { HTTOPLEFT,    fOnResizeBorder ? HTTOP : HTCAPTION,    HTTOPRIGHT },
        { HTLEFT,       HTNOWHERE,     HTRIGHT },
        { HTBOTTOMLEFT, HTBOTTOM, HTBOTTOMRIGHT },
    };

    return hitTests[uRow][uCol];
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    bool fCallDWP = true;
    BOOL fDwmEnabled = FALSE;
    LRESULT lRet = 0;
    HRESULT hr = S_OK;

    // Winproc worker for custom frame issues.
    hr = DwmIsCompositionEnabled(&fDwmEnabled);
    if (SUCCEEDED(hr))
    {
        lRet = CustomCaptionProc(hWnd, message, wParam, lParam, &fCallDWP);
    }

    // Winproc worker for the rest of the application.
    if (fCallDWP)
    {
        //    lRet = AppWinProc(hWnd, message, wParam, lParam);
        lRet = DefWindowProc(hWnd, message, wParam, lParam);
    }
    return lRet;
}