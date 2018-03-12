#include "Physics.h"

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




int PhysicsWorld::createCollision_Box(BoxInfo& info)
{
	// Creates a Box Object.

	btCollisionShape* colShape = new btBoxShape(info.halfscale);
	collisionShapes.push_back(colShape);

	btTransform startTransform;
	startTransform.setIdentity();

	btScalar	mass = info.mass;

	//rigidbody is dynamic if and only if mass is non zero, otherwise static
	bool isDynamic = (mass != 0.f);

	btVector3 localInertia = info.localInertia;
	if (isDynamic)
		colShape->calculateLocalInertia(mass, localInertia);

	startTransform.setOrigin(info.orgin);

	//using motionstate is recommended, it provides interpolation capabilities, and only synchronizes 'active' objects
	btDefaultMotionState* myMotionState = new btDefaultMotionState(startTransform);
	btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, myMotionState, colShape, localInertia);
	btRigidBody* body = new btRigidBody(rbInfo);

	rbInfo.m_friction = info.friction;
	rbInfo.m_restitution = info.restitution;
	rbInfo.m_linearDamping = info.linearDamping;
	rbInfo.m_angularDamping = info.angularDamping;

	dynamicsWorld->addRigidBody(body);

	mNumOfObjects++;
	return mNumOfObjects;

}

int PhysicsWorld::createCollision_Sphere(SphereInfo& info)
{
	//create a dynamic rigidbody Sphere

	btCollisionShape* colShape = new btSphereShape(btScalar(info.radius));
	collisionShapes.push_back(colShape);

	/// Create Dynamic Objects
	btTransform startTransform;
	startTransform.setIdentity();

	btScalar	mass = info.mass;

	//rigidbody is dynamic if and only if mass is non zero, otherwise static
	bool isDynamic = (mass != 0.f);

	btVector3 localInertia = info.localInertia;
	if (isDynamic)
		colShape->calculateLocalInertia(mass, localInertia);

	startTransform.setOrigin(info.orgin);

	//using motionstate is recommended, it provides interpolation capabilities, and only synchronizes 'active' objects
	btDefaultMotionState* myMotionState = new btDefaultMotionState(startTransform);
	btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, myMotionState, colShape, localInertia);

	rbInfo.m_friction = info.friction;
	rbInfo.m_restitution = info.restitution;
	rbInfo.m_linearDamping = info.linearDamping;
	rbInfo.m_angularDamping = info.angularDamping;
	btRigidBody* body = new btRigidBody(rbInfo);

	dynamicsWorld->addRigidBody(body);

	mNumOfObjects++;
	return mNumOfObjects;
}

int PhysicsWorld::createCollision_Cylinder(BoxInfo& info)
{
	// Creates a Box Object.

	btCollisionShape* colShape = new btCylinderShape(info.halfscale);
	collisionShapes.push_back(colShape);

	btTransform startTransform;
	startTransform.setIdentity();

	btScalar	mass = info.mass;

	//rigidbody is dynamic if and only if mass is non zero, otherwise static
	bool isDynamic = (mass != 0.f);

	btVector3 localInertia = info.localInertia;
	if (isDynamic)
		colShape->calculateLocalInertia(mass, localInertia);

	startTransform.setOrigin(info.orgin);

	//using motionstate is recommended, it provides interpolation capabilities, and only synchronizes 'active' objects
	btDefaultMotionState* myMotionState = new btDefaultMotionState(startTransform);
	btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, myMotionState, colShape, localInertia);
	btRigidBody* body = new btRigidBody(rbInfo);

	rbInfo.m_friction = info.friction;
	rbInfo.m_restitution = info.restitution;
	rbInfo.m_linearDamping = info.linearDamping;
	rbInfo.m_angularDamping = info.angularDamping;

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




btVector3 PhysicsWorld::getLinearVelocity(int id)
{
	btVector3 vec(0, 0, 0);
	btCollisionObject* obj = dynamicsWorld->getCollisionObjectArray()[id];
	btRigidBody* body = btRigidBody::upcast(obj);
	if (body && body->getMotionState())
	{
		vec = body->getLinearVelocity();
	}
	return vec;
}

void PhysicsWorld::setLinearVelocity(int id, btVector3& vel)
{
	btCollisionObject* obj = dynamicsWorld->getCollisionObjectArray()[id];
	btRigidBody* body = btRigidBody::upcast(obj);
	if (body && body->getMotionState())
	{
		body->setLinearVelocity(vel);
	}
}




void PhysicsWorld::applyCentralForce(int id, btVector3& force)
{
	btCollisionObject* obj = dynamicsWorld->getCollisionObjectArray()[id];
	btRigidBody* body = btRigidBody::upcast(obj);
	if (body && body->getMotionState())
	{
		body->applyCentralForce(force);
	}
}




btVector3 PhysicsWorld::getRotation(int id)
{
	btVector3 rot;

	btCollisionObject* obj = dynamicsWorld->getCollisionObjectArray()[id];
	btRigidBody* body = btRigidBody::upcast(obj);
	if (body && body->getMotionState())
	{
		btTransform trans;
		body->getMotionState()->getWorldTransform(trans);

		rot = btVector3(float(trans.getRotation().getX()),
			float(trans.getRotation().getY()), float(trans.getRotation().getZ()));
		rot.setW(float(trans.getRotation().getW()));
	}

	return rot;
}

void PhysicsWorld::setRotation(int id, btVector3& axis, float degree)
{
	degree = (3.14159265 / 180)*degree;
	btQuaternion rot(btVector3(axis.getX(), axis.getY(), axis.getZ()), degree);
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

void PhysicsWorld::setRotation(int id, btQuaternion& quat)
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

void PhysicsWorld::rotateOnX(int id, float degree)
{
	btVector3 vec;
	vec = getRotation(id);
	btQuaternion q1(vec.getX(), vec.getY(), vec.getZ(), vec.w());

	vec.setValue(-20, 0, 0);
	btQuaternion q2(vec, degree*(3.14159265 / 180));
	btQuaternion q3;
	q3 = q2*q1;

	setRotation(id, q3);
}

void PhysicsWorld::rotateOnY(int id, float degree)
{
	btVector3 vec;
	vec = getRotation(id);
	btQuaternion q1(vec.getX(), vec.getY(), vec.getZ(), vec.w());

	vec.setValue(0, -20, 0);
	btQuaternion q2(vec, degree*(3.14159265 / 180));
	btQuaternion q3;
	q3 = q2*q1;

	setRotation(id, q3);
}

void PhysicsWorld::rotateOnZ(int id, float degree)
{
	btVector3 vec;
	vec = getRotation(id);
	btQuaternion q1(vec.getX(), vec.getY(), vec.getZ(), vec.w());

	vec.setValue(0, 0, -20);
	btQuaternion q2(vec, degree*(3.14159265 / 180));
	btQuaternion q3;
	q3 = q2*q1;

	setRotation(id, q3);
}




btVector3 PhysicsWorld::convertToBtVec(D3DXVECTOR3& old)
{
	btVector3 newVec(old.x, old.y, old.z);

	return newVec;
}


D3DXVECTOR3 PhysicsWorld::convertToDxVec(btVector3& old)
{
	D3DXVECTOR3 newVec(old.x(), old.y(), old.z());

	return newVec;
}


btQuaternion PhysicsWorld::convertToBtQuat(D3DXQUATERNION& old)
{
	btQuaternion tempQuat(old.x, old.y, old.z, old.w);

	return tempQuat;
}

D3DXQUATERNION PhysicsWorld::convertToDxRot(btVector3 &old)
{
	D3DXQUATERNION tempQuat(old.x(), old.y(), old.z(), old.w());

	return tempQuat;
}




void PhysicsWorld::setPhysics()
{
}
