#pragma once
#include "cinder/Vector.h"
#include "cinder/Color.h"
#include <vector>

class Photon {
public:
	Photon();
	Photon( ci::vec3 pos, ci::vec3 vel );
	void update( float t );
	void draw();
	void drawTail();
	
	int			mLen;
	std::vector<ci::vec3> mPs;
	
	ci::vec3	mPos;
	ci::vec3	mVel;
	
	float		mRadius;
	
	float		mAge, mAgePer;
	float		mLifespan;
	bool		mIsDead;
};