#ifndef _STRUCTPHYSICS_H
#define _STRUCTPHYSICS_H

//structs based on UML diagram, to be fed into EntityData class - DN

#include "btBulletDynamicsCommon.h" //changed btVector3 to directX XMFLOAT3, leaving the include for now in case we revert
#include "directXmath.h"
struct PhysicsMat
{
	int type;
	float mass;
	float friction;
	float restitution;
	float linearDamping;
	float angularDamping;
	DirectX::XMFLOAT3 inertialTensor; 
	// to load floats into direct X vector:    DirectX::XMVECTOR inertialTensor = DirectX::XMLoadFloat3(&inertialTensor);
	DirectX::XMFLOAT3 origin;
};

struct PlanePMat :PhysicsMat
{
	DirectX::XMFLOAT4 pNormal;
};

struct BoxPMat :PhysicsMat
{
	DirectX::XMFLOAT3 scalar;
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