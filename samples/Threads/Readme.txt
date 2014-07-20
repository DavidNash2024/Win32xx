Threads Example
==============
This project demonstrates the ability of Win32++ to create windows in
different threads. Each thread containing a window runs a message loop.

The CMyThread class inherits from CWinThread. After this class is instantiated, 
CreateThread is used to create the thread. When the thread runs, the 
InitInstance function is called and the TestWindow is created.


Features demonstrated in this example
=====================================
* How to use the CThread class to create threads
* How to create a thread that is initially suspended
* How to create windows in different threads
* Using PostMessage to communicate information between threads
* Using PostQuitMessage to end a GUI thread
* Using an Edit control as a child window to display text

