// ========================================================================= //
// * Engine
// Copyright (C) 2015 Jordan Sparks. All Rights Reserved.
// Unauthorized copying of this file, via any medium is strictly prohibited.
// Proprietary and confidential.
// Written by Jordan Sparks <unixunited@live.com> January 2015.
// ========================================================================= //
// File: GraphicsComponent.cpp
// Author: Jordan Sparks <unixunited@live.com>
// ========================================================================= //
// Implements GraphicsComponent class.
// ========================================================================= //

#include "ComponentMessage.hpp"
#include "ModelComponent.hpp"
#include "World/World.hpp"

// ========================================================================= //

ModelComponent::ModelComponent(const std::string& meshFilename) :
Component(),
m_entity(nullptr),
m_meshFilename(meshFilename)
{
	this->setName("ModelComponent");
}

// ========================================================================= //

ModelComponent::~ModelComponent(void)
{

}

// ========================================================================= //

void ModelComponent::init(EntityPtr entity, World& world)
{
	m_entity = world.getSceneManager()->createEntity(m_meshFilename);
}

// ========================================================================= //

void ModelComponent::destroy(EntityPtr entity, World& world)
{

}

// ========================================================================= //

void ModelComponent::update(EntityPtr entity, World& world)
{

}

// ========================================================================= //

void ModelComponent::message(const ComponentMessage& msg)
{

}

// ========================================================================= //