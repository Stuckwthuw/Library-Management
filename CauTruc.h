#pragma once

// ============================================================================
// ĐỒ ÁN CẤU TRÚC DỮ LIỆU & GIẢI THUẬT: QUẢN LÝ THƯ VIỆN (C++)
// Cấu trúc dữ liệu dùng chung
// ============================================================================

// Hằng số toàn cục
const int MAX_DAUSACH   = 10000; // Số lượng đầu sách tối đa
const int MAX_MUON_SACH = 3;     // Số sách mượn tối đa cho 1 độc giả
const int MAX_NGAY_MUON = 7;     // Số ngày mượn tối đa trước khi tính quá hạn

// ----------------------------------------------------------------------------
// 1. Cấu trúc ngày tháng (Đức phụ trách)
// ----------------------------------------------------------------------------
struct Date {
    int ngay;
    int thang;
    int nam;
};

// ----------------------------------------------------------------------------
// 2. Danh mục sách - DSLK đơn (Đức phụ trách)
// ----------------------------------------------------------------------------
struct DanhMucSach {
    char maSach[20];       // Mã sách [ISBN]_[STT]
    int trangThai;         // 0: Cho mượn, 1: Đã mượn, 2: Thanh lý
    char viTri[50];        // Vị trí để sách (kệ/ngăn)
};

struct NodeDMS {
    DanhMucSach data;
    NodeDMS* pNext;
};

// ----------------------------------------------------------------------------
// 3. Đầu sách - Mảng con trỏ (Đức phụ trách)
// ----------------------------------------------------------------------------
struct DauSach {
    char ISBN[15];         // Mã ISBN
    char tenSach[100];     // Tên sách (khóa sắp xếp)
    int soTrang;
    char tacGia[50];
    int namXuatBan;
    char theLoai[30];
    NodeDMS* dms;          // Con trỏ tới danh mục sách
};

struct DanhSachDauSach {
    int n;
    DauSach* nodes[MAX_DAUSACH];
};

// ----------------------------------------------------------------------------
// 4. Mượn trả - DSLK đơn (Hiếu phụ trách)
// ----------------------------------------------------------------------------
struct MuonTra {
    char maSach[20];       // Mã sách đang mượn
    Date ngayMuon;
    Date ngayTra;
    int trangThai;         // 0: Đang mượn, 1: Đã trả, 2: Làm mất
};

struct NodeMuonTra {
    MuonTra data;
    NodeMuonTra* pNext;
};

// ----------------------------------------------------------------------------
// 5. Thẻ độc giả - Cây nhị phân tìm kiếm (Hiếu phụ trách)
// ----------------------------------------------------------------------------
struct TheDocGia {
    int maThe;              // Khóa chính của cây BST
    char ho[50];
    char ten[20];
    char phai[5];           // "Nam" hoac "Nu"
    int trangThai;          // 0: Khoa, 1: Hoat dong
    NodeMuonTra* dsMuonTra; // Danh sách các lượt mượn trả
};

struct NodeDocGia {
    TheDocGia data;
    NodeDocGia* pLeft;
    NodeDocGia* pRight;
};

typedef NodeDocGia* TREE_DocGia;
