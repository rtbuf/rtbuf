OutFile "rtbuf-0.2.3_setup.exe"
InstallDir $DESKTOP\rtbuf-0.2.3

Section

WriteUninstaller $INSTDIR\uninstaller.exe

SetOutPath $INSTDIR
File C:\msys64\usr\bin\msys-2.0.dll
File .libs\msys-rtbuf-0.dll

File C:\msys64\mingw64\bin\libreadline8.dll
File C:\msys64\mingw64\bin\libtermcap-0.dll
File .libs\rtbuf.exe

File C:\msys64\mingw64\bin\libgdk-3-0.dll 
File C:\msys64\mingw64\bin\libcairo-2.dll 
File C:\msys64\mingw64\bin\libgio-2.0-0.dll 
File C:\msys64\mingw64\bin\libglib-2.0-0.dll 
File C:\msys64\mingw64\bin\libgcc_s_seh-1.dll 
File C:\msys64\mingw64\bin\libgobject-2.0-0.dll 
File C:\msys64\mingw64\bin\libgtk-3-0.dll 
File C:\msys64\mingw64\bin\libintl-8.dll 
File C:\msys64\mingw64\bin\libgcc_s_seh-1.dll 
File C:\msys64\mingw64\bin\libpixman-1-0.dll 
File C:\msys64\mingw64\bin\libfontconfig-1.dll 
File C:\msys64\mingw64\bin\libfreetype-6.dll 
File C:\msys64\mingw64\bin\libpng16-16.dll 
File C:\msys64\mingw64\bin\zlib1.dll 
File C:\msys64\mingw64\bin\libwinpthread-1.dll 
File C:\msys64\mingw64\bin\libcairo-gobject-2.dll 
File C:\msys64\mingw64\bin\libpcre-1.dll 
File C:\msys64\mingw64\bin\libepoxy-0.dll 
File C:\msys64\mingw64\bin\libfribidi-0.dll 
File C:\msys64\mingw64\bin\libgdk_pixbuf-2.0-0.dll 
File C:\msys64\usr\bin\libffi-6.dll 
File C:\msys64\mingw64\bin\libpango-1.0-0.dll 
File C:\msys64\mingw64\bin\libpangocairo-1.0-0.dll 
File C:\msys64\mingw64\bin\libpangowin32-1.0-0.dll 
File C:\msys64\mingw64\bin\libgmodule-2.0-0.dll 
File C:\msys64\mingw64\bin\libiconv-2.dll 
File C:\msys64\mingw64\bin\libatk-1.0-0.dll 
File C:\msys64\mingw64\bin\libharfbuzz-0.dll 
File C:\msys64\mingw64\bin\libpangoft2-1.0-0.dll 
File C:\msys64\mingw64\bin\libexpat-1.dll 
File C:\msys64\mingw64\bin\libbz2-1.dll 
File C:\msys64\mingw64\bin\libgcc_s_seh-1.dll 
File C:\msys64\mingw64\bin\libthai-0.dll 
File C:\msys64\mingw64\bin\libgraphite2.dll 
File C:\msys64\mingw64\bin\libdatrie-1.dll 
File C:\msys64\mingw64\bin\libstdc++-6.dll 
File C:\msys64\mingw64\bin\libgcc_s_seh-1.dll 

File .libs\rtbuf-gtk.exe

SectionEnd
 

Section "Uninstall"
 
Delete $INSTDIR\rtbuf.exe
Delete $INSTDIR\rtbuf-gtk.exe

Delete $INSTDIR\msys-2.0.dll
Delete $INSTDIR\msys-rtbuf-0.dll

Delete $INSTDIR\libreadline8.dll
Delete $INSTDIR\libtermcap-0.dll
Delete $INSTDIR\rtbuf.exe

Delete $INSTDIR\libgdk-3-0.dll 
Delete $INSTDIR\libcairo-2.dll 
Delete $INSTDIR\libgio-2.0-0.dll 
Delete $INSTDIR\libglib-2.0-0.dll 
Delete $INSTDIR\libgcc_s_seh-1.dll 
Delete $INSTDIR\libgobject-2.0-0.dll 
Delete $INSTDIR\libgtk-3-0.dll 
Delete $INSTDIR\libintl-8.dll 
Delete $INSTDIR\libgcc_s_seh-1.dll 
Delete $INSTDIR\libpixman-1-0.dll 
Delete $INSTDIR\libfontconfig-1.dll 
Delete $INSTDIR\libfreetype-6.dll 
Delete $INSTDIR\libpng16-16.dll 
Delete $INSTDIR\zlib1.dll 
Delete $INSTDIR\libwinpthread-1.dll 
Delete $INSTDIR\libcairo-gobject-2.dll 
Delete $INSTDIR\libpcre-1.dll 
Delete $INSTDIR\libepoxy-0.dll 
Delete $INSTDIR\libfribidi-0.dll 
Delete $INSTDIR\libgdk_pixbuf-2.0-0.dll 
Delete $INSTDIR\libffi-6.dll 
Delete $INSTDIR\libpango-1.0-0.dll 
Delete $INSTDIR\libpangocairo-1.0-0.dll 
Delete $INSTDIR\libpangowin32-1.0-0.dll 
Delete $INSTDIR\libgmodule-2.0-0.dll 
Delete $INSTDIR\libiconv-2.dll 
Delete $INSTDIR\libatk-1.0-0.dll 
Delete $INSTDIR\libharfbuzz-0.dll 
Delete $INSTDIR\libpangoft2-1.0-0.dll 
Delete $INSTDIR\libexpat-1.dll 
Delete $INSTDIR\libbz2-1.dll 
Delete $INSTDIR\libgcc_s_seh-1.dll 
Delete $INSTDIR\libthai-0.dll 
Delete $INSTDIR\libgraphite2.dll 
Delete $INSTDIR\libdatrie-1.dll 
Delete $INSTDIR\libstdc++-6.dll 
Delete $INSTDIR\libgcc_s_seh-1.dll 

Delete $INSTDIR\rtbuf-gtk.exe

Delete $INSTDIR\uninstaller.exe
 
SectionEnd
