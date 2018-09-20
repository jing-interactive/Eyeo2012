//
//  SpringCam.cpp
//  Matter
//
//  Created by Robert Hodgin on 3/28/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#include "SpringCam.h"

using namespace ci;

SpringCam::SpringCam()
{
}

SpringCam::SpringCam( float camDist, float aspectRatio )
{
	mCamDist		= camDist;
	
	mEye			= vec3( 0.0f, 0.0f, mCamDist );
	mCenter			= vec3( 0.0f, 0.0f, 0.0f );
	mUp				= {0, 1, 0};
	
	mEyeNode		= SpringNode( mEye );
	mCenNode		= SpringNode( mCenter );
	mUpNode			= SpringNode( mUp );
	
	mFov			= 65.0f;
	
	mCam.setPerspective( mFov, aspectRatio, 1.0f, 3000.0f );
}

void SpringCam::update( float power, float dt )
{	
	if( power > 0.5f ){
		mFov -= ( mFov - 30.0f ) * 0.025f;
	} else {
		mFov -= ( mFov - 60.0f ) * 0.025f;		
	}
	mCam.setFov( mFov );
	
	mEyeNode.update( dt );
	mCenNode.update( dt );
	
	mCam.lookAt( mEyeNode.mPos, mCenNode.mPos, mUpNode.mPos );
	mMvpMatrix = mCam.getProjectionMatrix() * mCam.getViewMatrix();
}

void SpringCam::dragCam( const vec2 &posOffset, float distFromCenter )
{
	mEyeNode.mAcc += vec3( posOffset.xy(), distFromCenter );
}

void SpringCam::setEye( const ci::vec3 &eye )
{
	mEyeNode.setPos( eye );
}

void SpringCam::resetEye()
{
	mEyeNode.setPos( vec3( 0.0f, 0.0f, mCamDist ) );
}

void SpringCam::setPreset( int i )
{
	if( i == 0 ){
		mEyeNode.setPos( vec3( 0.0f, 0.0f, mCamDist ) );
		mCenNode.setPos( vec3( 0.0f, -100.0f, 0.0f ) );
	} else if( i == 1 ){
		mEyeNode.setPos( vec3( mCamDist * 0.4f, -175.0f, -100.0f ) );
		mCenNode.setPos( vec3( 0.0f, -190.0f, 0.0f ) );
	} else if( i == 2 ){
		mEyeNode.setPos( vec3( -174.0f, -97.8f, -20.0f ) );
		mCenNode.setPos( vec3( 0.0f, -190.0f, 0.0f ) );
	}
}