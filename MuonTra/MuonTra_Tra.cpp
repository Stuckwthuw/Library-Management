#include "../DocGia.h"
#include "../MuonTra.h"
#include <cstring>
#include <iostream>

using namespace std;

// ============================================================================
// PHÂN HỆ: GIAO DỊCH MƯỢN TRẢ
// FILE: MuonTra_Tra.cpp — Chức năng (g): Trả Sách / Báo Mất Sách
// PHỤ TRÁCH: LÊ VĂN HOÀNG HIẾU (N24DECE018)
// ============================================================================

// [HÀM TRẢ SÁCH]
// Mục đích: Cập nhật trạng thái giao dịch mượn trả khi độc giả trả sách.
// Luồng xử lý:
//   1. Tìm độc giả trên BST → O(log N)
//   2. Duyệt DSLK tìm node có maSach khớp VÀ trangThai == 0 → O(K)
//   3. Cập nhật: trangThai = 1 (đã trả), ngayTra = ngàyHiệnTại → O(1)
//   4. Gọi API Đức: cập nhật sách DMS → 0 (cho mượn lại)
// Tại sao tìm node thay vì xóa: Giữ lại lịch sử mượn/trả để phục vụ
//   thống kê Top 10 sách mượn nhiều nhất (chức năng j của Đức).
// Bẫy lỗi: maThe không tồn tại, DSLK rỗng, maSach không tìm thấy
//   hoặc không có giao dịch đang mượn (trangThai != 0).
bool TraSach(TREE_DocGia root, int maThe, const char *maSach) {
  // Bước 1: Tìm độc giả trên cây BST
  NodeDocGia *pDG = TimKiemDocGia(root, maThe);
  if (pDG == nullptr) {
    cout << "[LOI] Khong tim thay doc gia co ma the: " << maThe << "\n";
    return false;
  }

  // Bước 2: Duyệt DSLK tìm giao dịch mượn đang hoạt động cho cuốn sách này
  // Điều kiện tìm: maSach khớp VÀ trangThai == 0 (đang mượn)
  NodeMuonTra *pCurr = pDG->data.dsMuonTra;
  while (pCurr != nullptr) {
    if (pCurr->data.trangThai == 0 &&
        strcmp(pCurr->data.maSach, maSach) == 0) {
      // ── Tìm thấy → Cập nhật trạng thái trả sách ──
      pCurr->data.trangThai = 1; // 1 = Đã trả
      pCurr->data.ngayTra =
          LayNgayHienTai_Stub(); // TODO: Thay bằng LayNgayHienTai() của Đức

      // Cập nhật trạng thái sách bên DMS → 0 (cho mượn lại)
      // TODO: Khi ghép nối module Đức:
      //   CapNhatTrangThaiSach(dsDS, maSach, 0);

      cout << "[THANH CONG] Doc gia " << maThe << " da tra sach " << maSach
           << " ngay " << pCurr->data.ngayTra.ngay << "/"
           << pCurr->data.ngayTra.thang << "/" << pCurr->data.ngayTra.nam
           << ".\n";
      return true;
    }
    pCurr = pCurr->pNext;
  }

  // Không tìm thấy giao dịch mượn đang hoạt động
  cout << "[LOI] Doc gia " << maThe << " khong co giao dich muon sach "
       << maSach << " dang hoat dong!\n";
  return false;
}

// [HÀM BÁO MẤT SÁCH]
// Mục đích: Xử lý khi độc giả báo mất sách đang mượn.
// Khác biệt so với TraSach:
//   - trangThai = 2 (Mất sách) thay vì 1 (Đã trả)
//   - Sách bên DMS chuyển sang trạng thái 2 (Thanh lý) thay vì 0 (Cho mượn)
// Ý nghĩa nghiệp vụ: Sách bị mất không thể cho mượn lại, cần thanh lý khỏi kho.
//   Thông tin mất sách được lưu trong lịch sử giao dịch để Thầy/Cô kiểm tra.
// Bẫy lỗi: Tương tự TraSach — maThe không tồn tại, DSLK rỗng, maSach không khớp.
bool BaoMatSach(TREE_DocGia root, int maThe, const char *maSach) {
  // Bước 1: Tìm độc giả
  NodeDocGia *pDG = TimKiemDocGia(root, maThe);
  if (pDG == nullptr) {
    cout << "[LOI] Khong tim thay doc gia co ma the: " << maThe << "\n";
    return false;
  }

  // Bước 2: Duyệt DSLK tìm giao dịch mượn đang hoạt động
  NodeMuonTra *pCurr = pDG->data.dsMuonTra;
  while (pCurr != nullptr) {
    if (pCurr->data.trangThai == 0 &&
        strcmp(pCurr->data.maSach, maSach) == 0) {
      // ── Tìm thấy → Cập nhật trạng thái mất sách ──
      pCurr->data.trangThai = 2; // 2 = Mất sách
      pCurr->data.ngayTra =
          LayNgayHienTai_Stub(); // Ghi nhận ngày báo mất

      // Cập nhật sách bên DMS → 2 (Thanh lý)
      // TODO: Khi ghép nối module Đức:
      //   CapNhatTrangThaiSach(dsDS, maSach, 2);

      cout << "[THANH CONG] Doc gia " << maThe << " da bao mat sach "
           << maSach << ". Sach se duoc thanh ly.\n";
      return true;
    }
    pCurr = pCurr->pNext;
  }

  cout << "[LOI] Doc gia " << maThe << " khong co giao dich muon sach "
       << maSach << " dang hoat dong!\n";
  return false;
}
