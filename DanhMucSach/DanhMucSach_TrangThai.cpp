#include "../DanhMucSach.h"

// API cho Hiếu sau khi kiểm tra nghiệp vụ; không ghi trạng thái ngoài 0/1/2.
bool capNhatTrangThaiSach(NodeDMS *node, int trangThai) {
    if (!node || trangThai < 0 || trangThai > 2) return false;
    node->data.trangThai = trangThai;
    return true;
}
