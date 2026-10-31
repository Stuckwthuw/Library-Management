#include "../DocGia.h"
#include "../MuonTra.h"
#include "../DauSach.h"     // Móc nối API Đức: timSachTheoMaSach()
#include "../DanhMucSach.h" // Móc nối API Đức: capNhatTrangThaiSach()
#include <cstring>
#include <iostream>

using namespace std;

// ============================================================================
// PHÂN HỆ: GIAO DỊCH MƯỢN TRẢ
// FILE: MuonTra_Muon.cpp — Chức năng (f): Nghiệp vụ Mượn Sách (Ghép nối DMS)
// PHỤ TRÁCH: LÊ VĂN HOÀNG HIẾU (N24DECE018)
// ============================================================================

// [HÀM MƯỢN SÁCH — PHIÊN BẢN GHÉP NỐI ĐẦY ĐỦ DMS]
// Mục đích: Thực hiện giao dịch mượn sách cho một độc giả, kiểm tra tồn tại và
//   khả dụng trong Danh Mục Sách của Đức, tự động cập nhật trạng thái sách.
// Luồng xử lý 7 bước:
//   1. Tìm độc giả trên BST theo maThe → O(log N)
//   2. Kiểm tra 3 điều kiện mượn (thẻ mở, < 3 cuốn, không quá hạn) → O(K)
//   3. Kiểm tra trùng lặp maSach đang mượn → O(K)
//   4. Kiểm tra sách có khả dụng trong DMS (gọi API Đức: timSachTheoMaSach) → O(log N + M)
//   5. Cấp phát NodeMuonTra mới → O(1)
//   6. Chèn vào ĐẦU DSLK dsMuonTra → O(1)
//   7. Cập nhật trạng thái sách bên DMS sang 1 (đã mượn - gọi API Đức: capNhatTrangThaiSach) → O(1)
bool MuonSach(TREE_DocGia root, DanhSachDauSach &dsDS, int maThe,
              const char *maSach) {
  // ── Bẫy lỗi định dạng mã sách [ISBN]_[STT] ──
  if (!KiemTraDinhDangMaSach(maSach)) {
    cout << "[LOI] Ma sach \"" << (maSach ? maSach : "")
         << "\" khong dung dinh dang [ISBN]_[STT] (Vi du: CS101_1)!\n";
    return false;
  }

  // ── Bước 1: Tìm độc giả trên cây BST ──
  NodeDocGia *pDG = TimKiemDocGia(root, maThe);
  if (pDG == nullptr) {
    cout << "[LOI] Khong tim thay doc gia co ma the: " << maThe << "\n";
    return false;
  }

  // ── Bước 2: Kiểm tra 3 điều kiện mượn (phiên bản gộp tối ưu) ──
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

  // ── Bước 4: Kiểm tra sách có khả dụng trong DMS (Gọi API Đức) ──
  DauSach *pDauSach = nullptr;
  NodeDMS *pCuonSach = nullptr;
  if (!timSachTheoMaSach(dsDS, maSach, pDauSach, pCuonSach) ||
      pCuonSach == nullptr) {
    cout << "[LOI] Ma sach " << maSach << " khong ton tai trong thu vien!\n";
    return false;
  }
  if (pCuonSach->data.trangThai != 0) {
    if (pCuonSach->data.trangThai == 1) {
      cout << "[LOI] Cuon sach " << maSach << " da co doc gia khac muon!\n";
    } else {
      cout << "[LOI] Cuon sach " << maSach
           << " da bi thanh ly hoac khong con su dung!\n";
    }
    return false;
  }

  // ── Bước 5: Cấp phát NodeMuonTra mới ──
  NodeMuonTra *pNew = new NodeMuonTra;
  if (pNew == nullptr) {
    cout << "[LOI] Khong du bo nho de tao giao dich muon tra!\n";
    return false;
  }

  // Khởi tạo dữ liệu giao dịch mượn
  strcpy(pNew->data.maSach, maSach);
  pNew->data.ngayMuon = LayNgayHienTai();
  pNew->data.ngayTra.ngay = 0;
  pNew->data.ngayTra.thang = 0;
  pNew->data.ngayTra.nam = 0;
  pNew->data.trangThai = 0; // 0 = Đang mượn

  // ── Bước 6: Chèn vào ĐẦU DSLK dsMuonTra — O(1) ──
  pNew->pNext = pDG->data.dsMuonTra;
  pDG->data.dsMuonTra = pNew;

  // ── Bước 7: Cập nhật trạng thái sách bên DMS sang 1 (đã mượn) ──
  capNhatTrangThaiSach(pCuonSach, 1);

  cout << "[THANH CONG] Doc gia " << pDG->data.ho << " " << pDG->data.ten
       << " (Ma: " << maThe << ") da muon sach \""
       << (pDauSach ? pDauSach->tenSach : maSach) << "\" (Ma: " << maSach
       << ") vao ngay " << pNew->data.ngayMuon.ngay << "/"
       << pNew->data.ngayMuon.thang << "/" << pNew->data.ngayMuon.nam << ".\n";
  return true;
}
