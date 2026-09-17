#include "DocGia.h"
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <fstream>
#include <iostream>
#include <windows.h>

using namespace std;

// ============================================================================
// NHÓM 1: CÁC HÀM CƠ BẢN VÀ QUẢN LÝ BỘ NHỚ TRÊN CÂY BST
// ============================================================================

// [HÀM KHỞI TẠO CÂY BST]
// Mục đích: Thiết lập con trỏ gốc về nullptr trước khi sử dụng cây.
// Lý do truyền tham chiếu (&root): Cần thay đổi giá trị của con trỏ gốc bên
// ngoài hàm.
void KhoiTaoCay(TREE_DocGia &root) { root = nullptr; }

// [HÀM KIỂM TRA CÂY RỖNG]
// Mục đích: Bẫy lỗi đầu vào trước khi thực hiện các thao tác duyệt cây.
// Tất cả các hàm duyệt, in, tìm kiếm đều phải kiểm tra cây rỗng trước.
bool KiemTraCayRong(TREE_DocGia root) { return root == nullptr; }

// [HÀM ĐẾM TỔNG SỐ LƯỢNG ĐỘC GIẢ - ĐỆ QUY]
// Mục đích: Đếm chính xác số node trên cây BST để cấp phát mảng con trỏ tạm
// thời cho chức năng sắp xếp theo Tên + Họ. Giải thuật: Chia để trị (Divide and
// Conquer). SoLuong(cây) = 1 (node gốc) + cây con trái + cây con phải.
int DemSoLuongDocGia(TREE_DocGia root) {
  // Trường hợp cơ sở: Cây rỗng hoặc đã đến lá, không đếm thêm được.
  if (root == nullptr)
    return 0;
  // Đệ quy: Tổng số node = 1 (gốc hiện tại) + số node cây trái + số node cây
  // phải
  return 1 + DemSoLuongDocGia(root->pLeft) + DemSoLuongDocGia(root->pRight);
}

// [HÀM GIẢI PHÓNG DSLK MUON TRA CỦA MỘT ĐỘC GIẢ - HÀM NỘI BỘ]
// Mục đích: Giải phóng toàn bộ bộ nhớ cấp phát động cho DSLK mượn trả trước khi
// xóa node độc giả để tránh rò rỉ bộ nhớ (Memory Leak). Giải thuật: Dùng 2 con
// trỏ pCurr và pNext duyệt lần lượt, delete từng node.
static void GiaiPhongDSMuonTra(NodeMuonTra *&dsMuonTra) {
  NodeMuonTra *pCurr = dsMuonTra;
  while (pCurr != nullptr) {
    NodeMuonTra *pNext = pCurr->pNext; // Lưu lại địa chỉ node tiếp theo
    delete pCurr;                      // Thu hồi bộ nhớ node hiện tại
    pCurr = pNext;                     // Tiến sang node tiếp theo
  }
  dsMuonTra = nullptr; // Chống dangling pointer
}

// [HÀM GIẢI PHÓNG TOÀN BỘ BỘ NHỚ CÂY BST - HẬU THỨ TỰ LRN]
// Mục đích: Thu hồi toàn bộ bộ nhớ động đã cấp phát, tránh Memory Leak khi
// chương trình kết thúc hoặc khi cần tái khởi tạo cây. Giải thuật: Duyệt Hậu
// thứ tự (Post-order: LRN - Trái -> Phải -> Gốc). Với mỗi NodeDocGia, PHẢI giải
// phóng dsMuonTra TRƯỚC khi delete node đó, vì dsMuonTra là cấp phát động lồng
// bên trong.
void GiaiPhongCayDocGia(TREE_DocGia &root) {
  if (root == nullptr)
    return; // cây con rỗng thì dừng
  // Đệ quy giải phóng toàn bộ cây con bên trái
  GiaiPhongCayDocGia(root->pLeft);
  // Đệ quy giải phóng toàn bộ cây con bên phải
  GiaiPhongCayDocGia(root->pRight);
  // Giải phóng DSLK mượn trả gắn với node hiện tại TRƯỚC
  GiaiPhongDSMuonTra(root->data.dsMuonTra);
  // Giải phóng bản thân node hiện tại và chống dangling pointer
  delete root;
  root = nullptr;
}

// ============================================================================
// NHÓM 2: TÌM KIẾM VÀ SINH MÃ THẺ TỰ ĐỘNG
// ============================================================================

// [HÀM TÌM KIẾM ĐỘC GIẢ THEO MÃ THẺ - TÌM KIẾM NHỊ PHÂN TRÊN BST]
NodeDocGia *TimKiemDocGia(TREE_DocGia root, int maThe) {
  if (root == nullptr)
    return nullptr; // Không tìm thấy
  if (maThe == root->data.maThe)
    return root; // Tìm thấy chính xác
  if (maThe < root->data.maThe)
    return TimKiemDocGia(root->pLeft, maThe); // Tìm ở cây con trái
  return TimKiemDocGia(root->pRight, maThe);  // Tìm ở cây con phải
}

// [HÀM KIỂM TRA MÃ THẺ TỒN TẠI]
// Mục đích: Kiểm tra nhanh trước khi thêm mới để đảm bảo tính duy nhất của khóa
// BST. Giải thuật: Gọi TimKiemDocGia; nếu kết quả khác nullptr thì đã tồn tại.
bool KiemTraMaTheTonTai(TREE_DocGia root, int maThe) {
  return TimKiemDocGia(root, maThe) != nullptr;
}

// ============================================================================
// BỘ SINH ĐỒNG DƯ TUYẾN TÍNH CHU KỲ ĐẦY
// (Full-Period Linear Congruential Generator — LCG)
// ============================================================================
//
// [BÀI TOÁN]
//   Sinh mã thẻ độc giả thỏa 3 yêu cầu đồng thời:
//     (1) Không trùng lặp.
//     (2) Phân bố giả ngẫu nhiên → cây BST tự nhiên gần cân bằng (chiều cao
//         trung bình ≈ 2·ln(N) ≈ 1.386·log₂(N)) mà KHÔNG CẦN cây AVL.
//     (3) Hiệu năng O(1) mỗi lần sinh, kể cả khi khoảng đã lấp đầy 99.99%.
//
// [GIẢI THUẬT - CÔNG THỨC LCG]
//   X_{n+1} = (a × X_n + c) mod M
//
//   Trong đó:
//     M = kích thước khoảng (số giá trị có thể sinh)
//     a = nhân tử (multiplier)
//     c = gia số (increment)
//     X_0 = hạt giống khởi đầu (seed)
//
// [ĐỊNH LÝ HULL-DOBELL (1962) — ĐIỀU KIỆN CHU KỲ ĐẦY]
//   Bộ sinh LCG có chu kỳ = M (ghé thăm MỌI giá trị trong [0, M-1] đúng 1 lần
//   trước khi lặp lại) khi và chỉ khi thỏa đồng thời 3 điều kiện:
//
//   Điều kiện 1: gcd(c, M) = 1
//     → c và M phải nguyên tố cùng nhau.
//
//   Điều kiện 2: (a - 1) chia hết cho MỌI thừa số nguyên tố của M.
//     → Nếu M = p1^e1 × p2^e2 × ... thì (a-1) mod pi == 0 với mọi i.
//
//   Điều kiện 3: Nếu 4 | M thì 4 | (a - 1).
//     → Nếu M chia hết cho 4 thì (a-1) cũng phải chia hết cho 4.
//
// [CHỨNG MINH THAM SỐ ĐƯỢC CHỌN THỎA MÃN HULL-DOBELL]
//
//   M = 90.000 = 2⁴ × 3² × 5⁴
//   Các thừa số nguyên tố của M: {2, 3, 5}
//
//   a = 421, suy ra a - 1 = 420 = 2² × 3 × 5 × 7
//     Kiểm tra ĐK 2: 420 mod 2 = 0 ✓, 420 mod 3 = 0 ✓, 420 mod 5 = 0 ✓
//     Kiểm tra ĐK 3: 4 | 90.000? Có. 4 | 420? 420/4 = 105 ✓
//
//   c = 7 (số nguyên tố, không phải thừa số của M)
//     Kiểm tra ĐK 1: gcd(7, 90.000) = 1 ✓ (vì 7 ∉ {2, 3, 5})
//
//   → Cả 3 điều kiện Hull-Dobell đều thỏa mãn.
//   → Bộ sinh có chu kỳ = 90.000 (ghé thăm mọi giá trị đúng 1 lần).
//
// [TẠI SAO PHƯƠNG PHÁP NÀY GIỮ CÂY BST CÂN BẰNG]
//   Chuỗi LCG có tính chất phân bố đều (uniformly distributed) trên [0, M-1].
//   Khi chèn N giá trị phân bố đều vào BST, chiều cao kỳ vọng:
//     E[h] = 2·ln(N) ≈ 1.386 × log₂(N)
//   So với cây cân bằng lý tưởng (chiều cao = log₂(N)), chỉ tệ hơn ~38%.
//   So với max+1 (cây lệch hoàn toàn, chiều cao = N), tốt hơn hàng nghìn lần.
//
// [ĐỘ PHỨC TẠP]
//   Sinh mã: O(1) — chỉ 1 phép nhân + 1 phép cộng + 1 phép chia lấy dư.
//   Kiểm tra trùng với dữ liệu cũ (từ file): O(log N) trên BST gần cân bằng.
//   Trong cùng một phiên chạy: KHÔNG BAO GIỜ tự trùng (chu kỳ đầy đảm bảo).
// ============================================================================

// --- Hằng số LCG (đã chứng minh thỏa Hull-Dobell ở trên) ---
static const int LCG_M = 90000; // Chu kỳ = kích thước khoảng sinh mã
static const int LCG_A = 421;   // Nhân tử (a - 1 = 420 chia hết cho 2,3,4,5)
static const int LCG_C = 7;     // Gia số (nguyên tố cùng nhau với M)
static const int MA_OFFSET = 10000; // Dịch chuyển: mã thẻ ∈ [10000, 99999]

// Trạng thái hiện tại của bộ sinh (X_n)
// Biến static nội bộ file: chỉ DocGia.cpp truy cập được, tránh ô nhiễm
// namespace. Giá trị -1 nghĩa là chưa được khởi tạo.
static int g_lcgState = -1;

// [HÀM KHỞI TẠO HẠT GIỐNG LCG - NỘI BỘ]
// Mục đích: Khởi tạo X_0 (seed) từ đồng hồ hệ thống nếu chưa được thiết lập.
// Tại sao dùng time(NULL): Mỗi lần chạy chương trình sẽ có seed khác nhau
//   → chuỗi mã thẻ khác nhau mỗi phiên → phân bố đều trên nhiều phiên.
// Tại sao lấy mod LCG_M: Seed phải nằm trong [0, M-1] để bộ sinh hoạt động
// đúng.
static void KhoiTaoLCG() {
  if (g_lcgState == -1) {
    srand((unsigned int)time(NULL));
    g_lcgState = rand() % LCG_M;
  }
}

// [HÀM SINH MÃ THẺ TỰ ĐỘNG - LCG CHU KỲ ĐẦY]
// Mục đích: Cấp mã thẻ duy nhất với phân bố giả ngẫu nhiên giúp BST cân bằng.
// Quy trình:
//   1. Khởi tạo seed nếu lần đầu gọi.
//   2. Tính X_{n+1} = (421 × X_n + 7) mod 90.000     [O(1)]
//   3. Mã thẻ = X_{n+1} + 10.000                      [O(1)]
//   4. Kiểm tra mã này đã tồn tại trên cây chưa       [O(log N)]
//      (chỉ có thể trùng với dữ liệu nạp từ file ở phiên trước,
//       KHÔNG BAO GIỜ trùng với mã sinh trong phiên hiện tại).
//   5. Nếu trùng → lặp bước 2 (cực hiếm khi khoảng thưa).
//
// Phân tích khi gần đầy (N ≈ M):
//   Vì LCG chu kỳ đầy, nó sẽ duyệt qua TẤT CẢ M giá trị theo thứ tự cố định.
//   Nên dù chỉ còn 1 ô trống, nó cũng TÌM ĐƯỢC trong tối đa M bước.
//   Đây là ưu điểm vượt trội so với random thuần (có thể lặp vô hạn ở cùng giá
//   trị).
//
// So sánh hiệu năng:
//   | Phương pháp     | Khi thưa (N << M) | Khi gần đầy (N ≈ M)         |
//   |-----------------|--------------------|-----------------------------|
//   | max + 1         | O(h) nhưng lệch   | O(h) nhưng cây lệch O(N)   |
//   | Random thuần    | O(log N) ~1 lần    | O(M/(M-N) × log N) rất chậm |
//   | LCG chu kỳ đầy  | O(log N) ~1 lần    | O(M × log N) tối đa, đảm bảo tìm
//   được |
int SinhMaTheTuDong(TREE_DocGia root) {
  KhoiTaoLCG();
  int maThe;
  int count = 0;
  do {
    // Bước tiến LCG: X_{n+1} = (a × X_n + c) mod M
    // Phép nhân có thể tràn int 32-bit nếu a × X_n > 2^31.
    // Giá trị lớn nhất: 421 × 89.999 = 37.889.579 < 2^31 = 2.147.483.648 → AN TOÀN.
    g_lcgState = (LCG_A * g_lcgState + LCG_C) % LCG_M;
    // Dịch chuyển vào khoảng mã thẻ [10.000, 99.999]
    maThe = g_lcgState + MA_OFFSET;
    count++;
    // Bẫy lỗi biên: Nếu đã duyệt hết một vòng chu kỳ M mà không còn ô trống
    if (count > LCG_M) {
      cout << "[LOI] Kho ma the da day (toi da 90.000 the)!\n";
      return -1;
    }
  } while (KiemTraMaTheTonTai(root, maThe));
  // Mã này chưa tồn tại trên cây → hợp lệ, trả về
  return maThe;
}

// ============================================================================
// NHÓM 3: CHỨC NĂNG (a) - THÊM, HIỆU CHỈNH, XÓA THẺ ĐỘC GIẢ
// ============================================================================

// [HÀM KIỂM TRA ĐỘC GIẢ CÓ ĐANG MƯỢN SÁCH HAY KHÔNG]
// Mục đích: Ràng buộc nghiệp vụ - chặn thao tác xóa thẻ khi độc giả còn
//   giữ sách của thư viện chưa trả.
// Giải thuật: Duyệt tuyến tính DSLK dsMuonTra của độc giả, tìm node có
//   trangThai == 0 (đang mượn, chưa trả).
// Độ phức tạp: O(M) với M là số lần mượn, thực tế M <= MAX_MUON_SACH = 3.
// Trả về: true nếu đang mượn ít nhất 1 cuốn, false nếu đã trả hết hoặc chưa
// mượn.
bool KiemTraDocGiaDangMuonSach(NodeDocGia *pDG) {
  if (pDG == nullptr)
    return false;
  NodeMuonTra *pCurr = pDG->data.dsMuonTra;
  while (pCurr != nullptr) {
    if (pCurr->data.trangThai == 0)
      return true; // Đang mượn sách!
    pCurr = pCurr->pNext;
  }
  return false; // Đã trả hết hoặc chưa mượn
}

// [HÀM THÊM ĐỘC GIẢ MỚI VÀO CÂY BST - ĐỆ QUY]
// Mục đích: Chèn một thẻ độc giả mới vào đúng vị trí trên cây BST sao cho
//   tính chất BST (trái < gốc < phải theo khóa maThe) không bị vi phạm.
// Giải thuật: Đệ quy so sánh maThe với khóa node hiện tại để tìm vị trí lá
//   trống phù hợp rồi cắm node mới vào đó.
// Bẫy lỗi:
//   - Từ chối nếu maThe đã tồn tại (tính duy nhất của khóa BST).
//   - Bẫy lỗi cấp phát RAM thất bại (new trả về nullptr do hết bộ nhớ).
//   - Khởi tạo pLeft, pRight = nullptr và dsMuonTra = nullptr cho node mới.
// Độ phức tạp: Trung bình O(log N), tệ nhất O(N) khi cây lệch.
bool ThemDocGia(TREE_DocGia &root, TheDocGia dg) {
  if (root == nullptr) {
    // Tìm được vị trí lá trống phù hợp, tiến hành cấp phát node mới
    NodeDocGia *pNew = new NodeDocGia;
    if (pNew == nullptr) {
      // Bẫy lỗi: Hết bộ nhớ RAM (rất hiếm xảy ra trên máy hiện đại)
      cout << "[LOI] Khong du bo nho de cap phat node moi!\n";
      return false;
    }
    pNew->data = dg;
    pNew->data.dsMuonTra =
        dg.dsMuonTra; // Giữ nguyên DSLK đã nạp (khi đọc file)
    pNew->pLeft = nullptr;
    pNew->pRight = nullptr;
    root = pNew;
    return true;
  }
  // Kiểm tra trùng khóa - không cho thêm mã thẻ đã tồn tại
  if (dg.maThe == root->data.maThe) {
    cout << "[LOI] Ma the " << dg.maThe << " da ton tai trong he thong!\n";
    return false;
  }
  // Đệ quy tìm vị trí: nhỏ hơn đi trái, lớn hơn đi phải
  if (dg.maThe < root->data.maThe)
    return ThemDocGia(root->pLeft, dg);
  return ThemDocGia(root->pRight, dg);
}

// [HÀM HIỆU CHỈNH THÔNG TIN ĐỘC GIẢ]
// Mục đích: Cập nhật họ, tên, phái, trạng thái khóa/mở của thẻ độc giả.
// Ràng buộc bất biến: maThe là KHÓA CỦA CÂY BST - tuyệt đối KHÔNG được sửa.
//   Nếu sửa maThe, tính chất BST sẽ bị vi phạm mà không phát hiện được ngay,
//   dẫn đến tìm kiếm sai kết quả về sau.
// Giải thuật: Dùng TimKiemDocGia để tìm node O(log N), sau đó cập nhật trực
// tiếp.
bool HieuChinhDocGia(TREE_DocGia root, int maThe, const char *ho,
                     const char *ten, const char *phai, int trangThai) {
  NodeDocGia *pDG = TimKiemDocGia(root, maThe);
  if (pDG == nullptr) {
    cout << "[LOI] Khong tim thay doc gia co ma the: " << maThe << "\n";
    return false;
  }
  // Cập nhật các trường được phép thay đổi (không cho sửa maThe)
  strncpy(pDG->data.ho, ho, sizeof(pDG->data.ho) - 1);
  strncpy(pDG->data.ten, ten, sizeof(pDG->data.ten) - 1);
  strncpy(pDG->data.phai, phai, sizeof(pDG->data.phai) - 1);
  pDG->data.trangThai = trangThai;
  // Đảm bảo chuỗi kết thúc bằng ký tự null (tránh lỗi khi in ra)
  pDG->data.ho[sizeof(pDG->data.ho) - 1] = '\0';
  pDG->data.ten[sizeof(pDG->data.ten) - 1] = '\0';
  pDG->data.phai[sizeof(pDG->data.phai) - 1] = '\0';
  return true;
}

// [HÀM TÌM NODE THẾ MẠNG - DÙNG CHO THUẬT TOÁN XÓA NODE 2 CON]
// Mục đích: Khi xóa node có 2 con, cần tìm node THAY THẾ (thế mạng) để
//   duy trì tính chất cây BST mà không cần tái cấu trúc lại toàn bộ cây.
// Lý do chọn node cực trái của cây con phải (Phần tử Nhỏ Nhất Bên Phải):
//   - Nó lớn hơn TẤT CẢ node bên cây con trái của X → thỏa điều kiện bên trái.
//   - Nó nhỏ hơn TẤT CẢ node còn lại bên cây con phải của X → thỏa điều kiện
//   bên phải. → Thay X bằng nó là an toàn, tính chất BST được bảo toàn hoàn
//   toàn.
// Cơ chế:
//   - X: Node cần xóa (có 2 con). Ban đầu X = node cần xóa.
//   - Y: Node đang duyệt tìm node thế mạng (bắt đầu = X->pRight).
//   - Đi theo pLeft đến khi Y->pLeft == nullptr: Y chính là node thế mạng.
//   - Sao chép data của Y sang X (ghi đè X), sau đó X trỏ đến Y (để xóa Y).
// Kết quả: X trỏ đến node thế mạng cần xóa thực sự, Y là pRight của nó.
void TimNodeTheMang(NodeDocGia *&X, NodeDocGia *&Y) {
  if (Y->pLeft != nullptr) {
    // Tiếp tục đi sang trái tìm node cực trái (phần tử nhỏ nhất bên phải)
    TimNodeTheMang(X, Y->pLeft);
  } else {
    // Y là node thế mạng: Sao chép data của Y vào X (ghi đè, không sửa con trỏ cây)
    // QUAN TRỌNG: Chỉ sao chép TheDocGia (trường data), không sao chép pLeft/pRight.
    X->data = Y->data;
    // BẪY LỖI QUẢN LÝ BỘ NHỚ: Gán dsMuonTra của Y về nullptr để khi delete Y,
    // danh sách mượn trả đã chuyển giao sang X không bị xóa nhầm (chống Dangling Pointer / Double Free).
    Y->data.dsMuonTra = nullptr;
    // Chuyển X trỏ đến Y - Y sẽ là node bị xóa thực sự (delete)
    X = Y;
    // Kết nối lại: cha của Y sẽ trỏ đến con phải của Y (vì Y không có con trái)
    Y = Y->pRight;
  }
}

// [HÀM XÓA THẺ ĐỘC GIẢ KHỎI CÂY BST]
// Mục đích: Xóa một độc giả khỏi hệ thống theo maThe.
// Ràng buộc nghiệp vụ: Không cho xóa nếu độc giả đang giữ sách chưa trả.
// Giải thuật: Đệ quy tìm node cần xóa, sau đó xử lý 3 trường hợp:
//   TRƯỜNG HỢP 1 - Node lá (0 con):
//     Giải phóng dsMuonTra → delete node → gán nullptr. Đơn giản nhất.
//   TRƯỜNG HỢP 2 - Node 1 con (chỉ có trái HOẶC chỉ có phải):
//     Cho cha trỏ thẳng đến con duy nhất → delete node. Nối tắt qua node bị xóa.
//   TRƯỜNG HỢP 3 - Node 2 con (phức tạp nhất):
//     Giải phóng dsMuonTra của node bị xóa TRƯỚC (chống Memory Leak).
//     Gọi TimNodeTheMang để tìm node cực trái của cây con phải và sao chép data.
//     Thực tế xóa node thế mạng (đã tách an toàn dsMuonTra = nullptr).
// Độ phức tạp: Trung bình O(log N), tệ nhất O(N).
bool XoaDocGia(TREE_DocGia &root, int maThe) {
  if (root == nullptr) {
    cout << "[LOI] Khong tim thay doc gia co ma the: " << maThe << "\n";
    return false;
  }

  if (maThe < root->data.maThe) {
    return XoaDocGia(root->pLeft, maThe);
  } else if (maThe > root->data.maThe) {
    return XoaDocGia(root->pRight, maThe);
  } else {
    // Đã tìm được node cần xóa
    // Kiểm tra ràng buộc nghiệp vụ: không xóa khi đang mượn sách
    if (KiemTraDocGiaDangMuonSach(root)) {
      cout << "[CANH BAO] Doc gia ma the " << maThe
           << " dang muon sach chua tra. Khong the xoa!\n";
      return false;
    }

    NodeDocGia *pXoa = nullptr;

    if (root->pLeft == nullptr && root->pRight == nullptr) {
      // ---- TRƯỜNG HỢP 1: Node lá (không có con) ----
      pXoa = root;
      root = nullptr;
      GiaiPhongDSMuonTra(pXoa->data.dsMuonTra);
      delete pXoa;
    } else if (root->pLeft == nullptr) {
      // ---- TRƯỜNG HỢP 2a: Chỉ có con phải ----
      pXoa = root;
      root = root->pRight;
      GiaiPhongDSMuonTra(pXoa->data.dsMuonTra);
      delete pXoa;
    } else if (root->pRight == nullptr) {
      // ---- TRƯỜNG HỢP 2b: Chỉ có con trái ----
      pXoa = root;
      root = root->pLeft;
      GiaiPhongDSMuonTra(pXoa->data.dsMuonTra);
      delete pXoa;
    } else {
      // ---- TRƯỜNG HỢP 3: Có đủ 2 con ----
      // BẪY LỖI MEMORY LEAK: Giải phóng lịch sử mượn trả của chính độc giả bị xóa TRƯỚC
      // khi dữ liệu node thế mạng ghi đè lên root->data.
      GiaiPhongDSMuonTra(root->data.dsMuonTra);
      pXoa = root;
      TimNodeTheMang(pXoa, root->pRight);
      // Lúc này pXoa trỏ đến node thế mạng Y cũ (Y->data.dsMuonTra đã được gán nullptr an toàn).
      delete pXoa;
    }

    return true;
  }
}

// ============================================================================
// NHÓM 4: CHỨC NĂNG (b) - IN DANH SÁCH ĐỘC GIẢ (2 CHẾ ĐỘ)
// ============================================================================

// [HÀM ĐỆ QUY DUYỆT LNR ĐỂ IN ĐỘC GIẢ - NỘI BỘ]
static void DuyetLNR_InDocGia(TREE_DocGia root) {
  if (root == nullptr)
    return;
  DuyetLNR_InDocGia(root->pLeft);
  printf("%-8d | %-50s %-20s | %-4s | %s\n", root->data.maThe,
         root->data.ho, root->data.ten, root->data.phai,
         root->data.trangThai == 1 ? "Hoat dong" : "Bi khoa");
  DuyetLNR_InDocGia(root->pRight);
}

// [CHẾ ĐỘ 1: IN DANH SÁCH THEO MÃ THẺ TĂNG DẦN - DUYỆT LNR]
// Mục đích: In toàn bộ danh sách độc giả theo thứ tự maThe tăng dần.
// Giải thuật: Duyệt Trung thứ tự (In-order: LNR - Trái -> Gốc -> Phải).
//   Nhờ tính chất BST: duyệt LNR tự nhiên cho ra thứ tự tăng dần của khóa
//   (maThe) mà không cần bất kỳ bước sắp xếp bổ sung nào.
// Độ phức tạp: O(N) - thăm đúng mỗi node một lần.
void InDanhSachDocGia_TheoMaThe(TREE_DocGia root) {
  if (KiemTraCayRong(root)) {
    cout << "[THONG BAO] Danh sach doc gia trong rong.\n";
    return;
  }
  printf("\n%-8s | %-50s %-20s | %-4s | %s\n", "Ma The", "Ho",
         "Ten", "Phai", "Trang Thai");
  for (int i = 0; i < 95; i++)
    printf("-");
  printf("\n");
  DuyetLNR_InDocGia(root);
}

// [HÀM PHỤ TRỢ: ĐỔ TOÀN BỘ NODE TRÊN CÂY BST VÀO MẢNG CON TRỎ TẠM]
// Mục đích: Chuẩn bị dữ liệu cho bước sắp xếp QuickSort theo Tên + Họ.
// Giải thuật: Duyệt LNR và gán ĐỊA CHỈ của trường data vào từng ô mảng.
// Ưu điểm: Mảng chỉ lưu con trỏ TheDocGia* (8 byte trên 64-bit),
//   không sao chép struct lớn. Sắp xếp bằng cách hoán đổi con trỏ.
// Tham số index: Truyền theo tham chiếu để cập nhật đồng bộ qua các lần đệ quy.
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
//   Lý do: Tiếng Việt gọi tên trước họ khi tra cứu theo bảng chữ cái.
// Trả về: < 0 (a đứng trước b), = 0 (giống nhau), > 0 (a đứng sau b).
int SoSanhTenHo(const TheDocGia *a, const TheDocGia *b) {
  int cmpTen = strcmp(a->ten, b->ten);
  if (cmpTen != 0)
    return cmpTen;
  return strcmp(a->ho, b->ho);
}

// [THUẬT TOÁN QUICKSORT - PHÂN HOẠCH VỚI MEDIAN-OF-THREE PIVOT]
// Mục đích: Sắp xếp mảng con trỏ độc giả arr[left..right] theo Tên + Họ tăng
// dần.
//
// TẠI SAO DÙNG MEDIAN-OF-THREE THAY VÌ PIVOT CỐ ĐỊNH:
//   - Pivot cố định (Lomuto - luôn chọn phần tử cuối):
//     + Tệ nhất O(N²) khi mảng đã sắp xếp sẵn (xảy ra thường xuyên khi tải từ
//     file).
//   - Median-of-Three:
//     + Chọn trung vị của 3 phần tử (đầu, giữa, cuối) làm pivot.
//     + Loại bỏ trường hợp tệ nhất khi dữ liệu đã/gần sắp xếp.
//     + Giữ O(N log N) ổn định hơn trong thực tế.
//
// CƠ CHẾ:
//   1. Sắp xếp thủ công 3 phần tử arr[left], arr[mid], arr[right] → arr[mid] =
//   median.
//   2. Đưa arr[mid] (median/pivot) về vị trí arr[right-1].
//   3. Phân hoạch: i duyệt từ trái, j từ phải, hoán đổi khi lệch vùng.
//   4. Đặt pivot về vị trí phân hoạch cuối cùng.
//   5. Đệ quy sắp xếp 2 nửa trái/phải.
//
// Bẫy lỗi: Khi left >= right (mảng <= 1 phần tử) thì dừng ngay.
// Độ phức tạp: Trung bình O(N log N), không gian O(log N) cho ngăn xếp đệ quy.
void QuickSortDocGia(TheDocGia *arr[], int left, int right) {
  if (left >= right)
    return; // Mảng 0 hoặc 1 phần tử, không cần sắp

  // Bẫy lỗi: Mảng 2 phần tử (right - left == 1):
  // Không thể áp dụng Median-of-Three + phân hoạch vì arr[right-1] == arr[left]
  // → Vòng lặp --j sẽ truy cập arr[left-1] ra ngoài biên mảng.
  // Giải pháp: Xử lý thủ công bằng 1 phép so sánh-hoán đổi đơn giản.
  if (right - left == 1) {
    if (SoSanhTenHo(arr[left], arr[right]) > 0)
      swap(arr[left], arr[right]);
    return;
  }

  int mid = left + (right - left) / 2; // Tránh tràn số nguyên

  // ---- BƯỚC 1: SẮP XẾP 3 PHẦN TỬ ĐẦU-GIỮA-CUỐI ĐỂ TÌM MEDIAN ----
  // Sau bước này: arr[left] <= arr[mid] <= arr[right].
  // arr[mid] là median của 3 phần tử → pivot phân hoạch cân bằng nhất.
  if (SoSanhTenHo(arr[left], arr[mid]) > 0)
    swap(arr[left], arr[mid]);
  if (SoSanhTenHo(arr[left], arr[right]) > 0)
    swap(arr[left], arr[right]);
  if (SoSanhTenHo(arr[mid], arr[right]) > 0)
    swap(arr[mid], arr[right]);

  // Đưa pivot (arr[mid]) về arr[right-1] để phân hoạch vùng [left+1 .. right-2]
  // arr[left] đã là phần tử nhỏ nhất → luôn ở bên trái pivot (không cần so sánh
  // lại) arr[right] đã là phần tử lớn nhất → luôn ở bên phải pivot (không cần
  // so sánh lại)
  swap(arr[mid], arr[right - 1]);
  TheDocGia *pivot = arr[right - 1];

  // ---- BƯỚC 2: PHÂN HOẠCH ----
  // i bắt đầu từ left (sẽ ++i ngay trong vòng while → left+1)
  // j bắt đầu từ right-1 (sẽ --j ngay trong vòng while → right-2)
  // Vùng phân hoạch thực sự: [left+1 .. right-2] (left và right đã đứng đúng vị
  // trí)
  int i = left, j = right - 1;
  while (true) {
    while (SoSanhTenHo(arr[++i], pivot) < 0)
      ; // Tiến i qua phần tử nhỏ hơn pivot
    while (SoSanhTenHo(arr[--j], pivot) > 0)
      ; // Lùi j qua phần tử lớn hơn pivot
    if (i >= j)
      break;              // Hai vùng đã gặp nhau → dừng
    swap(arr[i], arr[j]); // Hoán đổi phần tử lệch vị trí
  }
  // Đặt pivot vào đúng vị trí phân hoạch cuối cùng
  swap(arr[i], arr[right - 1]);

  // ---- BƯỚC 3: ĐỆ QUY SẮP XẾP 2 PHẦN ĐÃ PHÂN HOẠCH ----
  QuickSortDocGia(arr, left, i - 1);  // Phần bên trái pivot
  QuickSortDocGia(arr, i + 1, right); // Phần bên phải pivot
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

  // Sắp xếp mảng (hoán đổi địa chỉ, không di chuyển data lớn)
  if (n > 1) {
    QuickSortDocGia(arr, 0, n - 1);
  }

  // In bảng kết quả
  printf("\n%-4s | %-8s | %-50s %-20s | %-4s | %s\n", "STT", "Ma The", "Ho",
         "Ten", "Phai", "Trang Thai");
  for (int i = 0; i < 110; i++)
    printf("-");
  printf("\n");

  for (int i = 0; i < n; i++) {
    printf("%-4d | %-8d | %-50s %-20s | %-4s | %s\n", i + 1, arr[i]->maThe,
           arr[i]->ho, arr[i]->ten, arr[i]->phai,
           arr[i]->trangThai == 1 ? "Hoat dong" : "Bi khoa");
  }

  // Giải phóng mảng con trỏ tạm thời
  // CHÚ Ý: Chỉ delete[] mảng, KHÔNG delete từng arr[i]!
  // arr[i] là địa chỉ của trường data trong node trên cây, không phải cấp phát
  // riêng.
  delete[] arr;
  arr = nullptr;
}

// ============================================================================
// NHÓM 5: LƯU TRỮ VÀ PHỤC HỒI DỮ LIỆU TỪ FILE (FILE I/O)
// ============================================================================

// [HÀM GHI DSLK MUON TRA CỦA MỘT ĐỘC GIẢ VÀO FILE - HÀM NỘI BỘ]
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
    fout << mt.maSach << " " << mt.ngayMuon.ngay << " " << mt.ngayMuon.thang
         << " " << mt.ngayMuon.nam << " " << mt.ngayTra.ngay << " "
         << mt.ngayTra.thang << " " << mt.ngayTra.nam << " " << mt.trangThai
         << "\n";
    pCurr = pCurr->pNext;
  }
}

// [HÀM GHI NỘI BỘ CÂY THEO NLR VÀO OFSTREAM ĐÃ MỞ SẴN]
// Mục đích: Hàm đệ quy nội bộ duyệt NLR và ghi vào file stream đã được mở.
//   Tách riêng để GhiFileDocGia chỉ cần mở/đóng file một lần duy nhất,
//   tránh hoàn toàn vấn đề với biến static và mở file nhiều lần.
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
//   trái/phải → Cây BST được TÁI TẠO đúng cấu trúc ban đầu. (Nếu ghi LNR = tăng
//   dần, khi chèn lại sẽ tạo cây lệch hoàn toàn, O(N) tệ nhất).
// Thiết kế: Mở file một lần duy nhất (truncate) → truyền stream vào GhiCayNLR →
// đóng.
//   Tự động tạo thư mục "Data" nếu chưa có trên hệ thống Windows.
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
// Mục đích: Phục hồi cây BST và toàn bộ DSLK mượn trả khi khởi động chương
// trình. Giải thuật: Đọc tuần tự từng record rồi gọi ThemDocGia chèn vào cây.
//   Vì file ghi theo NLR, thứ tự đọc và chèn sẽ tái tạo đúng cấu trúc cây ban
//   đầu.
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
      fin >> mt.maSach >> mt.ngayMuon.ngay >> mt.ngayMuon.thang >>
          mt.ngayMuon.nam >> mt.ngayTra.ngay >> mt.ngayTra.thang >>
          mt.ngayTra.nam >> mt.trangThai;
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
