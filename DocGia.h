#pragma once

#include "CauTruc.h"

// ============================================================================
// PHÂN HỆ: QUẢN LÝ THẺ ĐỘC GIẢ (CÂY NHỊ PHÂN TÌM KIẾM - BST)
// NGƯỜI PHỤ TRÁCH: LÊ VĂN HOÀNG HIẾU
// FILE: DocGia.h - Khai báo các nguyên mẫu hàm (Function Prototypes)
// ----------------------------------------------------------------------------
// NHÓM 1: CÁC HÀM CƠ BẢN VÀ QUẢN LÝ BỘ NHỚ TRÊN CÂY BST
// ----------------------------------------------------------------------------

// [HÀM KHỞI TẠO CÂY BST]
void KhoiTaoCay(TREE_DocGia &root);

// [HÀM KIỂM TRA CÂY RỖNG]
bool KiemTraCayRong(TREE_DocGia root);

// [HÀM ĐẾM TỔNG SỐ LƯỢNG ĐỘC GIẢ]
// Mục đích: Đếm số lượng node hiện có trên cây BST bằng giải thuật đệ quy.
int DemSoLuongDocGia(TREE_DocGia root);

// [HÀM GIẢI PHÓNG DSLK MƯỢN TRẢ CỦA MỘT ĐỘC GIẢ]
// Mục đích: Thu hồi bộ nhớ danh sách liên kết mượn trả khi xóa một độc giả hoặc khi giải phóng toàn bộ cây.
void GiaiPhongDSMuonTra(NodeMuonTra *&dsMuonTra);

// [HÀM GIẢI PHÓNG TOÀN BỘ BỘ NHỚ CỦA CÂY BST]
// Mục đích: Thu hồi toàn bộ bộ nhớ đã cấp phát động (tránh rò rỉ bộ nhớ -
// Memory Leak). Giải thuật: Duyệt hậu thứ tự (Post-order: LRN - Trái -> Phải ->
// Gốc). Chú ý: Trước khi delete mỗi NodeDocGia, bắt buộc phải duyệt giải phóng
// toàn bộ DSLK dsMuonTra gắn với độc giả đó. Cuối cùng gán root = nullptr.
void GiaiPhongCayDocGia(TREE_DocGia &root);

// ----------------------------------------------------------------------------
// NHÓM 2: TÌM KIẾM VÀ SINH MÃ THẺ TỰ ĐỘNG
// ----------------------------------------------------------------------------

// [HÀM TÌM KIẾM ĐỘC GIẢ THEO MÃ THẺ]
// Mục đích: Tìm kiếm một node trong cây BST dựa vào khóa chính maThe.
// Giải thuật: Tìm kiếm nhị phân trên cây BST (O(log N) trung bình, O(N) tệ
// nhất).
NodeDocGia *TimKiemDocGia(TREE_DocGia root, int maThe);

// [HÀM KIỂM TRA MÃ THẺ TỒN TẠI]
// Mục đích: Kiểm tra nhanh xem maThe đã có trong hệ thống hay chưa.
// Trả về: true nếu đã tồn tại, false nếu chưa có.
bool KiemTraMaTheTonTai(TREE_DocGia root, int maThe);

// [HÀM SINH MÃ THẺ TỰ ĐỘNG - BỘ SINH ĐỒNG DƯ TUYẾN TÍNH CHU KỲ ĐẦY]
// Mục đích: Tự động cấp mã thẻ duy nhất, phân bố giả ngẫu nhiên để cây BST
//   gần cân bằng mà không cần dùng cây AVL.
// Giải thuật: Full-Period Linear Congruential Generator (Định lý Hull-Dobell).
//   Công thức: X_{n+1} = (a * X_n + c) mod M
//   Đảm bảo: Ghé thăm đúng M giá trị khác nhau trước khi lặp lại → O(1) mỗi lần
//   sinh, không bao giờ tự trùng trong cùng một phiên chạy.
int SinhMaTheTuDong(TREE_DocGia root);

// ----------------------------------------------------------------------------
// NHÓM 3: CHỨC NĂNG (a) - THÊM, HIỆU CHỈNH, XÓA THẺ ĐỘC GIẢ
// ----------------------------------------------------------------------------

// [HÀM KIỂM TRA ĐỘC GIẢ CÓ ĐANG MƯỢN SÁCH HAY KHÔNG]
// Mục đích: Ràng buộc an toàn - Độc giả đang giữ sách thì KHÔNG ĐƯỢC XÓA THẺ.
// Giải thuật: Duyệt DSLK dsMuonTra của độc giả, tìm xem có node nào mang
// trangThai == 0. Trả về: true nếu đang mượn (chưa trả), false nếu đã trả hết
// hoặc chưa mượn cuốn nào.
bool KiemTraDocGiaDangMuonSach(NodeDocGia *pDG);

// [HÀM THÊM ĐỘC GIẢ MỚI VÀO CÂY BST]
// Mục đích: Thêm một thẻ độc giả mới vào cây theo đúng quy tắc BST.
bool ThemDocGia(TREE_DocGia &root, TheDocGia dg);

// [HÀM HIỆU CHỈNH THÔNG TIN ĐỘC GIẢ]
// Mục đích: Cập nhật thông tin họ tên, phái hoặc trạng thái khóa/mở của thẻ.
// Lưu ý bảo toàn cấu trúc: maThe là KHÓA của cây BST nên TUYỆT ĐỐI KHÔNG ĐƯỢC
// SỬA maThe.
bool HieuChinhDocGia(TREE_DocGia root, int maThe, const char *ho,
                     const char *ten, const char *phai, int trangThai);

// [HÀM TÌM NODE THẾ MẠNG CHO THUẬT TOÁN XÓA NODE 2 CON TRÊN CÂY BST]
// Mục đích: Khi xóa một node có 2 con, cần tìm một node thay thế sao cho không
// phá vỡ tính chất cây BST. Lựa chọn: Tìm node CỰC TRÁI của cây con BÊN PHẢI
// (phần tử nhỏ nhất bên nhánh phải). Tham số:
//   - X: Node cần xóa (node có 2 con).
//   - Y: Node duyệt đi tìm node thế mạng (bắt đầu từ X->pRight).
void TimNodeTheMang(NodeDocGia *&X, NodeDocGia *&Y);

// [HÀM XÓA THẺ ĐỘC GIẢ KHỎI CÂY BST]
// Mục đích: Xóa một độc giả khỏi cây BST dựa vào maThe.
// Ràng buộc kiểm tra trước: Phải gọi KiemTraDocGiaDangMuonSach trước; nếu đang
// giữ sách thì từ chối. 3 trường hợp xóa node BST:
//   1. Node cần xóa là Node lá (không có con): delete trực tiếp.
//   2. Node cần xóa chỉ có 1 con (trái hoặc phải): nối cha với con của nó rồi
//   delete.
//   3. Node cần xóa có đủ 2 con: gọi TimNodeTheMang để hoán đổi dữ liệu rồi
//   xóa.
bool XoaDocGia(TREE_DocGia &root, int maThe);

// ----------------------------------------------------------------------------
// NHÓM 4: CHỨC NĂNG (b) - IN DANH SÁCH ĐỘC GIẢ (2 CHẾ ĐỘ)
// ----------------------------------------------------------------------------

// [CHẾ ĐỘ 1: IN DANH SÁCH THEO MÃ THẺ TĂNG DẦN]
// Mục đích: Xuất danh sách toàn bộ độc giả có thứ tự maThe tăng dần.
// Giải thuật: Duyệt trung thứ tự (In-order traversal: LNR - Trái -> Gốc ->
// Phải). Vì tính chất BST, duyệt LNR luôn cho thứ tự tăng dần tự nhiên mà không
// cần sắp xếp.
void InDanhSachDocGia_TheoMaThe(TREE_DocGia root);

// [HÀM PHỤ TRỢ: ĐỔ TOÀN BỘ NODE TRÊN CÂY BST VÀO MẢNG CON TRỎ TẠM]
// Mục đích: Lấy toàn bộ địa chỉ node độc giả đưa vào mảng để chuẩn bị sắp xếp
// theo Tên + Họ.
void DoCayVaoMang(TREE_DocGia root, TheDocGia *arr[], int &index);

// [HÀM SO SÁNH TÊN VÀ HỌ CỦA 2 ĐỘC GIẢ]
// Mục đích: Tiêu chuẩn sắp xếp: Tên tăng dần (A-Z). Nếu trùng tên thì xét đến
// Họ tăng dần.
int SoSanhTenHo(const TheDocGia *a, const TheDocGia *b);

// [THUẬT TOÁN QUICKSORT]
// Mục đích: Sắp xếp mảng con trỏ độc giả arr[] từ vị trí left đến right theo
// Tên + Họ tăng dần.
void QuickSortDocGia(TheDocGia *arr[], int left, int right);

// [CHẾ ĐỘ 2: IN DANH SÁCH THEO TÊN + HỌ TĂNG DẦN]
// Mục đích: Xuất danh sách độc giả sắp xếp theo thứ tự bảng chữ cái của Tên,
// trùng Tên thì xét Họ.
void InDanhSachDocGia_TheoTenHo(TREE_DocGia root);

// ----------------------------------------------------------------------------
// NHÓM 5: LƯU TRỮ VÀ PHỤC HỒI DỮ LIỆU TỪ FILE (FILE I/O)
// ----------------------------------------------------------------------------

// [HÀM GHI TOÀN BỘ CÂY ĐỘC GIẢ VÀO FILE]
// Mục đích: Lưu trữ toàn bộ thông tin thẻ độc giả và lịch sử mượn trả khi đóng
// chương trình. Đường dẫn mặc định: "Data/DocGia.txt"
// Giải thuật: Duyệt tiền thứ tự (Pre-order: NLR - Gốc -> Trái -> Phải) để khi
// đọc lại nạp vào cây sẽ giữ nguyên cấu trúc phân nhánh BST ban đầu.
bool GhiFileDocGia(TREE_DocGia root, const char *filePath = "Data/DocGia.txt");

// [HÀM ĐỌC TOÀN BỘ DỮ LIỆU ĐỘC GIẢ TỪ FILE]
// Mục đích: Phục hồi lại cây BST Thẻ Độc Giả và các DSLK Mượn Trả khi khởi động
// chương trình. Đường dẫn mặc định: "Data/DocGia.txt"
bool DocFileDocGia(TREE_DocGia &root, const char *filePath = "Data/DocGia.txt");
