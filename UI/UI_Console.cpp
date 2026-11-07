#include "../UI.h"
#include <iostream>
#include <conio.h>
#include <string>
#ifdef _WIN32
#include <windows.h>
#endif

// Khởi tạo bảng mã UTF-8 cho console
void KhoiTaoConsole() {
    SetConsoleCP(CP_UTF8);
    SetConsoleOutputCP(CP_UTF8);
}
bool LaConsoleTuongTac() {
    DWORD mode;
    return GetConsoleMode(GetStdHandle(STD_INPUT_HANDLE), &mode) != 0;
}
bool LaConsoleXuat() {
    DWORD mode;
    return GetConsoleMode(GetStdHandle(STD_OUTPUT_HANDLE), &mode) != 0;
}

// Đọc phím bấm từ bàn phím (hỗ trợ phím mũi tên)
int DocPhimUI() {
    if (LaConsoleTuongTac()) {
        int c = _getch();
        return c == 0 || c == 224 ? 256 + _getch() : c;
    }
    std::string line;
    if (!std::getline(std::cin, line)) return -1;
    return line.size() == 1 ? static_cast<unsigned char>(line[0]) : 0;
}
void DatViTriConTro(int x, int y) {
    if (x < 0 || y < 0 || x > 32767 || y > 32767) return;
    COORD p = {static_cast<SHORT>(x), static_cast<SHORT>(y)};
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), p);
}
void HienConTro(bool hien) {
    CONSOLE_CURSOR_INFO info;
    HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
    if (GetConsoleCursorInfo(h, &info)) {
        info.bVisible = hien;
        SetConsoleCursorInfo(h, &info);
    }
}
void VeKhung(int x, int y, int rong, int cao) {
    if (rong < 2 || cao < 2) return;
    for (int row = 0; row < cao; ++row) {
        DatViTriConTro(x, y + row);
        for (int col = 0; col < rong; ++col) {
            bool bienDoc = col == 0 || col == rong - 1;
            bool bienNgang = row == 0 || row == cao - 1;
            std::cout << (bienNgang ? (bienDoc ? '+' : '-') : (bienDoc ? '|' : ' '));
        }
    }
}

// Xóa màn hình console
void XoaManHinhUI() {
    std::cout.flush();
#ifdef _WIN32
    HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD mode = 0;
    if (GetConsoleMode(h, &mode) &&
        SetConsoleMode(h, mode | ENABLE_VIRTUAL_TERMINAL_PROCESSING)) {
        std::cout << "\x1b[2J\x1b[H" << std::flush;
        return;
    }
    CONSOLE_SCREEN_BUFFER_INFO info;
    if (GetConsoleScreenBufferInfo(h, &info)) {
        for (SHORT y = info.srWindow.Top; y <= info.srWindow.Bottom; ++y) {
            DWORD n = 0;
            COORD start = {info.srWindow.Left, y};
            DWORD width = static_cast<DWORD>(info.srWindow.Right - info.srWindow.Left + 1);
            FillConsoleOutputCharacterA(h, ' ', width, start, &n);
            FillConsoleOutputAttribute(h, info.wAttributes, width, start, &n);
        }
        COORD start = {info.srWindow.Left, info.srWindow.Top};
        SetConsoleCursorPosition(h, start);
        return;
    }
#endif
}
