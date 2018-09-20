//
//  Shockwave.h
//  Collider
//
//  Created by Robert Hodgin on 3/6/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#pragma once

#include "cinder/Color.h"
#include "cinder/Vector.h"

class Shockwave {
  public:
	Shockwave();
	Shockwave( const ci::vec3 &pos, const ci::vec3 &axis, float lifespan, float speed );
	void update( float timeDelta );

	ci::vec3	mPos;
	ci::vec3	mAxis;
	float		mRadius, mRadiusPrev;
	float		mRot, mRotVel;
	float		mStrength;
	float		mImpulse;
	float		mShell, mShellDest;
	float		mAge, mAgePer;
	float		mSpeed;
	float		mLifespan;
	ci::Color	mColor;
	bool		mIsDead;
	
};