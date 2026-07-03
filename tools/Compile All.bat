REM  Clean the projects folders.
call "CleanSamples.bat"
call "CleanTutorials.bat"

cmd /c call "Compile VS2022 Community.bat"

REM  Clean the projects folders.
call "CleanSamples.bat"
call "CleanTutorials.bat"

REM  Run these compile scripts concurrently.
start "Building CodeBlocks" cmd /c ""Compile CodeBlocks.bat""
start "Building C++ Builder" cmd /c ""Compile C++ Builder.bat""
start "Building VS2026 Community" cmd /c ""Compile VS2026 Community.bat""
