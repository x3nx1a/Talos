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
// File: Engine.hpp
// Author: Jordan Sparks <unixunited@live.com>
// ========================================================================= //
// Defines Engine class.
// ========================================================================= //

#ifndef __ENGINE_HPP__
#define __ENGINE_HPP__

// ========================================================================= //

#include "EngineState/EngineState.hpp"
#include "Observer/Observer.hpp"
#include "stdafx.hpp"

// ========================================================================= //

class Input;
class Physics;

typedef std::stack<EngineStatePtr> EngineStateStack;
typedef std::vector<EngineStatePtr> EngineStateList;

// ========================================================================= //
// Holds rendering components, manages engine state stack.
class Engine final : public Observer
{
public:
    // Reserves contiguous memory for EngineState's.
    explicit Engine(void);

    // Empty destructor.
    virtual ~Engine(void) override;

    // Sets up initial rendering components and stack for engine states.
    bool init(void);

    // Frees Engine's data and resources.
    void shutdown(void);

    // Updates the active state and calculates elapsed time. This is the main
    // game update loop.
    void start(const EngineStateID);

    // Allocates an EngineState object, adds it to the list of states, and 
    // injects dependencies.
    void registerState(const EngineStateID);

    // Pushes a new active state onto the active state stack.
    void pushState(const EngineStateID);

    // Pops the active state off the active state stack, activating the next one.
    // If the stack is empty, the engine is shut down.
    void popState(void);

    // Receive notifications from Subjects (EngineStates).
    virtual void onNotify(const unsigned int) override;

    // Enumerations:

    enum StateID{
        STATE_INTRO = 0,
        STATE_MAIN_MENU,
        STATE_GAME,
        STATE_PAUSED,

        NUM_STATES
    };

    enum Notification{
        POP = 1
    };

private:
    // Ogre3D components.
    Ogre::Root*    m_root;
    Ogre::RenderWindow* m_renderWindow;
    Ogre::Viewport*    m_viewport;
    Ogre::Log* m_log;
    std::shared_ptr<Ogre::Timer> m_timer; // The core engine timer.

    // SDL components.
    SDL_Window* m_sdlWindow;

    // CEGUI components.
    CEGUI::OgreRenderer* m_ceguiRenderer;

    // Physics.
    std::shared_ptr<Physics> m_physics;

    // Input handler.
    std::shared_ptr<Input> m_input;

    // State management.
    EngineStateList m_states;
    EngineStateStack m_stateStack;

    // Engine data.
    bool m_active;
};

// ========================================================================= //

#endif

// ========================================================================= //