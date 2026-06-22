# CLAUDE.md

Guidance for AI coding agents (Claude Code and similar) working in this repository.
Read this first — it captures the non-obvious conventions that keep the C++/WinRT
build healthy.

## What this is

A minimal, opinionated **WinUI 3 + C++/WinRT desktop app template** targeting the
**Windows App SDK 2.2.0** in **unpackaged** (`WindowsPackageType=None`) mode. It
demonstrates a Mica backdrop, a custom extended title bar, `NavigationView`-based
multi-page shell, and async work via C++ coroutines.

The default project/namespace is `HelloWorldWinUI3`. A user may rename it with
`Rename-WinUIProject.ps1 -NewName "<X>"`; after that, substitute `<X>` for
`HelloWorldWinUI3` everywhere below (solution, project folder, namespace).

## Build / run / lint / test

There is no CI and no unit-test project; "test" is a manual runtime checklist.
Run these from PowerShell or a VS Developer Prompt. Always target **x64**.

```powershell
# Restore (uses packages.config — classic NuGet, not PackageReference)
nuget restore HelloWorldWinUI3.sln

# Build (clean rebuild)
msbuild HelloWorldWinUI3.sln /p:Configuration=Debug /p:Platform=x64 /t:Rebuild

# Lint / static analysis (C++ Core Guidelines + MSVC analyzers)
msbuild HelloWorldWinUI3.sln /p:RunCodeAnalysis=true /p:Configuration=Debug /p:Platform=x64

# Run the unpackaged exe
.\x64\Debug\HelloWorldWinUI3\HelloWorldWinUI3.exe
```

- If `msbuild`/`nuget` aren't on `PATH`, resolve MSBuild via
  `vswhere -latest -find MSBuild\**\Bin\MSBuild.exe`.
- The project compiles at **`/W4` (WarningLevel Level4)** with `/bigobj`. It is
  **not** currently `/WX` (warnings-as-errors), but treat any new warning as a
  defect — keep builds warning-free.
- Language standard is **C++20** under VS 18+ (falls back to C++17 on older VS).

## Manual test checklist (no automated tests exist)

After a change, launch the app and verify:
1. Window paints with **Mica** material (translucent, picks up desktop tint).
2. The **custom title bar** drags the window; Min/Max/Close blend with Mica.
3. `NavigationView` switches between **Home** and **Settings**.
4. Home's **"Process Data (Async)"** button runs without freezing the UI and
   reports success (exercises background → UI-thread hop).

## Architecture (file map)

All source lives flat in `HelloWorldWinUI3/` (see the hard rule below).

| File | Role |
|------|------|
| `App.xaml[.h/.cpp]` | App entry; `OnLaunched` creates `MainWindow`. |
| `MainWindow.xaml[.h/.cpp]` | Shell: Mica backdrop, custom title bar, `NavigationView` + `Frame`. Title-bar wiring (`ExtendsContentIntoTitleBar`, `SetTitleBar`, button colors) is **code-behind only** — those APIs aren't available in XAML. |
| `HomePage.xaml[.h/.cpp]` | Demo page; async button handler showing the coroutine threading pattern. |
| `SettingsPage.xaml[.h/.cpp]` | Settings UI (reached via the built-in `NavigationView` settings item). |
| `*.idl` | WinRT runtime-class declarations — **required** for any XAML-instantiated class. |
| `XamlMetaDataProvider.*` | Generated XAML type metadata plumbing. |
| `pch.h` | Precompiled header with the full WinRT namespace include set + `wil/cppwinrt_helpers.h`. Add new WinRT includes here. |
| `app.manifest` | Declares Win10+ compatibility (needed for custom title bar) and PerMonitorV2 DPI. |
| `packages.config` | NuGet deps (Windows App SDK, CppWinRT, WIL, SDK BuildTools). |
| `Rename-WinUIProject.ps1` | Renames project/namespace/files and regenerates the project GUID. |

## Hard rules (these break the build or theming if violated)

1. **Keep XAML flat.** Do not put `.xaml` in nested folders (e.g. `Views/`).
   C++/WinRT projection/codegen generates header include paths that assume the
   page's `.idl/.h/.cpp` sit beside it in the project root. Nesting causes
   MSBuild include-path mismatches.
2. **No hardcoded colors.** Use theme resources
   (`{ThemeResource ApplicationForegroundThemeBrush}`,
   `{ThemeResource CardBackgroundFillColorDefaultBrush}`,
   `{ThemeResource DividerStrokeColorDefaultBrush}`, …) so Light/Dark switch
   automatically.
3. **Every XAML-instantiated class needs an `.idl`** declaring its runtime class.
   Add the new page to the `.vcxproj` (`<Page>` / `<Midl>` / `<ClInclude>` /
   `<ClCompile>` item groups) alongside the existing pages.
4. **Implementation files use the generated-header guard:**
   ```cpp
   #include "pch.h"
   #include "MyPage.xaml.h"
   #if __has_include("MyPage.g.cpp")
   #include "MyPage.g.cpp"
   #endif
   ```
5. **Don't block the UI thread.** Offload with `co_await winrt::resume_background()`,
   return to UI with `co_await wil::resume_foreground(DispatcherQueue())`. Type
   async methods as `winrt::Windows::Foundation::IAsyncAction` /
   `IAsyncOperation<T>`. See `HomePage::ProcessDataAsync` for the canonical shape.

## Conventions

- Code comments and UI strings are **English**.
- 4-space indentation; `namespace winrt::HelloWorldWinUI3::implementation { … }`.
- Add new WinRT `#include`s to `pch.h`, not individual translation units.
- Commit only source — build outputs (`x64/`, `Generated Files/`, `packages/`,
  `.vs/`) are git-ignored.

## Gotchas

- Generated files under `Generated Files/` and `x64/` are build artifacts; never
  edit or commit them. If projections look stale, do a `/t:Rebuild`.
- The exe is **unpackaged**; it relies on the Windows App SDK runtime being
  bootstrapped (handled by the auto-initializers linked into the project).
- `pch.h` `#undef GetCurrentTime` is intentional — it avoids the Win32 macro
  clashing with `Storyboard::GetCurrentTime`. Don't remove it.
