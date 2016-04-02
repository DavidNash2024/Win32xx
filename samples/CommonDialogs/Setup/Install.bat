@ECHO OFF
REM A batch file that creates folders within %AppData% and copies 2 files there.

ECHO Create the folders within AppData

ECHO.
ECHO Creating the Win32++ folder
mkdir "%AppData%\Win32++"

ECHO.
ECHO Creating the CommonDialogs folder
mkdir "%AppData%\Win32++\CommonDialogs"

ECHO.
ECHO Copy the files to the AppData folder
copy CommonDialogs.chm "%AppData%\Win32++\CommonDialogs" 
copy CommonDialogs.arc "%AppData%\Win32++\CommonDialogs"

ECHO.
PAUSE
