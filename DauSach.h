#pragma once
#include "CauTruc.h"
#include <string>

// ============================================================================
// PHÂN HỆ: QUẢN LÝ ĐẦU SÁCH (MẢNG CON TRỎ)
// NGƯỜI PHỤ TRÁCH: LÊ ANH ĐỨC (N24DECE012)
// ============================================================================

// Khởi tạo và giải phóng mảng con trỏ
void khoiTaoDSDS(DanhSachDauSach &ds);
bool dauSachRong(const DanhSachDauSach &ds);
bool dauSachDay(const DanhSachDauSach &ds);
void giaiPhongDauSach(DanhSachDauSach &ds);

// Chức năng thêm và tìm kiếm đầu sách
bool themDauSach(DanhSachDauSach &ds, DauSach *moi);
DauSach *timDauSachTheoISBN(const DanhSachDauSach &ds, const char *ISBN);
bool timSachTheoMaSach(const DanhSachDauSach &ds, const char *maSach,
                      DauSach *&dauSach, NodeDMS *&cuonSach);
std::string trichXuatISBN(const char *maSach);

void voHieuChiMucISBN();
int soSanhTen(const char *a, const char *b);
bool tenChuaTuKhoa(const char *ten, const std::string &tuKhoa);

int timDauSachTheoTen(const DanhSachDauSach &ds, const char *tuKhoa,
                     DauSach **ketQua, int sucChua);
int layDauSachTheoTheLoai(const DanhSachDauSach &ds,
                         DauSach **ketQua, int sucChua);

// Cấu trúc thống kê số lượt mượn phục vụ Top 10
struct ThongKeSach {
    const DauSach *sach;
    int soLuotMuon;
};

bool thongKeLuotMuon(const DanhSachDauSach &ds, const NodeDocGia *root,
                    ThongKeSach *ketQua, int sucChua, const NodeMuonTra *luuTru = nullptr);
int layTop10(const DanhSachDauSach &ds, const NodeDocGia *root,
             ThongKeSach *ketQua, int sucChua, const NodeMuonTra *luuTru = nullptr);

// Đọc và ghi file đầu sách
bool ghiDauSach(const DanhSachDauSach &ds, const char *path);
bool docDauSach(DanhSachDauSach &ds, const char *path);
