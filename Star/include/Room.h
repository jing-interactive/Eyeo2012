//
//  Room.h
//  Stillness
//
//  Created by Robert Hodgin on 01/01/12.


#pragma once
#include "cinder/gl/Vbo.h"

class Room
{
public:
	Room();
	Room( const ci::vec3 &dims, bool isPowerOn, bool isGravityOn );
	void		init();
	void		updateTime();
	void		update();
	void		draw();
	
	void		setDims( const ci::vec3 &dims ){ mDims = dims; };
	ci::vec3	getDims(){ return mDims; };
	ci::vec3	getRandCeilingPos();
	ci::vec3	getCornerCeilingPos();
	ci::vec3	getCornerFloorPos();
	float		getFloorLevel();
	
	void		adjustTimeMulti( float amt );
	float		getTimePer();
	float		getTimeDelta();
	bool		getTick();
	
	void		togglePower(){		mIsPowerOn = !mIsPowerOn;		};
	float		getPower(){			return mPower;					};
	bool		isPowerOn(){		return mIsPowerOn;				};
	
	float		getLightPower();
	
	void		toggleGravity(){	mIsGravityOn = !mIsGravityOn;	};
	ci::vec3	getGravity(){		return mGravity;				};
	bool		isGravityOn(){		return mIsGravityOn;			};
	
	ci::gl::VboMesh mVbo;
	
	// TIME
	float			mTime;				// Time elapsed in real world seconds
	float			mTimeElapsed;		// Time elapsed in simulation seconds
	float			mTimeMulti;			// Speed at which time passes
	float			mTimeAdjusted;		// Amount of time passed between last frame and current frame
	float			mTimer;				// A resetting counter for determining if a Tick has occured
	bool			mTick;				// Tick (aka step) for triggering discrete events
	
	// DIMENSIONS
	ci::vec3		mDims;				// Hesitant to rename this to 'bounds'. Might make it too easy to
	ci::vec3		mDimsDest;			// confuse with mRoomFbo->getBounds() which would return the Fbo Area.
	
	// POWER
	bool			mIsPowerOn;			// Power ranges from 0.0 to 1.0.
	float			mPower;
	
	// GRAVITY
	ci::vec3		mGravity;			// Gravity vector
	ci::vec3		mDefaultGravity;	// The default Gravity vector, in case you want to mess with gravity
	// but always be able to get back to the original condition.
	bool			mIsGravityOn;		// For turning Gravity on and off
};