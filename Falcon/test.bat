@ECHO OFF
"C:\Program Files\Git\usr\bin\echo.exe" -e "\x1b[31mThis text has a red foreground using SGR.31.\r\n"
"C:\Program Files\Git\usr\bin\echo.exe" -e "\x1b[1mThis text has a bright (bold) red foreground using SGR.1 to affect the previous color setting.\r\n"
"C:\Program Files\Git\usr\bin\echo.exe" -e "\x1b[mThis text has returned to default colors using SGR.0 implicitly.\r\n"
::"C:\Program Files\Git\usr\bin\echo.exe" -e "\x1b[34;46mThis text shows the foreground and background change at the same time.\r\n"
::"C:\Program Files\Git\usr\bin\echo.exe" -e "\x1b[0mThis text has returned to default colors using SGR.0 explicitly.\r\n"
::"C:\Program Files\Git\usr\bin\echo.exe" -e "\x1b[31;32;33;34;35;36;101;102;103;104;105;106;107mThis text attempts to apply many colors in the same command. Note the colors are applied from left to right so only the right-most option of foreground cyan (SGR.36) and background bright white (SGR.107) is effective.\r\n"
::"C:\Program Files\Git\usr\bin\echo.exe" -e "\x1b[39mThis text has restored the foreground color only.\r\n"
::"C:\Program Files\Git\usr\bin\echo.exe" -e "\x1b[49mThis text has restored the background color only.\r\n"