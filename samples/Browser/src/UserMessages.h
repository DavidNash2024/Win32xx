/////////////////////////////
// UserMessages.h
//

#ifndef _USER_MESSAGES_H_
#define _USER_MESSAGES_H_


// User defined messages used in this application.
#define UWM_ADDCOMBOBAND          (WM_APP + 0x0001)
#define UWM_HISTORYCHANGED        (WM_APP + 0x0002)
#define UWM_NAVIGATIONCOMPLETED   (WM_APP + 0x0003)
#define UWM_NAVIGATIONSTARTED     (WM_APP + 0x0004)
#define UWM_SOURCECHANGED         (WM_APP + 0x0005)

// UWM_ADDCOMBOBAND parameters
//   wparam - This parameter is not used.
//   lparam - This parameter is not used.

// UWM_HISTORYCHANGED parameters
//   wparam - This parameter is not used.
//   lparam - This parameter is not used.

// UWM_NAVIGATIONCOMPLETED parameters
//   wparam - is a BOOL value, TRUE for navigation success, FALSE otherwise.
//   lparam - This parameter is not used.

// UWM_NAVIGATIONSTARTED parameters
//   wparam - This parameter is not used.
//   lparam - This parameter is not used.

// UWM_SOURCECHANGED parameters
//   wparam - This parameter is not used.
//   lparam - Pointer to a null-terminated string that is webview's source URI.


#endif // _USER_MESSAGES_H_
