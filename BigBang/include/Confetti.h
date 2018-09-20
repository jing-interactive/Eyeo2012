//
//  Confetti.h
//  BigBang
//
//  Created by Robert Hodgin on 5/15/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#pragma once


class Confetti {
  public:
	Confetti();
	Confetti( const ci::vec3 &pos, float speedMulti, int presetIndex );
	void update( const ci::vec3 &roomDims, float dt );
	void checkBounds( const ci::vec3 &roomDims );
	void draw();
	
	ci::vec3		mPos;
	ci::vec3		mVel;
	ci::vec3		mDriftVel;
	ci::vec3		mAcc;
	
	float			mRadius;
	ci::Color		mColor;
	float			mXRot, mYRot, mZRot;
	float			mGravity;
	
	ci::mat4	mMatrix;
	
	float			mAge;
	float			mAgePer;
	float			mLifespan;
	bool			mIsDead;
	bool			mHasLanded;
};