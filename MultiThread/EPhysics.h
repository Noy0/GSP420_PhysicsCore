#ifndef _EPHYSICS_H
#define _EPHYSICS_H

enum EBodyType
{
	BODYTYPE_NULL,
	BODYTYPE_SPHERE,
	BODYTYPE_CYLINDER,
	BODYTYPE_BOX,
	//ADDED FOR GSP420 -DN
	BODYTYPE_CAPSULE,
	BODYTYPE_PLANE
	//END ADDITION
};

enum EPhysicsType
{
	PHYSICSTYPE_NULL, 
	//ADDED FOR GSP420-DN
	PHYSICSTYPE_STATIC,  
	//END ADDITION
	PHYSICSTYPE_DYNAMIC,
	PHYSICSTYPE_KINEMATIC
};

#endif