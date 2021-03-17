#pragma once

#include "MainPage.g.h"
#include <winrt/Windows.Graphics.Imaging.h>

namespace winrt::ocr_1::implementation
{
    struct MainPage : MainPageT<MainPage>
    {
        MainPage();
        winrt::Windows::Foundation::IAsyncAction button1_Click(winrt::Windows::Foundation::IInspectable const& sender, winrt::Windows::UI::Xaml::RoutedEventArgs const& e);
    
    private:
        winrt::Windows::Graphics::Imaging::SoftwareBitmap showpic{ nullptr };
    
    public:
        winrt::Windows::Foundation::IAsyncAction button_ocr_Click(winrt::Windows::Foundation::IInspectable const& sender, winrt::Windows::UI::Xaml::RoutedEventArgs const& e);
        winrt::Windows::Foundation::IAsyncAction button_save_Click(winrt::Windows::Foundation::IInspectable const& sender, winrt::Windows::UI::Xaml::RoutedEventArgs const& e);
    };
}

namespace winrt::ocr_1::factory_implementation
{
    struct MainPage : MainPageT<MainPage, implementation::MainPage>
    {
    };
}
