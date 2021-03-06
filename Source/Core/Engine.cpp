// ========================================================================= //
// Talos - A 3D game engine with network multiplayer.
// Copyright(C) 2015 Jordan Sparks <unixunited@live.com>
//
// This program is free software; you can redistribute it and / or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 3
// of the License, or(at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU General Public License for more details.
// 
// You should have received a copy of the GNU General Public License
// along with this program. If not, see <http://www.gnu.org/licenses/>.
// ========================================================================= //
// File: Engine.cpp
// Author: Jordan Sparks <unixunited@live.com>
// ========================================================================= //
// Implements Engine class.
// ========================================================================= //

#include "Engine.hpp"
#include "EngineNotifications.hpp"
#include "EngineState/GameState.hpp"
#include "EngineState/LobbyState.hpp"
#include "EngineState/MainMenuState.hpp"
#include "EngineState/StartupState.hpp"
#include "Input/Input.hpp"
#include "Network/Client/Client.hpp"
#include "Network/Server/Server.hpp"
#include "Physics/Physics.hpp"
#include "Resources.hpp"
#include "Talos.hpp"
#include "World/World.hpp"

// ========================================================================= //

Engine::Engine(void) :
m_root(nullptr),
m_renderWindow(nullptr),
m_viewport(nullptr),
m_log(nullptr),
m_timer(nullptr),
m_sdlWindow(nullptr),
m_ceguiRenderer(nullptr),
m_physics(nullptr),
m_server(nullptr),
m_client(nullptr),
m_input(nullptr),
m_soundEngine(nullptr),
m_states(),
m_stateStack(),
m_active(false)
{
    m_states.reserve(EngineStateID::NumStates);
}

// ========================================================================= //

Engine::~Engine(void)
{

}

// ========================================================================= //

bool Engine::init(void)
{
    // === //

    // Ogre3D and SDL:

    // Create logging system.
    Ogre::LogManager* logMgr = new Ogre::LogManager();
    m_log = Ogre::LogManager::getSingleton().createLog("Talos.log");

    // Allocate Talos Log singleton.
    new Talos::Log();

    // Initialize Ogre's root component.
    m_root = new Ogre::Root();

    // Set render system.
    Ogre::RenderSystem* rs = m_root->getRenderSystemByName(
        "Direct3D9 Rendering Subsystem");
    if (!(rs->getName() == "Direct3D9 Rendering Subsystem")){
        return false;
    }
    m_root->setRenderSystem(rs);

    // Initialize Ogre root with render system.
    m_root->initialise(false);

    // Initialize SDL.
    if (SDL_Init(SDL_INIT_VIDEO | 
        SDL_INIT_GAMECONTROLLER | 
        SDL_INIT_JOYSTICK) != 0){
        return false;
    }   

    // Create the main window with SDL.
    const int width = 1280;
    const int height = 960;
    m_sdlWindow = SDL_CreateWindow(
        "Engine",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        width,
        height,
        SDL_WINDOW_RESIZABLE);

    // Bind mouse to window.
    SDL_ShowCursor(0);

    // Give the window handle to Ogre.
    SDL_SysWMinfo wmInfo;
    SDL_GetVersion(&wmInfo.version);
    if (SDL_GetWindowWMInfo(m_sdlWindow, &wmInfo) == SDL_FALSE){
        return false;
    }
    Ogre::String wHandle;
#ifdef WIN32
    wHandle = Ogre::StringConverter::toString(
        reinterpret_cast<unsigned long>(wmInfo.info.win.window));
#elif __LINUX__
    wHandle = Ogre::StringConverter::toString(
        reinterpret_cast<unsigned long>(wmInfo.info.x11.window));
#endif

    Ogre::NameValuePairList params;
    params["externalWindowHandle"] = wHandle;
    params["vsync"] = "true";
    params["FSAA"] = "16";
    m_renderWindow = m_root->createRenderWindow("Talos",
                                                width,
                                                height,
                                                false,
                                                &params);

    Ogre::MaterialManager::getSingleton().
        setDefaultTextureFiltering(Ogre::TFO_ANISOTROPIC);

    // Initialize Ogre viewport for render window.
    m_viewport = m_renderWindow->addViewport(nullptr);
    m_viewport->setBackgroundColour(Ogre::ColourValue::Black);

    /*m_viewport->setClearEveryFrame(false);
    m_renderWindow->setAutoUpdated(false);*/

    // Activate the  Ogre render window.
    m_renderWindow->setActive(true);

    // Allocate the main Ogre timer.
    m_timer.reset(new Ogre::Timer());
    m_timer->reset(); // Activate timer.

    // Load graphics settings.
    // @TODO: Load from config file.
    m_graphics.meshes = Graphics::Setting::High;
    m_graphics.textures = Graphics::Setting::High;
    m_graphics.shadows = Graphics::Setting::Off;
    m_graphics.ssao = Graphics::Setting::Off;
#ifdef _DEBUG
    m_graphics.ocean = Graphics::Setting::Low;
#else
    m_graphics.ocean = Graphics::Setting::High;
#endif
    m_graphics.sky = Graphics::Setting::High;    

    // === //

    // CEGUI:

    // Bootstrap the rendering system.
    m_ceguiRenderer = &CEGUI::OgreRenderer::bootstrapSystem(
        *(m_root->getRenderTarget("Talos"))); 

    // === //

    // PhysX:

    m_physics.reset(new Physics());
    if (m_physics->init() == false){
        return false;
    }

    // === //

    // Network:
    m_server.reset(new Server());
    m_client.reset(new Client());

    // === //

    // Audio:

    m_soundEngine = irrklang::createIrrKlangDevice();
    if (!m_soundEngine){
        throw std::exception("Failed to create irrKlang sound engine");
    }

    // === //

    // Engine:

    // Allocate input handler.
    m_input.reset(new Input());

    // Register all needed game states.
    // @TODO: Define these in data.
    this->registerState(EngineStateID::Startup);
    this->registerState(EngineStateID::MainMenu);
    this->registerState(EngineStateID::Lobby);
    this->registerState(EngineStateID::Game);

    // === //

    return true;
}

// ========================================================================= //

void Engine::shutdown(void)
{
    m_soundEngine->drop();
    m_physics->destroy();
    delete m_root;
    delete Talos::Log::getSingletonPtr();
    delete Ogre::LogManager::getSingletonPtr();
}

// ========================================================================= //

void Engine::start(const EngineStateID id)
{
    m_active = true;

    // Push the specified state onto the active stack and run the game loop.
    this->pushState(id);

    float prev = m_timer->getMilliseconds();
    float lag = 0.0;

    while (m_active == true){
        // Check for window closing.
        if (m_renderWindow->isClosed()){
            m_active = false;
        }

        // Update the engine if the render window is active.
        if (m_renderWindow->isActive()){
            //Ogre::WindowEventUtilities::messagePump();

            float current = m_timer->getMilliseconds();
            float elapsed = current - prev;
            prev = current;
            lag += elapsed;

            // Update the current state.
            while (lag >= Talos::MS_PER_UPDATE && m_active == true){
                m_stateStack.top()->update();

                lag -= Talos::MS_PER_UPDATE;
            }

            //m_root->getRenderSystem()->clearFrameBuffer(Ogre::FBT_COLOUR | Ogre::FBT_DEPTH);

            // Render the updated frame, compensating for lag.
            m_root->renderOneFrame(lag / Talos::MS_PER_UPDATE);
        }
    }
}

// ========================================================================= //

void Engine::registerState(const EngineStateID id)
{
    EngineStatePtr state(nullptr);

    switch (id){
    default:
        return;

    case EngineStateID::Startup:
        state.reset(new StartupState());
        break;

    case EngineStateID::MainMenu:
        state.reset(new MainMenuState());
        break;

    case EngineStateID::Lobby:
        state.reset(new LobbyState());
        break;

    case EngineStateID::Game:
        state.reset(new GameState());
        break;
    }

    Assert(state != nullptr, "Invalid EngineState");

    // Inject dependencies from Engine into World.
    World::Dependencies deps;
    deps.root = m_root;
    deps.viewport = m_viewport;
    deps.physics = m_physics;
    deps.input = m_input.get();
    deps.graphics = m_graphics;
    deps.server = m_server;
    deps.client = m_client;
    deps.soundEngine = m_soundEngine;
    state->getWorld()->injectDependencies(deps);

    // Add self as an Observer to listen for events.
    state->getSubject().addObserver(this);

    // Add state to list of states.
    m_states.push_back(state);

    Talos::Log::getSingleton().log("Registered EngineStateID " +
                                   toString(id) + " to Engine.");
}

// ========================================================================= //

void Engine::pushState(const EngineStateID id)
{
    Assert(id < EngineStateID::NumStates, "Invalid EngineStateID");

    // Pause current state.
    if (m_stateStack.empty() == false){
        m_stateStack.top()->setActive(false);
        m_stateStack.top()->pause();
    }

    // Get a pointer to the specified state and push it into active state stack.
    EngineStatePtr state = m_states[id];
    m_stateStack.push(state);

    // Initialize the state.
    state->setActive(true);
    state->enter();

    Talos::Log::getSingleton().log("Pushed engine state ID " + toString(id));
}

// ========================================================================= //

void Engine::popState(void)
{
    EngineStatePtr state = m_stateStack.top();
    state->setActive(false);
    state->exit();

    Talos::Log::getSingleton().log("Popped engine state ID " + 
                                   toString(state->getID()));

    m_stateStack.pop();
    // If there are no more states awaiting execution, shutdown the engine.
    if (m_stateStack.empty() == true){
        m_active = false;
    }
    else{
        // Resume last state.
        m_stateStack.top()->setActive(true);
        m_stateStack.top()->resume();
        Talos::Log::getSingleton().log("Resumed engine state ID " +
                                       toString(m_stateStack.top()->getID()));
    }
}

// ========================================================================= //

void Engine::popAndPushState(const EngineStateID id)
{
    // Pop current state.
    EngineStatePtr state = m_stateStack.top();
    state->setActive(false);
    state->exit();
    m_stateStack.pop();
    
    Talos::Log::getSingleton().log("Popped engine state ID " +
                                   toString(state->getID()));

    // Push new state.
    state = m_states[id];
    m_stateStack.push(state);

    // Initialize.
    state->setActive(true);
    state->enter();

    Talos::Log::getSingleton().log("Pushed engine state ID " + toString(id));
}

// ========================================================================= //

void Engine::onNotify(const unsigned int id, const unsigned int arg)
{
    switch (id){
    default:
        break;

    case EngineNotification::Pop:
        this->popState();
        break;

    case EngineNotification::Push:
        this->pushState(static_cast<EngineStateID>(arg));
        break;

    case EngineNotification::PopAndPush:
        this->popAndPushState(static_cast<EngineStateID>(arg));
        break;
    }
}

// ========================================================================= //