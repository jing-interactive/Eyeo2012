//
//  GlowCube.cpp
//  BubbleChamber
//
//  Created by Robert Hodgin on 5/7/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#include "cinder/app/App.h"
#include "cinder/gl/gl.h"
#include "cinder/Rand.h"
#include "GlowCube.h"

using namespace ci;

GlowCube::GlowCube()
{
}

GlowCube::GlowCube( const vec3 &pos )
: mPos( pos )
{
	mAcc		= vec3();
	mVel		= vec3();
	mAxis		= Rand::randvec3();
	mAngle		= Rand::randFloat( M_PI * 2.0f );
	mAngleVel	= Rand::randFloat( -0.1f, 0.1f );
	mIsDead		= false;
	
	mAge		= Rand::randFloat( 6.0f );
	mAgePer		= 1.0f;
	mLifespan	= Rand::randFloat( 50.0f, 150.0f );
	mRadius		= Rand::randFloat( 4.0f, 10.0f );
}

void GlowCube::update( float dt )
{
//	mVel += mAcc * dt;
//	mPos += mVel * dt;
//	mVel -= mVel * 0.01f * dt;
//	mAcc = vec3();
	
	mAngle += dt * mAngleVel;
	
	mMatrix.setToIdentity();
	mMatrix.translate( mPos );
	mMatrix.scale( vec3( mRadius, mRadius, mRadius ) );
	mMatrix.rotate( mAxis * mAngle );
	
	mAge += dt;
	mAgePer = 1.0f - mAge/mLifespan;
	if( mAge > mLifespan ){
		mIsDead = true;
	}
}

void GlowCube::draw()
{
	gl::drawCube( vec3(), vec3( 1.0f, 1.0f, 1.0f ) * mAgePer );
}