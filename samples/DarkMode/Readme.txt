Dark Mode Example
=================
This project demonstrates the support dark mode with Win32++.

This is a frame based application which automatically detects dark mode, and
adjust its colors accordingly.

This code requires Visual Studio Community 2022 with the Windows App SDK
installed. The Windows App SDK can be installed using the VS2022 installer
as follows:
* Select Desktop development with C++.
* Then in the Installation details pane of the installation dialog box, 
  select Windows App SDK C++ Templates (at the bottom of the list).

More detailed instructions can be found on the following web site.
https://learn.microsoft.com/en-us/windows/apps/windows-app-sdk/set-up-your-development-environment

Some of the code in this sample is based on code provided here:
https://learn.microsoft.com/en-us/windows/apps/desktop/modernize/apply-windows-themes


This sample detects both high contrast themes and dark mode themes. Dark mode
themes that are high contrast behave differently to normal dark mode themes.

Dark high contrast automatically change the colours of ordinary dialogs. Normal
dark mode themes don't automatically change the colours of ordinary dialogs, 
but do change the colours of common dialogs such as the File Save dialog.




Features demonstrated in this example
=====================================
* How to detect when a dark mode theme is in use.
* How to detect when a high contrast theme is in use.
* How to automatically adjust the frame's colours to support dark mode.
* How to enable owner draw in the status bar for different text colours.

