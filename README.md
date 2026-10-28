# foo_tag_sync - a foobar2000 component for synchronizing custom metadata with external key-value store.

## Building

Prerequisites (Windows): Visual Studio 2026 with the "Desktop development with C++" workload (v145 toolset), NuGet (`nuget.exe`), PowerShell 5.1+.

### 1. Bootstrap the SDK layout

The solution references the foobar2000 SDK via relative paths. Run from the repository root:

```
.\bootstrap.ps1
```

The script unpacks the pinned SDK (`vendor\SDK-2015-08-03.zip`) and retargets its projects to the v145 toolset. The resulting layout (relative to the repository):

```
<repo-parent-parent>\
|-- pfc\
`-- foobar2000\
    |-- ATLHelpers\, SDK\, helpers\, foobar2000_component_client\, shared\
    `-- foo_tag_sync\   (junction to this repository if it was cloned elsewhere)
```

The script is idempotent; use `.\bootstrap.ps1 -Force` to re-extract the SDK.

### 2. Restore NuGet packages and build

```
nuget.exe restore foo_tag_sync.sln -PackagesDirectory src\packages
msbuild foo_tag_sync.sln /p:Configuration=Release /p:Platform=x86 /m
```

(or open `foo_tag_sync.sln` from the junction path and build `Release|x86`).

The resulting `foo_tag_sync.dll` is placed in `artifacts\<Configuration>\`. Drop it into the foobar2000 `components` folder and restart the player.

### CI

GitHub Actions (`.github/workflows/build.yml`) builds `Release|Win32` on every push to master and pull request, and publishes `foo_tag_sync.dll` as a workflow artifact.

## Pinned SDK

`vendor\SDK-2015-08-03.zip`, SHA256 `ff1a23c4e9b96b5dc7135478281f029530d84d732c3cb5b39342237d0a9df251` (from https://www.foobar2000.org/SDK). The SDK is redistributed for foobar2000 component development purposes per its license.
