#include "../DocGia.h"
#include <iostream>
#include <cstdio>
#include <cstring>
#include <utility> // std::swap

using namespace std;

// ============================================================================
// PHÂN HỆ: QUẢN LÝ THẺ ĐỘC GIẢ (CÂY NHỊ PHÂN TÌM KIẾM - BST)
// FILE: DocGia_In.cpp - Chức năng In danh sách độc giả (Theo Mã Thẻ & Theo Tên Họ)
// PHỤ TRÁCH: LÊ VĂN HOÀNG HIẾU
// ============================================================================

// [HÀM ĐỆ QUY DUYỆT LNR ĐỂ IN ĐỘC GIẢ - NỘI BỘ]
// Giải thuật: Duyệt LNR (Trái -> Gốc -> Phải).
// Nhờ tính chất BST (khóa con trái < gốc < khóa con phải), kết quả in ra sẽ
// tự động có thứ tự maThe tăng dần mà không cần thêm bất kỳ bước sắp xếp nào.
static void DuyetLNR_InDocGia(TREE_DocGia root) {
    if (root == nullptr)
        return;
    DuyetLNR_InDocGia(root->pLeft);
    printf("%-8d | %-50s %-20s | %-4s | %s\n", 
           root->data.maThe, root->data.ho, root->data.ten, 
           root->data.phai, root->data.trangThai == 1 ? "Hoat dong" : "Bi khoa");
    DuyetLNR_InDocGia(root->pRight);
}

// [CHẾ ĐỘ 1: IN DANH SÁCH THEO MÃ THẺ TĂNG DẦN - DUYỆT LNR]
// Mục đích: In toàn bộ danh sách độc giả theo thứ tự maThe tăng dần.
// Độ phức tạp: O(N) - Thăm đúng mỗi node trên cây một lần.
void InDanhSachDocGia_TheoMaThe(TREE_DocGia root) {
    if (KiemTraCayRong(root)) {
        cout << "[THONG BAO] Danh sach doc gia trong rong.\n";
        return;
    }
    printf("\n%-8s | %-50s %-20s | %-4s | %s\n", 
           "Ma The", "Ho", "Ten", "Phai", "Trang Thai");
    for (int i = 0; i < 95; i++)
        printf("-");
    printf("\n");
    DuyetLNR_InDocGia(root);
}

// [HÀM PHỤ TRỢ: ĐỔ TOÀN BỘ NODE TRÊN CÂY BST VÀO MẢNG CON TRỎ TẠM]
// Mục đích: Chuẩn bị dữ liệu cho bước sắp xếp QuickSort theo Tên + Họ.
// Giải thuật: Duyệt LNR và gán ĐỊA CHỈ của trường data vào từng ô mảng.
// Ưu điểm: Mảng chỉ lưu con trỏ TheDocGia* (8 byte), không sao chép struct lớn.
//   Khi sắp xếp chỉ hoán đổi con trỏ, tối ưu bộ nhớ và thời gian.
// Tham số index: Truyền tham chiếu để cập nhật vị trí đồng bộ qua các lần đệ quy.
void DoCayVaoMang(TREE_DocGia root, TheDocGia *arr[], int &index) {
    if (root == nullptr)
        return;
    DoCayVaoMang(root->pLeft, arr, index);
    arr[index++] = &(root->data); // Lưu địa chỉ trường data, không sao chép
    DoCayVaoMang(root->pRight, arr, index);
}

// [HÀM SO SÁNH TÊN VÀ HỌ CỦA 2 ĐỘC GIẢ]
// Mục đích: Định nghĩa tiêu chuẩn sắp xếp dùng cho QuickSort.
// Ưu tiên: So sánh Tên trước; nếu trùng Tên mới so sánh Họ.
//   Lý do: Theo chuẩn tiếng Việt, khi tra cứu bảng chữ cái sẽ ưu tiên Tên trước.
// Trả về: < 0 (a đứng trước b), = 0 (giống nhau), > 0 (a đứng sau b).
int SoSanhTenHo(const TheDocGia *a, const TheDocGia *b) {
    int cmpTen = strcmp(a->ten, b->ten);
    if (cmpTen != 0)
        return cmpTen;
    return strcmp(a->ho, b->ho);
}

// [THUẬT TOÁN QUICKSORT - PHÂN HOẠCH VỚI MEDIAN-OF-THREE PIVOT]
// Mục đích: Sắp xếp mảng con trỏ độc giả arr[left..right] theo Tên + Họ tăng dần.
// TẠI SAO DÙNG MEDIAN-OF-THREE THAY VÌ PIVOT CỐ ĐỊNH:
//   - Pivot cố định (Lomuto - luôn chọn phần tử cuối):
//     Tệ nhất O(N²) khi mảng đã sắp xếp sẵn (xảy ra thường xuyên khi tải từ file).
//   - Median-of-Three:
//     Chọn trung vị của 3 phần tử (đầu, giữa, cuối) làm pivot.
//     Loại bỏ trường hợp tệ nhất O(N²), duy trì O(N log N) ổn định trong thực tế.
// Bẫy lỗi: Khi left >= right thì dừng ngay. Mảng 2 phần tử xử lý hoán đổi trực tiếp.
// Độ phức tạp: Trung bình O(N log N), không gian O(log N) cho ngăn xếp đệ quy.
void QuickSortDocGia(TheDocGia *arr[], int left, int right) {
    if (left >= right)
        return; // Mảng 0 hoặc 1 phần tử, không cần sắp xếp

    // Bẫy lỗi: Mảng 2 phần tử (right - left == 1):
    // Xử lý thủ công bằng 1 phép so sánh-hoán đổi để tránh tràn biên chỉ số.
    if (right - left == 1) {
        if (SoSanhTenHo(arr[left], arr[right]) > 0)
            swap(arr[left], arr[right]);
        return;
    }

    int mid = left + (right - left) / 2; // Tránh tràn số nguyên

    // ---- BƯỚC 1: SẮP XẾP 3 PHẦN TỬ ĐẦU-GIỮA-CUỐI ĐỂ TÌM MEDIAN ----
    if (SoSanhTenHo(arr[left], arr[mid]) > 0)
        swap(arr[left], arr[mid]);
    if (SoSanhTenHo(arr[left], arr[right]) > 0)
        swap(arr[left], arr[right]);
    if (SoSanhTenHo(arr[mid], arr[right]) > 0)
        swap(arr[mid], arr[right]);

    // Đưa pivot (arr[mid]) về arr[right-1] để phân hoạch vùng [left+1 .. right-2]
    swap(arr[mid], arr[right - 1]);
    TheDocGia *pivot = arr[right - 1];

    // ---- BƯỚC 2: PHÂN HOẠCH ----
    int i = left, j = right - 1;
    while (true) {
        while (SoSanhTenHo(arr[++i], pivot) < 0)
            ; // Tiến i qua phần tử nhỏ hơn pivot
        while (SoSanhTenHo(arr[--j], pivot) > 0)
            ; // Lùi j qua phần tử lớn hơn pivot
        if (i >= j)
            break;              // Hai con trỏ đã gặp nhau → dừng
        swap(arr[i], arr[j]);   // Hoán đổi phần tử lệch vị trí
    }
    // Đặt pivot vào đúng vị trí phân hoạch cuối cùng
    swap(arr[i], arr[right - 1]);

    // ---- BƯỚC 3: ĐỆ QUY SẮP XẾP 2 PHẦN ĐÃ PHÂN HOẠCH ----
    QuickSortDocGia(arr, left, i - 1);  // Nửa bên trái pivot
    QuickSortDocGia(arr, i + 1, right); // Nửa bên phải pivot
}

// [CHẾ ĐỘ 2: IN DANH SÁCH THEO TÊN + HỌ TĂNG DẦN]
// Mục đích: In danh sách độc giả sắp xếp theo bảng chữ cái của Tên (sau đó Họ).
// Các bước thực hiện:
//   1. Đếm N = DemSoLuongDocGia(root).
//   2. Cấp phát mảng con trỏ động TheDocGia** arr = new TheDocGia*[N].
//   3. Nạp N địa chỉ vào mảng bằng DoCayVaoMang (duyệt LNR).
//   4. Sắp xếp mảng bằng QuickSortDocGia (Median-of-Three).
//   5. In bảng kết quả.
//   6. Giải phóng mảng: delete[] arr (KHÔNG delete từng arr[i]!).
void InDanhSachDocGia_TheoTenHo(TREE_DocGia root) {
    if (KiemTraCayRong(root)) {
        cout << "[THONG BAO] Danh sach doc gia trong rong.\n";
        return;
    }

    int n = DemSoLuongDocGia(root);

    // Cấp phát mảng con trỏ động (chỉ lưu địa chỉ, không sao chép data)
    TheDocGia **arr = new TheDocGia *[n];
    if (arr == nullptr) {
        cout << "[LOI] Khong du bo nho!\n";
        return;
    }

    // Nạp toàn bộ địa chỉ node vào mảng
    int index = 0;
    DoCayVaoMang(root, arr, index);

    // Sắp xếp mảng con trỏ (hoán đổi địa chỉ, không di chuyển dữ liệu lớn)
    if (n > 1) {
        QuickSortDocGia(arr, 0, n - 1);
    }

    // In bảng kết quả
    printf("\n%-4s | %-8s | %-50s %-20s | %-4s | %s\n", 
           "STT", "Ma The", "Ho", "Ten", "Phai", "Trang Thai");
    for (int i = 0; i < 110; i++)
        printf("-");
    printf("\n");

    for (int i = 0; i < n; i++) {
        printf("%-4d | %-8d | %-50s %-20s | %-4s | %s\n", 
               i + 1, arr[i]->maThe, arr[i]->ho, arr[i]->ten, 
               arr[i]->phai, arr[i]->trangThai == 1 ? "Hoat dong" : "Bi khoa");
    }

    // Giải phóng mảng con trỏ tạm thời
    // CHÚ Ý: Chỉ delete[] mảng con trỏ, KHÔNG delete từng arr[i]!
    // arr[i] trỏ đến trường data trong node trên cây BST.
    delete[] arr;
    arr = nullptr;
}
