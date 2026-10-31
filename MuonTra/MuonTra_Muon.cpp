#include "../DocGia.h"
#include "../MuonTra.h"
#include <cstring>
#include <iostream>

using namespace std;

// ============================================================================
// PHÂN HỆ: GIAO DỊCH MƯỢN TRẢ
// FILE: MuonTra_Muon.cpp — Chức năng (f): Nghiệp vụ Mượn Sách
// PHỤ TRÁCH: LÊ VĂN HOÀNG HIẾU (N24DECE018)
// ============================================================================

// [HÀM MƯỢN SÁCH]
// Mục đích: Thực hiện giao dịch mượn sách cho một độc giả.
// Luồng xử lý 7 bước:
//   1. Tìm độc giả trên BST theo maThe
//   2. Kiểm tra 3 điều kiện mượn
//   3. Kiểm tra trùng lặp maSach đang mượn
//   4. Kiểm tra sách có khả dụng trong DMS (gọi API Đức) → sẽ bổ sung
//   5. Cấp phát NodeMuonTra mới
//   6. Chèn vào ĐẦU DSLK dsMuonTra
//   7. Cập nhật trạng thái sách bên DMS → sẽ bổ sung
bool MuonSach(TREE_DocGia root, int maThe, const char *maSach) {
  // ── Bước 1: Tìm độc giả trên cây BST ──
  NodeDocGia *pDG = TimKiemDocGia(root, maThe);
  if (pDG == nullptr) {
    cout << "[LOI] Khong tim thay doc gia co ma the: " << maThe << "\n";
    return false;
  }

  // ── Bước 2: Kiểm tra 3 điều kiện mượn ──
  KetQuaKiemTraMuon maLoi = KiemTraDieuKienMuon(pDG);
  switch (maLoi) {
  case LOI_THE_BI_KHOA:
    cout << "[LOI] The doc gia " << maThe
         << " dang bi khoa. Khong the muon sach!\n";
    return false;
  case LOI_DA_MUON_TOI_DA:
    cout << "[LOI] Doc gia " << maThe << " da muon toi da " << MAX_MUON_SACH
         << " cuon. Khong the muon them!\n";
    return false;
  case LOI_CO_SACH_QUA_HAN:
    cout << "[LOI] Doc gia " << maThe << " dang co sach muon qua han "
         << MAX_NGAY_MUON << " ngay. Khong the muon them!\n";
    return false;
  case DU_DIEU_KIEN_MUON:
    break; // Đủ điều kiện, tiếp tục xử lý mượn sách
  }

  // ── Bước 3: Kiểm tra trùng lặp — độc giả đã đang mượn cuốn này chưa? ──
  // Ngăn mượn 2 lần cùng 1 cuốn sách (cùng maSach và trangThai == 0)
  NodeMuonTra *pCheck = pDG->data.dsMuonTra;
  while (pCheck != nullptr) {
    if (pCheck->data.trangThai == 0 &&
        strcmp(pCheck->data.maSach, maSach) == 0) {
      cout << "[LOI] Doc gia " << maThe << " dang muon cuon sach " << maSach
           << ". Khong the muon trung!\n";
      return false;
    }
    pCheck = pCheck->pNext;
  }

  // ── Bước 4: Kiểm tra sách có khả dụng trong DMS (Module Đức) ──
  // TODO: Khi ghép nối module Đức, bỏ comment đoạn dưới:
  //   NodeDMS *pSach = TimSachTheoMa(dsDS, maSach);
  //   if (pSach == nullptr) {
  //       cout << "[LOI] Ma sach " << maSach << " khong ton tai!\n";
  //       return false;
  //   }
  //   if (pSach->data.trangThai != 0) {
  //       cout << "[LOI] Sach " << maSach << " khong kha dung (trangThai = "
  //            << pSach->data.trangThai << ")!\n";
  //       return false;
  //   }

  // ── Bước 5: Cấp phát NodeMuonTra mới ──
  NodeMuonTra *pNew = new NodeMuonTra;
  if (pNew == nullptr) {
    cout << "[LOI] Khong du bo nho de tao giao dich muon tra!\n";
    return false;
  }

  // Khởi tạo dữ liệu giao dịch mượn
  strcpy(pNew->data.maSach, maSach);
  pNew->data.ngayMuon =
      LayNgayHienTai_Stub(); // TODO: Thay bằng LayNgayHienTai() của Đức
  // Ngày trả chưa có → gán mặc định 0/0/0 (quy ước chưa trả)
  pNew->data.ngayTra.ngay = 0;
  pNew->data.ngayTra.thang = 0;
  pNew->data.ngayTra.nam = 0;
  pNew->data.trangThai = 0; // 0 = Đang mượn

  // ── Bước 6: Chèn vào ĐẦU DSLK dsMuonTra — O(1) ──
  // Kỹ thuật: pNew->pNext trỏ đến node đầu cũ, rồi cập nhật head = pNew
  pNew->pNext = pDG->data.dsMuonTra;
  pDG->data.dsMuonTra = pNew;

  // ── Bước 7: Cập nhật trạng thái sách bên DMS → 1 (đã mượn) ──
  // TODO: Khi ghép nối module Đức:
  //   CapNhatTrangThaiSach(dsDS, maSach, 1);

  cout << "[THANH CONG] Doc gia " << maThe << " da muon sach " << maSach
       << " ngay " << pNew->data.ngayMuon.ngay << "/"
       << pNew->data.ngayMuon.thang << "/" << pNew->data.ngayMuon.nam << ".\n";
  return true;
}
