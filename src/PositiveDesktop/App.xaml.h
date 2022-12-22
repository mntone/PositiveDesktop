#pragma once
#include "App.xaml.g.h"

#include "Services/Services.h"

namespace winrt::PositiveDesktop::implementation
{
    struct App : AppT<App>
    {
        App();

        void OnLaunched(Microsoft::UI::Xaml::LaunchActivatedEventArgs const&);
        void OnSuspending(IInspectable const&, Windows::ApplicationModel::SuspendingEventArgs const&);

    private:
        std::unique_ptr<struct ServiceManager> serviceBag_;
        winrt::Microsoft::UI::Xaml::Window window_{ nullptr };
    };
}
