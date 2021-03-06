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
// File: World.hpp
// Author: Jordan Sparks <unixunited@live.com>
// ========================================================================= //
// Defines World class.
// ========================================================================= //

#ifndef __WORLD_HPP__
#define __WORLD_HPP__

// ========================================================================= //

#include "Component/ComponentDecls.hpp"
#include "stdafx.hpp"

// ========================================================================= //

class AbstractPool;

// Hash table for fast Entity lookup.
typedef std::unordered_map<EntityID, EntityPtr> EntityIDMap;
typedef std::unordered_map<const std::type_info*, 
    std::shared_ptr<AbstractPool>> PoolTable;

// ========================================================================= //
// Represents everything in the physical game world. The World holds a
// collection of Entities (which have pointers to the needed Components).
// Entities and Components are retrieved from pools in the World allocated
// during World::init().
class World final : public std::enable_shared_from_this<World>
{
public:
    // Default initializes member data.
    explicit World(void);

    // Empty destructor.
    ~World(void);

    // Passes needed dependencies into World.
    struct Dependencies;
    void injectDependencies(const Dependencies& deps);

    // Allocates the Ogre::SceneManager.
    void init(const bool usePhysics = false);

    // De-allocates all data.
    void destroy(void);

    // Pauses environment.
    void pause(void);

    // Resumes world state, assigns main camera to viewport.
    void resume(void); 

    // Updates every active Entity in the game world, updates environment.
    void update(void);

    // === //

    // Entity functions:

    // Get the next free Entity from the internal pool and return a pointer.
    EntityPtr createEntity(void);

    // Calls destroy on Entity.
    void destroyEntity(EntityPtr entity);

    // Checks if each Entity's Components have been initialized and linked. 
    // Returns true if they all have. Should be called after initial scene 
    // setup. Links all components together by calling onComponentAttached()
    // for each component. Adds Entities to Systems as needed.
    const bool setupEntities(void) const;

    // Returns pointer to Entity if it exists in World.
    EntityPtr getEntityPtr(const EntityID id);

    // === //

    // Physics functions:

    // Creates physics scene (PScene).
    void initPhysics(void);

    // === //
    
    // Network functions:

    // Initializes Server, update will process Server updates.
    void initServer(const int port, const std::string& username);

    // Destroys Server.
    void destroyServer(void);

    // Initializes Client, update will process Client updates.
    void initClient(void);

    // Destroys Client.
    void destroyClient(void);

    // === //

    // Component creation:

    // Specialize return types for each Component. This is necessary
    // because with template specialization, it's not possible to specify 
    // the return type as the generic type.
    template<typename T>
    struct componentReturn{ typedef T* type; };    

    // Creates component from pool, initializes it, and attaches it 
    // to the entity.
    template<typename T>
    typename componentReturn<T>::type attachComponent(EntityPtr entity);

    // === //

    // Systems:

    // Inserts new System into SystemManager.
    void addSystem(System* system);

    // Attaches Entity to internal System if it meets the requirements of
    // any registered system.
    void addEntityToSystem(EntityPtr entity);

    // === //

    // Input:

    // Handles input using internal Input object, returns CommandPtr that was
    // generated.
    CommandPtr handleInput(const SDL_Event& e);

    // === //

    // Music:

    // Starts background music on loop.
    void playMusic(const std::string& file, const uint32_t instance = 0);

    // Stops background music of specified instance.
    void stopMusic(const uint32_t instance = 0);

    // === //

    // Getters:

    // Returns pointer to Ogre::Root object.
    Ogre::Root* getRoot(void) const;

    // Returns pointer to Ogre::SceneManager for this world.
    Ogre::SceneManager* getSceneManager(void) const;

    // Returns pointer to the Ogre::Viewport for this world.
    Ogre::Viewport* getViewport(void) const;

    // Returns pointer to internal Environment.
    std::shared_ptr<Environment> getEnvironment(void) const;

    // Returns graphics settings data.
    const Graphics& getGraphics(void) const;

    // Returns pointer to player-controlled Entity.
    EntityPtr getPlayer(void) const;

    // Returns pointer to camera which views the game world.
    CameraComponentPtr getMainCamera(void) const;

    // Returns pointer to PScene (physics scene).
    std::shared_ptr<PScene> getPScene(void) const;

    // Returns pointer to Network.
    Network* getNetwork(void) const;

    // Returns reference to internal Input instance.
    Input* getInput(void) const;

    // Returns pointer to internal irrKlang sound engine.
    irrklang::ISoundEngine* getSoundEngine(void) const;

    // Setters:

    // Sets pointer to Entity controlled by player.
    void setPlayer(const EntityPtr);

    // Sets pointer to main camera which views the game world.
    void setMainCamera(CameraComponentPtr);

    // === // 

    // Dependency data that is injected from Engine.
    struct Dependencies{
        Ogre::Root* root;
        Ogre::Viewport* viewport;
        std::shared_ptr<Physics> physics;
        Input* input;
        Graphics graphics;
        std::shared_ptr<Network> server;
        std::shared_ptr<Network> client;
        irrklang::ISoundEngine* soundEngine;
    };

private:
    // Ogre3D.
    Ogre::Root* m_root;
    Ogre::SceneManager* m_scene;
    Ogre::Viewport* m_viewport;
    
    // Environment control.
    std::shared_ptr<Environment> m_environment;

    // Graphics settings.
    Graphics m_graphics;

    // PhysX.    
    std::shared_ptr<Physics> m_physics;
    std::shared_ptr<PScene> m_PScene;
    bool m_usePhysics;

    // Network.
    Network* m_network;
    std::shared_ptr<Network> m_server;
    std::shared_ptr<Network> m_client;

    // Entity pool.
    std::shared_ptr<EntityPool> m_entityPool;
    EntityIDMap m_entityIDMap;

    // Component pools for creating all components.
    PoolTable m_componentPools;

    // System manager for entity-component management.
    std::shared_ptr<SystemManager> m_systemManager;

    // The Entity the player controls.
    EntityPtr m_player;
    bool m_hasPlayer;

    // Main camera which renders to viewport.
    CameraComponentPtr m_mainCameraC;

    // Input component.
    Input* m_input;

    // Audio.
    irrklang::ISoundEngine* m_soundEngine;
    std::vector<irrklang::ISound*> m_sounds;
    uint32_t m_musicCount;    
};

// ========================================================================= //

// Getters:

inline Ogre::Root* World::getRoot(void) const{
    return m_root;
}

inline Ogre::SceneManager* World::getSceneManager(void) const{
    return m_scene;
}

inline Ogre::Viewport* World::getViewport(void) const{
    return m_viewport;
}

inline std::shared_ptr<Environment> World::getEnvironment(void) const{
    return m_environment;
}

inline const Graphics& World::getGraphics(void) const{
    return m_graphics;
}

inline EntityPtr World::getPlayer(void) const{
    return m_player;
}

inline CameraComponentPtr World::getMainCamera(void) const{
    return m_mainCameraC;
}

inline std::shared_ptr<PScene> World::getPScene(void) const{
    return m_PScene;
}

inline Network* World::getNetwork(void) const{
    return m_network;
}

inline Input* World::getInput(void) const{
    return m_input;
}

inline irrklang::ISoundEngine* World::getSoundEngine(void) const{
    return m_soundEngine;
}

// Setters:

inline void World::setMainCamera(CameraComponentPtr cameraC){
    m_mainCameraC = cameraC;
}

// ========================================================================= //

#endif

// ========================================================================= //