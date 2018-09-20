//
//  Room.cpp
//  Stillness
//
//  Created by Robert Hodgin on 12/20/11.
//  Copyright (c) 2011 __MyCompanyName__. All rights reserved.
//

#include "cinder/app/App.h"
#include "cinder/gl/Texture.h"
#include "cinder/Rand.h"
#include "Room.h"

const float MAX_TIMEMULTI	= 120.0f;
const float GRAVITY			= -0.02f;

using namespace ci;

Room::Room()
{
}

Room::Room( const vec3 &dims, bool isPowerOn, bool isGravityOn )
{	
	// TIME
	mTime			= (float)app::getElapsedSeconds();
	mTimeElapsed	= 0.0f;
	mTimeMulti		= 60.0f;
	mTimer			= 0.0f;
	mTick			= false;
	
	mDims		= dims;
	mDimsDest	= dims;
	
	mIsPowerOn		= isPowerOn;
	if( mIsPowerOn ) mPower = 1.0f;
	else			 mPower = 0.0f;
	
	mIsGravityOn	= isGravityOn;
	mDefaultGravity = vec3( 0.0f, GRAVITY, 0.0f );
}

void Room::init()
{
	int						index;
	std::vector<uint32_t>	indices;
	std::vector<ci::vec3>	posCoords;
	std::vector<ci::vec3>	normals;
	std::vector<ci::vec2>	texCoords;
	
	float X = 1.0f;
	float Y = 1.0f;
	float Z = 1.0f;
	
	static vec3 verts[8] = {
		vec3(-X,-Y,-Z ), vec3(-X,-Y, Z ), 
		vec3( X,-Y, Z ), vec3( X,-Y,-Z ),
		vec3(-X, Y,-Z ), vec3(-X, Y, Z ), 
		vec3( X, Y, Z ), vec3( X, Y,-Z ) };
	
	static GLuint vIndices[12][3] = { 
		{0,1,3}, {1,2,3},	// floor
		{4,7,5}, {7,6,5},	// ceiling
		{0,4,1}, {4,5,1},	// left
		{2,6,3}, {6,7,3},	// right
		{1,5,2}, {5,6,2},	// back
		{3,7,0}, {7,4,0} }; // front
	
	static vec3 vNormals[6] = {
		vec3( 0, 1, 0 ),	// floor
		vec3( 0,-1, 0 ),	// ceiling
		vec3( 1, 0, 0 ),	// left	
		vec3(-1, 0, 0 ),	// right
		vec3( 0, 0,-1 ),	// back
		vec3( 0, 0, 1 ) };	// front
	
	static vec2 vTexCoords[4] = {
		vec2( 0.0f, 0.0f ),
		vec2( 0.0f, 1.0f ), 
		vec2( 1.0f, 1.0f ), 
		vec2( 1.0f, 0.0f ) };
	
	static GLuint tIndices[12][3] = {
		{0,1,3}, {1,2,3},	// floor
		{0,1,3}, {1,2,3},	// ceiling
		{0,1,3}, {1,2,3},	// left
		{0,1,3}, {1,2,3},	// right
		{0,1,3}, {1,2,3},	// back
		{0,1,3}, {1,2,3} };	// front
		
	gl::VboMesh::Layout layout;
	layout.setStaticIndices();
	layout.setStaticPositions();
	layout.setStaticNormals();
	layout.setStaticTexCoords2d();
	
	index = 0;
	posCoords.clear();
	normals.clear();
	indices.clear();
	texCoords.clear();
	
	for( int i=0; i<12; i++ ){
		posCoords.push_back( verts[vIndices[i][0]] );
		posCoords.push_back( verts[vIndices[i][1]] );
		posCoords.push_back( verts[vIndices[i][2]] );
		indices.push_back( index++ );
		indices.push_back( index++ );
		indices.push_back( index++ );
		normals.push_back( vNormals[i/2] );
		normals.push_back( vNormals[i/2] );
		normals.push_back( vNormals[i/2] );
		texCoords.push_back( vTexCoords[tIndices[i][0]] );
		texCoords.push_back( vTexCoords[tIndices[i][1]] );
		texCoords.push_back( vTexCoords[tIndices[i][2]] );
	}
	
//	console() << "posCoords size = " << posCoords.size() << std::endl;
//	console() << "indices size = " << indices.size() << std::endl;
//	console() << "normals size = " << normals.size() << std::endl;
//	console() << "texCoords size = " << texCoords.size() << std::endl;	

	mVbo = gl::VboMesh( posCoords.size(), indices.size(), layout, GL_TRIANGLES );	
	mVbo.bufferIndices( indices );
	mVbo.bufferPositions( posCoords );
	mVbo.bufferNormals( normals );
	mVbo.bufferTexCoords2d( 0, texCoords );
	mVbo.unbindBuffers();
}

void Room::updateTime()
{
	float prevTime	= mTime;
	mTime			= (float)app::getElapsedSeconds();
	float dt		= mTime - prevTime;
	mTimeAdjusted	= dt * mTimeMulti;
	mTimeElapsed	+= mTimeAdjusted;
	
	mTimer += mTimeAdjusted;
	mTick = false;
	if( mTimer > 1.0f ){
		mTick = true;
		mTimer = 0.0f;
	}
}

void Room::update()
{
	if( mIsPowerOn )	mPower -= ( mPower - 1.0f ) * 0.2f;
	else				mPower -= ( mPower - 0.0f ) * 0.2f;
	
	if( mIsGravityOn )	mGravity -= ( mGravity - mDefaultGravity ) * 0.2f;
	else				mGravity -= ( mGravity - vec3() ) * 0.2f;
	
	mDims -= ( mDims - mDimsDest ) * 0.1f;
	
	updateTime();
}

void Room::draw()
{
	gl::draw( mVbo );
}

void Room::adjustTimeMulti( float amt )
{
	mTimeMulti = constrain( mTimeMulti - amt, 0.0f, (float)MAX_TIMEMULTI );
}

float Room::getTimePer()
{
	return mTimeMulti/MAX_TIMEMULTI;
}

float Room::getTimeDelta()
{
	return mTimeAdjusted;
}

bool Room::getTick()
{
	return mTick;
}

float Room::getLightPower()
{
	float p = getPower() * 5.0f * M_PI;
	float lightPower = cos( p ) * 0.5f + 0.5f;	
	return lightPower;
}

vec3 Room::getRandRoomPos()
{
	return vec3( Rand::randFloat( -mDims.x, mDims.x ) * 0.9f,
				 Rand::randFloat( -mDims.y, mDims.y ) * 0.9f,
				 Rand::randFloat( -mDims.z, mDims.z ) * 0.9f );
}

vec3 Room::getRandCeilingPos()
{
	return vec3( Rand::randFloat( -mDims.x * 0.8f, mDims.x * 0.8f ), 
				 mDims.y, 
				 Rand::randFloat( -mDims.z * 0.5f, mDims.z * 0.5f ) );
}

vec3 Room::getCornerCeilingPos()
{
	return vec3( mDims.x, mDims.y,-mDims.z ) * 0.9f;
}

vec3 Room::getCornerFloorPos()
{
	return vec3(-mDims.x,-mDims.y,-mDims.z ) * 0.9f;	
}

float Room::getFloorLevel()
{
	return -mDims.y;
}

