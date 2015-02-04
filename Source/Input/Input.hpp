// ========================================================================= //
// * Engine
// Copyright (C) 2015 Jordan Sparks. All Rights Reserved.
// Unauthorized copying of this file, via any medium is strictly prohibited.
// Proprietary and confidential.
// Written by Jordan Sparks <unixunited@live.com> January 2015.
// ========================================================================= //
// File: Input.hpp
// Author: Jordan Sparks <unixunited@live.com>
// ========================================================================= //
// Defines Input class.
// ========================================================================= //

#ifndef __INPUT_HPP__
#define __INPUT_HPP__

// ========================================================================= //

#include "stdafx.hpp"

// ========================================================================= //

class Entity;

typedef Entity* EntityPtr;

// ========================================================================= //
// Handles input events generated by SDL. Delegates those events to either the 
// player Entity or the GUI by sending a message. The pointers to these two
// objects must be set manually.
class Input
{
public:
	// Default initializes player and gui pointers to nullptr.
	explicit Input(void);

	// Empty destructor.
	~Input(void);

	// Events returned to the calling EngineState.
	enum StateEvent{
		NIL = 0,
		POP
	};

	// Processes SDL input events. Returns a StateEvent if needed.
	const StateEvent handle(const SDL_Event&);

	// Setters:

	// Sets pointer to player Entity.
	void setPlayer(const EntityPtr);

	// Sets pointer to GUI Entity.
	void setGUI(const EntityPtr);

private:
	// There are only two things we need input for: player control and UI.
	EntityPtr m_player;
	EntityPtr m_gui;
};

// ========================================================================= //

// Setters:

inline void Input::setPlayer(const EntityPtr player){
	m_player = player;
}

inline void Input::setGUI(const EntityPtr gui){
	m_gui = gui;
}

// ========================================================================= //

#endif

// ========================================================================= //