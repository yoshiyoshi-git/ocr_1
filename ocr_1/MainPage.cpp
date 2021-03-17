#include "pch.h"
#include "MainPage.h"
#include "MainPage.g.cpp"


#include <winrt/Windows.Storage.h>
#include <winrt/Windows.Storage.Streams.h>
#include <winrt/Windows.Storage.Pickers.h>
#include <winrt/Windows.Foundation.h>
#include <winrt/Windows.UI.Xaml.Media.Imaging.h>
#include "winrt/Windows.Media.Ocr.h"


using namespace winrt;
using namespace Windows::UI::Xaml;


namespace winrt::ocr_1::implementation
{
    MainPage::MainPage()
    {
        InitializeComponent();
    }
}

winrt::Windows::Foundation::IAsyncAction winrt::ocr_1::implementation::MainPage::button1_Click(winrt::Windows::Foundation::IInspectable const& sender, winrt::Windows::UI::Xaml::RoutedEventArgs const& e)
{
    //ここで画像を選択して、表示します。

    winrt::Windows::Storage::Pickers::FileOpenPicker picPicker;
    picPicker.SuggestedStartLocation(winrt::Windows::Storage::Pickers::PickerLocationId::Desktop);
    picPicker.FileTypeFilter().ReplaceAll({ L".jpg", L".jpeg", L".png", L".bmp" });

    auto picfile = co_await picPicker.PickSingleFileAsync();

    if (picfile == nullptr)
        res1().Text(L"選んで無いよ！");
    else
    {
        res1().Text(picfile.Path());

        auto picstream = co_await picfile.OpenAsync(winrt::Windows::Storage::FileAccessMode::Read);
        auto picdecoder = co_await winrt::Windows::Graphics::Imaging::BitmapDecoder::CreateAsync(picstream);

        showpic = co_await picdecoder.GetSoftwareBitmapAsync(winrt::Windows::Graphics::Imaging::BitmapPixelFormat::Bgra8, winrt::Windows::Graphics::Imaging::BitmapAlphaMode::Premultiplied);

        auto picsource = winrt::Windows::UI::Xaml::Media::Imaging::WriteableBitmap(showpic.PixelWidth(), showpic.PixelHeight());

        showpic.CopyToBuffer(picsource.PixelBuffer());
        pic1().Source(picsource);
    }
}


winrt::Windows::Foundation::IAsyncAction winrt::ocr_1::implementation::MainPage::button_ocr_Click(winrt::Windows::Foundation::IInspectable const& sender, winrt::Windows::UI::Xaml::RoutedEventArgs const& e)
{
    //ここで画像から文字を抽出します

    winrt::Windows::Media::Ocr::OcrEngine ocr = nullptr;
    ocr = winrt::Windows::Media::Ocr::OcrEngine::TryCreateFromUserProfileLanguages();

    auto result = co_await ocr.RecognizeAsync(showpic);
    res1().Text(result.Text());
}


winrt::Windows::Foundation::IAsyncAction winrt::ocr_1::implementation::MainPage::button_save_Click(winrt::Windows::Foundation::IInspectable const& sender, winrt::Windows::UI::Xaml::RoutedEventArgs const& e)
{
    //ここで抽出した文字列をテキストファイルへ保存します

    winrt::Windows::Storage::Pickers::FileSavePicker textPicker;
    textPicker.SuggestedStartLocation(winrt::Windows::Storage::Pickers::PickerLocationId::Desktop);
    textPicker.FileTypeChoices().Insert(L"Plain Text", single_threaded_vector<hstring>({ L".txt" }));
    textPicker.SuggestedFileName(L"NewText");

    auto textfile = co_await textPicker.PickSaveFileAsync();
    if (textfile != nullptr)
    {
        co_await winrt::Windows::Storage::FileIO::WriteTextAsync(textfile, res1().Text());
    }
}
