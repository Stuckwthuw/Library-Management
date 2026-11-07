# Đồ án CTDL&GT — Quản lý thư viện C++

- **Lê Văn Hoàng Hiếu — N24DECE018:** BST độc giả, DSLK mượn/trả, quá hạn và lưu độc giả.
- **Lê Anh Đức — N24DECE012:** mảng con trỏ đầu sách, DMS, tìm kiếm, Top 10, Date và Core UI.
- Phân công gốc 50/50: [kế hoạch chi tiết](KE_HOACH_PHAN_CHIA_CONG_VIEC.md).
- Kế hoạch hoàn thiện, quyền thực hiện đã được Hiếu xác nhận và các việc nghiệm thu còn lại:
  [KE_HOACH_HOAN_THIEN.md](KE_HOACH_HOAN_THIEN.md).

## Chức năng hiện có

- a. Thêm, sửa, xóa thẻ độc giả; chặn xóa thẻ còn giữ sách.
- b. Danh sách theo mã thẻ (LNR) hoặc tên/họ (QuickSort), có phân trang.
- c. Thêm đầu sách và bản sao, sinh mã `[ISBN]_[STT]`, giữ thứ tự đầu sách theo tên.
- d. Danh sách theo thể loại/tên, có phân trang và chọn STT xem chi tiết.
- e. Tìm tên sách không phân biệt hoa/thường tiếng Việt; xem đủ thông tin và DMS phân trang.
- f. Mượn sách: thẻ mở, dưới 3 cuốn, không quá hạn, cuốn sách sẵn có.
- g. Trả/báo mất: kiểm tra liên kết và ngày trước khi đổi đồng thời giao dịch và DMS.
- h. Liệt kê sách đang giữ và ngày mượn.
- i. Quá hạn khi số ngày mượn **> 7**, giảm dần số ngày quá hạn, có phân trang.
- j. Top 10 tính mọi lượt đang mượn/đã trả/mất, **kể cả lịch sử của thẻ đã xóa**.
- Tra cứu độc giả theo mã hoặc tên/họ, hỗ trợ tiếng Việt và phân trang kết quả.

Các màn hình thay đổi dữ liệu có xác nhận Y/N; mượn/trả/báo mất có xem trước.
Sau thay đổi, hệ thống lưu đồng bộ hai file; chỉ báo đã lưu khi thao tác lưu thành công.
Nhập số phải khớp trọn dòng: `12abc`, `1.5`, tràn số hoặc sai miền đều bị từ chối.
Chuỗi quá dài được yêu cầu nhập lại, không cắt âm thầm. EOF kết thúc phiên hữu hạn.

## Môi trường và biên dịch

Windows, MinGW g++ hỗ trợ C++11, mã nguồn và dữ liệu UTF-8. `CauTruc.h` giữ nguyên.
Không dùng `vector/list/map/set/queue/stack` hoặc `std::sort`. `unique_ptr<T[]>` chỉ
quản lý cặp `new[]/delete[]`; các cấu trúc và thuật toán được tự cài đặt.

Tại thư mục repository, trong PowerShell:

```powershell
powershell -NoProfile -ExecutionPolicy Bypass -File .\build.ps1
.\QuanLyThuVien.exe
```

Script build bật `-Wall -Wextra -Wpedantic -Werror`: yêu cầu không lỗi/cảnh báo.
Nếu dùng IDE, thêm các `.cpp` trực tiếp trong `DocGia`, `MuonTra`, `DauSach`,
`DanhMucSach`, `Date`, `UI`, **`LuuTru`** và `main.cpp`; không đưa file test vào app.

## Cấu trúc mã nguồn

- `DocGia.h` / `DocGia/`: BST, LCG, chuẩn hóa, thêm/sửa/xóa, sắp xếp,
  đọc/ghi độc giả và kho lịch sử thẻ đã xóa (`DocGia_LuuTru.cpp`).
- `MuonTra.h` / `MuonTra/`: giao dịch, truy vấn đang mượn, lập danh sách quá hạn.
- `DauSach.h` / `DauSach/`: mảng con trỏ, chỉ mục ISBN, thể loại, Top 10, file đầu sách.
- `DanhMucSach.h` / `DanhMucSach/`: DSLK bản sao và trạng thái 0/1/2.
- `Date.h` / `Date/`: ngày hợp lệ, năm nhuận, khoảng cách ngày.
- `UI.h` / `UI/`: console, menu, bảng, nhập liệu, màn hình độc giả/sách/mượn trả.
- `Common/`: chứa các header phụ trợ dùng chung `LuuTru.h`, `Chuoi.h` và `FileUtils.h`.
- `LuuTru/`: kiểm tra liên kết toàn hệ thống và giao dịch lưu hai file.
- `Common/Chuoi.h`: đổi UTF-8/UTF-16, phân loại, đổi hoa/thường và so sánh tiếng Việt trên Windows.
- `Common/FileUtils.h`: mã hóa trường, đọc số nghiêm ngặt và thay file qua bản tạm.
- `main.cpp`: vòng đời dữ liệu và điều hướng qua các master header.

## Dữ liệu và phục hồi

Ứng dụng dùng `Data/DocGia.txt` và `Data/DauSach.txt` tương đối với thư mục chạy.
Cả hai chưa tồn tại là thư viện mới. Chỉ thiếu một file, file sai hoặc liên kết
mượn–sách không nhất quán thì từ chối mở, không ghi đè dữ liệu đang có.

**Độc giả:** đọc được file nhiều dòng cũ (`ma,ho,ten,phai,trangThai`, số lượt,
các dòng giao dịch). Lần lưu thành công tiếp theo tự chuyển sang **DOCGIA_V3**.
V3 là một dòng, các trường phân cách bằng ` | `; ký tự `%` và `|` được mã hóa.
Thứ tự trường:

1. `DOCGIA_V3`, số độc giả.
2. Với mỗi độc giả theo NLR: mã, họ, tên, phái, trạng thái, số lượt mượn.
3. Mỗi lượt là một trường: `maSach dMuon mMuon yMuon dTra mTra yTra trangThai`.
4. Cuối cùng: số lượt trong kho lưu trữ thẻ đã xóa và các lượt cùng cấu trúc.

Ví dụ thư viện không còn thẻ, có một lượt đã trả được giữ lại:

```text
DOCGIA_V3 | 0 | 1 | CS101_1 1 10 2026 5 10 2026 1
```

File độc giả rỗng kiểu cũ biểu diễn cây rỗng. File V3 rỗng hợp lệ là
`DOCGIA_V3 | 0 | 0`. V3 không tương thích ngược với chương trình cũ chỉ đọc CSV;
không dùng chương trình cũ ghi đè sau khi đã chuyển đổi. Không hỗ trợ định dạng
`DOCGIA_V2` từng được mô tả nhầm trong README cũ.

**Đầu sách:** giữ định dạng `DAUSACH_V1` một dòng như trước:

```text
DAUSACH_V1 | 1 | CS101 | Cấu trúc dữ liệu | 300 | Nguyễn Văn An | 2025 | Tin học | 1 | CS101_1 | 0 | Kệ A1
```

**Lưu đồng bộ:** `LuuDuLieu` kiểm tra liên kết, tạo hai ảnh `.prepare`, ghi nhật ký
`Data/DocGia.txt.pending` qua `.tmp`, flush rồi mới thay lần lượt hai file đích.
Nhật ký chứa cả hai ảnh và checksum phát hiện hỏng dữ liệu ngoài ý muốn. Nếu bị
ngắt giữa chừng, lần mở sau hoàn tất cùng bản dữ liệu mới rồi xóa nhật ký.
Nhật ký hỏng/không thể phục hồi sẽ làm ứng dụng dừng để bảo vệ dữ liệu.

Nếu báo lỗi lưu, kiểm tra dung lượng/quyền ghi và đóng chương trình khác đang khóa file,
sau đó chọn thử lại. Nếu dừng phiên, **giữ nguyên `.pending` nếu có** để phục hồi.
Thất bại trước khi công bố nhật ký giữ cặp file cũ; thay đổi trên RAM chưa được
xác nhận lưu có thể mất nếu chọn dừng. Không xóa thủ công `.pending` hoặc sửa hai
file trong khi chương trình đang chạy. Thiết kế dành cho **một tiến trình** quản lý
thư viện; checksum không phải chữ ký chống sửa file có chủ đích, và phục hồi không
thay thế sao lưu bên ngoài khi thiết bị lưu trữ hỏng.

## Giao ước API và thuật toán cần nắm khi bảo vệ

- `themDauSach` nhận sở hữu con trỏ/DMS chỉ khi trả `true`; thất bại bên gọi thu hồi.
- `ThemDocGia` nhận sở hữu DSLK mượn của dữ liệu truyền vào chỉ khi thành công.
- `XoaDocGia` chuyển lịch sử hoàn tất sang kho riêng trước khi thu hồi thẻ.
  Cuối phiên phải gọi cả `GiaiPhongCayDocGia` và `GiaiPhongLichSuLuuTru`.
- `layTop10`/`thongKeLuotMuon` nhận tham số cuối tùy chọn là kho lịch sử.
  App truyền `LayLichSuLuuTru()`; bỏ tham số này thì chỉ thống kê cây được truyền vào.
- `DocFileDocGia` chỉ thay cây và kho lịch sử sau khi toàn bộ file hợp lệ.
  App dùng `NapDuLieu` để kiểm tra thêm liên kết với DMS; không coi đọc từng file
  thành công riêng lẻ là đủ để vận hành giao dịch.
- BST tìm/chèn/xóa O(H), tệ nhất O(N). LCG chu kỳ 90.000 giúp phân tán mã nhưng
  **không bảo đảm BST cân bằng**. Sinh một mã là O(T·H), tối đa 90.000 ứng viên.
- QuickSort tự viết trung bình O(N log N), tệ nhất O(N²); median-of-three chỉ giảm
  nguy cơ phân hoạch lệch. Không dùng lập luận “loại bỏ hoàn toàn O(N²)”.
- Mảng đầu sách luôn tăng theo tên; chỉ mục ISBN phụ xây O(N log N), truy vấn tiếp
  theo O(log N). Tìm cuốn trong DMS O(M), tìm chuỗi con tên sách duyệt O(N).
- Kiểm tra liên kết dùng mảng phụ tự sắp xếp: O(C log C + L log C), bộ nhớ O(C),
  với C bản sách và L tổng lượt lịch sử. Top 10 đếm cả lịch sử lưu trữ.
- Chuỗi giới hạn theo **byte**, không phải số chữ. Bảng căn cột theo ký tự tiếng Việt;
  dấu `~` báo nội dung bị rút gọn, có thể xem chi tiết đầu sách bằng STT.

## Kiểm thử tự động

Cần g++ trong PATH; bộ test menu dùng Python 3 (thư viện chuẩn).

```powershell
powershell -NoProfile -ExecutionPolicy Bypass -File .\tests\chay_kiem_thu.ps1
```

Script build app và chạy:

- `DauSach/tests/kiem_thu_core.cpp`: đầu sách, DMS, tìm kiếm, thể loại, Top 10, Date, QuickSort.
- `DauSach/tests/kiem_thu_file.cpp`: file đầu sách, UTF-8, ký tự `|/%`, file lỗi/rỗng.
- `tests/kiem_thu_tich_hop.cpp`: BST, Unicode, nhập số, mượn/trả/mất, biên 7/8 ngày,
  bảo toàn lịch sử, đọc file cũ/V3, file hỏng, giữ RAM khi nạp lỗi, lỗi ghi và phục hồi.
- `tests/kiem_thu_menu.py`: chạy executable qua stdin, có timeout; thêm/tìm/hủy,
  EOF, khởi động mới, file lỗi, mượn–trả–mất–xóa, Top 10 và bảng 25 dòng.

Mỗi lần chạy tạo thư mục mới trong `%TEMP%`; không dùng hoặc ghi dữ liệu thật của repo.
Ca phục hồi khóa file đầu sách bằng Windows API để ép lỗi sau khi file độc giả đã
được thay, rồi mở khóa và xác nhận nạp lại cả hai file nhất quán.

Kiểm tra trực tiếp còn cần: mũi tên/Enter/Esc, Home/End, khung/cột với font console
của máy và công cụ kiểm tra bộ nhớ. Test stdin không chứng minh các thao tác console
thật hoặc mọi đường rò rỉ bộ nhớ đều đạt.
