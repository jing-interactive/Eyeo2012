//
//  Shard.h
//  Sol
//
//  Created by Robert Hodgin on 12/10/11.
//  Copyright (c) 2011 __MyCompanyName__. All rights reserved.
//

#pragma once
#include "cinder/Vector.h"
#include "cinder/Color.h"
#include "cinder/TriMesh.h"

#include <vector>

class Shard {
public:
	Shard( ci::vec3 pos, ci::vec3 dir, float height, float radius );
	void init( ci::TriMesh *mesh, const ci::Color &c );
	ci::vec3 calcSurfaceNormal( const ci::vec3 &p1, const ci::vec3 &p2, const ci::vec3 &p3 );
	void update();
	void draw();

	ci::vec3	mPos;
	ci::vec3	mDir;
	ci::vec3	mTip;	// tip of shard
	ci::vec3	mMid;	// where tapering facets begin

	std::vector<ci::vec3> mBasePs;
	std::vector<ci::vec3> mMidPs;
	std::vector<ci::vec3> mTipPs;

	float		mHeight;
	float		mRadius;
};
