# Phân công hiện hành: web localhost

Hiếu và Đức đã thống nhất chuyển toàn bộ giao diện sang web, loại bỏ UI console.
Phân công này thay thế phân công giao diện trước đây. Tỷ lệ 50/50 là kế hoạch gốc;
không áp dụng máy móc cho phần việc web bổ sung.

## Hiếu — Lê Văn Hoàng Hiếu (N24DECE018)

- BST độc giả, mã thẻ, thêm/sửa/xóa, tìm kiếm và sắp xếp độc giả.
- DSLK mượn trả, quá hạn, file độc giả và lịch sử lưu trữ.
- Toàn bộ Web/, Server/, Server.h: HTML/CSS/JS, HTTP/API, kiểm tra dữ liệu và tích hợp lưu.
- Thay main bằng vòng đời server, gỡ UI.h/UI, cập nhật build và kiểm thử web.
- Báo cáo và vấn đáp: giao diện, request/response, phiên bản dữ liệu, xử lý lỗi lưu, BST và DSLK mượn trả.

## Đức — Lê Anh Đức (N24DECE012)

- Mảng con trỏ Đầu sách, DSLK DMS, mã bản sao và trạng thái sách.
- Tìm kiếm, thể loại, Top 10, Date và file sách.
- Cung cấp API nghiệp vụ sách, review tích hợp web; sửa thuật toán thuộc phân hệ mình.
- Báo cáo và vấn đáp: cấu trúc sách, tìm kiếm, sắp xếp, thống kê và ngày tháng.
- Không còn phụ trách Core UI console hoặc thiết kế giao diện.

## Quyền thay đổi và phối hợp

Hiếu được phép thay UI console, main, build, lớp tích hợp và tài liệu theo thống nhất chung.
Nghiệp vụ sách/Date vẫn thuộc Đức; không tự ý thay thuật toán. CauTruc.h là giao ước bất biến.
Dùng nhánh hieu/web-localhost; không commit/push trực tiếp main. Hai thành viên review trước merge.
Phân công gốc và lịch sử thay đổi được lưu trong Git.

## Các mốc triển khai

1. Ghi nhận trạng thái hiện có, cập nhật phân công; tách nghiệp vụ khỏi console.
2. HTTP/API, codec, quản lý nạp/lưu và độc giả.
3. Web sách, mượn trả, quá hạn, Top 10; xem trước và xác nhận.
4. Gỡ UI console, cập nhật tài liệu, kiểm thử nghiệp vụ và HTTP.
5. Hai người kiểm tra trình duyệt, dữ liệu thực tế trên bản sao và review trước merge.

## Tiêu chí nghiệm thu

Chức năng a–j chạy offline qua localhost; mọi thao tác thay đổi có xác nhận và lưu ngay.
Giữ BST/DSLK/mảng, không STL container/std::sort. Build không lỗi/cảnh báo.
Kiểm tra dữ liệu rỗng/sai, quá hạn 7/8 ngày, giới hạn 3 sách, ISBN trùng,
trả/báo mất, lịch sử thẻ xóa, lỗi lưu, phục hồi pending và nhiều tab.
Không còn UI console trong source/build; báo cáo và demo giao diện do Hiếu phụ trách.
