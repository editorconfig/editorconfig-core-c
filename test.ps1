param(
    [ValidateSet("pcre2","core")]
    [string] $proj = "core",
    
    [ValidateSet("ON","OFF")]
    [string] $static = "ON",

    [ValidateSet("Debug","Release")]
    [string] $config = "Release",

    [ValidateSet("x86","x64")]
    [string] $arch = "x64"
)

$ErrorActionPreference="stop"


$dest = "bin"

if ((Test-Path $dest) -ne $true){
    throw "Missing build path! Used init?"
}

$dest += "\$arch"

if($static -eq "ON"){
    $dest += "-static"
}


$bin = "$dest\build\bin"
$dest += "\$proj"

if ((Test-Path $dest) -ne $true){
    throw "Missing build path! Used init?"
}

$env:Path += ";" + (Resolve-Path $bin)

"Testing $proj" | Write-Host -ForegroundColor DarkGreen
Push-Location $dest
try {
    ctest -E utf_8_char -VV --output-on-failure . -C $config
} finally {
    Pop-Location
}
