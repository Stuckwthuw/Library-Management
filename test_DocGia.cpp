#include "DocGia.h"
#include <iostream>
#include <cassert>
#include <cstring>

using namespace std;

int main() {
    cout << "=== KIEM THU MODULE DOC GIA DA TACH BIET ===" << endl;

    TREE_DocGia root;
    KhoiTaoCay(root);
    assert(KiemTraCayRong(root) == true);
    cout << "1. Khoi tao & Kiem tra cay rong: PASS" << endl;

    // Sinh ma the tu dong
    int ma1 = SinhMaTheTuDong(root);
    assert(ma1 >= 10000 && ma1 <= 99999);
    cout << "2. Sinh ma the tu dong LCG: " << ma1 << " - PASS" << endl;

    // Them doc gia
    TheDocGia dg1;
    dg1.maThe = ma1;
    strcpy(dg1.ho, "Nguyen Van");
    strcpy(dg1.ten, "An");
    strcpy(dg1.phai, "Nam");
    dg1.trangThai = 1;
    dg1.dsMuonTra = nullptr;
    assert(ThemDocGia(root, dg1) == true);
    assert(DemSoLuongDocGia(root) == 1);
    cout << "3. Them doc gia vao cay BST: PASS" << endl;

    // Kiem tra ma the ton tai
    assert(KiemTraMaTheTonTai(root, ma1) == true);
    assert(KiemTraMaTheTonTai(root, 9999) == false);
    cout << "4. Tim kiem ma the ton tai: PASS" << endl;

    // Them 2 doc gia nua de test In & Xoa
    int ma2 = SinhMaTheTuDong(root);
    TheDocGia dg2;
    dg2.maThe = ma2;
    strcpy(dg2.ho, "Tran Thi");
    strcpy(dg2.ten, "Binh");
    strcpy(dg2.phai, "Nu");
    dg2.trangThai = 1;
    dg2.dsMuonTra = nullptr;
    ThemDocGia(root, dg2);

    int ma3 = SinhMaTheTuDong(root);
    TheDocGia dg3;
    dg3.maThe = ma3;
    strcpy(dg3.ho, "Le Van");
    strcpy(dg3.ten, "An"); // trung ten de test xet ho
    strcpy(dg3.phai, "Nam");
    dg3.trangThai = 1;
    dg3.dsMuonTra = nullptr;
    ThemDocGia(root, dg3);

    cout << "5. In danh sach theo ma the:" << endl;
    InDanhSachDocGia_TheoMaThe(root);

    cout << "6. In danh sach theo ten ho:" << endl;
    InDanhSachDocGia_TheoTenHo(root);

    // Hieu chinh doc gia
    assert(HieuChinhDocGia(root, ma1, "Nguyen Van", "An", "Nam", 0) == true);
    NodeDocGia* p = TimKiemDocGia(root, ma1);
    assert(p != nullptr && p->data.trangThai == 0);
    cout << "7. Hieu chinh doc gia (doi trang thai): PASS" << endl;

    // Xoa doc gia
    assert(XoaDocGia(root, ma2) == true);
    assert(DemSoLuongDocGia(root) == 2);
    assert(TimKiemDocGia(root, ma2) == nullptr);
    cout << "8. Xoa doc gia khoi cay BST: PASS" << endl;

    // Test File I/O
    assert(GhiFileDocGia(root, "Data/test_docgia.txt") == true);
    cout << "9. Ghi file DocGia: PASS" << endl;

    TREE_DocGia rootDocLai;
    KhoiTaoCay(rootDocLai);
    assert(DocFileDocGia(rootDocLai, "Data/test_docgia.txt") == true);
    assert(DemSoLuongDocGia(rootDocLai) == 2);
    cout << "10. Doc file DocGia tai tao cay BST: PASS" << endl;

    // Giai phong cay
    GiaiPhongCayDocGia(root);
    GiaiPhongCayDocGia(rootDocLai);
    assert(root == nullptr && rootDocLai == nullptr);
    cout << "11. Giai phong bo nho cay BST (LRN): PASS" << endl;

    cout << "\n>>> TOAN BO 11 TEST CASES CUA MODULE DOC GIA CHAY THANH CONG! <<<" << endl;
    return 0;
}
