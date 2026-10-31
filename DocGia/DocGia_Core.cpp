#include "../DocGia.h"
#include <iostream>

using namespace std;

// PHÂN HỆ: QUẢN LÝ THẺ ĐỘC GIẢ (CÂY NHỊ PHÂN TÌM KIẾM - BST)
// FILE: DocGia_Core.cpp - Các hàm khởi tạo, kiểm tra và quản lý bộ nhớ cây BST
// PHỤ TRÁCH: LÊ VĂN HOÀNG HIẾU

// [HÀM KHỞI TẠO CÂY BST]
// Mục đích: Thiết lập con trỏ gốc về nullptr trước khi sử dụng cây.
void KhoiTaoCay(TREE_DocGia &root) { root = nullptr; }

// [HÀM KIỂM TRA CÂY RỖNG]
bool KiemTraCayRong(TREE_DocGia root) { return root == nullptr; }

// [HÀM ĐẾM TỔNG SỐ LƯỢNG ĐỘC GIẢ - ĐỆ QUY]
// SoLuong(cây) = 1 (node gốc) + cây con trái + cây con phải.
int DemSoLuongDocGia(TREE_DocGia root) {
  if (root == nullptr)
    return 0;
  return 1 + DemSoLuongDocGia(root->pLeft) + DemSoLuongDocGia(root->pRight);
}

// [HÀM GIẢI PHÓNG DSLK MƯỢN TRẢ CỦA MỘT ĐỘC GIẢ]
// Chống con trỏ hoang (Dangling Pointer): Gán lại dsMuonTra = nullptr.
void GiaiPhongDSMuonTra(NodeMuonTra *&dsMuonTra) {
  NodeMuonTra *pCurr = dsMuonTra;
  while (pCurr != nullptr) {
    NodeMuonTra *pNext = pCurr->pNext; // Lưu lại địa chỉ node tiếp theo
    delete pCurr;                      // Thu hồi bộ nhớ node hiện tại
    pCurr = pNext;                     // Tiến sang node tiếp theo
  }
  dsMuonTra = nullptr; // Chống dangling pointer
}

// [HÀM GIẢI PHÓNG TOÀN BỘ BỘ NHỚ CÂY BST - HẬU THỨ TỰ LRN]
// Sau khi delete root, luôn gán root = nullptr để chống con trỏ hoang.
void GiaiPhongCayDocGia(TREE_DocGia &root) {
  if (root == nullptr)
    return; // Cây con rỗng thì dừng
  // Đệ quy giải phóng toàn bộ cây con bên trái
  GiaiPhongCayDocGia(root->pLeft);
  // Đệ quy giải phóng toàn bộ cây con bên phải
  GiaiPhongCayDocGia(root->pRight);
  // Giải phóng DSLK mượn trả gắn với node hiện tại TRƯỚC
  GiaiPhongDSMuonTra(root->data.dsMuonTra);
  // Giải phóng bản thân node hiện tại và chống dangling pointer
  delete root;
  root = nullptr;
}
