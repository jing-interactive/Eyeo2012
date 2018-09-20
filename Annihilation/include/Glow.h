//
//  Glow.h
//  Collider
//
//  Created by Robert Hodgin on 3/6/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#pragma once

#include "cinder/Vector.h"

class Glow {
public:
	
	Glow();
	Glow( const ci::vec3 &pos, const ci::vec3 &vel, float radius, float lifespan, const ci::vec3 &right, const ci::vec3 &up );
	void update( float timeDelta );
	void draw();
	
	ci::vec3	mPos;
	ci::vec3	mVel;
	float		mRot;
	float		mRadius;
	float		mAge, mAgePer, mLifespan;
	bool		mIsDead;
	
	ci::vec3	mRight;
	ci::vec3	mUp;
};