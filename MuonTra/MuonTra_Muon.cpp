#include "../DocGia.h"
#include "../MuonTra.h"
#include "../DauSach.h"
#include "../DanhMucSach.h"
#include <cstring>
#include <iostream>
#include <new>

using namespace std;

// ============================================================================
// PHÂN HỆ: GIAO DỊCH MƯỢN TRẢ
// FILE: MuonTra_Muon.cpp — Chức năng mượn sách
// PHỤ TRÁCH: LÊ VĂN HOÀNG HIẾU (N24DECE018)
// ============================================================================

// Xử lý giao dịch mượn sách cho độc giả
bool MuonSach(TREE_DocGia root, DanhSachDauSach &dsDS, int maThe,
              const char *maSach) {
  if (!KiemTraDinhDangMaSach(maSach)) {
    cout << "[LOI] Ma sach \"" << (maSach ? maSach : "")
         << "\" khong dung dinh dang [ISBN]_[STT] (Vi du: CS101_1)!\n";
    return false;
  }

  NodeDocGia *pDG = TimKiemDocGia(root, maThe);
  if (pDG == nullptr) {
    cout << "[LOI] Khong tim thay doc gia co ma the: " << maThe << "\n";
    return false;
  }

  KetQuaKiemTraMuon maLoi = KiemTraDieuKienMuon(pDG);
  switch (maLoi) {
  case LOI_DU_LIEU_MUON:
    cout << "[LOI] Du lieu muon khong hop le.\n";
    return false;
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
    break;
  }

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

  NodeMuonTra *pNew = new (std::nothrow) NodeMuonTra;
  if (pNew == nullptr) {
    cout << "[LOI] Khong du bo nho de tao giao dich muon tra!\n";
    return false;
  }

  strncpy(pNew->data.maSach, maSach, sizeof(pNew->data.maSach) - 1);
  pNew->data.maSach[sizeof(pNew->data.maSach) - 1] = '\0';
  pNew->data.ngayMuon = LayNgayHienTai();
  pNew->data.ngayTra = {0, 0, 0};
  pNew->data.trangThai = 0;

  pNew->pNext = pDG->data.dsMuonTra;
  pDG->data.dsMuonTra = pNew;

  capNhatTrangThaiSach(pCuonSach, 1);

  cout << "[THANH CONG] Doc gia " << pDG->data.ho << " " << pDG->data.ten
       << " (Ma: " << maThe << ") da muon sach \""
       << (pDauSach ? pDauSach->tenSach : maSach)
       << "\" (Ma: " << maSach << ") vao ngay " << pNew->data.ngayMuon.ngay
       << "/" << pNew->data.ngayMuon.thang << "/"
       << pNew->data.ngayMuon.nam << ".\n";

  return true;
}
