#pragma once

// ============================================================================
// ĐỒ ÁN CẤU TRÚC DỮ LIỆU & GIẢI THUẬT: QUẢN LÝ THƯ VIỆN (C++)
// FILE GIAO ƯỚC DỮ LIỆU CHUNG (FROZEN CONTRACT): CauTruc.h
// Cả hai thành viên (Hiếu & Đức) đều sử dụng các cấu trúc dữ liệu này.
// Tuyệt đối KHÔNG tự ý thay đổi tên struct hoặc kiểu dữ liệu các trường.
// ============================================================================

// ----------------------------------------------------------------------------
// CÁC HẰNG SỐ TOÀN CỤC (GLOBAL CONSTANTS)
// ----------------------------------------------------------------------------
const int MAX_DAUSACH   = 10000; // Số lượng đầu sách tối đa trong thư viện
const int MAX_MUON_SACH = 3;     // Số sách tối đa một độc giả được phép mượn cùng lúc
const int MAX_NGAY_MUON = 7;     // Số ngày mượn tối đa cho 1 cuốn sách trước khi bị tính quá hạn

// ============================================================================
// 1. CẤU TRÚC NGÀY THÁNG (MODULE DATE - ĐỨC PHỤ TRÁCH CÀI ĐẶT)
// ============================================================================
// Mục đích: Quản lý ngày mượn, ngày trả và tính khoảng cách số ngày mượn sách.
struct Date {
    int ngay;   // Ngày (1 - 31)
    int thang;  // Tháng (1 - 12)
    int nam;    // Năm (>= 1900)
};

// ============================================================================
// 2. DANH MỤC SÁCH - DSLK ĐƠN (ĐỨC PHỤ TRÁCH CÀI ĐẶT)
// ============================================================================
// Mỗi đầu sách có một danh sách liên kết đơn chứa các bản sách cụ thể.
// Mã sách được đánh tự động theo quy tắc: [ISBN]_[STT] (Ví dụ: CS101_1, CS101_2).
// Trạng thái:
//   0: Cho mượn được (Sách đang nằm trong kho)
//   1: Đã có độc giả mượn (Đang lưu hành bên ngoài)
//   2: Đã thanh lý (Mất, rách, hỏng không còn sử dụng)
struct DanhMucSach {
    char maSach[20];       // Mã sách con duy nhất (gồm ISBN + số thứ tự)
    int trangThai;         // 0: Cho mượn, 1: Đã mượn, 2: Thanh lý
    char viTri[50];        // Vị trí lưu trữ thực tế (Ví dụ: Kệ A1, Ngăn 3)
};

// Node của Danh Sách Liên Kết Đơn Danh Mục Sách
struct NodeDMS {
    DanhMucSach data;      // Dữ liệu của một cuốn sách cụ thể
    NodeDMS* pNext;        // Con trỏ trỏ đến cuốn sách tiếp theo trong cùng đầu sách
};

// ============================================================================
// 3. ĐẦU SÁCH - MẢNG CON TRỎ (ĐỨC PHỤ TRÁCH CÀI ĐẶT)
// ============================================================================
// Danh sách tuyến tính được cài đặt bằng Mảng Con Trỏ.
// Ưu điểm: Tiết kiệm bộ nhớ (chỉ cấp phát khi có sách thật), khi chèn/sắp xếp
// chỉ cần hoán đổi địa chỉ con trỏ 4/8 byte thay vì sao chép cả struct lớn.
// Mảng luôn được duy trì thứ tự TĂNG DẦN theo tenSach để tìm kiếm nhanh.
struct DauSach {
    char ISBN[15];         // Mã tiêu chuẩn quốc tế của sách (Khóa duy nhất)
    char tenSach[100];     // Tên sách (Khóa sắp xếp chính của mảng con trỏ)
    int soTrang;           // Số trang sách
    char tacGia[50];       // Tên tác giả
    int namXuatBan;        // Năm xuất bản
    char theLoai[30];      // Thể loại sách (Tin học, Toán học, Văn học...)
    NodeDMS* dms;          // Con trỏ trỏ đến đầu DSLK Danh Mục Sách của đầu sách này
};

// Cấu trúc quản lý Mảng Con Trỏ Đầu Sách
struct DanhSachDauSach {
    int n;                         // Số lượng đầu sách hiện có trong thư viện (0 <= n <= MAX_DAUSACH)
    DauSach* nodes[MAX_DAUSACH];   // Mảng chứa các con trỏ trỏ đến từng Đầu Sách
};

// ============================================================================
// 4. MƯỢN TRẢ - DSLK ĐƠN (HIẾU PHỤ TRÁCH CÀI ĐẶT)
// ============================================================================
// Gắn với từng độc giả, lưu lại toàn bộ lịch sử các lần mượn và các sách đang giữ.
// Trạng thái:
//   0: Đang mượn (Chưa trả sách, độc giả đang giữ)
//   1: Đã trả (Đã hoàn tất trả sách về thư viện)
//   2: Làm mất sách (Độc giả báo mất, sách chuyển sang thanh lý)
struct MuonTra {
    char maSach[20];       // Mã cuốn sách đang mượn (định dạng [ISBN]_[STT])
    Date ngayMuon;         // Ngày bắt đầu mượn sách
    Date ngayTra;          // Ngày trả lại sách (chỉ có giá trị khi trangThai == 1)
    int trangThai;         // 0: Đang mượn, 1: Đã trả, 2: Làm mất sách
};

// Node của Danh Sách Liên Kết Đơn Mượn Trả
struct NodeMuonTra {
    MuonTra data;          // Thông tin một lượt mượn/trả
    NodeMuonTra* pNext;    // Con trỏ trỏ đến lượt mượn/trả tiếp theo
};

// ============================================================================
// 5. THẺ ĐỘC GIẢ - CÂY NHỊ PHÂN TÌM KIẾM (BST) (HIẾU PHỤ TRÁCH CÀI ĐẶT)
// ============================================================================
// Cài đặt bằng Cây Nhị Phân Tìm Kiếm (Binary Search Tree - BST).
// Khóa chính (Key) là maThe: Số nguyên tự động sinh, không trùng lặp.
// Tính chất BST: Cây con trái có maThe nhỏ hơn node hiện tại, cây con phải lớn hơn.
// Trạng thái thẻ:
//   0: Thẻ bị khóa (Không được phép mượn thêm sách)
//   1: Thẻ đang hoạt động bình thường
struct TheDocGia {
    int maThe;              // Mã thẻ độc giả (Khóa tìm kiếm chính của cây BST)
    char ho[50];            // Họ và chữ lót của độc giả
    char ten[20];           // Tên của độc giả
    char phai[5];           // Giới tính ("Nam" hoặc "Nu")
    int trangThai;          // 0: Bị khóa, 1: Đang hoạt động
    NodeMuonTra* dsMuonTra; // Con trỏ trỏ đến DSLK các lượt mượn/trả của độc giả này
};

// Node của Cây BST Thẻ Độc Giả
struct NodeDocGia {
    TheDocGia data;         // Dữ liệu thông tin thẻ độc giả
    NodeDocGia* pLeft;      // Con trỏ nhánh cây con bên trái (chứa các mã thẻ nhỏ hơn)
    NodeDocGia* pRight;     // Con trỏ nhánh cây con bên phải (chứa các mã thẻ lớn hơn)
};

// Con trỏ gốc của Cây BST Thẻ Độc Giả
typedef NodeDocGia* TREE_DocGia;
