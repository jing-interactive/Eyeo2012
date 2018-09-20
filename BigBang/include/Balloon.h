//
//  Confetti.h
//  BigBang
//
//  Created by Robert Hodgin on 5/15/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#pragma once
#include "cinder/Camera.h"

class Balloon {
  public:
	Balloon();
	Balloon( const ci::vec3 &pos, int presetIndex );
	void update( const ci::Camera &cam, const ci::vec3 &roomDims, float dt );
	void updateSpring( float dt );
	void checkBounds( const ci::vec3 &roomDims );
	void draw();
	
	ci::vec3	mPos;
	ci::vec3	mVel;
	ci::vec3	mAcc;
	
	ci::vec3	mSpringPos;
	ci::vec3	mSpringVel;
	ci::vec3	mSpringAcc;
	
	ci::vec2	mScreenPos;
	float		mScreenRadius;
	
	ci::mat4	mMatrix;
	
	float		mBuoyancy;
	float		mRadius;
	ci::Color	mColor;
	
	float		mAge;
	float		mLifespan;
	bool		mIsDead;
};