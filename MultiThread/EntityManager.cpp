#include "EntityManager.h"
#include "EntityPropertyManager.h"


EntityData::EntityData(): Position(0, 0, 0), Velocity(0, 0, 0), Force(0, 0, 0), Rotation(0, 0, 0, 1),
						  PhysicsID(-1), SteeringType(0), ScriptFlag1(0), ScriptFlag2(0) {}

void EntityData::SetData(int type, void* value)
{
	switch (type)
	{
	case EDTYPE_POSITION:
		Position = *((D3DXVECTOR3*)value);
		break;

	case EDTYPE_VELOCITY:
		Velocity = *((D3DXVECTOR3*)value);
		break;

	case EDTYPE_FORCE:
		Force = *((D3DXVECTOR3*)value);
		break;

	case EDTYPE_ROTATION:
		Rotation = *((D3DXQUATERNION*)value);
		break;

	case EDTYPE_PHYSICSID:
		PhysicsID = *((int*)value);
		break;

	case EDTYPE_SCRIPTFLAG1:
		ScriptFlag1 = *((int*)value);
		break;

	case EDTYPE_SCRIPTFLAG2:
		ScriptFlag2 = *((int*)value);
		break;

	case EDTYPE_STEERINGTYPE:
		SteeringType = *((int*)value);
		break;
	}
}



EntityManager::EntityManager()
{
	m_NextID = 0;
}

EntityManager::~EntityManager()
{
}

void EntityManager::Clear()
{
	m_Items.clear();
}

int EntityManager::AddItem(int type, int *id_OUT)
{
	pair<int, EntityData> newitem;
	newitem.second.Type = type;
	newitem.first = m_NextID;
	if(id_OUT != 0)
		*id_OUT = m_NextID;
	m_Items.push_back(newitem);
	return m_NextID++;
}

bool EntityManager::RemoveItem(int id)
{
	for(int i = 0 ;i < m_Items.size(); ++i)
	{
		if(m_Items[i].first == id)
		{
			m_Items.erase(m_Items.begin() + i);
			return true;
		}
	}
	return false;
}

void EntityManager::SetValue(int *id, int type, void *value)
{
	for(int i = 0; i < m_Items.size(); ++i)
	{
		if(m_Items[i].first == *id)
		{
			m_Items[i].second.SetData(type, value);
		}
	}
}