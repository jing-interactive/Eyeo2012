//
//  Nebula.cpp
//  Collider
//
//  Created by Robert Hodgin on 2/9/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#include "cinder/app/App.h"
#include "cinder/gl/gl.h"
#include "cinder/Rand.h"
#include "Nebula.h"

using namespace ci;

Nebula::Nebula()
{
}

Nebula::Nebula( const vec3 &pos, const vec3 &vel, float radius, Color c, float lifespan )
{
	mPos			= pos;
	mVel			= vel;
	mRadiusDest		= radius;
	mRadius			= radius;
	mColor			= c;
	mAge			= 0.0f;
	mLifespan		= Rand::randFloat( lifespan * 0.5f, lifespan );
	mRadiusMulti	= 0.3f;
	mRot			= Rand::randFloat( 360.0f );
	mAgePer			= 0.0f;
	mIsDead			= false;
}

void Nebula::update( float dt )
{
	mPos	+= mVel * dt;
	mVel	-= mVel * 0.01 * dt;
	mRadiusDest += mRadiusMulti * dt;
	mRadiusMulti -= mRadiusMulti * 0.03f * dt;
	
	mRadius -= ( mRadius - mRadiusDest ) * 0.1f * dt;
	
	mAge += dt;
	mAgePer = 1.0f - mAge/mLifespan;//sin( ( mAge/mLifespan ) * M_PI );
	
	if( mAge > mLifespan ){
		mIsDead = true;
	}
}

void Nebula::draw( const vec3 &right, const vec3 &up )
{
	gl::drawBillboard( mPos, vec2( mRadius, mRadius ), mRot, right, up );
}
