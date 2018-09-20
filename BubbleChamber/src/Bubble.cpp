//
//  Particle.cpp
//  BubbleChamber
//
//  Created by Robert Hodgin on 5/3/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#include "Bubble.h"
#include "cinder/gl/gl.h"
#include "cinder/Rand.h"

using namespace ci;

vec3 Bubble::mBuoyancy = vec3( 0.0f, 0.01f, 0.0f );

Bubble::Bubble()
{
}

Bubble::Bubble( const vec3 &pos, const vec3 &vel, float age )
: mPos( pos ), mVel( vel ), mAge( age )
{
	mAcc		= vec3();
	mVel += mBuoyancy * mAge;
	mPos += mVel * mAge;
	
	mLifespan	= Rand::randFloat( 10.0f, 40.0f );
	mAgePer		= 1.0f;
	
	mIsDead		= false;
}

void Bubble::update( Room *room, float dt )
{
	mAcc += mBuoyancy * dt;
	mVel += mAcc * dt;
	mPos += mVel * dt;
	mVel -= mVel * 0.01f * dt;
	mAcc = vec3();
	
	// TEST BOUNDARY
	vec3 dims = room->getDims();
	
	if( mPos.x > dims.x )		mIsDead = true;
	else if( mPos.x < -dims.x ) mIsDead = true;
	
	if( mPos.y > dims.y )		mIsDead = true;
	else if( mPos.y < -dims.y ) mIsDead = true;
	
	if( mPos.z > dims.z )		mIsDead = true;
	else if( mPos.z < -dims.z ) mIsDead = true;
	
	mAge += dt;
	mAgePer = 1.0f - mAge/mLifespan;
	if( mAge > mLifespan ){
		mIsDead = true;
	}
}
