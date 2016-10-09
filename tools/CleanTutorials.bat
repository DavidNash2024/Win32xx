
::A batch file to remove unnecessary files from
:: each Visual Studio project in Tutorials

REM: Change the directory to the parent
pushd ..\Tutorials


::Remove directories
FOR /D %%f IN ("*.") DO RMDIR /S /Q %%f\Borland
FOR /D %%f IN ("*.") DO RMDIR /S /Q %%f\Debug
FOR /D %%f IN ("*.") DO RMDIR /S /Q %%f\Debug_Build
FOR /D %%f IN ("*.") DO RMDIR /S /Q %%f\Dev-C++\Debug
FOR /D %%f IN ("*.") DO RMDIR /S /Q %%f\Dev-C++\Release
FOR /D %%f IN ("*.") DO RMDIR /S /Q %%f\GNU
FOR /D %%f IN ("*.") DO RMDIR /S /Q %%f\Microsoft
FOR /D %%f IN ("*.") DO RMDIR /S /Q %%f\Release
FOR /D %%f IN ("*.") DO RMDIR /S /Q %%f\Release_Build
FOR /D %%f IN ("*.") DO RMDIR /S /Q %%f\x64
FOR /D %%f IN ("*.") DO RMDIR /S /Q %%f\_UpgradeReport_Files
FOR /D %%f IN ("*.") DO RMDIR /S /Q %%f\__history
FOR /D %%f IN ("*.") DO RMDIR /S /Q %%f\ipch
FOR /D %%f IN ("*.") DO RMDIR /S /Q %%f\.vs


::Remove files
FOR /D %%f IN ("*.") DO DEL /Q %%f\"*.layout"
FOR /D %%f IN ("*.") DO DEL /Q %%f\"*.ncb"
FOR /D %%f IN ("*.") DO DEL /Q /AH %%f\"*.suo"
FOR /D %%f IN ("*.") DO DEL /Q %%f\"*.plg"
FOR /D %%f IN ("*.") DO DEL /Q %%f\"*private.*"
FOR /D %%f IN ("*.") DO DEL /Q %%f\"err*.*"
FOR /D %%f IN ("*.") DO DEL /Q %%f\"tmp*.*"
FOR /D %%f IN ("*.") DO DEL /Q %%f\"*.pdb"
FOR /D %%f IN ("*.") DO DEL /Q %%f\"*.aps"
FOR /D %%f IN ("*.") DO DEL /Q %%f\"*.opt"
FOR /D %%f IN ("*.") DO DEL /Q %%f\"*.user"
FOR /D %%f IN ("*.") DO DEL /Q %%f\"*.depend"
FOR /D %%f IN ("*.") DO DEL /Q %%f\"*.XML"
FOR /D %%f IN ("*.") DO DEL /Q %%f\"*.o"
FOR /D %%f IN ("*.") DO DEL /Q /AH %%f\"*.old"
FOR /D %%f IN ("*.") DO DEL /Q %%f\"*.old"
FOR /D %%f IN ("*.") DO DEL /Q %%f\"*.local"
FOR /D %%f IN ("*.") DO DEL /Q %%f\"*.dat"
FOR /D %%f IN ("*.") DO DEL /Q %%f\"Makefile.win"
FOR /D %%f IN ("*.") DO DEL /Q %%f\"*.bak"
FOR /D %%f IN ("*.") DO DEL /Q %%f\"*.vcb"
FOR /D %%f IN ("*.") DO DEL /Q %%f\"*.vcl"
FOR /D %%f IN ("*.") DO DEL /Q %%f\"*.vco"
FOR /D %%f IN ("*.") DO DEL /Q %%f\"*.log"
FOR /D %%f IN ("*.") DO DEL /Q %%f\"*.sdf"
FOR /D %%f IN ("*.") DO DEL /Q %%f\"*.db"
FOR /D %%f IN ("*.") DO DEL /Q %%f\"Dev-C++\*.*"


popd

