#pragma once
#include "cinder/gl/Gl.h"
#include "cinder/gl/GlslProg.h"
#include "cinder/gl/Vbo.h"
#include "Particle.h"
#include "Photon.h"
#include "Shockwave.h"
#include "Smoke.h"
#include "SpringCam.h"
#include <list>
#include <vector>

class Controller {
public:
	struct AnchorVertex {
        ci::vec3 vertex;
        ci::vec4 color;
    };
	
	Controller();
	void init( Room *room );
	void createSphere( ci::gl::VboMesh &mesh, int res );
	void drawSphereTri( ci::vec3 va, ci::vec3 vb, ci::vec3 vc, int div );
	void applyShockwavesToCam( SpringCam &cam );
	void applyShockwavesToTime();
	void applyForceToParticles();
	bool didParticlesCollide( const ci::vec3 &dir, const ci::vec3 &dirNormal, const float dist, const float sumRadii, const float sumRadiiSqrd, ci::vec3 *moveVec );
	void annihilate( Particle *p1, Particle *p2 );
	void update();
	void reset();
	void clearRoom();
	void draw( ci::gl::GlslProg *shader );
	void drawParticleShadows();
	void drawPhotons();
	void drawShockwaves( const ci::vec3 &camPos );
	void drawSmokes( const ci::vec3 &right, const ci::vec3 &up );
	void drawSphericalBillboard( const ci::vec3 &camEye, const ci::vec3 &objPos, const ci::vec2 &scale, float rotInRadians );
	void finish();
	void addParticles( int amt );
	void addParticle( float charge, const ci::vec3 &pos );
	void removeParticles( int amt );
	void preset( int i );

	Room					*mRoom;
	
	int						mNumMatter;
	int						mNumAntimatter;
	
	std::list<Particle>		mParticles;
	std::list<Photon>		mPhotons;
	std::vector<Shockwave>	mShockwaves;
	std::vector<Smoke>		mSmokes;
	
	ci::gl::VboMesh			mSphereLo, mSphereHi;
	int						mIndex;
	std::vector<uint32_t>	mIndices;
	std::vector<ci::vec3>	mPosCoords;
	std::vector<ci::vec3>	mNormals;
	std::vector<ci::vec2>	mTexCoords;
	
	int mPreset;
	bool mRecycle;
};