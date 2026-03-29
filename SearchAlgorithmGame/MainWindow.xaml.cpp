#include "pch.h"
#include "MainWindow.xaml.h"
#if __has_include("MainWindow.g.cpp")
#include "MainWindow.g.cpp"
#endif

#include <winrt/Windows.Foundation.h>
#include <winrt/Microsoft.Graphics.Canvas.Text.h>
#include <winrt/Microsoft.Graphics.Canvas.h>
#include <winrt/Microsoft.Graphics.Canvas.UI.Xaml.h>
#include <winrt/Microsoft.UI.Input.h>

#include <PPl.h>

using namespace winrt;
using namespace Microsoft::UI::Xaml;

// To learn more about WinUI, the WinUI project structure,
// and more about our project templates, see: http://aka.ms/winui-project-info.

namespace winrt::SearchAlgorithmGame::implementation
{
    int32_t MainWindow::MyProperty()
    {
        throw hresult_not_implemented();
    }

    void MainWindow::MyProperty(int32_t /* value */)
    {
        throw hresult_not_implemented();
    }
}

void winrt::SearchAlgorithmGame::implementation::MainWindow::MainCanvas_Draw(winrt::Microsoft::Graphics::Canvas::UI::Xaml::ICanvasControl const& sender, winrt::Microsoft::Graphics::Canvas::UI::Xaml::CanvasDrawEventArgs const& args)
{
    auto ds = args.DrawingSession();
    ds.Clear(winrt::Microsoft::UI::Colors::White());

    if(!m_textFormat || m_grid.size() < 1)
		return;

    // Calculate the x and y offsets to center the grid on the canvas
    m_xOffset = (MainCanvas().ActualWidth() - (m_grid.size() * GridItem::m_width)) / 2;
    m_yOffset = (MainCanvas().ActualHeight() - (m_grid.front().size() *GridItem::m_height)) / 2;

	// Draw the grid items
	for (int i = 0; i < m_grid.size(); ++i) // Rows
    {
        for(int j = 0; j < m_grid[i].size(); ++j) // Columns
        {
            auto& item = m_grid[i][j];
			ds.DrawRectangle(item.position.X + m_xOffset, item.position.Y + m_yOffset, GridItem::m_width, GridItem::m_height, winrt::Microsoft::UI::Colors::Black());
            ds.FillRectangle(item.position.X + m_xOffset, item.position.Y + m_yOffset, GridItem::m_width, GridItem::m_height, item.m_fillColor);
            ds.DrawTextW(winrt::to_hstring(i + j * m_grid[i].size()), item.position.X + m_xOffset + GridItem::m_width / 2.0f,
                         item.position.Y + m_yOffset + GridItem::m_height / 2.0f, winrt::Microsoft::UI::Colors::Gray(), m_textFormat);
        }
    }
}

void winrt::SearchAlgorithmGame::implementation::MainWindow::MainCanvas_Loaded(winrt::Windows::Foundation::IInspectable const& sender, winrt::Microsoft::UI::Xaml::RoutedEventArgs const& e)
{
	// Calculate aspect ratio based grid item size based on the canvas size and a desired number of rows and columns
    InitializeGrid_FromGridItemSize(50, 50);
}

winrt::Windows::Foundation::Point winrt::SearchAlgorithmGame::implementation::MainWindow::GetGridSize()
{
    auto width = MainCanvas().ActualWidth();
    auto height = MainCanvas().ActualHeight();
    UINT rows = width / GridItem::m_width;
    UINT columns = height / GridItem::m_height;
    return { (float)rows, (float)columns };
}

void winrt::SearchAlgorithmGame::implementation::MainWindow::MainCanvas_SizeChanged(winrt::Windows::Foundation::IInspectable const& sender, winrt::Microsoft::UI::Xaml::SizeChangedEventArgs const& e)
{
    // Calculate the x and y offsets to center the grid on the canvas
    m_xOffset = (MainCanvas().ActualWidth() - (m_nRows * GridItem::m_width)) / 2.0f;
    m_yOffset = (MainCanvas().ActualHeight() - (m_nCols * GridItem::m_height)) / 2.0f;

    if (m_xOffset < 0)
        m_xOffset = 0;
    if (m_yOffset < 0)
        m_yOffset = 0;
}

void winrt::SearchAlgorithmGame::implementation::MainWindow::MainCanvas_CreateResources(winrt::Microsoft::Graphics::Canvas::UI::Xaml::CanvasControl const& sender, winrt::Microsoft::Graphics::Canvas::UI::CanvasCreateResourcesEventArgs const& args)
{
    // If text format is null, initialize it
    if (!m_textFormat)
    {
        m_textFormat = winrt::Microsoft::Graphics::Canvas::Text::CanvasTextFormat();
        m_textFormat.FontSize(8);
        m_textFormat.HorizontalAlignment(winrt::Microsoft::Graphics::Canvas::Text::CanvasHorizontalAlignment::Center);
        m_textFormat.VerticalAlignment(winrt::Microsoft::Graphics::Canvas::Text::CanvasVerticalAlignment::Center);
    }
}

winrt::fire_and_forget  winrt::SearchAlgorithmGame::implementation::MainWindow::MainCanvas_Tapped(winrt::Windows::Foundation::IInspectable const& sender, winrt::Microsoft::UI::Xaml::Input::TappedRoutedEventArgs const& e)
{
	// If the tap event was caused by a pointer pressed event, ignore it to prevent double processing of the same input
    if(m_suppressTap)
    {
        m_suppressTap = false;
        co_return;
	}

    auto pos = e.GetPosition(MainCanvas());
	co_await GridItemClicked(pos);
}

winrt::fire_and_forget winrt::SearchAlgorithmGame::implementation::MainWindow::MainCanvas_PointerPressed(winrt::Windows::Foundation::IInspectable const& sender, winrt::Microsoft::UI::Xaml::Input::PointerRoutedEventArgs const& e)
{
    auto pos = e.GetCurrentPoint(MainCanvas()).Position();
	m_suppressTap = true;
    co_await GridItemClicked(pos);
}

winrt::Windows::Foundation::Size winrt::SearchAlgorithmGame::implementation::MainWindow::GetGridItemSize(UINT rows, UINT columns)
{
    auto canvasWidth = MainCanvas().ActualWidth();
    auto canvasHeight = MainCanvas().ActualHeight();
    float width = canvasWidth / rows;
    float height = canvasHeight / columns;
    return { width, height };
}

void winrt::SearchAlgorithmGame::implementation::MainWindow::InitializeGrid_FromDimesions(UINT rows, UINT columns)
{
	// Set the grid item size based on the canvas size and the number of rows and columns
	auto size = GetGridItemSize(rows, columns);
	GridItem::m_width = size.Width;
	GridItem::m_height = size.Height;

    m_nRows = rows;
	m_nCols = columns;

	// Initialize the grid with default GridItems based on the number of rows and columns
    m_grid.reserve(rows);
    for(UINT i = 0; i < rows; ++i)
    {
        std::vector<GridItem> row;
        row.reserve(columns);
        for (UINT j = 0; j < columns; ++j)
        {
            float x = i * GridItem::m_width;
            float y = j * GridItem::m_height;
            auto item = GridItem();
            item.position = { x, y };
            row.emplace_back(item);
        }
        m_grid.emplace_back(row);
    };
}

void winrt::SearchAlgorithmGame::implementation::MainWindow::InitializeGrid_FromGridItemSize(UINT gridItemWidth, UINT gridItemHeight)
{

	// Set the grid item size based on the provided dimensions
    GridItem::m_width = gridItemWidth;
    GridItem::m_height = gridItemHeight;

	// Get the grid size based on the canvas size and the grid item size
    auto gridSize = GetGridSize();
    UINT rows = gridSize.X;
	UINT columns = gridSize.Y;

    m_nRows = rows;
    m_nCols = columns;

	// Initialize the grid with default GridItems based on the number of rows and columns
    m_grid.clear();
    m_grid.reserve(rows);
    for (UINT i = 0; i < rows; ++i)
    {
        std::vector<GridItem> row;
        row.reserve(columns);
        for (UINT j = 0; j < columns; ++j)
        {
            float x = i * GridItem::m_width;
            float y = j * GridItem::m_height;
            auto item = GridItem();
            item.position = { x, y };
            row.emplace_back(item);
        }
        m_grid.emplace_back(row);
    };
}

winrt::Windows::Foundation::IAsyncAction winrt::SearchAlgorithmGame::implementation::MainWindow::GridItemClicked(winrt::Windows::Foundation::Point pos)
{
    apartment_context ui_thread;
    std::pair<UINT, UINT> gridPos = { 0, 0 };
	bool itemFound = false;
    co_await resume_background();

    // ========== BACKGROUND THREAD ==========
    concurrency::cancellation_token_source cts;
    concurrency::run_with_cancellation_token([&]() {
        concurrency::parallel_for(size_t(0), size_t(m_nRows), [&](UINT i) {
            // Check if cancelled before doing work for this iteration
            if (concurrency::is_current_task_group_canceling())
                return;

            for (int j = 0; j < m_nCols; ++j)
            {
                if (m_grid[i][j].HitTest(pos, m_xOffset, m_yOffset))
                {
					itemFound = true;
                    gridPos = { (float)i, (float)j };
                    cts.cancel();
                }
            }
            });
        }, cts.get_token());


    co_await ui_thread;

    if (!itemFound)
        co_return;

    // ========== UI THREAD ==========
    m_grid[gridPos.first][gridPos.second].m_fillColor = winrt::Microsoft::UI::Colors::DarkGray();
    MainCanvas().Invalidate();
}

void winrt::SearchAlgorithmGame::implementation::MainWindow::ResetCanvasButton_Click(winrt::Windows::Foundation::IInspectable const& sender, winrt::Microsoft::UI::Xaml::RoutedEventArgs const& e)
{
	UINT boxSize = (UINT)BoxSizeNumberBox().Value();
    InitializeGrid_FromGridItemSize(boxSize, boxSize);
    MainCanvas().Invalidate();
}
