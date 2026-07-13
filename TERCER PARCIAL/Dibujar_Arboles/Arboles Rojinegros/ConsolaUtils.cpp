#include "ConsolaUtils.h"

HWND GetConsoleHwnd()
{
    HWND hwndFound;
    char windowTitle[1024];
    
    GetConsoleTitleA(windowTitle, sizeof(windowTitle));
    hwndFound = FindWindowA(NULL, windowTitle);
    
    if (hwndFound == NULL)
    {
        hwndFound = GetConsoleWindow();
    }
    
    return hwndFound;
}