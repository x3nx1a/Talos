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
// File: Assert.hpp
// Author: Jordan Sparks <unixunited@live.com>
// ========================================================================= //
// Defines Assert macro.
// ========================================================================= //

#ifndef __ASSERT_HPP__
#define __ASSERT_HPP__

// ========================================================================= //
// Offers user a chance to break into the debugger when an assertion fails.
extern const bool CustomAssert(const bool, 
                               const char*, 
                               const int, 
                               const char*);

// ========================================================================= //

#ifdef _DEBUG
#define Assert(exp, desc)\
if (CustomAssert(static_cast<const bool>(exp), desc, __LINE__, __FILE__))\
    { _asm { int 3 } } // Triggers break in debugger.
#else
#define Assert(exp, desc) ;
#endif

// ========================================================================= //

#endif

// ========================================================================= //