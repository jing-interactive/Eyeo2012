#pragma once
#include "cinder/Vector.h"
#include "cinder/Color.h"
#include "Room.h"
#include <vector>

class Particle {
public:
	Particle();
	Particle( const ci::vec3 &pos, const ci::vec3 &vel );
	Particle( const ci::vec3 &pos, const ci::vec3 &vel, float charge );
	void init( const ci::vec3 &pos, const ci::vec3 &vel, float charge );
	float getMassFromRadius( float r );
	void update( Room *room, float t );
	void draw( ci::gl::VboMesh &sphereLo, ci::gl::VboMesh &sphereHi );
	void finish();

	ci::vec3	mPos;
	ci::vec3	mVel;
	ci::vec3	mAcc;
	
	float		mDistPer;
	float		mDistToClosestNeighbor;
	
	float		mFusionThresh;
	
	ci::Color	mCol;
	
	float		mCharge;
	float		mMass, mInvMass;
	float		mDecay;
	float		mRadius, mRadiusInit, mRadiusDest;
	
	bool		mIsDead;
};