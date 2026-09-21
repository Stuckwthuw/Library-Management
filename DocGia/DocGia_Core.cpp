#include "../DocGia.h"
#include <iostream>

using namespace std;

// ============================================================================
// PHÂN HỆ: QUẢN LÝ THẺ ĐỘC GIẢ (CÂY NHỊ PHÂN TÌM KIẾM - BST)
// FILE: DocGia_Core.cpp - Các hàm khởi tạo, kiểm tra và quản lý bộ nhớ cây BST
// PHỤ TRÁCH: LÊ VĂN HOÀNG HIẾU
// ============================================================================

// [HÀM KHỞI TẠO CÂY BST]
// Mục đích: Thiết lập con trỏ gốc về nullptr trước khi sử dụng cây.
// Lý do truyền tham chiếu (&root): Cần thay đổi giá trị của con trỏ gốc bên ngoài hàm.
void KhoiTaoCay(TREE_DocGia &root) { 
    root = nullptr; 
}

// [HÀM KIỂM TRA CÂY RỖNG]
// Mục đích: Bẫy lỗi đầu vào trước khi thực hiện các thao tác duyệt cây.
// Tất cả các hàm duyệt, in, tìm kiếm đều phải kiểm tra cây rỗng trước.
bool KiemTraCayRong(TREE_DocGia root) { 
    return root == nullptr; 
}

// [HÀM ĐẾM TỔNG SỐ LƯỢNG ĐỘC GIẢ - ĐỆ QUY]
// Mục đích: Đếm chính xác số node trên cây BST để cấp phát mảng con trỏ tạm thời
//   cho chức năng sắp xếp theo Tên + Họ.
// Giải thuật: Chia để trị (Divide and Conquer). 
//   SoLuong(cây) = 1 (node gốc) + cây con trái + cây con phải.
// Độ phức tạp: O(N) - Duyệt qua mỗi node đúng một lần.
int DemSoLuongDocGia(TREE_DocGia root) {
    if (root == nullptr)
        return 0;
    return 1 + DemSoLuongDocGia(root->pLeft) + DemSoLuongDocGia(root->pRight);
}

// [HÀM GIẢI PHÓNG DSLK MƯỢN TRẢ CỦA MỘT ĐỘC GIẢ]
// Mục đích: Giải phóng toàn bộ bộ nhớ cấp phát động cho DSLK mượn trả trước khi
//   xóa node độc giả để tránh rò rỉ bộ nhớ (Memory Leak).
// Giải thuật: Dùng 2 con trỏ pCurr và pNext duyệt tuyến tính, delete từng node.
// Chống con trỏ hoang (Dangling Pointer): Gán lại dsMuonTra = nullptr.
void GiaiPhongDSMuonTra(NodeMuonTra *&dsMuonTra) {
    NodeMuonTra *pCurr = dsMuonTra;
    while (pCurr != nullptr) {
        NodeMuonTra *pNext = pCurr->pNext; // Lưu lại địa chỉ node tiếp theo
        delete pCurr;                      // Thu hồi bộ nhớ node hiện tại
        pCurr = pNext;                     // Tiến sang node tiếp theo
    }
    dsMuonTra = nullptr;                   // Chống dangling pointer
}

// [HÀM GIẢI PHÓNG TOÀN BỘ BỘ NHỚ CÂY BST - HẬU THỨ TỰ LRN]
// Mục đích: Thu hồi toàn bộ bộ nhớ động đã cấp phát, tránh Memory Leak khi
//   chương trình kết thúc hoặc khi cần tái khởi tạo cây.
// Giải thuật: Duyệt Hậu thứ tự (Post-order: LRN - Trái -> Phải -> Gốc).
//   Với mỗi NodeDocGia, PHẢI giải phóng dsMuonTra TRƯỚC khi delete node đó,
//   vì dsMuonTra là cấp phát động lồng bên trong.
// Bẫy lỗi: Sau khi delete root, luôn gán root = nullptr để chống con trỏ hoang.
// Độ phức tạp: O(N) thời gian, O(h) không gian ngăn xếp đệ quy.
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
