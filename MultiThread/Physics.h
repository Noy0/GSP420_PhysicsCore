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

	void createWorld(WorldInfo& WldInfo);
	void clearPhysics();

	void updateWorld(btScalar timeStep = (1 / 60), int maxSubSteps = 1);

	void displayCallback();

	//int createCollision_Plane(PMatPlane& mat);
	int createCollision_Box(BoxInfo& info);//int createCollision_Box(PMatBox& mat);
	int createCollision_Sphere(SphereInfo& info);//int createCollision_Sphere(PMatSphere& mat);
	//int createCollision_Capsule(BoxInfo& info);
	int createCollision_Cylinder(BoxInfo& info);//int createCollision_Cylinder(PMatCylinder& mat);

	void deletePhysicsObject(int id);

	bool isKinematic(int id);
	void setAsKinematic(int id);

	D3DXVECTOR3 getPosition(int id);
	void setPosition(int id, D3DXVECTOR3& position);

	D3DXVECTOR3 getLinearVelocity(int id);
	void setLinearVelocity(int id, D3DXVECTOR3& vel);

	void applyCentralForce(int id, btVector3& force);//void applyCentralForce(int id, D3DXVECTOR3& force);
	
	btVector3 getRotation(int id);//D3DXQUATERNION getRotation(int id);
	void setRotation(int id, btQuaternion& quat );//void setRotation(int id, D3DXQUATERNION& quat);
	void setRotation(int id, btVector3& axis,float degree);//void setRotation_Simple(int id, D3DXVECTOR3& axis, float degree);
	void rotateOnX(int id, float degree);//void rotateOnCoordAxis(int id, float angle, AxisID axis);
	void rotateOnY(int id, float degree);
	void rotateOnZ(int id, float degree);

	btVector3 convertToBtVec(const D3DXVECTOR3& old);
	D3DXVECTOR3 convertToDxVec(const btVector3& old);

	btQuaternion convertToBtQuat(const D3DXQUATERNION& old);
	D3DXQUATERNION convertToDxRot(const btVector3& old);//D3DXQUATERNION convertToDxRot(const btQuaternion& old);

	void setPhysics();
};

extern PhysicsWorld* gPhysics;

#endif
