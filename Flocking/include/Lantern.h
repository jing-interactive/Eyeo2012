//
//  Bait.h
//  Flocking
//
//  Created by Robert Hodgin on 4/26/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#pragma once
#include "cinder/Vector.h"
#include "cinder/Color.h"

class Lantern {
  public:
	Lantern();
	Lantern( const ci::vec3 &pos );
	void update( float dt, float yFloor );
	void draw();
	
	ci::vec3	mPos;
	float		mRadius;
	float		mRadiusDest;
	float		mFallSpeed;
	ci::Color	mColor;
	
	float		mVisiblePer;
	
	bool		mIsDead;
	bool		mIsSinking;
	bool		mIsDying;
};