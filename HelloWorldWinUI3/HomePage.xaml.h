#pragma once

#include "HomePage.g.h"

namespace winrt::HelloWorldWinUI3::implementation
{
    struct HomePage : HomePageT<HomePage>
    {
        HomePage();

        void MyButton_Click(
            winrt::Windows::Foundation::IInspectable const& sender,
            winrt::Microsoft::UI::Xaml::RoutedEventArgs const& args);

        winrt::Windows::Foundation::IAsyncAction ProcessDataAsync();
    };
}

namespace winrt::HelloWorldWinUI3::factory_implementation
{
    struct HomePage : HomePageT<HomePage, implementation::HomePage>
    {
    };
}
