#include "../DocGia.h"
#include <cctype>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>

using namespace std;

// ============================================================================
// PHÂN HỆ: QUẢN LÝ THẺ ĐỘC GIẢ (CÂY NHỊ PHÂN TÌM KIẾM - BST)
// FILE: DocGia_Core.cpp - Các hàm khởi tạo, kiểm tra, tiện ích chuẩn hóa chuỗi
// PHỤ TRÁCH: LÊ VĂN HOÀNG HIẾU (N24DECE018)
// ============================================================================

// ############################################################################
// NHÓM 0: CÁC TIỆN ÍCH CHUẨN HÓA VÀ XỬ LÝ CHUỖI NHẬP LIỆU
// ############################################################################

// [HÀM KIỂM TRA CHUỖI RỖNG]
// Mục đích: Xác định xem chuỗi có bị bỏ trống hoặc chỉ chứa toàn dấu cách hay không.
// Trả về: true nếu rỗng hoặc chỉ có khoảng trắng, false nếu có ít nhất 1 ký tự hợp lệ.
bool KiemTraChuoiRong(const char *s) {
  if (s == nullptr)
    return true;
  while (*s != '\0') {
    if (!isspace((unsigned char)*s))
      return false; // Phát hiện ký tự hiển thị được
    s++;
  }
  return true;
}

// [HÀM CHUẨN HÓA KHOẢNG TRẮNG - TRIM VÀ COLLAPSE SPACES]
// Mục đích: Loại bỏ khoảng trắng thừa ở đầu và cuối chuỗi; đồng thời gộp nhiều
//   khoảng trắng liên tiếp ở giữa thành đúng 1 khoảng trắng đơn.
// Giải thuật: Kỹ thuật Two-Pointers (Con trỏ Đọc i và Con trỏ Ghi j) in-place:
//   - Con trỏ i duyệt toàn bộ chuỗi gốc.
//   - Bỏ qua các dấu cách ở đầu chuỗi.
//   - Khi gặp chuỗi dấu cách liên tiếp, cờ spaceFound chỉ cho phép ghi đúng 1 ký tự ' '.
//   - Xóa khoảng trắng cuối chuỗi nếu có và kết thúc chuỗi bằng ký tự '\0'.
// Độ phức tạp: O(N) thời gian, O(1) bộ nhớ phụ (xử lý trực tiếp trên mảng).
void ChuanHoaKhoangTrang(char *s) {
  if (s == nullptr)
    return;

  int i = 0, j = 0;
  // Bỏ qua toàn bộ khoảng trắng ở đầu chuỗi
  while (s[i] != '\0' && isspace((unsigned char)s[i])) {
    i++;
  }

  bool spaceFound = false;
  while (s[i] != '\0') {
    if (isspace((unsigned char)s[i])) {
      if (!spaceFound) {
        s[j++] = ' ';
        spaceFound = true;
      }
    } else {
      s[j++] = s[i];
      spaceFound = false;
    }
    i++;
  }

  // Cắt bỏ ký tự khoảng trắng thừa ở cuối chuỗi nếu có
  if (j > 0 && s[j - 1] == ' ') {
    j--;
  }
  s[j] = '\0';
}

// [HÀM ĐỊNH DẠNG TÊN ĐỘC GIẢ - TITLE CASE]
// Mục đích: Tự động chuyển đổi chuỗi tên người dùng nhập về chuẩn văn phong:
//   - Chữ cái đầu tiên của mỗi từ được VIẾT HOA.
//   - Các chữ cái còn lại trong từ được viết thường.
//   - Loại bỏ toàn bộ khoảng trắng thừa (gọi ChuanHoaKhoangTrang trước).
//   - An toàn tuyệt đối với chuỗi UTF-8 tiếng Việt (không làm hỏng byte >= 128).
// Ví dụ: "   ngUYEn   vAN   a  " -> "Nguyen Van A"
void DinhDangHoTen(char *s) {
  if (s == nullptr)
    return;

  ChuanHoaKhoangTrang(s);
  if (s[0] == '\0')
    return;

  bool newWord = true; // Cờ đánh dấu bắt đầu một từ mới
  for (int i = 0; s[i] != '\0'; i++) {
    unsigned char c = (unsigned char)s[i];
    if (isspace(c)) {
      newWord = true;
    } else if (c < 128) {
      if (newWord) {
        s[i] = (char)toupper(c);
        newWord = false;
      } else {
        s[i] = (char)tolower(c);
      }
    } else {
      // Byte UTF-8 tiếng Việt (>= 128): bảo toàn nguyên vẹn
      newWord = false;
    }
  }
}

// [HÀM CHUẨN HÓA MÃ KÝ HIỆU - CHỮ IN HOA KHÔNG KHOẢNG TRẮNG]
// Mục đích: Chuẩn hóa mã ISBN hoặc Mã Sách sang dạng chữ in hoa và loại bỏ hoàn toàn
//   các khoảng trắng vô tình gõ nhầm.
// Ví dụ: "  cs101_1  " -> "CS101_1", " it001 " -> "IT001"
void ChuanHoaMa(char *s) {
  if (s == nullptr)
    return;

  int i = 0, j = 0;
  while (s[i] != '\0') {
    if (!isspace((unsigned char)s[i])) {
      s[j++] = (char)toupper((unsigned char)s[i]);
    }
    i++;
  }
  s[j] = '\0';
}

// [HÀM PHÂN TÍCH LỖI HỌ TÊN]
// Mục đích: Kiểm tra tính hợp lệ của chuỗi Họ hoặc Tên theo quy chuẩn môn học và nghiệp vụ thư viện:
//   - Không được để trống hoặc chỉ có dấu cách.
//   - Cho phép chữ cái Latin (a-z, A-Z) và tiếng Việt UTF-8 (ký tự đa byte >= 128).
//   - Cho phép dấu cách giữa các từ (cho họ có đệm, hoặc tên 2 chữ như "Anh Thư").
//   - TUYỆT ĐỐI KHÔNG CHỨA CHỮ SỐ (0-9) VÀ KÝ TỰ ĐẶC BIỆT (!@#$%...).
// Trả về:
//   0 = Hoàn toàn hợp lệ.
//   1 = Chuỗi rỗng.
//   2 = Chứa chữ số (0-9), gán ký tự số vào kyTuViPham để thông báo trực quan.
//   3 = Chứa ký tự đặc biệt, gán ký tự đó vào kyTuViPham.
//   4 = Không chứa bất kỳ chữ cái nào.
int KiemTraLoiHoTen(const char *s, char &kyTuViPham) {
  kyTuViPham = '\0';
  if (s == nullptr || KiemTraChuoiRong(s))
    return 1; // Rỗng

  bool coChuCai = false;
  for (int i = 0; s[i] != '\0'; i++) {
    unsigned char c = (unsigned char)s[i];
    if (c >= 128) {
      // Byte đa byte UTF-8 tiếng Việt hợp lệ (á, à, ả, ã, ạ, đ, ...)
      coChuCai = true;
    } else if (isalpha(c)) {
      // Ký tự chữ cái Latin (A-Z, a-z)
      coChuCai = true;
    } else if (c == ' ') {
      // Khoảng trắng hợp lệ giữa các từ
      continue;
    } else if (isdigit(c)) {
      // Bẫy lỗi: Chứa chữ số (ví dụ: "Lê 1", "An 123")
      kyTuViPham = (char)c;
      return 2;
    } else {
      // Bẫy lỗi: Chứa ký tự đặc biệt (ví dụ: "Lê @#", "Trần_Văn")
      kyTuViPham = (char)c;
      return 3;
    }
  }

  if (!coChuCai)
    return 4; // Toàn ký tự không hợp lệ

  return 0; // Hợp lệ
}

// [HÀM KIỂM TRA CHỈ CHỨA CHỮ CÁI VÀ KHOẢNG TRẮNG]
// Mục đích: Trả về true nếu họ hoặc tên hợp lệ, false nếu chứa số hoặc ký tự lạ.
bool KiemTraChuCaiVaKhoangTrang(const char *s) {
  char dummy = '\0';
  return (KiemTraLoiHoTen(s, dummy) == 0);
}

// [HÀM KIỂM TRA ĐỊNH DẠNG MÃ SÁCH CON]
// Mục đích: Xác thực mã sách có đúng cấu trúc [ISBN]_[STT] để tương thích với API Danh Mục Sách.
// Quy tắc:
//   - Phải có đúng ký tự '_' phân cách giữa ISBN và số thứ tự bản sao sách.
//   - Phần ISBN trước dấu '_' không được rỗng và không chứa khoảng trắng.
//   - Phần STT sau dấu '_' phải là số nguyên dương >= 1 (toàn chữ số 0-9).
// Ví dụ đúng: "CS101_1", "TOAN_12"
// Ví dụ sai:  "CS101" (quên _), "_1" (thiếu ISBN), "CS101_" (thiếu STT), "CS101_0" (STT < 1)
bool KiemTraDinhDangMaSach(const char *maSach) {
  if (maSach == nullptr || maSach[0] == '\0')
    return false;

  const char *underscore = strchr(maSach, '_');
  if (underscore == nullptr)
    return false; // Thiếu dấu '_' phân cách

  // Phần ISBN trước dấu '_' phải có ít nhất 1 ký tự và không chứa khoảng trắng
  if (underscore == maSach)
    return false; // Rỗng phần ISBN

  for (const char *p = maSach; p < underscore; p++) {
    if (isspace((unsigned char)*p))
      return false; // ISBN không được có khoảng trắng
  }

  // Phần STT sau dấu '_' phải có ít nhất 1 ký tự và toàn là số
  const char *sttStr = underscore + 1;
  if (*sttStr == '\0')
    return false; // Rỗng phần số thứ tự

  for (int i = 0; sttStr[i] != '\0'; i++) {
    if (!isdigit((unsigned char)sttStr[i]))
      return false; // STT chứa ký tự không phải chữ số
  }

  // STT phải >= 1
  int sttVal = atoi(sttStr);
  if (sttVal <= 0)
    return false;

  return true;
}

// [HÀM SO SÁNH CHUỖI CON KHÔNG PHÂN BIỆT HOA THƯỜNG]
// Mục đích: Phục vụ tính năng tìm kiếm gần đúng (partial search).
// Giải thuật: Duyệt trượt chuỗi con subStr trên chuỗi mẹ str, so sánh từng ký tự
//   bằng hàm tolower().
// Trả về: true nếu subStr là chuỗi con của str (không phân biệt hoa/thường).
bool ChuaChuoiConKhongPhanBietHoaThuong(const char *str, const char *subStr) {
  if (str == nullptr || subStr == nullptr)
    return false;
  if (subStr[0] == '\0')
    return true; // Chuỗi rỗng luôn là chuỗi con

  int lenStr = (int)strlen(str);
  int lenSub = (int)strlen(subStr);
  if (lenSub > lenStr)
    return false;

  for (int i = 0; i <= lenStr - lenSub; i++) {
    bool match = true;
    for (int j = 0; j < lenSub; j++) {
      if (tolower((unsigned char)str[i + j]) !=
          tolower((unsigned char)subStr[j])) {
        match = false;
        break;
      }
    }
    if (match)
      return true;
  }
  return false;
}

// ############################################################################
// NHÓM 1: CÁC HÀM CƠ BẢN VÀ QUẢN LÝ BỘ NHỚ TRÊN CÂY BST
// ############################################################################

// [HÀM KHỞI TẠO CÂY BST]
// Mục đích: Thiết lập con trỏ gốc về nullptr trước khi sử dụng cây.
void KhoiTaoCay(TREE_DocGia &root) { root = nullptr; }

// [HÀM KIỂM TRA CÂY RỖNG]
bool KiemTraCayRong(TREE_DocGia root) { return root == nullptr; }

// [HÀM ĐẾM TỔNG SỐ LƯỢNG ĐỘC GIẢ - ĐỆ QUY]
// SoLuong(cây) = 1 (node gốc) + cây con trái + cây con phải.
int DemSoLuongDocGia(TREE_DocGia root) {
  if (root == nullptr)
    return 0;
  return 1 + DemSoLuongDocGia(root->pLeft) + DemSoLuongDocGia(root->pRight);
}

// [HÀM GIẢI PHÓNG DSLK MƯỢN TRẢ CỦA MỘT ĐỘC GIẢ]
// Chống con trỏ hoang (Dangling Pointer): Gán lại dsMuonTra = nullptr.
void GiaiPhongDSMuonTra(NodeMuonTra *&dsMuonTra) {
  NodeMuonTra *pCurr = dsMuonTra;
  while (pCurr != nullptr) {
    NodeMuonTra *pNext = pCurr->pNext; // Lưu lại địa chỉ node tiếp theo
    delete pCurr;                      // Thu hồi bộ nhớ node hiện tại
    pCurr = pNext;                     // Tiến sang node tiếp theo
  }
  dsMuonTra = nullptr; // Chống dangling pointer
}

// [HÀM GIẢI PHÓNG TOÀN BỘ BỘ NHỚ CÂY BST - HẬU THỨ TỰ LRN]
// Sau khi delete root, luôn gán root = nullptr để chống con trỏ hoang.
void GiaiPhongCayDocGia(TREE_DocGia &root) {
  if (root == nullptr)
    return; // Cây con rỗng thì dừng
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

// ############################################################################
// NHÓM 2: TÌM KIẾM ĐỘC GIẢ THEO TÊN / HỌ TÊN
// ############################################################################

// [HÀM PHỤ TRỢ: DUYỆT CÂY BST TÌM KIẾM THEO TỪ KHÓA]
// Giải thuật: Duyệt trung thứ tự (LNR), kiểm tra từ khóa khớp với:
//   - Tên độc giả
//   - Họ và chữ lót
//   - Cả họ và tên kết hợp ("Ho Ten")
// Kết quả trả về tự động sắp xếp theo thứ tự mã thẻ tăng dần do duyệt LNR.
static void DuyetLNR_TimDocGia(TREE_DocGia root, const char *tuKhoa,
                               TheDocGia *arr[], int &count, int maxKetQua) {
  if (root == nullptr || count >= maxKetQua)
    return;

  // Duyệt cây con trái
  DuyetLNR_TimDocGia(root->pLeft, tuKhoa, arr, count, maxKetQua);

  if (count < maxKetQua) {
    char hoTen[75];
    snprintf(hoTen, sizeof(hoTen), "%s %s", root->data.ho, root->data.ten);

    if (ChuaChuoiConKhongPhanBietHoaThuong(root->data.ten, tuKhoa) ||
        ChuaChuoiConKhongPhanBietHoaThuong(root->data.ho, tuKhoa) ||
        ChuaChuoiConKhongPhanBietHoaThuong(hoTen, tuKhoa)) {
      arr[count++] = &(root->data); // Lưu con trỏ trực tiếp, không sao chép struct
    }
  }

  // Duyệt cây con phải
  DuyetLNR_TimDocGia(root->pRight, tuKhoa, arr, count, maxKetQua);
}

// [HÀM TÌM KIẾM ĐỘC GIẢ THEO TÊN HOẶC HỌ TÊN]
// Nhận vào từ khóa, trả về số lượng độc giả tìm được trong mảng arr[].
int TimKiemDocGiaTheoTen(TREE_DocGia root, const char *tuKhoa, TheDocGia *arr[],
                         int maxKetQua) {
  int count = 0;
  if (root == nullptr || tuKhoa == nullptr || maxKetQua <= 0)
    return 0;

  // Chuẩn hóa từ khóa tìm kiếm (bỏ khoảng trắng thừa) trước khi tìm
  char tuKhoaChuan[100];
  strncpy(tuKhoaChuan, tuKhoa, sizeof(tuKhoaChuan) - 1);
  tuKhoaChuan[sizeof(tuKhoaChuan) - 1] = '\0';
  ChuanHoaKhoangTrang(tuKhoaChuan);

  if (tuKhoaChuan[0] == '\0')
    return 0;

  DuyetLNR_TimDocGia(root, tuKhoaChuan, arr, count, maxKetQua);
  return count;
}
