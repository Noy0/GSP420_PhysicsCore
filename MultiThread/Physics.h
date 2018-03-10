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
	//Kinematic funtions
	void displayCallback();	
	bool IsKinematic(int id);
	void SetShapePosition(int id, btVector3& position);
	void SetShapeRotation(int id,btQuaternion& quat );
	void SetShapeRotation(int id, btVector3& axis,float degree);
	void RotateShapeOnX(int id, float degree);
	void RotateShapeOnY(int id, float degree);
	void RotateShapeOnZ(int id, float degree);
	void wipePhysics();
	void setPhysics();

	//Create sphere
	int CreateCollisionSphere(SphereInfo& info);
	//Create box
	int CreateCollisionBox(BoxInfo& info);
	//create cylinder using box info
	int PhysicsWorld::CreateCollisionCylinder(BoxInfo& info);

	//dynamic functions

	void SetAsKinematic(int id);
	void ApplyCentralForce(int id,btVector3& force);

	btVector3 GetShapePosition(int id);
	btVector3 GetShapeRotation(int id);

	void CreateWorld(WorldInfo& WldInfo);
	void UpdateWorld(btScalar timeStep=(1/60), int maxSubSteps=1);
	PhysicsWorld();
	~PhysicsWorld();

	btVector3 ConvertToBtVec(D3DXVECTOR3& old);
	D3DXVECTOR3 convertToDxVec(btVector3& old);

	D3DXQUATERNION ConvertToDxRot(btVector3& oldRot);


	btVector3 GetLinearVelocity(int id);
	void SetLinearVelocity(int id, btVector3& vel);

	void PhysicsWorld::DeletePhysicsObject(int id);
	


};

extern PhysicsWorld* gPhysics;

#endif
