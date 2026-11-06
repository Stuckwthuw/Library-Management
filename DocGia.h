#pragma once

#include "CauTruc.h"

// ============================================================================
// PHÂN HỆ: QUẢN LÝ THẺ ĐỘC GIẢ (CÂY NHỊ PHÂN TÌM KIẾM - BST)
// NGƯỜI PHỤ TRÁCH: LÊ VĂN HOÀNG HIẾU
// FILE: DocGia.h - Khai báo các nguyên mẫu hàm (Function Prototypes)
// ----------------------------------------------------------------------------
// NHÓM 0: TIỆN ÍCH CHUẨN HÓA CHUỖI & XỬ LÝ DỮ LIỆU NHẬP
// ----------------------------------------------------------------------------

// [HÀM KIỂM TRA CHUỖI RỖNG]
bool KiemTraChuoiRong(const char *s);

// [HÀM CHUẨN HÓA KHOẢNG TRẮNG]
void ChuanHoaKhoangTrang(char *s);

// [HÀM ĐỊNH DẠNG TÊN / TITLE CASE]
void DinhDangHoTen(char *s);

// [HÀM CHUẨN HÓA MÃ KÝ HIỆU]
void ChuanHoaMa(char *s);

// [HÀM PHÂN TÍCH LỖI HỌ TÊN]
int KiemTraLoiHoTen(const char *s, char &kyTuViPham);

// [HÀM KIỂM TRA CHỈ CHỨA CHỮ CÁI VÀ KHOẢNG TRẮNG]
bool KiemTraChuCaiVaKhoangTrang(const char *s);

// [HÀM KIỂM TRA ĐỊNH DẠNG MÃ SÁCH CON]
bool KiemTraDinhDangMaSach(const char *maSach);

// [HÀM KIỂM TRA CHỨA CHUỖI CON KHÔNG PHÂN BIỆT HOA THƯỜNG]
bool ChuaChuoiConKhongPhanBietHoaThuong(const char *str, const char *subStr);

// ----------------------------------------------------------------------------
// NHÓM 1: CÁC HÀM CƠ BẢN VÀ QUẢN LÝ BỘ NHỚ TRÊN CÂY BST
// ----------------------------------------------------------------------------

// [HÀM KHỞI TẠO CÂY BST]
void KhoiTaoCay(TREE_DocGia &root);

// [HÀM KIỂM TRA CÂY RỖNG]
bool KiemTraCayRong(TREE_DocGia root);

// [HÀM ĐẾM TỔNG SỐ LƯỢNG ĐỘC GIẢ]
int DemSoLuongDocGia(TREE_DocGia root);

// [HÀM GIẢI PHÓNG DSLK MƯỢN TRẢ CỦA MỘT ĐỘC GIẢ]
void GiaiPhongDSMuonTra(NodeMuonTra *&dsMuonTra);

// [HÀM GIẢI PHÓNG TOÀN BỘ BỘ NHỚ CỦA CÂY BST]
void GiaiPhongCayDocGia(TREE_DocGia &root);

// ----------------------------------------------------------------------------
// NHÓM 2: TÌM KIẾM VÀ SINH MÃ THẺ TỰ ĐỘNG
// ----------------------------------------------------------------------------

// [HÀM TÌM KIẾM ĐỘC GIẢ THEO MÃ THẺ]
NodeDocGia *TimKiemDocGia(TREE_DocGia root, int maThe);

// [HÀM KIỂM TRA MÃ THẺ TỒN TẠI]
bool KiemTraMaTheTonTai(TREE_DocGia root, int maThe);

// [HÀM TÌM KIẾM ĐỘC GIẢ THEO TÊN HOẶC HỌ TÊN (KHÔNG PHÂN BIỆT HOA THƯỜNG)]
int TimKiemDocGiaTheoTen(TREE_DocGia root, const char *tuKhoa, TheDocGia *arr[],
                         int maxKetQua);

// [HÀM SINH MÃ THẺ TỰ ĐỘNG - BỘ SINH ĐỒNG DƯ TUYẾN TÍNH CHU KỲ ĐẦY]
int SinhMaTheTuDong(TREE_DocGia root);

// ----------------------------------------------------------------------------
// NHÓM 3: CHỨC NĂNG (a) - THÊM, HIỆU CHỈNH, XÓA THẺ ĐỘC GIẢ
// ----------------------------------------------------------------------------

// [HÀM KIỂM TRA ĐỘC GIẢ CÓ ĐANG MƯỢN SÁCH HAY KHÔNG]
bool KiemTraDocGiaDangMuonSach(NodeDocGia *pDG);

// [HÀM THÊM ĐỘC GIẢ MỚI VÀO CÂY BST]

bool ThemDocGia(TREE_DocGia &root, TheDocGia dg);

// [HÀM HIỆU CHỈNH THÔNG TIN ĐỘC GIẢ]
bool HieuChinhDocGia(TREE_DocGia root, int maThe, const char *ho,
                     const char *ten, const char *phai, int trangThai);

// [HÀM TÌM NODE THẾ MẠNG CHO THUẬT TOÁN XÓA NODE 2 CON TRÊN CÂY BST]
// Mục đích: Khi xóa một node có 2 con, cần tìm một node thay thế sao cho không
// phá vỡ tính chất cây BST.
void TimNodeTheMang(NodeDocGia *&X, NodeDocGia *&Y);

// [HÀM XÓA THẺ ĐỘC GIẢ KHỎI CÂY BST]
bool XoaDocGia(TREE_DocGia &root, int maThe);

// ----------------------------------------------------------------------------
// NHÓM 4: CHỨC NĂNG (b) - IN DANH SÁCH ĐỘC GIẢ (2 CHẾ ĐỘ)
// ----------------------------------------------------------------------------

// [CHẾ ĐỘ 1: IN DANH SÁCH THEO MÃ THẺ TĂNG DẦN]
void InDanhSachDocGia_TheoMaThe(TREE_DocGia root);

// [HÀM PHỤ TRỢ: ĐỔ TOÀN BỘ NODE TRÊN CÂY BST VÀO MẢNG CON TRỎ TẠM]
void DoCayVaoMang(TREE_DocGia root, TheDocGia *arr[], int &index);

// [HÀM SO SÁNH TÊN VÀ HỌ CỦA 2 ĐỘC GIẢ]
int SoSanhTenHo(const TheDocGia *a, const TheDocGia *b);

// [THUẬT TOÁN QUICKSORT]
void QuickSortDocGia(TheDocGia *arr[], int left, int right);

// [CHẾ ĐỘ 2: IN DANH SÁCH THEO TÊN + HỌ TĂNG DẦN]
void InDanhSachDocGia_TheoTenHo(TREE_DocGia root);

// ----------------------------------------------------------------------------
// NHÓM 5: LƯU TRỮ VÀ PHỤC HỒI DỮ LIỆU TỪ FILE (FILE I/O)
// ----------------------------------------------------------------------------

// [HÀM GHI TOÀN BỘ CÂY ĐỘC GIẢ VÀO FILE]
bool GhiFileDocGia(TREE_DocGia root, const char *filePath = "Data/DocGia.txt");

// [HÀM ĐỌC TOÀN BỘ DỮ LIỆU ĐỘC GIẢ TỪ FILE]
bool DocFileDocGia(TREE_DocGia &root, const char *filePath = "Data/DocGia.txt");
