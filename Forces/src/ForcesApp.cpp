#include "cinder/app/App.h"
#include "cinder/gl/gl.h"
#include "cinder/gl/Texture.h"
#include "cinder/Text.h"
#include "cinder/gl/Fbo.h"
#include "cinder/gl/Vbo.h"
#include "cinder/gl/GlslProg.h"
#include "cinder/ImageIo.h"
#include "cinder/Utilities.h"
#include "cinder/Camera.h"
#include "cinder/Rand.h"
#include "cinder/TriMesh.h"
#include "Resources.h"
#include "CubeMap.h"
#include "Controller.h"
#include "Room.h"
#include "SpringCam.h"

using namespace ci;
using namespace ci::app;
using namespace std;

#define APP_WIDTH		1280
#define APP_HEIGHT		720
#define ROOM_FBO_RES	2
#define MAX_PARTICLES	8

class ForcesApp : public App {
  public:
	virtual void	prepareSettings( Settings *settings );
	virtual void	setup();
	virtual void	mouseDown( MouseEvent event );
	virtual void	mouseUp( MouseEvent event );
	virtual void	mouseMove( MouseEvent event );
	virtual void	mouseDrag( MouseEvent event );
	virtual void	mouseWheel( MouseEvent event );
	virtual void	keyDown( KeyEvent event );
	virtual void	update();
	void			drawIntoRoomFbo();
	void			drawIntoLightsFbo();
	virtual void	draw();
	void			drawInfoPanel();
	
	// CAMERA
	SpringCam			mSpringCam;
	
	// SHADERS
	gl::GlslProg		mRoomShader;
	gl::GlslProg		mShardShader;
	gl::GlslProg		mFieldShader;
	
	// TEXTURES
	gl::Texture			mIconTex;
	gl::Texture			mGlowTex;
	gl::Texture			mNebulaTex;
	gl::Texture			mCoronaTex;
	CubeMap				mCubeMap;
	
	// ROOM
	Room				mRoom;
	gl::Fbo				mRoomFbo;
	
	// LANTERNS (point lights)
	gl::Fbo				mLightsFbo;
	
	// CONTROLLER
	Controller			mController;

	
	// MOUSE
	vec2				mMousePos, mMouseDownPos, mMouseOffset;
	bool				mMousePressed;
	
	bool				mSaveFrames;
	int					mNumSavedFrames;
};

void ForcesApp::prepareSettings( Settings *settings )
{
	settings->setWindowSize( APP_WIDTH, APP_HEIGHT );
	settings->setBorderless();
}

void ForcesApp::setup()
{	
	// CAMERA	
	mSpringCam		= SpringCam( -450.0f, getWindowAspectRatio() );
	
	// FBOS
	gl::Fbo::Format mRgba16Format;
	mRgba16Format.setColorInternalFormat( GL_RGBA16F_ARB );
	mRgba16Format.setMinFilter( GL_NEAREST );
	mRgba16Format.setMagFilter( GL_NEAREST );
	mLightsFbo			= gl::Fbo::create( MAX_PARTICLES, 2, mRgba16Format );
	
	
	// LOAD SHADERS
	try {
		mRoomShader		= gl::GlslProg::create( loadResource( "room.vert" ), loadResource( "room.frag" ) );
		mShardShader	= gl::GlslProg::create( loadResource( "shard.vert" ), loadResource( "shard.frag" ) );
		mFieldShader	= gl::GlslProg::create( loadResource( "passThru.vert" ), loadResource( "field.frag" ) );
	} catch( gl::GlslProgCompileExc e ) {
		console() << e.what() << std::endl;
		quit();
	}
	
	// TEXTURE FORMAT
	gl::Texture::Format mipFmt;
    mipFmt.enableMipmapping( true );
    mipFmt.setMinFilter( GL_LINEAR_MIPMAP_LINEAR );    
    mipFmt.setMagFilter( GL_LINEAR );
	
	// LOAD TEXTURES
	mGlowTex		= gl::Texture::create( loadImage( loadResource( "glow.png" ) ), mipFmt );
	mNebulaTex		= gl::Texture::create( loadImage( loadResource( "nebula.png" ) ), mipFmt );
	mCoronaTex		= gl::Texture::create( loadImage( loadResource( "corona.png" ) ), mipFmt );
	mIconTex		= gl::Texture::create( loadImage( loadResource( "iconForces.png" ) ), mipFmt );
	mCubeMap		= CubeMap( GLsizei(512), GLsizei(512),
							   Surface8u( loadImage( loadResource( RES_CUBE1_ID ) ) ),
							   Surface8u( loadImage( loadResource( RES_CUBE2_ID ) ) ),
							   Surface8u( loadImage( loadResource( RES_CUBE3_ID ) ) ),
							   Surface8u( loadImage( loadResource( RES_CUBE4_ID ) ) ),
							   Surface8u( loadImage( loadResource( RES_CUBE5_ID ) ) ),
							   Surface8u( loadImage( loadResource( RES_CUBE6_ID ) ) )
							   );
	
	// ROOM
	gl::Fbo::Format roomFormat;
	roomFormat.setColorInternalFormat( GL_RGB );
	mRoomFbo			= gl::Fbo::create( APP_WIDTH/ROOM_FBO_RES, APP_HEIGHT/ROOM_FBO_RES, roomFormat );
	bool isPowerOn		= false;
	bool isGravityOn	= true;
	mRoom				= Room( vec3( 350.0f, 200.0f, 350.0f ), isPowerOn, isGravityOn );	
	mRoom.init();

	// MOUSE
	mMousePos			= vec2();
	mMouseDownPos		= vec2();
	mMouseOffset		= vec2();
	mMousePressed		= false;
	
	// CONTROLLER
	mController.init( &mRoom, MAX_PARTICLES );
	
	mSaveFrames			= false;
	mNumSavedFrames		= 0;
	
//	// PARAMS
//	mShowParams		= false;
//	mParams			= params::InterfaceGl( "Flocking", ivec2( 200, 150 ) );
//	mParams.addParam( "Time Multi", &mTimeMulti, "min=0.0 max=180.0 step=1.0 keyIncr=t keyDecr=T" );
}

void ForcesApp::mouseDown( MouseEvent event )
{
	mMouseDownPos = event.getPos();
	mMousePressed = true;
	mMouseOffset = vec2();
//	mController.checkForParticleTouch( mMouseDownPos );
}

void ForcesApp::mouseUp( MouseEvent event )
{
	mMousePressed = false;
	mMouseOffset = vec2();
//	mController.releaseDraggedParticles();
}

void ForcesApp::mouseMove( MouseEvent event )
{
	mMousePos = event.getPos();
}

void ForcesApp::mouseDrag( MouseEvent event )
{
	mouseMove( event );
	mMouseOffset = ( mMousePos - mMouseDownPos ) * 0.4f;
}

void ForcesApp::mouseWheel( MouseEvent event )
{
	float dWheel	= event.getWheelIncrement();
	mRoom.adjustTimeMulti( dWheel );
}

void ForcesApp::keyDown( KeyEvent event )
{
	switch( event.getChar() ){
		case ' ':	mRoom.togglePower();			break;
		case 'c':	mController.clear();			break;
//		case 's':	mSaveFrames = !mSaveFrames;		break;
		default:									break;
	}
	
	switch( event.getCode() ){
		case KeyEvent::KEY_UP:		mSpringCam.setEye( mRoom.getCornerCeilingPos() );	break;
		case KeyEvent::KEY_DOWN:	mSpringCam.setEye( mRoom.getCornerFloorPos() );		break;
		case KeyEvent::KEY_RIGHT:	mSpringCam.resetEye();								break;
		case KeyEvent::KEY_LEFT:	mSpringCam.setEye( mRoom.getRightWallPos() );		break;
		default: break;
	}
}

void ForcesApp::update()
{	
	// CAMERA
	if( mMousePressed ) 
		mSpringCam.dragCam( ( mMouseOffset ) * 0.02f, ( mMouseOffset ).length() * 0.02f );
	mSpringCam.update( 0.5f );
	
	// ROOM
	mRoom.update( mSaveFrames );
	
	// CONTROLLER
	mController.dragParticle( mMousePos );
	mController.update( mSpringCam.getCam(), mRoom.mTimeAdjusted, mRoom.mTick );

	gl::disableAlphaBlending();
	gl::disableDepthRead();
	gl::disableDepthWrite();
	drawIntoLightsFbo();
	drawIntoRoomFbo();
}

void ForcesApp::drawIntoRoomFbo()
{
	mRoomFbo->bindFramebuffer();
	gl::clear( ColorA( 0.0f, 0.0f, 0.0f, 0.0f ), true );
	
	gl::setMatricesWindow( mRoomFbo->getSize(), false );
	gl::viewport( mRoomFbo->getBounds() );
	gl::disableAlphaBlending();
	gl::enable( GL_TEXTURE_2D );
	glEnable( GL_CULL_FACE );
	glCullFace( GL_BACK );
	mat4 m;
	m.setToIdentity();
	m.scale( mRoom.getDims() );
	
	mRoomShader->bind();
	mRoomShader->uniform( "mvpMatrix", mSpringCam.mMvpMatrix );
	mRoomShader->uniform( "matrix", m );
	mRoomShader->uniform( "eyePos", mSpringCam.mEye );
	mRoomShader->uniform( "roomDims", mRoom.getDims() );
	mRoomShader->uniform( "power", mRoom.getPower() );
	mRoomShader->uniform( "lightPower", mRoom.getLightPower() );
	mRoomShader->uniform( "timePer", mRoom.getTimePer() * 1.5f + 0.5f );
	mRoom.draw();
	mRoomShader->unbind();
	
	mRoomFbo->unbindFramebuffer();
	glDisable( GL_CULL_FACE );
}

void ForcesApp::draw()
{	
	gl::clear( ColorA( 0.1f, 0.1f, 0.1f, 0.0f ), true );
	
	gl::setMatricesWindow( getWindowSize(), false );
	gl::viewport( getWindowBounds() );

	gl::disableDepthRead();
	gl::disableDepthWrite();
	gl::enableAlphaBlending();
	gl::enable( GL_TEXTURE_2D );
	gl::color( ColorA( 1.0f, 1.0f, 1.0f, 1.0f ) );
	
	// DRAW ROOM FBO
	mRoomFbo->bindTexture();
	gl::drawSolidRect( getWindowBounds() );
	
	gl::disable( GL_TEXTURE_2D );
	gl::setMatrices( mSpringCam.getCam() );
	
	// DRAW INFO PANEL
	drawInfoPanel();
	
	gl::enableDepthRead();
	gl::disableDepthWrite();
	gl::enableAlphaBlending();
	
	// FIELD LINES
	mCubeMap.bind();
	mLightsFbo.bindTexture( 1 );
	mFieldShader.bind();
	mFieldShader.uniform( "cubeMap", 0 );
	mFieldShader.uniform( "lightsTex", 1 );
	mFieldShader.uniform( "att", 1.015f );
	mFieldShader.uniform( "numLights", (float)MAX_PARTICLES );
	mFieldShader.uniform( "invNumLights", 1.0f/(float)MAX_PARTICLES );
	mFieldShader.uniform( "invNumLightsHalf", 1.0f/(float)MAX_PARTICLES * 0.5f );
	mFieldShader.uniform( "mvpMatrix", mSpringCam.mMvpMatrix );
	mFieldShader.uniform( "power", mRoom.getPower() );
	mFieldShader.uniform( "eyePos", mSpringCam.getEye() );
	mController.drawFieldLines( &mFieldShader );
	mFieldShader.unbind();
	
	gl::enableDepthWrite();
	gl::color( ColorA( 1.0f, 1.0f, 1.0f, 1.0f ) );
	
	// SHARDS
	mCubeMap.bind();
	mLightsFbo.bindTexture( 1 );
	mShardShader.bind();
	mShardShader.uniform( "cubeMap", 0 );
	mShardShader.uniform( "lightsTex", 1 );
	mShardShader.uniform( "att", 1.015f );
	mShardShader.uniform( "numLights", (float)MAX_PARTICLES );
	mShardShader.uniform( "invNumLights", 1.0f/(float)MAX_PARTICLES );
	mShardShader.uniform( "invNumLightsHalf", 1.0f/(float)MAX_PARTICLES * 0.5f );
	mShardShader.uniform( "mvpMatrix", mSpringCam.mMvpMatrix );
	mShardShader.uniform( "power", mRoom.getPower() );
	mShardShader.uniform( "eyePos", mSpringCam.getEye() );
	mController.drawShards( &mShardShader );
	mShardShader.unbind();
	
//	// PARTICLE CENTER SPHERES
//	mController.draw();
	
	gl::disableDepthWrite();
	gl::enable( GL_TEXTURE_2D );
	
	// CORONAS
	mCoronaTex.bind();
	mController.drawCoronas( mSpringCam.mBbRight, mSpringCam.mBbUp );
	mController.drawGlobs( mSpringCam.mBbRight, mSpringCam.mBbUp );
	// GLOWS
	mGlowTex.bind();
	mController.drawGlows( mSpringCam.mBbRight, mSpringCam.mBbUp );
	
	// NEBULAS
	mNebulaTex.bind();
	mController.drawNebulas( mSpringCam.mBbRight, mSpringCam.mBbUp );
	
	
	gl::disableDepthRead();
	
	if( mSaveFrames ){
		writeImage( getHomeDirectory() + "Forces/" + toString( mNumSavedFrames ) + ".png", copyWindowSurface() );
		mNumSavedFrames ++;
	}
	
	if( getElapsedFrames()%60 == 59 ){
		console() << "FPS: " << getAverageFps() << std::endl;
	}
}

void ForcesApp::drawInfoPanel()
{
	gl::pushMatrices();
	gl::translate( mRoom.getDims() );
	gl::scale( vec3( -1.0f, -1.0f, 1.0f ) );
	gl::color( Color( 1.0f, 1.0f, 1.0f ) * ( 1.0 - mRoom.getPower() ) );
	gl::enableAlphaBlending();
	
	float iconWidth		= 50.0f;
	
	float X0			= 15.0f;
	float X1			= X0 + iconWidth;
	float Y0			= 300.0f;
	float Y1			= Y0 + iconWidth;
	
	// DRAW ICON
	float c = mRoom.getPower() * 0.5f + 0.5f;
	gl::color( ColorA( c, c, c, 0.5f ) );
	gl::draw( mIconTex, Rectf( X0, Y0, X1, Y1 ) );
	
	c = mRoom.getPower();
	gl::color( ColorA( c, c, c, 0.5f ) );
	gl::disable( GL_TEXTURE_2D );
	
	// DRAW TIME BAR
	float timePer		= mRoom.getTimePer();
	gl::drawSolidRect( Rectf( vec2( X0, Y1 + 2.0f ), vec2( X0 + timePer * ( iconWidth ), Y1 + 2.0f + 4.0f ) ) );
	
	// DRAW FPS BAR
	float fpsPer		= getAverageFps()/60.0f;
	gl::drawSolidRect( Rectf( vec2( X0, Y1 + 4.0f + 4.0f ), vec2( X0 + fpsPer * ( iconWidth ), Y1 + 4.0f + 6.0f ) ) );
	
	
	gl::popMatrices();
}

// HOLDS DATA FOR LANTERNS AND PREDATORS
void ForcesApp::drawIntoLightsFbo()
{
	Surface32f lightsSurface( mLightsFbo.getTexture() );
	Surface32f::Iter it = lightsSurface.getIter();
	while( it.line() ){
		while( it.pixel() ){
			int index = it.x();
			
			if( it.y() == 0 ){ // set light position
				if( index < (int)mController.mParticles.size() ){
					it.r() = mController.mParticles[index].mPos.x;
					it.g() = mController.mParticles[index].mPos.y;
					it.b() = mController.mParticles[index].mPos.z;
					it.a() = mController.mParticles[index].mRadius;
				} else { // if the light shouldnt exist, put it way out there
					it.r() = 0.0f;
					it.g() = 0.0f;
					it.b() = 0.0f;
					it.a() = 1.0f;
				}
			} else {	// set light color
				if( index < (int)mController.mParticles.size() ){
					it.r() = mController.mParticles[index].mColor;
					it.g() = 0.0f;
					it.b() = 0.0f;
					it.a() = 1.0f;
				} else { 
					it.r() = 0.0f;
					it.g() = 0.0f;
					it.b() = 0.0f;
					it.a() = 1.0f;
				}
			}
		}
	}
	
	mLightsFbo.bindFramebuffer();
	gl::setMatricesWindow( mLightsFbo.getSize(), false );
	gl::viewport( mLightsFbo.getBounds() );
	gl::draw( gl::Texture::create( lightsSurface ) );
	mLightsFbo.unbindFramebuffer();
}


CINDER_APP_BASIC( ForcesApp, RendererGl )
