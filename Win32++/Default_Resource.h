// This file contains the resource ID definitions for Win32++.


// The resource ID for MENU, ICON, Toolbar Bitmap, Accelerator,
//  and Window Caption
#define IDW_MAIN                        1

// Resource ID for the About dialog
#define IDW_ABOUT                       2

// Resource IDs for menu items
#define IDW_VIEW_TOOLBAR                3
#define IDW_VIEW_STATUSBAR              4

// Resource IDs for MDI menu items
#define IDW_WINDOW_CASCADE              5
#define IDW_WINDOW_TILE                 6
#define IDW_WINDOW_ARRANGE              7
#define IDW_WINDOW_CLOSEALL             8
#define IDW_FIRSTCHILD                  9
#define IDW_CHILD2                      10
#define IDW_CHILD3                      11
#define IDW_CHILD4                      12
#define IDW_CHILD5                      13
#define IDW_CHILD6                      14
#define IDW_CHILD7                      15
#define IDW_CHILD8                      16
#define IDW_CHILD9                      17

// A generic ID for any static control
#define IDW_STATIC                      -1


// Notes about Resource IDs
// * In general, resource IDs can have values from 1 to 65535. Programs with
//   resource IDs higher than 65535 aren't supported on Windows 95
//
// * Windows uses the icon with the lowest resource ID as the applications icon
//   That is why the resource ID of 1 is used for IDW_MAIN
//
// * When more than one static control is used in a dialog, the controls should
//   have a unique ID, unless a resource ID of -1 is used.
//
// * Users of Win32++ are advised to begin their resource IDs from 100 to
//   allow for possible expansion of Win32++.


