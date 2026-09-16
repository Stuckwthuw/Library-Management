---
name: library-management
description: >-
  Specialized skill for Lê Văn Hoàng Hiếu on the C++ Library Management project.
  Focuses strictly on BST Reader Management, Linked List Borrow/Return transactions,
  overdue calculation, algorithm evaluation/optimization, and academic DSA compliance without encroaching on Đức's modules.
---

# Skill: Quản Lý Thư Viện (Phân hệ Lê Văn Hoàng Hiếu)

Skill chuyên biệt hỗ trợ **Lê Văn Hoàng Hiếu** (MSSV: N24DECE018 - Trưởng nhóm) phát triển và bảo trì phân hệ **Thẻ Độc Giả & Giao Dịch Mượn Trả**.

---

## 1. RANH GIỚI TRÁCH NHIỆM & BẢO VỆ MODULE

- **Thành viên đang tương tác**: **Lê Văn Hoàng Hiếu**
- **Files được phép chỉnh sửa trực tiếp**:
  - `DocGia.h`, `DocGia.cpp`: Cây BST Độc giả, Chức năng a, b, nạp/lưu file `DocGia.txt`.
  - `MuonTra.h`, `MuonTra.cpp`: DSLK Đơn Mượn Trả, Chức năng f, g, h, i.
  - `Data/DocGia.txt`: File dữ liệu phân hệ của Hiếu.
  - `main.cpp`: Kết nối điều hướng menu cho phân hệ của Hiếu.
- **Files KHÔNG ĐƯỢC TỰ Ý SỬA (Phân hệ của Lê Anh Đức)**:
  - `DauSach.h / .cpp`, `DanhMucSach.h / .cpp`, `UI.h / .cpp`, `Date.h / .cpp`, `Data/DauSach.txt`.
  - *Nguyên tắc phối hợp*: Khi cần tra cứu thông tin sách hoặc cập nhật trạng thái sách con, chỉ gọi các hàm giao tiếp (API prototypes) do Đức cung cấp. **Tuyệt đối không tự sửa code bên file của Đức**.
- **Giao ước chung bất biến (`CauTruc.h`)**:
  - Tuyệt đối không tự ý thêm, sửa, xóa các trường trong các `struct` nếu chưa có sự đồng thuận của cả Hiếu và Đức.
- **Quy định Git**:
  - Nhánh làm việc của Hiếu: `feature/doc-gia` hoặc `hieu/<ten-chuc-nang>`.
  - Nghiêm cấm commit/push trực tiếp lên nhánh `main`.

---

## 2. CẤU TRÚC DỮ LIỆU HIẾU QUẢN LÝ (`CauTruc.h`)

```cpp
// Ràng buộc hằng số toàn cục
const int MAX_MUON_SACH = 3;  // Mỗi độc giả mượn tối đa 3 cuốn
const int MAX_NGAY_MUON = 7;  // Mượn quá 7 ngày bị tính là quá hạn

// 1. DSLK đơn Mượn Trả (Gắn với từng độc giả)
struct MuonTra {
    char maSach[20];       // Mã sách định dạng [ISBN]_[STT]
    Date ngayMuon;         // Ngày mượn sách
    Date ngayTra;          // Ngày trả sách (rỗng nếu chưa trả)
    int trangThai;         // 0: Đang mượn, 1: Đã trả, 2: Làm mất sách
};

struct NodeMuonTra {
    MuonTra data;
    NodeMuonTra* pNext;
};

// 2. Thẻ Độc Giả (Cây Nhị Phân Tìm Kiếm - BST)
struct TheDocGia {
    int maThe;             // Số nguyên sinh tự động không trùng lặp (Khóa chính BST)
    char ho[50];
    char ten[20];
    char phai[5];          // "Nam" hoặc "Nu"
    int trangThai;         // 0: Bị khóa, 1: Đang hoạt động
    NodeMuonTra* dsMuonTra;// Con trỏ trỏ đến DSLK Mượn Trả
};

struct NodeDocGia {
    TheDocGia data;
    NodeDocGia* pLeft;
    NodeDocGia* pRight;
};
typedef NodeDocGia* TREE_DocGia;
```

---

## 3. CÁC NGHIỆP VỤ & THUẬT TOÁN HIẾU PHỤ TRÁCH

### (a) Quản lý Thẻ Độc Giả:
- **Thêm độc giả**:
  - Sinh `maThe` tự động: Số nguyên ngẫu nhiên hoặc tăng dần không trùng lặp với các mã đã có trên cây BST.
  - Chuẩn hóa: `phai` chỉ nhận "Nam" hoặc "Nu", `trangThai` mặc định = 1 (hoạt động).
  - Chèn node vào Cây BST theo khóa `maThe` (nhỏ hơn sang trái, lớn hơn sang phải).
- **Hiệu chỉnh thông tin**:
  - Tìm kiếm node trên cây theo `maThe` với độ phức tạp $O(\log N)$.
  - Cho phép sửa: Họ, Tên, Phái, Trạng thái (khóa/mở).
- **Xóa thẻ độc giả**:
  - **Bẫy lỗi bắt buộc**: Kiểm tra `dsMuonTra` của độc giả. Nếu còn sách có `trangThai == 0` (đang mượn) $\rightarrow$ **Báo lỗi và không cho xóa**.
  - Thuật toán xóa node BST:
    + Node lá (0 con): Giải phóng `delete` và gán lại `nullptr`.
    + Node 1 con: Cho cha trỏ trực tiếp đến con duy nhất của node cần xóa.
    + Node 2 con: Tìm **node thế mạng** (node cực trái của cây con bên phải) để sao chép dữ liệu và xóa node thế mạng.

### (b) In Danh Sách Độc Giả:
- **Chế độ 1 (Theo Mã Thẻ tăng dần)**:
  - Duyệt cây BST theo thứ tự **LNR (In-order traversal)**. Nhờ tính chất cây BST, kết quả in ra sẽ tự động có thứ tự `maThe` tăng dần.
- **Chế độ 2 (Theo Tên + Họ tăng dần)**:
  - Đếm tổng số lượng độc giả trên cây BST ($N$).
  - Cấp phát mảng con trỏ phụ: `TheDocGia* dsTam[N]`.
  - Duyệt cây (LNR hoặc NLR) để gán con trỏ độc giả vào mảng phụ mà không sao chép lại toàn bộ dữ liệu.
  - Tự cài đặt thuật toán **QuickSort** sắp xếp mảng phụ: So sánh `ten` trước; nếu trùng tên thì so sánh `ho`.

### (f) Nghiệp vụ Mượn Sách:
- **Kiểm tra 3 điều kiện mượn**:
  1. Thẻ độc giả có đang hoạt động không (`trangThai == 1`)?
  2. Tổng số sách đang mượn (`trangThai == 0`) có $< 3$ cuốn không?
  3. Có cuốn sách nào đang mượn quá hạn 7 ngày không?
- **Nếu đủ điều kiện**:
  - Nhập mã sách, kiểm tra sách bên Danh mục sách (DMS) của Đức có `trangThai == 0` (cho mượn được) không.
  - Tạo `NodeMuonTra` mới thêm vào DSLK của độc giả (`ngayMuon = ngayHienTai`, `trangThai = 0`).
  - Gọi hàm cập nhật trạng thái sách bên DMS thành `1` (đã mượn).

### (g) Nghiệp vụ Trả Sách / Báo Mất Sách:
- Nhập `maThe` và `maSach`.
- Cập nhật `ngayTra = ngayHienTai`.
- **Trả sách**:
  - Đổi `MuonTra.trangThai = 1`.
  - Gọi hàm cập nhật sách bên DMS về `0` (cho mượn được).
- **Mất sách**:
  - Đổi `MuonTra.trangThai = 2`.
  - Gọi hàm cập nhật sách bên DMS thành `2` (thanh lý).

### (h) Liệt Kê Sách Đang Mượn Của Độc Giả:
- Nhập `maThe` $\rightarrow$ duyệt DSLK `dsMuonTra` lọc các node có `trangThai == 0`.
- Gọi hàm tra cứu từ mã sách để lấy `tenSach` từ module của Đức in ra màn hình.

### (i) Danh Sách Độc Giả Mượn Quá Hạn:
- Duyệt toàn bộ Cây BST độc giả.
- Với mỗi độc giả, duyệt `dsMuonTra` tìm các sách có `trangThai == 0` và `(NgayHienTai - NgayMuon) > 7` ngày.
- Nạp vào danh sách quá hạn tạm (gồm thông tin độc giả, mã sách, số ngày quá hạn).
- Tự viết thuật toán sắp xếp giảm dần theo số ngày quá hạn và hiển thị.

### (File I/O) Lưu trữ `Data/DocGia.txt`:
- **Nạp file khi mở chương trình**: Đọc tuần tự dữ liệu thẻ độc giả và tái dựng cây BST kèm danh sách liên kết mượn trả của từng người.
- **Ghi file khi kết thúc**: Duyệt cây BST (theo thứ tự NLR để khi nạp lại giữ được cấu trúc cây ban đầu) và ghi đầy đủ danh sách mượn trả ra file.

---

## 4. QUY CHUẨN LẬP TRÌNH & BẢO VỆ ĐỒ ÁN (DSA CONSTRAINTS)

1. **Tuyệt đối không dùng STL**:
   - 🚫 Cấm: `std::vector`, `std::list`, `std::queue`, `std::stack`, `std::sort`.
   - Bắt buộc: Tự cấp phát động bằng con trỏ, struct, tự giải phóng bộ nhớ (`delete`), chống dangling pointer (`ptr = nullptr`).
2. **Comment giải thích phục vụ Thầy/Cô hỏi thi (Why, not just What)**:
   - Các giải thuật cốt lõi (Xóa node BST 2 con, Duyệt LNR, Đổi mảng sắp xếp QuickSort, Kiểm tra quá hạn) bắt buộc chú thích rõ:
     + **Mục đích giải thuật**.
     + **Tại sao chọn cách làm này** (ưu điểm thời gian $O$, bộ nhớ).
     + **Trường hợp biên đã bẫy lỗi** (đang mượn không cho xóa thẻ, cây rỗng, danh sách rỗng).

3. **BÁO CÁO 5 NỘI DUNG BẮT BUỘC SAU MỖI LẦN CODE / FIX BUG**:
   - **1. Danh sách thay đổi (What Changed)**: File nào, hàm nào, dòng nào được thêm/sửa/xóa.
   - **2. Đánh giá & Đề xuất cải tiến thuật toán (Algorithm Review & Optimization Proposal)**:
     + *Đánh giá thuật toán hiện tại*: Phân tích độ phức tạp thời gian ($O$), không gian bộ nhớ, ưu/nhược điểm thực tế.
     + *Có đề xuất cải tiến hay không?*
       - **Nếu CÓ**: Nêu rõ phương án tối ưu (ví dụ: khử đệ quy, tối ưu số lần duyệt cây, đổi thuật toán sắp xếp mảng con trỏ, tối ưu đếm node) và so sánh hiệu năng trước/sau.
       - **Nếu KHÔNG**: Giải thích rõ tại sao thuật toán hiện tại đã đạt tối ưu tốt nhất trong khuôn khổ đề tài và không cần thay đổi.
   - **3. Phân tích nguyên nhân gốc rễ nếu sửa lỗi (Root Cause Bug Analysis)**: Lỗi gì, tại sao sai, giải pháp khắc phục.
   - **4. Cảnh báo tác động (Side-effect Warning)**: Có ảnh hưởng đến các hàm khác trong hệ thống hoặc module của Đức hay không.
   - **5. Hướng dẫn kiểm thử (Verification Guide)**: Kịch bản test chi tiết (Input nhập vào và Output mong đợi).
