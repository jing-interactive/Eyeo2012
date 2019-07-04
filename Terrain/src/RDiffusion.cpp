//
//  RDiffusion.cpp
//  RDTerrain
//
//  Created by Robert Hodgin on 4/8/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#include "RDiffusion.h"
#include "cinder/app/App.h"
#include "cinder/gl/Fbo.h"

using namespace ci;

RDiffusion::RDiffusion()
{
}

RDiffusion::RDiffusion( int w, int h )
{
	mFboWidth		= w;
	mFboHeight		= h;
	mFboSize		= vec2( w, h );
	mFboBounds		= Area( 0, 0, w, h );
	
	mWindowSize		= app::getWindowSize();
	mWindowBounds	= app::getWindowBounds();
	
	mXOffset		= 1.0f/(float)mWindowSize.x;
	mYOffset		= 1.0f/(float)mWindowSize.y;

	mParamU			= 0.1335f;
	mParamV			= 0.0360f;
	mParamF			= 0.0003f;
	mParamK			= 0.0250f;
	mParamN			= 0.03f;//0.985f;
	mParamWind		= 1.0f;

	mThisFbo		= 0;
	mPrevFbo		= 1;
	
	gl::Fbo::Format format;
	format.setColorInternalFormat( GL_RGBA32F_ARB );
	format.setWrap( GL_REPEAT, GL_REPEAT );
	
	mFbo			= gl::Fbo::create( mFboWidth, mFboHeight, format );
	mFbos[0]		= gl::Fbo::create( mFboWidth, mFboHeight, format );
	mFbos[1]		= gl::Fbo::create( mFboWidth, mFboHeight, format );
	mHeightsFbo		= gl::Fbo::create( mFboWidth, mFboHeight, format );
	mNormalsFbo		= gl::Fbo::create( mFboWidth, mFboHeight, format );
	
	
	float W			= 1.0f/(float)app::getWindowWidth();
	float H			= 1.0f/(float)app::getWindowHeight();
	mOffset[0] 		= vec2( -W, -H);
	mOffset[1] 		= vec2(0.0, -H);
	mOffset[2] 		= vec2(  W, -H);
	mOffset[3] 		= vec2( -W, 0.0);
	mOffset[4] 		= vec2(0.0, 0.0);
	mOffset[5] 		= vec2(  W, 0.0);
	mOffset[6]		= vec2( -W, H);
	mOffset[7]		= vec2(0.0, H);
	mOffset[8]		= vec2(  W, H);
	
	float diag		= 0.707106781186f;
	float side		= 1.0f;
	float center	= -6.828427124746f;
	mKernel[0]		= diag;
	mKernel[1]		= side;
	mKernel[2]		= diag;
	mKernel[3]		= side;
	mKernel[4]		= center;
	mKernel[5]		= side;
	mKernel[6]		= diag;
	mKernel[7]		= side;
	mKernel[8]		= diag;
	
	
	reset();
}

void RDiffusion::reset()
{
	for( int i = 0; i < 2; i++ ){
		mFbos[i].bindFramebuffer();
		gl::clear( Color( 0, 0, 0 ) );
		mFbos[i].unbindFramebuffer();
	}
	
	mNormalsFbo.bindFramebuffer();
	gl::clear( Color( 0, 0, 0 ) );
	mNormalsFbo.unbindFramebuffer();
	
	mHeightsFbo.bindFramebuffer();
	gl::setMatricesWindow( mWindowSize, false );
	glBegin( GL_TRIANGLE_STRIP );
	glColor4f( 0, 0, 0, 1 );
	glTexCoord2f( 0, 0 );
	glVertex3f( 0, 0, 0 );
	
	glColor4f( 1, 0, 0, 1 );
	glTexCoord2f( 1, 0 );
	glVertex3f( mFboWidth, 0, 0 );
	
	glColor4f( 0, 1, 0, 1 );
	glTexCoord2f( 0, 1 );
	glVertex3f( 0, mFboHeight, 0 );
	
	glColor4f( 1, 1, 0, 1 );
	glTexCoord2f( 1, 1 );
	glVertex3f( mFboWidth, mFboHeight, 0 );
	glEnd();
	mHeightsFbo.unbindFramebuffer();
}

void RDiffusion::update( float dt, gl::GlslProg *shader, const gl::Texture &glowTex, const bool &isPressed, const ci::vec2 &spherePos, float zoom )
{	
	float xo	= 0.0f;
	float yo	= 0.0f;
	vec2 sphereNorm = ( spherePos + vec2( 300.0f, 300.0f ) )/vec2( 600.0f, 600.0f );
	xo			= ( sphereNorm.x - 0.5 ) * 2.0;
	yo			= ( ( 1.0f - sphereNorm.y ) - 0.5 ) * 2.0;

	sphereNorm	= ( sphereNorm - vec2( 0.5f, 0.5f ) ) * ( zoom * 0.96 + 0.04 ) + vec2( 0.5f, 0.5f );
	vec2 newSpherePos = sphereNorm * mFboSize;
	
	float diag	= 0.707106781186f;
	float side	= 1.0f;
	float center= -6.828427124746f;
	
	mKernel[0]	= diag - xo - yo;
	mKernel[1]	= side - yo;
	mKernel[2]	= diag + xo - yo;
	mKernel[3]	= side - xo;
	mKernel[4]	= center;
	mKernel[5]	= side + xo;
	mKernel[6]	= diag - xo + yo;
	mKernel[7]	= side + yo;
	mKernel[8]	= diag + xo + yo;
	
	const int ITERATIONS = 7;

	gl::setMatricesWindow( mFboSize, false );
	gl::viewport( mFboBounds );
	for( int i = 0; i < ITERATIONS; i++ ) {
		mThisFbo	= ( mThisFbo + 1 ) % 2;
		mPrevFbo	= ( mThisFbo + 1 ) % 2;
		
		mFbos[ mThisFbo ].bindFramebuffer();
		mFbos[ mPrevFbo ].bindTexture( 0 );
		mNormalsFbo.bindTexture( 1 );
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
		
		shader->bind();
		glUniform2fv( shader->getUniformLocation( "offset" ), 9, &(mOffset[0].x) );
		glUniform1fv( shader->getUniformLocation( "kernel" ), 9, mKernel );
		shader->uniform( "tex", 0 );
		shader->uniform( "normalsTex", 1 );
		shader->uniform( "width", (float)mFboWidth );
		shader->uniform( "ru", mParamU );
		shader->uniform( "rv", mParamV );
		shader->uniform( "k", mParamK );
		shader->uniform( "f", mParamF );
		shader->uniform( "n", mParamN );
		shader->uniform( "wind", mParamWind );
		shader->uniform( "dt", dt * 0.25f );
		
		gl::drawSolidRect( mFboBounds );
		
		shader->unbind();
		
		gl::color( ColorA( 1, 1, 1, zoom * 0.97 + 0.03 ) );
		glowTex.enableAndBind();
		gl::enableAlphaBlending();
		float r = 20.0f - ( 1.0f - zoom ) * 12.0f;
		gl::drawSolidRect( Rectf( newSpherePos.x - r, newSpherePos.y - r, newSpherePos.x + r, newSpherePos.y + r ) );
		glowTex.disable();
		gl::disableAlphaBlending();

		mFbos[ mThisFbo ].unbindFramebuffer();
	}
	
	mFbo.bindFramebuffer();
	gl::setMatricesWindow( mWindowSize, false );
	gl::draw( mFbos[ mThisFbo ].getTexture(), mFboBounds );
	mFbo.unbindFramebuffer();
}

void RDiffusion::drawIntoHeightsFbo( gl::GlslProg *shader, vec3 scale )
{
	gl::Texture tmp = mHeightsFbo.getTexture();
	
	mHeightsFbo.bindFramebuffer();
	mFbo.bindTexture( 0 );
	tmp.bind( 1 );
	
	shader->bind();
	shader->uniform( "reactionTex", 0 );
	shader->uniform( "heightTex", 1 );
	
	gl::setMatricesWindow( mFboSize, false );
	gl::viewport( mFboBounds );
	gl::drawSolidRect( mFboBounds );
	
	shader->unbind();

	mHeightsFbo.unbindFramebuffer();
}


void RDiffusion::drawIntoNormalsFbo( gl::GlslProg *shader )
{
	mNormalsFbo.bindFramebuffer();
	mHeightsFbo.bindTexture( 0 );
	
	shader->bind();
	shader->uniform( "tex", 0 );
	shader->uniform( "xOffset", mXOffset );
	shader->uniform( "yOffset", mYOffset );
	
	gl::setMatricesWindow( mFboSize, false );
	gl::viewport( mFboBounds );
	gl::drawSolidRect( mFboBounds );
	shader->unbind();
	
	mNormalsFbo.unbindFramebuffer();
}

void RDiffusion::setMode( int index )
{
	if( index == 1 ){
		mParamU	= 0.1335f;
		mParamV	= 0.0360f;
		mParamF	= 0.0003f;
		mParamK	= 0.0250f;
		mParamN = 0.09850f;
		mParamWind = 1.0f;
	} else if( index == 2 ){
		mParamU = 0.1335f;
		mParamV = 0.0451f;
		mParamF	= 0.0100f;
		mParamK	= 0.0368f;
		mParamN	= 0.01f;
		mParamWind = 0.0f;
	} else if( index == 3 ){
		mParamU = 0.1335f;
		mParamV = 0.0400f;
		mParamF	= 0.0006f;
		mParamK	= 0.0368f;
		mParamN	= 0.4150f;
		mParamWind = 1.0f;
	}
}

ivec2 RDiffusion::toFboVec( const ci::vec3 &pos, float scale, float res )
{
	int xi = (int)( pos.x/(scale*res) ) + mFboSize.x/2;
	int zi = (int)( pos.z/(scale*res) ) + mFboSize.x/2;
	
	return ivec2( xi, zi );
}

gl::Texture RDiffusion::getTexture()
{
	return mFbos[mThisFbo].getTexture();
}

gl::Texture RDiffusion::getHeightsTexture()
{
	return mHeightsFbo.getTexture();
}

gl::Texture RDiffusion::getNormalsTexture()
{
	return mNormalsFbo.getTexture();	
}

