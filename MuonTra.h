#pragma once

#include "CauTruc.h"

// ============================================================================
// PHÂN HỆ: GIAO DỊCH MƯỢN TRẢ (DANH SÁCH LIÊN KẾT ĐƠN)
// NGƯỜI PHỤ TRÁCH: LÊ VĂN HOÀNG HIẾU (N24DECE018)
// FILE: MuonTra.h — Khai báo các nguyên mẫu hàm (Function Prototypes)
// ============================================================================

struct QuaHanInfo {
  int maThe;        // Mã thẻ độc giả
  char ho[50];      // Họ
  char ten[20];     // Tên
  char maSach[20];  // Mã sách đang quá hạn
  int soNgayQuaHan; // Số ngày quá hạn = soNgayMuon - MAX_NGAY_MUON
};

// ENUM: MÃ KẾT QUẢ KIỂM TRA ĐIỀU KIỆN MƯỢN SÁCH
enum KetQuaKiemTraMuon {
  DU_DIEU_KIEN_MUON = 0,  // Độc giả đủ điều kiện mượn sách
  LOI_THE_BI_KHOA = 1,    // Thẻ độc giả đang bị khóa (trangThai != 1)
  LOI_DA_MUON_TOI_DA = 2, // Đã mượn tối đa MAX_MUON_SACH (3 cuốn)
  LOI_CO_SACH_QUA_HAN = 3 // Đang có sách mượn quá hạn > MAX_NGAY_MUON (7 ngày)
};

// ----------------------------------------------------------------------------
// NHÓM 1: HÀM PHỤ TRỢ NỘI BỘ (MuonTra_Core.cpp)
// ----------------------------------------------------------------------------

// [STUB TẠM THỜI — Sẽ thay bằng module Date của Đức khi ghép nối]
Date LayNgayHienTai_Stub();

// Tính khoảng cách ngày giữa 2 mốc Date (d2 - d1), đơn vị: ngày
int TinhKhoangCachNgay_Stub(Date d1, Date d2);

// [HÀM ĐẾM SỐ SÁCH ĐANG MƯỢN CỦA MỘT ĐỘC GIẢ]
int DemSoSachDangMuon(NodeMuonTra *dsMuonTra);

// [HÀM KIỂM TRA CÓ SÁCH QUÁ HẠN TRONG DSLK]
bool KiemTraCoSachQuaHan(NodeMuonTra *dsMuonTra);

// [HÀM KIỂM TRA MƯỢN THEO QUY TẮC]
KetQuaKiemTraMuon KiemTraDieuKienMuon(NodeDocGia *pDG);

// [HÀM TÁCH MÃ ISBN TỪ MÃ SÁCH]
// Ví dụ: "CS101_2" → isbn = "CS101"
void TachISBN(const char *maSach, char *isbn);

// ----------------------------------------------------------------------------
// NHÓM 2: CHỨC NĂNG (f) — MƯỢN SÁCH (MuonTra_Muon.cpp)
// ----------------------------------------------------------------------------

// [HÀM MƯỢN SÁCH]
// Bẫy 3 điều kiện: thẻ mở, < 3 cuốn, không quá hạn.
// Chèn NodeMuonTra vào ĐẦU DSLK dsMuonTra: O(1).
bool MuonSach(TREE_DocGia root, int maThe, const char *maSach);

// ----------------------------------------------------------------------------
// NHÓM 3: CHỨC NĂNG (g) — TRẢ SÁCH / BÁO MẤT (MuonTra_Tra.cpp)
// ----------------------------------------------------------------------------

// [HÀM TRẢ SÁCH]
// Tìm giao dịch đang mượn → đổi trangThai = 1, ghi ngayTra
bool TraSach(TREE_DocGia root, int maThe, const char *maSach);

// [HÀM BÁO MẤT SÁCH]
// Tìm giao dịch đang mượn → đổi trangThai = 2, sách DMS → thanh lý
bool BaoMatSach(TREE_DocGia root, int maThe, const char *maSach);

// ----------------------------------------------------------------------------
// NHÓM 4: CHỨC NĂNG (h) — LIỆT KÊ SÁCH ĐANG MƯỢN (MuonTra_LietKe.cpp)
// ----------------------------------------------------------------------------

// [HÀM LIỆT KÊ SÁCH ĐANG MƯỢN CỦA MỘT ĐỘC GIẢ]
// Duyệt DSLK lọc trangThai == 0, tra cứu tên sách từ module Đức
void LietKeSachDangMuon(TREE_DocGia root, int maThe);

// ----------------------------------------------------------------------------
// NHÓM 5: CHỨC NĂNG (i) — DANH SÁCH QUÁ HẠN (MuonTra_QuaHan.cpp)
// ----------------------------------------------------------------------------

// [HÀM ĐẾM TỔNG SỐ SÁCH QUÁ HẠN TRÊN TOÀN HỆ THỐNG]
void DemTongSachQuaHan_DuyetCay(TREE_DocGia root, int &count);
int DemTongSachQuaHan(TREE_DocGia root);

// [HÀM NẠP DANH SÁCH QUÁ HẠN VÀO MẢNG]
// Duyệt cây BST lần 2, nạp thông tin quá hạn vào mảng QuaHanInfo[]
void NapDanhSachQuaHan(TREE_DocGia root, QuaHanInfo arr[], int &index);

// [THUẬT TOÁN QUICKSORT CHO MẢNG QUÁ HẠN]
// Sắp xếp giảm dần theo soNgayQuaHan
void QuickSortQuaHan(QuaHanInfo arr[], int left, int right);

// [HÀM CHÍNH: IN DANH SÁCH ĐỘC GIẢ MƯỢN SÁCH QUÁ HẠN]
// Tổng hợp: Đếm → Cấp phát → Nạp → QuickSort → In → Giải phóng
void InDanhSachDocGiaQuaHan(TREE_DocGia root);
