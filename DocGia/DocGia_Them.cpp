#include "../DocGia.h"
#include <iostream>

using namespace std;

// ============================================================================
// PHÂN HỆ: QUẢN LÝ THẺ ĐỘC GIẢ (CÂY NHỊ PHÂN TÌM KIẾM - BST)
// FILE: DocGia_Them.cpp - Chức năng Thêm thẻ độc giả mới vào cây BST
// PHỤ TRÁCH: LÊ VĂN HOÀNG HIẾU
// ============================================================================

// [HÀM THÊM ĐỘC GIẢ MỚI VÀO CÂY BST - ĐỆ QUY]
// Mục đích: Chèn một thẻ độc giả mới vào đúng vị trí trên cây BST sao cho
//   tính chất BST (trái < gốc < phải theo khóa maThe) không bị vi phạm.
// Giải thuật: Đệ quy so sánh maThe với khóa node hiện tại để tìm vị trí lá
//   trống phù hợp rồi cấp phát động và cắm node mới vào đó.
// Bẫy lỗi:
//   - Từ chối nếu maThe đã tồn tại (tính duy nhất của khóa BST).
//   - Bẫy lỗi cấp phát RAM thất bại (new trả về nullptr do hết bộ nhớ).
//   - Khởi tạo pLeft = nullptr, pRight = nullptr và dsMuonTra (giữ nguyên hoặc nullptr).
// Độ phức tạp: Trung bình O(log N), tệ nhất O(N) khi cây lệch.
bool ThemDocGia(TREE_DocGia &root, TheDocGia dg) {
    if (root == nullptr) {
        // Tìm được vị trí lá trống phù hợp, tiến hành cấp phát node mới
        NodeDocGia *pNew = new NodeDocGia;
        if (pNew == nullptr) {
            // Bẫy lỗi: Hết bộ nhớ RAM
            cout << "[LOI] Khong du bo nho de cap phat node moi!\n";
            return false;
        }
        pNew->data = dg;
        pNew->data.dsMuonTra = dg.dsMuonTra; // Giữ nguyên DSLK đã nạp (khi đọc file)
        pNew->pLeft = nullptr;
        pNew->pRight = nullptr;
        root = pNew;
        return true;
    }

    // Kiểm tra trùng khóa - không cho thêm mã thẻ đã tồn tại
    if (dg.maThe == root->data.maThe) {
        cout << "[LOI] Ma the " << dg.maThe << " da ton tai trong he thong!\n";
        return false;
    }

    // Đệ quy tìm vị trí: nhỏ hơn đi sang nhánh trái, lớn hơn đi sang nhánh phải
    if (dg.maThe < root->data.maThe)
        return ThemDocGia(root->pLeft, dg);
    return ThemDocGia(root->pRight, dg);
}
