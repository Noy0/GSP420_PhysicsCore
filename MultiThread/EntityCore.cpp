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
	m_Physics.CreateWorld(winfo);
}

void EntityCore::Update(float dt)
{
	CopyMessage();
	EntityMgrMsg();
	PhysicsMsg();
	m_Physics.UpdateWorld(dt, 1);
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
		SMessage* p_msg = m_QueueBuffer.front();
		switch(p_msg->Message)
		{
		case MSG_ADDENTITY:
			{
				SMessageAddEntity* p_msgx = (SMessageAddEntity*)p_msg;
				EntityPropertyData* p_newEPD = gEPMgr->GetItem(p_msgx->Type);
				//Add entity to manager
				EntityData* p_entity = m_EntityMgr.NewEntity();
				*(p_msgx->ID) = p_entity->ID;
				p_entity->type = p_msgx->Type;
				p_entity->position = p_msgx->Position;

				//Add object to physics
				switch(p_newEPD->BodyType)
				{
				case COLLIDER_SPHERE:
					{
						SpherePMat pMat;
						pMat.friction = p_newEPD->Friction;
						pMat.restitution = p_newEPD->Restitution;
						pMat.mass = p_newEPD->Mass;
						pMat.angularDamping = p_newEPD->AngularDampen;
						pMat.radius = p_newEPD->Radius;
						int pID = m_Physics.CreatePhysics_Object(pMat, p_msgx->Position);
						p_entity->physicsID = pID;
						if(p_newEPD->PhysicsType == PHYSICS_KINEMATIC)
							m_Physics.SetAsKinematic(pID);

						break;
					}
				case COLLIDER_CYLINDER:
					{
						CylinderPMat pMat;
						pMat.friction = p_newEPD->Friction;
						pMat.restitution = p_newEPD->Restitution;
						pMat.mass = p_newEPD->Mass;
						pMat.scalar = p_newEPD->HalfScale;
						//radius
						//length
						int pID = m_Physics.CreatePhysics_Object(pMat, p_msgx->Position);
						p_entity->physicsID = pID;
						if(p_newEPD->PhysicsType == PHYSICS_KINEMATIC)
							m_Physics.SetAsKinematic(pID);

						break;
					}
				case COLLIDER_BOX:
					{
						BoxPMat pMat;
						pMat.friction = p_newEPD->Friction;
						pMat.restitution = p_newEPD->Restitution;
						pMat.mass = p_newEPD->Mass;
						pMat.scalar = p_newEPD->HalfScale;
						int pID = m_Physics.CreatePhysics_Object(pMat, p_msgx->Position);
						p_entity->physicsID = pID;
						if(p_newEPD->PhysicsType == PHYSICS_KINEMATIC)
							m_Physics.SetAsKinematic(pID);

						break;
					}
				}

				delete p_msg;
				break;
			}
		case MSG_REMOVEENTITY:
			{
				SMessageRemoveEntity* msgx = (SMessageRemoveEntity*)p_msg;
				int eID = *(msgx->ID);
				EntityData* p_entity = m_EntityMgr.GetEntity(eID);

				//Remove object from physics
				if(p_entity->physicsID > -1)
					m_Physics.DeletePhysicsObject(p_entity->physicsID);
				//Remove entity from manager
				m_EntityMgr.RemoveEntity(eID);

				delete p_msg;
				break;
			}
		case MSG_SETVELOCITY:
			{
				SMessageSetVelocity* msgx = (SMessageSetVelocity*)p_msg;
				int eID = *(msgx->ID);
				EntityData* p_entity = m_EntityMgr.GetEntity(eID);

				p_entity->velocity = msgx->Velocity;

				if (p_entity->physicsID > -1)
					m_Physics.SetLinearVelocity(p_entity->physicsID, msgx->Velocity);

				delete p_msg;
				break;
			}
		case MSG_SETFORCE:
			{
				SMessageSetForce* msgx = (SMessageSetForce*)p_msg;
				int eID = *(msgx->ID);
				EntityData* p_entity = m_EntityMgr.GetEntity(eID);

				p_entity->force = msgx->Force;

				delete p_msg;
				break;
			}
		case MSG_INCSCRIPTFLAG1:
			{
				SMessageIncScriptFlag1* msgx = (SMessageIncScriptFlag1*)p_msg;
				int eID = msgx->ID;
				EntityData* p_entity = m_EntityMgr.GetEntity(eID);

				++(p_entity->scriptFlag1);

				delete p_msg;
				break;
			}
		case MSG_SETPOSITION:
			{
				SMessageSetPosition* msgx = (SMessageSetPosition*)p_msg;
				int eID = *(msgx->ID);
				EntityData* p_entity = m_EntityMgr.GetEntity(eID);

				p_entity->position = msgx->Position;

				if (p_entity->physicsID > -1)
					m_Physics.SetPosition(p_entity->physicsID, msgx->Position);

				delete p_msg;
				break;
			}
		case MSG_SETROTATION:
			{
				SMessageSetRotation* msgx = (SMessageSetRotation*)p_msg;
				int eID = *(msgx->ID);
				EntityData* p_entity = m_EntityMgr.GetEntity(eID);

				if (p_entity->physicsID > -1)
					m_Physics.SetRotation(p_entity->physicsID, msgx->Axis, msgx->Degree);

				D3DXQUATERNION Rot = m_Physics.GetRotation(p_entity->physicsID);// WARNING - relies on entity having physics id
				p_entity->rotation = Rot;

				delete p_msg;
				break;
			}
		case MSG_ROTATEONX:
			{
				SMessageAddRotationOnX* msgx = (SMessageAddRotationOnX*)p_msg;
				int eID = *(msgx->ID);
				EntityData* p_entity = m_EntityMgr.GetEntity(eID);

				if(p_entity->physicsID > -1)
					m_Physics.RotateOnCoordAxis(p_entity->physicsID, msgx->Degree, X);

				D3DXQUATERNION Rot = m_Physics.GetRotation(p_entity->physicsID);// WARNING - relies on entity having physics id
				p_entity->rotation = Rot;

				delete p_msg;
				break;
			}
			case MSG_ROTATEONY:
			{
				SMessageAddRotationOnY* msgx = (SMessageAddRotationOnY*)p_msg;
				int eID = *(msgx->ID);
				EntityData* p_entity = m_EntityMgr.GetEntity(eID);

				if (p_entity->physicsID > -1)
					m_Physics.RotateOnCoordAxis(p_entity->physicsID, msgx->Degree, Y);

				D3DXQUATERNION Rot = m_Physics.GetRotation(p_entity->physicsID);// WARNING - relies on entity having physics id
				p_entity->rotation = Rot;

				delete p_msg;
				break;
			}
			case MSG_ROTATEONZ:
			{
				SMessageAddRotationOnZ* msgx = (SMessageAddRotationOnZ*)p_msg;
				int eID = *(msgx->ID);
				EntityData* p_entity = m_EntityMgr.GetEntity(eID);

				if (p_entity->physicsID > -1)
					m_Physics.RotateOnCoordAxis(p_entity->physicsID, msgx->Degree, Z);

				D3DXQUATERNION Rot = m_Physics.GetRotation(p_entity->physicsID);// WARNING - relies on entity having physics id
				p_entity->rotation = Rot;

				delete p_msg;
				break;
			}
			case MSG_SETSTEERING:
			{
				SMessageSetSteering* msgx = (SMessageSetSteering*)p_msg;
				int eID = *(msgx->ID);
				EntityData* p_entity = m_EntityMgr.GetEntity(eID);

				p_entity->steeringType = msgx->SteeringType;

				delete p_msg;
				break;
			}
		default:
			{
				tempQueue.push(p_msg);
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
	m_EntityMgr.CloneInto(entityList);

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
		switch(gEPMgr->GetItem(entityData.type)->RenderType)
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
				Pickingtemp.Position.push_back(entityData.position);
				Pickingtemp.Halfscale.push_back(gEPMgr->GetItem(entityData.type)->HalfScale);
				break;
			}
		}
		//Export to Physics
		if(entityData.physicsID > -1)
		{
			m_Physics.ApplyCentralForce(entityData.physicsID, entityData.force);
		}
		//if(entityData.Type == EP_SMALLMARBLE || entityData.Type == EP_MEDIUMMARBLE ||
		//	entityData.Type == EP_LARGEMARBLE || entityData.Type == EP_PLAYERMARBLE)
		{
			DDGameData temp;
			temp.ID = entityData.ID;
			temp.Type = entityData.type;
			temp.Position = entityData.position;
			posExport.push_back(temp);
		}

		//Export to AI
		if(entityData.steeringType &&
			(entityData.type == EP_SMALLMARBLE || entityData.type == EP_MEDIUMMARBLE ||
			entityData.type == EP_LARGEMARBLE))
		{
			DDSteeringData temp;
			temp.ID = entityData.ID;
			temp.m_dSpeed = D3DXVec3Length(&(entityData.velocity));
			temp.m_iEvadeID = 0;
			temp.m_iPursuitID = 0;
			temp.SHeading = SVector3D(0, 0);
			temp.SPosition = SVector3D(entityData.position.x, entityData.position.z);
			temp.SteeringType = entityData.steeringType;
			temp.SVelocity = SVector3D(entityData.velocity.x, entityData.velocity.z);

			steerExport.push_back(temp);
		}
		if(entityData.type == EP_WALL || entityData.type == EP_BOX)
		{
			SteeringObstacles temp;
			temp.m_iID = entityData.ID;
			temp.m_dRadius = gEPMgr->GetItem(entityData.type)->HalfScale.x;
			temp.m_dScale = 1;
			temp.m_dTagged = false;
			temp.SPosition = SVector3D(entityData.position.x, entityData.position.z);
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
	EntityList::iterator end = m_EntityMgr.End();
	for (EntityList::iterator itr = m_EntityMgr.Begin(); itr != end; ++itr)
	{
		EntityData& entityData = *(itr);

		if (entityData.physicsID > -1)
		{
			entityData.position = m_Physics.GetPosition(entityData.physicsID);
			entityData.velocity = m_Physics.GetLinearVelocity(entityData.physicsID);
			entityData.rotation = m_Physics.GetRotation(entityData.physicsID);
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
	out.MeshID = gEPMgr->GetItem(entity.type)->GResourceID;
	out.TextureID = 0;
	D3DXMATRIX m;
	D3DXMatrixTransformation(&m, &D3DXVECTOR3(0,0,0), 0, 
		&(gEPMgr->GetItem(entity.type)->GraphicsScale), &D3DXVECTOR3(0,0,0), 
		&(entity.rotation), &(entity.position));
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
	out.objType = gEPMgr->GetItem(entity.type)->ScriptName;
	out.LMouseClick = entity.scriptFlag1;
	out.RMouseClick = entity.scriptFlag2;
	out.Collision = 0;
}
