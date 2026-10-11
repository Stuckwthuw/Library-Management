# ĐỒ ÁN CẤU TRÚC DỮ LIỆU & GIẢI THUẬT
## ĐỀ TÀI: QUẢN LÝ THƯ VIỆN (C++)


## Bản lõi phần Đức (PR đầu tiên)

Các API trong `DauSach.h`, `DanhMucSach.h`, `Date.h` triển khai phần cốt lõi Đ1–Đ6: mảng con trỏ tăng theo tên, DSLK danh mục, mã `ISBN_STT`, tìm kiếm ISBN/tên, gom thể loại, Top 10 và ngày tháng. Đây là thư viện hàm để ghép với UI và module của Hiếu; các mục d/e/j trả dữ liệu cho lớp hiển thị. Giao diện, nhập liệu console, đọc/ghi file (Đ7–Đ8) và chương trình hoàn chỉnh sẽ ở PR sau.

`CauTruc.h` giữ nguyên. Không dùng container STL hoặc `std::sort`. QuickSort tự cài trong `DauSach/DauSach_NoiBo.h`; mảng tạm cấp phát bằng `new[]`, được `unique_ptr<T[]>` thu hồi tự động bằng `delete[]`.

### Kiểm thử độc lập bản lõi (Windows / MinGW, C++11)

Trong PowerShell tại thư mục repository, với `g++` trong PATH:

```powershell
$modules = Get-ChildItem DauSach,DanhMucSach,Date -Filter '*.cpp' | ForEach-Object { $_.FullName }
g++ -std=c++11 -Wall -Wextra -Wpedantic -Werror -finput-charset=UTF-8 -fexec-charset=UTF-8 DauSach/tests/kiem_thu_core.cpp $modules -o kiem_thu_core.exe
.\kiem_thu_core.exe
```

Kết quả mong đợi: `PASS: DauSach, DMS, tim kiem, the loai, Top10, Date, QuickSort.`

Các ca kiểm thử gồm: danh sách rỗng/đầy, ISBN trùng, tên và ISBN có thứ tự khác nhau, bộ đệm kết quả thiếu, tìm tiếng Việt trên Windows, mã sách vượt độ dài, dãy STT có khoảng trống, trạng thái sai, Top 10 ít/nhiều hơn 10 đầu sách, lượt đang mượn/đã trả/mất, mốc 7/8 ngày, năm nhuận và biên tháng/năm 1901–2101.

### Giao tiếp để ghép module

- `themDauSach`: nhận quyền sở hữu con trỏ và DMS khi thành công; nếu trả `false`, bên gọi vẫn phải quản lý/giải phóng chúng. `giaiPhongDauSach` thu hồi cả đầu sách và các cuốn con.
- `themBanSao`: sinh mã nối tiếp STT lớn nhất, kiểm tra vừa `maSach[20]`, tạo chuỗi tạm trước khi nối để không để lại dữ liệu dở dang khi lỗi. Giữ cuốn thanh lý trong DMS để không tái sử dụng mã.
- `timSachTheoMaSach`: trả đầu sách và node DMS cho Hiếu kiểm tra nghiệp vụ; `capNhatTrangThaiSach` chỉ nhận 0/1/2.
- `timDauSachTheoTen`, `layDauSachTheoTheLoai`: bên gọi cấp mảng ít nhất `ds.n` con trỏ; trả số kết quả hoặc -1 nếu bộ đệm không hợp lệ. Con trỏ kết quả thuộc danh sách gốc. Từ đầu sách có thể lấy đầy đủ ISBN, tên, tác giả, năm, thể loại và duyệt DMS để in mục e.
- `layTop10`: nhận cây độc giả chỉ đọc; cấp bộ đệm 10 phần tử `ThongKeSach`, nhận tối đa 10 kết quả giảm dần theo số lượt. Hòa lượt thì so tên rồi ISBN. Mọi trạng thái mượn trả đều được đếm; lịch sử phải còn được giữ để thống kê đầy đủ.
- `khoangCachNgay(tu, den)`: trả số ngày `den - tu`; quá hạn khi lớn hơn `MAX_NGAY_MUON`. `{0,0,0}` chỉ dùng biểu diễn ngày trả chưa có, không tính khoảng cách. Miền ngày hợp lệ 1900–9999.
- Chuỗi lưu UTF-8, giới hạn mảng `char` tính bằng byte; tìm tên không phân biệt hoa/thường, có phân biệt dấu. ISBN tối đa 14 byte, không chứa khoảng trắng hoặc dấu `_` dành cho phân tách mã.

Mảng chính luôn tăng theo tên. Chỉ mục ISBN phụ xây lại O(N log N) sau thay đổi, các truy vấn tiếp theo O(log N); tìm cuốn trong DMS O(M). Chỉ mục cache một danh sách tại một thời điểm, dùng trong luồng console đơn. Các API thêm/khởi tạo/giải phóng tự vô hiệu cache; nếu bên gọi sửa ISBN/con trỏ trực tiếp, phải gọi `voHieuChiMucISBN()`.

Các danh sách đầu vào cần được khởi tạo và có liên kết hợp lệ (không chu trình); module lõi không quản lý giao dịch mượn/trả. Lỗi đối số của hàm sinh mã/ngày và lỗi cấp phát có thể ném ngoại lệ, bên giao diện xử lý thông báo. Bộ test tạo cây và lịch sử bằng struct để xác nhận đọc dữ liệu, không triển khai module của Hiếu.

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
├── AGENTS.md                      # Hướng dẫn và quy chuẩn nghiêm ngặt cho AI Agents
├── CauTruc.h                      # [Frozen Contract] Khai báo struct và các hằng số toàn cục
│
├── ── PHÂN HỆ HIẾU (ĐỘC GIẢ & MƯỢN TRẢ) ──────────────────────────────────
├── DocGia.h                       # Khai báo nguyên mẫu hàm Thẻ độc giả
├── DocGia/                        # Thư mục module hóa các chức năng độc giả
│   ├── DocGia_Core.cpp            # Quản lý bộ nhớ & khởi tạo cây BST
│   ├── DocGia_MaThe.cpp           # Tìm kiếm mã thẻ & sinh mã tự động LCG
│   ├── DocGia_Them.cpp            # Nghiệp vụ thêm độc giả vào cây BST
│   ├── DocGia_HieuChinh.cpp       # Hiệu chỉnh thông tin độc giả
│   ├── DocGia_Xoa.cpp             # Xóa node BST (lá, 1 con, 2 con thế mạng)
│   ├── DocGia_In.cpp              # In LNR tăng dần & QuickSort theo tên họ
│   └── DocGia_File.cpp            # Đọc / Ghi file DocGia.txt (NLR)
├── MuonTra.h                      # Khai báo nguyên mẫu hàm Mượn Trả
├── MuonTra/                       # Thư mục module hóa giao dịch mượn trả
│   ├── MuonTra_Core.cpp           # Khởi tạo, giải phóng DSLK mượn trả
│   ├── MuonTra_Muon.cpp           # Nghiệp vụ mượn sách (bẫy 3 điều kiện)
│   ├── MuonTra_Tra.cpp            # Nghiệp vụ trả sách / báo mất sách
│   ├── MuonTra_LietKe.cpp         # Liệt kê sách đang mượn của độc giả
│   └── MuonTra_QuaHan.cpp         # Lọc danh sách độc giả quá hạn 7 ngày
│
├── ── PHÂN HỆ ĐỨC (ĐẦU SÁCH, DMS, UI & DATE) ─────────────────────────────
├── DauSach.h                      # Khai báo nguyên mẫu hàm Đầu sách
├── DauSach/                       # Thư mục module hóa mảng con trỏ đầu sách
│   ├── DauSach_Core.cpp           # Khởi tạo, giải phóng mảng con trỏ
│   ├── DauSach_Them.cpp           # Chèn đầu sách giữ thứ tự tăng dần theo tên
│   ├── DauSach_TimKiem.cpp        # Tìm kiếm sách theo tên, tìm nhị phân theo ISBN
│   ├── DauSach_TheLoai.cpp        # Lọc danh sách đầu sách theo thể loại
│   ├── DauSach_Top10.cpp          # Thống kê Top 10 sách mượn nhiều nhất
│   └── DauSach_File.cpp           # Đọc / Ghi file DauSach.txt
├── DanhMucSach.h                  # Khai báo nguyên mẫu hàm Danh mục sách
├── DanhMucSach/                   # Thư mục module hóa DSLK danh mục sách con
│   ├── DanhMucSach_Core.cpp       # Khởi tạo, giải phóng DSLK sách con
│   ├── DanhMucSach_MaSach.cpp     # Đánh mã sách tự động [ISBN]_[STT]
│   ├── DanhMucSach_Them.cpp       # Thêm sách con vào danh mục
│   └── DanhMucSach_TrangThai.cpp  # Cập nhật trạng thái sách (0, 1, 2)
├── Date.h                         # Khai báo hàm xử lý ngày tháng
├── Date/                          # Thư mục xử lý ngày tháng
│   ├── Date_Core.cpp              # Lấy ngày hệ thống, kiểm tra ngày hợp lệ (năm nhuận)
│   └── Date_KhoangCach.cpp        # Tính khoảng cách số ngày giữa 2 mốc Date
├── UI.h                           # Khai báo đồ họa console & bảng phân trang
├── UI/                            # Thư mục giao diện console
│   ├── UI_Console.cpp             # Xóa màn hình, gotoxy, màu sắc, vẽ box
│   ├── UI_Menu.cpp                # Menu điều hướng bằng phím mũi tên
│   └── UI_Table.cpp               # Bảng hiển thị phân trang chống tràn màn hình
│
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
# Biên dịch tất cả các file nguồn (theo kiến trúc thư mục module hóa)
g++ -std=c++11 main.cpp DauSach/*.cpp DanhMucSach/*.cpp DocGia/*.cpp MuonTra/*.cpp UI/*.cpp Date/*.cpp -o QuanLyThuVien.exe

# Chạy chương trình
./QuanLyThuVien.exe
```

### 3. Mở bằng Dev-C++
1. Chọn `File` -> `New` -> `Project...` -> Chọn `Console Application` (C++ Project).
2. Thêm tất cả các file `.h` và `.cpp` vào Project.
3. Nhấn `F11` (Compile & Run).
