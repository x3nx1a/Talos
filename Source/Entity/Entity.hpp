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
// File: Entity.hpp
// Author: Jordan Sparks <unixunited@live.com>
// ========================================================================= //
// Defines Entity class.
// ========================================================================= //

#ifndef __ENTITY_HPP__
#define __ENTITY_HPP__

// ========================================================================= //

#include "Component/ComponentDecls.hpp"
#include "stdafx.hpp"

// ========================================================================= //

typedef std::unordered_map<const std::type_info*, 
    ComponentPtr> ComponentHashTable;

// ========================================================================= //
// Class for anything in the game world, such as players, boxes,
// walls, doors, particle systems, etc. Components can be attached and 
// removed at run-time.
class Entity final
{
public:
    // Default initializes member data.
    explicit Entity(void);

    // Empty desctructor.
    ~Entity(void);

    // Calls init() on all attached components.
    void init(void);

    // Calls destroy() on all attached components.
    void destroy(void);

    // Calls update() on all attached components.
    void update(void);

    // Registers Component with the entity. Returns a pointer to the newly
    // attached Component for convenience.
    ComponentPtr attachComponent(const ComponentPtr component);

    // Unregisters component from the entity.
    void detachComponent(const ComponentPtr component);

    // Broadcasts ComponentMessage to all attached Components.
    void message(ComponentMessage& msg);

    // Getters:

    // Returns entity's ID.
    const EntityID getID(void) const;

    // Returns true if Entity has component of type T attached.
    template<typename T>
    bool hasComponent(void){
        return (m_components.count(&typeid(T)) != 0);
    }

    // Returns ComponentPtr to specified type from internal hash table.
    template<typename T>
    T* getComponent(void){
        if (m_components.count(&typeid(T)) != 0){
            return static_cast<T*>(m_components[&typeid(T)]);
        }
        else{
            return nullptr;
        }
    }

    // Returns next EntityPtr as part of the EntityPool.
    EntityPtr getNext(void) const;

    // Setters:

    // Sets a new ID for the entity.
    void setID(const EntityID id);

    // Sets the next EntityPtr for the EntityPool.
    void setNext(const EntityPtr next);

private:
    ComponentHashTable m_components;

    // Save memory for the EntityPool (see EntityPool.cpp).
    union{
        EntityID m_id;
        EntityPtr m_next;
    };
};

// ========================================================================= //

// Getters:

inline const EntityID Entity::getID(void) const{
    return m_id;
}

inline EntityPtr Entity::getNext(void) const{
    return m_next;
}

// Setters:

inline void Entity::setID(const EntityID id){
    m_id = id;
}

inline void Entity::setNext(const EntityPtr next){
    m_next = next;
}

// ========================================================================= //

#endif

// ========================================================================= //