#include "../DocGia.h"
#include <iostream>

using namespace std;

// ============================================================================
// PHÂN HỆ: QUẢN LÝ THẺ ĐỘC GIẢ (CÂY NHỊ PHÂN TÌM KIẾM - BST)
// FILE: DocGia_Xoa.cpp - Chức năng Xóa thẻ độc giả khỏi cây BST
// PHỤ TRÁCH: LÊ VĂN HOÀNG HIẾU (N24DECE018)
// ============================================================================

// Kiểm tra độc giả có đang mượn sách hay không
bool KiemTraDocGiaDangMuonSach(NodeDocGia *pDG) {
  if (pDG == nullptr)
    return false;
  NodeMuonTra *pCurr = pDG->data.dsMuonTra;
  while (pCurr != nullptr) {
    if (pCurr->data.trangThai == 0)
      return true;
    pCurr = pCurr->pNext;
  }
  return false;
}

// Tìm node thế mạng (node cực trái của cây con phải) để xóa node có 2 con
void TimNodeTheMang(NodeDocGia *&X, NodeDocGia *&Y) {
  if (Y->pLeft != nullptr) {
    TimNodeTheMang(X, Y->pLeft);
  } else {
    X->data = Y->data;
    Y->data.dsMuonTra = nullptr;
    X = Y;
    Y = Y->pRight;
  }
}

// Xóa thẻ độc giả khỏi cây BST theo mã thẻ
bool XoaDocGia(TREE_DocGia &root, int maThe) {
  if (root == nullptr) {
    cout << "[LOI] Khong tim thay doc gia co ma the: " << maThe << "\n";
    return false;
  }

  if (maThe < root->data.maThe) {
    return XoaDocGia(root->pLeft, maThe);
  } else if (maThe > root->data.maThe) {
    return XoaDocGia(root->pRight, maThe);
  } else {
    if (KiemTraDocGiaDangMuonSach(root)) {
      cout << "[CANH BAO] Doc gia ma the " << maThe
           << " dang muon sach chua tra. Khong the xoa!\n";
      return false;
    }

    NodeDocGia *pXoa = nullptr;

    if (root->pLeft == nullptr && root->pRight == nullptr) {
      // Trường hợp 1: Node lá
      pXoa = root;
      root = nullptr;
      LuuTruLichSu(pXoa->data.dsMuonTra);
      delete pXoa;
      pXoa = nullptr;
    } else if (root->pLeft == nullptr) {
      // Trường hợp 2a: Chỉ có cây con phải
      pXoa = root;
      root = root->pRight;
      LuuTruLichSu(pXoa->data.dsMuonTra);
      delete pXoa;
      pXoa = nullptr;
    } else if (root->pRight == nullptr) {
      // Trường hợp 2b: Chỉ có cây con trái
      pXoa = root;
      root = root->pLeft;
      LuuTruLichSu(pXoa->data.dsMuonTra);
      delete pXoa;
      pXoa = nullptr;
    } else {
      // Trường hợp 3: Node có đủ 2 con
      LuuTruLichSu(root->data.dsMuonTra);
      pXoa = root;
      TimNodeTheMang(pXoa, root->pRight);
      delete pXoa;
      pXoa = nullptr;
    }

    return true;
  }
}
