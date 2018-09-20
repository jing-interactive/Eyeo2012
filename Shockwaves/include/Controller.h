//
//  Controller.h
//  BubbleChamber
//
//  Created by Robert Hodgin on 5/3/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#pragma once
#include "cinder/gl/GlslProg.h"
#include "cinder/gl/Vbo.h"
#include "Room.h"
#include "Node.h"
#include "Shockwave.h"
#include "Smoke.h"
#include "Glow.h"
#include <vector>
#include <list>

class Controller 
{
public:
	Controller();
	void init( Room *room, int gridDim );
	void createNodes( int gridDim );
	void createSphere( ci::gl::VboMesh &mesh, int res );
	void drawSphereTri( ci::vec3 va, ci::vec3 vb, ci::vec3 vc, int div );
	void update( float dt, bool tick );
	void explode();
	void draw();
	void drawNodes( ci::gl::GlslProg *shader );
	void drawShockwaves( ci::gl::GlslProg *shader );
	void drawShockwaveCenters();
	void drawConnections();
	void drawSmokes( const ci::vec3 &right, const ci::vec3 &up );
	void drawGlows( const ci::vec3 &right, const ci::vec3 &up );
	void addSmokes( const ci::vec3 &vec, int amt );
	void addGlows( const ci::vec3 &vec, int amt );
	void clear();
	void preset( int index );
	int mPresetIndex;
	
	int	mGridDim;
	
	Room					*mRoom;
	
	std::vector<Node>		mNodes;
	std::vector<Node>		mNewNodes;
	std::vector<Shockwave>	mShockwaves;
	std::vector<Smoke>		mSmokes;
	std::vector<Glow>		mGlows;
	
	ci::gl::VboMesh			mSphereVbo;
	std::vector<ci::vec3>	mPosCoords;
	std::vector<ci::vec3>	mNormals;
};
bool depthSortFunc( Shockwave a, Shockwave b );
