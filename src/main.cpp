/*=======================================================================
 * main.cpp — OceanRoute Nav Entry Point
 * Maritime Navigation Optimizer
 *
 * Modern modular architecture with scene management system.
 * Authors: Shahzad Ahmad (24i-3090), Syed Muhammad Rayyan Hassan (24i-3000)
 *=======================================================================*/

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>

// Core
#include "core/Constants.h"
#include "core/DataStructures.h"
#include "core/RouteData.h"

// Systems
#include "systems/AssetManager.h"
#include "systems/SceneManager.h"

// Graph
#include "graph/PortManagementSystem.h"
#include "graph/UserRouteList.h"

// User
#include "user/User.h"

// Scenes
#include "scenes/MainMenuScene.h"
#include "scenes/MapViewScene.h"
#include "scenes/BookingScene.h"
#include "scenes/DijkstraScene.h"
#include "scenes/AStarScene.h"
#include "scenes/CustomShipScene.h"
#include "scenes/DockingScene.h"
#include "scenes/MultiLegScene.h"

using namespace std;
using namespace sf;

int main() {
    cout << "============================================\n";
    cout << "  " << Config::APP_TITLE << " v" << Config::APP_VERSION << "\n";
    cout << "  " << Config::APP_SUBTITLE << "\n";
    cout << "  " << Config::AUTHORS << "\n";
    cout << "============================================\n\n";

    // ── Create Window ───────────────────────────────────────────────
    RenderWindow window(
        VideoMode(Config::DEFAULT_WIDTH, Config::DEFAULT_HEIGHT),
        Config::APP_TITLE + " - " + Config::APP_SUBTITLE,
        Style::Close | Style::Titlebar
    );
    window.setFramerateLimit(Config::FRAMERATE_LIMIT);

    // ── Load Assets ─────────────────────────────────────────────────
    AssetManager assets;
    if (!assets.loadAll()) {
        cout << "[FATAL] Failed to load essential assets.\n";
        cout << "Make sure the 'assets/' directory is in the working directory.\n";
        return 1;
    }

    // ── Initialize Port Management System ───────────────────────────
    PortManagementSystem portSystem;
    portSystem.readFromFile(Config::Assets::DATA_ROUTES,
                            Config::Assets::DATA_CHARGES);
    portSystem.placeAllPortCircles();

    // ── User Data ───────────────────────────────────────────────────
    User user;

    // ── Scene Manager ───────────────────────────────────────────────
    SceneManager sceneManager;

    // Create all scenes
    MainMenuScene   mainMenuScene(assets, sceneManager);
    MapViewScene    mapViewScene(assets, sceneManager, portSystem);
    BookingScene    bookingScene(assets, sceneManager, portSystem, user);
    DijkstraScene   dijkstraScene(assets, sceneManager, portSystem);
    AStarScene      aStarScene(assets, sceneManager, portSystem);
    CustomShipScene customShipScene(assets, sceneManager, portSystem);
    DockingScene    dockingScene(assets, sceneManager, portSystem);
    MultiLegScene   multiLegScene(assets, sceneManager, portSystem);

    // Register scenes
    sceneManager.registerScene(SceneType::MAIN_MENU,   &mainMenuScene);
    sceneManager.registerScene(SceneType::MAP_VIEW,    &mapViewScene);
    sceneManager.registerScene(SceneType::BOOKING,     &bookingScene);
    sceneManager.registerScene(SceneType::DIJKSTRA,    &dijkstraScene);
    sceneManager.registerScene(SceneType::ASTAR,       &aStarScene);
    sceneManager.registerScene(SceneType::CUSTOM_SHIP, &customShipScene);
    sceneManager.registerScene(SceneType::DOCKING,     &dockingScene);
    sceneManager.registerScene(SceneType::MULTI_LEG,   &multiLegScene);

    // Start at main menu
    sceneManager.switchTo(SceneType::MAIN_MENU);

    // ── Game Clock ──────────────────────────────────────────────────
    Clock deltaClock;

    cout << "[OceanRoute] Application started successfully.\n";

    // ── Main Loop ───────────────────────────────────────────────────
    while (window.isOpen()) {
        float dt = deltaClock.restart().asSeconds();

        // ── Event Handling ──────────────────────────────────────────
        Event event;
        while (window.pollEvent(event)) {
            if (event.type == Event::Closed) {
                window.close();
                break;
            }

            // Forward events to current scene
            Scene* currentScene = sceneManager.getCurrentScene();
            if (currentScene) {
                currentScene->handleEvent(event);
            }
        }

        // ── Update ──────────────────────────────────────────────────
        sceneManager.update(dt);
        Scene* currentScene = sceneManager.getCurrentScene();
        if (currentScene) {
            currentScene->update(dt);
        }

        // ── Render ──────────────────────────────────────────────────
        window.clear(Config::Colors::BG_DEEP);
        sceneManager.render(window);
        window.display();
    }

    cout << "\n[OceanRoute] Application closed.\n";
    return 0;
}
