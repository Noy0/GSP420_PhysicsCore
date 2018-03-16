#include "Physics.h"

//Heavily Edited done by - TP

PhysicsWorld* gPhysics = 0;


PhysicsWorld::PhysicsWorld(){
	mNumOfObjects=-1;
}

PhysicsWorld::~PhysicsWorld()
{
	//cleanup in the reverse order of creation/initialization

	//remove the rigidbodies from the dynamics world and delete them
	//for (int i=dynamicsWorld->getNumCollisionObjects()-1; i>=0 ;i--)
	for (int i = dynamicsWorld->getNumCollisionObjects() - 1; i >= 0; i--)
	{
		btCollisionObject* obj = dynamicsWorld->getCollisionObjectArray()[i];
		btRigidBody* body = btRigidBody::upcast(obj);
		if (body && body->getMotionState())
		{
			delete body->getMotionState();
		}
		dynamicsWorld->removeCollisionObject(obj);
		delete obj;
	}

	//delete collision shapes
	for (int j = 0; j<collisionShapes.size(); j++)
	{
		btCollisionShape* shape = collisionShapes[j];
		collisionShapes[j] = 0;
		delete shape;
	}

	//delete dynamics world
	delete dynamicsWorld;

	//delete solver
	delete solver;

	//delete broadphase
	delete overlappingPairCache;

	//delete dispatcher
	delete dispatcher;

	delete collisionConfiguration;

	//next line is optional: it will be cleared by the destructor when the array goes out of scope
	collisionShapes.clear();

}




void PhysicsWorld::createWorld(WorldInfo& WldInfo)
{
	///collision configuration contains default setup for memory, collision setup. Advanced users can create their own configuration.
	collisionConfiguration = new btDefaultCollisionConfiguration();

	///use the default collision dispatcher. For parallel processing you can use a diffent dispatcher (see Extras/BulletMultiThreaded)
	dispatcher = new	btCollisionDispatcher(collisionConfiguration);

	///the maximum size of the collision world. Make sure objects stay within these boundaries
	///Don't make the world AABB size too large, it will harm simulation quality and performance
	btVector3 worldAabbMin=WldInfo.WorldAabbMin;
	btVector3 worldAabbMax=WldInfo.WorldAabbMax;
	int	maxProxies = 1024;
	
	overlappingPairCache = new btAxisSweep3(worldAabbMin,worldAabbMax,maxProxies);

	///the default constraint solver. For parallel processing you can use a different solver (see Extras/BulletMultiThreaded)
	solver = new btSequentialImpulseConstraintSolver;

	dynamicsWorld = new btDiscreteDynamicsWorld(dispatcher,overlappingPairCache,solver,collisionConfiguration);

	dynamicsWorld->setGravity(WldInfo.Gravity);

	//keep track of the shapes, we release memory at exit.
	//make sure to re-use collision shapes among rigid bodies whenever possible!
	btAlignedObjectArray<btCollisionShape*> collisionShapes;
}

void PhysicsWorld::clearPhysics()
{
	for (int i = 7; i <= dynamicsWorld->getNumCollisionObjects() - 1; i++)
	{
		btCollisionObject* obj = dynamicsWorld->getCollisionObjectArray()[i];
		btRigidBody* body = btRigidBody::upcast(obj);
		if (body && body->getMotionState())
		{
			delete body->getMotionState();
		}
		dynamicsWorld->removeCollisionObject(obj);
		delete obj;
	}


}




void PhysicsWorld::updateWorld(btScalar timeStep, int maxSubSteps)
{
	dynamicsWorld->stepSimulation(timeStep, maxSubSteps);
}




void PhysicsWorld::displayCallback()
{
	//if (dynamicsWorld)
	//	dynamicsWorld->performDiscreteCollisionDetection();

	int i;

	///one way to draw all the contact points is iterating over contact manifolds / points:

	int numManifolds = dynamicsWorld->getDispatcher()->getNumManifolds();
	for (i = 0; i<numManifolds; i++)
	{
		btPersistentManifold* contactManifold = dynamicsWorld->getDispatcher()->getManifoldByIndexInternal(i);
		btCollisionObject* obA = static_cast<btCollisionObject*>(contactManifold->getBody0());
		btCollisionObject* obB = static_cast<btCollisionObject*>(contactManifold->getBody1());

		int numContacts = contactManifold->getNumContacts();
		for (int j = 0; j<numContacts; j++)
		{
			btManifoldPoint& pt = contactManifold->getContactPoint(j);

		}
	}
}



int PhysicsWorld::createCollision_Object(PhysicsMat& pMat, D3DXVECTOR3 position)
{
	btCollisionShape* colShape;
	switch (pMat.type)
	{
		case COLLIDER_PLANE:
		{
			break;
		}
		case COLLIDER_BOX:
		{
			BoxPMat& boxPMat = (BoxPMat&)pMat;
			btVector3 size(convertToBtVec(boxPMat.scalar));

			colShape = new btBoxShape(size);
			break;
		}
		case COLLIDER_SPHERE:
		{
			SpherePMat& sphPMat = (SpherePMat&)pMat;

			colShape = new btSphereShape(btScalar(sphPMat.radius));
			break;
		}
		case COLLIDER_CYLINDER:
		{
			CylinderPMat& cylPMat = (CylinderPMat&)pMat;
			btVector3 size(cylPMat.radius * cylPMat.scalar.x, cylPMat.length * cylPMat.scalar.y, cylPMat.radius * cylPMat.scalar.z);

			colShape = new btCylinderShape(size);
			break;
		}
		case COLLIDER_CAPSULE:
		{

			break;
		}
	default:
		return mNumOfObjects;
	}
	collisionShapes.push_back(colShape);

	btTransform startTransform;
	startTransform.setIdentity();

	btScalar mass = pMat.mass;

	//rigidbody is dynamic if and only if mass is non zero, otherwise static
	bool isDynamic = (mass != 0.f);

	btVector3 inertiaTensor = convertToBtVec(pMat.inertiaTensor);
	if (isDynamic)
		colShape->calculateLocalInertia(mass, inertiaTensor);

	startTransform.setOrigin(convertToBtVec(position));

	//using motionstate is recommended, it provides interpolation capabilities, and only synchronizes 'active' objects
	btDefaultMotionState* myMotionState = new btDefaultMotionState(startTransform);
	btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, myMotionState, colShape, inertiaTensor);
	
	rbInfo.m_friction = pMat.friction;
	rbInfo.m_restitution = pMat.restitution;
	rbInfo.m_linearDamping = pMat.linearDamping;
	rbInfo.m_angularDamping = pMat.angularDamping;
	btRigidBody* body = new btRigidBody(rbInfo);

	dynamicsWorld->addRigidBody(body);

	mNumOfObjects++;
	return mNumOfObjects;
}




void PhysicsWorld::deletePhysicsObject(int id)
{
	/// works like a deque...remove one and they push forward...i think
	btCollisionObject* obj = dynamicsWorld->getCollisionObjectArray()[id];
	dynamicsWorld->removeCollisionObject(obj);
}




bool PhysicsWorld::isKinematic(int id)
{
	btCollisionObject* obj = dynamicsWorld->getCollisionObjectArray()[id];
	btRigidBody* body = btRigidBody::upcast(obj);

	return body->isKinematicObject();
}

void PhysicsWorld::setAsKinematic(int id)
{
	btCollisionObject* obj = dynamicsWorld->getCollisionObjectArray()[id];
	btRigidBody* body = btRigidBody::upcast(obj);
	if (body && body->getMotionState())
	{
		body->setCollisionFlags(body->getCollisionFlags() | btCollisionObject::CF_KINEMATIC_OBJECT);
		body->setActivationState(DISABLE_DEACTIVATION);
	}
}




D3DXVECTOR3 PhysicsWorld::getPosition(int id)
{
	D3DXVECTOR3 pos;

	btCollisionObject* obj = dynamicsWorld->getCollisionObjectArray()[id];
	btRigidBody* body = btRigidBody::upcast(obj);
	if (body && body->getMotionState())
	{
		btTransform trans;
		body->getMotionState()->getWorldTransform(trans);

		pos = convertToDxVec(trans.getOrigin());
	}
	return pos;

}

void PhysicsWorld::setPosition(int id, D3DXVECTOR3& position)
{
	btVector3 btPosition = convertToBtVec(position);
	btCollisionObject* obj = dynamicsWorld->getCollisionObjectArray()[id];
	btRigidBody* body = btRigidBody::upcast(obj);
	if (body && body->getMotionState())
	{
		btTransform trans;
		body->getMotionState()->getWorldTransform(trans);
		trans.setOrigin(btPosition);
		body->getMotionState()->setWorldTransform(trans);
	}
	else// I dont think we will be useing this..but might as well have it just in case.
	{
		dynamicsWorld->getCollisionObjectArray()[0]->getWorldTransform().setOrigin(btPosition);
	}
}




D3DXVECTOR3 PhysicsWorld::getLinearVelocity(int id)
{
	D3DXVECTOR3 vec;
	btCollisionObject* obj = dynamicsWorld->getCollisionObjectArray()[id];
	btRigidBody* body = btRigidBody::upcast(obj);
	if (body && body->getMotionState())
	{
		vec = convertToDxVec(body->getLinearVelocity());
	}
	return vec;
}

void PhysicsWorld::setLinearVelocity(int id, D3DXVECTOR3& vel)
{
	btVector3 btVel = convertToBtVec(vel);
	btCollisionObject* obj = dynamicsWorld->getCollisionObjectArray()[id];
	btRigidBody* body = btRigidBody::upcast(obj);
	if (body && body->getMotionState())
	{
		body->setLinearVelocity(btVel);
	}
}




void PhysicsWorld::applyCentralForce(int id, D3DXVECTOR3& force)
{
	btVector3 btForce = convertToBtVec(force);
	btCollisionObject* obj = dynamicsWorld->getCollisionObjectArray()[id];
	btRigidBody* body = btRigidBody::upcast(obj);
	if (body && body->getMotionState())
	{
		body->applyCentralForce(btForce);
	}
}




D3DXQUATERNION PhysicsWorld::getRotation(int id)
{
	return convertToDxRot(getBtRotation(id));
	/*
	D3DXQUATERNION rot;

	btCollisionObject* obj = dynamicsWorld->getCollisionObjectArray()[id];
	btRigidBody* body = btRigidBody::upcast(obj);
	if (body && body->getMotionState())
	{
		btTransform trans;
		body->getMotionState()->getWorldTransform(trans);

		rot = convertToDxRot(trans.getRotation());
	}

	return rot;*/
}

btQuaternion PhysicsWorld::getBtRotation(int id)
{
	btQuaternion rot;

	btCollisionObject* obj = dynamicsWorld->getCollisionObjectArray()[id];
	btRigidBody* body = btRigidBody::upcast(obj);
	if (body && body->getMotionState())
	{
		btTransform trans;
		body->getMotionState()->getWorldTransform(trans);

		rot = trans.getRotation();
	}

	return rot;
}

void PhysicsWorld::setRotation(int id, D3DXQUATERNION& quat)
{
	setBtRotation(id, convertToBtQuat(quat));
	/*
	btQuaternion btQuat = convertToBtQuat(quat);
	btCollisionObject* obj = dynamicsWorld->getCollisionObjectArray()[id];
	btRigidBody* body = btRigidBody::upcast(obj);
	if (body && body->getMotionState())
	{
		btTransform trans;
		body->getMotionState()->getWorldTransform(trans);
		trans.setRotation(btQuat);
		body->getMotionState()->setWorldTransform(trans);
	}
	else// I dont think we will be useing this..but might as well have it just in case.
	{
		dynamicsWorld->getCollisionObjectArray()[0]->getWorldTransform().setRotation(btQuat);
	}
	*/
}

void PhysicsWorld::setBtRotation(int id, btQuaternion& quat)
{
	btCollisionObject* obj = dynamicsWorld->getCollisionObjectArray()[id];
	btRigidBody* body = btRigidBody::upcast(obj);
	if (body && body->getMotionState())
	{
		btTransform trans;
		body->getMotionState()->getWorldTransform(trans);
		trans.setRotation(quat);
		body->getMotionState()->setWorldTransform(trans);
	}
	else// I dont think we will be useing this..but might as well have it just in case.
	{
		dynamicsWorld->getCollisionObjectArray()[0]->getWorldTransform().setRotation(quat);
	}
}

void PhysicsWorld::setRotation(int id, D3DXVECTOR3& axis, float degree)
{
	degree = (3.14159265 / 180)*degree;
	btQuaternion rot(btVector3(axis.x, axis.y, axis.z), degree);
	btCollisionObject* obj = dynamicsWorld->getCollisionObjectArray()[id];
	btRigidBody* body = btRigidBody::upcast(obj);
	if (body && body->getMotionState())
	{
		btTransform trans;
		body->getMotionState()->getWorldTransform(trans);
		trans.setRotation(rot);
		body->getMotionState()->setWorldTransform(trans);
	}
	else// I dont think we will be useing this..but might as well have it just in case.
	{
		dynamicsWorld->getCollisionObjectArray()[0]->getWorldTransform().setRotation(rot);
	}
}

void PhysicsWorld::rotateOnCoordAxis(int id, float degree, AxisID axis)
{
	btQuaternion q1 = getBtRotation(id);
	btVector3 vec;
	switch (axis)
	{
	case X:
		vec = btVector3(-20, 0, 0);
		break;
	case Y:
		vec = btVector3(0, -20, 0);
		break;
	case Z:
		vec = btVector3(0, 0, -20);
		break;
	}
	btQuaternion q2(vec, degree*(3.14159265 / 180));
	btQuaternion q3;
	q3 = q2*q1;

	setBtRotation(id, q3);
}




btVector3 PhysicsWorld::convertToBtVec(const D3DXVECTOR3& old)
{
	btVector3 newVec(old.x, old.y, old.z);

	return newVec;
}

D3DXVECTOR3 PhysicsWorld::convertToDxVec(const btVector3& old)
{
	D3DXVECTOR3 newVec(old.x(), old.y(), old.z());

	return newVec;
}




btQuaternion PhysicsWorld::convertToBtQuat(const D3DXQUATERNION& old)
{
	btQuaternion tempQuat(old.x, old.y, old.z, old.w);

	return tempQuat;
}

D3DXQUATERNION PhysicsWorld::convertToDxRot(const btQuaternion &old)
{
	D3DXQUATERNION tempQuat(old.x(), old.y(), old.z(), old.w());

	return tempQuat;
}




void PhysicsWorld::setPhysics()
{
}
