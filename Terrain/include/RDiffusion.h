//
//  RDiffusion.h
//  RDTerrain
//
//  Created by Robert Hodgin on 4/8/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#pragma once

#include "cinder/gl/Gl.h"
#include "cinder/gl/Fbo.h"
#include "cinder/gl/GlslProg.h"

class RDiffusion {
  public:
	RDiffusion();
	RDiffusion( int fboWidth, int fboHeight );
	void			reset();
	void			update( float dt, ci::gl::GlslProg *shader,
							const ci::gl::Texture &glowTex, 
						    const bool &isPressed, 
						    const ci::vec2 &mousePos,
						    float zoom );
	void			drawIntoHeightsFbo( ci::gl::GlslProg *shader, ci::vec3 scale );
	void			drawIntoNormalsFbo( ci::gl::GlslProg *shader );
	void			draw();
	void			setMode( int index );
	ci::Vec2i		toFboVec( const ci::vec3 &pos, float scale, float res );
	ci::gl::Texture getTexture();
	ci::gl::Texture getHeightsTexture();
	ci::gl::Texture getNormalsTexture();
	
	int					mFboWidth, mFboHeight;
	ci::vec2			mFboSize;
	ci::Area			mFboBounds;
	ci::vec2			mWindowSize;
	ci::Area			mWindowBounds;
	
	float				mXOffset, mYOffset;
	
	int					mThisFbo, mPrevFbo;
	ci::gl::Fbo			mFbo;
	ci::gl::Fbo			mFbos[2];
	ci::gl::Fbo			mHeightsFbo;
	ci::gl::Fbo			mNormalsFbo;

	float				mKernel[9];	
	ci::vec2			mOffset[9];

	float				mParamU;
	float				mParamV;
	float				mParamK;
	float				mParamF;
	float				mParamN;
	float				mParamWind;
};