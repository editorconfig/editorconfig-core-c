param(
    [ValidateSet("pcre2","core", "all")]
    [string] $proj = "all",

    [switch] $init,

    [switch] $install,
    
    [ValidateSet("ON","OFF")]
    [string] $static = "ON",

    [ValidateSet("Debug","Release")]
    [string] $config = "Release",

    [ValidateSet("x86","x64")]
    [string] $arch = "x64",
    

    [ValidateSet("15","14","12")]
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
    .\build.ps1 -proj pcre2 -init:$init -install:$install -arch $arch -config $config -static $static
    .\build.ps1 -proj core -init:$init -install:$install -arch $arch -config $config -static $static
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

    if($arch -eq "x64"){
        $gen += " Win64"
    }

    mkdir $dest -ErrorAction SilentlyContinue | Out-Null
    Push-Location $dest
    try {
        switch ($proj) {
            pcre2 {
                $BUILD_SHARED_LIBS = "ON"
                if ($static -eq "ON"){ $BUILD_SHARED_LIBS = "OFF"}
                exec { cmake -G "$gen" -DCMAKE_INSTALL_PREFIX="$PREFIX" -DPCRE2_STATIC_RUNTIME="$static" `
                    -DBUILD_SHARED_LIBS="$BUILD_SHARED_LIBS" -DPCRE2_BUILD_PCRE2GREP="OFF" -DPCRE2_BUILD_TESTS="OFF" `
                    "../../pcre2" }
            }
            core {
                $MSVC_MD = "ON"
                if ($static -eq "ON"){ $MSVC_MD = "OFF"}
                exec { cmake -G "$gen" -DCMAKE_INSTALL_PREFIX="$PREFIX" -DMSVC_MD="$MSVC_MD" -DPCRE2_STATIC="$static" `
                    "../../../." }
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
