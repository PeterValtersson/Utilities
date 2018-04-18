@if (@X)==(@Y) @end /* JScript comment
    @echo off

    set "extension=dot"

    setlocal enableDelayedExpansion
    for /R %%a in (*%extension%) do (
        for /f %%# in ('cscript //E:JScript //nologo "%~f0" %%a') do set "cdate=%%#"
       echo "%%~a"
	   echo "%%~dpa%%~na_!cdate!.pdf"
	   dot -Tpdf "%%~a" -o "%%~dpa%%~na_!cdate!.pdf"
	   del "%%~a"
    )

    rem cscript //E:JScript //nologo "%~f0" %*
    exit /b %errorlevel%
@if (@X)==(@Y) @end JScript comment */


FSOObj = new ActiveXObject("Scripting.FileSystemObject");
var ARGS = WScript.Arguments;
var file=ARGS.Item(0);

var d1=FSOObj.GetFile(file).DateCreated;

d2=new Date(d1);
var year=d2.getFullYear();
var mon=d2.getMonth();
var day=d2.getDate();
var h=d2.getHours();
var m=d2.getMinutes();
var s=d2.getSeconds();
var ms=d2.getMilliseconds();

if (mon<10){mon="0"+mon;}
if (day<10){day="0"+day;}
if (h<10){h="0"+h;}
if (m<10){m="0"+m;}
if (s<10){s="0"+s;}
if (ms<10){ms="00"+ms;}else if(ms<100){ms="0"+ms;}

WScript.Echo(""+year+mon+day+h+m+s+ms);