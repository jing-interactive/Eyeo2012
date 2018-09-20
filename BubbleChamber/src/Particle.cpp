//
//  Particle.cpp
//  BubbleChamber
//
//  Created by Robert Hodgin on 5/3/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#include "Particle.h"
#include "Controller.h"
#include "cinder/gl/gl.h"
#include "cinder/Rand.h"

using namespace ci;
using std::vector;

Particle::Particle()
{
}

Particle::Particle( Controller *controller, int gen, const vec3 &pos, const vec3 &dir, float speed, int len )
	: mController( controller ), mGen( gen ), mPos( pos ), mSpeed( speed ), mLen( len )
{
	mVel				= dir * mSpeed;
	mPosLastTick		= mPos;
	
	for( int i=0; i<mLen; i++ ){
		mPs.push_back( mPos );
	}
	
	mLifespan			= 150.0f;
	
	mRay				= Ray( mPos, mVel.normalized() );
	
	mAxis				= dir.cross( Rand::randvec3() );
	mAxis.normalize();
	mPerp1				= mAxis.cross( {0, 1, 0} );
	mPerp1.normalize();
	mPerp2				= mAxis.cross( mPerp1 );
	mPerp2.normalize();

	mCharge				= Rand::randFloat( -1.0f, 1.0f );
	mAngle				= 0.0f;
	mAngleDelta			= Rand::randPosNegFloat( 0.0035f, 0.007f );
	mAngleDeltaDelta	= Rand::randPosNegFloat( 0.03f, 0.05f );
	
	mAge				= 0.0f;
	mAgePer				= 1.0f;
	
	mBounced			= false;
	mIsDead				= false;
	mIsDying			= false;
	mDyingCount			= 0.0f;
	
//	vec3 origin		= vec3( 0.0f, -room->getDimensions().y, 0.0f );
//	vec3 normal		= vec3( 0.0f, 1.0f, 0.0f );
//	float distance;
//	mIntersectsFloor	= mRay.calcPlaneIntersection( origin, normal, &distance );
//	mDeathPos			= mRay.getOrigin() + mRay.getDirection() * distance;
}

void Particle::update( Room *room, float dt, bool tick )
{
	if( !mIsDying ){
		if( mGen > 0 ){
			mVel		-= mVel * 0.025f * dt;
			mAngleDelta += mAngleDeltaDelta * dt;
			mAngle		+= mCharge * mAngleDelta * dt;
			vec3 dir	 = ( cos( mAngle ) * mPerp1 + sin( mAngle ) * mPerp2 );
			mVel		+= dir * dt;
		}
		
		mPos   += mVel * dt;
	}
	
	if( tick ){
		for( int i=mLen-1; i>0; i-- ){
			mPs[i] = mPs[i-1];
		}
	}
	mPs[0] = mPos;
	
	mRay.setOrigin( mPos );
	mRay.setDirection( mVel.normalized() );
	
	if( !mIsDead )
		mBounced = isOutOfBounds( room );
	
	mAge += dt;
	mAgePer = 1.0f - mAge/mLifespan;
	if( mAge > mLifespan ){
		mIsDead = true;
	}
	
	if( mIsDying ){
		mDyingCount += dt;
		if( mDyingCount > 15.0f ){
			mIsDead = true;
		}
	}
}

bool Particle::isOutOfBounds( Room *room )
{	
	bool b		= false;
	vec3 dim	= room->getDims();
	if( mPos.x < -dim.x || mPos.x > dim.x ){
		b = true;
		
		vec3 origin;
		vec3 normal;
		if( mPos.x < 0.0f ){
			origin = vec3(-dim.x, 0.0f, 0.0f );
			normal = vec3( 1.0f, 0.0f, 0.0f );
		} else {
			origin = vec3( dim.x, 0.0f, 0.0f );
			normal = vec3(-1.0f, 0.0f, 0.0f );
		}
		float distance = 0.0f;
		bool intersects = mRay.calcPlaneIntersection( origin, normal, &distance );
		if( intersects ) mDeathPos	= mRay.getOrigin() + mRay.getDirection() * distance;
		mPos		= mDeathPos;
		mDeathAxis	= normal;
		mPs[0]		= mPos;
		
	} else if( mPos.y < -dim.y || mPos.y > dim.y ){
		b = true;
		
		vec3 origin;
		vec3 normal;
		if( mPos.y < 0.0f ){
			origin = vec3( 0.0f,-dim.y, 0.0f );
			normal = vec3( 0.0f, 1.0f, 0.0f );
		} else {
			origin = vec3( 0.0f, dim.y, 0.0f );
			normal = vec3( 0.0f,-1.0f, 0.0f );
		}
		float distance = 0.0f;
		bool intersects = mRay.calcPlaneIntersection( origin, normal, &distance );
		if( intersects ) mDeathPos	= mRay.getOrigin() + mRay.getDirection() * distance;
		mPos		= mDeathPos;
		mDeathAxis	= normal;
		mPs[0]		= mPos;
		
	} else if( mPos.z < -dim.z || mPos.z > dim.z ){
		b = true;
		
		vec3 origin;
		vec3 normal;
		if( mPos.z < 0.0f ){
			origin = vec3( 0.0f, 0.0f,-dim.z );
			normal = vec3( 0.0f, 0.0f, 1.0f );
		} else {
			origin = vec3( 0.0f, 0.0f, dim.z );
			normal = vec3( 0.0f, 0.0f,-1.0f );
		}
		float distance = 0.0f;
		bool intersects = mRay.calcPlaneIntersection( origin, normal, &distance );
		if( intersects ) mDeathPos	= mRay.getOrigin() + mRay.getDirection() * distance;
		mPos		= mDeathPos;
		mDeathAxis	= normal;
		mPs[0] = mPos;
	}
	
	return b;
}


