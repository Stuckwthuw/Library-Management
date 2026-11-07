#include "../DocGia.h"

// ============================================================================
// PHÂN HỆ: QUẢN LÝ THẺ ĐỘC GIẢ (CÂY NHỊ PHÂN TÌM KIẾM - BST)
// FILE: DocGia_LuuTru.cpp - Lưu trữ lịch sử mượn trả của các thẻ đã xóa
// PHỤ TRÁCH: LÊ VĂN HOÀNG HIẾU (N24DECE018)
// ============================================================================

namespace {
NodeMuonTra *lichSu = nullptr;
NodeMuonTra *cuoi = nullptr;
}

// Lấy danh sách lịch sử lưu trữ
const NodeMuonTra *LayLichSuLuuTru() { return lichSu; }

// Tách danh sách lịch sử lưu trữ ra ngoài
NodeMuonTra *TachLichSuLuuTru() {
    NodeMuonTra *result = lichSu;
    lichSu = nullptr;
    cuoi = nullptr;
    return result;
}

// Nối danh sách mượn trả vào kho lưu trữ khi xóa thẻ độc giả
void LuuTruLichSu(NodeMuonTra *&head) {
    if (!head) return;
    if (cuoi) cuoi->pNext = head;
    else lichSu = head;
    cuoi = head;
    while (cuoi->pNext) cuoi = cuoi->pNext;
    head = nullptr;
}

// Giải phóng bộ nhớ danh sách lịch sử
void GiaiPhongLichSuLuuTru() {
    GiaiPhongDSMuonTra(lichSu);
    cuoi = nullptr;
}

// Cập nhật lại danh sách lịch sử
void ThayLichSuLuuTru(NodeMuonTra *&head) {
    GiaiPhongLichSuLuuTru();
    LuuTruLichSu(head);
}
