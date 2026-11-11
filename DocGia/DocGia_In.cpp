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
    int cmpHo = Chuoi::soSanh(a->ho, b->ho);
    if (cmpHo != 0) return cmpHo;
    // Mã thẻ phân định hai người trùng họ tên, tránh thứ tự thay đổi sau QuickSort.
    return (a->maThe > b->maThe) - (a->maThe < b->maThe);
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
