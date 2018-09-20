//
//  Controller.cpp
//  BubbleChamber
//
//  Created by Robert Hodgin on 5/3/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#include "cinder/app/App.h"
#include "cinder/gl/gl.h"
#include "cinder/Rand.h"
#include "Controller.h"

using namespace ci;
using std::vector;
using std::list;

Controller::Controller()
{
}

void Controller::init( Room *room, int gridDim )
{
	mRoom					= room;
	mGridDim				= gridDim;
	
	mPresetIndex = 1;
	
	createNodes( gridDim );
	
	createSphere( mSphereVbo, 3 );
}

void Controller::createNodes( int gridDim )
{
	int dim		= gridDim;
	int halfDim = floor( gridDim/2 );
	float spacing	= 10.0f;
	
	for( int z=0; z<dim; z++ ){
		for( int x=0; x<dim; x++ ){
			float y;
			if( mPresetIndex == 1 ){
				y = -100.0f;
			} else if( mPresetIndex == 2 ){
				y = Rand::randFloat( -mRoom->getDims().y, mRoom->getDims().y ) * 0.5f;
			}
			
			vec3 pos = vec3( ( x - halfDim ) * spacing, y, ( z - halfDim ) * spacing );
			vec3 vel = vec3();
			mNodes.push_back( Node( pos, vel ) );
		}
	}
}

void Controller::createSphere( gl::VboMesh &vbo, int res )
{
	float X = 0.525731112119f; 
	float Z = 0.850650808352f;
	
	static vec3 verts[12] = {
		vec3( -X, 0.0f, Z ), vec3( X, 0.0f, Z ), vec3( -X, 0.0f, -Z ), vec3( X, 0.0f, -Z ),
		vec3( 0.0f, Z, X ), vec3( 0.0f, Z, -X ), vec3( 0.0f, -Z, X ), vec3( 0.0f, -Z, -X ),
		vec3( Z, X, 0.0f ), vec3( -Z, X, 0.0f ), vec3( Z, -X, 0.0f ), vec3( -Z, -X, 0.0f ) };
	
	static GLuint triIndices[20][3] = { 
		{0,4,1}, {0,9,4}, {9,5,4}, {4,5,8}, {4,8,1}, {8,10,1}, {8,3,10}, {5,3,8}, {5,2,3}, {2,7,3},
		{7,10,3}, {7,6,10}, {7,11,6}, {11,0,6}, {0,1,6}, {6,1,10}, {9,0,11}, {9,11,2}, {9,2,5}, {7,2,11} };
	
	gl::VboMesh::Layout layout;
	layout.setStaticPositions();
	layout.setStaticNormals();
	
	mPosCoords.clear();
	mNormals.clear();
	for( int i=0; i<20; i++ ){
		drawSphereTri( verts[triIndices[i][0]], verts[triIndices[i][1]], verts[triIndices[i][2]], res );
	}
	vbo = gl::VboMesh( mPosCoords.size(), 0, layout, GL_TRIANGLES );	
	vbo.bufferPositions( mPosCoords );
	vbo.bufferNormals( mNormals );
}

void Controller::drawSphereTri( vec3 va, vec3 vb, vec3 vc, int div )
{
	if( div <= 0 ){
		mPosCoords.push_back( va );
		mPosCoords.push_back( vb );
		mPosCoords.push_back( vc );
		vec3 vn = ( va + vb + vc ) * 0.3333f;
		mNormals.push_back( va );
		mNormals.push_back( vb );
		mNormals.push_back( vc );
	} else {
		vec3 vab = ( ( va + vb ) * 0.5f ).normalized();
		vec3 vac = ( ( va + vc ) * 0.5f ).normalized();
		vec3 vbc = ( ( vb + vc ) * 0.5f ).normalized();
		drawSphereTri( va, vab, vac, div-1 );
		drawSphereTri( vb, vbc, vab, div-1 );
		drawSphereTri( vc, vac, vbc, div-1 );
		drawSphereTri( vab, vbc, vac, div-1 );
	}
}

void Controller::update( float dt, bool tick )
{
//	mNewNodes.clear();
	
	// SMOKES
	for( vector<Smoke>::iterator it = mSmokes.begin(); it != mSmokes.end(); ){
		if( it->mIsDead ){
			it = mSmokes.erase( it );
		} else {
			it->update( dt );
			++ it;
		}
	}
	
	// GLOWS
	for( vector<Glow>::iterator it = mGlows.begin(); it != mGlows.end(); ){
		if( it->mIsDead ){
			it = mGlows.erase( it );
		} else {
			it->update( dt );
			++ it;
		}
	}
	
	// SHOCKWAVES
	for( vector<Shockwave>::iterator it = mShockwaves.begin(); it != mShockwaves.end(); ){
		if( it->mIsDead ){
			it = mShockwaves.erase( it );
		} else {
			it->update( dt );
			++it;
		}
	}
	sort( mShockwaves.begin(), mShockwaves.end(), depthSortFunc );
	
	// NODES
	for( vector<Node>::iterator it = mNodes.begin(); it != mNodes.end(); ){
		// APPLY SHOCKWAVES TO NODES
		for( vector<Shockwave>::iterator shockIt = mShockwaves.begin(); shockIt != mShockwaves.end(); ++shockIt )
		{
			vec3 dirToParticle = shockIt->mPos - it->mPos;
			float dist = dirToParticle.length();
			if( dist > shockIt->mRadiusPrev && dist < shockIt->mRadius ){
				vec3 dirToParticleNorm = dirToParticle.normalized();
				it->mAcc -= dirToParticleNorm * shockIt->mImpulse;
			}
		}
		
		it->update( dt );
		++it;
	}
	
//	for( vector<Node>::iterator it = mNewNodes.begin(); it != mNewNodes.end(); ++it ){
//		mNodes.push_back( *it );
//	}
}

void Controller::explode()
{
	vec3 pos		= vec3();//mRoom->getRandRoomPos() * vec3( 0.4f, 0.2f, 0.4f );
	pos.y			= -150.0f;
	if( mPresetIndex == 2 )
		pos.y		= 0.0f;
	
	float lifespan  = 100.0f;
	float speed		= 2.0f;
	mShockwaves.push_back( Shockwave( pos, lifespan, speed ) );
	
	addSmokes( pos, 2 );
	addGlows( pos, 4 );
}

void Controller::draw()
{
}

void Controller::drawNodes( ci::gl::GlslProg *shader )
{
	for( vector<Node>::iterator it = mNodes.begin(); it != mNodes.end(); ++it ){
		shader->uniform( "matrix", it->mMatrix );
		shader->uniform( "color", it->mColor );
		it->draw();
	}
}

void Controller::drawConnections()
{
	glBegin( GL_LINES );
	for( vector<Node>::iterator it = mNodes.begin(); it != mNodes.end(); ++it ){
		gl::color( Color( 1.0f, 1.0f, 1.0f ) );
		gl::vertex( it->mPos );
		gl::color( Color( 0.0f, 0.0f, 0.0f ) );
		gl::vertex( it->mPosInit );
	}
	glEnd();
}

void Controller::drawShockwaves( gl::GlslProg *shader )
{
	for( vector<Shockwave>::iterator it = mShockwaves.begin(); it != mShockwaves.end(); ++it ){
		shader->uniform( "matrix", it->mMatrix );
		shader->uniform( "color", it->mColor );
		shader->uniform( "alpha", it->mAgePer );
		gl::draw( mSphereVbo );
	}
}

void Controller::drawShockwaveCenters()
{
	for( vector<Shockwave>::iterator it = mShockwaves.begin(); it != mShockwaves.end(); ++it ){
		it->draw();
	}
}

void Controller::drawSmokes( const vec3 &right, const vec3 &up )
{
	for( vector<Smoke>::iterator it = mSmokes.begin(); it != mSmokes.end(); ++it ){
		it->draw( right, up );
	}
}

void Controller::drawGlows( const vec3 &right, const vec3 &up )
{
	for( vector<Glow>::iterator it = mGlows.begin(); it != mGlows.end(); ++it ){
		it->draw( right, up );
	}
}

void Controller::addSmokes( const vec3 &v, int amt )
{
	for( int i=0; i<amt; i++ ){
		vec3 dir		= Rand::randvec3();
		float radius	= Rand::randFloat( 0.1f, 0.2f );
		vec3 pos		= v + dir * Rand::randFloat( radius );
		vec3 vel		= vec3();
		float lifespan	= Rand::randFloat( 65.0f, 85.0f );
		
		mSmokes.push_back( Smoke( pos, vel, radius, lifespan ) );
	}
}

void Controller::addGlows( const vec3 &v, int amt )
{
	for( int i=0; i<amt; i++ ){
		vec3 dir		= Rand::randvec3();
		float radius	= Rand::randFloat( 50.0f, 60.0f );
		vec3 pos		= v + dir;
		vec3 vel		= vec3();
		float lifespan	= Rand::randFloat( 20.0f, 25.0f );
		
		mGlows.push_back( Glow( pos, vel, radius, lifespan ) );
	}
}

void Controller::preset( int presetIndex )
{
	mPresetIndex = presetIndex;
	clear();
	createNodes( mGridDim );
}

void Controller::clear()
{
	mNodes.clear();
}

bool depthSortFunc( Shockwave a, Shockwave b ){
	return a.mPos.z > b.mPos.z;
}


