cd /d %~dp0
copy /y C:\Prime\DevelopLib\Dev_RisLib\RisLib\Source\*.* .
del risSystemTime*.*
del risSerial*.*
del *_win.*
copy ..\..\src\*.* .
copy stdafx.h aaaa_stdafx.h
pause
