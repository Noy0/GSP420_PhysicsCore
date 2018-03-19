#ifndef _ENTITYMANAGER_H
#define _ENTITYMANAGER_H

#include <d3d9.h>
#include <d3dx9.h>
#include <List>
#include "PhysicsMat.h"

//Heavily Edited done by - TP  (to fix seriously crappy implemntation of a list manager)

struct EntityData
{
public:
	int ID;
	int physicsID;
	int type;

	//deprecated to match UML -DN
	////Physics attribures
	//D3DXVECTOR3 position;
	//D3DXVECTOR3 velocity;
	//D3DXVECTOR3 force;
	//D3DXQUATERNION rotation;
	//end deprecation

//private:
	//additions made for GSP420 -DN
	D3DXVECTOR3 position;
	D3DXQUATERNION rotation;
	D3DXVECTOR3 velocity;
	D3DXVECTOR3 force;
	PhysicsMat* mat;
	TypePhysics isKinematic;

public:
	//methods
	PhysicsMat getPMaterial();
	TypePhysics getIsKinematic();
	D3DXVECTOR3 getPosition();
	D3DXVECTOR3 getVelocity();
	D3DXVECTOR3 getAcceleration();
	D3DXVECTOR3 getMomentum();
	D3DXVECTOR3 getForce();
	D3DXQUATERNION getRotation();
	D3DXQUATERNION getAngularSpeed();
	D3DXQUATERNION getAngularAcceleration();
	D3DXQUATERNION getAngularMomentum();
	D3DXQUATERNION getAngularForce();
	void setPhysicsMat(PhysicsMat mat);
	void setIsKinematic(TypePhysics mode);
	void setPosition(D3DXVECTOR3 position);
	void setVelocity(D3DXVECTOR3 velocity);
	void setForce(D3DXVECTOR3 force);
	void setRotation(D3DXVECTOR3 rotation);
	void setAngularVelocity(D3DXVECTOR3 angularVelocity);
	void setAngularForce(D3DXVECTOR3 angularForce);
	void translate(D3DXVECTOR3 translation);
	//end additions -DN



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
