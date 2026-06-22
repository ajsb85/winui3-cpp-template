# WinUI 3 C++/WinRT Modern App Template

A highly optimized, modular C++/WinRT template for building desktop applications using WinUI 3 and the Windows App SDK.

This template is designed to follow modern Windows 11 Fluent Design guidelines and C++/WinRT best practices.

## 🚀 Features

- **🎨 Mica Backdrop:** Built-in support for native Mica backdrop (base material) configured in XAML with automatic fallback support on older Windows versions.
- **🖥️ Custom Extended Titlebar:** The app window extends its canvas into the title bar area, drawing a custom draggable title bar containing modern icons and text that automatically adapt between dark and light themes. Native Caption buttons (Min/Max/Close) are styled seamlessly.
- **🧭 Multi-Page Navigation:** Standard shell layout utilizing a `NavigationView` and page frame navigation. Includes `HomePage` and `SettingsPage` components out-of-the-box.
- **⚡ Asynchronous Processing:** Implementation of native C++ coroutines (`co_await`, `co_return`) for background tasks. Heavy operations are dispatched to background threads using `winrt::resume_background()` and return back to the UI thread using Windows Implementation Library's `wil::resume_foreground()`.
- **📦 Namespace Renaming Utility:** Includes an automated renaming script to change project namespaces, filenames, and GUIDs when bootstrapping a new application.

## 🛠️ Prerequisites

- **Visual Studio 2022** with the following workloads:
  - Desktop development with C++
  - Universal Windows Platform development (optional)
  - C++/WinRT Visual Studio Extension (VSIX)
- **Windows App SDK** NuGet packages (automatically restored on build).

## 🎬 Getting Started (Instantiating a New Project)

To create a new application from this template:

1. Clone or download this template folder to your workspace.
2. Open PowerShell in the root directory.
3. Run the renaming script with your desired application name:
   ```powershell
   .\Rename-WinUIProject.ps1 -NewName "YourAppName"
   ```
4. Open the newly created `YourAppName.sln` in Visual Studio.
5. Rebuild the solution (Configuration: `Debug` or `Release`, Platform: `x64`).
6. Press **F5** to run the application.

## 📁 Project Structure

```text
├── WinUI3Template.sln             # Visual Studio Solution File
└── WinUI3Template/                # Primary Project Directory
    ├── App.xaml                   # Application entry point & resource definitions
    ├── MainWindow.xaml            # Root Shell containing the NavigationView and Title Bar
    ├── HomePage.xaml              # Primary application page showing async action demonstration
    ├── SettingsPage.xaml          # Personalization and settings configuration page
    ├── pch.h / pch.cpp            # Precompiled headers with complete WinRT namespaces
    ├── app.manifest               # Application manifest declaring unpackaged runtime identity
    └── packages.config            # NuGet packages including Windows App SDK 2.2.0
```

## 📜 License

This project is licensed under the MIT License - see the LICENSE file for details.
