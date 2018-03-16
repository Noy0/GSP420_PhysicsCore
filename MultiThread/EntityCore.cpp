#include "EntityCore.h"
#include "EntityPropertyManager.h"
#include "EntityPropertyList.h"
#include "EntityMessage.h"
#include "GraphicsMessage.h"
#include "ScriptingCore\ScriptMessages.h"
#include "CoreManager.h"
//#include "Util.h"
#include "PhysicsMat.h"
#include "EPhysics.h"
#include "UIMessage.h"
#include "GameMessage.h"
#include "AI_Message.h"


//Editing done by - TP

EntityCore::EntityCore()
{
	WorldInfo winfo;
	m_Physics.createWorld(winfo);
}

void EntityCore::Update(float dt)
{
	CopyMessage();
	EntityMgrMsg();
	PhysicsMsg();
	m_Physics.updateWorld(dt, 1);
	PollPhysics();
	Export();
}

void EntityCore::OnLaunch()
{
	//Register graphics export
	gCoreMgr->HandleMessage(new SMessageRegisterSceneList(0, SCENE_STATICMESH, &m_StaticMeshExport));
	//Register AI export
	gCoreMgr->HandleMessage(new SMessageRegisterScriptObject(&m_ScriptExport));
	//Register scripting export
	//Register UI export
	gCoreMgr->HandleMessage(new SMessageRegisterPicking(&m_PickingExport));

	//Register AI Steering
	gCoreMgr->HandleMessage(new SMessageRegisterSteering(&m_SteeringExport,&m_ObstacleExport));

	//Register Game export
	gCoreMgr->HandleMessage(new SMessageGameRegisterPosition(&m_PositionExport));
	gCoreMgr->HandleMessage(new SMessageRegisterAIPosition(&m_PositionExport));
}

void EntityCore::EntityMgrMsg()
{
	queue<SMessage*> tempQueue;
	while(!m_QueueBuffer.empty())
	{
		SMessage* msg = m_QueueBuffer.front();
		switch(msg->Message)
		{
		case MSG_ADDENTITY:
			{
				SMessageAddEntity* msgx = (SMessageAddEntity*)msg;
				EntityPropertyData* newEPD = gEPMgr->GetItem(msgx->Type);
				//Add entity to manager
				EntityData* entity = m_EntityMgr.newEntity();
				*(msgx->ID) = entity->ID;
				entity->Type = msgx->Type;
				entity->Position = msgx->Position;

				//Add object to physics
				switch(newEPD->BodyType)
				{
				case BODYTYPE_SPHERE:
					{
						SpherePMat pMat;
						pMat.friction = newEPD->Friction;
						pMat.restitution = newEPD->Restitution;
						pMat.mass = newEPD->Mass;
						pMat.angularDamping = newEPD->AngularDampen;
						pMat.radius = newEPD->Radius;
						int pID = m_Physics.createCollision_Sphere(pMat, msgx->Position);
						entity->PhysicsID = pID;
						if(newEPD->PhysicsType == PHYSICSTYPE_KINEMATIC)
							m_Physics.setAsKinematic(pID);

						break;
					}
				case BODYTYPE_CYLINDER:
					{
						CylinderPMat pMat;
						pMat.friction = newEPD->Friction;
						pMat.restitution = newEPD->Restitution;
						pMat.mass = newEPD->Mass;
						pMat.scalar = newEPD->HalfScale;
						//radius
						//length
						int pID = m_Physics.createCollision_Cylinder(pMat, msgx->Position);
						entity->PhysicsID = pID;
						if(newEPD->PhysicsType == PHYSICSTYPE_KINEMATIC)
							m_Physics.setAsKinematic(pID);

						break;
					}
				case BODYTYPE_BOX:
					{
						BoxPMat pMat;
						pMat.friction = newEPD->Friction;
						pMat.restitution = newEPD->Restitution;
						pMat.mass = newEPD->Mass;
						pMat.scalar = newEPD->HalfScale;
						int pID = m_Physics.createCollision_Box(pMat, msgx->Position);
						entity->PhysicsID = pID;
						if(newEPD->PhysicsType == PHYSICSTYPE_KINEMATIC)
							m_Physics.setAsKinematic(pID);

						break;
					}
				}

				delete msg;
				break;
			}
		case MSG_REMOVEENTITY:
			{
				SMessageRemoveEntity* msgx = (SMessageRemoveEntity*)msg;
				int eID = *(msgx->ID);
				EntityData* entity = m_EntityMgr.getEntity(eID);

				//Remove object from physics
				if(entity->PhysicsID > -1)
					m_Physics.deletePhysicsObject(entity->PhysicsID);
				//Remove entity from manager
				m_EntityMgr.removeEntity(eID);

				delete msg;
				break;
			}
		case MSG_SETVELOCITY:
			{
				SMessageSetVelocity* msgx = (SMessageSetVelocity*)msg;
				int eID = *(msgx->ID);
				EntityData* entity = m_EntityMgr.getEntity(eID);

				entity->Velocity = msgx->Velocity;

				if (entity->PhysicsID > -1)
					m_Physics.setLinearVelocity(entity->PhysicsID, msgx->Velocity);

				delete msg;
				break;
			}
		case MSG_SETFORCE:
			{
				SMessageSetForce* msgx = (SMessageSetForce*)msg;
				int eID = *(msgx->ID);
				EntityData* entity = m_EntityMgr.getEntity(eID);

				entity->Force = msgx->Force;

				delete msg;
				break;
			}
		case MSG_INCSCRIPTFLAG1:
			{
				SMessageIncScriptFlag1* msgx = (SMessageIncScriptFlag1*)msg;
				int eID = msgx->ID;
				EntityData* entity = m_EntityMgr.getEntity(eID);

				++(entity->ScriptFlag1);

				delete msg;
				break;
			}
		case MSG_SETPOSITION:
			{
				SMessageSetPosition* msgx = (SMessageSetPosition*)msg;
				int eID = *(msgx->ID);
				EntityData* entity = m_EntityMgr.getEntity(eID);

				entity->Position = msgx->Position;

				if (entity->PhysicsID > -1)
					m_Physics.setPosition(entity->PhysicsID, msgx->Position);

				delete msg;
				break;
			}
		case MSG_SETROTATION:
			{
				SMessageSetRotation* msgx = (SMessageSetRotation*)msg;
				int eID = *(msgx->ID);
				EntityData* entity = m_EntityMgr.getEntity(eID);

				if (entity->PhysicsID > -1)
					m_Physics.setRotation(entity->PhysicsID, msgx->Axis, msgx->Degree);

				D3DXQUATERNION Rot = m_Physics.getRotation(entity->PhysicsID);// WARNING - relies on entity having physics id
				entity->Rotation = Rot;

				delete msg;
				break;
			}
		case MSG_ROTATEONX:
			{
				SMessageAddRotationOnX* msgx = (SMessageAddRotationOnX*)msg;
				int eID = *(msgx->ID);
				EntityData* entity = m_EntityMgr.getEntity(eID);

				if(entity->PhysicsID > -1)
					m_Physics.rotateOnCoordAxis(entity->PhysicsID, msgx->Degree, X);

				D3DXQUATERNION Rot = m_Physics.getRotation(entity->PhysicsID);// WARNING - relies on entity having physics id
				entity->Rotation = Rot;

				delete msg;
				break;
			}
			case MSG_ROTATEONY:
			{
				SMessageAddRotationOnY* msgx = (SMessageAddRotationOnY*)msg;
				int eID = *(msgx->ID);
				EntityData* entity = m_EntityMgr.getEntity(eID);

				if (entity->PhysicsID > -1)
					m_Physics.rotateOnCoordAxis(entity->PhysicsID, msgx->Degree, Y);

				D3DXQUATERNION Rot = m_Physics.getRotation(entity->PhysicsID);// WARNING - relies on entity having physics id
				entity->Rotation = Rot;

				delete msg;
				break;
			}
			case MSG_ROTATEONZ:
			{
				SMessageAddRotationOnZ* msgx = (SMessageAddRotationOnZ*)msg;
				int eID = *(msgx->ID);
				EntityData* entity = m_EntityMgr.getEntity(eID);

				if (entity->PhysicsID > -1)
					m_Physics.rotateOnCoordAxis(entity->PhysicsID, msgx->Degree, Z);

				D3DXQUATERNION Rot = m_Physics.getRotation(entity->PhysicsID);// WARNING - relies on entity having physics id
				entity->Rotation = Rot;

				delete msg;
				break;
			}
			case MSG_SETSTEERING:
			{
				SMessageSetSteering* msgx = (SMessageSetSteering*)msg;
				int eID = *(msgx->ID);
				EntityData* entity = m_EntityMgr.getEntity(eID);

				entity->SteeringType = msgx->SteeringType;

				delete msg;
				break;
			}
		default:
			{
				tempQueue.push(msg);
				break;
			}
		}
		m_QueueBuffer.pop();
	}
	m_QueueBuffer = tempQueue;
}

void EntityCore::PhysicsMsg()
{
}

void EntityCore::Export()
{
	EntityList entityList;
	m_EntityMgr.cloneInto(entityList);

	//GraphicsCore export
	vector<DD_StaticMeshData> staticMeshExport;
	//ScriptingCore export
	vector<ScriptData> scriptExport;
	//Picking export
	DDPickingData Pickingtemp;
	//Position export
	vector<DDGameData> posExport;
	//Steering export
	vector<DDSteeringData> steerExport;
	vector<SteeringObstacles> obstacleExport;

	EntityList::iterator end = entityList.end();
	for (EntityList::iterator itr = entityList.begin(); itr != end; ++itr)
	{
		EntityData entityData = *itr;
		//Export to Graphics
		switch(gEPMgr->GetItem(entityData.Type)->RenderType)
		{
		case SCENE_STATICMESH:
			{
				//Export Graphics
				DD_StaticMeshData SMDtemp;
				TranslateToStaticMesh(entityData, SMDtemp);
				staticMeshExport.push_back(SMDtemp);
				//Export scripting
				ScriptData SDtemp;
				TranslateToScriptData(entityData, SDtemp);
				scriptExport.push_back(SDtemp);
				//Export picking
				Pickingtemp.ID.push_back(entityData.ID);
				Pickingtemp.Position.push_back(entityData.Position);
				Pickingtemp.Halfscale.push_back(gEPMgr->GetItem(entityData.Type)->HalfScale);
				break;
			}
		}
		//Export to Physics
		if(entityData.PhysicsID > -1)
		{
			m_Physics.applyCentralForce(entityData.PhysicsID, entityData.Force);
		}
		//if(entityData.Type == EP_SMALLMARBLE || entityData.Type == EP_MEDIUMMARBLE ||
		//	entityData.Type == EP_LARGEMARBLE || entityData.Type == EP_PLAYERMARBLE)
		{
			DDGameData temp;
			temp.ID = entityData.ID;
			temp.Type = entityData.Type;
			temp.Position = entityData.Position;
			posExport.push_back(temp);
		}

		//Export to AI
		if(entityData.SteeringType &&
			(entityData.Type == EP_SMALLMARBLE || entityData.Type == EP_MEDIUMMARBLE ||
			entityData.Type == EP_LARGEMARBLE))
		{
			DDSteeringData temp;
			temp.ID = entityData.ID;
			temp.m_dSpeed = D3DXVec3Length(&(entityData.Velocity));
			temp.m_iEvadeID = 0;
			temp.m_iPursuitID = 0;
			temp.SHeading = SVector3D(0, 0);
			temp.SPosition = SVector3D(entityData.Position.x, entityData.Position.z);
			temp.SteeringType = entityData.SteeringType;
			temp.SVelocity = SVector3D(entityData.Velocity.x, entityData.Velocity.z);

			steerExport.push_back(temp);
		}
		if(entityData.Type == EP_WALL || entityData.Type == EP_BOX)
		{
			SteeringObstacles temp;
			temp.m_iID = entityData.ID;
			temp.m_dRadius = gEPMgr->GetItem(entityData.Type)->HalfScale.x;
			temp.m_dScale = 1;
			temp.m_dTagged = false;
			temp.SPosition = SVector3D(entityData.Position.x, entityData.Position.z);
			obstacleExport.push_back(temp);
		}
	}
	m_PositionExport.Overwrite(posExport);
	m_StaticMeshExport.Overwrite(staticMeshExport);
	m_ScriptExport.Overwrite(scriptExport);
	m_PickingExport.Overwrite(Pickingtemp);
	m_SteeringExport.Overwrite(steerExport);
	m_ObstacleExport.Overwrite(obstacleExport);
}

void EntityCore::PollPhysics()
{
	EntityList::iterator end = m_EntityMgr.end();
	for (EntityList::iterator itr = m_EntityMgr.begin(); itr != end; ++itr)
	{
		EntityData& entityData = *(itr);

		if (entityData.PhysicsID > -1)
		{
			entityData.Position = m_Physics.getPosition(entityData.PhysicsID);
			entityData.Velocity = m_Physics.getLinearVelocity(entityData.PhysicsID);
			entityData.Rotation = m_Physics.getRotation(entityData.PhysicsID);
		}
	}
}

/*
This function translates the entity into static mesh
takes in and entity + id and DD_StaticMeshData, both reference
outputs onto the second parameter
*/
void EntityCore::TranslateToStaticMesh(EntityData &entity, DD_StaticMeshData &out)
{
	out.MeshID = gEPMgr->GetItem(entity.Type)->GResourceID;
	out.TextureID = 0;
	D3DXMATRIX m;
	D3DXMatrixTransformation(&m, &D3DXVECTOR3(0,0,0), 0, 
		&(gEPMgr->GetItem(entity.Type)->GraphicsScale), &D3DXVECTOR3(0,0,0), 
		&(entity.Rotation), &(entity.Position));
	out.Transform = m;
}

/*
This function translates the entity into scripting data
takes in an entity + id and ScriptData, both reference as inputs
outputs onto the second parameter
*/
void EntityCore::TranslateToScriptData(EntityData &entity, ScriptData &out)
{
	out.objID = entity.ID;
	out.objType = gEPMgr->GetItem(entity.Type)->ScriptName;
	out.LMouseClick = entity.ScriptFlag1;
	out.RMouseClick = entity.ScriptFlag2;
	out.Collision = 0;
}
