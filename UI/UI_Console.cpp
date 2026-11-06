#include "../UI.h"
#include <iostream>
#ifdef _WIN32
#include <windows.h>
#endif

void XoaManHinhUI() {
#ifdef _WIN32
    HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO info;
    DWORD n;
    if (GetConsoleScreenBufferInfo(h, &info)) {
        DWORD size = static_cast<DWORD>(info.dwSize.X) * info.dwSize.Y;
        COORD start = {0, 0};
        FillConsoleOutputCharacterA(h, ' ', size, start, &n);
        FillConsoleOutputAttribute(h, info.wAttributes, size, start, &n);
        SetConsoleCursorPosition(h, start);
        return;
    }
#endif
    std::cout << '\n';
}
