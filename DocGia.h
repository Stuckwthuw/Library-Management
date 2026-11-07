#pragma once

#include "CauTruc.h"
#include <cstddef>

// ============================================================================
// PHÂN HỆ: QUẢN LÝ THẺ ĐỘC GIẢ (CÂY NHỊ PHÂN TÌM KIẾM - BST)
// NGƯỜI PHỤ TRÁCH: LÊ VĂN HOÀNG HIẾU (N24DECE018)
// ============================================================================

// ----------------------------------------------------------------------------
// Nhóm 0: Tiện ích chuỗi & xử lý dữ liệu nhập
// ----------------------------------------------------------------------------
bool KiemTraChuoiRong(const char *s);
void ChuanHoaKhoangTrang(char *s);
bool DinhDangHoTen(char *s, int sucChua);
template<std::size_t N> bool DinhDangHoTen(char (&s)[N]) {
    return DinhDangHoTen(s, static_cast<int>(N));
}
void ChuanHoaMa(char *s);
int KiemTraLoiHoTen(const char *s, char &kyTuViPham);
bool KiemTraChuCaiVaKhoangTrang(const char *s);
bool KiemTraDinhDangMaSach(const char *maSach);
bool ChuaChuoiConKhongPhanBietHoaThuong(const char *str, const char *subStr);

// ----------------------------------------------------------------------------
// Nhóm 1: Các thao tác cơ bản & giải phóng bộ nhớ cây BST
// ----------------------------------------------------------------------------
void KhoiTaoCay(TREE_DocGia &root);
bool KiemTraCayRong(TREE_DocGia root);
int DemSoLuongDocGia(TREE_DocGia root);
void GiaiPhongDSMuonTra(NodeMuonTra *&dsMuonTra);
void GiaiPhongCayDocGia(TREE_DocGia &root);

// ----------------------------------------------------------------------------
// Nhóm 2: Tìm kiếm & sinh mã thẻ
// ----------------------------------------------------------------------------
NodeDocGia *TimKiemDocGia(TREE_DocGia root, int maThe);
bool KiemTraMaTheTonTai(TREE_DocGia root, int maThe);
int TimKiemDocGiaTheoTen(TREE_DocGia root, const char *tuKhoa, TheDocGia *arr[], int maxKetQua);
int SinhMaTheTuDong(TREE_DocGia root);

// ----------------------------------------------------------------------------
// Nhóm 3: Chức năng (a) - Thêm, hiệu chỉnh, xóa thẻ độc giả
// ----------------------------------------------------------------------------
bool KiemTraDocGiaDangMuonSach(NodeDocGia *pDG);
bool ThemDocGia(TREE_DocGia &root, TheDocGia dg);
bool HieuChinhDocGia(TREE_DocGia root, int maThe, const char *ho,
                     const char *ten, const char *phai, int trangThai);
void TimNodeTheMang(NodeDocGia *&X, NodeDocGia *&Y);
bool XoaDocGia(TREE_DocGia &root, int maThe);

// ----------------------------------------------------------------------------
// Nhóm 4: Chức năng (b) - In danh sách độc giả
// ----------------------------------------------------------------------------
void InDanhSachDocGia_TheoMaThe(TREE_DocGia root);
void DoCayVaoMang(TREE_DocGia root, TheDocGia *arr[], int &index);
int SoSanhTenHo(const TheDocGia *a, const TheDocGia *b);
void QuickSortDocGia(TheDocGia *arr[], int left, int right);
void InDanhSachDocGia_TheoTenHo(TREE_DocGia root);

// ----------------------------------------------------------------------------
// Nhóm 5: Đọc và ghi file
// ----------------------------------------------------------------------------
bool GhiFileDocGia(TREE_DocGia root, const char *filePath = "Data/DocGia.txt");
bool DocFileDocGia(TREE_DocGia &root, const char *filePath = "Data/DocGia.txt");

// Quản lý lịch sử mượn trả của thẻ đã xóa
const NodeMuonTra *LayLichSuLuuTru();
void LuuTruLichSu(NodeMuonTra *&head);
void GiaiPhongLichSuLuuTru();
void ThayLichSuLuuTru(NodeMuonTra *&head);
NodeMuonTra *TachLichSuLuuTru();
