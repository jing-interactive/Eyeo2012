//
//  Predator.h
//  FlockingFix
//
//  Created by Robert Hodgin on 5/12/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#pragma once
#include "cinder/Vector.h"
#include "cinder/Color.h"
#include <vector>

class Predator {
public:
	Predator();
	Predator( const ci::vec3 &v );
	void update( const ci::vec3 &v );
	void draw();
	
	int mLen;
	ci::vec3	mPos;
	std::vector<ci::vec3>	mPositions;
	
	float					mRadius;
	std::vector<float>		mRadii;
};