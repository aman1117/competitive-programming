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
    [switch]$Diagnostic,
    [ValidateRange(0, 86400)]
    [double]$TimeoutSeconds = 0,
    [string]$Compiler = 'g++'
)

$ErrorActionPreference = 'Stop'
$python = Get-Command python -CommandType Application -ErrorAction Stop | Select-Object -First 1
$mode = if ($Diagnostic) { 'diagnostic' } elseif ($Local) { 'debug' } else { 'release' }
$arguments = @(
    (Join-Path $PSScriptRoot 'cp_workflow.py'), 'run', $Source,
    '--compiler', $Compiler, '--mode', $mode,
    '--timeout', $TimeoutSeconds.ToString([System.Globalization.CultureInfo]::InvariantCulture)
)
if ($InputFile) { $arguments += @('--input', $InputFile) }
& $python.Source @arguments
if ($LASTEXITCODE -ne 0) { throw "C++ compile/run failed (exit $LASTEXITCODE). See the error above." }
