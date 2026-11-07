#include "../UI.h"
#include "../Common/Chuoi.h"
#include <iostream>
#include <string>

namespace {
// Căn lề từng ô theo độ rộng ký tự
void inO(const char *text, int width) {
    std::wstring w = Chuoi::rong(text);
    if (static_cast<int>(w.size()) > width) {
        w.resize(width > 0 ? width - 1 : 0);
        if (!w.empty() && w.back() >= 0xD800 && w.back() <= 0xDBFF) w.pop_back();
        if (width > 0) w += L'~';
    }
    std::cout << ' ' << Chuoi::utf8(w) << std::string(width - w.size(), ' ') << " |";
}
void vien(const CauHinhCot *cot, int n) {
    std::cout << '+';
    for (int j = 0; j < n; ++j) std::cout << std::string(cot[j].doRong + 2, '-') << '+';
    std::cout << '\n';
}
}

// Hiển thị bảng dữ liệu có hỗ trợ phân trang
void HienThiBangPhanTrang(const char *tieuDe, const CauHinhCot *cot, int soCot,
                          HamLayDuLieu lay, int soDong, int moiTrang) {
    if (!cot || !lay || soCot <= 0 || moiTrang <= 0 || soDong <= 0) {
        std::cout << "[THONG BAO] Khong co du lieu de hien thi.\n";
        return;
    }
    for (int j = 0; j < soCot; ++j)
        if (cot[j].doRong < 1 || cot[j].doRong > 200) return;
    int trang = 0, tong = 1 + (soDong - 1) / moiTrang;
    for (;;) {
        XoaManHinhUI();
        std::cout << "\n  THU VIEN / " << tieuDe << "\n\n";
        vien(cot, soCot);
        std::cout << '|';
        for (int j = 0; j < soCot; ++j) inO(cot[j].tieuDe, cot[j].doRong);
        std::cout << '\n';
        vien(cot, soCot);
        int cuoi = trang * moiTrang + (soDong - trang * moiTrang < moiTrang ? soDong - trang * moiTrang : moiTrang);
        for (int i = trang * moiTrang; i < cuoi; ++i) {
            std::cout << '|';
            for (int j = 0; j < soCot; ++j) {
                char buf[256] = {};
                lay(i, j, buf, sizeof(buf));
                buf[sizeof(buf)-1] = '\0';
                inO(buf, cot[j].doRong);
            }
            std::cout << '\n';
        }
        vien(cot, soCot);
        std::cout << "  Trang " << trang + 1 << " / " << tong << " | Tong: " << soDong << '\n';
        if (!LaConsoleTuongTac()) {
            if (++trang == tong) return;
            continue;
        }
        std::cout << "  Trai/Phai (P/N): chuyen trang | Home/End | Esc: quay lai\n";
        int key = DocPhimUI();
        if (key < 0 || key == 27 || key == '0') return;
        if ((key == 256+75 || key == 256+73 || key == 'p' || key == 'P') && trang > 0) --trang;
        if ((key == 256+77 || key == 256+81 || key == 'n' || key == 'N') && trang + 1 < tong) ++trang;
        if (key == 256+71) trang = 0;
        if (key == 256+79) trang = tong - 1;
    }
}
