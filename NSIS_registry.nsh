; Asociación de archivos .lat (Windows) usando ProgID Latino.File
WriteRegStr HKCR ".lat" "" "Latino.File"
WriteRegStr HKCR "Latino.File" "" "Archivo LAT de Latino"
WriteRegStr HKCR "Latino.File\DefaultIcon" "" "$INSTDIR\resources\icons\latino_archivo_Win.ico, 1"
WriteRegStr HKCR "Latino.File\shell\open\command" "" '"$INSTDIR\bin\latino.exe" "%1"'
