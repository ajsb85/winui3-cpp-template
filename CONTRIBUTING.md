# Contributing to WinUI 3 C++/WinRT Modern App Template

Thank you for your interest in contributing! We welcome contributions to make this template even more helpful for C++/WinRT developers.

Please take a moment to review these guidelines before submitting a pull request.

## 📐 Design & Coding Guidelines

To maintain code quality and build reliability, please adhere to the following standards:

### 1. File Organization (Flat XAML Views)
- **Do not place XAML files in nested subdirectories** (e.g., `Views/HomePage.xaml`).
- Due to the way the C++/WinRT compilation targets generate projections, nested XAML files cause header include path mismatches during MSBuild. Keep all XAML pages in the **root project directory** alongside their corresponding `.idl`, `.h`, and `.cpp` files.

### 2. Modern Fluent Styling
- **Never use hardcoded hex or RGB colors** for UI controls.
- Use Windows theme resources (e.g., `{ThemeResource ApplicationForegroundThemeBrush}` or `{ThemeResource CardBackgroundFillColorDefaultBrush}`) so the application UI automatically switches between Light and Dark mode.

### 3. Native Coroutines & UI Thread Safety
- Do not perform heavy work, network operations, or disk I/O on the UI thread.
- Switch to background threads using `winrt::resume_background()`.
- Return to the UI thread using Windows Implementation Library's `wil::resume_foreground()` helper with the page dispatcher (e.g., `co_await wil::resume_foreground(DispatcherQueue())`).
- Properly qualify async return types using `winrt::Windows::Foundation::IAsyncAction` or `winrt::Windows::Foundation::IAsyncOperation<T>`.

### 4. Code Structuring & IDL Files
- All classes instantiated or declared in XAML (like custom Pages or Controls) must have a corresponding `.idl` file declaring the WinRT runtime class.
- In implementation source files, follow this header include pattern:
  ```cpp
  #include "pch.h"
  #include "MyPage.xaml.h"
  #if __has_include("MyPage.g.cpp")
  #include "MyPage.g.cpp"
  #endif
  ```

## 🛠️ How to Contribute

1. **Fork the Repository** and clone your fork locally.
2. **Create a Feature Branch** off the `main` branch.
3. **Commit your changes** with clear and descriptive commit messages.
4. **Test & Build**: Perform a clean build and verify execution of the app template.
5. **Submit a Pull Request** describing your changes, design decisions, and testing validation.
