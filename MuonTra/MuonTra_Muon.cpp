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

    return false;
  }

  NodeDocGia *pDG = TimKiemDocGia(root, maThe);
  if (pDG == nullptr) {

    return false;
  }

  KetQuaKiemTraMuon maLoi = KiemTraDieuKienMuon(pDG);
  switch (maLoi) {
  case LOI_DU_LIEU_MUON:

    return false;
  case LOI_THE_BI_KHOA:

    return false;
  case LOI_DA_MUON_TOI_DA:

    return false;
  case LOI_CO_SACH_QUA_HAN:

    return false;
  case DU_DIEU_KIEN_MUON:
    break;
  }

  NodeMuonTra *pCheck = pDG->data.dsMuonTra;
  while (pCheck != nullptr) {
    if (pCheck->data.trangThai == 0 &&
        CungISBN(pCheck->data.maSach, maSach)) {

      return false;
    }
    pCheck = pCheck->pNext;
  }

  DauSach *pDauSach = nullptr;
  NodeDMS *pCuonSach = nullptr;
  if (!timSachTheoMaSach(dsDS, maSach, pDauSach, pCuonSach) ||
      pCuonSach == nullptr) {

    return false;
  }
  if (pCuonSach->data.trangThai != 0) {
    return false;
  }

  NodeMuonTra *pNew = new (std::nothrow) NodeMuonTra;
  if (pNew == nullptr) {

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



  return true;
}
