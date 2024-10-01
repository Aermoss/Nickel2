@echo off
"%~dp0\mono.exe" %MONO_OPTIONS% "%~dp0\..\lib\mono\xbuild\14.0\bin\xbuild.exe" %*
