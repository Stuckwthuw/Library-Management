#pragma once
#include "CauTruc.h"
#include <string>

// Các hàm lõi không đọc bàn phím, không phụ thuộc UI hoặc module của Hiếu.
void khoiTaoDSDS(DanhSachDauSach &ds);
bool dauSachRong(const DanhSachDauSach &ds);
bool dauSachDay(const DanhSachDauSach &ds);
void giaiPhongDauSach(DanhSachDauSach &ds);

// Thành công: ds sở hữu moi và danh mục của nó. Thất bại: bên gọi vẫn sở hữu.
bool themDauSach(DanhSachDauSach &ds, DauSach *moi);
DauSach *timDauSachTheoISBN(const DanhSachDauSach &ds, const char *ISBN);
bool timSachTheoMaSach(const DanhSachDauSach &ds, const char *maSach,
                      DauSach *&dauSach, NodeDMS *&cuonSach);
std::string trichXuatISBN(const char *maSach);

// Gọi sau khi thay ISBN/con trỏ bằng thao tác ngoài các API của module.
void voHieuChiMucISBN();
int soSanhTen(const char *a, const char *b);
bool tenChuaTuKhoa(const char *ten, const std::string &tuKhoa);

// Bên gọi cấp mảng đủ ds.n con trỏ; trả -1 nếu bộ đệm không đủ/hợp lệ.
// Các con trỏ kết quả mượn từ ds, không được delete riêng.
int timDauSachTheoTen(const DanhSachDauSach &ds, const char *tuKhoa,
                     DauSach **ketQua, int sucChua);
int layDauSachTheoTheLoai(const DanhSachDauSach &ds,
                         DauSach **ketQua, int sucChua);

struct ThongKeSach {
    const DauSach *sach;
    int soLuotMuon;
};

// Kết quả giảm dần theo lượt mượn; không thêm trường vào DauSach.
// root chỉ được đọc. Lịch sử cần còn được lưu để đếm đầy đủ các lượt.
bool thongKeLuotMuon(const DanhSachDauSach &ds, const NodeDocGia *root,
                    ThongKeSach *ketQua, int sucChua);
int layTop10(const DanhSachDauSach &ds, const NodeDocGia *root,
             ThongKeSach *ketQua, int sucChua);

// File UTF-8 một dòng phân cách bằng |; đọc lỗi thì giữ danh sách đang có.
bool ghiDauSach(const DanhSachDauSach &ds, const char *path);
bool docDauSach(DanhSachDauSach &ds, const char *path);
