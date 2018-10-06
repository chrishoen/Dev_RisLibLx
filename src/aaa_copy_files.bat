cd C:\Prime\DevelopLinux\Dev_RisLibLx\RisLib
copy /y C:\Prime\DevelopLib\Dev_RisLib\RisLib\Source\*.* .
del risSystemTime*.*
del *_win.*
copy ..\src\*.h .
copy ..\src\*.cpp .
pause
