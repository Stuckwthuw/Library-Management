#include "../DocGia.h"
#include <cstring>
#include <iostream>

using namespace std;

// ============================================================================
// PHÂN HỆ: QUẢN LÝ THẺ ĐỘC GIẢ (CÂY NHỊ PHÂN TÌM KIẾM - BST)
// FILE: DocGia_HieuChinh.cpp - Chức năng Hiệu chỉnh thông tin độc giả
// PHỤ TRÁCH: LÊ VĂN HOÀNG HIẾU (N24DECE018)
// ============================================================================

// Hiệu chỉnh thông tin độc giả (họ, tên, phái, trạng thái) theo mã thẻ
bool HieuChinhDocGia(TREE_DocGia root, int maThe, const char *ho,
                     const char *ten, const char *phai, int trangThai) {
  NodeDocGia *pDG = TimKiemDocGia(root, maThe);
  if (pDG == nullptr) {

    return false;
  }

  if (trangThai != 0 && trangThai != 1) {

    return false;
  }

  if (!KiemTraChuCaiVaKhoangTrang(ho)) {

    return false;
  }
  if (!KiemTraTenMotTu(ten)) {
    return false;
  }

  if (!ho || !ten || !phai || strlen(ho) >= sizeof(pDG->data.ho) ||
      strlen(ten) >= sizeof(pDG->data.ten) ||
      (strcmp(phai, "Nam") != 0 && strcmp(phai, "Nu") != 0)) return false;
  TheDocGia tam = pDG->data;
  strcpy(tam.ho, ho); strcpy(tam.ten, ten);
  if (!DinhDangHoTen(tam.ho) || !DinhDangHoTen(tam.ten)) return false;

  strncpy(pDG->data.ho, ho, sizeof(pDG->data.ho) - 1);
  strncpy(pDG->data.ten, ten, sizeof(pDG->data.ten) - 1);
  strncpy(pDG->data.phai, phai, sizeof(pDG->data.phai) - 1);
  pDG->data.trangThai = trangThai;

  pDG->data.ho[sizeof(pDG->data.ho) - 1] = '\0';
  pDG->data.ten[sizeof(pDG->data.ten) - 1] = '\0';
  pDG->data.phai[sizeof(pDG->data.phai) - 1] = '\0';

  DinhDangHoTen(pDG->data.ho);
  DinhDangHoTen(pDG->data.ten);

  return true;
}
