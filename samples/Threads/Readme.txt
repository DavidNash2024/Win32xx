Threads Example
==============
This project demonstrates the ability of Win32++ to create windows in
different threads. Each thread containing a window runs a message loop.

The CMyThread class inherits from CWinThread. After this class is instantiated, 
CreateThread is used to create the thread. When the thread runs, the 
InitInstance function is called and the TestWindow is created.

When a TestWindow is closed, its thread is ended.


Features demonstrated in this example
=====================================
* How to use the CThread class to create threads.
* How to create a thread that is initially suspended.
* How to create windows in different threads.
* How to use PostMessage to communicate information between threads.
* The use of smart pointers for the CMyThread objects.
* The use of CCriticalSection and CThreadLock to ensure thread safety.
* The use of PostThreadMessage to post a WM_QUIT and end a GUI thread.
* The use of PostQuitMessage to end the main thread.
* The use of WaitForSingleObject to synchronise threads.
* How to use an edit control as a child window to display text.

