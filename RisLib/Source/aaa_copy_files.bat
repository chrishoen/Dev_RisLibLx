cd /d %~dp0
copy /y C:\Prime\DevelopLib\Dev_RisLib\RisLib\Source\*.* .
copy stdafx.h aaaa_stdafx.h
del risSystemTime*.*
del *_win.*
copy ..\Source_linux\*.cpp .
pause
