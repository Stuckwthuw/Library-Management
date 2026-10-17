#include "../Date.h"
#include <stdexcept>

// Quy ngày Gregory thành số ngày nguyên O(1), tránh sai số giờ/múi giờ/DST.
long long soNgayTuyetDoi(Date date) {
    if (!ngayHopLe(date)) throw std::invalid_argument("Ngay khong hop le");
    const long long a = (14 - date.thang) / 12;
    const long long y = date.nam + 4800 - a;
    const long long m = date.thang + 12 * a - 3;
    return date.ngay + (153*m+2)/5 + 365*y + y/4 - y/100 + y/400 - 32045;
}

// Trả den - tu; quá hạn khi kết quả > MAX_NGAY_MUON, đúng 7 ngày chưa quá hạn.
int khoangCachNgay(Date tu, Date den) {
    return static_cast<int>(soNgayTuyetDoi(den) - soNgayTuyetDoi(tu));
}

// Dùng độ dài thực của tháng, không cộng cố định 28 khi lùi qua tháng trước.
// Chặn ngày ngoài miền trước vòng lặp, kể cả soNgay bằng INT_MIN/INT_MAX.
Date congNgay(Date date, int soNgay) {
    long long target = soNgayTuyetDoi(date) + soNgay;
    if (target < soNgayTuyetDoi(Date{1,1,1900}) ||
        target > soNgayTuyetDoi(Date{31,12,9999}))
        throw std::out_of_range("Ngay vuot pham vi");
    while (soNgay > 0) {
        if (++date.ngay > soNgayTrongThang(date.thang, date.nam)) {
            date.ngay = 1;
            if (++date.thang > 12) { date.thang = 1; ++date.nam; }
        }
        --soNgay;
    }
    while (soNgay < 0) {
        if (--date.ngay == 0) {
            if (--date.thang == 0) { date.thang = 12; --date.nam; }
            date.ngay = soNgayTrongThang(date.thang, date.nam);
        }
        ++soNgay;
    }
    return date;
}
