#pragma once

#include "CauTruc.h"

// ============================================================================
// PHÂN HỆ: GIAO DỊCH MƯỢN TRẢ (DANH SÁCH LIÊN KẾT ĐƠN)
// NGƯỜI PHỤ TRÁCH: LÊ VĂN HOÀNG HIẾU (N24DECE018)
// ============================================================================

// Thông tin độc giả quá hạn để in danh sách
struct QuaHanInfo {
  int maThe;
  char ho[50];
  char ten[20];
  char maSach[20];
  Date ngayMuon;
  int soNgayQuaHan;
};

// Mã kết quả kiểm tra điều kiện mượn sách
enum KetQuaKiemTraMuon {
  DU_DIEU_KIEN_MUON = 0,
  LOI_THE_BI_KHOA = 1,
  LOI_DA_MUON_TOI_DA = 2,
  LOI_CO_SACH_QUA_HAN = 3,
  LOI_DU_LIEU_MUON = 4
};

// ----------------------------------------------------------------------------
// Nhóm 1: Hàm phụ trợ nội bộ (MuonTra_Core.cpp)
// ----------------------------------------------------------------------------
Date LayNgayHienTai();
int TinhKhoangCachNgay(Date d1, Date d2);
int DemSoSachDangMuon(NodeMuonTra *dsMuonTra);
bool KiemTraCoSachQuaHan(NodeMuonTra *dsMuonTra);
KetQuaKiemTraMuon KiemTraDieuKienMuon(NodeDocGia *pDG);
void TachISBN(const char *maSach, char *isbn);
bool CungISBN(const char *a, const char *b);

// ----------------------------------------------------------------------------
// Nhóm 2: Chức năng (f) — Mượn sách (MuonTra_Muon.cpp)
// ----------------------------------------------------------------------------
bool MuonSach(TREE_DocGia root, DanhSachDauSach &dsDS, int maThe, const char *maSach);

// ----------------------------------------------------------------------------
// Nhóm 3: Chức năng (g) — Trả sách & Báo mất (MuonTra_Tra.cpp)
// ----------------------------------------------------------------------------
bool TraSach(TREE_DocGia root, DanhSachDauSach &dsDS, int maThe, const char *maSach);
bool BaoMatSach(TREE_DocGia root, DanhSachDauSach &dsDS, int maThe, const char *maSach);

// ----------------------------------------------------------------------------
// Nhóm 4: Chức năng (h) — Liệt kê sách đang mượn (MuonTra_LietKe.cpp)
// ----------------------------------------------------------------------------
int LaySachDangMuon(TREE_DocGia root, int maThe, const MuonTra **out, int capacity);

// ----------------------------------------------------------------------------
// Nhóm 5: Chức năng (i) — Thống kê quá hạn (MuonTra_QuaHan.cpp)
// ----------------------------------------------------------------------------
int DemTongSachQuaHan(TREE_DocGia root);
int LapDanhSachQuaHan(TREE_DocGia root, QuaHanInfo *&ketQua);
