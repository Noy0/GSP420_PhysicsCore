#ifndef _STRUCTPHYSICS_H
#define _STRUCTPHYSICS_H

//structs based on UML diagram, to be fed into EntityData class - DN

#include "btBulletDynamicsCommon.h"
struct PhysicsMat
{
	int type;
	float mass;
	float friction;
	float restitution;
	float linearDamping;
	float angularDamping;
	btVector3 inertialTensor; //assuming using bt library, so sticking with bt types -DN
	btVector3 origin;
};

struct PlanePMat :PhysicsMat
{
	btVector4 pNormal;
};

struct BoxPMat :PhysicsMat
{
	btVector3 scalar;
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