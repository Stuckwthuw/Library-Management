#pragma once

struct MenuItem {
    const char *nhan;
    int ma;
};

// Menu không sở hữu chuỗi; mã âm biểu diễn tiêu đề nhóm không chọn được.
int HienThiMenu(const char *tieuDe, const MenuItem *muc, int soMuc);
void XoaManHinhUI();

struct CauHinhCot { const char *tieuDe; int doRong; };
typedef void (*HamLayDuLieu)(int dong, int cot, char *buf, int maxLen);
void HienThiBangPhanTrang(const char *tieuDe, const CauHinhCot *cot, int soCot,
                          HamLayDuLieu lay, int soDong, int moiTrang = 10);
