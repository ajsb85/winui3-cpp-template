#pragma once

#include "MainWindow.g.h"

namespace winrt::HelloWorldWinUI3::implementation
{
    struct MainWindow : MainWindowT<MainWindow>
    {
        MainWindow();

        void MainNavView_Loaded(
            winrt::Windows::Foundation::IInspectable const& sender,
            winrt::Microsoft::UI::Xaml::RoutedEventArgs const& args);

        void MainNavView_ItemInvoked(
            winrt::Microsoft::UI::Xaml::Controls::NavigationView const& sender,
            winrt::Microsoft::UI::Xaml::Controls::NavigationViewItemInvokedEventArgs const& args);

    private:
        void ConfigureTitleBarButtonColours();
    };
}

namespace winrt::HelloWorldWinUI3::factory_implementation
{
    struct MainWindow : MainWindowT<MainWindow, implementation::MainWindow>
    {
    };
}
