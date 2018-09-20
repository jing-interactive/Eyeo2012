//
//  Nebula.h
//  Collider
//
//  Created by Robert Hodgin on 2/9/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#pragma once

class Nebula {
public:
	Nebula();
	Nebula( const ci::vec3 &pos, const ci::vec3 &vel, float radius, float charge, float lifespan );
	
	void update( float dt );
	void draw( const ci::vec3 &right, const ci::vec3 &up );
	
	ci::vec3	mPos, mVel;
	float		mCharge;
	float		mColor;
	float		mRadius, mRadiusDest, mRadiusMulti;
	float		mRot;
	float		mAge, mAgePer;
	float		mLifespan;
	bool		mIsDead;
	
};
