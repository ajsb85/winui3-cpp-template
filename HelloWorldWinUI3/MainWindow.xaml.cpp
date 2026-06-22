#include "pch.h"
#include "MainWindow.xaml.h"
#if __has_include("MainWindow.g.cpp")
#include "MainWindow.g.cpp"
#endif

#include <winrt/Windows.Foundation.h>
#include <winrt/Windows.System.h>
#include <chrono>
#include <format>

// Views
#include "HomePage.xaml.h"
#include "SettingsPage.xaml.h"

using namespace winrt;
using namespace Microsoft::UI::Xaml;
using namespace Microsoft::UI::Xaml::Controls;
using namespace Microsoft::UI::Windowing; // AppWindow, AppWindowTitleBar
using namespace std::chrono_literals;

namespace winrt::HelloWorldWinUI3::implementation
{
    // ── Constructor ─────────────────────────────────────────────────────────

    MainWindow::MainWindow()
    {
        InitializeComponent();

        // 2. Push app content into the title-bar area.
        ExtendsContentIntoTitleBar(true);

        // 3. Register our custom AppTitleBar XAML element as the drag region.
        SetTitleBar(AppTitleBar());

        // 4. Customise the native system-button colours via AppWindowTitleBar
        ConfigureTitleBarButtonColours();
    }

    void MainWindow::MainNavView_Loaded(
        IInspectable const&,
        RoutedEventArgs const&)
    {
        // Select Home page by default
        MainNavView().SelectedItem(MainNavView().MenuItems().GetAt(0));
        ContentFrame().Navigate(xaml_typename<HelloWorldWinUI3::HomePage>());
    }

    void MainWindow::MainNavView_ItemInvoked(
        NavigationView const&,
        NavigationViewItemInvokedEventArgs const& args)
    {
        if (args.IsSettingsInvoked())
        {
            ContentFrame().Navigate(xaml_typename<HelloWorldWinUI3::SettingsPage>());
        }
        else
        {
            auto tag = unbox_value<hstring>(args.InvokedItemContainer().Tag());
            if (tag == L"Home")
            {
                ContentFrame().Navigate(xaml_typename<HelloWorldWinUI3::HomePage>());
            }
        }
    }

    // ── ConfigureTitleBarButtonColours ──────────────────────────────────────

    void MainWindow::ConfigureTitleBarButtonColours()
    {
        if (!AppWindowTitleBar::IsCustomizationSupported())
        {
            return;
        }

        auto titleBar = AppWindow().TitleBar();

        Windows::UI::Color transparent{ 0, 0, 0, 0 };
        titleBar.ButtonBackgroundColor(transparent);
        titleBar.ButtonInactiveBackgroundColor(transparent);
        titleBar.ButtonInactiveForegroundColor(transparent);

        Windows::UI::Color hoverBg{ 30, 255, 255, 255 };
        titleBar.ButtonHoverBackgroundColor(hoverBg);

        Windows::UI::Color pressedBg{ 55, 255, 255, 255 };
        titleBar.ButtonPressedBackgroundColor(pressedBg);
    }
}
