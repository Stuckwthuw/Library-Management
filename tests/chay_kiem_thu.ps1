$ErrorActionPreference = 'Stop'
$repo = Split-Path $PSScriptRoot -Parent
$run = Join-Path ([System.IO.Path]::GetTempPath()) ('library-tests-' + [guid]::NewGuid().ToString('N'))
New-Item -ItemType Directory -Path $run | Out-Null
$flags = @('-std=c++11', '-Wall', '-Wextra', '-Wpedantic', '-Werror', '-finput-charset=UTF-8', '-fexec-charset=UTF-8')
Push-Location $repo
try {
    $core = @(Get-ChildItem DauSach,DanhMucSach,Date -Filter '*.cpp' | ForEach-Object { $_.FullName })
    $all = @($core) + @(Get-ChildItem DocGia,MuonTra,UI,LuuTru -Filter '*.cpp' | ForEach-Object { $_.FullName })
    $jobs = @(
        @{ Name = 'core'; Source = 'DauSach/tests/kiem_thu_core.cpp'; Modules = $core },
        @{ Name = 'file'; Source = 'DauSach/tests/kiem_thu_file.cpp'; Modules = $core },
        @{ Name = 'integration'; Source = 'tests/kiem_thu_tich_hop.cpp'; Modules = $all },
        @{ Name = 'library'; Source = 'main.cpp'; Modules = $all }
    )
    foreach ($job in $jobs) {
        $exe = Join-Path $run ($job.Name + '.exe')
        & g++ @flags $job.Source $job.Modules -o $exe
        if ($LASTEXITCODE -ne 0) { throw ('Build failed: ' + $job.Name) }
        if ($job.Name -ne 'library') {
            $caseDir = Join-Path $run $job.Name
            New-Item -ItemType Directory -Path $caseDir | Out-Null
            Push-Location $caseDir
            try {
                & $exe
                if ($LASTEXITCODE -ne 0) { throw ('Test failed: ' + $job.Name) }
            } finally { Pop-Location }
        }
    }
    & python tests/kiem_thu_menu.py (Join-Path $run 'library.exe') $run
    if ($LASTEXITCODE -ne 0) { throw 'Menu tests failed' }
    Write-Output ('PASS: all builds and tests. Artifacts: ' + $run)
} finally { Pop-Location }
