//
//  Shard.cpp
//  Sol
//
//  Created by Robert Hodgin on 12/10/11.
//  Copyright (c) 2011 __MyCompanyName__. All rights reserved.
//

#include "Shard.h"
#include "cinder/CinderMath.h"
#include "cinder/Rand.h"
#include "cinder/gl/gl.h"
#include "cinder/app/App.h"
#include <boost/foreach.hpp>

#define TWO_PI 6.28318531

using namespace ci;
using std::vector;

Shard::Shard( vec3 pos, vec3 dir, float height, float radius )
: mPos( pos ), mDir( dir ), mHeight( height ), mRadius( radius )
{
}


void Shard::init( TriMesh *mesh, const Color &c )
{
	int numVerts = mesh->getNumVertices();
	
	float per = 0.2 - mHeight;
	float initAngle = Rand::randFloat( M_PI * per );
	
	float facetPer	= Rand::randFloat();
	if( mRadius < 0.03f || facetPer < 0.2f )
		facetPer = 0.0f;
	
	
	float uc1 = 0.0f;
	float uc2 = 1.0f;
	
	float vc1 = 0.0f;//Rand::randFloat( 0.15f );
	float vc2 = mHeight * 0.8f;//0.75f;
	float vc3 = mHeight * 0.8f;//1.0f;
	float vc4 = mHeight;//1.0f;
	
	mMid	= mPos + mDir * ( mHeight - Rand::randFloat( mRadius * 0.75f, mRadius * 1.25f ) * ( 1.0f - facetPer ) * 2.0f );
	mTip	= mPos + mDir * mHeight;
	
	vec3 perp1 = mDir.cross( {0, 1, 0} );
	perp1.normalize();
	vec3 perp2 = perp1.cross( mDir );
	perp2.normalize();
	perp1		= perp2.cross( mDir );
	perp1.normalize();
	
	float numSides = 6.0f;
	float invNumSides = 1.0f/numSides;
	vector<vec3> tipPositions;
	for( int i=0; i<numSides; i++ ){
		vec3 tempV1, tempV2, tempV3, tempV4;
		vec3 v1, v2, v3, v4;
		vec2 t1, t2, t3, t4;

		for( int k=0; k<2; k++ ){
			float angle = initAngle + ( i + k ) * invNumSides * TWO_PI;
			float cosa	= cos( angle );
			float sina	= sin( angle );
			vec3 offset = ( perp1 * cosa + perp2 * sina ) * mRadius;
			
			v1 = mPos + offset;
			v2 = mMid + offset;
			v3 = mMid + offset;
			v4 = mTip + offset * facetPer;
			
			if( k == 0 ){
				tempV1 = v1;
				tempV2 = v2;
				tempV3 = v3;
				tempV4 = v4;
			}
			
			mesh->appendVertex( v1 );
			mesh->appendVertex( v2 );
			mesh->appendVertex( v3 );
			mesh->appendVertex( v4 );
			
			if( k == 0 ){
				t1	= vec2( uc1, vc1 );
				t2	= vec2( uc1, vc2 );
				t3	= vec2( uc1, vc3 );
				t4	= vec2( uc1, vc4 );
			} else {
				t1	= vec2( uc2, vc1 );
				t2	= vec2( uc2, vc2 );
				t3	= vec2( uc2, vc3 );
				t4	= vec2( uc2, vc4 );
			}
			
			
			
			mesh->appendTexCoord( t1 );
			mesh->appendTexCoord( t2 );
			mesh->appendTexCoord( t3 );
			mesh->appendTexCoord( t4 );
			
			mesh->appendColorRGB( c );
			mesh->appendColorRGB( c );
			mesh->appendColorRGB( c );
			mesh->appendColorRGB( c );
		}
		
		vec3 n1 = calcSurfaceNormal( tempV1, tempV2, v1 );
		vec3 n2 = calcSurfaceNormal( tempV3, tempV4, v3 );
		
		
		mesh->appendNormal( n1 );
		mesh->appendNormal( n1 );
		mesh->appendNormal( n2 );
		mesh->appendNormal( n2 );
		
		mesh->appendNormal( n1 );
		mesh->appendNormal( n1 );
		mesh->appendNormal( n2 );
		mesh->appendNormal( n2 );
		
		tipPositions.push_back( v4 );
	}
	
	BOOST_FOREACH( vec3 &v, tipPositions ){
		mesh->appendVertex( v );
		mesh->appendTexCoord( vec2( uc2, vc4 ) );
		mesh->appendColorRGB( c );
		mesh->appendNormal( mDir.normalized() );
	}

//	// base
//	mesh->appendTriangle( 0 + numVerts, 16 + numVerts,  8 + numVerts );
//	mesh->appendTriangle( 0 + numVerts, 24 + numVerts, 16 + numVerts );
//	mesh->appendTriangle( 0 + numVerts, 32 + numVerts, 24 + numVerts );
//	mesh->appendTriangle( 0 + numVerts, 40 + numVerts, 32 + numVerts );
	
	// sides
	for( int i=0; i<6; i++ ){
		int index = numVerts + i * 8;
		mesh->appendTriangle( index + 0, index + 1, index + 4 );
		mesh->appendTriangle( index + 1, index + 5, index + 4 );
		mesh->appendTriangle( index + 2, index + 3, index + 6 );
		mesh->appendTriangle( index + 3, index + 7, index + 6 );
	}
	
	// cap
	numVerts = mesh->getNumVertices();
	mesh->appendTriangle( numVerts - 1, numVerts - 3, numVerts - 2 );
	mesh->appendTriangle( numVerts - 1, numVerts - 4, numVerts - 3 );
	mesh->appendTriangle( numVerts - 1, numVerts - 5, numVerts - 4 );
	mesh->appendTriangle( numVerts - 1, numVerts - 6, numVerts - 5 );
}

vec3 Shard::calcSurfaceNormal( const ci::vec3 &p1, const ci::vec3 &p2, const ci::vec3 &p3 )
{
	ci::vec3 normal;
	ci::vec3 u = p1 - p3;
	ci::vec3 v = p1 - p2;
	
	normal.x = u.y * v.z - u.z * v.y;
	normal.y = u.z * v.x - u.x * v.z;
	normal.z = u.x * v.y - u.y * v.x;
	return normal.normalized();
}

void Shard::update()
{
}

void Shard::draw()
{
}

