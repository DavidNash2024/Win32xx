 // SimpleWin32Prj.cpp : Defines the entry point for the application.
  //

  // The major portion of the code can be found here:
  // http://msdn.microsoft.com/en-us/library/bb688195(VS.85).aspx

  #include "stdafx.h"
  #include "SimpleWin32Prj.h"

  #include "tmschema.h"

  #include "UXTheme.h"

  #include "dwmapi_proxy.h"
  #include "UxThemeEx.h"

  #define MAX_LOADSTRING 100

  #define LEFTEXTENDWIDTH        8
  #define RIGHTEXTENDWIDTH    8
  #define BOTTOMEXTENDWIDTH    20
  #define TOPEXTENDWIDTH      161

  #define RECTWIDTH(r)    r.right - r.left
  #define RECTHEIGHT(r)    r.bottom - r.top

  #define BIT_COUNT 32

  #ifndef GET_X_LPARAM
  #define GET_X_LPARAM(lp)                        ((int)(short)LOWORD(lp))
  #endif
  #ifndef GET_Y_LPARAM
  #define GET_Y_LPARAM(lp)                        ((int)(short)HIWORD(lp))
  #endif

  //CMFCDib gMFCDib;

  // Global Variables:
  HINSTANCE hInst;                                // current instance
  TCHAR szTitle[MAX_LOADSTRING];                    // The title bar text
  TCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name

  // Forward declarations of functions included in this code module:
  ATOM                MyRegisterClass(HINSTANCE hInstance);
  BOOL                InitInstance(HINSTANCE, int);
  LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
  INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

  LRESULT CustomCaptionProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam, bool* pfCallDWP);
  LRESULT AppWinProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

  void PaintCustomCaption(HWND hWnd, HDC hdc);

  LRESULT HitTestNCA(HWND hWnd, WPARAM wParam, LPARAM lParam);

  int APIENTRY _tWinMain(HINSTANCE hInstance,
                       HINSTANCE hPrevInstance,
                       LPTSTR    lpCmdLine,
                       int       nCmdShow)
  {
      UNREFERENCED_PARAMETER(hPrevInstance);
      UNREFERENCED_PARAMETER(lpCmdLine);

       // TODO: Place code here.
      MSG msg;
      HACCEL hAccelTable;

      // Initialize global strings
      LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
      LoadString(hInstance, IDC_SIMPLEWIN32PRJ, szWindowClass, MAX_LOADSTRING);
      MyRegisterClass(hInstance);

      // Perform application initialization:
      if (!InitInstance (hInstance, nCmdShow))
      {
          return FALSE;
      }

      hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_SIMPLEWIN32PRJ));

      // Main message loop:
      while (GetMessage(&msg, NULL, 0, 0))
      {
          if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
          {
              TranslateMessage(&msg);
              DispatchMessage(&msg);
          }
      }

      return (int) msg.wParam;
  }



  //
  //  FUNCTION: MyRegisterClass()
  //
  //  PURPOSE: Registers the window class.
  //
  //  COMMENTS:
  //
  //    This function and its usage are only necessary if you want this code
  //    to be compatible with Win32 systems prior to the 'RegisterClassEx'
  //    function that was added to Windows 95. It is important to call this function
  //    so that the application will get 'well formed' small icons associated
  //    with it.
  //
  ATOM MyRegisterClass(HINSTANCE hInstance)
  {
      WNDCLASSEX wcex;

      wcex.cbSize = sizeof(WNDCLASSEX);

      wcex.style            = CS_HREDRAW | CS_VREDRAW;
      wcex.lpfnWndProc    = WndProc;
      wcex.cbClsExtra        = 0;
      wcex.cbWndExtra        = 0;
      wcex.hInstance        = hInstance;
      wcex.hIcon            = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_SIMPLEWIN32PRJ));
      wcex.hCursor        = LoadCursor(NULL, IDC_ARROW);
      wcex.hbrBackground    = (HBRUSH)(COLOR_WINDOW+1);
      wcex.lpszMenuName    = NULL;//MAKEINTRESOURCE(IDC_SIMPLEWIN32PRJ);
      wcex.lpszClassName    = szWindowClass;
      wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

      return RegisterClassEx(&wcex);
  }

  //
  //   FUNCTION: InitInstance(HINSTANCE, int)
  //
  //   PURPOSE: Saves instance handle and creates main window
  //
  //   COMMENTS:
  //
  //        In this function, we save the instance handle in a global variable and
  //        create and display the main program window.
  //
  BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
  {
     HWND hWnd;

     hInst = hInstance; // Store instance handle in our global variable

     hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);

     if (!hWnd)
     {
        return FALSE;
     }

     ShowWindow(hWnd, nCmdShow);
     UpdateWindow(hWnd);

     return TRUE;
  }

  //
  //  Main WinProc.
  //
  //
  //  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
  //
  //  PURPOSE:  Processes messages for the main window.
  //
  //  WM_COMMAND    - process the application menu
  //  WM_PAINT    - Paint the main window
  //  WM_DESTROY    - post a quit message and return
  //
  //

  LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
  {
      bool fCallDWP = true;
      BOOL fDwmEnabled = FALSE;
      LRESULT lRet = 0;

      // Winproc worker for custom frame issues.
      if (SUCCEEDED(DwmIsCompositionEnabled(&fDwmEnabled))){
          lRet = CustomCaptionProc(hWnd, message, wParam, lParam, &fCallDWP);
      }

      // Winproc worker for the rest of the application.
      if (fCallDWP)
      {
          lRet = AppWinProc(hWnd, message, wParam, lParam);
      }

      return lRet;
  }

  //
  // Message handler for handling the custom caption messages.
  //
  LRESULT CustomCaptionProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam, bool* pfCallDWP)
  {
      LRESULT lRet = 0;
      HRESULT hr = S_OK;
      bool fCallDWP = true; // Pass on to DefWindowProc?

      PAINTSTRUCT ps;

      fCallDWP = !DwmDefWindowProc(hWnd, message, wParam, lParam, &lRet);

      // Handle window creation.
      if (message == WM_CREATE)
      {
          RECT rcClient;
          GetWindowRect(hWnd, &rcClient);

          // Inform application of the frame change.
          SetWindowPos(
              hWnd,
              NULL,
              rcClient.left, rcClient.top,
              RECTWIDTH(rcClient), RECTHEIGHT(rcClient),
              SWP_FRAMECHANGED);

          fCallDWP = true;
          lRet = 0;
      }

      // Handle window activation.
      if (message == WM_ACTIVATE)
      {
          // Extend the frame into the client area.
          MARGINS margins;

          margins.cxLeftWidth = LEFTEXTENDWIDTH;      //8
          margins.cxRightWidth = RIGHTEXTENDWIDTH;    //8
          margins.cyBottomHeight = BOTTOMEXTENDWIDTH; //20
          margins.cyTopHeight = TOPEXTENDWIDTH;       //27

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
          HDC hdc;
          {
              hdc = BeginPaint(hWnd, &ps);

              PaintCustomCaption(hWnd, hdc);

              RECT rc;
              GetClientRect(hWnd, &rc);
              rc.top += 200;
              SetBkColor(hdc, RGB(255, 0, 0));
              SetBkMode(hdc, OPAQUE);
              DrawText(hdc, _T("Custom Drawing"), -1, &rc, DT_SINGLELINE | DT_CENTER);

              EndPaint(hWnd, &ps);
          }

          fCallDWP = true;
          lRet = 0;
      }

      // Handle the non-client size message.
      if ((message == WM_NCCALCSIZE) && (wParam == TRUE))
      {
          // Calculate new NCCALCSIZE_PARAMS based on custom NCA inset.
          NCCALCSIZE_PARAMS *pncsp = reinterpret_cast<NCCALCSIZE_PARAMS*>(lParam);

          pncsp->rgrc[0].left   = pncsp->rgrc[0].left   + 0;
          pncsp->rgrc[0].top    = pncsp->rgrc[0].top    + 0;
          pncsp->rgrc[0].right  = pncsp->rgrc[0].right  - 0;
          pncsp->rgrc[0].bottom = pncsp->rgrc[0].bottom - 0;

          lRet = 0;
          // No need to pass message on to the DefWindowProc.
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

      *pfCallDWP = fCallDWP;

      return lRet;
  }

  //
  // Message handler the application.
  //
  LRESULT AppWinProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
  {
      int wmId, wmEvent;
      PAINTSTRUCT ps;
      HDC hdc;
      HRESULT hr;
      LRESULT result = 0;

      switch (message)
      {
      case WM_COMMAND:
          wmId    = LOWORD(wParam);
          wmEvent = HIWORD(wParam);
          // Parse the menu selections:
          switch (wmId)
          {
          default:
              return DefWindowProc(hWnd, message, wParam, lParam);
          }
          break;
      case WM_DESTROY:
          PostQuitMessage(0);
          break;
      default:
          return DefWindowProc(hWnd, message, wParam, lParam);
      }
      return 0;
  }

  // Paint the title on the custom frame.

  void PaintCustomCaption(HWND hWnd, HDC hdc)
  {
      RECT rcClient;
      GetClientRect(hWnd, &rcClient);

      HTHEME hTheme = OpenThemeData(NULL, L"CompositedWindow::Window");
      if (hTheme)
      {
          HDC hdcPaint = CreateCompatibleDC(hdc);

          HDC hdcRes = CreateCompatibleDC(hdc);

          //DWORD dwTransparencyFactor =  0x7F000000; // Semi Transparent, the most significant bits (i.e 7F) controls the transparency
          DWORD dwTransparencyFactor = 0xFF000000; // Fully Opaque

          HBITMAP hBmpRes = LoadDIBSectionFromFile(_T("Bitmap32.bmp"), 0, 0, 0, TRUE, RGB(255, 255, 255), dwTransparencyFactor);

          SIZE szBmpRes;
          BITMAP  rBitmap;
          ::GetObject (hBmpRes, sizeof (BITMAP), &rBitmap);
          szBmpRes.cx = rBitmap.bmWidth;
          szBmpRes.cy = rBitmap.bmHeight;

          HBITMAP hOldBmpRes = (HBITMAP)SelectObject(hdcRes, (HBITMAP)hBmpRes);

          if (hdcPaint)
          {
              int cx = RECTWIDTH(rcClient);
              int cy = RECTHEIGHT(rcClient);

              // Define the BITMAPINFO structure used to draw text.
              // Note that biHeight is negative. This is done because
              // DrawThemeTextEx() needs the bitmap to be in top-to-bottom
              // order.
              BITMAPINFO dib = { 0 };
              dib.bmiHeader.biSize            = sizeof(BITMAPINFOHEADER);
              dib.bmiHeader.biWidth           = cx;
              dib.bmiHeader.biHeight          = -cy;
              dib.bmiHeader.biPlanes          = 1;
              dib.bmiHeader.biBitCount        = BIT_COUNT;
              dib.bmiHeader.biCompression     = BI_RGB;


              HBITMAP hbm = CreateDIBSection(hdc, &dib, DIB_RGB_COLORS, NULL, NULL, 0);

              if (hbm)
              {
                  HBITMAP hbmOld = (HBITMAP)SelectObject(hdcPaint, hbm);

                  // Setup the theme drawing options.
                  DTTOPTS DttOpts = {sizeof(DTTOPTS)};
                  DttOpts.dwFlags = DTT_COMPOSITED | DTT_GLOWSIZE;
                  DttOpts.iGlowSize = 15;

                  // Select a font.
                  LOGFONT lgFont;
                  HFONT hFontOld = NULL;
                  HFONT hFont = NULL;
                  if (SUCCEEDED(GetThemeSysFont(hTheme, TMT_CAPTIONFONT, &lgFont)))
                  {
                      hFont = CreateFontIndirect(&lgFont);
                      if(hFont)
                      {
                          hFontOld = (HFONT) SelectObject(hdcPaint, hFont);
                      }
                  }

                  // Draw the bitmap
                  BitBlt(hdcPaint, 0, 0, cx, cy, hdcRes, 0, 0, SRCCOPY);

                  // Draw the title.
                  RECT rcPaint = rcClient;
                  rcPaint.top += 8;
                  rcPaint.left += szBmpRes.cx + 2;

                  // Blit text to the frame.
                  DrawThemeTextEx(hTheme, hdcPaint, 0, 0, szTitle, -1, DT_LEFT | DT_WORD_ELLIPSIS, &rcPaint, &DttOpts);

                  BitBlt(hdc, 0, 0, cx, cy, hdcPaint, 0, 0, SRCCOPY);

                  SelectObject(hdcPaint, hbmOld);

                  if (hFont)
                  {
                      SelectObject(hdcPaint, hFontOld);
                      DeleteObject(hFont);
                  }

                  DeleteObject(hbm);
              }

              DeleteDC(hdcPaint);

          }

          SelectObject(hdcRes, (HBITMAP)hOldBmpRes);
          DeleteObject(hBmpRes);

          DeleteDC(hdcRes);

          CloseThemeData(hTheme);
      }
  }


  // Hit test the frame for resizing and moving.
  LRESULT HitTestNCA(HWND hWnd, WPARAM wParam, LPARAM lParam)
  {
      // Get point for the hit test.
      POINT ptMouse = { GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam)};

      // Get the window rectangle.
      RECT rcWindow;
      GetWindowRect(hWnd, &rcWindow);

      // Get frame rectangle, adjusted for the style without a caption.
      RECT rcFrame = { 0 };
      AdjustWindowRectEx(&rcFrame, WS_OVERLAPPEDWINDOW & ~WS_CAPTION, FALSE, NULL);

      // Determine if hit test is for resizing, default middle (1,1).
      USHORT uRow = 1;
      USHORT uCol = 1;
      bool fOnResizeBorder = false;

      // Determine if the point is at the top or bottom of the window.
      if ( ptMouse.y >= rcWindow.top && ptMouse.y < rcWindow.top + TOPEXTENDWIDTH)
      {
          fOnResizeBorder = (ptMouse.y < (rcWindow.top - rcFrame.top));
          uRow = 0;
      }
      else if ( ptMouse.y < rcWindow.bottom && ptMouse.y >= rcWindow.bottom - BOTTOMEXTENDWIDTH)
      {
          uRow = 2;
      }

      // Determine if the point is at the left or right of the window.
      if ( ptMouse.x >= rcWindow.left && ptMouse.x < rcWindow.left + LEFTEXTENDWIDTH)
      {
          uCol = 0; //left side
      }
      else if ( ptMouse.x < rcWindow.right && ptMouse.x >= rcWindow.right - RIGHTEXTENDWIDTH)
      {
          uCol = 2; //right side
      }

      // Hit test (HTTOPLEFT, ... HTBOTTOMRIGHT)
      LRESULT hitTests[3][3] =
      {
          { HTTOPLEFT,    fOnResizeBorder ? HTTOP : HTCAPTION,    HTTOPRIGHT    },
          { HTLEFT,       HTNOWHERE,     HTRIGHT       },
          { HTBOTTOMLEFT, HTBOTTOM, HTBOTTOMRIGHT },
      };

      return hitTests[uRow][uCol];
  }


  // Message handler for about box.
  INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
  {
      UNREFERENCED_PARAMETER(lParam);
      switch (message)
      {
      case WM_INITDIALOG:
          return (INT_PTR)TRUE;

      case WM_COMMAND:
          if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
          {
              EndDialog(hDlg, LOWORD(wParam));
              return (INT_PTR)TRUE;
          }
          break;
      }
      return (INT_PTR)FALSE;
  }