#pragma once
#include <cstring>
#include <cstddef>

namespace DauSachNoiBo {
// Kiểm tra dấu kết thúc trước khi dùng strcmp/strcpy, tránh đọc ngoài mảng char.
template<std::size_t N>
bool chuoiHopLe(const char (&s)[N]) {
    return s[0] != '\0' && std::memchr(s, '\0', N) != nullptr;
}

// QuickSort tự cài đặt: trung bình O(N log N), không dùng std::sort/container.
// Pivot median-of-three hạn chế phân hoạch lệch với dữ liệu đã có thứ tự.
// Chỉ đệ quy nhánh ngắn hơn để chặn độ sâu ngăn xếp ở O(log N).
template<class T, class Less>
void sapXep(T *a, int trai, int phai, Less less) {
    while (trai < phai) {
        int giua = trai + (phai - trai) / 2;
        if (less(a[giua], a[trai])) {
            T tam = a[giua]; a[giua] = a[trai]; a[trai] = tam;
        }
        if (less(a[phai], a[trai])) {
            T tam = a[phai]; a[phai] = a[trai]; a[trai] = tam;
        }
        if (less(a[phai], a[giua])) {
            T tam = a[phai]; a[phai] = a[giua]; a[giua] = tam;
        }
        T pivot = a[giua];
        int i = trai, j = phai;
        while (i <= j) {
            while (less(a[i], pivot)) ++i;
            while (less(pivot, a[j])) --j;
            if (i <= j) {
                T tam = a[i]; a[i] = a[j]; a[j] = tam;
                ++i; --j;
            }
        }
        if (j - trai < phai - i) {
            if (trai < j) sapXep(a, trai, j, less);
            trai = i;
        } else {
            if (i < phai) sapXep(a, i, phai, less);
            phai = j;
        }
    }
}
}
