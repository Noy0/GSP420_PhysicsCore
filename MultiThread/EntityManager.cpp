#include "EntityManager.h"
#include "EntityPropertyManager.h"

//Heavily Edited done by - TP


EntityData::EntityData(): Position(0, 0, 0), Velocity(0, 0, 0), Force(0, 0, 0), Rotation(0, 0, 0, 1),
						  PhysicsID(-1), SteeringType(0), ScriptFlag1(0), ScriptFlag2(0) {}





EntityManager::EntityManager()
{
	nextID = 0;
}

EntityManager::~EntityManager()
{
	entities.clear();
}



EntityData* EntityManager::newEntity()
{
	EntityData temp;
	temp.ID = nextID++;
	entities.push_back(temp);
	return &entities.back();
}

void EntityManager::addEntity(EntityData& entity)
{
	entities.push_back(entity);
}

bool EntityManager::removeEntity(int id)
{
	for(EntityIterator itr = entities.begin(); itr != entities.end(); ++itr)
	{
		if(itr->ID == id)
		{
			itr = entities.erase(itr);
			return true;
		}
	}
	return false;
}

void EntityManager::clear()
{
	entities.clear();
}



EntityData* EntityManager::getEntity(int id)
{
	for (EntityIterator itr = entities.begin(); itr != entities.end(); ++itr)
	{
		if (itr->ID == id)
		{
			return &(*itr);
		}
	}
	return nullptr;
}



void EntityManager::cloneInto(EntityList& entityList)
{
	entityList = entities;
}

EntityIterator EntityManager::begin()
{
	return entities.begin();
}

EntityIterator EntityManager::end()
{
	return entities.end();
}