// ========================================================================= //
// * Engine
// Copyright (C) 2015 Jordan Sparks. All Rights Reserved.
// Unauthorized copying of this file, via any medium is strictly prohibited.
// Proprietary and confidential.
// Written by Jordan Sparks <unixunited@live.com> January 2015.
// ========================================================================= //
// File: BaseComponent.hpp
// Author: Jordan Sparks <unixunited@live.com>
// ========================================================================= //
// Defines Component class.
// ========================================================================= //

#ifndef __COMPONENT_HPP__
#define __COMPONENT_HPP__

// ========================================================================= //

#include "stdafx.hpp"

// ========================================================================= //

class Component
{
public:
	// Initializes m_name to "nil".
	explicit Component(void);

	// Empty destructor.
	virtual ~Component(void) = 0;

	// Getters:

	// Returns the name of the component type.
	const std::string getName(void) const;

	// Setters:

	// Sets the name of the component (meaning the type).
	void setName(const std::string& name);

private:
	std::string m_name;
};

// ========================================================================= //

// Getters:

const std::string Component::getName(void) const{
	return m_name;
}

// Setters:

void Component::setName(const std::string& name){
	m_name = name;
}

// ========================================================================= //

typedef std::shared_ptr<Component> ComponentPtr;

// ========================================================================= //

#endif

// ========================================================================= //