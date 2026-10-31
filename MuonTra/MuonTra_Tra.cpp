#include "../DocGia.h"
#include "../MuonTra.h"
#include "../DauSach.h"     // Móc nối API Đức: timSachTheoMaSach()
#include "../DanhMucSach.h" // Móc nối API Đức: capNhatTrangThaiSach()
#include <cstring>
#include <iostream>

using namespace std;

// ============================================================================
// PHÂN HỆ: GIAO DỊCH MƯỢN TRẢ
// FILE: MuonTra_Tra.cpp — Chức năng (g): Trả Sách / Báo Mất Sách (Ghép nối DMS)
// PHỤ TRÁCH: LÊ VĂN HOÀNG HIẾU (N24DECE018)
// ============================================================================

// [HÀM TRẢ SÁCH — PHIÊN BẢN GHÉP NỐI ĐẦY ĐỦ DMS]
// Mục đích: Cập nhật trạng thái giao dịch mượn trả khi độc giả trả sách,
//   đồng thời trả trạng thái sách bên DMS của Đức về 0 (Cho mượn lại).
// Luồng xử lý:
//   1. Tìm độc giả trên BST → O(log N)
//   2. Duyệt DSLK tìm node có maSach khớp VÀ trangThai == 0 (đang mượn) → O(K)
//   3. Cập nhật: trangThai = 1 (đã trả), ngayTra = ngày hiện tại → O(1)
//   4. Gọi API Đức: capNhatTrangThaiSach(pCuonSach, 0)
// Lưu ý: Không xóa node giao dịch để bảo toàn lịch sử phục vụ Top 10 của Đức.
bool TraSach(TREE_DocGia root, DanhSachDauSach &dsDS, int maThe,
             const char *maSach) {
  // Bước 1: Tìm độc giả trên cây BST
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
      // Tìm thấy → Cập nhật trạng thái trả sách
      pCurr->data.trangThai = 1; // 1 = Đã trả
      pCurr->data.ngayTra = LayNgayHienTai();

      // Cập nhật trạng thái sách bên DMS → 0 (cho mượn lại)
      DauSach *pDauSach = nullptr;
      NodeDMS *pCuonSach = nullptr;
      if (timSachTheoMaSach(dsDS, maSach, pDauSach, pCuonSach) &&
          pCuonSach != nullptr) {
        capNhatTrangThaiSach(pCuonSach, 0);
      }

      cout << "[THANH CONG] Doc gia " << pDG->data.ho << " " << pDG->data.ten
           << " da tra sach \"" << (pDauSach ? pDauSach->tenSach : maSach)
           << "\" (Ma: " << maSach << ") vao ngay " << pCurr->data.ngayTra.ngay
           << "/" << pCurr->data.ngayTra.thang << "/"
           << pCurr->data.ngayTra.nam << ".\n";
      return true;
    }
    pCurr = pCurr->pNext;
  }

  cout << "[LOI] Doc gia " << maThe << " khong co giao dich muon sach "
       << maSach << " dang hoat dong!\n";
  return false;
}

// [HÀM BÁO MẤT SÁCH — PHIÊN BẢN GHÉP NỐI ĐẦY ĐỦ DMS]
// Mục đích: Xử lý khi độc giả báo mất sách:
//   - Giao dịch mượn trả chuyển sang trangThai = 2 (Làm mất sách)
//   - Sách bên DMS của Đức chuyển sang trangThai = 2 (Thanh lý)
bool BaoMatSach(TREE_DocGia root, DanhSachDauSach &dsDS, int maThe,
                const char *maSach) {
  NodeDocGia *pDG = TimKiemDocGia(root, maThe);
  if (pDG == nullptr) {
    cout << "[LOI] Khong tim thay doc gia co ma the: " << maThe << "\n";
    return false;
  }

  NodeMuonTra *pCurr = pDG->data.dsMuonTra;
  while (pCurr != nullptr) {
    if (pCurr->data.trangThai == 0 &&
        strcmp(pCurr->data.maSach, maSach) == 0) {
      // Tìm thấy → Cập nhật trạng thái mất sách
      pCurr->data.trangThai = 2; // 2 = Mất sách
      pCurr->data.ngayTra = LayNgayHienTai();

      // Cập nhật sách bên DMS → 2 (Thanh lý)
      DauSach *pDauSach = nullptr;
      NodeDMS *pCuonSach = nullptr;
      if (timSachTheoMaSach(dsDS, maSach, pDauSach, pCuonSach) &&
          pCuonSach != nullptr) {
        capNhatTrangThaiSach(pCuonSach, 2);
      }

      cout << "[THANH CONG] Doc gia " << pDG->data.ho << " " << pDG->data.ten
           << " da bao mat sach \"" << (pDauSach ? pDauSach->tenSach : maSach)
           << "\" (Ma: " << maSach << "). Sach da duoc chuyen sang thanh ly.\n";
      return true;
    }
    pCurr = pCurr->pNext;
  }

  cout << "[LOI] Doc gia " << maThe << " khong co giao dich muon sach "
       << maSach << " dang hoat dong!\n";
  return false;
}
