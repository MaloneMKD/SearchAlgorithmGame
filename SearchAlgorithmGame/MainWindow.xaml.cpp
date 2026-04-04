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
#include <winrt/Microsoft.UI.Composition.h>

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
    InitializeGrid_FromGridItemSize(35, 35);
}

Point_Int winrt::SearchAlgorithmGame::implementation::MainWindow::GetGridSize()
{
    auto width = MainCanvas().ActualWidth();
    auto height = MainCanvas().ActualHeight();
    int rows = width / GridItem::m_width;
    int columns = height / GridItem::m_height;
    return { rows, columns };
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
    auto isLeftClick = e.GetCurrentPoint(MainCanvas()).Properties().IsLeftButtonPressed();
	m_suppressTap = true;
    co_await GridItemClicked(pos, isLeftClick);
}

winrt::Windows::Foundation::Size winrt::SearchAlgorithmGame::implementation::MainWindow::GetGridItemSize(int rows, int columns)
{
    auto canvasWidth = MainCanvas().ActualWidth();
    auto canvasHeight = MainCanvas().ActualHeight();
    float width = canvasWidth / rows;
    float height = canvasHeight / columns;
    return { width, height };
}

void winrt::SearchAlgorithmGame::implementation::MainWindow::InitializeGrid_FromDimesions(int rows, int columns)
{
	// Set the grid item size based on the canvas size and the number of rows and columns
	auto size = GetGridItemSize(rows, columns);
	GridItem::m_width = size.Width;
	GridItem::m_height = size.Height;

    m_nRows = rows;
	m_nCols = columns;

	// Initialize the grid with default GridItems based on the number of rows and columns
    m_grid.reserve(rows);
    for(int i = 0; i < rows; ++i)
    {
        std::vector<GridItem> row;
        row.reserve(columns);
        for (int j = 0; j < columns; ++j)
        {
            int x = i * GridItem::m_width;
            int y = j * GridItem::m_height;
            auto item = GridItem();
            item.position = { x, y };
            row.emplace_back(item);
        }
        m_grid.emplace_back(row);
    };
}

void winrt::SearchAlgorithmGame::implementation::MainWindow::InitializeGrid_FromGridItemSize(int gridItemWidth, int gridItemHeight)
{

	// Set the grid item size based on the provided dimensions
    GridItem::m_width = gridItemWidth;
    GridItem::m_height = gridItemHeight;

	// Get the grid size based on the canvas size and the grid item size
    auto gridSize = GetGridSize();
    int rows = gridSize.X;
    int columns = gridSize.Y;

    m_nRows = rows;
    m_nCols = columns;

	// Initialize the grid with default GridItems based on the number of rows and columns
    m_grid.clear();
    m_grid.reserve(rows);
    for (int i = 0; i < rows; ++i)
    {
        std::vector<GridItem> row;
        row.reserve(columns);
        for (int j = 0; j < columns; ++j)
        {
            int x = i * GridItem::m_width;
            int y = j * GridItem::m_height;
            auto item = GridItem();
            item.position = { x, y };
            row.emplace_back(item);
        }
        m_grid.emplace_back(row);
    };
}

winrt::Windows::Foundation::IAsyncAction winrt::SearchAlgorithmGame::implementation::MainWindow::GridItemClicked(winrt::Windows::Foundation::Point pos, bool isLeftClick)
{
    if(m_animationRunning)
		co_return;

    apartment_context ui_thread;
    std::pair<int, int> gridPos = { 0, 0 };
	bool itemFound = false;
    co_await resume_background();

    // ========== BACKGROUND THREAD ==========
    concurrency::cancellation_token_source cts;
    concurrency::run_with_cancellation_token([&]() {
        concurrency::parallel_for(size_t(0), size_t(m_nRows), [&](int i) {
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
	hstring selectedAction = EditActionComboBox().SelectedItem().as<winrt::Microsoft::UI::Xaml::Controls::ComboBoxItem>().Content().as<winrt::hstring>();
	auto& gridItem = m_grid[gridPos.first][gridPos.second];

    if (selectedAction == L"Set Wall" && !gridItem.isStart && !gridItem.isGoal && isLeftClick)
    {
        gridItem.m_fillColor = winrt::Microsoft::UI::Colors::DarkGray();
		gridItem.isWall = true;
        m_wallIndices.push_back({ gridPos.first, gridPos.second });
    }
    else if (selectedAction == L"Set Start" && !gridItem.isWall && !gridItem.isGoal && isLeftClick)
    {
        // Clear any existing start
        for (auto& item : m_grid)
        {
            for (auto& gridItem : item)
            {
                if (gridItem.isStart)
                {
                    gridItem.m_fillColor = winrt::Microsoft::UI::Colors::White();
                    gridItem.isStart = false;
                }
            }
        }

		// Set as start
        gridItem.m_fillColor = winrt::Microsoft::UI::Colors::Green();
		gridItem.isStart = true;
		m_startIndex = { gridPos.first, gridPos.second };
    }
    else if (selectedAction == L"Set Goal" && !gridItem.isStart && !gridItem.isWall && isLeftClick)
    {
		// Clear any existing goal
        for (auto& item : m_grid)
        {
            for (auto& gridItem : item)
            {
                if (gridItem.isGoal)
                {
                    gridItem.m_fillColor = winrt::Microsoft::UI::Colors::White();
                    gridItem.isGoal = false;
                }
            }
        }

        // Set as goal
        gridItem.m_fillColor = winrt::Microsoft::UI::Colors::Red();
		gridItem.isGoal = true;
		m_goalIndex = { gridPos.first, gridPos.second };
    }
    else if (selectedAction == L"Erase" || !isLeftClick)
    {
        gridItem.m_fillColor = winrt::Microsoft::UI::Colors::White();
		gridItem.isWall = false;
		gridItem.isStart = false;
		gridItem.isGoal = false;
    }
    MainCanvas().Invalidate();
}

winrt::Windows::Foundation::IAsyncAction winrt::SearchAlgorithmGame::implementation::MainWindow::RunSolutionAnimation(const std::vector <Point_Int>& solution, winrt::Windows::UI::Color color)
{
	m_animationRunning = true;
    std::vector<Point_Int> solution_copy{solution.begin(), solution.end()};

    int i = 0;
	apartment_context ui_thread;
    auto compositor = Window::Current().Compositor();
	for (const auto& point : solution_copy)
    {
        auto& gridItem = m_grid[point.X][point.Y];
		if (gridItem.isStart || gridItem.isGoal)
            continue;
        gridItem.m_fillColor = color;
        MainCanvas().Invalidate();
        co_await 10ms;
		co_await ui_thread;
    }
    m_animationRunning = false;
}


void winrt::SearchAlgorithmGame::implementation::MainWindow::ResetCanvasButton_Click(winrt::Windows::Foundation::IInspectable const& sender, winrt::Microsoft::UI::Xaml::RoutedEventArgs const& e)
{
	UINT boxSize = (UINT)BoxSizeNumberBox().Value();
    InitializeGrid_FromGridItemSize(boxSize, boxSize);
    MainCanvas().Invalidate();
}

winrt::fire_and_forget winrt::SearchAlgorithmGame::implementation::MainWindow::SearchButton_Click(winrt::Windows::Foundation::IInspectable const& sender, winrt::Microsoft::UI::Xaml::RoutedEventArgs const& e)
{
	hstring algorithm = AlgorithmComboBox().SelectedItem().as<winrt::Microsoft::UI::Xaml::Controls::ComboBoxItem>().Content().as<winrt::hstring>();
	if (algorithm == L"A*")
    {
        //SearchAlgorithm::AStar(m_grid, m_startIndex, m_goalIndex);
    }
    else if(algorithm == L"Greedy-Best-First")
    {
        Greedy_Best_First_Search gbfs;
        auto solution = gbfs.FindPath(m_grid, m_startIndex, m_goalIndex);
        RunSolutionAnimation(solution);
    }
    else if(algorithm == L"Uniform-Cost")
    {
        Uniform_Cost_Search ucs;
        if (ucs.FindPath(m_grid, m_startIndex, m_goalIndex))
        {
            co_await RunSolutionAnimation(ucs.m_explored, winrt::Microsoft::UI::Colors::Orange());
            RunSolutionAnimation(ucs.m_solution);
        }
    }
     else if(algorithm == L"Breadth-First")
    {
        Breadth_First_Search bfs;
        auto solution = bfs.FindPath(m_grid, m_startIndex, m_goalIndex);
		RunSolutionAnimation(solution);
    }
     else if(algorithm == L"Depth-First")
    {
        Depth_First_Search dfs;
        auto solution = dfs.FindPath(m_grid, m_startIndex, m_goalIndex);
        RunSolutionAnimation(solution);
    }
}

void winrt::SearchAlgorithmGame::implementation::MainWindow::ReplayButton_Click(winrt::Windows::Foundation::IInspectable const& sender, winrt::Microsoft::UI::Xaml::RoutedEventArgs const& e)
{

}
