param(
    $path,
    $patch
)

$ErrorActionPreference="Stop"
$dest = "tools"

if ((Test-Path $dest) -ne $true){
    New-Item $dest -ItemType Directory | Out-Null
}

#####################################################################
# patch
#####################################################################
if (-ne Test-Path "$dest\bin\patch.exe") {
    $url = "http://gnuwin32.sourceforge.net/downlinks/patch-bin-zip.php"
    $output = "$dest\patch.zip"

    "Downloading patch binary" | Write-Host -ForegroundColor DarkGreen
    Start-BitsTransfer -Source $url -Destination $output

    "Extracting patch binary" | Write-Host -ForegroundColor DarkGreen
    Expand-Archive -Path $output -DestinationPath $dest
@"
<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
<assembly xmlns="urn:schemas-microsoft-com:asm.v1" manifestVersion="1.0">
<trustInfo xmlns="urn:schemas-microsoft-com:asm.v3">
    <security>
    <requestedPrivileges>
        <!-- Make sure that UAC believes
        that it does not require administrative privilege -->
        <requestedExecutionLevel level="asInvoker" uiAccess="false"/>
    </requestedPrivileges>
    </security>
</trustInfo>
</assembly>
"@ | Out-File -FilePath $dest\bin\patch.exe.manifest -Encoding utf8
    (Get-ChildItem $dest\bin\patch.exe).LastWriteTime = Get-Date
}

Get-Content $patch | & $dest\bin\patch.exe -d $path