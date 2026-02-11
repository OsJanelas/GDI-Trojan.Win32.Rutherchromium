start ByteBeats1.exe
start cursorpayload.exe
start payload1.exe
timeout /t 50
taskkill /f /im payload1.exe
start payload2.exe
timeout /t 40
taskkill /f /im payload2.exe
start payload3.exe
timeout /t 60
taskkill /f /im payload3.exe
start payload4.exe
timeout /t 50
taskkill /f /im payload4.exe
start payload5.exe
timeout /t 60
taskkill /f /im payload5.exe
taskkill /f /im ByteBeats1.exe
start payload6.exe
start ByteBeats2.exe
timeout /t 5
taskkill /f /im payload6.exe
start payload7.exe
timeout /t 80
taskkill /f /im payload7.exe
start payload8.exe
timeout /t 50
taskkill /f /im payload8.exe
start payload9.exe
timeout /t 60
taskkill /f /im payload9.exe
start payload10.exe
timeout /t 70
taskkill /f /im payload10.exe
start payload11.exe
timeout /t 30
start payload12.exe
timeout /t 100