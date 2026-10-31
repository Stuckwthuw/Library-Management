#pragma once

#include "CauTruc.h"

// ============================================================================
// PHÂN HỆ: GIAO DỊCH MƯỢN TRẢ (DANH SÁCH LIÊN KẾT ĐƠN)
// NGƯỜI PHỤ TRÁCH: LÊ VĂN HOÀNG HIẾU (N24DECE018)
// FILE: MuonTra.h — Khai báo các nguyên mẫu hàm (Function Prototypes)
// ============================================================================

// [STRUCT LƯU THÔNG TIN ĐỘC GIẢ QUÁ HẠN ĐỂ SẮP XẾP VÀ IN BÁO CÁO]
struct QuaHanInfo {
  int maThe;        // Mã thẻ độc giả
  char ho[50];      // Họ và tên đệm
  char ten[20];     // Tên
  char maSach[20];  // Mã sách đang mượn quá hạn
  int soNgayQuaHan; // Số ngày mượn quá hạn = soNgayMuon - MAX_NGAY_MUON
};

// [ENUM: MÃ KẾT QUẢ KIỂM TRA ĐIỀU KIỆN MƯỢN SÁCH]
enum KetQuaKiemTraMuon {
  DU_DIEU_KIEN_MUON = 0,  // Độc giả đủ điều kiện mượn sách
  LOI_THE_BI_KHOA = 1,    // Thẻ độc giả đang bị khóa (trangThai != 1)
  LOI_DA_MUON_TOI_DA = 2, // Đã mượn tối đa MAX_MUON_SACH (3 cuốn)
  LOI_CO_SACH_QUA_HAN = 3 // Đang có sách mượn quá hạn > MAX_NGAY_MUON (7 ngày)
};

// ----------------------------------------------------------------------------
// NHÓM 1: HÀM PHỤ TRỢ NỘI BỘ (MuonTra_Core.cpp)
// ----------------------------------------------------------------------------

// [HÀM LẤY NGÀY HIỆN TẠI — KẾT NỐI MODULE DATE CỦA ĐỨC]
Date LayNgayHienTai();

// [HÀM TÍNH KHOẢNG CÁCH NGÀY (d2 - d1) — KẾT NỐI MODULE DATE CỦA ĐỨC]
int TinhKhoangCachNgay(Date d1, Date d2);

// [HÀM ĐẾM SỐ SÁCH ĐANG MƯỢN CỦA MỘT ĐỘC GIẢ]
int DemSoSachDangMuon(NodeMuonTra *dsMuonTra);

// [HÀM KIỂM TRA CÓ SÁCH QUÁ HẠN TRONG DSLK]
bool KiemTraCoSachQuaHan(NodeMuonTra *dsMuonTra);

// [HÀM KIỂM TRA ĐIỀU KIỆN MƯỢN THEO QUY TẮC]
KetQuaKiemTraMuon KiemTraDieuKienMuon(NodeDocGia *pDG);

// [HÀM TÁCH MÃ ISBN TỪ MÃ SÁCH CON]
// Ví dụ: "CS101_2" → isbn = "CS101"
void TachISBN(const char *maSach, char *isbn);

// ----------------------------------------------------------------------------
// NHÓM 2: CHỨC NĂNG (f) — GIAO DỊCH MƯỢN SÁCH (MuonTra_Muon.cpp)
// ----------------------------------------------------------------------------

// [HÀM MƯỢN SÁCH — GHÉP NỐI DANH MỤC SÁCH CỦA ĐỨC]
// Bẫy 3 điều kiện: thẻ mở, < 3 cuốn, không quá hạn.
// Kiểm tra mã sách tồn tại trong DMS của Đức và đang ở trạng thái 0 (cho mượn).
// Chèn NodeMuonTra vào ĐẦU DSLK dsMuonTra: O(1).
// Cập nhật trạng thái sách trong DMS sang 1 (đã mượn).
bool MuonSach(TREE_DocGia root, DanhSachDauSach &dsDS, int maThe, const char *maSach);

// ----------------------------------------------------------------------------
// NHÓM 3: CHỨC NĂNG (g) — GIAO DỊCH TRẢ SÁCH / BÁO MẤT (MuonTra_Tra.cpp)
// ----------------------------------------------------------------------------

// [HÀM TRẢ SÁCH — GHÉP NỐI DANH MỤC SÁCH CỦA ĐỨC]
// Tìm giao dịch đang mượn → đổi trangThai = 1 (đã trả), ghi ngayTra = ngày hiện tại.
// Cập nhật trạng thái sách trong DMS của Đức về 0 (cho mượn lại).
bool TraSach(TREE_DocGia root, DanhSachDauSach &dsDS, int maThe, const char *maSach);

// [HÀM BÁO MẤT SÁCH — GHÉP NỐI DANH MỤC SÁCH CỦA ĐỨC]
// Tìm giao dịch đang mượn → đổi trangThai = 2 (làm mất), ghi ngayTra = ngày hiện tại.
// Cập nhật trạng thái sách trong DMS của Đức sang 2 (thanh lý).
bool BaoMatSach(TREE_DocGia root, DanhSachDauSach &dsDS, int maThe, const char *maSach);

// ----------------------------------------------------------------------------
// NHÓM 4: CHỨC NĂNG (h) — LIỆT KÊ SÁCH ĐANG MƯỢN (MuonTra_LietKe.cpp)
// ----------------------------------------------------------------------------

// [HÀM LIỆT KÊ SÁCH ĐANG MƯỢN CỦA MỘT ĐỘC GIẢ]
// Duyệt DSLK lọc trangThai == 0, tra cứu tên sách từ dsDS của Đức để in bảng.
void LietKeSachDangMuon(TREE_DocGia root, const DanhSachDauSach &dsDS, int maThe);

// ----------------------------------------------------------------------------
// NHÓM 5: CHỨC NĂNG (i) — THỐNG KÊ QUÁ HẠN (MuonTra_QuaHan.cpp)
// ----------------------------------------------------------------------------

// [HÀM ĐẾM TỔNG SỐ SÁCH QUÁ HẠN TRÊN TOÀN HỆ THỐNG]
int DemTongSachQuaHan(TREE_DocGia root);

// [HÀM CHÍNH: IN DANH SÁCH ĐỘC GIẢ MƯỢN SÁCH QUÁ HẠN 7 NGÀY]
// Duyệt cây BST → Cấp phát mảng → Nạp dữ liệu → QuickSort giảm dần → In bảng.
void InDanhSachDocGiaQuaHan(TREE_DocGia root);
