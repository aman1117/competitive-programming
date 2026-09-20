<#
.SYNOPSIS
Compile a C++17 file, then run it only if compilation succeeds.
.EXAMPLE
cprun .\main.cpp -InputFile .\input.txt
.EXAMPLE
cprun .\practice\solution.cpp -Local
#>
[CmdletBinding()]
param(
    [Parameter(Position = 0)]
    [string]$Source = '.\main.cpp',
    [string]$InputFile,
    [switch]$Local,
    [string]$Compiler = 'g++'
)

$ErrorActionPreference = 'Stop'
$sourcePath = (Resolve-Path -LiteralPath $Source -ErrorAction Stop).ProviderPath
if ([System.IO.Path]::GetExtension($sourcePath) -ne '.cpp') {
    throw "Expected a .cpp file: $sourcePath"
}
$inputPath = if ($InputFile) {
    (Resolve-Path -LiteralPath $InputFile -ErrorAction Stop).ProviderPath
} else { $null }
$compilerCommand = Get-Command -Name $Compiler -CommandType Application -ErrorAction Stop
$root = Split-Path -Parent $PSScriptRoot
$buildDirectory = Join-Path $root '.build'
New-Item -ItemType Directory -Path $buildDirectory -Force | Out-Null
# A unique executable avoids collisions between files with the same name and
# concurrent terminals. Never execute a stale binary after a failed build.
$executable = Join-Path $buildDirectory ("run-{0}.exe" -f [guid]::NewGuid().ToString('N'))
$flags = @('-std=c++17', '-Wall', '-Wextra', '-Wshadow', '-I', (Join-Path $root 'include'))
if ($Local) { $flags += @('-O0', '-g', '-DLOCAL') }
else { $flags += '-O2' }

try {
    & $compilerCommand.Source @flags $sourcePath '-o' $executable
    if ($LASTEXITCODE -ne 0) {
        throw "C++ compilation failed (exit $LASTEXITCODE). Program was not run."
    }
    if ($inputPath) {
        # Native redirection preserves bytes; PowerShell's Get-Content pipeline
        # can otherwise alter encodings/newlines. Output stays in the terminal.
        $process = Start-Process -FilePath $executable -NoNewWindow -Wait -PassThru -RedirectStandardInput $inputPath
        $global:LASTEXITCODE = $process.ExitCode
        $process.Dispose()
    } else {
        & $executable
    }
    if ($LASTEXITCODE -ne 0) { throw "C++ program exited with code $LASTEXITCODE." }
} finally {
    if (Test-Path -LiteralPath $executable) {
        Remove-Item -LiteralPath $executable -Force
    }
}
