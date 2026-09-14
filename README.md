# ĐỒ ÁN CẤU TRÚC DỮ LIỆU & GIẢI THUẬT
## ĐỀ TÀI: QUẢN LÝ THƯ VIỆN (C++)

---

## 👥 THÀNH VIÊN NHÓM & PHÂN CHIA CÔNG VIỆC

| STT | Họ và Tên | MSSV | Vai trò | Phân hệ phụ trách | Tỷ lệ đóng góp |
| :---: | :--- | :---: | :--- | :--- | :---: |
| 1 | **Lê Văn Hoàng Hiếu** | **N24DECE018** | Trưởng nhóm | **Phân hệ Thẻ Độc Giả & Giao Dịch Mượn Trả**<br>• Cài đặt Cây nhị phân tìm kiếm BST (Thẻ độc giả)<br>• Cài đặt DSLK đơn Mượn Trả<br>• Nghiệp vụ mượn sách, trả sách, phạt quá hạn 7 ngày<br>• In danh sách độc giả (LNR / sắp xếp theo tên họ)<br>• Xử lý đọc/ghi file `DocGia.txt` | **50%** |
| 2 | **Lê Anh Đức** | **N24DECE012** | Thành viên | **Phân hệ Đầu Sách, Danh Mục Sách & Core UI**<br>• Cài đặt Mảng con trỏ Đầu sách (luôn tăng dần theo tên)<br>• Cài đặt DSLK đơn Danh mục sách (DMS)<br>• Đánh mã sách tự động `[ISBN]_[STT]`, tìm kiếm sách theo tên<br>• Lọc sách theo thể loại, thống kê Top 10 sách mượn nhiều nhất<br>• Xây dựng Core UI Console & xử lý ngày tháng (`Date`)<br>• Xử lý đọc/ghi file `DauSach.txt` | **50%** |

👉 **Chi tiết kế hoạch triển khai, phân tích giải thuật và lộ trình 4 tuần xem tại:**  
📄 [KE_HOACH_PHAN_CHIA_CONG_VIEC.md](./KE_HOACH_PHAN_CHIA_CONG_VIEC.md)

---

## 🛠️ TỔ CHỨC 4 CẤU TRÚC DỮ LIỆU CỐT LÕI

1. **Đầu sách**: Danh sách tuyến tính là 1 **Mảng con trỏ** (`DauSach* nodes[MAX]`). Mảng luôn được duy trì thứ tự tăng dần theo tên sách để tối ưu hóa tìm kiếm.
2. **Danh mục sách (DMS)**: **Danh sách liên kết đơn** gắn với từng đầu sách. Mỗi cuốn sách có một mã duy nhất định danh theo quy tắc `[ISBN]_[SốThứTự]` phục vụ tìm kiếm nhị phân nhanh chóng. Trạng thái: `0` (cho mượn được), `1` (đã có độc giả mượn), `2` (đã thanh lý).
3. **Danh sách thẻ độc giả**: **Cây nhị phân tìm kiếm (BST)** với khóa chính là `MATHE` (số nguyên tự động cung cấp không trùng lặp). Trạng thái: `0` (bị khóa), `1` (hoạt động).
4. **Danh sách MUONTRA**: **Danh sách liên kết đơn** gắn với từng độc giả lưu lịch sử và các cuốn sách đang mượn. Trạng thái: `0` (đang mượn), `1` (đã trả), `2` (làm mất sách).

---

## 📋 DANH SÁCH CHỨC NĂNG CHƯƠNG TRÌNH

- [x] **a. Quản lý thẻ độc giả**: Thêm mới (sinh mã tự động), Xóa (thuật toán xóa node cây BST), Hiệu chỉnh thông tin độc giả.
- [x] **b. In danh sách độc giả**: In theo thứ tự Tên + Họ tăng dần hoặc theo Mã độc giả tăng dần (duyệt cây LNR).
- [x] **c. Nhập đầu sách & Đánh mã tự động**: Chèn giữ thứ tự theo tên sách, tự động sinh mã sách con trong danh mục sách.
- [x] **d. In danh sách đầu sách theo thể loại**: Gom nhóm theo từng thể loại, tên sách tăng dần.
- [x] **e. Tìm thông tin sách**: Tra cứu theo tên sách, in đầy đủ thông tin xuất bản và trạng thái từng cuốn sách con.
- [x] **f. Mượn sách**: Kiểm tra thẻ mở, tối đa 3 cuốn, không giữ sách quá hạn (> 7 ngày), sách có sẵn để mượn.
- [x] **g. Trả sách**: Cập nhật ngày trả, đổi trạng thái mượn trả và trạng thái sách trong danh mục.
- [x] **h. Liệt kê sách đang mượn**: In danh sách mã sách, tên sách độc giả X đang mượn.
- [x] **i. In độc giả mượn sách quá hạn**: Sắp xếp giảm dần theo số ngày quá hạn.
- [x] **j. Thống kê Top 10 sách**: Xuất 10 đầu sách có số lượt mượn nhiều nhất.

---

## 📁 CẤU TRÚC THƯ MỤC NGUỒN (SOURCE CODE)

```
QuanLyThuVien/
│
├── README.md                      # Giới thiệu đồ án & phân chia công việc
├── KE_HOACH_PHAN_CHIA_CONG_VIEC.md# Tài liệu chi tiết kế hoạch & phân công WBS
├── CauTruc.h                      # Khai báo struct và các hằng số toàn cục
├── Date.h / Date.cpp              # Xử lý ngày tháng, kiểm tra hợp lệ, tính số ngày quá hạn
├── UI.h / UI.cpp                  # Giao diện Console, màu sắc, bắt phím mũi tên, phân trang
├── DauSach.h / DauSach.cpp        # Quản lý Mảng con trỏ Đầu sách & chức năng c, d, e, j
├── DanhMucSach.h / .cpp           # Quản lý DSLK Danh mục sách
├── DocGia.h / DocGia.cpp          # Quản lý Cây BST Thẻ độc giả & chức năng a, b
├── MuonTra.h / MuonTra.cpp        # Quản lý DSLK Mượn Trả & nghiệp vụ f, g, h, i
├── Data/                          # Thư mục lưu trữ file dữ liệu (DocGia.txt, DauSach.txt)
└── main.cpp                       # Điểm khởi chạy, menu điều hướng và giải phóng bộ nhớ
```

---

## 💻 HƯỚNG DẪN BIÊN DỊCH VÀ CHẠY

### 1. Yêu cầu hệ thống
- Hệ điều hành: Windows (tương thích các hàm đồ họa Console `windows.h`, `conio.h`).
- Trình biên dịch: GCC / MinGW (C++11 trở lên), Dev-C++ 5.11, hoặc Visual Studio 2019/2022.

### 2. Biên dịch bằng dòng lệnh (g++ MinGW)
```bash
# Biên dịch tất cả các file nguồn
g++ -std=c++11 main.cpp DauSach.cpp DanhMucSach.cpp DocGia.cpp MuonTra.cpp UI.cpp Date.cpp -o QuanLyThuVien.exe

# Chạy chương trình
./QuanLyThuVien.exe
```

### 3. Mở bằng Dev-C++
1. Chọn `File` -> `New` -> `Project...` -> Chọn `Console Application` (C++ Project).
2. Thêm tất cả các file `.h` và `.cpp` vào Project.
3. Nhấn `F11` (Compile & Run).
