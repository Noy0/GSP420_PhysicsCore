#ifndef _ENTITYMANAGER_H
#define _ENTITYMANAGER_H

#include <d3d9.h>
#include <d3dx9.h>
#include <List>

//Heavily Edited done by - TP  (to fix seriously crappy implemntation of a list manager)

struct EntityData
{
	int ID;
	int type;

	//Physics attribures
	D3DXVECTOR3 position;
	D3DXVECTOR3 velocity;
	D3DXVECTOR3 force;
	D3DXQUATERNION rotation;
	int physicsID;

	//Graphics attribures

	//Scripting attributes
	int scriptFlag1;
	int scriptFlag2;

	//AI attributes
		//Steering
	int steeringType;

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

	EntityData* NewEntity();
	void AddEntity(EntityData& entity);
	bool RemoveEntity(int id);
	void Clear();

	EntityData* GetEntity(int id);

	void CloneInto(EntityList& entityList);
	EntityIterator Begin();
	EntityIterator End();
};

#endif
