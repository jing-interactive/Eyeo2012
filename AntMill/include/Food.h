//
//  Food.h
//  AntMill
//
//  Created by Robert Hodgin on 5/26/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#pragma once

class Food 
{
  public:
	Food();
	Food( const ci::vec3 &pos, float charge );
	void update( float dt, const ci::vec3 &roomDims );
	void draw();
	void stayInBounds( ci::vec3 *v, const ci::vec3 &roomDims );
	
	
	ci::vec3	mPos;
	ci::vec3	mVel;
	float		mCharge;
	ci::Color	mColor;
	bool		mGrabbed;
	bool		mIsGone;
};