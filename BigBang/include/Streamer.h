//
//  Confetti.h
//  BigBang
//
//  Created by Robert Hodgin on 5/15/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#pragma once
#include "cinder/Perlin.h"

class Streamer {
  public:
	Streamer();
	Streamer( const ci::vec3 &pos, int presetIndex );
	void update( const ci::vec3 &roomDims, float dt, bool tick );
	void checkBounds( const ci::vec3 &roomDims );
	void draw();
	
	ci::vec3	mPos;
	int			mLen;
	std::vector<ci::vec3>	mPositions;
	std::vector<ci::vec3>	mVelocities;
	std::vector<ci::vec3>	mAccels;
	ci::vec3	mVel;
	ci::vec3	mAcc;
	
	float		mGravity;
	
	float		mRadius;
	ci::Color	mColor;
	
	int			mFrameCount;
	float		mAge;
	float		mAgePer;
	float		mLifespan;
	bool		mIsDead;
};