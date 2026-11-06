#include "../UI.h"
#include <iostream>
#ifdef _WIN32
#include <conio.h>
#include <windows.h>
#endif

// Bỏ qua tiêu đề nhóm và quay vòng, bảo đảm chỉ chọn được mục nghiệp vụ.
int HienThiMenu(const char *tieuDe, const MenuItem *muc, int soMuc) {
    if (!muc || soMuc <= 0) return 0;
    int vitri = 0;
    while (vitri < soMuc && muc[vitri].ma < 0) ++vitri;
    if (vitri == soMuc) return 0;
    for (;;) {
        XoaManHinhUI();
        std::cout << "\n  THU VIEN  /  " << tieuDe
                  << "\n  -------------------------------------------------------------\n";
        for (int i = 0; i < soMuc; ++i) {
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
        std::cout << "\n  -------------------------------------------------------------\n"
                     "  Mui ten: di chuyen   Enter: chon   Esc: quay lai\n";
#ifdef _WIN32
        int key = _getch();
        if (key == 0 || key == 224) {
            key = _getch();
            int buoc = key == 72 ? -1 : key == 80 ? 1 : 0;
            if (buoc) do { vitri = (vitri + buoc + soMuc) % soMuc; } while (muc[vitri].ma < 0);
        } else if (key == 13) return muc[vitri].ma;
        else if (key == 27 || key == '0') return 0;
        else for (int i = 0; i < soMuc; ++i)
            if (muc[i].ma > 0 && muc[i].nhan[0] == key) return muc[i].ma;
#else
        int choice;
        if (!(std::cin >> choice)) return 0;
        std::cin.ignore(10000, '\n');
        for (int i = 0; i < soMuc; ++i) if (muc[i].ma == choice) return choice;
#endif
    }
}
