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
	mPrevTotalVerts		= -1;
	mTotalVerts			= 0;
    mFieldLineVerts		= NULL;
}

void Controller::init( Room *room, int maxParticles )
{
	mRoom				= room;
	mMaxParticles		= maxParticles;
	
	mDraggedParticle	= NULL;
	
	addParticle( vec3(-340.0f,   0.0f,-28.0f ), 1.0f );
	addParticle( vec3( 340.0f,   0.0f,-10.0f ),-1.0f );
	addParticle( vec3(   0.0f,-200.0f,-30.0f ), 1.0f );
	addParticle( vec3(   0.0f, 200.0f, 40.0f ),-1.0f );
	addParticle( vec3(-200.0f, 100.0f, 50.0f ),-1.0f );
	addParticle( vec3( 200.0f, 100.0f, 70.0f ), 1.0f );
}

void Controller::initParticles()
{
	int numParticles = mMaxParticles;
	for( int i=0; i<numParticles; i++ ){
		vec3 pos = mRoom->getRandRoomPos();
		float charge = 1.0f;
		if( Rand::randBool() ) charge = -1.0f;
		addParticle( pos, charge );
	}
}

void Controller::applyForce()
{
//	vec3 dir			= mLeftParticle->mPos - mRightParticle->mPos;
//	float distSqrd		= dir.lengthSquared();
//	float q				= ( mLeftParticle->mCharge * mRightParticle->mCharge ) / distSqrd;
//	mLeftParticle->mForce  = q;
//	mRightParticle->mForce = q;
	
	for( vector<Particle>::iterator particle = mParticles.begin(); particle != mParticles.end(); ++particle ){
		for( vector<FieldLine>::iterator it = mFieldLines.begin(); it != mFieldLines.end(); ++it ){
			vec3 dir			= it->mPos - particle->mPos;
			float totalCharge	= it->mCharge * particle->mCharge;
			float distSqrd		= dir.lengthSquared();
			
			float q			= totalCharge / distSqrd;
			dir				= dir.normalized() * q;
			
			it->mVel		+= dir * 200000.0f;
		}
	}
}

void Controller::update( const ci::Camera &cam, float dt, bool tick )
{
	applyForce();
	
	// UPDATE PARTICLES / ADD SHARDS
	for( vector<Particle>::iterator it = mParticles.begin(); it != mParticles.end(); ++it ){
		it->update( cam, dt );
		
		if( Rand::randFloat() < 0.1f && tick && it->mCharge < 0.0f ){
			it->addShards( 3 );
		}
	}

	// ADD EFFECTS
	if( tick ){
		// ADD FIELD LINES
		int numFieldLinesToSpawn	= 20;
		int numGlowsToSpawn			= 3;
		int numNebulasToSpawn		= 0;
		int numGlobsToSpawn			= 1;
		
		for( vector<Particle>::iterator it = mParticles.begin(); it != mParticles.end(); ++it ){
			addFieldLines( &(*it), numFieldLinesToSpawn );
			
			if( it->mCharge > 0.0f ){
				addGlows( &(*it), numGlowsToSpawn );
				
				if( Rand::randFloat() < 0.04f ){
					addNebulas( &(*it), numNebulasToSpawn );
					addGlobs( it->mPos, numGlobsToSpawn );
				}
			}
		}
	}
	
	// UPDATE EFFECTS
	updateFieldLines( dt, tick );
	
	// GLOWS
	for( vector<Glow>::iterator it = mGlows.begin(); it != mGlows.end(); ){
		if( it->mIsDead ){
			it = mGlows.erase( it );
		} else {
			it->update( dt );
			++ it;
		}
	}
	
	// NEBULAS
	for( vector<Nebula>::iterator it = mNebulas.begin(); it != mNebulas.end(); ){
		if( it->mIsDead ){
			it = mNebulas.erase( it );
		} else {
			it->update( dt );
			++ it;
		}
	}
	
	// GLOBS
	vector<Glob> newGlobs;
	for( vector<Glob>::iterator it = mGlobs.begin(); it != mGlobs.end(); ){
		if( it->mIsDead ){
			it = mGlobs.erase( it );
		} else {
			it->update( mRoom->getFloorLevel(), dt );
			
			if( it->mBounced ){
				if( it->mRadius > 3.0f ){
					int numNewGlobs = 10;
					for( int i=0; i<numNewGlobs; i++ ){
						vec3 vel			= it->mVel * 0.5f + Rand::randvec3();
						if( vel.y < 0.0f ) vel.y *= -0.8f;
						float radius		= it->mRadius * 0.5f;
						float lifespan		= it->mLifespan * 0.25f;
						newGlobs.push_back( Glob( it->mPos, vel, radius, lifespan ) );
						
						it->mIsDead = true;
					}
				}
				
				it->mBounced = false;
			}
			++ it;
		}
	}
	
	for( vector<Glob>::iterator it = newGlobs.begin(); it != newGlobs.end(); ++it ){
		mGlobs.push_back( *it );
	}
}

void Controller::updateFieldLines( float dt, bool tick )
{
	// FIELD LINES
	for( vector<FieldLine>::iterator it = mFieldLines.begin(); it != mFieldLines.end(); ){
		if( it->mIsDead ){
			it = mFieldLines.erase( it );
		} else {
			it->update( dt, tick );
			++ it;
		}
	}
	mTotalVerts = mFieldLines.size() * ( FieldLine::sLen - 1 ) * 2;
	
    if( mTotalVerts != mPrevTotalVerts ){
        if (mFieldLineVerts != NULL) {
            delete[] mFieldLineVerts;
        }
        mFieldLineVerts = new VboVertex[mTotalVerts];
        mPrevTotalVerts = mTotalVerts;
    }
	
	int vIndex = 0;
	for( vector<FieldLine>::iterator it = mFieldLines.begin(); it != mFieldLines.end(); ++it ){
		for( int i=0; i<FieldLine::sLen-1; i++ ){
			float alpha = ( 1.0f - i*FieldLine::sInvLen ) * it->mAgePer * 0.5f;
			mFieldLineVerts[vIndex].vertex = it->mPositions[i];
//			mFieldLineVerts[vIndex].normal = it->mNormals[i];
			mFieldLineVerts[vIndex].color  = Vec4f( it->mColor, it->mColor, it->mColor, alpha );
			vIndex++;
			
			alpha = ( 1.0f - (i+1)*FieldLine::sInvLen ) * it->mAgePer * 0.5f;
			mFieldLineVerts[vIndex].vertex = it->mPositions[i+1];
//			mFieldLineVerts[vIndex].normal = it->mNormals[i+1];
			mFieldLineVerts[vIndex].color  = Vec4f( it->mColor, it->mColor, it->mColor, alpha );
			vIndex++;
		}
	}
}

void Controller::draw()
{
	for( vector<Particle>::iterator it = mParticles.begin(); it != mParticles.end(); ++it ){
		it->draw();
	}
}

void Controller::drawFieldLines( gl::GlslProg *shader )
{
	glLineWidth( 1.0f );
	if( mTotalVerts > 0 ){
		glEnableClientState( GL_VERTEX_ARRAY );
//		glEnableClientState( GL_NORMAL_ARRAY );
		glEnableClientState( GL_COLOR_ARRAY );
		glVertexPointer( 3, GL_FLOAT, sizeof(VboVertex), mFieldLineVerts );
//		glNormalPointer( GL_FLOAT, sizeof(VboVertex), &mFieldLineVerts[0].normal );
		glColorPointer( 4, GL_FLOAT, sizeof(VboVertex), &mFieldLineVerts[0].color );
		
//		shader->uniform( "shadow", 1.0f );
		glDrawArrays( GL_LINES, 0, mTotalVerts );
//		gl::pushModelView();
//		gl::translate( vec3( 0.0f, 0.3f, 0.0f ) );
//		shader->uniform( "shadow", 0.0f );
//		glDrawArrays( GL_LINES, 0, mTotalVerts );
//		gl::popModelView();
		
		glDisableClientState( GL_VERTEX_ARRAY );
//		glDisableClientState( GL_NORMAL_ARRAY );
		glDisableClientState( GL_COLOR_ARRAY );
	}
}

void Controller::drawCoronas( const vec3 &right, const vec3 &up )
{
	for( vector<Particle>::iterator it = mParticles.begin(); it != mParticles.end(); ++it ){
		if( it->mCharge > 0.0f ){
			vec3 pos	= it->mPos;
			vec2 size	= vec2( it->mRadius, it->mRadius ) * 5.0f;
			float rot	= 0.0f;
			float c		= it->mColor;
			
			gl::color( ColorA( c, c, c, 1.0f ) );
			gl::drawBillboard( pos, size, rot, right, up );
		}
	}
}

void Controller::drawGlows( const vec3 &right, const vec3 &up )
{
	for( vector<Glow>::iterator it = mGlows.begin(); it != mGlows.end(); ++it ){
		float c		= it->mColor;
		gl::color( ColorA( c, c, c, 1.0f ) );
		it->draw( right, up );
	}
}

void Controller::drawNebulas( const vec3 &right, const vec3 &up )
{
	for( vector<Nebula>::iterator it = mNebulas.begin(); it != mNebulas.end(); ++it ){
		float c		= it->mColor;
		gl::color( ColorA( c, c, c, 1.0f ) );

		it->draw( right, up );
	}
}

void Controller::drawGlobs( const vec3 &right, const vec3 &up )
{
	for( vector<Glob>::iterator it = mGlobs.begin(); it != mGlobs.end(); ++it ){
		it->draw( right, up );
	}
}

void Controller::drawShards( gl::GlslProg *shader )
{
	for( vector<Particle>::iterator it = mParticles.begin(); it != mParticles.end(); ++it ){
		shader->uniform( "matrix", it->mMatrix );
		gl::draw( it->mShardMesh );
	}
}

void Controller::addParticle( const vec3 &pos, float charge )
{
	mParticles.push_back( Particle( pos, charge ) );
}

void Controller::addFieldLines( Particle *p, int amt )
{
	for( int i=0; i<amt; i++ ){
		vec3 dir		= Rand::randvec3();
		vec3 pos		= p->mPos + dir * p->mRadius;
		vec3 vel		= dir;
		float lifespan	= 30.0f;
		
		mFieldLines.push_back( FieldLine( pos, vel, p->mCharge, lifespan ) );
	}
}

void Controller::addGlows( Particle *p, int amt )
{
	for( int i=0; i<amt; i++ ){
		float radius	= Rand::randFloat( 10.0f, 18.0f );
		vec3 dir		= Rand::randvec3();
		vec3 pos		= p->mPos + dir;
		vec3 vel		= dir * Rand::randFloat( 0.5f, 0.75f );
		float lifespan	= 30.0f;
		
		mGlows.push_back( Glow( pos, vel, radius, p->mCharge, lifespan ) );
	}
}

void Controller::addNebulas( Particle *p, int amt )
{
	for( int i=0; i<amt; i++ ){
		float radius		= Rand::randFloat( 40.0f, 50.0f );
		vec3 dir			= Rand::randvec3();
		vec3 pos			= p->mPos + dir * ( p->mRadius - radius * 0.25f );
		vec3 vel			= dir * Rand::randFloat( 0.1f, 0.3f );
		float lifespan		= 45.0f;
		
		mNebulas.push_back( Nebula( pos, vel, radius, p->mCharge, lifespan ) );
	}
}

void Controller::addGlobs( const vec3 &pos, int amt )
{
	for( int i=0; i<amt; i++ ){
		vec3 vel			= vec3();
		float radius		= Rand::randFloat( 5.0f, 8.0f );
		float lifespan		= 500.0f;
		mGlobs.push_back( Glob( pos, vel, radius, lifespan ) );
	}
}

void Controller::checkForParticleTouch( const vec2 &mousePos )
{
	for( vector<Particle>::iterator it = mParticles.begin(); it != mParticles.end(); ++it ){
		vec2 dir	= it->mScreenPos - mousePos;
		float dist	= dir.length();
		if( dist < it->mScreenRadius ){
			mDraggedParticle = &(*it);
		}
	}
}

void Controller::dragParticle( const vec2 &mousePos )
{
	if( mDraggedParticle != NULL ){
		float x = -( mousePos.x - app::getWindowCenter().x )/app::getWindowCenter().x;
		float y = -( mousePos.y - app::getWindowCenter().y )/app::getWindowCenter().y;
		vec3 mouseWorldPos = vec3( x * mRoom->mDims.x, y * mRoom->mDims.y, mDraggedParticle->mPos.z/1.425f ) * 1.425f;
		mDraggedParticle->mPos = mouseWorldPos;
	}
}

void Controller::releaseDraggedParticles()
{
	mDraggedParticle = NULL;
}

void Controller::clear()
{
	mFieldLines.clear();
	mGlows.clear();
	mNebulas.clear();
	mParticles.clear();
	mGlobs.clear();
	
	initParticles();
}

