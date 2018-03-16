#ifndef _PHYSICSMAT_H
#define _PHYSICSMAT_H

//structs based on UML diagram, to be fed into EntityData class - DN, TP

#include "EPhysics.h"
#include <d3dx9.h>

struct PhysicsMat
{
	PhysicsMat() : inertiaTensor(0, 0, 0), friction(1), restitution(0.0), linearDamping(0),
		angularDamping(0), mass(1), type(COLLIDER_NULL) {}
	TypeCollider type;
	float mass;
	float friction;
	float restitution;
	float linearDamping;
	float angularDamping;
	D3DXVECTOR3 inertiaTensor;
};

struct PlanePMat :PhysicsMat
{
	PlanePMat() : PhysicsMat(), pNormal(0, 1.0, 0, 0) { type = COLLIDER_PLANE; }
	D3DXVECTOR4 pNormal;
};

struct BoxPMat :PhysicsMat
{
	BoxPMat() : PhysicsMat(), scalar(1.0, 1.0, 1.0) { type = COLLIDER_BOX; }
	D3DXVECTOR3 scalar;
};

struct SpherePMat :PhysicsMat
{
	SpherePMat() : PhysicsMat(), scalar(1.0, 1.0, 1.0), radius(1) { type = COLLIDER_SPHERE; }
	D3DXVECTOR3 scalar;//used to deform model - currently not implemented
	float radius;
};

struct CapsulePMat :PhysicsMat
{
	CapsulePMat() : PhysicsMat(), scalar(1.0, 1.0, 1.0), radius(1), length(1) { type = COLLIDER_CAPSULE; }
	D3DXVECTOR3 scalar;//used to deform model
	float radius;
	float length;
};

struct CylinderPMat :PhysicsMat
{
	CylinderPMat() : PhysicsMat(), scalar(1.0, 1.0, 1.0), radius(1), length(1) { type = COLLIDER_CYLINDER; }
	D3DXVECTOR3 scalar;//used to deform model
	float radius;
	float length;
};

#endif