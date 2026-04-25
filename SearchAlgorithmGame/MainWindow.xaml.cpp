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

#include <winrt/Windows.Data.Json.h>
#include <winrt/Windows.Storage.h>
#include <winrt/Microsoft.Windows.Storage.Pickers.h>

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

    if(m_textFormat == nullptr || m_grid.size() < 1)
		return;

    // Calculate the x and y offsets to center the grid on the canvas
    m_xOffset = (m_canvasSize.Width - (m_nCols * m_gridItemWidth)) / 2;
    m_yOffset = (m_canvasSize.Height - (m_nRows * m_gridItemHeight)) / 2;

	if (m_xOffset > m_canvasSize.Width) m_xOffset = 0;
	if (m_yOffset > m_canvasSize.Height) m_yOffset = 0;

	// Draw the grid items
	for (int i = 0; i < m_nRows; ++i) // Rows
    {
        for(int j = 0; j < m_nCols; ++j) // Columns
        {
            auto& item = m_grid[i][j];
			ds.DrawRectangle(item.position.X + m_xOffset, item.position.Y + m_yOffset, m_gridItemWidth, m_gridItemHeight, winrt::Microsoft::UI::Colors::Black());
            ds.FillRectangle(item.position.X + m_xOffset, item.position.Y + m_yOffset, m_gridItemWidth, m_gridItemHeight, item.m_fillColor);
            ds.DrawText(winrt::to_hstring(j + i * (int)m_grid[i].size()), 
                         item.position.X + m_xOffset + m_gridItemWidth / 2.0f,
                         item.position.Y + m_yOffset + m_gridItemHeight / 2.0f,
                         winrt::Microsoft::UI::Colors::Gray(),
                         m_textFormat);
        }   
    }
}

void winrt::SearchAlgorithmGame::implementation::MainWindow::MainCanvas_Loaded(winrt::Windows::Foundation::IInspectable const& sender, winrt::Microsoft::UI::Xaml::RoutedEventArgs const& e)
{
	// Calculate aspect ratio based grid item size based on the canvas size and a desired number of rows and columns
    InitializeGrid_FromGridItemSize(35, 35);

     // If text format is null, initialize it
    if (m_textFormat == nullptr)
    {
        m_textFormat = winrt::Microsoft::Graphics::Canvas::Text::CanvasTextFormat();
        m_textFormat.FontSize(8);
        m_textFormat.HorizontalAlignment(winrt::Microsoft::Graphics::Canvas::Text::CanvasHorizontalAlignment::Center);
        m_textFormat.VerticalAlignment(winrt::Microsoft::Graphics::Canvas::Text::CanvasVerticalAlignment::Center);
    }
}

Point_Int winrt::SearchAlgorithmGame::implementation::MainWindow::GetGridSize()
{
    int rows = m_canvasSize.Height / m_gridItemHeight;
    int columns = m_canvasSize.Width / m_gridItemWidth;
    return { columns, rows };
}

void winrt::SearchAlgorithmGame::implementation::MainWindow::MainCanvas_SizeChanged(winrt::Windows::Foundation::IInspectable const& sender, winrt::Microsoft::UI::Xaml::SizeChangedEventArgs const& e)
{
    m_canvasSize = e.NewSize();

    // Calculate the x and y offsets to center the grid on the canvas
    m_xOffset = (m_canvasSize.Width - (m_nCols * m_gridItemWidth)) / 2.0f;
    m_yOffset = (m_canvasSize.Height - (m_nRows * m_gridItemHeight)) / 2.0f;

    if (m_xOffset < 0)
        m_xOffset = 0;
    if (m_yOffset < 0)
        m_yOffset = 0;
}

void  winrt::SearchAlgorithmGame::implementation::MainWindow::MainCanvas_Tapped(winrt::Windows::Foundation::IInspectable const& sender, winrt::Microsoft::UI::Xaml::Input::TappedRoutedEventArgs const& e)
{
	// If the tap event was caused by a pointer pressed event, ignore it to prevent double processing of the same input
    if(m_suppressTap)
    {
        m_suppressTap = false;
        return;
	}

    auto pos = e.GetPosition(MainCanvas());
	GridItemClicked(pos);
}

void winrt::SearchAlgorithmGame::implementation::MainWindow::MainCanvas_PointerPressed(winrt::Windows::Foundation::IInspectable const& sender, winrt::Microsoft::UI::Xaml::Input::PointerRoutedEventArgs const& e)
{
    auto pos = e.GetCurrentPoint(MainCanvas()).Position();
    auto isLeftClick = e.GetCurrentPoint(MainCanvas()).Properties().IsLeftButtonPressed();
	m_suppressTap = true;
    GridItemClicked(pos, isLeftClick);
}

winrt::Windows::Foundation::Size winrt::SearchAlgorithmGame::implementation::MainWindow::GetGridItemSize(int rows, int columns)
{
    float width = m_canvasSize.Width / columns;
    float height = m_canvasSize.Height / rows;
    return { width, height };
}

void winrt::SearchAlgorithmGame::implementation::MainWindow::InitializeGrid_FromDimesions(int rows, int columns)
{
	// Set the grid item size based on the canvas size and the number of rows and columns
	auto size = GetGridItemSize(rows, columns);
    m_gridItemWidth = size.Width;
	m_gridItemHeight = size.Height;

    BoxSizeNumberBox().Value(m_gridItemHeight);

    m_nRows = rows;
	m_nCols = columns;

	// Initialize the grid with default GridItems based on the number of rows and columns
    m_grid.clear();
    m_grid.resize(rows, std::vector<GridItem>(columns));
    for (int i = 0; i < rows; ++i)
    {
        for (int j = 0; j < columns; ++j)
        {
            auto& item = m_grid[i][j];
            item.m_width = m_gridItemWidth;
            item.m_height = m_gridItemHeight;
            item.position = { j * m_gridItemWidth, i * m_gridItemHeight };
        }
    }
}

void winrt::SearchAlgorithmGame::implementation::MainWindow::InitializeGrid_FromGridItemSize(int gridItemWidth, int gridItemHeight)
{
	// Set the grid item size based on the provided dimensions
    m_gridItemWidth = gridItemWidth;
    m_gridItemHeight = gridItemHeight;

	// Get the grid size based on the canvas size and the grid item size
    auto gridSize = GetGridSize();
    m_nCols = gridSize.X;
    m_nRows = gridSize.Y;

	// Initialize the grid with default GridItems based on the number of rows and columns
    m_grid.clear();
    m_grid.resize(m_nRows, std::vector<GridItem>(m_nCols));
    for (int i = 0; i < m_nRows; ++i)
    {
        for (int j = 0; j < m_nCols; ++j)
        {
            auto& item = m_grid[i][j];
            item.m_width = m_gridItemWidth;
            item.m_height = m_gridItemHeight;
            item.position = { j * m_gridItemWidth, i * m_gridItemHeight };
        }
    }
}

void winrt::SearchAlgorithmGame::implementation::MainWindow::GridItemClicked(winrt::Windows::Foundation::Point pos, bool isLeftClick)
{
    if(m_animationRunning)
		return;

    std::pair<int, int> gridPos = { 0, 0 };
	bool itemFound = false;

    for(int i = 0; i < m_nRows; i++) 
    {
        for (int j = 0; j < m_nCols; ++j)
        {
            if (m_grid[i][j].HitTest(pos, m_xOffset, m_yOffset))
            {
				itemFound = true;
                gridPos = { i, j };
                break;
            }
        }
    };

    if (!itemFound)
        return;

	hstring selectedAction = EditActionComboBox().SelectedItem().as<winrt::Microsoft::UI::Xaml::Controls::ComboBoxItem>().Content().as<winrt::hstring>();
	auto& gridItem = m_grid[gridPos.first][gridPos.second];

    if (selectedAction == L"Set Wall" && !gridItem.isStart && !gridItem.isGoal && !gridItem.isWall && isLeftClick)
    {
        gridItem.m_fillColor = winrt::Microsoft::UI::Colors::DarkGray();
		gridItem.isWall = true;
        m_wallIndices.push_back({ gridPos.first, gridPos.second });
    }
    else if (selectedAction == L"Set Start" && !gridItem.isWall && !gridItem.isGoal && isLeftClick)
    {
        // Clear any existing start
        if (m_grid[m_startIndex.X][m_startIndex.Y].isStart)
        {
            m_grid[m_startIndex.X][m_startIndex.Y].m_fillColor = winrt::Microsoft::UI::Colors::White();
            m_grid[m_startIndex.X][m_startIndex.Y].isStart = false;
        }

		// Set as start
        gridItem.m_fillColor = winrt::Microsoft::UI::Colors::Green();
		gridItem.isStart = true;
		m_startIndex = { gridPos.first, gridPos.second };
    }
    else if (selectedAction == L"Set Goal" && !gridItem.isStart && !gridItem.isWall && isLeftClick)
    {
		// Clear any existing goal
        if (m_grid[m_goalIndex.X][m_goalIndex.Y].isGoal)
        {
            m_grid[m_goalIndex.X][m_goalIndex.Y].m_fillColor = winrt::Microsoft::UI::Colors::White();
            m_grid[m_goalIndex.X][m_goalIndex.Y].isGoal = false;
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

        // Remove from wall indices if it was a wall
        auto it = std::find(m_wallIndices.begin(), m_wallIndices.end(), Point_Int{ gridPos.first, gridPos.second });
        if (it != m_wallIndices.end())
			m_wallIndices.erase(it);
    }
    MainCanvas().Invalidate();
}

winrt::Windows::Foundation::IAsyncAction winrt::SearchAlgorithmGame::implementation::MainWindow::RunSolutionAnimation(const std::vector <Point_Int>& solution, winrt::Windows::UI::Color color)
{
	StopSearchButton().Visibility(winrt::Microsoft::UI::Xaml::Visibility::Visible);
	m_animationRunning = true;
    std::vector<Point_Int> solution_copy{solution.begin(), solution.end()};

    int i = 0;
	apartment_context ui_thread;
	for (const auto& point : solution_copy)
    {
        if (m_stopAnimation)
            break;

        auto& gridItem = m_grid[point.X][point.Y];
		if (gridItem.isStart || gridItem.isGoal)
            continue;
        gridItem.m_fillColor = color;
        MainCanvas().Invalidate();
        co_await 10ms;
		co_await ui_thread;
    }
    m_animationRunning = false;
    StopSearchButton().Visibility(winrt::Microsoft::UI::Xaml::Visibility::Collapsed);
    co_return;
}

winrt::Windows::Foundation::IAsyncAction winrt::SearchAlgorithmGame::implementation::MainWindow::ClearPath()
{
	winrt::apartment_context ui_thread;
	co_await resume_background();
    for(int i = 0; i < m_nRows; i++) {
        for (int j = 0; j < m_nCols; ++j)
        {
            if(!m_grid[i][j].isGoal && !m_grid[i][j].isStart && !m_grid[i][j].isWall)
                m_grid[i][j].m_fillColor = winrt::Microsoft::UI::Colors::White();
        }
    };
	co_await ui_thread;
	MainCanvas().Invalidate();
}

void winrt::SearchAlgorithmGame::implementation::MainWindow::DisplayMessage(winrt::hstring title, winrt::hstring message)
{
    winrt::Microsoft::UI::Xaml::Controls::ContentDialog dialog;
    dialog.Title(winrt::box_value(title));
    dialog.Content(winrt::box_value(message));
    dialog.CloseButtonText(L"OK");
    dialog.XamlRoot(MainGrid().XamlRoot());
    dialog.ShowAsync();
}

void winrt::SearchAlgorithmGame::implementation::MainWindow::ResetCanvasButton_Click(winrt::Windows::Foundation::IInspectable const& sender, winrt::Microsoft::UI::Xaml::RoutedEventArgs const& e)
{
	int boxSize = (int)BoxSizeNumberBox().Value();
    InitializeGrid_FromGridItemSize(boxSize, boxSize);
    m_wallIndices.clear();
    m_startIndex = { 0, 0 };
    m_goalIndex = { 0, 0 };
    MainCanvas().Invalidate();
}

winrt::fire_and_forget winrt::SearchAlgorithmGame::implementation::MainWindow::SearchButton_Click(winrt::Windows::Foundation::IInspectable const& sender, winrt::Microsoft::UI::Xaml::RoutedEventArgs const& e)
{
	// Clear any existing path visualization
	winrt::apartment_context ui_thread;
    co_await ClearPath();

    m_stopAnimation = false;

	hstring algorithm = AlgorithmComboBox().SelectedItem().as<winrt::Microsoft::UI::Xaml::Controls::ComboBoxItem>().Content().as<winrt::hstring>();
	if (algorithm == L"A*")
    {
		A_Star_Search aStar(m_gridType);
        if (aStar.FindPath(m_grid, m_startIndex, m_goalIndex))
        {            
            co_await RunSolutionAnimation(aStar.m_explored, winrt::Microsoft::UI::Colors::Orange());
            RunSolutionAnimation(aStar.m_solution);
        }
    }
    else if(algorithm == L"Greedy-Best-First")
    {
        Greedy_Best_First_Search gbfs(m_gridType);
        if (gbfs.FindPath(m_grid, m_startIndex, m_goalIndex))
        {
            RunSolutionAnimation(gbfs.m_solution);
        }
    }
    else if(algorithm == L"Uniform-Cost")
    {
        Uniform_Cost_Search ucs(m_gridType);
        if (ucs.FindPath(m_grid, m_startIndex, m_goalIndex))
        {
            co_await RunSolutionAnimation(ucs.m_explored, winrt::Microsoft::UI::Colors::Orange());
            RunSolutionAnimation(ucs.m_solution);
        }
    }
     else if(algorithm == L"Breadth-First")
    {
        Breadth_First_Search bfs(m_gridType);
        if(bfs.FindPath(m_grid, m_startIndex, m_goalIndex))
		    RunSolutionAnimation(bfs.m_explored);
    }
     else if(algorithm == L"Depth-First")
    {
        Depth_First_Search dfs(m_gridType);
        if(dfs.FindPath(m_grid, m_startIndex, m_goalIndex))
            RunSolutionAnimation(dfs.m_explored);
    }
    co_return;
}

void winrt::SearchAlgorithmGame::implementation::MainWindow::ClearPathsButton_Click(winrt::Windows::Foundation::IInspectable const& sender, winrt::Microsoft::UI::Xaml::RoutedEventArgs const& e)
{
    ClearPath();
}

void winrt::SearchAlgorithmGame::implementation::MainWindow::StopSearchButton_Click(winrt::Windows::Foundation::IInspectable const& sender, winrt::Microsoft::UI::Xaml::RoutedEventArgs const& e)
{
	m_stopAnimation = true;
}

void winrt::SearchAlgorithmGame::implementation::MainWindow::GridTypeItem_Click(winrt::Windows::Foundation::IInspectable const& sender, winrt::Microsoft::UI::Xaml::RoutedEventArgs const& e)
{
	hstring name =sender.as<winrt::Microsoft::UI::Xaml::Controls::MenuFlyoutItem>().Name();
	if (name == L"FourConnectedMenuItem")
        m_gridType = GridType::Four_Connected;
    else if(name == L"EightConnectedMenuItem")
        m_gridType = GridType::Eight_Connected;
}

winrt::Windows::Foundation::IAsyncAction winrt::SearchAlgorithmGame::implementation::MainWindow::SaveDesignMenuItem_Click(winrt::Windows::Foundation::IInspectable const& sender, winrt::Microsoft::UI::Xaml::RoutedEventArgs const& e)
{
    auto ui_thread = winrt::apartment_context();
    // Open file explorer to select save location and file name
    auto picker = Microsoft::Windows::Storage::Pickers::FileSavePicker(this->AppWindow().Id());
    picker.SuggestedStartLocation(Microsoft::Windows::Storage::Pickers::PickerLocationId::Downloads);
    picker.SuggestedFileName(L"GridDesign");
    picker.FileTypeChoices().Insert(L"JSON File", winrt::single_threaded_vector<hstring>({ L".json" }));

    // Show the picker and handle the result
    auto result = co_await picker.PickSaveFileAsync();
    // --------------------- BACKGROUND THREAD ---------------------
    if (!result)
    {
		DisplayMessage(L"Save Cancelled", L"Failed to get save location.");
        co_return;
    }

    co_await ui_thread;
    // --------------------- UI THREAD ---------------------
	LoadingWindow().Visibility(winrt::Microsoft::UI::Xaml::Visibility::Visible);
	LoadingText().Text(L"Saving design...");
	co_await 100ms;

    // --------------------- BACKGROUND THREAD ---------------------

	// Create a JSON object to represent the grid design
    winrt::Windows::Data::Json::JsonObject rootObject;
    rootObject.SetNamedValue(L"AppName", winrt::Windows::Data::Json::JsonValue::CreateStringValue(L"Search Algorithm Game"));
    rootObject.SetNamedValue(L"Version", winrt::Windows::Data::Json::JsonValue::CreateStringValue(L"1.0"));

	// Design data
	winrt::Windows::Data::Json::JsonObject designDataObject;
	winrt::Windows::Data::Json::JsonObject gridDimensionsObject;
	gridDimensionsObject.SetNamedValue(L"Rows", winrt::Windows::Data::Json::JsonValue::CreateNumberValue(m_nRows));
	gridDimensionsObject.SetNamedValue(L"Cols", winrt::Windows::Data::Json::JsonValue::CreateNumberValue(m_nCols));
	designDataObject.SetNamedValue(L"GridDimensions", gridDimensionsObject);

    // Walls
	winrt::Windows::Data::Json::JsonArray wallsArray;
    for (auto& wall : m_wallIndices)
    {
        winrt::Windows::Data::Json::JsonObject wallObject;
		wallObject.SetNamedValue(L"Row", winrt::Windows::Data::Json::JsonValue::CreateNumberValue(wall.X));
		wallObject.SetNamedValue(L"Col", winrt::Windows::Data::Json::JsonValue::CreateNumberValue(wall.Y));
        wallsArray.Append(wallObject);
    }
	designDataObject.SetNamedValue(L"Walls", wallsArray);

	// Start and goal positions
    winrt::Windows::Data::Json::JsonObject startDimObject;
	startDimObject.SetNamedValue(L"Row", winrt::Windows::Data::Json::JsonValue::CreateNumberValue(m_startIndex.X));
    startDimObject.SetNamedValue(L"Col", winrt::Windows::Data::Json::JsonValue::CreateNumberValue(m_startIndex.Y));
	designDataObject.SetNamedValue(L"StartDimensions", startDimObject);

	winrt::Windows::Data::Json::JsonObject goalDimObject;
    goalDimObject.SetNamedValue(L"Row", winrt::Windows::Data::Json::JsonValue::CreateNumberValue(m_goalIndex.X ));
	goalDimObject.SetNamedValue(L"Col", winrt::Windows::Data::Json::JsonValue::CreateNumberValue(m_goalIndex.Y ));
	designDataObject.SetNamedValue(L"GoalDimensions", goalDimObject);

	// Add design data to root object
	rootObject.SetNamedValue(L"DesignData", designDataObject);

    // Serialize to string
    hstring jsonString = rootObject.Stringify();

	// Save the JSON string to the selected file
    auto storageFile = winrt::Windows::Storage::StorageFile(nullptr);
	auto file = co_await storageFile.GetFileFromPathAsync(result.Path());
    co_await winrt::Windows::Storage::FileIO::WriteTextAsync(file, jsonString);

	co_await ui_thread;
	// --------------------- UI THREAD ---------------------
    LoadingWindow().Visibility(winrt::Microsoft::UI::Xaml::Visibility::Collapsed);
	DisplayMessage(L"Design Saved", L"Your grid design has been saved successfully.");
}

winrt::Windows::Foundation::IAsyncAction winrt::SearchAlgorithmGame::implementation::MainWindow::LoadDesignMenuItem_Click(winrt::Windows::Foundation::IInspectable const& sender, winrt::Microsoft::UI::Xaml::RoutedEventArgs const& e)
{
    // Clear wall indices
	m_wallIndices.clear();

    // Open file explorer to select save location and file name
    auto picker = Microsoft::Windows::Storage::Pickers::FileOpenPicker(this->AppWindow().Id());
    picker.SuggestedStartLocation(Microsoft::Windows::Storage::Pickers::PickerLocationId::Downloads);
	picker.FileTypeFilter().Append(L".json");

    // Show the picker and handle the result
	auto result = co_await picker.PickSingleFileAsync();
    if (result)
    {
        auto ui_thread = winrt::apartment_context();
        LoadingWindow().Visibility(winrt::Microsoft::UI::Xaml::Visibility::Visible);
        LoadingText().Text(L"Loading design...");
        co_await 100ms; 

        // --------------------- BACKGROUND THREAD ---------------------

        auto storageFile = winrt::Windows::Storage::StorageFile(nullptr);
        auto const& file = co_await storageFile.GetFileFromPathAsync(result.Path());
        auto const& text = co_await winrt::Windows::Storage::FileIO::ReadTextAsync(file);
        
        // App info 
		winrt::Windows::Data::Json::JsonObject infoObject = winrt::Windows::Data::Json::JsonObject::Parse(text);
		winrt::hstring appName = infoObject.GetNamedString(L"AppName");
        winrt::hstring version = infoObject.GetNamedString(L"Version");

		// Design data
		winrt::Windows::Data::Json::JsonObject designDataObject = infoObject.GetNamedObject(L"DesignData");

		// Grid dimensions
		winrt::Windows::Data::Json::JsonObject gridDimensionsObject = designDataObject.GetNamedObject(L"GridDimensions");
		m_nRows = (int)gridDimensionsObject.GetNamedNumber(L"Rows");
		m_nCols = (int)gridDimensionsObject.GetNamedNumber(L"Cols");

		// Walls
		winrt::Windows::Data::Json::JsonArray wallsArray = designDataObject.GetNamedArray(L"Walls");
        for (size_t i = 0; i < wallsArray.Size(); i++)
        {
            winrt::Windows::Data::Json::JsonObject wallObject = wallsArray.GetObjectAt(i);
			m_wallIndices.push_back({ (int)wallObject.GetNamedNumber(L"Row"), (int)wallObject.GetNamedNumber(L"Col") });
        }

		// Start and goal positions
		winrt::Windows::Data::Json::JsonObject startDimObject = designDataObject.GetNamedObject(L"StartDimensions");
		m_startIndex = { (int)startDimObject.GetNamedNumber(L"Row"), (int)startDimObject.GetNamedNumber(L"Col") };

		winrt::Windows::Data::Json::JsonObject goalDimObject = designDataObject.GetNamedObject(L"GoalDimensions");
		m_goalIndex = { (int)goalDimObject.GetNamedNumber(L"Row"), (int)goalDimObject.GetNamedNumber(L"Col") };

        co_await ui_thread;
		// --------------------- UI THREAD ---------------------
        
		// Initialize the grid based on the loaded design
		InitializeGrid_FromDimesions(m_nRows, m_nCols);

        // Set wall colors
        for (auto& wall : m_wallIndices)
        {
            m_grid[wall.X][wall.Y].isWall = true;
            m_grid[wall.X][wall.Y].m_fillColor = winrt::Microsoft::UI::Colors::DarkGray();
		}

		// Set start and goal colors
        m_grid[m_startIndex.X][m_startIndex.Y].isStart = true;
        m_grid[m_startIndex.X][m_startIndex.Y].m_fillColor = winrt::Microsoft::UI::Colors::Green();
        m_grid[m_goalIndex.X][m_goalIndex.Y].isGoal = true;
        m_grid[m_goalIndex.X][m_goalIndex.Y].m_fillColor = winrt::Microsoft::UI::Colors::Red();

        LoadingWindow().Visibility(winrt::Microsoft::UI::Xaml::Visibility::Collapsed);

		// Redraw the canvas with the loaded design
		MainCanvas().Invalidate();
    }
}

void winrt::SearchAlgorithmGame::implementation::MainWindow::ExitMenuItem_Click(winrt::Windows::Foundation::IInspectable const& sender, winrt::Microsoft::UI::Xaml::RoutedEventArgs const& e)
{
	Close();
}

winrt::Windows::Foundation::IAsyncAction winrt::SearchAlgorithmGame::implementation::MainWindow::GenerateMazeButton_Click(winrt::Windows::Foundation::IInspectable const& sender, winrt::Microsoft::UI::Xaml::RoutedEventArgs const& e)
{
    winrt::apartment_context ui_thread;

    // Enusre rows and cols are odd
	if (m_nRows % 2 == 0)
        m_nRows--;
	if (m_nCols % 2 == 0)
        m_nCols--;

	// Regenerate the grid with all walls
	InitializeGrid_FromDimesions(m_nRows, m_nCols);

	// Make every cell a wall 
    for (int i = 0; i < m_nRows; i++) {
        for (int j = 0; j < m_nCols; ++j)
        {
            m_grid[i][j].m_fillColor = winrt::Microsoft::UI::Colors::DarkGray();
            m_grid[i][j].isWall = true;
            m_grid[i][j].isStart = false;
			m_grid[i][j].isGoal = false;
        }
    };

	// Generate the maze using the recursive backtracking algorithm
    Recursive_Backtracking_Maze_Generator generator(m_gridType);
	generator.GenerateMaze(m_nRows, m_nCols, m_gridType);

	// Set the path cells to be non-walls and update their colors
    m_animationRunning = true;
	for (Point_Int pathPoint : generator.m_mazePath)
    {
        auto& gridItem = m_grid[pathPoint.X][pathPoint.Y];
        gridItem.m_fillColor = winrt::Microsoft::UI::Colors::White();
        gridItem.isWall = false;
		m_wallIndices.erase(std::remove(m_wallIndices.begin(), m_wallIndices.end(), pathPoint), m_wallIndices.end());

		if (!m_animateMazeGeneration)
            continue;

        MainCanvas().Invalidate();
        co_await 10ms;
        co_await ui_thread;
    }
    m_animationRunning = false;

    // Redraw the canvas with the loaded design
    MainCanvas().Invalidate();
}

void winrt::SearchAlgorithmGame::implementation::MainWindow::AnimateMazeGeneration_Click(winrt::Windows::Foundation::IInspectable const& sender, winrt::Microsoft::UI::Xaml::RoutedEventArgs const& e)
{
	m_animateMazeGeneration = !m_animateMazeGeneration;
}
