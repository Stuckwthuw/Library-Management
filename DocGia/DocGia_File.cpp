#include "../DocGia.h"
#include <iostream>
#include <fstream>
#include <windows.h>

using namespace std;

// ============================================================================
// PHÂN HỆ: QUẢN LÝ THẺ ĐỘC GIẢ (CÂY NHỊ PHÂN TÌM KIẾM - BST)
// FILE: DocGia_File.cpp - Lưu trữ và phục hồi dữ liệu thẻ độc giả từ file
// PHỤ TRÁCH: LÊ VĂN HOÀNG HIẾU
// ============================================================================

// [HÀM GHI DSLK MUON TRA CỦA MỘT ĐỘC GIẢ VÀO FILE - NỘI BỘ]
// Định dạng mỗi lượt mượn:
//   <maSach> <ngayMuon.ngay> <ngayMuon.thang> <ngayMuon.nam>
//   <ngayTra.ngay> <ngayTra.thang> <ngayTra.nam> <trangThai>
static void GhiDSMuonTraVaoFile(ofstream &fout, NodeMuonTra *dsMuonTra) {
    // Đếm số lượng lượt mượn để ghi tiêu đề
    int soLuong = 0;
    NodeMuonTra *pDem = dsMuonTra;
    while (pDem != nullptr) {
        soLuong++;
        pDem = pDem->pNext;
    }
    fout << soLuong << "\n";

    NodeMuonTra *pCurr = dsMuonTra;
    while (pCurr != nullptr) {
        MuonTra &mt = pCurr->data;
        fout << mt.maSach << " " 
             << mt.ngayMuon.ngay << " " << mt.ngayMuon.thang << " " << mt.ngayMuon.nam << " " 
             << mt.ngayTra.ngay << " " << mt.ngayTra.thang << " " << mt.ngayTra.nam << " " 
             << mt.trangThai << "\n";
        pCurr = pCurr->pNext;
    }
}

// [HÀM GHI NỘI BỘ CÂY THEO NLR VÀO OFSTREAM ĐÃ MỞ SẴN]
// Mục đích: Hàm đệ quy nội bộ duyệt NLR và ghi vào file stream đã được mở.
// Định dạng: maThe,ho,ten,phai,trangThai (dùng dấu phẩy bảo toàn khoảng trắng trong Họ).
static void GhiCayNLR(ofstream &fout, TREE_DocGia root) {
    if (root == nullptr)
        return;
    // N - Ghi gốc trước (Pre-order)
    TheDocGia &dg = root->data;
    fout << dg.maThe << "," << dg.ho << "," << dg.ten << "," << dg.phai << ","
         << dg.trangThai << "\n";
    GhiDSMuonTraVaoFile(fout, dg.dsMuonTra);
    // L - Ghi cây con trái
    GhiCayNLR(fout, root->pLeft);
    // R - Ghi cây con phải
    GhiCayNLR(fout, root->pRight);
}

// [HÀM GHI TOÀN BỘ CÂY ĐỘC GIẢ VÀO FILE - DUYỆT TIỀN THỨ TỰ NLR]
// Mục đích: Lưu trữ bền vững cây BST khi đóng chương trình.
// Giải thuật: Duyệt Tiền thứ tự (Pre-order: NLR - Gốc -> Trái -> Phải).
//   Lý do chọn NLR: Khi đọc lại và chèn lần lượt vào cây mới, vì node gốc
//   được ghi đầu tiên và chèn đầu tiên, các node con sẽ rơi đúng nhánh
//   trái/phải → Cây BST được TÁI TẠO đúng cấu trúc ban đầu.
//   (Nếu ghi LNR = tăng dần, khi chèn lại sẽ tạo cây lệch hoàn toàn, O(N) tệ nhất).
// Thiết kế: Tự động tạo thư mục "Data" nếu chưa có trên hệ thống Windows.
bool GhiFileDocGia(TREE_DocGia root, const char *filePath) {
    // Tạo thư mục "Data" an toàn nếu chưa tồn tại
    CreateDirectoryA("Data", NULL);

    ofstream fout(filePath, ios::out | ios::trunc);
    if (!fout.is_open()) {
        cout << "[LOI] Khong mo duoc file de ghi: " << filePath << "\n";
        return false;
    }
    // Duyệt NLR và ghi toàn bộ cây vào file stream đã mở
    GhiCayNLR(fout, root);
    fout.close();
    return true;
}

// [HÀM ĐỌC TOÀN BỘ DỮ LIỆU ĐỘC GIẢ TỪ FILE]
// Mục đích: Phục hồi cây BST và toàn bộ DSLK mượn trả khi khởi động chương trình.
// Giải thuật: Đọc tuần tự từng record rồi gọi ThemDocGia chèn vào cây.
//   Vì file ghi theo NLR, thứ tự đọc và chèn sẽ tái tạo đúng cấu trúc cây ban đầu.
// Bẫy lỗi:
//   - File chưa tồn tại: Báo thông báo và trả về false (không crash).
//   - Đọc họ tên có khoảng trắng: Dùng fin.getline với dấu phẩy phân cách.
//   - Đọc số lượng lượt mượn; cấp phát và nối từng NodeMuonTra vào cuối DSLK.
bool DocFileDocGia(TREE_DocGia &root, const char *filePath) {
    ifstream fin(filePath);
    if (!fin.is_open()) {
        cout << "[THONG BAO] File " << filePath
             << " chua ton tai. He thong khoi dong moi.\n";
        return false;
    }

    TheDocGia dg;
    // Đọc mã thẻ trước; nếu tới cuối file (EOF) thì dừng vòng lặp
    while (fin >> dg.maThe) {
        fin.ignore(); // Bỏ qua dấu phẩy ',' ngay sau maThe
        fin.getline(dg.ho, sizeof(dg.ho), ',');
        fin.getline(dg.ten, sizeof(dg.ten), ',');
        fin.getline(dg.phai, sizeof(dg.phai), ',');
        fin >> dg.trangThai;

        dg.dsMuonTra = nullptr;

        int soLuotMuon = 0;
        fin >> soLuotMuon;

        // Đọc và xây dựng DSLK mượn trả, chèn vào CUỐI để giữ thứ tự lịch sử
        NodeMuonTra *pTail = nullptr;
        for (int i = 0; i < soLuotMuon; i++) {
            NodeMuonTra *pNew = new NodeMuonTra;
            if (pNew == nullptr) {
                cout << "[LOI] Het bo nho khi doc file!\n";
                fin.close();
                return false;
            }
            MuonTra &mt = pNew->data;
            fin >> mt.maSach >> mt.ngayMuon.ngay >> mt.ngayMuon.thang >> mt.ngayMuon.nam 
                >> mt.ngayTra.ngay >> mt.ngayTra.thang >> mt.ngayTra.nam >> mt.trangThai;
            pNew->pNext = nullptr;

            // Nối vào cuối DSLK mượn trả
            if (dg.dsMuonTra == nullptr) {
                dg.dsMuonTra = pNew;
                pTail = pNew;
            } else {
                pTail->pNext = pNew;
                pTail = pNew;
            }
        }

        // Chèn độc giả (kèm DSLK mượn trả đầy đủ) vào cây BST
        ThemDocGia(root, dg);
    }

    fin.close();
    return true;
}
