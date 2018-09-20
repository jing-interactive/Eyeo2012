//
//  GlowCube.h
//  BubbleChamber
//
//  Created by Robert Hodgin on 5/23/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#pragma once

#include "cinder/Vector.h"

class GlowCube {
public:
	GlowCube();
	GlowCube( const ci::vec3 &pos );
	void update( float dt );
	void draw();
	
	ci::vec3		mPos;
	ci::vec3		mVel;
	ci::vec3		mAcc;
	
	ci::vec3		mAxis;
	float			mAngle, mAngleVel;
	float			mRadius;
	
	ci::mat4	mMatrix;
	
	float			mAge;
	float			mAgePer;
	float			mLifespan;
	bool			mIsDead;
};