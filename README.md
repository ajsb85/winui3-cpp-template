# WinUI 3 C++/WinRT Modern App Template

A lean, modular **C++/WinRT** starting point for building native **WinUI 3** desktop
apps on the **Windows App SDK**. It ships with the pieces every modern Windows 11
app needs — a Mica backdrop, a custom extended title bar, multi-page navigation,
and a correct async/threading pattern — so you can delete the demo content and
start shipping features.

<p>
  <img alt="Platform" src="https://img.shields.io/badge/platform-Windows%2011-0078D6?logo=windows11&logoColor=white">
  <img alt="Language" src="https://img.shields.io/badge/C%2B%2B-20-00599C?logo=cplusplus&logoColor=white">
  <img alt="Framework" src="https://img.shields.io/badge/WinUI-3-512BD4">
  <img alt="Windows App SDK" src="https://img.shields.io/badge/Windows%20App%20SDK-2.2.0-512BD4">
  <img alt="Arch" src="https://img.shields.io/badge/arch-x64-blue">
  <img alt="License" src="https://img.shields.io/badge/license-MIT-green">
</p>

> Built to follow modern Windows 11 Fluent Design guidelines and C++/WinRT best
> practices, with no XAML islands, no MFC, and no managed code.

## ✨ Features

- **🎨 Mica backdrop** — native `MicaBackdrop Kind="Base"` declared directly in XAML, with automatic fallback on older hosts (Windows 10, battery saver, transparency off). No controller code required.
- **🖥️ Custom extended title bar** — content extends into the title bar; a draggable region with a Fluent glyph + caption adapts to Light/Dark automatically, and the native Min/Max/Close buttons blend seamlessly with Mica.
- **🧭 Multi-page navigation** — `NavigationView` shell with a `Frame` host and ready-made **Home** and **Settings** pages (Settings wired through the built-in settings item).
- **⚡ Correct async pattern** — C++ coroutines (`co_await` / `co_return`) that offload work with `winrt::resume_background()` and marshal back to the UI thread with WIL's `wil::resume_foreground(DispatcherQueue())`.
- **🌗 Theme-safe styling** — zero hardcoded colors; everything uses WinUI theme resources so Light/Dark switching is free.
- **📦 One-command renaming** — `Rename-WinUIProject.ps1` rewrites the namespace, filenames, and project GUID to bootstrap a new app.
- **🧰 Unpackaged by default** — `WindowsPackageType=None`; runs as a plain `.exe` with the Windows App SDK runtime auto-bootstrapped.

## 🛠️ Prerequisites

- **Visual Studio 2022 or newer** with:
  - *Desktop development with C++*
  - *C++/WinRT* support (the VSIX or the CppWinRT NuGet package — already referenced here)
- **Windows 10 1809 (10.0.17763)** or later at runtime; built against the Windows 11 SDK.
- Windows App SDK NuGet packages — **restored automatically** on build.

> CLI builds also work with just **MSBuild + NuGet** (no full IDE required). See
> [Development Commands](#-development-commands).

## 🚀 Quick Start

### Use it as a template

```powershell
# 1. Clone
git clone https://github.com/ajsb85/winui3-cpp-template.git
cd winui3-cpp-template

# 2. (Optional) Rename to your app
.\Rename-WinUIProject.ps1 -NewName "YourAppName"

# 3. Restore + build (x64)
nuget restore HelloWorldWinUI3.sln
msbuild HelloWorldWinUI3.sln /p:Configuration=Debug /p:Platform=x64 /t:Rebuild

# 4. Run
.\x64\Debug\HelloWorldWinUI3\HelloWorldWinUI3.exe
```

Or just open `HelloWorldWinUI3.sln` in Visual Studio, set the platform to **x64**,
and press **F5**.

> [!NOTE]
> After running `Rename-WinUIProject.ps1`, replace `HelloWorldWinUI3` with your new
> name in all of the commands above.

## 🧩 Architecture

```
App.OnLaunched
   └─ MainWindow                 ← shell window
        ├─ MicaBackdrop (XAML)   ← base material
        ├─ Custom title bar      ← ExtendsContentIntoTitleBar + SetTitleBar (code-behind)
        └─ NavigationView
             └─ Frame            ← navigates between pages
                  ├─ HomePage    ← async demo (background → UI thread hop)
                  └─ SettingsPage
```

Key design decisions:

| Concern | Approach |
|--------|----------|
| Backdrop | `MicaBackdrop` in XAML; SDK handles fallback. |
| Title bar | Drawn in XAML, **wired in C++** (`ExtendsContentIntoTitleBar`, `SetTitleBar`, `AppWindowTitleBar` button colors) since those APIs are code-only. |
| Theming | Theme resources only — no hardcoded hex/RGB. |
| Threading | `resume_background()` for work, `wil::resume_foreground()` to return to UI. |
| Packaging | Unpackaged (`WindowsPackageType=None`), PerMonitorV2 DPI. |

## 📁 Project Structure

```text
winui3-cpp-template/
├── HelloWorldWinUI3.sln           # Visual Studio solution
├── Rename-WinUIProject.ps1        # Namespace/file/GUID renaming utility
├── README.md
├── CONTRIBUTING.md                # Coding guidelines & dev commands
├── CLAUDE.md                      # Guidance for AI coding agents
└── HelloWorldWinUI3/              # Primary project (flat — no nested views)
    ├── App.xaml[.h/.cpp/.idl]     # App entry point & resources
    ├── MainWindow.xaml[.h/.cpp/.idl]   # Shell: NavigationView + custom title bar
    ├── HomePage.xaml[.h/.cpp/.idl]     # Demo page with async action
    ├── SettingsPage.xaml[.h/.cpp/.idl] # Settings page
    ├── XamlMetaDataProvider.*     # XAML type metadata plumbing
    ├── pch.h / pch.cpp            # Precompiled headers (WinRT namespaces + WIL)
    ├── app.manifest               # Unpackaged identity, Win10+ compat, DPI
    └── packages.config            # NuGet packages (Windows App SDK 2.2.0, etc.)
```

> XAML pages are kept **flat** in the project root by design — nesting them breaks
> C++/WinRT projection include paths. See `CONTRIBUTING.md` for the rationale.

## 💻 Development Commands

All commands target **x64** from PowerShell or a VS Developer Prompt.

| Task | Command |
|------|---------|
| Restore | `nuget restore HelloWorldWinUI3.sln` |
| Build | `msbuild HelloWorldWinUI3.sln /p:Configuration=Debug /p:Platform=x64 /t:Rebuild` |
| Lint / analysis | `msbuild HelloWorldWinUI3.sln /p:RunCodeAnalysis=true /p:Configuration=Debug /p:Platform=x64` |
| Run | `.\x64\Debug\HelloWorldWinUI3\HelloWorldWinUI3.exe` |
| Release | `msbuild HelloWorldWinUI3.sln /p:Configuration=Release /p:Platform=x64 /t:Rebuild` |

The project builds at warning level `/W4`; keep it warning-free. There is no
automated test suite — use the manual checklist in `CONTRIBUTING.md`.

## 🧪 Tech Stack

- **WinUI 3** (Windows App SDK **2.2.0**)
- **C++/WinRT** (`Microsoft.Windows.CppWinRT`) — **C++20**
- **Windows Implementation Library (WIL)** for coroutine helpers
- **MSBuild** + classic **NuGet** (`packages.config`)

## 🐛 Troubleshooting

- **`nuget`/`msbuild` not found** — open a *Developer PowerShell for VS*, or locate MSBuild via `vswhere -latest -find MSBuild\**\Bin\MSBuild.exe`.
- **Build fails after renaming** — confirm `Rename-WinUIProject.ps1` updated the `.sln`, project folder, and GUID, then do a clean `/t:Rebuild`.
- **Stale XAML/codegen errors** — delete `x64/` and `Generated Files/` and rebuild.
- **No Mica / solid background** — expected on Windows 10, with transparency disabled, or in battery-saver mode (automatic fallback).

## 🤝 Contributing

Contributions are welcome — please read [CONTRIBUTING.md](CONTRIBUTING.md) for the
design rules (flat XAML, theme resources, coroutine/threading patterns) and the
build/run/lint/test workflow before opening a PR.

## 📜 License

This project is licensed under the MIT License — see the `LICENSE` file for details.
