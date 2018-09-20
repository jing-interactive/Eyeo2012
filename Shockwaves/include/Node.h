//
//  Confetti.h
//  BigBang
//
//  Created by Robert Hodgin on 5/15/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#pragma once


class Node {
  public:
	Node();
	Node( const ci::vec3 &pos, const ci::vec3 &vel );
	void update( float dt );
	void draw();
	
	ci::mat4	mMatrix;
	ci::vec3		mPosInit;
	ci::vec3		mPos;
	ci::vec3		mVel;
	ci::vec3		mAcc;
	
	float			mRadius;
	ci::Color		mColor;
	
	float			mAge;
};