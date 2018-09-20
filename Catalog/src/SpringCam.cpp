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
	
	mCam.setPerspective( 65.0f, aspectRatio, 5.0f, 200000.0f );
}

void SpringCam::update( float timeDelta )
{	
	mEyeNode.apply();
	mCenNode.apply();
	
	mEyeNode.update( timeDelta );
	mCenNode.update( timeDelta );
	
	vec3 dir		= mCenNode.mPos - mEyeNode.mPos;
	vec3 dirNorm	= dir.normalized();
//	vec3 up		= dirNorm.cross( vec3::xAxis() );
//	up.normalize();
//	vec3 temp		= dirNorm.cross( up );
//	temp.normalize();
//	up = dirNorm.cross( temp );
//	up.normalize();
	
	vec3 newEyePos = mEyeNode.mPos + dirNorm * 10.0f;

	mCam.lookAt( newEyePos, mCenNode.mPos, mUpNode.mPos );
	mMvpMatrix = mCam.getProjectionMatrix() * mCam.getViewMatrix();
}

void SpringCam::dragCam( const vec2 &posOffset, float distFromCenter )
{
	mEyeNode.mAcc += vec3( posOffset.xy(), distFromCenter );
}

void SpringCam::setEye( const vec3 &v )
{
	mEyeNode.mRestPos = v;
}

void SpringCam::setCenter( const vec3 &v )
{
	mCenNode.mRestPos = v;
}