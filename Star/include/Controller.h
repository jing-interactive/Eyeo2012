#pragma once
#include "cinder/gl/Gl.h"
#include "cinder/gl/GlslProg.h"
#include "cinder/gl/Vbo.h"
#include "Star.h"
#include "Glow.h"
#include "Nebula.h"

#include <vector>

class Controller {
  public:
	struct DustVertex {
        ci::vec3 vertex;
        ci::Vec4f color;
    };
	
	struct Dust {
		Dust( const ci::vec3 &pos, const ci::vec3 &vel )
			: mPos( pos ), mVel( vel )
		{
			mPosInit		= mPos;
			mLifespanInit	= ci::Rand::randFloat( 5.0f, 12.0f );
			mLifespan		= mLifespanInit;
			mInvLifespan	= 0.1f;
			mAge			= 0.0f;
			mCol			= ci::ColorA( 1.0f, 1.0f, 1.0f, 1.0f );
		}
		
		void update( float dt ){
			mPos	+= mVel * dt;
			mAge	+= dt;
			
			if( mAge > mLifespan ){
				mLifespan += 10.0f;
				mPos = mPosInit;
			}
			mAgePer = 1.0 - ( mAge - mLifespan ) * mInvLifespan;
			
			mCol	= ci::ColorA( 1.0f, 1.0f, 1.0f, mAgePer * 0.15f );
		}
		
		ci::vec3 mPosInit, mPos, mVel;
		ci::ColorA mCol;
		float mAge, mLifespanInit, mLifespan, mInvLifespan, mAgePer;
		bool mIsDead;
	};
	
	Controller();
	void update( float dt );
	void updateDusts( float dt );
	void drawGlows( ci::gl::GlslProg *shader, const ci::vec3 &right, const ci::vec3 &up );
	void drawNebulas( ci::gl::GlslProg *shader, const ci::vec3 &right, const ci::vec3 &up );
	void drawDusts();
	
	void addGlows( const Star &star, float power, int amt );
	void addNebulas( const Star &star, int amt );
	void addDusts( const Star &star, int amt );
	
	// CANIS MAJORIS
	void addCMNebulas( const ci::vec3 &starPos, float starRadius, float radiusMulti, int amt );
	void addCMGlows( const ci::vec3 &starPos, float starRadius, int amt );
	
	std::vector<Glow>	mGlows;
	std::vector<Nebula>	mNebulas;
	std::vector<Dust>	mDusts;
	int					mTotalDustVerts;
    int					mPrevTotalDustVerts;
    DustVertex			*mDustVerts;
};