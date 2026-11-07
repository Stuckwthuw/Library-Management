#include "../DocGia.h"
#include <cstdlib>
#include <ctime>
#include <iostream>

// ============================================================================
// PHÂN HỆ: QUẢN LÝ THẺ ĐỘC GIẢ (CÂY NHỊ PHÂN TÌM KIẾM - BST)
// FILE: DocGia_MaThe.cpp - Tìm kiếm và sinh mã thẻ tự động
// PHỤ TRÁCH: LÊ VĂN HOÀNG HIẾU (N24DECE018)
// ============================================================================

// Tìm kiếm độc giả theo mã thẻ trên cây BST
NodeDocGia *TimKiemDocGia(TREE_DocGia root, int maThe) {
    while (root) {
        if (root->data.maThe == maThe) return root;
        root = maThe < root->data.maThe ? root->pLeft : root->pRight;
    }
    return nullptr;
}

// Kiểm tra mã thẻ đã tồn tại trên cây hay chưa
bool KiemTraMaTheTonTai(TREE_DocGia root, int maThe) {
    return TimKiemDocGia(root, maThe) != nullptr;
}

namespace {
const int M = 90000, A = 421, C = 7, OFFSET = 10000;
int state = -1;
}

// Sinh mã thẻ tự động ngẫu nhiên (dãy 5 chữ số 10000 - 99999)
int SinhMaTheTuDong(TREE_DocGia root) {
    if (state < 0) {
        std::srand(static_cast<unsigned>(std::time(nullptr)));
        state = std::rand() % M;
    }
    for (int i = 0; i < M; ++i) {
        state = (A * state + C) % M;
        int ma = state + OFFSET;
        if (!KiemTraMaTheTonTai(root, ma)) return ma;
    }
    std::cout << "[LOI] Kho ma the da day (90000 the).\n";
    return -1;
}
