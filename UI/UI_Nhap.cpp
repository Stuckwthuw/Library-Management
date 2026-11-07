#include "../UI.h"
#include "../Common/Chuoi.h"
#include "../Common/LuuTru.h"
#include <cstring>
#include <iostream>
#include <sstream>

const char *FILE_DOCGIA = "Data/DocGia.txt";
const char *FILE_DAUSACH = "Data/DauSach.txt";

// Nhập chuỗi an toàn chống tràn bộ đệm
void NhapChuoiAnToan(char *buf, int maxLen, const char *prompt) {
    if (!buf || maxLen <= 0) throw std::invalid_argument("Bo dem nhap khong hop le");
    for (;;) {
        if (prompt) std::cout << prompt;
        std::string line;
        if (!std::getline(std::cin, line)) throw HetDuLieuNhap();
        if (line.size() >= static_cast<unsigned>(maxLen) || !Chuoi::hopLe(line.c_str()) ||
            line.find('\0') != std::string::npos) {
            std::cout << "[LOI] Chuoi qua dai hoac UTF-8 khong hop le. Nhap lai (toi da "
                      << maxLen - 1 << " byte): ";
            continue;
        }
        std::strcpy(buf, line.c_str());
        return;
    }
}

// Nhập số nguyên an toàn trong khoảng [minVal, maxVal]
bool NhapSoNguyenAnToan(int &val, int minVal, int maxVal, const char *prompt) {
    std::cout << prompt;
    std::string line;
    if (!std::getline(std::cin, line)) throw HetDuLieuNhap();
    std::istringstream input(line);
    int number = 0;
    if (input >> number) {
        input >> std::ws;
        if (input.eof() && number >= minVal && number <= maxVal) {
            val = number;
            return true;
        }
    }
    std::cout << "[LOI] Nhap mot so nguyen tu " << minVal << " den " << maxVal << ".\n";
    return false;
}

bool XacNhanHanhDong(const char *prompt) {
    for (;;) {
        std::cout << prompt << " (Y/N): ";
        std::string line;
        if (!std::getline(std::cin, line)) return false;
        std::istringstream input(line);
        char c = 0;
        if (input >> c) {
            input >> std::ws;
            if (input.eof()) {
                if (c == 'Y' || c == 'y') return true;
                if (c == 'N' || c == 'n') return false;
            }
        }
        std::cout << "[LOI] Chi nhap Y hoac N.\n";
    }
}

void DungManHinh() {
    if (!LaConsoleTuongTac()) return;
    std::cout << "\nNhan Enter de tiep tuc...";
    std::string line;
    if (!std::getline(std::cin, line)) throw HetDuLieuNhap();
}

// Tự động lưu dữ liệu xuống đĩa sau mỗi thay đổi
void LuuSauThayDoi(TREE_DocGia root, const DanhSachDauSach &ds) {
    std::string loi;
    while (!LuuDuLieu(root, ds, FILE_DOCGIA, FILE_DAUSACH, loi)) {
        std::cerr << "[LOI] " << loi << '\n';
        if (!XacNhanHanhDong("Thu luu lai?"))
            throw std::runtime_error("Dung phien: chua xac nhan luu thanh cong. Giu file .pending neu co de phuc hoi.");
    }
    std::cout << "[THANH CONG] Da luu dong bo doc gia va dau sach.\n";
}
