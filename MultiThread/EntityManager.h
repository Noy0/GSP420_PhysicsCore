#ifndef _ENTITYMANAGER_H
#define _ENTITYMANAGER_H

#include <d3d9.h>
#include <d3dx9.h>
#include "BaseManager.h"


enum EEDType
{
	EDTYPE_POSITION,
	EDTYPE_VELOCITY,
	EDTYPE_FORCE,
	EDTYPE_ROTATION,
	EDTYPE_PHYSICSID,
	EDTYPE_SCRIPTFLAG1,
	EDTYPE_SCRIPTFLAG2,
	EDTYPE_STEERINGTYPE
};

struct EntityData
{
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

	void SetData(int type, void* value);

};

class EntityManager : public BaseManager<EntityData>
{
public:
	EntityManager();
	~EntityManager();

	void Clear();
	int AddItem(int type,int *id_OUT);
	bool RemoveItem(int id);
	void SetValue(int *id, int type, void* value);
private:
};

#endif
