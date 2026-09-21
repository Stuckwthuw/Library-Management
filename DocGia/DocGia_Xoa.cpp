#include "../DocGia.h"
#include <iostream>

using namespace std;

// ============================================================================
// PHÂN HỆ: QUẢN LÝ THẺ ĐỘC GIẢ (CÂY NHỊ PHÂN TÌM KIẾM - BST)
// FILE: DocGia_Xoa.cpp - Chức năng Xóa thẻ độc giả khỏi cây BST
// PHỤ TRÁCH: LÊ VĂN HOÀNG HIẾU
// ============================================================================

// [HÀM KIỂM TRA ĐỘC GIẢ CÓ ĐANG MƯỢN SÁCH HAY KHÔNG]
// Mục đích: Ràng buộc nghiệp vụ - chặn thao tác xóa thẻ khi độc giả còn
//   giữ sách của thư viện chưa trả.
// Giải thuật: Duyệt tuyến tính DSLK dsMuonTra của độc giả, tìm node có
//   trangThai == 0 (đang mượn, chưa trả).
// Độ phức tạp: O(M) với M là số lần mượn, thực tế M <= MAX_MUON_SACH = 3 cuốn.
// Trả về: true nếu đang mượn ít nhất 1 cuốn, false nếu đã trả hết hoặc chưa mượn.
bool KiemTraDocGiaDangMuonSach(NodeDocGia *pDG) {
    if (pDG == nullptr)
        return false;
    NodeMuonTra *pCurr = pDG->data.dsMuonTra;
    while (pCurr != nullptr) {
        if (pCurr->data.trangThai == 0)
            return true; // Đang mượn sách!
        pCurr = pCurr->pNext;
    }
    return false; // Đã trả hết hoặc chưa từng mượn
}

// [HÀM TÌM NODE THẾ MẠNG - DÙNG CHO THUẬT TOÁN XÓA NODE 2 CON TRÊN CÂY BST]
// Mục đích: Khi xóa node có 2 con, cần tìm node THAY THẾ (thế mạng) để
//   duy trì tính chất cây BST mà không cần tái cấu trúc lại toàn bộ cây.
// Lý do chọn node cực trái của cây con phải (Phần tử Nhỏ Nhất Bên Phải):
//   - Nó lớn hơn TẤT CẢ node bên cây con trái của X → thỏa điều kiện bên trái.
//   - Nó nhỏ hơn TẤT CẢ node còn lại bên cây con phải của X → thỏa điều kiện bên phải.
//   → Thay X bằng nó là an toàn, tính chất BST được bảo toàn hoàn toàn.
// Cơ chế hoạt động:
//   - X: Node cần xóa (có 2 con). Ban đầu X = node cần xóa.
//   - Y: Node đang duyệt tìm node thế mạng (bắt đầu = X->pRight).
//   - Đi theo nhánh pLeft đến khi Y->pLeft == nullptr: Y chính là node thế mạng.
//   - Sao chép data của Y sang X (ghi đè X), sau đó X trỏ đến Y (để giải phóng Y).
// BẪY LỖI BỘ NHỚ QUAN TRỌNG:
//   Gán Y->data.dsMuonTra = nullptr để khi delete Y, danh sách mượn trả đã
//   chuyển giao sang X không bị xóa nhầm (chống Dangling Pointer / Double Free).
void TimNodeTheMang(NodeDocGia *&X, NodeDocGia *&Y) {
    if (Y->pLeft != nullptr) {
        // Tiếp tục đi sang trái tìm node cực trái (phần tử nhỏ nhất bên phải)
        TimNodeTheMang(X, Y->pLeft);
    } else {
        // Y là node thế mạng: Sao chép data của Y vào X (ghi đè, không sửa con trỏ cây)
        // QUAN TRỌNG: Chỉ sao chép TheDocGia (trường data), không sao chép pLeft/pRight.
        X->data = Y->data;
        // Chống Double Free / Dangling Pointer:
        // Danh sách mượn trả đã chuyển giao sang X, ta ngắt liên kết ở Y
        Y->data.dsMuonTra = nullptr;
        // Chuyển X trỏ đến Y để hàm gọi bên ngoài thực hiện delete pXoa (chính là Y)
        X = Y;
        // Nối lại liên kết: cha của Y trỏ đến con phải của Y (vì Y không còn con trái)
        Y = Y->pRight;
    }
}

// [HÀM XÓA THẺ ĐỘC GIẢ KHỎI CÂY BST]
// Mục đích: Xóa một độc giả khỏi hệ thống theo maThe.
// Ràng buộc nghiệp vụ: Không cho xóa nếu độc giả đang giữ sách chưa trả.
// Giải thuật: Đệ quy tìm node cần xóa, sau đó xử lý 3 trường hợp:
//   TRƯỜNG HỢP 1 - Node lá (0 con):
//     Giải phóng dsMuonTra → delete node → gán nullptr.
//   TRƯỜNG HỢP 2 - Node 1 con (chỉ có con trái HOẶC chỉ có con phải):
//     Cho cha trỏ thẳng đến con duy nhất → delete node. Nối tắt qua node bị xóa.
//   TRƯỜNG HỢP 3 - Node 2 con (phức tạp nhất):
//     Giải phóng dsMuonTra của node bị xóa TRƯỚC (chống Memory Leak).
//     Gọi TimNodeTheMang để tìm node cực trái của cây con phải và sao chép data.
//     Thực tế xóa node thế mạng (đã tách an toàn dsMuonTra = nullptr).
// Độ phức tạp: Trung bình O(log N), tệ nhất O(N).
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
        // Đã tìm được node cần xóa
        // Kiểm tra ràng buộc nghiệp vụ: không xóa khi đang mượn sách
        if (KiemTraDocGiaDangMuonSach(root)) {
            cout << "[CANH BAO] Doc gia ma the " << maThe
                 << " dang muon sach chua tra. Khong the xoa!\n";
            return false;
        }

        NodeDocGia *pXoa = nullptr;

        if (root->pLeft == nullptr && root->pRight == nullptr) {
            // ---- TRƯỜNG HỢP 1: Node lá (không có con) ----
            pXoa = root;
            root = nullptr;
            GiaiPhongDSMuonTra(pXoa->data.dsMuonTra);
            delete pXoa;
        } else if (root->pLeft == nullptr) {
            // ---- TRƯỜNG HỢP 2a: Chỉ có con phải ----
            pXoa = root;
            root = root->pRight;
            GiaiPhongDSMuonTra(pXoa->data.dsMuonTra);
            delete pXoa;
        } else if (root->pRight == nullptr) {
            // ---- TRƯỜNG HỢP 2b: Chỉ có con trái ----
            pXoa = root;
            root = root->pLeft;
            GiaiPhongDSMuonTra(pXoa->data.dsMuonTra);
            delete pXoa;
        } else {
            // ---- TRƯỜNG HỢP 3: Có đủ 2 con ----
            // BẪY LỖI MEMORY LEAK: Giải phóng lịch sử mượn trả của chính độc giả bị
            // xóa TRƯỚC khi dữ liệu node thế mạng ghi đè lên root->data.
            GiaiPhongDSMuonTra(root->data.dsMuonTra);
            pXoa = root;
            TimNodeTheMang(pXoa, root->pRight);
            // Lúc này pXoa trỏ đến node thế mạng Y cũ (Y->data.dsMuonTra đã được gán nullptr an toàn).
            delete pXoa;
        }

        return true;
    }
}
