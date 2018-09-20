#pragma once
#include "cinder/gl/Gl.h"
#include "cinder/gl/GlslProg.h"
#include "cinder/gl/Vbo.h"
#include "cinder/TriMesh.h"
#include "cinder/Rand.h"

#include "Room.h"
#include "SpringCam.h"
#include "Matter.h"
#include "Antimatter.h"
#include "Shockwave.h"
#include "Smoke.h"
#include "Spark.h"
#include "Glow.h"

#include "ParticleSystem.h"
#include "ModifiedEulerIntegrator.h"
#include "Particle.h"
#include "Spring.h"

#include <list>
#include <vector>

#define NUM_SHELLS 4

class Controller {
public:
	struct SparkVertex {
        ci::vec3 vertex;
        ci::Vec4f color;
    };
	
	struct Hair {
		Hair(){
			mInitRot = ci::Rand::randFloat( 6.28318f );
		}
		
		void addNode( traer::physics::Particle *node ){
			mNodes.push_back( node );
		}
		
		void addSpring( traer::physics::Spring *spring ){
			mSprings.push_back( spring );
		}
		
		void addRadii( float amt ){
			mRadii.push_back( amt );
		}
		
		ci::vec3 getTipPos(){
			return mNodes[ mNodes.size() - 1 ]->mPos;
		}
		
		ci::vec3 getTipDir(){
			ci::vec3 p1( mNodes[ mNodes.size() - 1 ]->mPos );
			ci::vec3 p2( mNodes[ mNodes.size() - 2 ]->mPos );
			return ( p1 - p2 ).normalized();
		}
		
		ci::vec3 getPos( int i ){
			return mNodes[i]->mPos;
		}
		
		float getRadii( int i ){
			return mRadii[i];
		}
		
		std::vector<traer::physics::Particle*>	mNodes;
		std::vector<traer::physics::Spring*>	mSprings;
		std::vector<float>						mRadii;
		float									mInitRot;
		
		
	};
	
	
	
	Controller();
	void init( Room *room );
	void createSphere( ci::gl::VboMesh &mesh, int res, bool savePositions );
	void drawSphereTri( ci::vec3 va, ci::vec3 vb, ci::vec3 vc, int div, bool savePositions );
	void applyForces();
	void zap( traer::physics::Particle *p );
	void annihilate();
	void update();
	void updateSparks( float t );
	void draw( ci::gl::GlslProg *shader );
	void drawNodes( ci::gl::GlslProg *shader );
	void drawSprings();
	void drawMatter();
	void drawAntimatter();
	void drawMatterShadow( Room *room );
	void drawAntimatterParticles();
	void drawShockwaves( const ci::vec3 &camPos );
	void drawSmokes( const ci::vec3 &right, const ci::vec3 &up );
	void drawSparks();
	void drawGlows();
	void drawHairs();
	void makeParticles();
	void drawSphericalBillboard( const ci::vec3 &camEye, const ci::vec3 &objPos, const ci::vec2 &scale, float rotInRadians );

	Room					*mRoom;
	
	// TRAER PHYSICS
	std::vector<ci::vec3>					mStartPositions;
	float mRadii[NUM_SHELLS];
	traer::physics::ParticleSystem*			mPhysics;
	std::vector<traer::physics::Particle>	mParticles;
	int						mNumParticles;
	
	std::vector<Hair>		mHairs;
	
	std::vector<Shockwave>	mShockwaves;
	std::vector<Glow>		mGlows;
	std::vector<Smoke>		mSmokes;
	
	ci::TriMesh				mHairMesh;
	
	std::list<Spark>		mSparks;
	int						mTotalSparkVertices;
    int						mPrevTotalSparkVertices;
    SparkVertex				*mSparkVerts;
	
	Matter					*mMatter;
	Antimatter				*mAntimatter;
	float					mDistanceMax;
	float					mDistance, mDistanceDest;
	float					mDistPer;
	
	ci::gl::VboMesh			mSphereLo, mSphereMd, mSphereHi;
	int						mIndex;
	std::vector<uint32_t>	mIndices;
	std::vector<ci::vec3>	mPosCoords;
	std::vector<ci::vec3>	mNormals;
	std::vector<ci::vec2>	mTexCoords;
	
	bool					mBroken;
};