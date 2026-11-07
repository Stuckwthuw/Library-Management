#pragma once
#include "CauTruc.h"
#include <stdexcept>

extern const char *FILE_DOCGIA;
extern const char *FILE_DAUSACH;
struct HetDuLieuNhap : std::runtime_error {
    HetDuLieuNhap() : std::runtime_error("Da dong luong nhap") {}
};
void KhoiTaoConsole();
bool LaConsoleTuongTac();
bool LaConsoleXuat();
int DocPhimUI();
void DatViTriConTro(int x, int y);
void HienConTro(bool hien);
void VeKhung(int x, int y, int rong, int cao);
void DungManHinh();
void NhapChuoiAnToan(char *buf, int maxLen, const char *prompt = nullptr);
bool NhapSoNguyenAnToan(int &val, int minVal, int maxVal, const char *prompt);
bool XacNhanHanhDong(const char *prompt);
void LuuSauThayDoi(TREE_DocGia root, const DanhSachDauSach &ds);
void BangDocGia(TREE_DocGia root, bool theoTen);
void ChucNang_ThemDocGia(TREE_DocGia &root, DanhSachDauSach &ds);
void ChucNang_HieuChinhDocGia(TREE_DocGia root, DanhSachDauSach &ds);
void ChucNang_XoaDocGia(TREE_DocGia &root, DanhSachDauSach &ds);
void ChucNang_TraCuuDocGia(TREE_DocGia root);
void ChucNang_ThemDauSach(TREE_DocGia root, DanhSachDauSach &ds);
void ChucNang_ThemBanSao(TREE_DocGia root, DanhSachDauSach &ds);
void ChucNang_TimKiemSach(const DanhSachDauSach &ds);
void ChucNang_InSachTheoTheLoai(const DanhSachDauSach &ds);
void ChucNang_MuonSach(TREE_DocGia root, DanhSachDauSach &ds);
void ChucNang_TraSach(TREE_DocGia root, DanhSachDauSach &ds);
void ChucNang_BaoMatSach(TREE_DocGia root, DanhSachDauSach &ds);
void ChucNang_LietKeSachDangMuon(TREE_DocGia root, const DanhSachDauSach &ds);
void ChucNang_Top10(const DanhSachDauSach &ds, const NodeDocGia *root);

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
