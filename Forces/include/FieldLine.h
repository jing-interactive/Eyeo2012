//
//  FieldLine.h
//  Forces
//
//  Created by Robert Hodgin on 5/14/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#pragma once

#include "cinder/Vector.h"
#include <vector>

class FieldLine {
public:
	
	FieldLine();
	FieldLine( const ci::vec3 &pos, const ci::vec3 &vel, float charge, float lifespan );
	void update( float dt, bool tick );
	void draw();
	
	ci::vec3	mPos;
	ci::vec3	mVel;
	
	std::vector<ci::vec3>	mPositions;
//	std::vector<ci::vec3>	mNormals;
//	std::vector<ci::vec3>	mPerps;
	
	float		mMaxSpeed;
	float		mCharge;
	float		mAge, mAgePer, mLifespan;
	float		mColor;
	bool		mIsDead;
	
	static int		sLen;
	static float	sInvLen;
	static int		sNumVerts;
};