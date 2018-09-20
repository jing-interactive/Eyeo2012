//
//  SpringCam.h
//  PROTOTYPE

#pragma once

#define SPRING_STRENGTH 0.02
#define SPRING_DAMPING 0.25
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
		
		void update( float dt ){
			ci::vec3 dir		= mPos - mRestPos;
			float dist			= dir.length();
			glm::normalize(dir);
			float springForce	= -( dist - REST_LENGTH ) * SPRING_STRENGTH;
			float dampingForce	= -SPRING_DAMPING * ( dir.x*mVel.x + dir.y*mVel.y + dir.z*mVel.z );
			float r				= springForce + dampingForce;
			dir *= r;
			mAcc += dir;
			
			mVel += mAcc * dt;
			mPos += mVel * dt;
			mVel -= mVel * 0.04f * dt;
			mAcc = {};
		}
		
		void setPos( const ci::vec3 &v ){ mRestPos = v; }

		ci::vec3 mRestPos;
		ci::vec3 mPos, mVel, mAcc;
	};
	
	
	
	SpringCam();
	SpringCam( float camDist, float aspectRatio );
	void update( float dt );
	void dragCam( const ci::vec2 &posOffset, float distFromCenter );
	void draw();
	ci::CameraPersp getCam(){ return mCam; }
	ci::vec3 getEye(){ return mCam.getEyePoint(); }
	void setEye( const ci::vec3 &eye );
	void resetEye();
	
	ci::CameraPersp		mCam;
	float				mCamDist;
	ci::vec3			mEye, mCenter, mUp;
	
	ci::mat4		mMvpMatrix;
	
	SpringNode			mEyeNode;
	SpringNode			mCenNode;
	SpringNode			mUpNode;
};