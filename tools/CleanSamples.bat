
REM:A batch file to remove unnecessary files from
REM: each Visual Studio project

REM: Change the directory to the parent
pushd ..\Samples

::Remove directories
FOR /D %%f IN ("*.") DO RMDIR /S /Q "%%f\ProjectFiles\Borland"
FOR /D %%f IN ("*.") DO RMDIR /S /Q "%%f\ProjectFiles\Debug"
FOR /D %%f IN ("*.") DO RMDIR /S /Q "%%f\ProjectFiles\Debug_Build"
FOR /D %%f IN ("*.") DO RMDIR /S /Q "%%f\ProjectFiles\Dev-C++\Debug"
FOR /D %%f IN ("*.") DO RMDIR /S /Q "%%f\ProjectFiles\Dev-C++\Release"
FOR /D %%f IN ("*.") DO RMDIR /S /Q "%%f\ProjectFiles\GNU"
FOR /D %%f IN ("*.") DO RMDIR /S /Q "%%f\ProjectFiles\Microsoft"
FOR /D %%f IN ("*.") DO RMDIR /S /Q "%%f\ProjectFiles\Release"
FOR /D %%f IN ("*.") DO RMDIR /S /Q "%%f\ProjectFiles\Release_Build"
FOR /D %%f IN ("*.") DO RMDIR /S /Q "%%f\ProjectFiles\x64"
FOR /D %%f IN ("*.") DO RMDIR /S /Q "%%f\ProjectFiles\_UpgradeReport_Files"
FOR /D %%f IN ("*.") DO RMDIR /S /Q "%%f\ProjectFiles\__history"
FOR /D %%f IN ("*.") DO RMDIR /S /Q "%%f\ProjectFiles\ipch"
FOR /D %%f IN ("*.") DO RMDIR /S /Q "%%f\ProjectFiles\GNU64"
FOR /D %%f IN ("*.") DO RMDIR /S /Q "%%f\ProjectFiles\.vs
FOR /D %%f IN ("*.") DO RMDIR /S /Q "%%f\src\stdafx.h.gch"


::Remove files
FOR /D %%f IN ("*.") DO DEL /Q /AH "%%f\ProjectFiles\*.suo"
FOR /D %%f IN ("*.") DO DEL /Q /AH "%%f\ProjectFiles\*.old"
FOR /D %%f IN ("*.") DO DEL /Q "%%f\ProjectFiles\*.ncb"
FOR /D %%f IN ("*.") DO DEL /Q "%%f\ProjectFiles\*.plg"
FOR /D %%f IN ("*.") DO DEL /Q "%%f\ProjectFiles\err*.*"
FOR /D %%f IN ("*.") DO DEL /Q "%%f\ProjectFiles\tmp*.*"
FOR /D %%f IN ("*.") DO DEL /Q "%%f\ProjectFiles\*.pdb"
FOR /D %%f IN ("*.") DO DEL /Q "%%f\ProjectFiles\*.aps"
FOR /D %%f IN ("*.") DO DEL /Q "%%f\ProjectFiles\*.cbTemp"
FOR /D %%f IN ("*.") DO DEL /Q "%%f\ProjectFiles\*.opt"
FOR /D %%f IN ("*.") DO DEL /Q "%%f\ProjectFiles\*.user"
FOR /D %%f IN ("*.") DO DEL /Q "%%f\ProjectFiles\*.depend"
FOR /D %%f IN ("*.") DO DEL /Q "%%f\ProjectFiles\*.XML"
FOR /D %%f IN ("*.") DO DEL /Q "%%f\ProjectFiles\*.o"
FOR /D %%f IN ("*.") DO DEL /Q "%%f\ProjectFiles\*.old"
FOR /D %%f IN ("*.") DO DEL /Q "%%f\ProjectFiles\*.layout"
FOR /D %%f IN ("*.") DO DEL /Q "%%f\ProjectFiles\*.local"
FOR /D %%f IN ("*.") DO DEL /Q "%%f\ProjectFiles\*.log"
FOR /D %%f IN ("*.") DO DEL /Q "%%f\ProjectFiles\*.dat"
FOR /D %%f IN ("*.") DO DEL /Q "%%f\ProjectFiles\*.bak"
FOR /D %%f IN ("*.") DO DEL /Q "%%f\ProjectFiles\*.sdf"
FOR /D %%f IN ("*.") DO DEL /Q "%%f\ProjectFiles\*.vcb"
FOR /D %%f IN ("*.") DO DEL /Q "%%f\ProjectFiles\*.vcl"
FOR /D %%f IN ("*.") DO DEL /Q "%%f\ProjectFiles\*.vco"
FOR /D %%f IN ("*.") DO DEL /Q "%%f\ProjectFiles\Dev-C++\*.*"
FOR /D %%f IN ("*.") DO DEL /Q "%%f\ProjectFiles\*.db"
FOR /D %%f IN ("*.") DO DEL /Q "%%f\ProjectFiles\*private.*"
FOR /D %%f IN ("*.") DO DEL /Q "%%f\ProjectFiles\Makefile.win"
FOR /D %%f IN ("*.") DO DEL /Q "%%f\ProjectFiles\*.aps"
FOR /D %%f IN ("*.") DO DEL /Q "%%f\ProjectFiles\*.bak"
FOR /D %%f IN ("*.") DO DEL /Q "%%f\ProjectFiles\*.bml"
FOR /D %%f IN ("*.") DO DEL /Q "%%f\ProjectFiles\*.layout"
FOR /D %%f IN ("*.") DO DEL /Q "%%f\src\RibbonUI.h"
FOR /D %%f IN ("*.") DO DEL /Q "%%f\src\RibbonUI.rc"
FOR /D %%f IN ("*.") DO DEL /Q "%%f\src\*.aps"


REM: Pop the directory change off the stack
popd


