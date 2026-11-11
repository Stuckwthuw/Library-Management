#include "../DocGia.h"
#include <cstring>
#include <iostream>
#include <new>

using namespace std;

// ============================================================================
// PHÂN HỆ: QUẢN LÝ THẺ ĐỘC GIẢ (CÂY NHỊ PHÂN TÌM KIẾM - BST)
// FILE: DocGia_Them.cpp - Chức năng Thêm thẻ độc giả mới
// PHỤ TRÁCH: LÊ VĂN HOÀNG HIẾU (N24DECE018)
// ============================================================================

// Thêm thẻ độc giả mới vào cây BST
bool ThemDocGia(TREE_DocGia &root, TheDocGia dg) {
  if (!std::memchr(dg.ho, '\0', sizeof(dg.ho)) ||
      !std::memchr(dg.ten, '\0', sizeof(dg.ten)) ||
      !std::memchr(dg.phai, '\0', sizeof(dg.phai)) || dg.maThe < 1 ||
      dg.maThe > 999999 || (dg.trangThai != 0 && dg.trangThai != 1) ||
      (strcmp(dg.phai, "Nam") != 0 && strcmp(dg.phai, "Nu") != 0) ||
      !DinhDangHoTen(dg.ho) || !DinhDangHoTen(dg.ten))
    return false;

  if (!KiemTraChuCaiVaKhoangTrang(dg.ho)) {

    return false;
  }
  if (!KiemTraTenMotTu(dg.ten)) {
    return false;
  }

  if (root == nullptr) {
    NodeDocGia *pNew = new (std::nothrow) NodeDocGia;
    if (pNew == nullptr) {

      return false;
    }
    pNew->data = dg;
    pNew->data.dsMuonTra = dg.dsMuonTra;
    pNew->pLeft = nullptr;
    pNew->pRight = nullptr;
    root = pNew;
    return true;
  }

  if (dg.maThe == root->data.maThe) {

    return false;
  }

  if (dg.maThe < root->data.maThe)
    return ThemDocGia(root->pLeft, dg);
  return ThemDocGia(root->pRight, dg);
}
