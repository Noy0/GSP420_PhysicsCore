#include "EntityManager.h"
#include "EntityPropertyManager.h"

//Heavily Edited done by - TP  (to fix seriously crappy implemntation of a list manager)


EntityData::EntityData(): position(0, 0, 0), velocity(0, 0, 0), force(0, 0, 0), rotation(0, 0, 0, 1),
						  physicsID(-1), steeringType(0), scriptFlag1(0), scriptFlag2(0) {}



//EntityData methods - DN
PhysicsMat EntityData::getPMaterial()
{
	return *mat;
}

TypePhysics EntityData::getIsKinematic()
{
	return isKinematic;
}

D3DXVECTOR3 EntityData::getPosition()
{
	return position;
}

D3DXVECTOR3 EntityData::getVelocity()
{
	return velocity;
}

D3DXVECTOR3 EntityData::getAcceleration()
{
	return (force / mat->mass);
}

D3DXVECTOR3 EntityData::getMomentum()
{
	return (velocity * mat->mass);
}

D3DXVECTOR3 EntityData::getForce()
{
	return force;
}

D3DXQUATERNION EntityData::getRotation()
{
	return rotation;
}

D3DXQUATERNION EntityData::getAngularSpeed()
{
	//?
}

D3DXQUATERNION EntityData::getAngularAcceleration()
{
	D3DXVECTOR3 inertia;
	D3DXQUATERNION quatInertia; //convert to quat to match return type

	D3DXVec3Cross(&inertia, &angularForce, &mat->inertiaTensor);
	//turn vec3 into quaternion
	quatInertia.x = inertia.x;
	quatInertia.y = inertia.y;
	quatInertia.z = inertia.z;

	return quatInertia;
}

D3DXQUATERNION EntityData::getAngularMomentum()
{
	D3DXQUATERNION angularMomentum;
	D3DXVECTOR3 angMo;
	//cross product to find angular momentum
	D3DXVec3Cross(&angMo, &angularVelocity, &mat->inertiaTensor);
	//convert to quaterion (return type)
	angularMomentum.x = angMo.x;
	angularMomentum.y = angMo.y;
	angularMomentum.z = angMo.z;

	return angularMomentum;
}

D3DXQUATERNION EntityData::getAngularForce()
{
	return angularForce;
	
}

void EntityData::setPhysicsMat(PhysicsMat* mat)  
{
	this->mat = mat;
}

void EntityData::setIsKinematic(TypePhysics mode)
{
	this->isKinematic = mode;
}

void EntityData::setPosition(D3DXVECTOR3 position)
{
	this->position = position;
}

void EntityData::setVelocity(D3DXVECTOR3 velocity)
{
	this->velocity = velocity;
}

void EntityData::setForce(D3DXVECTOR3 force)
{
	this->force = force;
}

void EntityData::setRotation(D3DXVECTOR3 rotation)
{
	//passing vector3 parameter rotation to quaternion rotation member variable
	this->rotation.x = rotation.x;
	this->rotation.y = rotation.y;
	this->rotation.z = rotation.z;
}

void EntityData::setAngularVelocity(D3DXVECTOR3 angularVelocity)
{
	this->angularVelocity = angularVelocity;
}
void EntityData::setAngularForce(D3DXVECTOR3 angularForce)
{
	this->angularForce = angularForce;
}

void EntityData::translate(D3DXVECTOR3 translation)
{
	position += translation;
}

void EntityData::accelerate(D3DXVECTOR3 deltaVelocity)
{
	this->velocity += deltaVelocity;
	
}

void EntityData::applyImpulseForce(D3DXVECTOR3 deltaMomentum)
{
	//?
}

void EntityData::applyForce(D3DXVECTOR3 force)
{
	this->force += force;
	
}

void EntityData::rotate(D3DXQUATERNION rotation)
{
	this->rotation += rotation;
}

void EntityData::applySpin(D3DXQUATERNION deltaAngularVelocity)
{
	//?
}

void EntityData::applyImpulseRotation(D3DXQUATERNION deltaAngularMomentum)
{
	//?
}

void EntityData::applyRotationalForce(D3DXQUATERNION angularForce)
{
	//?
}


EntityManager::EntityManager()
{
	nextID = 0;
}

EntityManager::~EntityManager()
{
	entities.clear();
}



EntityData* EntityManager::NewEntity()
{
	EntityData temp;
	temp.ID = nextID++;
	entities.push_back(temp);
	return &entities.back();
}

void EntityManager::AddEntity(EntityData& entity)
{
	entities.push_back(entity);
}

bool EntityManager::RemoveEntity(int id)
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

void EntityManager::Clear()
{
	entities.clear();
}



EntityData* EntityManager::GetEntity(int id)
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



void EntityManager::CloneInto(EntityList& entityList)
{
	entityList = entities;
}

EntityIterator EntityManager::Begin()
{
	return entities.begin();
}

EntityIterator EntityManager::End()
{
	return entities.end();
}