//
//  SpringCam.cpp
//  PROTOTYPE

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
	
	mCam.setPerspective( 65.0f, aspectRatio, 5.0f, 3000.0f );
}

void SpringCam::update( float dt )
{	
	mEyeNode.update( dt );
	mCenNode.update( dt );
	
	mCam.lookAt( mEyeNode.mPos, mCenNode.mPos, mUpNode.mPos );
	mMvpMatrix = mCam.getProjectionMatrix() * mCam.getViewMatrix();
}

void SpringCam::dragCam( const vec2 &posOffset, float distFromCenter )
{
    mEyeNode.mAcc += vec3(posOffset, distFromCenter);
}

void SpringCam::setEye( const ci::vec3 &eye )
{
	mEyeNode.setPos( eye );
}

void SpringCam::resetEye()
{
	mEyeNode.setPos( vec3( 0.0f, 0.0f, mCamDist ) );
}