param(
    $pcre="10.32"
)

$ErrorActionPreference="Stop"
$dest = "bin"

if (Test-Path $dest){
    Remove-Item $dest -Recurse -Force -Confirm:$false
}

New-Item $dest -ItemType Directory -ErrorAction SilentlyContinue | Out-Null

#####################################################################
# pcre
#####################################################################
$url = "https://github.com/PCRE2Project/pcre2/releases/download/pcre2-$($pcre)/pcre2-$($pcre).zip"
$output = "$dest\pcre-$($pcre).zip"

"Downloading pcre2 v$pcre sources" | Write-Host -ForegroundColor DarkGreen
Start-BitsTransfer -Source $url -Destination $output

"Extracting pcre2 v$pcre sources" | Write-Host -ForegroundColor DarkGreen
Expand-Archive -Path $output -DestinationPath $dest
Rename-Item -Path "$dest\pcre2-$($pcre)" -NewName "pcre2"

if(Test-Path "pcre-$($pcre).patch") {
    "Applying pcre patches" | Write-Host -ForegroundColor DarkGreen
    .\patch.ps1 -patch "pcre-$($pcre).patch" -path $dest\pcre2
}
