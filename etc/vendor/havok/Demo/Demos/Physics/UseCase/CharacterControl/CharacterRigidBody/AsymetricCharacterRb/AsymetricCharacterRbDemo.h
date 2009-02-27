/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2008 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_ASYMETRIC_CHARACTER_RIGIDBODY_DEMO_H
#define HK_ASYMETRIC_CHARACTER_RIGIDBODY_DEMO_H

#include <Demos/DemoCommon/DemoFramework/hkDefaultPhysicsDemo.h>

class hkpRigidBody;
class hkpCharacterRigidBody;
class hkpCharacterContext;

struct hkpSurfaceInfo;

class AsymetricCharacterRbDemo : public hkDefaultPhysicsDemo
{
	public:

		HK_DECLARE_CLASS_ALLOCATOR(HK_MEMORY_CLASS_DEMO);

		AsymetricCharacterRbDemo(hkDemoEnvironment* env);
		
		~AsymetricCharacterRbDemo();

		hkDemo::Result stepDemo();

		// Forward orientation controller 
		hkBool reorientCharacter(const hkRotation& rotation);

	private:

		hkReal m_time;

		hkReal m_currentAngle;

		hkpCharacterRigidBody* m_characterRigidBody;

		hkpCharacterContext* m_characterContext;

		hkVector4	m_rigidBodyNormal;

		// An array of heighfield values we use for this demo
		hkReal*	m_data;

		// Previous surface info
		hkpSurfaceInfo*	m_previousGround;
		hkInt32 m_framesInAir;


};

#endif // HK_ASYMETRIC_CHARACTER_RIGIDBODY_DEMO_H

/*
* Havok SDK - NO SOURCE PC DOWNLOAD, BUILD(#20080925)
* 
* Confidential Information of Havok.  (C) Copyright 1999-2008
* Telekinesys Research Limited t/a Havok. All Rights Reserved. The Havok
* Logo, and the Havok buzzsaw logo are trademarks of Havok.  Title, ownership
* rights, and intellectual property rights in the Havok software remain in
* Havok and/or its suppliers.
* 
* Use of this software for evaluation purposes is subject to and indicates
* acceptance of the End User licence Agreement for this product. A copy of
* the license is included with this software and is also available at www.havok.com/tryhavok.
* 
*/