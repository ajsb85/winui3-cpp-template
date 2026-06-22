#include "pch.h"
#include "HomePage.xaml.h"
#if __has_include("HomePage.g.cpp")
#include "HomePage.g.cpp"
#endif

#include <winrt/Windows.Foundation.h>
#include <winrt/Windows.System.h>
#include <chrono>

using namespace winrt;
using namespace Microsoft::UI::Xaml;
using namespace std::chrono_literals;

namespace winrt::HelloWorldWinUI3::implementation
{
    HomePage::HomePage()
    {
        InitializeComponent();
    }

    void HomePage::MyButton_Click(IInspectable const&, RoutedEventArgs const&)
    {
        ProcesarInformacionAsincrona();
    }

    winrt::Windows::Foundation::IAsyncAction HomePage::ProcesarInformacionAsincrona()
    {
        // 1. Update text and disable button
        GreetingTextBlock().Text(L"Procesando información...");
        MyButton().IsEnabled(false);

        auto uiDispatcher = this->DispatcherQueue();

        // 2. Jump to background thread pool
        co_await winrt::resume_background();

        // Simulate workload
        std::this_thread::sleep_for(3s);

        // 3. Jump back to UI thread
        co_await wil::resume_foreground(uiDispatcher);

        // 4. Update UI
        GreetingTextBlock().Text(L"¡Información procesada con éxito!");
        MyButton().IsEnabled(true);
    }
}
