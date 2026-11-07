#include "../DocGia.h"
#include "../MuonTra.h"
#include <cstdio>
#include <cstring>
#include <iostream>

using namespace std;

// ============================================================================
// PHÂN HỆ: GIAO DỊCH MƯỢN TRẢ
// FILE: MuonTra_QuaHan.cpp — Danh sách độc giả mượn sách quá hạn
// PHỤ TRÁCH: LÊ VĂN HOÀNG HIẾU (N24DECE018)
// ============================================================================

// Duyệt cây BST đếm số sách mượn quá hạn
static void DemTongSachQuaHan_DuyetCay(TREE_DocGia root, const Date &homNay, int &count) {
  if (root == nullptr)
    return;

  DemTongSachQuaHan_DuyetCay(root->pLeft, homNay, count);

  NodeMuonTra *pCurr = root->data.dsMuonTra;
  while (pCurr != nullptr) {
    if (pCurr->data.trangThai == 0) {
      int soNgayMuon = TinhKhoangCachNgay(pCurr->data.ngayMuon, homNay);
      if (soNgayMuon > MAX_NGAY_MUON)
        count++;
    }
    pCurr = pCurr->pNext;
  }

  DemTongSachQuaHan_DuyetCay(root->pRight, homNay, count);
}

// Đếm tổng số sách quá hạn trên toàn hệ thống
int DemTongSachQuaHan(TREE_DocGia root) {
  int count = 0;
  Date homNay = LayNgayHienTai();
  DemTongSachQuaHan_DuyetCay(root, homNay, count);
  return count;
}

// Nạp thông tin các lượt mượn quá hạn vào mảng
static void NapDanhSachQuaHan(TREE_DocGia root, const Date &homNay, QuaHanInfo arr[],
                              int &index) {
  if (root == nullptr)
    return;

  NapDanhSachQuaHan(root->pLeft, homNay, arr, index);

  NodeMuonTra *pCurr = root->data.dsMuonTra;
  while (pCurr != nullptr) {
    if (pCurr->data.trangThai == 0) {
      int soNgayMuon = TinhKhoangCachNgay(pCurr->data.ngayMuon, homNay);
      if (soNgayMuon > MAX_NGAY_MUON) {
        arr[index].maThe = root->data.maThe;
        strcpy(arr[index].ho, root->data.ho);
        strcpy(arr[index].ten, root->data.ten);
        strcpy(arr[index].maSach, pCurr->data.maSach);
        arr[index].ngayMuon = pCurr->data.ngayMuon;
        arr[index].soNgayQuaHan = soNgayMuon - MAX_NGAY_MUON;
        index++;
      }
    }
    pCurr = pCurr->pNext;
  }

  NapDanhSachQuaHan(root->pRight, homNay, arr, index);
}

// Sắp xếp mảng quá hạn theo số ngày giảm dần bằng QuickSort
static void QuickSortQuaHan(QuaHanInfo arr[], int left, int right) {
  if (left >= right)
    return;

  int pivotValue = arr[(left + right) / 2].soNgayQuaHan;
  int i = left, j = right;

  while (i <= j) {
    while (arr[i].soNgayQuaHan > pivotValue)
      i++;
    while (arr[j].soNgayQuaHan < pivotValue)
      j--;

    if (i <= j) {
      QuaHanInfo temp = arr[i];
      arr[i] = arr[j];
      arr[j] = temp;
      i++;
      j--;
    }
  }

  if (left < j)
    QuickSortQuaHan(arr, left, j);
  if (i < right)
    QuickSortQuaHan(arr, i, right);
}

// Lập danh sách độc giả mượn sách quá hạn và sắp xếp giảm dần
int LapDanhSachQuaHan(TREE_DocGia root, QuaHanInfo *&ketQua) {
  ketQua = nullptr;
  Date homNay = LayNgayHienTai();
  int n = 0;
  DemTongSachQuaHan_DuyetCay(root, homNay, n);
  if (!n) return 0;
  ketQua = new QuaHanInfo[n];
  int index = 0;
  NapDanhSachQuaHan(root, homNay, ketQua, index);
  if (index > 1) QuickSortQuaHan(ketQua, 0, index - 1);
  return index;
}
