param(
    [ValidateSet("pcre2","core", "all")]
    [string] $proj = "all",

    [switch] $init,

    [switch] $install,
    
    [ValidateSet("ON","OFF")]
    [string] $static = "ON",

    [ValidateSet("Debug","Release")]
    [string] $config = "Release",

    [ValidateSet("x86","x64","arm64")]
    [string] $arch = "x64",
    

    [ValidateSet("17","16","15","14")]
    [int] $vsver = 15
)

$ErrorActionPreference="stop"

function exec
{
    param
    (
        [ScriptBlock] $ScriptBlock,
        [int[]] $AllowedExitCodes = @(0)
    )
 
    $backupErrorActionPreference = $script:ErrorActionPreference
 
    $script:ErrorActionPreference = "Continue"
    try
    {
        & $ScriptBlock
        if ($AllowedExitCodes -notcontains $LASTEXITCODE)
        {
            throw "Execution failed with exit code $LASTEXITCODE"
        }
    }
    finally
    {
        $script:ErrorActionPreference = $backupErrorActionPreference
    }
}

if ($proj -eq "all"){
    .\build.ps1 -proj pcre2 -init:$init -install:$install -vsver $vsver -arch $arch -config $config -static $static
    .\build.ps1 -proj core -init:$init -install:$install -vsver $vsver -arch $arch -config $config -static $static
    return
}

$PREFIX="../build"

$dest = "bin"

if ((Test-Path $dest) -ne $true){
    throw "Missing build path! Used init?"
}

$dest += "\$arch"

if($static -eq "ON"){
    $dest += "-static"
}

$dest += "\$proj"

if ($init) {
    "Generating $proj build files" | Write-Host -ForegroundColor DarkGreen

    $gen = "Visual Studio "

    switch ($vsver) {
        17 {
            $gen += "17 2022"
        }
        16 {
            $gen += "16 2019"
        }
        15 {
            $gen += "15 2017"
        }
        14 {
            $gen += "14 2015"
        }
        12 {
            $gen += "12 2013"
        }
        default {
            throw "Visual Studio version $vsver not supported!"
        }
    }

    $cmake_arch=""
    if($arch -eq "x64"){
        $cmake_arch += "x64"
    }
    elseif($arch -eq "x86"){
        $cmake_arch += "Win32"
    }
    elseif($arch -eq "arm64"){
        $cmake_arch += "ARM64"
    }

    mkdir $dest -ErrorAction SilentlyContinue | Out-Null
    Push-Location $dest
    try {
        switch ($proj) {
            pcre2 {
                $BUILD_SHARED_LIBS = "ON"
                if ($static -eq "ON"){ $BUILD_SHARED_LIBS = "OFF"}
                exec { cmake -G "$gen" -A $cmake_arch -DCMAKE_INSTALL_PREFIX="$PREFIX" -DPCRE2_STATIC_RUNTIME="$static" `
                    -DBUILD_SHARED_LIBS="$BUILD_SHARED_LIBS" -DPCRE2_BUILD_PCRE2GREP="OFF" -DPCRE2_BUILD_TESTS="OFF" `
                    "../../pcre2" }
            }
            core {
                $MsvcRuntime = if($static -eq "ON") { "MultiThreaded" } else { "MultiThreadedDLL" }
                exec { cmake -G "$gen" -A $cmake_arch -DCMAKE_INSTALL_PREFIX="$PREFIX" -DCMAKE_MSVC_RUNTIME_LIBRARY="$MsvcRuntime" -DPCRE2_STATIC="$static" "../../../." }
            }
        }
    } finally {
        Pop-Location
    }
}

if ((Test-Path $dest) -ne $true){
    throw "Missing build path! Used init?"
}

"Compiling $proj" | Write-Host -ForegroundColor DarkGreen

exec { cmake --build $dest `-- /p:Configuration=$config }

if ($install) {
    "Installing $proj" | Write-Host -ForegroundColor DarkGreen
    switch ($proj) {
        pcre2 {
            exec { cmake --build $dest --target install `-- /p:Configuration=$config }
        }
        core {
            exec { cmake --build $dest --target install `-- /p:Configuration=$config }
        }
    }
}
