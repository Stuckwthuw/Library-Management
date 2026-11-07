#include "../MuonTra.h"
#include "../DocGia.h"

// ============================================================================
// PHÂN HỆ: GIAO DỊCH MƯỢN TRẢ
// FILE: MuonTra_LietKe.cpp — Lấy danh sách sách đang mượn của độc giả
// PHỤ TRÁCH: LÊ VĂN HOÀNG HIẾU (N24DECE018)
// ============================================================================

// Lấy danh sách các giao dịch đang mượn của độc giả vào mảng con trỏ
int LaySachDangMuon(TREE_DocGia root, int maThe, const MuonTra **out, int capacity) {
    const NodeDocGia *dg = TimKiemDocGia(root, maThe);
    if (!dg || !out || capacity < 0) return -1;
    int n = 0;
    for (const NodeMuonTra *p = dg->data.dsMuonTra; p; p = p->pNext) {
        if (p->data.trangThai == 0) {
            if (n == capacity) return -1;
            out[n++] = &p->data;
        }
    }
    return n;
}
