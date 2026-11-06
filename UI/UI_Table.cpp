#include "../UI.h"
#include <cstdio>
#include <iostream>
#ifdef _WIN32
#include <conio.h>
#endif

// Callback giữ bảng độc lập với kiểu dữ liệu; mỗi lần chỉ vẽ các dòng của trang.
void HienThiBangPhanTrang(const char *tieuDe, const CauHinhCot *cot, int soCot,
                          HamLayDuLieu lay, int soDong, int moiTrang) {
    if (!cot || !lay || soCot <= 0 || moiTrang <= 0 || soDong <= 0) {
        std::cout << "[THONG BAO] Khong co du lieu de hien thi.\n";
        return;
    }
    int trang = 0, tong = 1 + (soDong - 1) / moiTrang;
    for (;;) {
        XoaManHinhUI();
        std::cout << "\n  THU VIEN / " << tieuDe << "\n\n";
        for (int j = 0; j < soCot; ++j) std::printf("%-*.*s ", cot[j].doRong, cot[j].doRong, cot[j].tieuDe);
        std::cout << '\n';
        int cuoi = (trang + 1) * moiTrang;
        if (cuoi > soDong) cuoi = soDong;
        for (int i = trang * moiTrang; i < cuoi; ++i) {
            for (int j = 0; j < soCot; ++j) {
                char buf[256] = {};
                lay(i, j, buf, sizeof(buf));
                std::printf("%-*.*s ", cot[j].doRong, cot[j].doRong, buf);
            }
            std::cout << '\n';
        }
        std::cout << "\n  Trang " << trang + 1 << " / " << tong << "  |  Tong: " << soDong
                  << "\n  Trai/Phai: chuyen trang  Home/End: dau/cuoi  Esc: quay lai\n";
#ifdef _WIN32
        int key = _getch();
        if (key == 27) return;
        if (key == 0 || key == 224) {
            key = _getch();
            if ((key == 75 || key == 73) && trang > 0) --trang;
            if ((key == 77 || key == 81) && trang + 1 < tong) ++trang;
            if (key == 71) trang = 0;
            if (key == 79) trang = tong - 1;
        }
#else
        return;
#endif
    }
}
