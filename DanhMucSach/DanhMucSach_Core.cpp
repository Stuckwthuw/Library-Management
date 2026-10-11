#include "../DanhMucSach.h"
#include <cstring>
#include <stdexcept>

// Khởi tạo danh sách mới; cần giải phóng trước nếu đang sở hữu các node.
void khoiTaoDMS(NodeDMS *&head) { head = nullptr; }
bool danhMucSachRong(NodeDMS *head) { return head == nullptr; }

// Từ chối chuỗi quá dài thay vì cắt: cắt mã có thể làm hai cuốn trùng mã.
NodeDMS *taoNodeDMS(const char *ma, int tt, const char *vt) {
    if (!ma || !vt || !*ma || !*vt ||
        std::strlen(ma) >= sizeof(DanhMucSach::maSach) ||
        std::strlen(vt) >= sizeof(DanhMucSach::viTri) || tt < 0 || tt > 2)
        throw std::invalid_argument("Ma sach, vi tri hoac trang thai khong hop le");
    NodeDMS *p = new NodeDMS{};
    std::strcpy(p->data.maSach, ma);
    std::strcpy(p->data.viTri, vt);
    p->data.trangThai = tt;
    return p;
}

// Nối node độc lập O(M). Từ chối mã trùng mà không nhận quyền sở hữu node.
bool themCuoiDMS(NodeDMS *&head, NodeDMS *node) {
    if (!node || node->pNext || timKiemDMS(head, node->data.maSach)) return false;
    NodeDMS **p = &head;
    while (*p) p = &(*p)->pNext;
    *p = node;
    return true;
}

int demSoLuongDMS(NodeDMS *head) {
    int n = 0;
    for (; head; head = head->pNext) ++n;
    return n;
}

// DSLK đơn cần duyệt tuần tự O(M); trả nullptr nếu rỗng/không tìm thấy.
NodeDMS *timKiemDMS(NodeDMS *head, const char *ma) {
    if (!ma) return nullptr;
    for (; head; head = head->pNext)
        if (std::strcmp(head->data.maSach, ma) == 0) return head;
    return nullptr;
}

// Giữ liên kết kế trước khi delete; đưa đầu danh sách về nullptr.
void giaiPhongDMS(NodeDMS *&head) {
    while (head) {
        NodeDMS *p = head;
        head = head->pNext;
        delete p;
        p = nullptr;
    }
}
