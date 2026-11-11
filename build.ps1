param([string]$OutputPath = 'QuanLyThuVien.exe')
$ErrorActionPreference = 'Stop'
Push-Location $PSScriptRoot
$stagingPath = $null
try {
    $targetPath = [System.IO.Path]::GetFullPath((Join-Path $PSScriptRoot $OutputPath))
    $targetDirectory = Split-Path -Parent $targetPath
    New-Item -ItemType Directory -Force -Path $targetDirectory | Out-Null
    # BiÃªn dá»‹ch sang tÃªn má»›i; chá»‰ thay executable Ä‘ang dÃ¹ng sau khi g++ thÃ nh cÃ´ng.
    $stagingPath = Join-Path $targetDirectory ([guid]::NewGuid().ToString('N') + '.building.exe')
    $modules = @(Get-ChildItem DauSach,DanhMucSach,DocGia,MuonTra,Date,Server,LuuTru -Filter '*.cpp' |
        ForEach-Object { $_.FullName })
    & g++ -std=c++11 -Wall -Wextra -Wpedantic -Werror -finput-charset=UTF-8 -fexec-charset=UTF-8 main.cpp $modules -lws2_32 -ladvapi32 -o $stagingPath
    if ($LASTEXITCODE -ne 0) { throw "g++ exited with code $LASTEXITCODE" }
    Move-Item -LiteralPath $stagingPath -Destination $targetPath -Force
    $stagingPath = $null
    Write-Output "Build OK: $targetPath"
} catch {
    [Console]::Error.WriteLine("[BUILD FAILED] $($_.Exception.Message)")
    [Console]::Error.WriteLine('If the executable is currently running, exit it normally and build again.')
    [Console]::Error.WriteLine('Any existing executable is the previous successful build, not the current source.')
    exit 1
} finally {
    if ($stagingPath -and (Test-Path -LiteralPath $stagingPath)) {
        Remove-Item -LiteralPath $stagingPath -Force
    }
    Pop-Location
}
