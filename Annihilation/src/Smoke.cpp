//
//  Glow.cpp
//  Collider
//
//  Created by Robert Hodgin on 3/6/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#include "Smoke.h"
#include "cinder/gl/gl.h"
#include "cinder/Rand.h"

using namespace ci;

Smoke::Smoke()
{
}

Smoke::Smoke( const vec3 &pos, const vec3 &vel, float radius, float lifespan )
{
	mPos		= pos;
	mVel		= vel;
	mRadius		= radius;
	mAge		= 0.0f;
	mLifespan	= lifespan;
	mIsDead		= false;
	mRot		= Rand::randFloat( 360.0f );
	mRotVel		= Rand::randFloat( -2.0f, 2.0f );
	mRadiusVel	= Rand::randFloat( 15.0f, 38.0f );
}

void Smoke::update( float timeDelta )
{
	mPos	+= mVel * timeDelta;
	mVel	-= mVel * 0.01f * timeDelta;
	mAge	+= timeDelta;
	mAgePer  = 1.0f - mAge/mLifespan;
	//			mRadius *= 0.99f;
	
//	mRot	+= mRotVel;
	mRadius += mRadiusVel * timeDelta;
	mRadiusVel *= 0.8f;
	
	if( mAge > mLifespan ){// || mRadius < 1.0f ){
		mIsDead = true;
		//				mRadius = 1.0f;
	}
}

void Smoke::draw( const vec3 &right, const vec3 &up )
{
	gl::color( ColorA( 1.0f, 1.0f, 1.0f, mAgePer * 0.75f ) );
	gl::drawBillboard( mPos, vec2( mRadius, mRadius ), mRot, right, up );
}
