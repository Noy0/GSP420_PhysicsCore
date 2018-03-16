#ifndef _PHYSICSMAT_H
#define _PHYSICSMAT_H

//structs based on UML diagram, to be fed into EntityData class - DN, TP

#include "EPhysics.h"
#include <d3dx9.h>

struct PhysicsMat
{
	TypeCollider type;
	float mass;
	float friction;
	float restitution;
	float linearDamping;
	float angularDamping;
	D3DXVECTOR3 inertialTensor;
};

struct PlanePMat :PhysicsMat
{
	D3DXVECTOR4 pNormal;
};

struct BoxPMat :PhysicsMat
{
	D3DXVECTOR3 scalar;
};

struct SpherePMat :PhysicsMat
{
	float radius;
};

struct CapsulePMat :PhysicsMat
{
	float radius;
	float length;
};

struct CylinderPMat :PhysicsMat
{
	float radius;
	float length;
};

#endif