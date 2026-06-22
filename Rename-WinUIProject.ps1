param(
    [Parameter(Mandatory=$true)]
    [string]$NewName
)

# Validate name (must be a valid C++ identifier)
if ($NewName -notmatch '^[a-zA-Z_][a-zA-Z0-9_]*$') {
    Write-Error "Invalid project name. It must be a valid C++ identifier/namespace (letters, numbers, underscores, starting with a letter/underscore)."
    exit 1
}

$oldName = "HelloWorldWinUI3"
$oldGuid = "{A1B2C3D4-E5F6-7890-ABCD-EF1234567890}"
$newGuid = "{" + [System.Guid]::NewGuid().ToString().ToUpper() + "}"

Write-Host "Renaming project from '$oldName' to '$NewName'..."
Write-Host "Generating new Project GUID: $newGuid"

# 1. Process and rename the solution file
if (Test-Path "$oldName.sln") {
    $slnContent = Get-Content "$oldName.sln" -Raw
    $slnContent = $slnContent -replace $oldName, $NewName
    $slnContent = $slnContent -replace [regex]::Escape($oldGuid), $newGuid
    $slnContent | Set-Content "$NewName.sln"
    Remove-Item "$oldName.sln" -Force
    Write-Host "Renamed solution file to $NewName.sln"
}

# 2. Process files inside the project directory
if (Test-Path $oldName -PathType Container) {
    # First, replace strings inside all source files in the project folder
    $filesToProcess = Get-ChildItem -Path $oldName -File -Recurse
    foreach ($file in $filesToProcess) {
        # Skip binary files if any are added in the future
        if ($file.Extension -match '\.(png|ico|jpg|gif|dll|exe|pri|xbf|obj|pdb)$') {
            continue
        }
        
        $content = Get-Content $file.FullName -Raw
        $content = $content -replace $oldName, $NewName
        if ($file.Name -like "*.vcxproj") {
            $content = $content -replace [regex]::Escape($oldGuid), $newGuid
        }
        $content | Set-Content $file.FullName -Force
        Write-Host "Updated references in file: $($file.FullName)"
    }

    # Second, rename files themselves that contain the old project name
    Get-ChildItem -Path $oldName -File -Filter "*$oldName*" | ForEach-Object {
        $newNameFile = $_.Name -replace $oldName, $NewName
        Rename-Item -Path $_.FullName -NewName $newNameFile -Force
        Write-Host "Renamed file: $($_.Name) -> $newNameFile"
    }

    # Finally, rename the project folder itself
    Rename-Item -Path $oldName -NewName $NewName -Force
    Write-Host "Renamed project directory to $NewName"
}

Write-Host "=========================================================="
Write-Host "Renaming complete! Folder structure and GUIDs updated."
Write-Host "Open '$NewName.sln' in Visual Studio and rebuild."
Write-Host "=========================================================="
