//
//  Moth.h
//  BubbleChamber
//
//  Created by Robert Hodgin on 5/7/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#pragma once

#include "cinder/Vector.h"
#include "Room.h"

class Moth {
public:
	Moth();
	Moth( const ci::vec3 &pos );
	void update( Room *room, float dt );
	void checkBoundary( Room *room );
	void draw();
	
	ci::vec3		mPos;
	ci::vec3		mVel;
	ci::vec3		mAcc;
	
	ci::vec3		mAxis;

	float			mColorf;
	
	float			mAge;
	bool			mIsDead;
};