#include "../DocGia.h"
#include "../Common/Chuoi.h"
#include <iostream>
#include <cstdio>
#include <cstring>
#include <utility>

using namespace std;

// ============================================================================
// PHÂN HỆ: QUẢN LÝ THẺ ĐỘC GIẢ (CÂY NHỊ PHÂN TÌM KIẾM - BST)
// FILE: DocGia_In.cpp - Chức năng In danh sách độc giả (Theo Mã Thẻ & Theo Tên Họ)
// PHỤ TRÁCH: LÊ VĂN HOÀNG HIẾU (N24DECE018)
// ============================================================================

// Duyệt LNR để in danh sách độc giả theo thứ tự mã thẻ tăng dần
static void DuyetLNR_InDocGia(TREE_DocGia root) {
    if (root == nullptr)
        return;
    DuyetLNR_InDocGia(root->pLeft);
    printf("%-8d | %-50s %-20s | %-4s | %s\n", 
           root->data.maThe, root->data.ho, root->data.ten, 
           root->data.phai, root->data.trangThai == 1 ? "Hoat dong" : "Bi khoa");
    DuyetLNR_InDocGia(root->pRight);
}

// In danh sách độc giả theo mã thẻ tăng dần
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

// Đổ các node trên cây BST vào mảng con trỏ để sắp xếp
void DoCayVaoMang(TREE_DocGia root, TheDocGia *arr[], int &index) {
    if (root == nullptr)
        return;
    DoCayVaoMang(root->pLeft, arr, index);
    arr[index++] = &(root->data);
    DoCayVaoMang(root->pRight, arr, index);
}

// So sánh thứ tự theo Tên trước, nếu trùng tên thì so sánh Họ
int SoSanhTenHo(const TheDocGia *a, const TheDocGia *b) {
    int cmpTen = Chuoi::soSanh(a->ten, b->ten);
    if (cmpTen != 0)
        return cmpTen;
    return Chuoi::soSanh(a->ho, b->ho);
}

// Sắp xếp mảng độc giả bằng QuickSort (phân hoạch Median-of-Three)
void QuickSortDocGia(TheDocGia *arr[], int left, int right) {
    if (left >= right)
        return;

    if (right - left == 1) {
        if (SoSanhTenHo(arr[left], arr[right]) > 0)
            swap(arr[left], arr[right]);
        return;
    }

    int mid = left + (right - left) / 2;

    if (SoSanhTenHo(arr[left], arr[mid]) > 0)
        swap(arr[left], arr[mid]);
    if (SoSanhTenHo(arr[left], arr[right]) > 0)
        swap(arr[left], arr[right]);
    if (SoSanhTenHo(arr[mid], arr[right]) > 0)
        swap(arr[mid], arr[right]);

    swap(arr[mid], arr[right - 1]);
    TheDocGia *pivot = arr[right - 1];

    int i = left;
    int j = right - 1;

    while (true) {
        while (SoSanhTenHo(arr[++i], pivot) < 0) {}
        while (SoSanhTenHo(arr[--j], pivot) > 0) {}

        if (i < j) {
            swap(arr[i], arr[j]);
        } else {
            break;
        }
    }

    swap(arr[i], arr[right - 1]);

    QuickSortDocGia(arr, left, i - 1);
    QuickSortDocGia(arr, i + 1, right);
}

// In danh sách độc giả theo thứ tự Tên + Họ tăng dần
void InDanhSachDocGia_TheoTenHo(TREE_DocGia root) {
    int tongSo = DemSoLuongDocGia(root);
    if (tongSo == 0) {
        cout << "[THONG BAO] Danh sach doc gia trong rong.\n";
        return;
    }

    TheDocGia **arr = new TheDocGia*[tongSo];

    int index = 0;
    DoCayVaoMang(root, arr, index);

    QuickSortDocGia(arr, 0, tongSo - 1);

    printf("\n%-8s | %-50s %-20s | %-4s | %s\n", 
           "Ma The", "Ho", "Ten", "Phai", "Trang Thai");
    for (int k = 0; k < 95; k++)
        printf("-");
    printf("\n");

    for (int i = 0; i < tongSo; i++) {
        printf("%-8d | %-50s %-20s | %-4s | %s\n", 
               arr[i]->maThe, arr[i]->ho, arr[i]->ten, 
               arr[i]->phai, arr[i]->trangThai == 1 ? "Hoat dong" : "Bi khoa");
    }

    delete[] arr;
    arr = nullptr;
}
