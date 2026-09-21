# AI AGENTS INSTRUCTIONS & DEVELOPMENT RULES
## PROJECT: QUẢN LÝ THƯ VIỆN (C++ DATA STRUCTURES & ALGORITHMS)

> **Dành cho mọi AI Coding Assistant (Antigravity, Cursor, GitHub Copilot, Claude, ChatGPT, v.v.):**  
> Khi clone hoặc mở thư mục đồ án này, bạn **BẮT BUỘC** phải đọc và tuân thủ tuyệt đối 9 nhóm nguyên tắc dưới đây trước khi sinh hoặc chỉnh sửa bất kỳ dòng code nào.

---

## 1. PHÂN ĐỊNH PHẠM VI TRÁCH NHIỆM (OWNERSHIP & BOUNDARIES)

Dự án được thực hiện bởi nhóm 2 sinh viên với phân chia nhiệm vụ độc lập (tỷ lệ 50/50). AI phải xác định rõ đang hỗ trợ ai trước khi chỉnh sửa file:

| Thành viên | Phân hệ phụ trách | Danh sách file được phép chỉnh sửa |
| :--- | :--- | :--- |
| **Lê Văn Hoàng Hiếu** (N24DECE018) | **Thẻ Độc Giả & Mượn Trả** | `DocGia.h`, `DocGia/*.cpp`, `MuonTra.h`, `MuonTra/*.cpp`, `Data/DocGia.txt` |
| **Lê Anh Đức** (N24DECE012) | **Đầu Sách, DMS & Core UI** | `DauSach.h`, `DauSach/*.cpp`, `DanhMucSach.h`, `DanhMucSach/*.cpp`, `UI.h`, `UI/*.cpp`, `Date.h`, `Date/*.cpp`, `Data/DauSach.txt` |
| **File dùng chung (Cả hai)** | **Khung kiến trúc & Khởi chạy** | `CauTruc.h`, `main.cpp`, `README.md`, `KE_HOACH_PHAN_CHIA_CONG_VIEC.md` |

### ⛔ QUY TẮC BẤT KHẢ XÂM PHẠM:
- **KHÔNG TỰ Ý SỬA CODE CỦA NGƯỜI CÒN LẠI**: Khi đang tương tác với một thành viên, nếu tác vụ đụng đến file của người kia, AI **KHÔNG ĐƯỢC TỰ Ý SỬA**. 
- **CƠ CHẾ ĐƯA RA ĐỀ XUẤT (PROPOSE, DO NOT OVERWRITE)**: AI phải giải thích rõ lý do tại sao cần sửa, thay đổi đó ảnh hưởng gì đến module của người kia, và tạo một đoạn code đề xuất riêng để thành viên đang làm việc đem đi bàn bạc và xin ý kiến của bạn cùng nhóm trước.
- **GIAO ƯỚC `CauTruc.h` LÀ BẤT BIẾN (FROZEN CONTRACT)**: Tuyệt đối không tự ý thêm, sửa, xóa các trường trong các `struct` tại `CauTruc.h` nếu chưa có sự đồng ý của cả Hiếu và Đức.

---

## 2. MODULAR HÓA & TÁCH BIỆT FILE MÃ NGUỒN (CLEAN ARCHITECTURE & SRP)

- **Tách file độc lập theo từng nhóm nghiệp vụ (Single Responsibility Principle - SRP)**:
  - **TUYỆT ĐỐI KHÔNG dồn code vào một file khổng lồ** (như `DauSach.cpp` hay `DocGia.cpp` 700-1000 dòng).
  - Bắt buộc chia nhỏ các chức năng thành từng file riêng trong thư mục của phân hệ để dễ debug, tra cứu nhanh và bảo vệ vấn đáp điểm tối đa.
- **Nguyên tắc Header tổng (`Master Header .h`) & Thư mục triển khai (`Subdirectory/*.cpp`)**:
  - Mỗi phân hệ có một **Header tổng** đặt tại thư mục gốc (`DocGia.h`, `MuonTra.h`, `DauSach.h`, `DanhMucSach.h`, `UI.h`, `Date.h`) chứa các nguyên mẫu hàm (prototypes).
  - Các hàm được cài đặt chi tiết trong các file `.cpp` độc lập bên trong thư mục con tương ứng. Các file con này chỉ cần `#include "../<TenHeader>.h"`.
- **Cấu trúc phân chia thư mục bắt buộc**:
  - **Phân hệ Hiếu**:
    + `DocGia.h` + Thư mục `DocGia/`: `DocGia_Core.cpp`, `DocGia_MaThe.cpp`, `DocGia_Them.cpp`, `DocGia_HieuChinh.cpp`, `DocGia_Xoa.cpp`, `DocGia_In.cpp`, `DocGia_File.cpp`.
    + `MuonTra.h` + Thư mục `MuonTra/`: `MuonTra_Core.cpp`, `MuonTra_Muon.cpp`, `MuonTra_Tra.cpp`, `MuonTra_LietKe.cpp`, `MuonTra_QuaHan.cpp`.
  - **Phân hệ Đức**:
    + `DauSach.h` + Thư mục `DauSach/`: `DauSach_Core.cpp`, `DauSach_Them.cpp`, `DauSach_TimKiem.cpp`, `DauSach_TheLoai.cpp`, `DauSach_Top10.cpp`, `DauSach_File.cpp`.
    + `DanhMucSach.h` + Thư mục `DanhMucSach/`: `DanhMucSach_Core.cpp`, `DanhMucSach_MaSach.cpp`, `DanhMucSach_Them.cpp`, `DanhMucSach_TrangThai.cpp`.
    + `Date.h` + Thư mục `Date/`: `Date_Core.cpp`, `Date_KhoangCach.cpp`.
    + `UI.h` + Thư mục `UI/`: `UI_Console.cpp`, `UI_Menu.cpp`, `UI_Table.cpp`.
  - `main.cpp`: Chỉ chứa vòng lặp menu chính và gọi các hàm từ Master Headers.

---

## 3. QUY TRÌNH GIT FLOW: NHÁNH RIÊNG & REVIEW TRƯỚC KHI MERGE

- **🚫 TUYỆT ĐỐI KHÔNG COMMIT/PUSH TRỰC TIẾP LÊN NHÁNH `main`**: Nhánh `main` là nhánh sản phẩm ổn định, chỉ được cập nhật khi đã kiểm tra kỹ lưỡng.
- **Bắt buộc làm việc trên Branch riêng**:
  - Hiếu làm việc trên nhánh: `feature/doc-gia` hoặc `hieu/<ten-chuc-nang>`
  - Đức làm việc trên nhánh: `feature/dau-sach` hoặc `duc/<ten-chuc-nang>`
- **Quy trình 4 bước trước khi hợp nhất (Merge) vào `main`**:
  1. **Tự kiểm thử (Local Test)**: Biên dịch không lỗi (`0 error, 0 warning`) và chạy thử các test case biên trên nhánh cá nhân.
  2. **Push lên GitHub**: Đẩy nhánh cá nhân lên remote repo (`git push origin <ten-branch>`).
  3. **Kiểm tra chéo (Cross-Review)**: Thông báo cho bạn cùng nhóm kiểm tra lại code, đảm bảo không làm đứt gãy tính tương thích với module khác.
  4. **Merge vào `main`**: Sau khi cả hai đồng ý mới thực hiện merge nhánh vào `main`.

---

## 4. MINH BẠCH & BÁO CÁO THAY ĐỔI / PHÂN TÍCH LỖI (CHANGE & BUG AUDIT)

Sau mỗi lần sinh code, chỉnh sửa hoặc sửa lỗi (fix bug), AI **BẮT BUỘC** phải có phần báo cáo tổng kết phản hồi cho sinh viên với 4 nội dung sau:
1. **Danh sách thay đổi (What Changed)**: Chỉ rõ tên file nào đã sửa, dòng nào hoặc hàm nào đã được thêm/bớt/sửa.
2. **Phân tích nguyên nhân gốc rễ nếu sửa lỗi (Root Cause Bug Analysis)**:
   - Nếu chương trình bị lỗi (crash, sai kết quả, rò rỉ bộ nhớ, lặp vô tận, v.v.), AI phải giải thích:
     + *Lỗi gì?* (ví dụ: truy cập con trỏ `nullptr`, đứt gãy liên kết khi xóa node BST, tràn mảng con trỏ, lỗi tính ngày nhuận).
     + *Tại sao lại sai?* (do điều kiện dừng sai, quên cập nhật con trỏ, hoặc chưa bẫy trường hợp danh sách rỗng).
     + *Giải pháp khắc phục là gì?*
3. **Cảnh báo tác động (Side-effect Warning)**: Cảnh báo xem thay đổi này có làm ảnh hưởng đến các hàm khác trong hệ thống hoặc ảnh hưởng đến module của bạn cùng nhóm hay không.
4. **Hướng dẫn kiểm thử (Verification Guide)**: Cung cấp kịch bản test cụ thể (input nhập vào là gì, output mong đợi là gì) để sinh viên tự kiểm chứng lại trên máy.

---

## 5. CHÚ THÍCH CODE BẮT BUỘC (CODE COMMENTS & EXPLANATION)

Đồ án sẽ được chấm qua hình thức **Bảo vệ Vấn đáp trực tiếp từng dòng code với Thầy/Cô**. Do đó:
- **Phải giải thích TẠI SAO (Why), không chỉ nói LÀM GÌ (What)**: 
  - Tại mỗi hàm và thuật toán cốt lõi (xóa node 2 con trên cây BST, chèn mảng con trỏ có thứ tự, đánh mã tự động `[ISBN]_[STT]`, xử lý quá hạn 7 ngày, thống kê Top 10), AI phải chú thích rõ:
    + Mục đích giải thuật.
    + Tại sao lại chọn cách cài đặt này (ưu điểm về thời gian \(O\), bộ nhớ).
    + Các trường hợp biên (Edge cases) đã bẫy lỗi (ví dụ: cây rỗng, mảng đầy, danh sách rỗng, ngày không hợp lệ).
- **Ngôn ngữ chú thích**: Tiếng Việt có dấu, diễn đạt mạch lạc, chuẩn thuật ngữ môn CTDL&GT (ví dụ: *node thế mạng*, *duyệt LNR*, *cấp phát động*, *thu hồi bộ nhớ*).

Ví dụ mẫu:
```cpp
// [GIẢI THUẬT XÓA NODE 2 CON TRÊN CÂY BST]
// Mục đích: Xóa một độc giả khỏi hệ thống dựa vào mã thẻ.
// Lý do chọn: Tìm node thế mạng là node cực trái của cây con phải (nhỏ nhất bên phải)
// để đảm bảo tính chất cây BST không bị phá vỡ sau khi xóa.
// Bẫy lỗi: Đảm bảo độc giả không còn giữ sách mượn trước khi thực hiện xóa.
void TimNodeTheMang(NodeDocGia*& X, NodeDocGia*& Y) {
    if (Y->pLeft != nullptr) {
        TimNodeTheMang(X, Y->pLeft); // Tiếp tục đi sang nhánh trái nhất
    } else {
        // Sao chép dữ liệu của node thế mạng sang node cần xóa
        X->data = Y->data;
        X = Y; 
        Y = Y->pRight; // Nối lại liên kết cho cây con phải của node thế mạng
    }
}
```

---

## 6. RÀNG BUỘC MÔN HỌC CTDL&GT (ACADEMIC DSA CONSTRAINTS)

- 🚫 **TUYỆT ĐỐI KHÔNG DÙNG CÁC CONTAINER CÓ SẴN CỦA STL**:
  - Nghiêm cấm dùng: `std::vector`, `std::list`, `std::map`, `std::set`, `std::queue`, `std::stack`.
  - Nếu cần danh sách liên kết, mảng động, ngăn xếp $\rightarrow$ **phải tự cài đặt thủ công** bằng struct, con trỏ và cấp phát động (`new`, `delete`).
- 🚫 **TUYỆT ĐỐI KHÔNG DÙNG THƯ VIỆN SẮP XẾP CÓ SẴN**: Không dùng `std::sort`. Phải tự viết các thuật toán sắp xếp (QuickSort, MergeSort, SelectionSort...).

---

## 7. AN TOÀN BỘ NHỚ & BẪY LỖI CON TRỎ (MEMORY SAFETY)

- **Cấp phát đi đôi với giải phóng**: Mỗi lệnh `new` bắt buộc phải có lệnh `delete` tương ứng khi xóa phần tử hoặc khi kết thúc chương trình. Không để xảy ra tình trạng Memory Leak.
- **Chống con trỏ hoang (Dangling Pointer)**: Sau khi `delete ptr;`, luôn gán lại `ptr = nullptr;`.
- **Kiểm tra con trỏ trước khi truy cập**: Luôn kiểm tra `if (p != nullptr)` trước khi gọi `p->data` hoặc `p->pNext`.

---

## 8. MÔI TRƯỜNG & KHẢ NĂNG TƯƠNG THÍCH (ENVIRONMENT COMPATIBILITY)

- Ngôn ngữ: C++11 trở lên.
- Nền tảng: Hệ điều hành Windows.
- Sử dụng các thư viện chuẩn cho Console Windows: `<iostream>`, `<fstream>`, `<windows.h>`, `<conio.h>`, `<cstring>`, `<ctime>`.
- Tránh dùng các hàm không an toàn gây warning như `gets()`, thay vào đó dùng `cin.getline()` hoặc `fgets()`.
- Bắt lỗi nhập liệu: Chống trôi lệnh (`cin.ignore()`), kiểm tra nhập chuỗi rỗng hoặc nhập chữ vào biến số nguyên.

---

## 9. QUY TRÌNH KHI BẮT ĐẦU MỘT PHIÊN LÀM VIỆC MỚI (BOOTSTRAP CHECKLIST)

Mỗi khi người dùng mở phiên chat mới và yêu cầu code:
1. Đọc lại file `README.md` và `KE_HOACH_PHAN_CHIA_CONG_VIEC.md` để nắm ngữ cảnh.
2. Xác nhận xem người đang yêu cầu là **Hiếu** hay **Đức**.
3. Chỉ thực hiện các thao tác nằm đúng trong phân hệ của thành viên đó và trên đúng nhánh Git của họ.
4. Nếu yêu cầu có chạm đến file của thành viên còn lại $\rightarrow$ lập tức dừng lại, cảnh báo và chỉ đưa ra gợi ý/đoạn code mẫu để trao đổi nội bộ.
5. Sau khi hoàn thành thao tác, lập tức báo cáo chi tiết theo đúng quy chuẩn tại **Mục 4**.
