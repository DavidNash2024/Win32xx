// This file contains the resource ID definitions for Win32++.


// The resource ID for MENU, ICON, Toolbar Bitmap, Accelerator,
//  and Window Caption
#define IDW_MAIN                        51

// Resource ID for the About dialog
#define IDW_ABOUT                       52

// Resource IDs for menu items
#define IDW_VIEW_TOOLBAR                53
#define IDW_VIEW_STATUSBAR              54

// Resource IDs for MDI menu items
#define IDW_WINDOW_CASCADE              55
#define IDW_WINDOW_TILE                 56
#define IDW_WINDOW_ARRANGE              57
#define IDW_WINDOW_CLOSEALL             58
#define IDW_FIRSTCHILD                  59
#define IDW_CHILD2                      60
#define IDW_CHILD3                      61
#define IDW_CHILD4                      62
#define IDW_CHILD5                      63
#define IDW_CHILD6                      64
#define IDW_CHILD7                      65
#define IDW_CHILD8                      66
#define IDW_CHILD9                      67

// A generic ID for any static control
#define IDW_STATIC                      -1


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


