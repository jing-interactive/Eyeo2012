//
//  Glob.h
//  Forces
//
//  Created by Robert Hodgin on 5/24/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#pragma once

class Glob {
public:
	Glob();
	Glob( const ci::vec3 &pos, const ci::vec3 &vel, float radius, float lifespan );
	
	void update( float floorLevel, float dt );
	void draw( const ci::vec3 &right, const ci::vec3 &up );
	
	ci::vec3	mPos, mVel;
	float		mColor;
	float		mRadius;
	float		mAge, mAgePer;
	float		mLifespan;
	bool		mIsDead;
	
	bool		mBounced;
	
};
