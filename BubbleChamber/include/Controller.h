//
//  Controller.h
//  BubbleChamber
//
//  Created by Robert Hodgin on 5/3/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#pragma once
#include "cinder/Perlin.h"
#include "cinder/gl/GlslProg.h"
#include "cinder/Rand.h"
#include "Room.h"
#include "Particle.h"
#include "Bubble.h"
#include "Smoke.h"
#include "Moth.h"
#include "Shockwave.h"
#include "GlowCube.h"
#include <vector>
#include <list>

class Controller 
{
  public:
	struct BubbleVertex {
        ci::vec3 vertex;
        ci::Vec4f color;
    };
	
	struct Gib {
		Gib() {}
		Gib( ci::vec3 pos, ci::vec3 vel, float radius ){
			mPos		= pos;
			mVel		= vel;
			mRadiusDest = radius;
			mRadius		= 10.0f;
			
			mAge		= 0.0f;
			mLifespan	= ci::Rand::randFloat( 10.0f, 100.0f );
			
			mIsDead		= false;
		}
		
		void update( const ci::vec3 &gravity, const ci::vec3 &roomDims, float dt ){
			mRadius -= ( mRadius - mRadiusDest ) * 0.2f * dt;
			mVel += gravity * dt;
			
			ci::vec3 tempPos = mPos + mVel * dt;

			if( tempPos.x - mRadius < -roomDims.x ){
				mIsDead = true;
				mDeathAxis = ci::vec3( -1.0f, 0.0f, 0.0f );
			} else if( tempPos.x + mRadius > roomDims.x ){
				mIsDead = true;
				mDeathAxis = ci::vec3( 1.0f, 0.0f, 0.0f );
			}
			
			if( tempPos.y - mRadius < -roomDims.y ){
				mIsDead = true;
				mDeathAxis = ci::vec3( 0.0f, -1.0f, 0.0f );
			} else if( tempPos.y + mRadius > roomDims.y ){
				mIsDead = true;
				mDeathAxis = ci::vec3( 0.0f, -1.0f, 0.0f );
			}
			
			if( tempPos.z - mRadius < -roomDims.z ){
				mIsDead = true;
				mDeathAxis = ci::vec3( 0.0f, 0.0f, -1.0f );
			} else if( tempPos.z + mRadius > roomDims.z ){
				mIsDead = true;
				mDeathAxis = ci::vec3( 0.0f, 0.0f, 1.0f );
			}
			
			mPos += mVel * dt;
			mVel -= mVel * 0.001f * dt;
			mAge += dt;
			mAgePer = 1.0f - mAge/mLifespan;
			if( mAge > mLifespan ) mIsDead = true;
		}
		
		void draw(){
			ci::gl::drawCube( mPos, ci::vec3( mRadius, mRadius, mRadius ) * mAgePer );
		}
		
		ci::vec3	mPos, mVel;
		ci::vec3	mDeathAxis;
		float		mRadius, mRadiusDest;
		float		mAge, mLifespan;
		float		mAgePer;
		bool		mIsDead;
	};
	
	struct Decal {
		Decal() {}
		Decal( ci::vec3 pos, ci::vec3 axis, float radius, float lifespan ){
			mPos	= pos;
			mAxis	= axis;
			mColor  = 0.0f;
			if( mAxis.x < -0.5f || mAxis.x > 0.5f ){
				mRight	= ci::vec3::zAxis();
				mUp		= ci::{0, 1, 0};
			} else if( mAxis.y < -0.5f || mAxis.y > 0.5f ){
				mRight	= ci::vec3::xAxis();
				mUp		= ci::vec3::zAxis();
			} else {
				mRight	= ci::vec3::xAxis();
				mUp		= ci::{0, 1, 0};
			}
			
			mRadiusDest	= radius;
			mRadius		= 0.0f;
			
			mAge		= 0.0f;
			mAgePer		= 0.0f;
			mLifespan	= lifespan;
			
			mIsDead		= false;
		}
		
		void update( float dt ){
			mAge	   += dt;
			mAgePer		= 1.0f - mAge/mLifespan;//sin( M_PI * ( mAge/mLifespan ) );
			
			mRadius		= mRadiusDest * mAgePer;
			
			if( mAge > mLifespan ) mIsDead = true;
		}
		
		void draw(){
			ci::gl::drawBillboard( mPos, ci::vec2( mRadius, mRadius ), 0.0f, mRight, mUp );
		}
		
		ci::vec3 mPos;
		ci::vec3 mAxis;
		ci::vec3 mRight, mUp;
		float mRadiusDest, mRadius;
		float mAge, mAgePer, mLifespan;
		float mColor;
		bool mIsDead;
	};
	
	Controller();
	void init( Room *room );
	void createShards( const ci::vec3 &spawnPos, const ci::vec3 &spawnDir, ci::TriMesh *shardMesh );
	void update();
	void updateParticles( float dt, bool tick );
	void updateBubbles( float dt );
	void updateDecals( float dt, bool tick );
	void updateSmokes( float dt );
	void updateMoths( float dt );
	void updateShockwaves( float dt );
	void updateGibs( float dt );
	void updateGlowCubes( float dt );
	bool checkMothCollisions( Particle *particle );
	void applyForcesToMoths();
	void explode( Particle *particle );
	void draw();
	void drawParticles( float power );
	void drawBubbles( float power );
	void drawDecals();
	void drawSmokes( const ci::vec3 &right, const ci::vec3 &up );
	void drawMoths();
	void drawGibs();
	void drawGlowCubes( ci::gl::GlslProg *shader );
	void addBank( const ci::vec3 &pos, const ci::vec3 &axis, float radius, float color );
	void addBubble( const ci::vec3 &pos, const ci::vec3 &vel, float age );
	void addParticle( int gen, const ci::vec3 &pos, const ci::vec3 &dir, float speed, int len );
	void addParticles( int amt, bool isShort );
	void addMoth( const ci::vec3 &pos );
	void releaseMoths();
	void preset( int i );
	void clearRoom();
	
	Room					*mRoom;
	
	std::list<Particle>		mParticles;
	std::list<Particle>		mNewParticles;
	
	std::vector<Bubble>		mBubbles;
	std::vector<Decal>		mDecals;
	std::vector<Smoke>		mSmokes;
	std::vector<Moth>		mMoths;
	std::vector<Moth*>		mExplodingMoths;
	std::vector<Shockwave>	mShockwaves;
	std::vector<GlowCube>	mGlowCubes;
	std::vector<Gib>		mGibs;

	float					mIntensity;
	
	int						mTotalBubbleVerts;
    int						mPrevTotalBubbleVerts;
    BubbleVertex			*mBubbleVerts;
};


