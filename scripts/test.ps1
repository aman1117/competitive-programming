[CmdletBinding()]
param([string]$Compiler = 'g++')
$ErrorActionPreference = 'Stop'
$compilerCommand = Get-Command -Name $Compiler -CommandType Application -ErrorAction Stop | Select-Object -First 1
$root = Split-Path -Parent $PSScriptRoot
$buildDirectory = Join-Path $root '.build'
New-Item -ItemType Directory -Path $buildDirectory -Force | Out-Null
$executable = Join-Path $buildDirectory ("tests-{0}.exe" -f [guid]::NewGuid().ToString('N'))
$flags = @('-std=c++17', '-O2', '-Wall', '-Wextra', '-Wshadow', '-Werror', '-I', (Join-Path $root 'include'))
try {
    # Each header must compile by itself, not rely on umbrella include order.
    foreach ($header in Get-ChildItem -LiteralPath (Join-Path $root 'include\cp') -Filter '*.hpp') {
        '' | & $compilerCommand.Source @flags '-x' 'c++' '-fsyntax-only' '-include' $header.FullName '-'
        if ($LASTEXITCODE -ne 0) { throw "Header failed: $($header.Name)" }
    }
    $starters = @((Join-Path $root 'main.cpp')) + @(
        Get-ChildItem -LiteralPath (Join-Path $root 'templates') -Filter '*.cpp' | Select-Object -ExpandProperty FullName
    )
    & $compilerCommand.Source @flags '-fsyntax-only' @starters
    if ($LASTEXITCODE -ne 0) { throw 'A submission starter failed to compile.' }
    & $compilerCommand.Source @flags '-DLOCAL' '-fsyntax-only' @starters
    if ($LASTEXITCODE -ne 0) { throw 'A LOCAL starter failed to compile.' }
    foreach ($test in Get-ChildItem -LiteralPath (Join-Path $root 'tests') -Filter '*_test.cpp' | Sort-Object Name) {
        & $compilerCommand.Source @flags $test.FullName '-o' $executable
        if ($LASTEXITCODE -ne 0) { throw "Test failed to compile: $($test.Name)" }
        & $executable
        if ($LASTEXITCODE -ne 0) { throw "$($test.Name) failed (exit $LASTEXITCODE)." }
    }
    $previousCompiler = $env:CXX
    try {
        $env:CXX = $compilerCommand.Source
        & python -m unittest discover -s (Join-Path $root 'tests') -p 'test_*.py'
        if ($LASTEXITCODE -ne 0) { throw 'Practice workflow tests failed.' }
    } finally {
        $env:CXX = $previousCompiler
    }
} finally {
    if (Test-Path -LiteralPath $executable) { Remove-Item -LiteralPath $executable -Force }
}
