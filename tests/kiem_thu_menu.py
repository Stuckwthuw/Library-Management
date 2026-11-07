"""Kiểm tra luồng console qua stdin; dữ liệu chỉ nằm trong thư mục test tạm."""
from datetime import date, timedelta
from pathlib import Path
import subprocess
import sys

exe = Path(sys.argv[1]).resolve()
base = Path(sys.argv[2]).resolve() / "menu"
base.mkdir()


def case(name, readers=None, books=None):
    folder = base / name
    (folder / "Data").mkdir(parents=True)
    if readers is not None:
        (folder / "Data/DocGia.txt").write_text(readers, encoding="utf-8")
    if books is not None:
        (folder / "Data/DauSach.txt").write_text(books, encoding="utf-8")
    return folder


def run(folder, text, code=0):
    result = subprocess.run([str(exe)], input=text.encode("utf-8"), capture_output=True,
                            cwd=folder, timeout=15)
    output = (result.stdout + result.stderr).decode("utf-8", errors="replace")
    assert result.returncode == code, (folder, result.returncode, output)
    return output


books = "DAUSACH_V1 | 1 | AUDIT | Sach kiem thu | 100 | Nguyen Van An | 2026 | Tin hoc | 4"
for i in range(1, 5):
    books += f" | AUDIT_{i} | 0 | Ke A1"
books += "\n"
reader = "50000,Nguyen Van,An,Nam,1\n0\n"

# Khởi động mới, EOF và thoát ở menu con đều kết thúc hữu hạn, tạo cặp file hợp lệ.
fresh = case("fresh")
run(fresh, "")
assert (fresh / "Data/DocGia.txt").read_text().startswith("DOCGIA_V3 | 0 | 0")
run(fresh, "a\n0\n0\nY\n")

# Sai số được nhập lại; thêm sách/bản sao lưu được ngay trong luồng thật.
created = case("create")
output = run(created, "c\n1\n cs101 \nCau truc du lieu\nnguyen van an\nTin hoc\n12abc\n100\n2026\nY\nY\n2\nKe A1\nY\n")
assert "[LOI] Nhap mot so nguyen" in output
assert "CS101_2 | 0 | Ke A1" in (created / "Data/DauSach.txt").read_text(encoding="utf-8")
run(created, "a\n1\nđẶNG văn\nánh\n2\nY\n")
assert "Đặng Văn | Ánh | Nu" in (created / "Data/DocGia.txt").read_text(encoding="utf-8")
output = run(created, "a\n4\n2\nĐẶNG\n")
assert "Đặng Văn Ánh" in output

# Mượn -> trả -> mượn -> báo mất -> xóa thẻ: Top 10 vẫn đếm hai lượt sau mở lại.
flow = case("flow", reader, books)
run(flow, "f\n50000\naudit_1\nY\ng\n1\n50000\nAUDIT_1\nY\nf\n50000\nAUDIT_1\nY\ng\n2\n50000\nAUDIT_1\nY\na\n3\n50000\nY\n")
assert (flow / "Data/DocGia.txt").read_text().startswith("DOCGIA_V3 | 0 | 2 |")
assert "AUDIT_1 | 2 | Ke A1" in (flow / "Data/DauSach.txt").read_text()
output = run(flow, "j\n")
assert "Sach kiem thu" in output and "| 2 " in output

# N không thực hiện giao dịch. Đóng đầu vào giữa lúc nhập không lặp vô tận.
cancel = case("cancel", reader, books)
run(cancel, "f\n50000\nAUDIT_1\nN\na\n1\nNguyen\n")
assert "AUDIT_1 | 0 | Ke A1" in (cancel / "Data/DauSach.txt").read_text()

# File lỗi bị từ chối và giữ nguyên; một file thiếu không bị coi là thư viện mới.
bad = case("bad", "BROKEN\n", books)
run(bad, "", code=1)
assert (bad / "Data/DocGia.txt").read_text() == "BROKEN\n"
missing = case("missing", reader)
run(missing, "", code=1)
assert not (missing / "Data/DauSach.txt").exists()

# Phân trang kết quả dài và ngày mượn thật trên bảng quá hạn.
borrowed = date.today() - timedelta(days=8)
readers = ""
many_books = "DAUSACH_V1 | 1 | AUDIT | Sach kiem thu | 100 | Nguyen Van An | 2026 | Tin hoc | 25"
for i in range(1, 26):
    readers += f"{50000+i},Nguyen Van,An,Nam,1\n1\nAUDIT_{i} {borrowed.day} {borrowed.month} {borrowed.year} 0 0 0 0\n"
    many_books += f" | AUDIT_{i} | 1 | Ke A1"
pages = case("pages", readers, many_books + "\n")
output = run(pages, "b\n1\ni\ne\nSach\n1\n0\n")
assert "Trang 3 / 3" in output and "AUDIT_25" in output
assert borrowed.strftime("%d/%m/%Y") in output and "(qua han)" not in output

print("PASS: menu, EOF, nhap so, Unicode, huy, muon/tra/mat/xoa, khoi dong, phan trang.")
