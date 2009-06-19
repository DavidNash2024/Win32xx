
::A batch file to remove unnecessary files from
:: each Visual Studio project

::Remove directories
FOR /D %%f IN ("*.") DO RMDIR /S /Q %%f\"ProjectFiles\Borland"
FOR /D %%f IN ("*.") DO RMDIR /S /Q %%f\"ProjectFiles\Debug"
FOR /D %%f IN ("*.") DO RMDIR /S /Q %%f\"ProjectFiles\Debug_Build"
FOR /D %%f IN ("*.") DO RMDIR /S /Q %%f\"src\Dev-C++"
FOR /D %%f IN ("*.") DO RMDIR /S /Q %%f\"ProjectFiles\GNU"
FOR /D %%f IN ("*.") DO RMDIR /S /Q %%f\"ProjectFiles\Microsoft"
FOR /D %%f IN ("*.") DO RMDIR /S /Q %%f\"ProjectFiles\Release"
FOR /D %%f IN ("*.") DO RMDIR /S /Q %%f\"ProjectFiles\Release_Build"
FOR /D %%f IN ("*.") DO RMDIR /S /Q %%f\"ProjectFiles\x64"
FOR /D %%f IN ("*.") DO RMDIR /S /Q %%f\"ProjectFiles\_UpgradeReport_Files"
FOR /D %%f IN ("*.") DO RMDIR /S /Q %%f\"ProjectFiles\__history"


::Remove files
FOR /D %%f IN ("*.") DO DEL /Q %%f\"ProjectFiles\*.layout"
FOR /D %%f IN ("*.") DO DEL /Q %%f\"ProjectFiles\*.ncb"
FOR /D %%f IN ("*.") DO DEL /Q /AH %%f\"ProjectFiles\*.suo"
FOR /D %%f IN ("*.") DO DEL /Q %%f\"ProjectFiles\*.plg"
FOR /D %%f IN ("*.") DO DEL /Q %%f\"src\*private.*"
FOR /D %%f IN ("*.") DO DEL /Q %%f\"ProjectFiles\err*.*"
FOR /D %%f IN ("*.") DO DEL /Q %%f\"ProjectFiles\tmp*.*"
FOR /D %%f IN ("*.") DO DEL /Q %%f\"ProjectFiles\*.pdb"
FOR /D %%f IN ("*.") DO DEL /Q %%f\"ProjectFiles\*.aps"
FOR /D %%f IN ("*.") DO DEL /Q %%f\"ProjectFiles\*.opt"
FOR /D %%f IN ("*.") DO DEL /Q %%f\"ProjectFiles\*.user"
FOR /D %%f IN ("*.") DO DEL /Q %%f\"ProjectFiles\*.depend"
FOR /D %%f IN ("*.") DO DEL /Q %%f\"ProjectFiles\*.XML"
FOR /D %%f IN ("*.") DO DEL /Q %%f\"ProjectFiles\*.o"
FOR /D %%f IN ("*.") DO DEL /Q /AH %%f\"ProjectFiles\*.old"
FOR /D %%f IN ("*.") DO DEL /Q %%f\"ProjectFiles\*.old"
FOR /D %%f IN ("*.") DO DEL /Q %%f\"ProjectFiles\*.local"
FOR /D %%f IN ("*.") DO DEL /Q %%f\"ProjectFiles\*.dat"
FOR /D %%f IN ("*.") DO DEL /Q %%f\"src\Makefile.win"
FOR /D %%f IN ("*.") DO DEL /Q %%f\"ProjectFiles\*.bak"
FOR /D %%f IN ("*.") DO DEL /Q %%f\"ProjectFiles\*.vcb"
FOR /D %%f IN ("*.") DO DEL /Q %%f\"ProjectFiles\*.vcl"
FOR /D %%f IN ("*.") DO DEL /Q %%f\"ProjectFiles\*.vco"


