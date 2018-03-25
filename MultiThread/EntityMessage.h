#ifndef _ENTITYMESSAGE_H
#define _ENTITYMESSAGE_H

#include "Message.h"
#include <d3dx9.h>
#include "EntityPropertyList.h"
#include "PhysicsMat.h"

enum EEntityMessage
{
	MSG_ADDENTITY,
	MSG_REMOVEENTITY,
	MSG_SETPHYSICSMATERIAL,
	MSG_SETISKINEMATIC,
	MSG_SETPOSITION,
	MSG_SETVELOCITY,
	MSG_SETFORCE,
	MSG_SETROTATION,
	MSG_SETROTATION_Q,
	MSG_SETANGULARVELOCITY,
	MSG_SETANGULARFORCE,
	MSG_TRANSLATE,
	MSG_ACCELERATE,
	MSG_IMPULSEFORCE,
	MSG_ADDFORCE,
	MSG_ROTATEONAXIS,
	MSG_ROTATE_Q,
	MSG_ROTATEONX,
	MSG_ROTATEONY,
	MSG_ROTATEONZ,
	MSG_ADDANGULARVELOCITY,
	MSG_IMPULSEANGULARFORCE,
	MSG_ADDANGULARFORCE,
	MSG_INCSCRIPTFLAG1,
	MSG_INCSCRIPTFLAG2,
	MSG_SETSTEERING
};

struct SMessageAddEntity : SMessage
{
	SMessageAddEntity(int *id, int type, D3DXVECTOR3 pos) : SMessage(CORE_ENTITY, MSG_ADDENTITY),
		ID(id), Type(type), Position(pos) {}
	int *ID;
	int Type;
	D3DXVECTOR3 Position;
};

struct SMessageRemoveEntity : SMessage
{
	SMessageRemoveEntity(int *id) : SMessage(CORE_ENTITY, MSG_REMOVEENTITY),
		ID(id) {}
	int *ID;
};

struct SMessageSetMaterial : SMessage
{
	SMessageSetMaterial(int *id, PhysicsMat* mat) : SMessage(CORE_ENTITY, MSG_SETPHYSICSMATERIAL),
		ID(id), Mat(mat) {}
	int *ID;
	PhysicsMat* Mat;
};

struct SMessageSetIsKinematic : SMessage
{
	SMessageSetIsKinematic(int *id, TypePhysics type) : SMessage(CORE_ENTITY, MSG_SETISKINEMATIC),
		ID(id), Type(type) {}
	int *ID;
	TypePhysics Type;
};

struct SMessageSetPosition : public SMessage
{
	SMessageSetPosition(int *id, D3DXVECTOR3 position) : SMessage(CORE_ENTITY, MSG_SETPOSITION),
		ID(id), Position(position) {}
	int *ID;
	D3DXVECTOR3 Position;
};

struct SMessageSetVelocity : public SMessage
{
	SMessageSetVelocity(int *id, D3DXVECTOR3 velocity):SMessage(CORE_ENTITY, MSG_SETVELOCITY),
		ID(id), Velocity(velocity) {}
	int *ID;
	D3DXVECTOR3 Velocity;
};

struct SMessageSetForce : public SMessage
{
	SMessageSetForce(int *id, D3DXVECTOR3 force) : SMessage(CORE_ENTITY, MSG_SETFORCE),
		ID(id), Force(force) {}
	int *ID;
	D3DXVECTOR3 Force;
};

struct SMessageSetRotation : public SMessage
{
	SMessageSetRotation(int *id, D3DXVECTOR3 axis, float degree) : SMessage(CORE_ENTITY, MSG_SETROTATION),
		ID(id), Axis(axis), Degree(degree) {}
	int *ID;
	D3DXVECTOR3 Axis;
	float Degree;
};

struct SMessageSetRotation_Quat : public SMessage
{
	SMessageSetRotation_Quat(int *id, D3DXQUATERNION rotation) : SMessage(CORE_ENTITY, MSG_SETROTATION_Q),
		ID(id), Rotation(rotation) {}
	int *ID;
	D3DXQUATERNION Rotation;
};

struct SMessageSetAngularVelocity : public SMessage
{
	SMessageSetAngularVelocity(int *id, D3DXVECTOR3 angularVelocity) :SMessage(CORE_ENTITY, MSG_SETANGULARVELOCITY),
		ID(id), AngularVelocity(angularVelocity) {}
	int *ID;
	D3DXVECTOR3 AngularVelocity;
};

struct SMessageSetAngularForce : public SMessage
{
	SMessageSetAngularForce(int *id, D3DXVECTOR3 angularForce) : SMessage(CORE_ENTITY, MSG_SETANGULARFORCE),
		ID(id), AngularForce(angularForce) {}
	int *ID;
	D3DXVECTOR3 AngularForce;
};

struct SMessageTranslate : public SMessage
{
	SMessageTranslate(int *id, D3DXVECTOR3 deltaPosition) : SMessage(CORE_ENTITY, MSG_TRANSLATE),
		ID(id), DeltaPosition(deltaPosition) {}
	int *ID;
	D3DXVECTOR3 DeltaPosition;
};

struct SMessageAccelerate : public SMessage
{
	SMessageAccelerate(int *id, D3DXVECTOR3 deltaVelocity) : SMessage(CORE_ENTITY, MSG_ACCELERATE),
		ID(id), DeltaVelocity(deltaVelocity) {}
	int *ID;
	D3DXVECTOR3 DeltaVelocity;
};

struct SMessageImpulseForce : public SMessage
{
	SMessageImpulseForce(int *id, D3DXVECTOR3 deltaMomentum) : SMessage(CORE_ENTITY, MSG_IMPULSEFORCE),
		ID(id), DeltaMomentum(deltaMomentum) {}
	int *ID;
	D3DXVECTOR3 DeltaMomentum;
};

struct SMessageAddForce : public SMessage
{
	SMessageAddForce(int *id, D3DXVECTOR3 deltaForce) : SMessage(CORE_ENTITY, MSG_ADDFORCE),
		ID(id), DeltaForce(deltaForce) {}
	int *ID;
	D3DXVECTOR3 DeltaForce;
};

struct SMessageAddRotationOnAxis : public SMessage
{
	SMessageAddRotationOnAxis(int *id, D3DXVECTOR3 axis, float degree) : SMessage(CORE_ENTITY, MSG_ROTATEONAXIS),
		ID(id), Axis(axis), Degree(degree) {}
	int *ID;
	D3DXVECTOR3 Axis;
	float Degree;
};

struct SMessageAddRotationQuat : public SMessage
{
	SMessageAddRotationQuat(int *id, D3DXQUATERNION rotation) : SMessage(CORE_ENTITY, MSG_ROTATE_Q),
		ID(id), Rotation(rotation) {}
	int *ID;
	D3DXQUATERNION Rotation;
};

struct SMessageAddRotationOnX : public SMessage
{
	SMessageAddRotationOnX(int *id, float degree) :SMessage(CORE_ENTITY, MSG_ROTATEONX),
		ID(id), Degree(degree) {}
	int *ID;
	float Degree;
};

struct SMessageAddRotationOnY : public SMessage
{
	SMessageAddRotationOnY(int *id, float degree) :SMessage(CORE_ENTITY, MSG_ROTATEONY),
		ID(id), Degree(degree) {}
	int *ID;
	float Degree;
};

struct SMessageAddRotationOnZ : public SMessage
{
	SMessageAddRotationOnZ(int *id, float degree) :SMessage(CORE_ENTITY, MSG_ROTATEONZ),
		ID(id), Degree(degree) {}
	int *ID;
	float Degree;
};

struct SMessageAddAngularVelocity : public SMessage
{
	SMessageAddAngularVelocity(int *id, D3DXVECTOR3 axis, float deltaDegree) : SMessage(CORE_ENTITY, MSG_ADDANGULARVELOCITY),
		ID(id), Axis(axis), DeltaDegree(deltaDegree) {}
	int *ID;
	D3DXVECTOR3 Axis;
	float DeltaDegree;
};

struct SMessageAddImpulseAngularForce : public SMessage
{
	SMessageAddImpulseAngularForce(int *id, D3DXVECTOR3 axis, float angularMomentum) : SMessage(CORE_ENTITY, MSG_IMPULSEANGULARFORCE),
		ID(id), Axis(axis), AngularMomentum(angularMomentum) {}
	int *ID;
	D3DXVECTOR3 Axis;
	float AngularMomentum;
};

struct SMessageAddAngularForce : public SMessage
{
	SMessageAddAngularForce(int *id, D3DXVECTOR3 axis, float deltaAngularForce) : SMessage(CORE_ENTITY, MSG_ADDANGULARFORCE),
		ID(id), Axis(axis), DeltaAngularForce(deltaAngularForce) {}
	int *ID;
	D3DXVECTOR3 Axis;
	float DeltaAngularForce;
};

struct SMessageIncScriptFlag1 : public SMessage
{
	SMessageIncScriptFlag1(int id) : SMessage(CORE_ENTITY, MSG_INCSCRIPTFLAG1), ID(id)
	{}
	int ID;
};

/*
MSG_INCSCRIPTFLAG2,
*/

struct SMessageSetSteering : public SMessage
{
	SMessageSetSteering(int *id, int steeringtype) : SMessage(CORE_ENTITY, MSG_SETSTEERING)
	{
		ID = id;
		SteeringType = steeringtype;
	}
	int *ID;
	int SteeringType;
};

#endif