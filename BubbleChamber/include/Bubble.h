//
//  Bubble.h
//  BubbleChamber
//
//  Created by Robert Hodgin on 5/3/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#pragma once

#include "cinder/Vector.h"
#include "Room.h"

class Bubble {
public:
	Bubble();
	Bubble( const ci::vec3 &pos, const ci::vec3 &vec, float age );
	void init( const ci::vec3 &pos, const ci::vec3 &vec, float lifespan );
	void update( Room *room, float dt );
	void draw();
	
	ci::vec3		mPos;
	ci::vec3		mVel;
	ci::vec3		mAcc;
	
	float			mAge, mAgePer;
	float			mLifespan;
	
	bool			mIsDead;
	
	static ci::vec3 mBuoyancy;
};