#pragma once

#include "CauTruc.h"
#include "Date.h"
#include "DauSach.h"
#include "DanhMucSach.h"

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

// [HÀM LẤY NGÀY HIỆN TẠI — KẾT NỐI CHÍNH THỨC MODULE DATE CỦA ĐỨC]
Date LayNgayHienTai();

// [HÀM TÍNH KHOẢNG CÁCH NGÀY (d2 - d1) — KẾT NỐI CHÍNH THỨC MODULE DATE CỦA ĐỨC]
int TinhKhoangCachNgay(Date d1, Date d2);

// [CÁC HÀM TƯƠNG THÍCH NGƯỢC TỪ GIAI ĐOẠN STUB]
Date LayNgayHienTai_Stub();
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

// [HÀM MƯỢN SÁCH — PHIÊN BẢN GHÉP NỐI ĐẦY ĐỦ DMS]
// Bẫy 3 điều kiện: thẻ mở, < 3 cuốn, không quá hạn.
// Kiểm tra mã sách tồn tại trong DMS của Đức và đang ở trạng thái 0 (cho mượn).
// Chèn NodeMuonTra vào ĐẦU DSLK dsMuonTra: O(1).
// Cập nhật trạng thái sách trong DMS sang 1 (đã mượn).
bool MuonSach(TREE_DocGia root, DanhSachDauSach &dsDS, int maThe, const char *maSach);

// [HÀM MƯỢN SÁCH — PHIÊN BẢN ĐỘC LẬP / KIỂM THỬ KHÔNG CẦN DMS]
bool MuonSach(TREE_DocGia root, int maThe, const char *maSach);

// ----------------------------------------------------------------------------
// NHÓM 3: CHỨC NĂNG (g) — TRẢ SÁCH / BÁO MẤT (MuonTra_Tra.cpp)
// ----------------------------------------------------------------------------

// [HÀM TRẢ SÁCH — PHIÊN BẢN GHÉP NỐI ĐẦY ĐỦ DMS]
// Tìm giao dịch đang mượn → đổi trangThai = 1, ghi ngayTra = ngày hiện tại
// Cập nhật trạng thái sách trong DMS của Đức về 0 (cho mượn lại)
bool TraSach(TREE_DocGia root, DanhSachDauSach &dsDS, int maThe, const char *maSach);
bool TraSach(TREE_DocGia root, int maThe, const char *maSach);

// [HÀM BÁO MẤT SÁCH — PHIÊN BẢN GHÉP NỐI ĐẦY ĐỦ DMS]
// Tìm giao dịch đang mượn → đổi trangThai = 2, ghi ngayTra = ngày hiện tại
// Cập nhật trạng thái sách trong DMS của Đức sang 2 (thanh lý)
bool BaoMatSach(TREE_DocGia root, DanhSachDauSach &dsDS, int maThe, const char *maSach);
bool BaoMatSach(TREE_DocGia root, int maThe, const char *maSach);

// ----------------------------------------------------------------------------
// NHÓM 4: CHỨC NĂNG (h) — LIỆT KÊ SÁCH ĐANG MƯỢN (MuonTra_LietKe.cpp)
// ----------------------------------------------------------------------------

// [HÀM LIỆT KÊ SÁCH ĐANG MƯỢN CỦA MỘT ĐỘC GIẢ]
// Duyệt DSLK lọc trangThai == 0, tra cứu tên sách từ dsDS của Đức để in bảng
void LietKeSachDangMuon(TREE_DocGia root, const DanhSachDauSach &dsDS, int maThe);
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
