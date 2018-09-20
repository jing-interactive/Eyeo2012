//
//  Particle.h
//  Forces
//
//  Created by Robert Hodgin on 5/14/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#pragma once
#include "cinder/Camera.h"
#include "cinder/TriMesh.h"
#include "Shard.h"
#include <vector>

class Particle {
  public:
	Particle();
	Particle( const ci::vec3 &pos, float charge );
	void update( const ci::Camera &cam, float dt );
	void draw();
	void addShards( int amt );
	
	ci::vec3	mPos;
	ci::vec3	mVel;
	ci::vec3	mAcc;
	float		mForce;	
	float		mColor;
	float		mRadius;
	float		mShellRadius;
	float		mCharge;
	
	ci::mat4 mMatrix;

	ci::vec2	mScreenPos;
	float		mScreenRadius;
	
	std::vector<Shard>	mShards;
	ci::TriMesh			mShardMesh;
};