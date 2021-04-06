/////////////////////////////
// UserMessages.h
//

#ifndef _USER_MESSAGES_H_
#define _USER_MESSAGES_H_


// User defined messages used in this application.
#define UWM_WINDOWCREATED WM_APP +1    // the message sent when window is created.
#define UWM_TESTMESSAGE   WM_APP +2    // the test message.
#define UWM_CLOSETHREAD   WM_APP +3    // close the thread specified as wparam,
#define UWM_APPENDTEXT    WM_APP +4    // append text to main window. wparam is CString pointer.


#endif // _USER_MESSAGES_H_
