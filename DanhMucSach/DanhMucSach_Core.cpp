#include "../DanhMucSach.h"
#include "../Common/Chuoi.h"
#include <cstring>
#include <stdexcept>

// ============================================================================
// PHÂN HỆ: QUẢN LÝ DANH MỤC SÁCH (DSLK ĐƠN)
// FILE: DanhMucSach_Core.cpp — Các hàm cơ bản của danh mục sách con
// PHỤ TRÁCH: LÊ ANH ĐỨC (N24DECE012)
// ============================================================================

// Khởi tạo danh mục sách
void khoiTaoDMS(NodeDMS *&head) { head = nullptr; }

// Kiểm tra danh mục sách rỗng
bool danhMucSachRong(NodeDMS *head) { return head == nullptr; }

// Tạo một node danh mục sách mới
NodeDMS *taoNodeDMS(const char *ma, int tt, const char *vt) {
    if (!ma || !vt || !*ma || !*vt ||
        std::strlen(ma) >= sizeof(DanhMucSach::maSach) ||
        std::strlen(vt) >= sizeof(DanhMucSach::viTri) || tt < 0 || tt > 2 ||
        !Chuoi::hopLe(ma) || !Chuoi::hopLe(vt) || !Chuoi::coNoiDung(vt))
        throw std::invalid_argument("Ma sach, vi tri hoac trang thai khong hop le");
    NodeDMS *p = new NodeDMS{};
    std::strcpy(p->data.maSach, ma);
    std::strcpy(p->data.viTri, vt);
    p->data.trangThai = tt;
    return p;
}

// Thêm node sách con vào cuối danh sách liên kết
bool themCuoiDMS(NodeDMS *&head, NodeDMS *node) {
    if (!node || node->pNext || timKiemDMS(head, node->data.maSach)) return false;
    NodeDMS **p = &head;
    while (*p) p = &(*p)->pNext;
    *p = node;
    return true;
}

// Đếm số lượng bản sao sách trong danh mục
int demSoLuongDMS(NodeDMS *head) {
    int n = 0;
    for (; head; head = head->pNext) ++n;
    return n;
}

// Tìm kiếm bản sao sách theo mã sách ([ISBN]_[STT])
NodeDMS *timKiemDMS(NodeDMS *head, const char *ma) {
    if (!ma) return nullptr;
    for (; head; head = head->pNext)
        if (std::strcmp(head->data.maSach, ma) == 0) return head;
    return nullptr;
}

// Giải phóng toàn bộ danh mục sách
void giaiPhongDMS(NodeDMS *&head) {
    while (head) {
        NodeDMS *p = head;
        head = head->pNext;
        delete p;
    }
}
