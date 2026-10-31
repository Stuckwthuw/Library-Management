#include "../DocGia.h"
#include "../MuonTra.h"
#include <cstdio>
#include <cstring>
#include <iostream>

using namespace std;

// ============================================================================
// PHÂN HỆ: GIAO DỊCH MƯỢN TRẢ
// FILE: MuonTra_QuaHan.cpp — Chức năng (i): Danh sách độc giả quá hạn (Ghép nối Date)
// PHỤ TRÁCH: LÊ VĂN HOÀNG HIẾU (N24DECE018)
// ============================================================================

// ############################################################################
// PHẦN 1: ĐẾM TỔNG SỐ SÁCH QUÁ HẠN (Cấp phát mảng đúng kích thước)
// ############################################################################

// [HÀM ĐỆ QUY DUYỆT CÂY BST ĐẾM SÁCH QUÁ HẠN]
// Duyệt toàn bộ cây BST (LNR), với mỗi độc giả duyệt DSLK mượn trả
// tìm các sách đang mượn (trangThai == 0) có số ngày mượn > MAX_NGAY_MUON.
// Tối ưu: Nhận mốc ngày hiện tại một lần duy nhất từ ngoài vào, tránh gọi time()
// lặp đi lặp lại ở mỗi node.
void DemTongSachQuaHan_DuyetCay(TREE_DocGia root, const Date &homNay, int &count) {
  if (root == nullptr)
    return;

  // Duyệt cây con trái
  DemTongSachQuaHan_DuyetCay(root->pLeft, homNay, count);

  // Xử lý node hiện tại: duyệt DSLK mượn trả của độc giả này
  NodeMuonTra *pCurr = root->data.dsMuonTra;
  while (pCurr != nullptr) {
    if (pCurr->data.trangThai == 0) { // Đang mượn
      int soNgayMuon = TinhKhoangCachNgay(pCurr->data.ngayMuon, homNay);
      if (soNgayMuon > MAX_NGAY_MUON)
        count++; // Quá hạn → đếm
    }
    pCurr = pCurr->pNext;
  }

  // Duyệt cây con phải
  DemTongSachQuaHan_DuyetCay(root->pRight, homNay, count);
}

// Wrapper tương thích nguyên mẫu
void DemTongSachQuaHan_DuyetCay(TREE_DocGia root, int &count) {
  Date homNay = LayNgayHienTai();
  DemTongSachQuaHan_DuyetCay(root, homNay, count);
}

int DemTongSachQuaHan(TREE_DocGia root) {
  int count = 0;
  Date homNay = LayNgayHienTai();
  DemTongSachQuaHan_DuyetCay(root, homNay, count);
  return count;
}

// ############################################################################
// PHẦN 2: NẠP THÔNG TIN QUÁ HẠN VÀO MẢNG
// ############################################################################

// [HÀM ĐỆ QUY NẠP DANH SÁCH QUÁ HẠN VÀO MẢNG]
// Duyệt cây BST lần 2 (sau khi đã biết kích thước mảng), nạp thông
// tin quá hạn vào mảng QuaHanInfo[] đã cấp phát đúng kích thước.
void NapDanhSachQuaHan(TREE_DocGia root, const Date &homNay, QuaHanInfo arr[],
                       int &index) {
  if (root == nullptr)
    return;

  // Duyệt cây con trái (LNR)
  NapDanhSachQuaHan(root->pLeft, homNay, arr, index);

  // Xử lý node hiện tại
  NodeMuonTra *pCurr = root->data.dsMuonTra;
  while (pCurr != nullptr) {
    if (pCurr->data.trangThai == 0) { // Đang mượn
      int soNgayMuon = TinhKhoangCachNgay(pCurr->data.ngayMuon, homNay);
      if (soNgayMuon > MAX_NGAY_MUON) {
        // Nạp thông tin quá hạn vào mảng
        arr[index].maThe = root->data.maThe;
        strcpy(arr[index].ho, root->data.ho);
        strcpy(arr[index].ten, root->data.ten);
        strcpy(arr[index].maSach, pCurr->data.maSach);
        arr[index].soNgayQuaHan =
            soNgayMuon - MAX_NGAY_MUON; // Số ngày vượt quá giới hạn
        index++;
      }
    }
    pCurr = pCurr->pNext;
  }

  // Duyệt cây con phải
  NapDanhSachQuaHan(root->pRight, homNay, arr, index);
}

// Wrapper tương thích nguyên mẫu
void NapDanhSachQuaHan(TREE_DocGia root, QuaHanInfo arr[], int &index) {
  Date homNay = LayNgayHienTai();
  NapDanhSachQuaHan(root, homNay, arr, index);
}

// ############################################################################
// PHẦN 3: QUICKSORT MẢNG QUÁ HẠN (GIẢM DẦN THEO SỐ NGÀY QUÁ HẠN)
// ############################################################################

// [THUẬT TOÁN QUICKSORT CHO MẢNG QuaHanInfo — GIẢM DẦN]
// Sắp xếp mảng QuaHanInfo[] theo soNgayQuaHan giảm dần.
//  Người quá hạn nhiều nhất nằm đầu bảng → dễ quản lý và nhắc nhở.
// Giải thuật: QuickSort in-place, chọn pivot ở giữa (middle element)..
void QuickSortQuaHan(QuaHanInfo arr[], int left, int right) {
  if (left >= right)
    return; // Điều kiện dừng đệ quy

  // Chọn pivot ở giữa mảng → giảm nguy cơ O(P^2) khi dữ liệu đã sắp xếp
  int pivotValue = arr[(left + right) / 2].soNgayQuaHan;
  int i = left, j = right;

  // Phân hoạch (Partition): dời các phần tử lớn sang trái, nhỏ sang phải
  // (vì sắp xếp GIẢM DẦN)
  while (i <= j) {
    // Tìm phần tử bên trái nhỏ hơn pivot (cần dời sang phải)
    while (arr[i].soNgayQuaHan > pivotValue)
      i++;
    // Tìm phần tử bên phải lớn hơn pivot (cần dời sang trái)
    while (arr[j].soNgayQuaHan < pivotValue)
      j--;

    if (i <= j) {
      // Hoán đổi 2 phần tử — swap toàn bộ struct QuaHanInfo
      QuaHanInfo temp = arr[i];
      arr[i] = arr[j];
      arr[j] = temp;
      i++;
      j--;
    }
  }

  // Đệ quy sắp xếp 2 nửa mảng con
  if (left < j)
    QuickSortQuaHan(arr, left, j);
  if (i < right)
    QuickSortQuaHan(arr, i, right);
}

// ############################################################################
// PHẦN 4: HÀM TỔNG HỢP — IN DANH SÁCH ĐỘC GIẢ QUÁ HẠN
// ############################################################################

// [HÀM CHÍNH: IN DANH SÁCH ĐỘC GIẢ MƯỢN SÁCH QUÁ HẠN]
// Mục đích: Tổng hợp toàn bộ quy trình từ đếm → cấp phát → nạp → sắp xếp → in.
// Luồng xử lý 5 bước:
//   1. Đếm tổng số sách quá hạn P (duyệt cây BST lần 1) → O(N * K)
//   2. Cấp phát mảng QuaHanInfo[P] bằng new → O(1)
//   3. Nạp dữ liệu quá hạn vào mảng (duyệt cây BST lần 2) → O(N * K)
//   4. QuickSort mảng giảm dần theo soNgayQuaHan → O(P log P)
//   5. In bảng kết quả → O(P)
//   6. Giải phóng mảng bằng delete[] → O(1)
// Tổng: O(N * K + P log P)
// Bẫy lỗi: Cây rỗng, không có sách quá hạn, hết RAM khi cấp phát mảng.
void InDanhSachDocGiaQuaHan(TREE_DocGia root) {
  // Bước 0: Kiểm tra cây rỗng
  if (root == nullptr) {
    cout << "[THONG BAO] Chua co doc gia nao trong he thong.\n";
    return;
  }

  // ── Bước 1: Đếm tổng số sách quá hạn (duyệt cây lần 1) ──
  int tongQuaHan = DemTongSachQuaHan(root);

  if (tongQuaHan == 0) {
    cout << "[THONG BAO] Hien khong co doc gia nao muon sach qua han.\n";
    return;
  }

  // ── Bước 2: Cấp phát mảng QuaHanInfo đúng kích thước ──
  QuaHanInfo *arr = new QuaHanInfo[tongQuaHan];
  if (arr == nullptr) {
    cout << "[LOI] Khong du bo nho de cap phat mang qua han!\n";
    return;
  }

  // ── Bước 3: Nạp dữ liệu quá hạn vào mảng (duyệt cây lần 2) ──
  Date homNay = LayNgayHienTai();
  int index = 0;
  NapDanhSachQuaHan(root, homNay, arr, index);

  // ── Bước 4: QuickSort giảm dần theo soNgayQuaHan ──
  if (index > 1) // Chỉ cần sắp xếp khi có ≥ 2 phần tử
    QuickSortQuaHan(arr, 0, index - 1);

  // ── Bước 5: In bảng kết quả ──
  printf("\n===== DANH SACH DOC GIA MUON SACH QUA HAN =====\n");
  printf("(Sap xep giam dan theo so ngay qua han)\n\n");
  printf("+-----+--------+--------------------+----------+---------------+"
         "-----------------+\n");
  printf("|     |  Ma    |                    |   Ma     |   Ngay Muon   "
         "|  So Ngay Qua Han|\n");
  printf("| STT |  The   |   Ho va Ten        |   Sach   |               "
         "|                 |\n");
  printf("+-----+--------+--------------------+----------+---------------+"
         "-----------------+\n");

  for (int i = 0; i < index; i++) {
    // Ghép họ tên để hiển thị
    char hoTen[72];
    sprintf(hoTen, "%s %s", arr[i].ho, arr[i].ten);

    printf("| %-3d | %-6d | %-18s | %-8s | (qua han)     | %-15d |\n", i + 1,
           arr[i].maThe, hoTen, arr[i].maSach, arr[i].soNgayQuaHan);
  }

  printf("+-----+--------+--------------------+----------+---------------+"
         "-----------------+\n");
  printf("Tong cong: %d luot muon qua han.\n", index);

  // ── Bước 6: Giải phóng mảng — Đi đôi với new ở Bước 2 ──
  delete[] arr;
  arr = nullptr; // Chống Dangling Pointer
}
