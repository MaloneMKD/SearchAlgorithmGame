# Search Algorithm Game

A high-performance Pathfinding Visualizer built with **C++/WinRT** and **Win2D**. This application allows users to generate complex mazes and watch various search algorithms navigate them in real-time.

<img width="1919" height="1075" alt="image" src="https://github.com/user-attachments/assets/76d521e0-d02c-4b41-806f-a819c082d8c4" />

## 🚀 FeaturesReal-Time Visualization: 

Watch the algorithm explore the "frontier" and backtrack to find the shortest path.

**Procedural Maze Generation**: Implements a Randomized Depth-First Search (Recursive Backtracker) to create perfect mazes.

**8-Connected Navigation**: Supports both cardinal (4-way) and diagonal (8-way) movement logic.

**Thread-Safe Interaction**: Custom coroutine management using winrt::apartment_context to ensure smooth UI updates during heavy calculations.

**Interactive Grid:** Manually "draw" walls, set start/goal points, and edit the grid mid-simulation.

## 🛠️ Technical StackFramework: 

**WinUI 3 (Windows App SDK)Graphics:** Win2D (Immediate mode rendering via CanvasControl)

**Language:** 

**C++17**

**Concurrency**: C++/WinRT Coroutines (co_await, resume_background)

## 🧠 Algorithms Implemented

|Algorithm  | Type      | Use Case                            |
|------------|-----------|-------------------------------------|
|Dijkstra    |Weighted   |Finding the guaranteed shortest path.|
|A Search*   |Heuristic  |Efficiently finding the goal using Manhattan/Euclidean distance.|
|BFS         |Unweighted |Exploring all possible paths layer-by-layer.|
|DFS         |Unweighted |Deep exploration, also used for our Maze Generator.|

## 🎮 How to Run

**Select Action**: Use the ComboBox to select "Set Wall," "Set Start," or "Set Goal". Click on the grid you want to make a wall, start or goal node. Right-Click to erase.

**Generate Maze**: Click the Generate Maze button to trigger the iterative backtracker animation.

**Run Search**: Choose your algorithm and watch the pathfinder explore the 8-connected grid.

**Clear**: Reset the grid at any time to start fresh or try a new configuration.🏗️

## Architecture Note

The project utilizes a decoupled Logic vs. UI approach:

**Logic**: Maze and Pathfinding vectors are generated on background threads to keep the UI responsive.

**UI**: The MainCanvas_Draw loop handles the rendering of m_grid and m_wallIndices based on the state updated by the logic functions

## 📜 License
Distributed under the MIT License.
