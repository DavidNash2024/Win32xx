// This file contains the resource ID definitions for Win32++.


// The resource ID for MENU, ICON, Toolbar Bitmap, Accelerator,
//  and Window Caption
#define IDW_MAIN                        51

// Resource ID for the About dialog
#define IDW_ABOUT                       52

// Resource IDs for menu items
#define IDW_VIEW_TOOLBAR                53
#define IDW_VIEW_STATUSBAR              54

// Resource IDs for the Command Bands
#define IDW_CMD_BANDS                   55
#define IDW_MENUBAR                     56
#define IDW_TOOLBAR                     57

// Resource ID for the Accelerator key
#define IDW_QUIT                        58

// Resource IDs for MDI menu items
#define IDW_WINDOW_CASCADE              60
#define IDW_WINDOW_TILE                 61
#define IDW_WINDOW_ARRANGE              62
#define IDW_WINDOW_CLOSEALL             63
#define IDW_FIRSTCHILD                  64
#define IDW_CHILD2                      65
#define IDW_CHILD3                      66
#define IDW_CHILD4                      67
#define IDW_CHILD5                      68
#define IDW_CHILD6                      69
#define IDW_CHILD7                      70
#define IDW_CHILD8                      71
#define IDW_CHILD9                      72
#define IDW_CHILD10                     73

// A generic ID for any static control
#ifndef IDC_STATIC
  #define IDC_STATIC                      -1
#endif


// Notes about Resource IDs
// * In general, resource IDs can have values from 1 to 65535. Programs with
//   resource IDs higher than 65535 aren't supported on Windows 95
//
// * CMenubar uses resource IDs beginning from 0 for the top level menu items.
//   Win32++ leaves resource IDs below 51 unallocated for top level menu items.
//
// * Windows uses the icon with the lowest resource ID as the application's
//   icon. The application's icon is IDW_MAIN, which is the first resource ID
//   defined by Win32++.
//
// * When more than one static control is used in a dialog, the controls should
//   have a unique ID, unless a resource ID of -1 is used.
//
// * Users of Win32++ are advised to begin their resource IDs from 100 to
//   allow for possible expansion of Win32++.


