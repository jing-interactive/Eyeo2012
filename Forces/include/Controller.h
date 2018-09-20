//
//  Controller.h
//  BubbleChamber
//
//  Created by Robert Hodgin on 5/3/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#pragma once
#include "cinder/Camera.h"
#include "cinder/gl/GlslProg.h"
#include "cinder/TriMesh.h"
#include "Room.h"
#include "Particle.h"
#include "FieldLine.h"
#include "Glow.h"
#include "Glob.h"
#include "Nebula.h"
#include <vector>
#include <list>

class Controller 
{
  public:
	struct VboVertex {
        ci::vec3 vertex;
//		ci::vec3 normal;
        ci::Vec4f color;
    };
	
	Controller();
	void init( Room *room, int maxParticles );
	void initParticles();
	void applyForce();
	void update( const ci::Camera &cam, float dt, bool tick );
	void updateFieldLines( float dt, bool tick );
	void draw();
	void drawFieldLines( ci::gl::GlslProg *shader );
	void drawGlows( const ci::vec3 &right, const ci::vec3 &up );
	void drawNebulas( const ci::vec3 &right, const ci::vec3 &up );
	void drawCoronas( const ci::vec3 &right, const ci::vec3 &up );
	void drawGlobs( const ci::vec3 &right, const ci::vec3 &up );
	void drawShards( ci::gl::GlslProg *shader );
	void clear();
	void addParticle( const ci::vec3 &pos, float charge );
	void addFieldLines( Particle *p, int amt );
	void addGlows( Particle *p, int amt );
	void addNebulas( Particle *p, int amt );
	void addGlobs( const ci::vec3 &pos, int amt );
	void checkForParticleTouch( const ci::vec2 &mousePos );
	void releaseDraggedParticles();
	void dragParticle( const ci::vec2 &mousePos );
	
	Room					*mRoom;
	int						mMaxParticles;
	
	std::vector<Particle>	mParticles;
	Particle				*mDraggedParticle;
	
	std::vector<FieldLine>	mFieldLines;
	int						mTotalVerts;
    int						mPrevTotalVerts;
    VboVertex				*mFieldLineVerts;
	
	std::vector<Glow>		mGlows;
	std::vector<Nebula>		mNebulas;
	std::vector<Glob>		mGlobs;
};

