#include "HavokPhysicsSystem.h"

#include <Common/Base/hkBase.h>
#include <Common/Base/System/hkBaseSystem.h>
#include <Common/Base/Memory/hkThreadMemory.h>
#include <Common/Base/Memory/Memory/Pool/hkPoolMemory.h>
#include <Common/Base/System/Error/hkDefaultError.h>
#include <Common/Base/Monitor/hkMonitorStream.h>

#include <Physics/Collide/hkpCollide.h>										
#include <Physics/Collide/Agent/ConvexAgent/SphereBox/hkpSphereBoxAgent.h>	
#include <Physics/Collide/Shape/Convex/Box/hkpBoxShape.h>					
#include <Physics/Collide/Shape/Convex/Sphere/hkpSphereShape.h>				
#include <Physics/Collide/Dispatch/hkpAgentRegisterUtil.h>					

#include <Physics/Collide/Query/CastUtil/hkpWorldRayCastInput.h>			
#include <Physics/Collide/Query/Collector/RayCollector/hkpAllRayHitCollector.h>
#include <Physics/Collide/Query/CastUtil/hkpWorldRayCastOutput.h>			

#include <Physics/Dynamics/World/hkpWorld.h>								
#include <Physics/Dynamics/Entity/hkpRigidBody.h>							
#include <Physics/Utilities/Dynamics/Inertia/hkpInertiaTensorComputer.h>	

#include <Common/Base/Thread/Job/ThreadPool/Cpu/hkCpuJobThreadPool.h>
#include <Common/Base/Thread/Job/ThreadPool/Spu/hkSpuJobThreadPool.h>
#include <Common/Base/Thread/JobQueue/hkJobQueue.h>	

#include <Common/Base/keycode.cxx>

#define INCLUDE_HAVOK_ANIMATION_CLASSES
#define INCLUDE_HAVOK_PHYSICS_CLASSES

#define HK_CLASSES_FILE <Common/Serialize/Classlist/hkClasses.h>
#include <Common/Serialize/Util/hkBuiltinTypeRegistry.cxx>
#define HK_COMPAT_FILE <Common/Compat/hkCompatVersions.h>
#include <Common/Compat/hkCompat_None.cxx> 

#include "../Logging/Logger.h"

#include "../System/Management.h"

ISystemScene* HavokPhysicsSystem::CreateScene( )
{
	hkpWorldCinfo info;

	info.m_simulationType = hkpWorldCinfo::SIMULATION_TYPE_DISCRETE;
	info.m_gravity.set( 0,-9.8f,0);
	info.m_collisionTolerance = 0.1f; 
	info.setBroadPhaseWorldSize( 2000.0f );
	info.setupSolverInfo( hkpWorldCinfo::SOLVER_TYPE_4ITERS_MEDIUM );

	HavokPhysicsSystemScene* scene = new HavokPhysicsSystemScene( info );

	_scenes.push_back( scene );

	return scene;
}

void HavokPhysicsSystem::Initialize()
{
	hkPoolMemory* memoryManager = new hkPoolMemory( );
	_threadMemory = new hkThreadMemory( memoryManager );
	hkBaseSystem::init( memoryManager, _threadMemory, errorReportFunction );
	memoryManager->removeReference( );

	int stackSize = 0x100000;
	_stackBuffer = hkAllocate<char>( stackSize, HK_MEMORY_CLASS_BASE );
	hkThreadMemory::getInstance( ).setStackArea( _stackBuffer, stackSize );

	Management::GetInstance( )->GetServiceManager( )->RegisterService( this ); 
}

void HavokPhysicsSystem::Release()
{
	_threadMemory->setStackArea(0, 0);
	hkDeallocate( _stackBuffer );
	_threadMemory->removeReference();

	hkBaseSystem::quit( );
}

void HavokPhysicsSystem::errorReportFunction( const char* errorMessage, void* errorOutputObject )
{
	Logger::GetInstance( )->Warn( errorMessage );
}

std::vector< std::string > HavokPhysicsSystem::RayQuery( MathVector3 origin, MathVector3 destination, bool sortByDistance )
{
	hkpWorldRayCastInput input;
	input.m_from = origin.AshkVector4( );
	input.m_to = destination.AshkVector4( );

	hkpAllRayHitCollector collector;

	_scenes[ 0 ]->GetWorld( )->castRay( input, collector );

	if( sortByDistance )
	{
		collector.sortHits( );
	}

	std::vector< std::string > results;

	for( int i = 0; i < collector.getHits( ).getSize( ); i++ )
	{
		if ( collector.getHits( )[ i ].hasHit( ) )
		{
			hkpRigidBody* body = hkGetRigidBody( collector.getHits( )[ i ].m_rootCollidable );

			if ( body )
			{
				results.push_back( body->getName( ) );
			}
		}
	}

	return results;
}

AnyValue::AnyValueMap HavokPhysicsSystem::Execute( const std::string& actionName, AnyValueMap parameters )
{
	AnyValue::AnyValueMap results;

	if ( actionName == "rayQuery" )
	{
		results[ "hits" ] = this->RayQuery( 
			parameters[ "origin" ].GetValue< MathVector3 >( ),
			parameters[ "destination" ].GetValue< MathVector3 >( ),
			parameters[ "sortByDistance" ].GetValue< bool >( )
			);
	}

	return results;
}