#include "../DanhMucSach.h"

// ============================================================================
// PHÂN HỆ: QUẢN LÝ DANH MỤC SÁCH (DSLK ĐƠN)
// FILE: DanhMucSach_TrangThai.cpp — Cập nhật trạng thái cuốn sách
// PHỤ TRÁCH: LÊ ANH ĐỨC (N24DECE012)
// ============================================================================

// Cập nhật trạng thái cuốn sách (0: Cho mượn, 1: Đã mượn, 2: Thanh lý)
bool capNhatTrangThaiSach(NodeDMS *node, int trangThai) {
    if (!node || trangThai < 0 || trangThai > 2) return false;
    node->data.trangThai = trangThai;
    return true;
}
