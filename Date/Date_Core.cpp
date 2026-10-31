#include "../Date.h"
#include <ctime>
#include <iomanip>
#include <sstream>
#include <stdexcept>

// Lịch Gregory: năm thế kỷ chỉ nhuận nếu chia hết cho 400.
bool namNhuan(int nam) {
    return nam % 400 == 0 || (nam % 4 == 0 && nam % 100 != 0);
}

int soNgayTrongThang(int thang, int nam) {
    const int ngay[] = {0,31,28,31,30,31,30,31,31,30,31,30,31};
    if (thang < 1 || thang > 12) return 0;
    return ngay[thang] + (thang == 2 && namNhuan(nam) ? 1 : 0);
}

// CauTruc.h quy định năm >= 1900; miền hiển thị dùng tối đa 4 chữ số năm.
bool ngayHopLe(Date date) {
    return date.nam >= 1900 && date.nam <= 9999 && date.ngay >= 1 &&
           date.ngay <= soNgayTrongThang(date.thang, date.nam);
}

Date ngayHienTaiHeThong() {
    std::time_t t = std::time(nullptr);
    std::tm *now = std::localtime(&t);
    if (!now) throw std::runtime_error("Khong doc duoc ngay he thong");
    return Date{now->tm_mday, now->tm_mon + 1, now->tm_year + 1900};
}

// Ngày trả {0,0,0} chưa có giá trị: hiển thị trống, không tính khoảng cách.
std::string dinhDangNgay(Date date) {
    if (date.ngay == 0 && date.thang == 0 && date.nam == 0) return "--/--/----";
    if (!ngayHopLe(date)) throw std::invalid_argument("Ngay khong hop le");
    std::ostringstream out;
    out << std::setfill('0') << std::setw(2) << date.ngay << '/'
        << std::setw(2) << date.thang << '/' << std::setw(4) << date.nam;
    return out.str();
}
