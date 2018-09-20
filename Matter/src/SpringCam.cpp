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
	mFov			= 65.0f;
	mFovDest		= 65.0f;
	
	mCamDist		= camDist;
	
	mEye			= vec3( 0.0f, 0.0f, mCamDist );
	mCenter			= vec3( 0.0f, 0.0f, 0.0f );
	mUp				= {0, 1, 0};
	
	mEyeNode		= SpringNode( mEye );
	mCenNode		= SpringNode( mCenter );
	mUpNode			= SpringNode( mUp );
	
	mCam.setPerspective( 65.0f, aspectRatio, 5.0f, 3000.0f );
}

void SpringCam::update( float timeDelta )
{	
	mFov -= ( mFov - mFovDest ) * 0.05f;
	mCam.setFov( mFov );
	
	mEyeNode.apply();
	mCenNode.apply();
	
	mEyeNode.update( timeDelta );
	mCenNode.update( timeDelta );
	
	mCam.lookAt( mEyeNode.mPos, mCenNode.mPos, mUpNode.mPos );
	mMvpMatrix		= mCam.getProjectionMatrix() * mCam.getViewMatrix();
}

void SpringCam::dragCam( const vec2 &posOffset, float distFromCenter )
{
	mEyeNode.mAcc += vec3( posOffset.xy(), distFromCenter );
//	mCenNode.mAcc -= vec3( posOffset.xy(), distFromCenter ) * 2.0f;
}