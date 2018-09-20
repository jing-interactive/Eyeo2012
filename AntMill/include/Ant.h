//
//  Ant.h
//  AntMill
//
//  Created by Robert Hodgin on 5/8/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#pragma once
#include <vector>
#include "Food.h"

class Controller;
class Food;

class Ant 
{
  public:
	Ant();
	Ant( Controller *controller, const ci::vec3 &pos );
	void update( float dt, const ci::vec3 &roomDims );
	void updateSpring( float dt );
	void applySpeedLimit();
	void applyJitter();
	void updateSensors();
	void updateParticles();
	void checkForPause();
	void checkForHome();
	void foundFood( Food *food );
	void stayInBounds( ci::vec3 *v, const ci::vec3 &roomDims );
	void findAntsInZone();
	void followAntsInZone();
	void draw();
	void turn( float amt );
	

	// PIN POINTS
	ci::vec3			mPinPerp;
	ci::vec3			mPinUp;
	
	ci::vec3			mAnchorPos;
	ci::vec3			mSpringPos;
	ci::vec3			mSpringVel;
	ci::vec3			mSpringAcc;
	
	Controller			*mController;
	static uint32_t		sNextUniqueId;
	uint32_t			mId;
	ci::vec3			mPos;
	ci::vec3			mVel;
	ci::vec3			mAcc;
	
	float				mAngle;
	ci::vec2			mOrientation;
	
	ci::vec3			mDir, mDirPerp;
	ci::vec3			mLeftSensorPos, mRightSensorPos;
	ci::vec3			mTailPos;
	
	Food				*mGrabbedFood;
	
	std::vector<Ant*>	mVisibleAnts;
	
	float				mLength;
	float				mRadius;
	float				mColor;
	float				mMaxSpeed;
	
	bool				mHasFood;
	bool				mIsHome;
	
	bool				mIsSpecial;
	float				mAge;
	bool				mIsInjured;
	bool				mIsDead;
	
	static float		sCenterOffsetMulti;
	static float		sCenterRadiusMulti;
};

