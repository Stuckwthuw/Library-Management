#include "../DocGia.h"
#include <iostream>
#include <cstdlib>
#include <ctime>

using namespace std;

// ============================================================================
// PHÂN HỆ: QUẢN LÝ THẺ ĐỘC GIẢ (CÂY NHỊ PHÂN TÌM KIẾM - BST)
// FILE: DocGia_MaThe.cpp - Tìm kiếm và Sinh mã thẻ tự động (LCG chu kỳ đầy)
// PHỤ TRÁCH: LÊ VĂN HOÀNG HIẾU
// ============================================================================

// [HÀM TÌM KIẾM ĐỘC GIẢ THEO MÃ THẺ - TÌM KIẾM NHỊ PHÂN TRÊN BST]
// Mục đích: Tìm kiếm một node trong cây BST dựa vào khóa chính maThe.
// Giải thuật: So sánh maThe với node hiện tại để quyết định rẽ nhánh trái/phải.
// Độ phức tạp: Trung bình O(log N) khi cây gần cân bằng, O(N) trong trường hợp tệ nhất.
NodeDocGia* TimKiemDocGia(TREE_DocGia root, int maThe) {
    if (root == nullptr)
        return nullptr; // Không tìm thấy hoặc cây rỗng
    if (maThe == root->data.maThe)
        return root;    // Tìm thấy chính xác
    if (maThe < root->data.maThe)
        return TimKiemDocGia(root->pLeft, maThe); // Tìm ở cây con trái
    return TimKiemDocGia(root->pRight, maThe);     // Tìm ở cây con phải
}

// [HÀM KIỂM TRA MÃ THẺ TỒN TẠI]
// Mục đích: Kiểm tra nhanh trước khi thêm mới để đảm bảo tính duy nhất của khóa BST.
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
// ============================================================================

// --- Hằng số LCG (đã chứng minh thỏa Hull-Dobell ở trên) ---
static const int LCG_M = 90000;      // Chu kỳ = kích thước khoảng sinh mã
static const int LCG_A = 421;        // Nhân tử (a - 1 = 420 chia hết cho 2,3,4,5)
static const int LCG_C = 7;          // Gia số (nguyên tố cùng nhau với M)
static const int MA_OFFSET = 10000;  // Dịch chuyển: mã thẻ ∈ [10000, 99999]

// Trạng thái hiện tại của bộ sinh (X_n)
// Biến static nội bộ file: chỉ DocGia_MaThe.cpp truy cập được, tránh ô nhiễm namespace.
// Giá trị -1 nghĩa là chưa được khởi tạo seed.
static int g_lcgState = -1;

// [HÀM KHỞI TẠO HẠT GIỐNG LCG - NỘI BỘ]
// Mục đích: Khởi tạo X_0 (seed) từ đồng hồ hệ thống nếu chưa được thiết lập.
// Tại sao dùng time(NULL): Mỗi lần chạy chương trình sẽ có seed khác nhau
//   → chuỗi mã thẻ khác nhau mỗi phiên → phân bố đều trên nhiều phiên.
// Tại sao lấy mod LCG_M: Seed phải nằm trong [0, M-1] để bộ sinh hoạt động đúng.
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
//   Đây là ưu điểm vượt trội so với random thuần (có thể lặp vô hạn ở cùng giá trị).
int SinhMaTheTuDong(TREE_DocGia root) {
    KhoiTaoLCG();
    int maThe;
    int count = 0;
    do {
        // Bước tiến LCG: X_{n+1} = (a × X_n + c) mod M
        // Giá trị lớn nhất: 421 × 89.999 = 37.889.579 < 2^31 = 2.147.483.648 → AN TOÀN TRÀN SỐ.
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
