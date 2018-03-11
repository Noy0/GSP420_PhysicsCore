#ifndef PHYSICS_H
#define PHYSICS_H

#include "btBulletDynamicsCommon.h"
#include "Util.h"
#include <d3d9.h>
#include <d3dx9.h>
//#include <stdio.h>

class PhysicsWorld
{
private:
	///collision configuration contains default setup for memory, collision setup. Advanced users can create their own configuration.
	btDefaultCollisionConfiguration* collisionConfiguration;

	///use the default collision dispatcher. For parallel processing you can use a diffent dispatcher (see Extras/BulletMultiThreaded)
	btCollisionDispatcher* dispatcher;

	btAxisSweep3* overlappingPairCache;

	///the default constraint solver. For parallel processing you can use a different solver (see Extras/BulletMultiThreaded)
	btSequentialImpulseConstraintSolver* solver;

	btDiscreteDynamicsWorld* dynamicsWorld;

	btAlignedObjectArray<btCollisionShape*> collisionShapes;

	int mNumOfObjects;

public:
	PhysicsWorld();
	~PhysicsWorld();

	void CreateWorld(WorldInfo& WldInfo);
	void wipePhysics();

	void UpdateWorld(btScalar timeStep = (1 / 60), int maxSubSteps = 1);

	void displayCallback();

	//int CreateCollision_Plane(PMatPlane& mat);
	int CreateCollisionBox(BoxInfo& info);//int CreateCollisionBox(PMatBox& mat);
	int CreateCollisionSphere(SphereInfo& info);//int CreateCollisionSphere(PMatSphere& mat);
	//int CreateCollision_Capsule(BoxInfo& info);
	int PhysicsWorld::CreateCollisionCylinder(BoxInfo& info);//int CreateCollisionCylinder(PMatCylinder& mat);

	void PhysicsWorld::DeletePhysicsObject(int id);

	bool IsKinematic(int id);
	void SetAsKinematic(int id);

	btVector3 GetShapePosition(int id);//D3DXVECTOR3 GetPosition(int id);
	void SetShapePosition(int id, btVector3& position);//void SetPosition(int id, D3DXVECTOR3& position);

	btVector3 GetLinearVelocity(int id);//D3DXVECTOR3 GetLinearVelocity(int id);
	void SetLinearVelocity(int id, btVector3& vel);//void SetLinearVelocity(int id, D3DXVECTOR3& vel);

	void ApplyCentralForce(int id, btVector3& force);//void ApplyCentralForce(int id, D3DXVECTOR3& force);
	
	btVector3 GetShapeRotation(int id);//D3DXQUATERNION GetRotation(int id);
	void SetShapeRotation(int id,btQuaternion& quat );//void SetRotation(int id, D3DXQUATERNION& quat);
	void SetShapeRotation(int id, btVector3& axis,float degree);//void SetRotation_Simple(int id, D3DXVECTOR3& axis, float degree);
	void RotateShapeOnX(int id, float degree);//void RotateOnCoordAxis(int id, float angle, AxisID axis);
	void RotateShapeOnY(int id, float degree);
	void RotateShapeOnZ(int id, float degree);

	btVector3 ConvertToBtVec(D3DXVECTOR3& old);
	D3DXVECTOR3 convertToDxVec(btVector3& old);

	//btQuaternion ConvertToBtQuat(D3DXQUATERNION& old);
	D3DXQUATERNION ConvertToDxRot(btVector3& oldRot);

	void setPhysics();
};

extern PhysicsWorld* gPhysics;

#endif
