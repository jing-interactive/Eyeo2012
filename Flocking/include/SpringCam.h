//
//  SpringCam.h
//  Matter
//
//  Created by Robert Hodgin on 3/28/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#pragma once

#define SPRING_STRENGTH 0.02
#define SPRING_DAMPING 0.05
#define REST_LENGTH 0.0

#include "cinder/Camera.h"
#include "cinder/Vector.h"

class SpringCam {
  public:
	struct SpringNode {
		SpringNode() {}
		SpringNode( ci::vec3 pos ){
			mRestPos	= pos;
			mPos		= pos;
			mVel		= {};
			mAcc		= {};
		}
		
		void apply(){
			ci::vec3 dir	= mPos - mRestPos;
			float dist		= dir.length();
			dir.safeNormalize();
			float springForce	= -( dist - (float)REST_LENGTH ) * (float)SPRING_STRENGTH;
			float dampingForce	= -(float)SPRING_DAMPING * ( dir.x*mVel.x + dir.y*mVel.y + dir.z*mVel.z );
			float r				= springForce + dampingForce;
			dir *= r;
			mAcc += dir;
		}
		
		void update( float timeDelta ){
			mVel += mAcc * timeDelta;
			mPos += mVel * timeDelta;
			mVel -= mVel * 0.04 * timeDelta;
			mAcc = {};
		}

		ci::vec3 mRestPos;
		ci::vec3 mPos, mVel, mAcc;
	};
	
	
	
	SpringCam();
	SpringCam( float camDist, float aspectRatio );
	void apply( float timeDelta );
	void update( float timeDelta );
	void dragCam( const ci::vec2 &posOffset, float distFromCenter );
	void draw();
	ci::CameraPersp getCam(){ return mCam; }
	
	ci::CameraPersp		mCam;
	float				mCamDist;
	ci::vec3			mEye, mCenter, mUp;
	
	ci::vec3			mBillboardRight;
	ci::vec3			mBillboardUp;
	
	ci::mat4		mMvpMatrix;
	
	SpringNode			mEyeNode;
	SpringNode			mCenNode;
	SpringNode			mUpNode;
};