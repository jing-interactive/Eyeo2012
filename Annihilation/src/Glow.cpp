//
//  Glow.cpp
//  Collider
//
//  Created by Robert Hodgin on 3/6/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#include "Glow.h"
#include "cinder/gl/gl.h"
#include "cinder/Rand.h"

using namespace ci;

Glow::Glow()
{
}

Glow::Glow( const vec3 &pos, const vec3 &vel, float radius, float lifespan, const vec3 &right, const vec3 &up )
{
	mPos		= pos;
	mVel		= vel;
	mRadius		= radius;
	mAge		= 0.0f;
	mLifespan	= lifespan;
	mRight		= right;
	mUp			= up;
	
	mIsDead		= false;
	mRot		= ci::Rand::randFloat( (float)M_PI * 2.0f );
}

void Glow::update( float timeDelta )
{
	mPos	+= mVel * timeDelta;
	mVel	-= mVel * 0.01 * timeDelta;
	mAge	+= timeDelta;
	mAgePer  = 1.0f - mAge/mLifespan;
	//			mRadius *= 0.99f;
	
	if( mAge > mLifespan ){// || mRadius < 1.0f ){
		mIsDead = true;
		//				mRadius = 1.0f;
	}
}

void Glow::draw()
{
	gl::drawBillboard( mPos, ci::vec2( mRadius, mRadius ), 0.0f, mRight, mUp );
}
