//
//  Controller.h
//  Flocking
//
//  Created by Robert Hodgin on 4/26/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#pragma once
#include "cinder/gl/GlslProg.h"
#include "cinder/gl/Fbo.h"
#include "Room.h"
#include "Lantern.h"
#include "Glow.h"
#include "Nebula.h"
#include <vector>

class Controller {
public:
	Controller();
	Controller( Room *room, int maxLanterns );
	void update();
	void updatePredatorBodies( ci::gl::Fbo *fbo );
	void drawLanterns( ci::gl::GlslProg *shader );
	void drawLanternGlows( const ci::vec3 &right, const ci::vec3 &up );
	void drawGlows( ci::gl::GlslProg *shader, const ci::vec3 &right, const ci::vec3 &up );
	void drawNebulas( ci::gl::GlslProg *shader, const ci::vec3 &right, const ci::vec3 &up );
	void addLantern( const ci::vec3 &pos );
	void addGlows( Lantern *lantern, int amt );
	void addNebulas( Lantern *lantern, int amt );
	Room					*mRoom;

	int						mNumLanterns;
	int						mMaxLanterns;
	std::vector<Lantern>	mLanterns;
	std::vector<Glow>		mGlows;
	std::vector<Nebula>		mNebulas;
};

bool depthSortFunc( Lantern a, Lantern b );


