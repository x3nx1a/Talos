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
// File: ParticleComponent.hpp
// Author: Jordan Sparks <unixunited@live.com>
// ========================================================================= //
// Defines ParticleComponent class.
// ========================================================================= //

#ifndef __PARTICLECOMPONENT_HPP__
#define __PARTICLECOMPONENT_HPP__

// ========================================================================= //

#include "Component.hpp"

// ========================================================================= //
// Holds a particle system.
class ParticleComponent : public Component
{
public:
    // Default initializes member data.
    explicit ParticleComponent(void);

    // Empty destructor.
    virtual ~ParticleComponent(void) override;

    
    virtual void init(void) override;

    virtual void destroy(void) override;

    virtual void update(void) override;

    virtual void message(ComponentMessage& msg) override;

    // Component functions:

    // Creates particle system under specified scene node.
    void setup(Ogre::SceneNode* node);

private:
    Ogre::ParticleSystem* m_ps;
};

// ========================================================================= //

#endif

// ========================================================================= //