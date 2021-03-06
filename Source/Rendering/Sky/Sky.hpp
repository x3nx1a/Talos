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
// File: Sky.hpp
// Author: Jordan Sparks <unixunited@live.com>
// ========================================================================= //
// Defines Sky interface.
// ========================================================================= //

#ifndef __SKY_HPP__
#define __SKY_HPP__

// ========================================================================= //

#include "stdafx.hpp"

// ========================================================================= //
// Interface for rendering a sky in the game world.
class Sky
{
public:
    explicit Sky(void) { }

    virtual ~Sky(void) = 0 { }

    virtual void loadPreset(const SkyPreset&) { }

    virtual void destroy(void) = 0;

    virtual void update(void) = 0;

    // Getters:

    virtual const Ogre::Real getTime(void) const { 
        return 0.f;
    }

    virtual const Ogre::Vector3 getSunDirection(void) const { 
        return Ogre::Vector3::ZERO;
    }

    virtual const Ogre::Vector3 getMoonDirection(void) const { 
        return Ogre::Vector3::ZERO;
    }

    virtual const Ogre::Real calcSkydomeRadius(void) const { 
        return 0.f;
    }
};

// ========================================================================= //

#endif

// ========================================================================= //