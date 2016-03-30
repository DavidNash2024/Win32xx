ECHO "Creating the folders within AppData"  > 	"Install.log"
mkdir "%AppData%\Win32++"
mkdir "%AppData%\Win32++\CommonDialogs"

ECHO "Copying the files to the AppData folder"  >> 	"Install.log"
copy CommonDialogs.chm "%AppData%\Win32++\CommonDialogs"    >> "Install.log"
copy CommonDialogs.arc "%AppData%\Win32++\CommonDialogs"    >> "Install.log

PAUSE
