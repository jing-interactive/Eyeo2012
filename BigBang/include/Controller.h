//
//  Controller.h
//  BubbleChamber
//
//  Created by Robert Hodgin on 5/3/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#pragma once
#include "cinder/gl/Gl.h"
#include "cinder/gl/GlslProg.h"
#include "cinder/gl/Vbo.h"
#include "cinder/Camera.h"
#include "cinder/Perlin.h"
#include "Room.h"
#include "Confetti.h"
#include "Streamer.h"
#include "Balloon.h"
#include "Shockwave.h"
#include <vector>
#include <list>

class Controller 
{
  public:
	Controller();
	void init( Room *room );
	void modVert( ci::vec3 *v );
	void createSphere( ci::gl::VboMesh &mesh, int res );
	void drawSphereTri( ci::vec3 va, ci::vec3 vb, ci::vec3 vc, int div );
	void bang();
	void checkForBalloonPop( const ci::vec2 &mousePos );
	void update( const ci::Camera &cam );
	void applyBalloonCollisions();
	bool didParticlesCollide( const ci::vec3 &dir, const ci::vec3 &dirNormal, const float dist, const float sumRadii, const float sumRadiiSqrd, ci::vec3 *moveVec );
	void draw();
	void drawConfettis( ci::gl::GlslProg *shader );
	void drawStreamers();
	void drawBalloons( ci::gl::GlslProg *shader );
	void drawPhysics();
	void addConfettis( int amt, const ci::vec3 &pos, float speedMulti );
	void addStreamers( int amt, const ci::vec3 &pos );
	void addBalloons( int amt, const ci::vec3 &pos );
	void clear();
	void preset( int index );
	int						mPresetIndex;
	
	Room					*mRoom;
	ci::Perlin				mPerlin;
	
	float					mTimeSinceBang;
	
	std::vector<Confetti>	mConfettis;
	std::vector<Streamer>	mStreamers;
	std::vector<Balloon>	mBalloons;
	std::vector<Shockwave>	mShockwaves;
	
	ci::gl::VboMesh			mBalloonsVbo;
	std::vector<ci::vec3>	mPosCoords;
	std::vector<ci::vec3>	mNormals;
};
bool depthSortFunc( Balloon a, Balloon b );

