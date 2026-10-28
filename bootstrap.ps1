# Bootstraps the foobar2000 SDK layout required to build foo_tag_sync.
#
# The component's solution and project files reference the foobar2000 SDK via
# relative paths, expecting this directory layout:
#
#   <layoutRoot>\
#   |-- pfc\
#   `-- foobar2000\
#       |-- ATLHelpers\
#       |-- SDK\
#       |-- helpers\
#       |-- foobar2000_component_client\
#       |-- shared\
#       `-- foo_tag_sync\   (junction to this repository if it was cloned elsewhere)
#
# The script unpacks the pinned SDK (vendor\SDK-2015-08-03.zip) into
# <layoutRoot> (the parent of the directory containing this repository),
# retargets the vintage SDK projects (v100/v120) to the v145 toolset, and
# creates a junction when the repository was cloned outside the SDK tree.
#
# Usage:  .\bootstrap.ps1          # idempotent
#         .\bootstrap.ps1 -Force   # re-extract the SDK over the existing layout

[CmdletBinding()]
param(
    [switch]$Force
)

$ErrorActionPreference = "Stop"

$repoRoot     = $PSScriptRoot
$repoName     = Split-Path $repoRoot -Leaf
$sdkZip       = Join-Path $repoRoot "vendor\SDK-2015-08-03.zip"
$expectedHash = "ff1a23c4e9b96b5dc7135478281f029530d84d732c3cb5b39342237d0a9df251"

$layoutRoot = Split-Path (Split-Path $repoRoot)

# Relative to $layoutRoot. Normalized to v145 after extraction.
$sdkProjects = @(
    "pfc\pfc.vcxproj",
    "foobar2000\SDK\foobar2000_SDK.vcxproj",
    "foobar2000\ATLHelpers\foobar2000_ATL_helpers.vcxproj",
    "foobar2000\helpers\foobar2000_sdk_helpers.vcxproj",
    "foobar2000\foobar2000_component_client\foobar2000_component_client.vcxproj"
)

# The repository must be reachable at <layoutRoot>\foobar2000\<repoName>.
$expectedRepoPath = Join-Path $layoutRoot ("foobar2000\" + $repoName)
$repoFullPath = (Resolve-Path $repoRoot).Path
if (Test-Path $expectedRepoPath) {
    # Resolve-Path does not resolve junctions to their target, compare the link target instead.
    $item = Get-Item $expectedRepoPath -Force
    $existing = if ($item.LinkType) { "$($item.Target | Select-Object -First 1)".TrimEnd('\') } else { $item.FullName.TrimEnd('\') }
    if ($existing -ine $repoFullPath.TrimEnd('\')) {
        throw "Path $expectedRepoPath already exists and is not this repository."
    }
}
else {
    Write-Host "Creating junction $expectedRepoPath -> $repoRoot"
    New-Item -ItemType Junction -Path $expectedRepoPath -Value $repoFullPath | Out-Null
}

function Test-LayoutComplete([string]$root) {
    foreach ($rel in $sdkProjects) {
        if (-not (Test-Path (Join-Path $root $rel))) { return $false }
    }
    return (Test-Path (Join-Path $root "foobar2000\shared\shared.lib"))
}

if (-not $Force -and (Test-LayoutComplete $layoutRoot)) {
    Write-Host "SDK layout already present at $layoutRoot (use -Force to re-extract)."
}
else {
    if (-not (Test-Path $sdkZip)) { throw "SDK archive not found: $sdkZip" }

    $actualHash = (Get-FileHash -Path $sdkZip -Algorithm SHA256).Hash.ToLowerInvariant()
    if ($actualHash -ne $expectedHash) {
        throw "SHA256 mismatch for $sdkZip`n  expected: $expectedHash`n  actual:   $actualHash"
    }

    $tempDir = Join-Path ([IO.Path]::GetTempPath()) ("fb2k-sdk-" + [Guid]::NewGuid().ToString("N"))
    Write-Host "Extracting $sdkZip ..."
    Expand-Archive -Path $sdkZip -DestinationPath $tempDir

    foreach ($dir in @("pfc", "foobar2000")) {
        # Copy the archive contents (not the container itself) so that re-runs
        # merge into the existing directory instead of nesting it.
        $dst = Join-Path $layoutRoot $dir
        New-Item -ItemType Directory -Path $dst -Force | Out-Null
        Copy-Item (Join-Path $tempDir "$dir\*") $dst -Recurse -Force
    }
    Remove-Item $tempDir -Recurse -Force
}

Write-Host "Normalizing platform toolsets to v145 ..."
foreach ($rel in $sdkProjects) {
    $path = Join-Path $layoutRoot $rel
    if (-not (Test-Path $path)) { throw "Missing SDK project: $path" }
    $text = [IO.File]::ReadAllText($path)
    $patched = $text -replace "<PlatformToolset>v1[0-9][0-9]</PlatformToolset>", "<PlatformToolset>v145</PlatformToolset>"
    if ($patched -ne $text) {
        [IO.File]::WriteAllText($path, $patched)
        Write-Host "  patched $rel"
    }
}

foreach ($rel in ($sdkProjects + @("foobar2000\shared\shared.lib"))) {
    $path = Join-Path $layoutRoot $rel
    if (-not (Test-Path $path)) { throw "Bootstrap validation failed, missing: $path" }
}

Write-Host ""
Write-Host "Bootstrap complete. Solution: $expectedRepoPath\foo_tag_sync.sln"
Write-Host "Build example:"
Write-Host "  msbuild `"$expectedRepoPath\foo_tag_sync.sln`" /p:Configuration=Release /p:Platform=x86 /m"

# Expose the build directory (the junction path) to GitHub Actions.
if ($env:GITHUB_OUTPUT) {
    Add-Content -Path $env:GITHUB_OUTPUT -Value "build_dir=$expectedRepoPath"
}
