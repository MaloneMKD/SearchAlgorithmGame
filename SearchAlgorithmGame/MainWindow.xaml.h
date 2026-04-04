#pragma once

#include "MainWindow.g.h"
#include "GridItem.h"
#include "SearchAlgorithm.h"

#include <mutex>
#include <chrono>
#include <vector>
#include <winrt/base.h>
#include <winrt/Windows.UI.Text.h>
#include <winrt/Microsoft.UI.Windowing.h>
#include <winrt/Microsoft.Graphics.Canvas.Text.h>

using namespace std::chrono_literals;

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

    private:
        // Private Functions
        Point_Int GetGridSize();
        winrt::Windows::Foundation::Size GetGridItemSize(int rows, int columns);
		void InitializeGrid_FromDimesions(int rows, int columns);
        void InitializeGrid_FromGridItemSize(int gridItemWidth, int gridItemHeight);
		winrt::Windows::Foundation::IAsyncAction GridItemClicked(winrt::Windows::Foundation::Point pos, bool isLeftClick = true);
		winrt::Windows::Foundation::IAsyncAction RunSolutionAnimation(const std::vector<Point_Int>&solution, 
            winrt::Windows::UI::Color color = winrt::Microsoft::UI::Colors::Blue());

        // Private members 
        float m_xOffset = 0;
		float m_yOffset = 0;

        Point_Int m_startIndex{ 0, 0 };
        Point_Int m_goalIndex{ 0, 0 };
        std::vector<Point_Int> m_wallIndices;

		bool m_suppressTap = false;
		bool m_animationRunning = false;

        int m_nRows = 35;
        int m_nCols = 35;
        std::vector<std::vector<GridItem>> m_grid;
        winrt::Microsoft::Graphics::Canvas::Text::CanvasTextFormat m_textFormat{ nullptr };
        std::mutex m_gridMutex; // Protects access to m_grid

		winrt::Windows::Foundation::Size m_canvasSize{ 0, 0 };
    public:
        void ResetCanvasButton_Click(winrt::Windows::Foundation::IInspectable const& sender, winrt::Microsoft::UI::Xaml::RoutedEventArgs const& e);
        winrt::fire_and_forget SearchButton_Click(winrt::Windows::Foundation::IInspectable const& sender, winrt::Microsoft::UI::Xaml::RoutedEventArgs const& e);
        void ReplayButton_Click(winrt::Windows::Foundation::IInspectable const& sender, winrt::Microsoft::UI::Xaml::RoutedEventArgs const& e);
    };
}

namespace winrt::SearchAlgorithmGame::factory_implementation
{
    struct MainWindow : MainWindowT<MainWindow, implementation::MainWindow>
    {
    };
}
