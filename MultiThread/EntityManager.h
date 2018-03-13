#ifndef _ENTITYMANAGER_H
#define _ENTITYMANAGER_H

#include <d3d9.h>
#include <d3dx9.h>
#include <List>

struct EntityData
{
	int ID;
	int Type;

	//Physics attribures
	D3DXVECTOR3 Position;
	D3DXVECTOR3 Velocity;
	D3DXVECTOR3 Force;
	D3DXQUATERNION Rotation;
	int PhysicsID;

	//Graphics attribures

	//Scripting attributes
	int ScriptFlag1;
	int ScriptFlag2;

	//AI attributes
		//Steering
	int SteeringType;

	EntityData();
};

typedef std::list<EntityData> EntityList;
typedef EntityList::iterator EntityIterator;
class EntityManager
{
	EntityList entities;
	int nextID;

public:
	EntityManager();
	~EntityManager();

	EntityData* EntityManager::newEntity();
	void addEntity(EntityData& entity);
	bool removeEntity(int id);
	void clear();

	EntityData* getEntity(int id);

	void cloneInto(EntityList& entityList);
	EntityIterator begin();
	EntityIterator end();
};

#endif
