#include "../UI.h"
#include <iostream>
#ifdef _WIN32
#include <conio.h>
#include <windows.h>
#endif

// Hiển thị menu điều hướng bằng phím mũi tên (Enter để chọn, Esc để quay lại)
int HienThiMenu(const char *tieuDe, const MenuItem *muc, int soMuc) {
    if (!muc || soMuc <= 0) return 0;
    struct ConTro { ConTro() { HienConTro(false); } ~ConTro() { HienConTro(true); } } conTro;
    int vitri = 0;
    bool veLaiManHinh = LaConsoleXuat();
    bool daVeKhungMenu = false;
    while (vitri < soMuc && muc[vitri].ma < 0) ++vitri;
    if (vitri == soMuc) return 0;
    for (;;) {
        bool canVeMenu = veLaiManHinh || !daVeKhungMenu;
        if (veLaiManHinh) XoaManHinhUI();
        if (canVeMenu) std::cout << "\n  THU VIEN  /  " << tieuDe
                  << "\n  -------------------------------------------------------------\n";
        for (int i = 0; canVeMenu && i < soMuc; ++i) {
            if (muc[i].ma < 0) { std::cout << "\n  " << muc[i].nhan << "\n"; continue; }
#ifdef _WIN32
            HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
            if (i == vitri) SetConsoleTextAttribute(h, BACKGROUND_BLUE | BACKGROUND_INTENSITY | 15);
#endif
            std::cout << (i == vitri ? "  >  " : "     ") << muc[i].nhan << '\n';
#ifdef _WIN32
            if (i == vitri) SetConsoleTextAttribute(h, 7);
#endif
        }
        if (canVeMenu) std::cout << "\n  -------------------------------------------------------------\n"
                     "  Mui ten: di chuyen   Enter: chon   Esc: quay lai\n";
        daVeKhungMenu = true;
        std::cout.flush();
        int key = DocPhimUI();
        if (key < 0) return -2;
        if (key >= 256) {
            key -= 256;
            int buoc = key == 72 ? -1 : key == 80 ? 1 : 0;
            if (buoc) {
                do { vitri = (vitri + buoc + soMuc) % soMuc; } while (muc[vitri].ma < 0);
                if (!veLaiManHinh)
                    std::cout << "\n  Dang chon: " << muc[vitri].nhan << std::flush;
            }
        } else if (key == 13) return muc[vitri].ma;
        else if (key == 27 || key == '0') return 0;
        else for (int i = 0; i < soMuc; ++i)
            if (muc[i].ma > 0 && muc[i].nhan[0] == key) return muc[i].ma;
    }
}
