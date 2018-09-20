//
//  Particle.h
//  BubbleChamber
//
//  Created by Robert Hodgin on 5/3/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#pragma once

#include "cinder/Vector.h"
#include "cinder/Ray.h"
#include "Room.h"
#include <vector>

class Controller;

class Particle {
  public:
	Particle();
	Particle( Controller *controller, int mGen, const ci::vec3 &pos, const ci::vec3 &dir, float speed, int len );
	void update( Room *room, float dt, bool tick );
	void draw();
	bool isOutOfBounds( Room *room );
	
	Controller		*mController;
	
	int				mGen;
	
	ci::Ray			mRay;
	
	int				mLen;
	std::vector<ci::vec3>	mPs;
	ci::vec3		mPos;
	ci::vec3		mVel;
	ci::vec3		mDeathAxis;
	float			mSpeed;
	
	float			mCharge;
	float			mAngle;
	float			mAngleDelta;
	float			mAngleDeltaDelta;
	
	ci::vec3		mAxis, mPerp1, mPerp2;
	
	ci::vec3		mDeathPos;
	ci::vec3		mPosLastTick;

	float			mAge, mAgePer;
	float			mLifespan;
	
	bool			mBounced;
	bool			mIsDying;
	float			mDyingCount;
	bool			mIsDead;
	bool			mIntersectsFloor;
};