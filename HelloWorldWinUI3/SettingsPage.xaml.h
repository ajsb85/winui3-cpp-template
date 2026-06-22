#pragma once

#include "SettingsPage.g.h"

namespace winrt::HelloWorldWinUI3::implementation
{
    struct SettingsPage : SettingsPageT<SettingsPage>
    {
        SettingsPage();
    };
}

namespace winrt::HelloWorldWinUI3::factory_implementation
{
    struct SettingsPage : SettingsPageT<SettingsPage, implementation::SettingsPage>
    {
    };
}
