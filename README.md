#  OceanRoute Nav — Maritime Navigation Optimizer

[![C++17](https://img.shields.io/badge/C%2B%2B-17-blue.svg)](https://isocpp.org/)
[![SFML 2.6.1](https://img.shields.io/badge/SFML-2.6.1-green.svg)](https://www.sfml-dev.org/)
[![Visual Studio 2022+](https://img.shields.io/badge/Visual%20Studio-2022+-purple.svg)](https://visualstudio.microsoft.com/)
[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](LICENSE)

> An interactive desktop application for maritime route optimization featuring real-time algorithm visualization, graph-based pathfinding, and a modern deep-ocean themed UI.

---

##  Features

| Feature | Description |
|---------|-------------|
|  **Interactive Map** | 40 global ports with real-world positions, click-to-explore routing |
|  **Dijkstra Visualization** | Step-by-step cost-optimized pathfinding with animated exploration |
|  **A* Visualization** | Time-optimized pathfinding with heuristic line display |
|  **Custom Ship Routing** | Filter by shipping company, exclude ports, find optimal paths |
|  **Route Booking** | DFS-based multi-path discovery, route selection, booking management |
|  **Docking Queue** | Real-time ship queue simulation with processing animations |
|  **Multi-Leg Routes** | Linked-list based journey builder with INSERT/DELETE operations |
|  **Modern UI** | Deep ocean theme, glow effects, smooth animations, glassmorphism panels |

##  Architecture

```
src/
├── core/           # Data structures, constants, route data
│   ├── DataStructures.h    # Custom Vector<T>, Queue<T>, MinHeap
│   ├── Constants.h         # Colors, fonts, coordinates, UI sizing
│   └── RouteData.h         # Routes and Ship structs
├── graph/          # Navigation graph and pathfinding
│   ├── GraphNode.h/.cpp          # Port node and utility functions
│   ├── PortManagementSystem.h/.cpp  # Dijkstra, A*, Custom, DFS
│   └── UserRouteList.h/.cpp      # Linked list for multi-leg routes
├── ui/             # Reusable UI widgets
│   ├── Button.h/.cpp      # Animated buttons with glow effects
│   ├── Panel.h/.cpp       # Glassmorphism info panels
│   ├── StatusBar.h/.cpp   # Top/bottom navigation bars
│   └── TextInput.h/.cpp   # Text input with cursor animation
├── scenes/         # Application states (Scene pattern)
│   ├── Scene.h            # Abstract base class
│   ├── MainMenuScene      # Animated main menu
│   ├── MapViewScene       # Interactive route data explorer
│   ├── BookingScene       # Route booking with DFS paths
│   ├── DijkstraScene      # Dijkstra visualization
│   ├── AStarScene         # A* visualization
│   ├── CustomShipScene    # Custom filtered routing
│   ├── DockingScene       # Port docking queue simulation
│   └── MultiLegScene      # Multi-leg route builder
├── rendering/      # Drawing utilities
│   ├── Renderer.h/.cpp    # Thick lines, arrows, glow circles
│   └── MapRenderer.h/.cpp # Port/edge/path rendering
├── systems/        # Engine systems
│   ├── AssetManager.h/.cpp   # Font, texture, sound loading
│   └── SceneManager.h/.cpp   # Scene transitions with fades
├── user/           # User data
│   └── User.h/.cpp        # Booking storage
└── main.cpp        # Application entry point
```

##  Academic Constraints

This project follows strict academic requirements:

-  **No STL containers** — Custom `Vector<T>`, `Queue<T>`, `MinHeap` used instead
-  `std::string`, `std::cout`, file I/O, math functions are allowed
-  All pathfinding algorithms implemented from scratch
-  Linked list (`UserRouteList`) used for multi-leg route management

##  Building

### Prerequisites
- **Visual Studio 2022+** with C++ Desktop Development workload
- **SFML 2.6.1** 
- **CMake 3.16+** (bundled with Visual Studio)

### Build Steps

1. Open **Developer Command Prompt for VS**
2. Navigate to project directory
3. Run:
```bash
cmake -B build -G "Visual Studio 17 2022" -A x64
cmake --build build --config Release
```
4. Run from: `build/bin/Release/OceanRouteNav.exe`

### Alternative: Visual Studio
1. Open `CMakeLists.txt` in Visual Studio (File → Open → CMake)
2. Build with `Ctrl+Shift+B`
3. Run with `F5`

##  Controls

| Key/Action | Effect |
|-----------|--------|
| **Left Click** | Select ports, click buttons |
| **ESC** | Return to main menu |
| **SPACE** | Pause/Resume algorithm animation |
| **Mouse Hover** | Show port/route tooltips |
| **Scroll** | Scroll info panels |

##  Data Files

- `assets/routes.txt` — Shipping route database
- `assets/portCharges.txt` — Port docking fees
- `assets/map.jpg` — World map background
- `assets/fonts/` — Raleway font family

##  Author

- **Shahzad Ahmad** 
- **Rayyan Hasan**

##  License

This project is for academic purposes. SFML is licensed under the zlib/png license.
