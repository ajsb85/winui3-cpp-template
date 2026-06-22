#include "pch.h"
#include "SettingsPage.xaml.h"
#if __has_include("SettingsPage.g.cpp")
#include "SettingsPage.g.cpp"
#endif

using namespace winrt;
using namespace Microsoft::UI::Xaml;

namespace winrt::HelloWorldWinUI3::implementation
{
    SettingsPage::SettingsPage()
    {
        InitializeComponent();
    }
}
