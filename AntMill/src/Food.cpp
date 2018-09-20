//
//  Food.cpp
//  AntMill
//
//  Created by Robert Hodgin on 5/26/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#include "cinder/app/App.h"
#include "Food.h"

using namespace ci;

Food::Food()
{
}

Food::Food( const vec3 &pos, float charge )
	: mPos( pos ), mCharge( charge )
{
	mVel		= vec3();
	mGrabbed	= false;
	float c		= mCharge * 0.5f + 0.5f;
	mColor		= Color( c, c, c );
	mIsGone		= false;
}

void Food::update( float dt, const vec3 &roomDims )
{
	if( mIsGone ){
		mVel += vec3( 0.0f, -0.2f, 0.0f );
	}
	
	mPos += mVel * dt;
	mVel -= mVel * 0.04f * dt;
	
	stayInBounds( &mPos, roomDims );
}

void Food::draw()
{
	gl::drawSphere( mPos, 2.0f, 4 );
}

void Food::stayInBounds( vec3 *v, const vec3 &roomDims )
{
//	float boundsOffset = 1.0f;
	if( v->x < -roomDims.x ){
		v->x = -roomDims.x;
		mVel.x *= -0.5f;
	} else if( v->x > roomDims.x ){
		v->x = roomDims.x;
		mVel.x *= -0.5f;
	}
	
	if( v->y < -roomDims.y ){
		v->y = -roomDims.y;
		mVel.y *= -0.5f;
	} else if( v->y > roomDims.y ){
		v->y = roomDims.y;
		mVel.y *= -0.5f;
	}
	
	if( v->z < -roomDims.z ){
		v->z = -roomDims.z;
		mVel.z *= -0.5f;
	} else if( v->z > roomDims.z ){
		v->z = roomDims.z;
		mVel.z *= -0.5f;
	}
}