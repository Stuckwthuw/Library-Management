# Quản lý thư viện — C++ và web localhost

Ứng dụng Windows chạy offline với HTML/CSS/JavaScript thuần và máy chủ C++11 Winsock.
Hiếu phụ trách web, HTTP/API, tích hợp, BST độc giả và DSLK mượn trả. Đức phụ trách Đầu sách, DMS, Date, tìm kiếm và Top 10.
Xem [phân công mới](KE_HOACH_PHAN_CHIA_CONG_VIEC.md).

## Khởi động

Cần MinGW g++ hỗ trợ C++11 và PowerShell. Không cần Node.js, npm hoặc Internet.
Chạy từ thư mục dự án:

```powershell
powershell -NoProfile -ExecutionPolicy Bypass -File .\build.ps1
.\QuanLyThuVien.exe
```

Mở **http://127.0.0.1:8080**. Đổi cổng: `.\QuanLyThuVien.exe 8081`.
Dùng nút **Dừng ứng dụng** trên web. Đóng tab không dừng server.
Terminal chỉ ghi log kỹ thuật. Mã thoát 3: cổng bận; mã 4: một phiên thư viện đang chạy.
Một tiến trình sở hữu dữ liệu; server bind loopback và xử lý yêu cầu tuần tự.

## Chức năng

- Độc giả: thêm/sửa/xóa, tìm mã/họ tên, sắp xếp mã hoặc tên/họ, xem sách đang giữ.
- Kho sách: thêm đầu sách/bản sao, tìm tên/ISBN, danh sách theo thể loại, xem kệ và trạng thái bản sao.
- Mượn/trả/báo mất: xem trước họ tên, số sách đang giữ, tên sách, kệ; xác nhận trước khi ghi.
- Quá hạn trên 7 ngày và Top 10 gồm lịch sử thẻ đã xóa.
- Bảng phân trang 20 dòng; API cho phép tối đa 100 dòng/trang.

Frontend chỉ trình bày. Nghiệp vụ, BST, DSLK, mảng con trỏ và sắp xếp tự viết chạy ở C++.
Không dùng STL container hoặc std::sort, kể cả lớp HTTP. CauTruc.h giữ nguyên.
Tên độc giả chỉ một từ; phần còn lại thuộc họ và tên đệm. Chuỗi giới hạn theo byte UTF-8.
Một độc giả không giữ đồng thời hai cuốn cùng ISBN, tối đa 3 cuốn và không được mượn khi quá hạn.

## Kiến trúc và API

Trình duyệt → HTTP/API → nghiệp vụ C++ → lưu đồng bộ hai file.
Web/ chứa tài nguyên tĩnh; Server/ chứa HTTP, codec và API; Server.h là master header.
main.cpp quản lý vòng đời. Không còn UI console hoặc nhập liệu bằng bàn phím terminal.

GET /api/v1/: status, readers, books, copies?isbn=, loans?id=,
preview?id=&code=, overdue, top. Danh sách nhận page, size, q;
readers nhận sort=name; books nhận genre.
POST dùng application/x-www-form-urlencoded: readers/add, readers/edit, readers/delete,
books/add, copies/add, borrow, return, lost, save, migrate, stop.
Mọi POST cần token và version lấy từ status. Phản hồi gồm ok, code, message, version, data.
Mã lỗi: 400 nhập liệu, 403 nguồn/phiên sai, 404 không tìm thấy, 409 xung đột, 503 lỗi nạp/lưu.
HTTP giới hạn header 8 KiB, body 64 KiB; không hỗ trợ chunked; đóng kết nối sau phản hồi.
Host/Origin và token chống yêu cầu ghi từ trang ngoài. Tài nguyên tĩnh được liệt kê cố định.

## Dữ liệu và phục hồi

Dữ liệu nằm trong Data/DocGia.txt và Data/DauSach.txt, tương đối với thư mục chạy.
Sao lưu cả hai trước khi chuyển phiên bản. Không chạy chương trình cũ để ghi đè định dạng mới.
DOCGIA_V3 lưu mỗi độc giả và lượt mượn trên dòng riêng; DAUSACH_V1 lưu bản sao trên dòng riêng.
Bộ đọc vẫn hỗ trợ định dạng một dòng cũ. Lịch sử thẻ đã xóa được giữ để tính Top 10.
Cả hai file chưa tồn tại là thư viện mới; thiếu một file, dữ liệu lỗi hoặc liên kết sai sẽ khóa nghiệp vụ.

Mọi thay đổi gọi LuuDuLieu trước khi báo thành công. Lưu đồng bộ dùng ảnh .prepare,
nhật ký .pending chứa checksum và hai ảnh dữ liệu; khởi động sau hoàn tất giao dịch dang dở.
Nếu lỗi lưu, web khóa giao dịch mới và hiện nút thử lưu lại. Không gửi lại thao tác thêm/mượn.
Giữ nguyên .pending nếu phục hồi thất bại; kiểm tra quyền ghi, dung lượng và file bị khóa.
Đóng cưỡng bức trước khi lưu có thể mất thay đổi trên RAM. Chỉ nút Dừng ứng dụng mới kiểm tra trạng thái chưa lưu.
Chuyển tên cũ nhiều từ cần xác nhận trên web. Dừng trước xác nhận không ghi chuyển đổi.
Tab cũ bị từ chối khi version không còn khớp; tải lại trước khi thao tác tiếp.

## Thuật toán và giao ước

BST tìm/chèn/xóa O(H), tệ nhất O(N). LCG phân tán mã nhưng không bảo đảm cây cân bằng.
QuickSort tự viết trung bình O(N log N), tệ nhất O(N²). Duyệt LNR cho thứ tự mã thẻ.
Danh sách sách theo thể loại, tìm kiếm và Top 10 dùng các API của Đức.
themDauSach và ThemDocGia chỉ nhận quyền sở hữu dữ liệu khi thành công.
Xóa độc giả chuyển lịch sử đã hoàn tất sang kho lưu trữ; giải phóng cây và kho khi dừng.

## Kiểm thử

```powershell
powershell -NoProfile -ExecutionPolicy Bypass -File tests/chay_kiem_thu.ps1
```

Cần Python 3 khi test. Script build với -Wall -Wextra -Wpedantic -Werror, kiểm thử nghiệp vụ,
file, tích hợp và HTTP trên dữ liệu tạm. Không dùng dữ liệu thật cho test phá hủy.
Kiểm tra thủ công: thêm độc giả/sách, mượn/trả/báo mất, hủy xác nhận, phân trang,
hai tab, Unicode, bàn phím, màn hình nhỏ và khởi động lại.
