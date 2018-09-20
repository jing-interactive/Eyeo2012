//
//  Spark.h
//  Attraction
//
//  Created by Robert Hodgin on 4/24/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#pragma once
#include "cinder/Vector.h"
#include "cinder/Color.h"

class Spark {
public:
	Spark();
	Spark( ci::vec3 pos, ci::vec3 vel );
	void update( float timeDelta );
	void draw();
	
	// properties
	ci::vec3	mPos;
	ci::vec3	mVel;
	float		mAge, mLifespan;
	float		mAgePer;
	
	// state
	bool		mIsDead;
};