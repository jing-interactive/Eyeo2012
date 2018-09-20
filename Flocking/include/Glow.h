//
//  Glow.h
//  Collider
//
//  Created by Robert Hodgin on 3/6/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#pragma once

#include "cinder/Vector.h"
#include "cinder/Color.h"

class Glow {
public:
	
	Glow();
	Glow( const ci::vec3 &pos, const ci::vec3 &vel, float radius, ci::Color c, float lifespan );
	void update( float timeDelta );
	void draw( const ci::vec3 &right, const ci::vec3 &up );
	
	ci::vec3	mPos;
	ci::vec3	mVel;
	float		mRot, mRotVel;
	float		mRadius, mRadiusDest;
	float		mAge, mAgePer, mLifespan;
	ci::Color	mColor;
	bool		mIsDead;
};