@ECHO OFF
REM A batch file that creates and populates folders within 
rem %AppData%

ECHO Create the folders within %AppData%

ECHO.
ECHO Creating the Win32++ folder
mkdir "%AppData%\Win32++"

ECHO.
ECHO Creating the ContextHelp folder
mkdir "%AppData%\Win32++\ContextHelp"

ECHO.
ECHO Copy the files to the AppData folder
copy ContextHelp.chm "%AppData%\Win32++\ContextHelp" 

ECHO.
PAUSE
