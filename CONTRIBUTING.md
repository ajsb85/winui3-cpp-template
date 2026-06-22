# Contributing to WinUI 3 C++/WinRT Modern App Template

Thank you for your interest in contributing! We welcome contributions that make
this template more helpful for C++/WinRT developers.

Please review these guidelines before submitting a pull request. If you're using an
AI coding agent, see [`CLAUDE.md`](CLAUDE.md) for a condensed rule set.

## 🧰 Prerequisites

- **Visual Studio 2022 or newer** with *Desktop development with C++* and C++/WinRT support.
- The **Windows 11 SDK** (targets `WindowsTargetPlatformVersion 10.0`, min `10.0.17763.0`).
- Windows App SDK NuGet packages — restored automatically on build.
- CLI-only contributors need just **MSBuild** and **NuGet** on `PATH`.

## 📐 Design & Coding Guidelines

To maintain code quality and build reliability, adhere to the following standards.

### 1. File Organization (Flat XAML Views)
- **Do not place XAML files in nested subdirectories** (e.g. `Views/HomePage.xaml`).
- Because of how the C++/WinRT compilation targets generate projections, nested XAML
  files cause header include-path mismatches during MSBuild. Keep all XAML pages in
  the **root project directory** alongside their `.idl`, `.h`, and `.cpp` files.

### 2. Modern Fluent Styling
- **Never use hardcoded hex or RGB colors** for UI controls.
- Use Windows theme resources (e.g. `{ThemeResource ApplicationForegroundThemeBrush}`,
  `{ThemeResource CardBackgroundFillColorDefaultBrush}`,
  `{ThemeResource DividerStrokeColorDefaultBrush}`) so the UI switches between Light
  and Dark mode automatically.

### 3. Native Coroutines & UI Thread Safety
- Do not perform heavy work, network operations, or disk I/O on the UI thread.
- Switch to background threads using `co_await winrt::resume_background()`.
- Return to the UI thread using WIL's `wil::resume_foreground()` with the page
  dispatcher (e.g. `co_await wil::resume_foreground(DispatcherQueue())`).
- Qualify async return types as `winrt::Windows::Foundation::IAsyncAction` or
  `winrt::Windows::Foundation::IAsyncOperation<T>`.
- `HomePage::ProcessDataAsync` is the canonical reference implementation.

### 4. Code Structuring & IDL Files
- Every class instantiated or declared in XAML (custom Pages/Controls) must have a
  corresponding `.idl` file declaring its WinRT runtime class, and must be registered
  in the `.vcxproj` item groups (`<Page>`, `<Midl>`, `<ClInclude>`, `<ClCompile>`).
- In implementation source files, follow this include pattern:
  ```cpp
  #include "pch.h"
  #include "MyPage.xaml.h"
  #if __has_include("MyPage.g.cpp")
  #include "MyPage.g.cpp"
  #endif
  ```
- Add new WinRT `#include`s to `pch.h` rather than to individual translation units.

### 5. Style Conventions
- **English** for all code comments and UI strings.
- 4-space indentation; keep implementations in
  `namespace winrt::HelloWorldWinUI3::implementation { … }`.
- Target **C++20** (the project selects `stdcpp20` on VS 18+).
- Commit **source only** — never commit `x64/`, `Generated Files/`, `packages/`,
  or `.vs/` (all git-ignored).

## 💻 Development Commands (Build, Run, Lint, Test)

Run from PowerShell or a VS Developer Command Prompt. Always target the **x64**
platform. If you renamed the project with `Rename-WinUIProject.ps1`, substitute your
new name for `HelloWorldWinUI3` below.

### 1. Restore Dependencies
```powershell
nuget restore HelloWorldWinUI3.sln
```

### 2. Build the Solution
```powershell
msbuild HelloWorldWinUI3.sln /p:Configuration=Debug /p:Platform=x64 /t:Rebuild
```

### 3. Run the Application
```powershell
.\x64\Debug\HelloWorldWinUI3\HelloWorldWinUI3.exe
```

### 4. Lint and Static Analysis
```powershell
msbuild HelloWorldWinUI3.sln /p:RunCodeAnalysis=true /p:Configuration=Debug /p:Platform=x64
```

The project compiles at **warning level `/W4` (Level4)** with `/bigobj`. Warnings are
not currently promoted to errors, but **all pull requests must build warning-free** —
treat any new warning as a defect.

### 5. Testing (Manual Runtime Checklist)
There is no automated test suite. Before submitting, launch the app and verify:
- The window background renders with **Mica** material.
- Dragging the **custom title bar** moves the window; Min/Max/Close blend with Mica.
- Navigating between **Home** and **Settings** in the `NavigationView` works.
- The Home page's **"Process Data (Async)"** button completes without freezing or
  crashing the UI (exercises the background → UI-thread hop).

## 🌿 Branching & Commits

- Branch off `main` using a descriptive prefix, e.g. `feature/…`, `fix/…`, `docs/…`.
- Write clear, imperative commit messages (e.g. "Add dark-theme toggle to Settings").
- Keep commits focused; separate unrelated changes.

## ✅ Pull Request Checklist

Before opening a PR, confirm:

- [ ] Clean `/t:Rebuild` succeeds on **x64** with **no warnings**.
- [ ] Static analysis (`/p:RunCodeAnalysis=true`) reports nothing new.
- [ ] The manual runtime checklist above passes.
- [ ] New XAML classes have matching `.idl` files and `.vcxproj` entries.
- [ ] No hardcoded colors; theme resources used throughout.
- [ ] No build artifacts committed.

## 🛠️ How to Contribute

1. **Fork** the repository and clone your fork.
2. **Create a feature branch** off `main`.
3. **Make your changes** following the guidelines above.
4. **Build & test**: clean build, run the app, walk the runtime checklist.
5. **Open a Pull Request** describing your changes, design decisions, and how you
   validated them.
