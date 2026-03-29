#pragma once

#include "MainWindow.g.h"
#include "GridItem.h"

#include <mutex>
#include <vector>
#include <winrt/base.h>
#include <winrt/Windows.UI.Text.h>
#include <winrt/Microsoft.UI.Windowing.h>
#include <winrt/Microsoft.Graphics.Canvas.Text.h>

namespace winrt::SearchAlgorithmGame::implementation
{
    struct MainWindow : MainWindowT<MainWindow>
    {
        MainWindow()
        {
            // Maximize
            this->Activated([this](auto&&, auto&&) {});
            {
                // Get the AppWindow
                auto appWindow = Microsoft::UI::Windowing::AppWindow::GetFromWindowId(this->AppWindow().Id());

                // Get the default presenter
                auto presenter = appWindow.Presenter().as<Microsoft::UI::Windowing::OverlappedPresenter>();
                presenter.Maximize();
            }

        }

        // Properties
        int32_t MyProperty();
        void MyProperty(int32_t value);

		// Event handlers
        void MainCanvas_Draw(winrt::Microsoft::Graphics::Canvas::UI::Xaml::ICanvasControl const& sender, winrt::Microsoft::Graphics::Canvas::UI::Xaml::CanvasDrawEventArgs const& args);
        void MainCanvas_Loaded(winrt::Windows::Foundation::IInspectable const& sender, winrt::Microsoft::UI::Xaml::RoutedEventArgs const& e);
        void MainCanvas_SizeChanged(winrt::Windows::Foundation::IInspectable const& sender, winrt::Microsoft::UI::Xaml::SizeChangedEventArgs const& e);
        void MainCanvas_CreateResources(winrt::Microsoft::Graphics::Canvas::UI::Xaml::CanvasControl const& sender, winrt::Microsoft::Graphics::Canvas::UI::CanvasCreateResourcesEventArgs const& args);
        winrt::fire_and_forget MainCanvas_Tapped(winrt::Windows::Foundation::IInspectable const& sender, winrt::Microsoft::UI::Xaml::Input::TappedRoutedEventArgs const& e);
        winrt::fire_and_forget MainCanvas_PointerPressed(winrt::Windows::Foundation::IInspectable const& sender, winrt::Microsoft::UI::Xaml::Input::PointerRoutedEventArgs const& e);


        // Private Functions
        winrt::Windows::Foundation::Point GetGridSize();
        winrt::Windows::Foundation::Size GetGridItemSize(UINT rows, UINT columns);
		void InitializeGrid_FromDimesions(UINT rows, UINT columns);
        void InitializeGrid_FromGridItemSize(UINT gridItemWidth, UINT gridItemHeight);
		winrt::Windows::Foundation::IAsyncAction GridItemClicked(winrt::Windows::Foundation::Point pos);

        // Private members 
    private:
        float m_xOffset = 0;
		float m_yOffset = 0;

		bool m_suppressTap = false;

		UINT m_nRows = 50;
		UINT m_nCols = 50;
        std::vector<std::vector<GridItem>> m_grid;
        winrt::Microsoft::Graphics::Canvas::Text::CanvasTextFormat m_textFormat{ nullptr };
        std::mutex m_gridMutex; // Protects access to m_grid
    public:
        void ResetCanvasButton_Click(winrt::Windows::Foundation::IInspectable const& sender, winrt::Microsoft::UI::Xaml::RoutedEventArgs const& e);
    };
}

namespace winrt::SearchAlgorithmGame::factory_implementation
{
    struct MainWindow : MainWindowT<MainWindow, implementation::MainWindow>
    {
    };
}
