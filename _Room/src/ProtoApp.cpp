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
#include "Resources.h"
#include "cinder/app/RendererGl.h"
#include "Controller.h"
#include "Room.h"
#include "SpringCam.h"

//#include "cinder/params/Params.h"

using namespace ci;
using namespace ci::app;
using namespace std;

#define APP_WIDTH		1280
#define APP_HEIGHT		720
#define ROOM_FBO_RES	2


class ProtoApp : public App {
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
	virtual void	draw();
	void			drawInfoPanel();
	
	// CAMERA
	SpringCam		mSpringCam;
	
	// SHADERS
	gl::GlslProgRef	mRoomShader;
	
	// TEXTURES
	gl::TextureRef		mIconTex;
	
	// ROOM
	Room			mRoom;
	gl::FboRef			mRoomFbo;
	
	// CONTROLLER
	Controller		mController;

	// MOUSE
	vec2			mMousePos, mMouseDownPos, mMouseOffset;
	bool			mMousePressed;
	
	bool			mSaveFrames;
	int				mNumSavedFrames;
	
//	// PARAMS
//	params::InterfaceGl	mParams;
//	bool				mShowParams;
};

void ProtoApp::prepareSettings( Settings *settings )
{
	settings->setWindowSize( APP_WIDTH, APP_HEIGHT );
	settings->setBorderless( true );
}

void ProtoApp::setup()
{	
    addAssetDirectory("../_Room/assets");
	// CAMERA	
	mSpringCam		= SpringCam( -450.0f, getWindowAspectRatio() );
	
	// LOAD SHADERS
	try {
		mRoomShader	= gl::GlslProg::create( loadAsset( "room.vert" ), loadAsset( "room.frag" ) );
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
	mIconTex			= gl::Texture::create( loadImage( loadAsset( "iconRoom.png" ) ), mipFmt );
	
	// ROOM
	gl::Fbo::Format roomFormat;
	//roomFormat.setColorInternalFormat( GL_RGB );
	mRoomFbo			= gl::Fbo::create( APP_WIDTH/ROOM_FBO_RES, APP_HEIGHT/ROOM_FBO_RES, roomFormat );
	bool isPowerOn		= true;
	bool isGravityOn	= true;
	mRoom				= Room( vec3( 350.0f, 200.0f, 350.0f ), isPowerOn, isGravityOn );	
	mRoom.init();
	
	// MOUSE
	mMousePos			= vec2();
	mMouseDownPos		= vec2();
	mMouseOffset		= vec2();
	mMousePressed		= false;

	// CONTROLLER
	mController.init( &mRoom );
	
	mSaveFrames		= false;
	mNumSavedFrames = 0;
	
//	// PARAMS
//	mShowParams		= false;
//	mParams			= params::InterfaceGl( "Flocking", Vec2i( 200, 150 ) );
//	mParams.addParam( "Time Multi", &mTimeMulti, "min=0.0 max=180.0 step=1.0 keyIncr=t keyDecr=T" );
}

void ProtoApp::mouseDown( MouseEvent event )
{
	mMouseDownPos = event.getPos();
	mMousePressed = true;
	mMouseOffset = vec2();
}

void ProtoApp::mouseUp( MouseEvent event )
{
	mMousePressed = false;
	mMouseOffset = vec2();
}

void ProtoApp::mouseMove( MouseEvent event )
{
	mMousePos = event.getPos();
}

void ProtoApp::mouseDrag( MouseEvent event )
{
	mouseMove( event );
	mMouseOffset = ( mMousePos - mMouseDownPos ) * 0.4f;
}

void ProtoApp::mouseWheel( MouseEvent event )
{
	float dWheel	= event.getWheelIncrement();
	mRoom.adjustTimeMulti( dWheel );
}

void ProtoApp::keyDown( KeyEvent event )
{
	switch( event.getChar() ){
		case ' ': mRoom.togglePower();			break;
		case 'g': mRoom.toggleGravity();		break;
		case 's': mSaveFrames = !mSaveFrames;	break;
		default:								break;
	}
	
	switch( event.getCode() ){
		case KeyEvent::KEY_UP:		mSpringCam.setEye( mRoom.getCornerCeilingPos() );	break;
		case KeyEvent::KEY_DOWN:	mSpringCam.setEye( mRoom.getCornerFloorPos() );		break;
		case KeyEvent::KEY_LEFT:	mSpringCam.setEye( mRoom.getLeftWallPos() );		break;
		case KeyEvent::KEY_RIGHT:	mSpringCam.resetEye();								break;
		default: break;
	}
}


void ProtoApp::update()
{	
	// ROOM
	mRoom.update( mSaveFrames );
	
	// CAMERA
	if( mMousePressed ) 
		mSpringCam.dragCam( ( mMouseOffset ) * 0.02f, ( mMouseOffset ).length() * 0.02f );
	mSpringCam.update( 0.5f );
	
	drawIntoRoomFbo();
}

void ProtoApp::drawIntoRoomFbo()
{
	mRoomFbo->bindFramebuffer();
	gl::clear( ColorA( 0.0f, 0.0f, 0.0f, 0.0f ), true );
	
	gl::setMatricesWindow( mRoomFbo->getSize(), false );
	gl::viewport( mRoomFbo->getSize() );
	gl::disableAlphaBlending();
	gl::enable( GL_TEXTURE_2D );
	glEnable( GL_CULL_FACE );
	glCullFace( GL_BACK );
    mat4 m = glm::scale(mRoom.getDims());
	
    gl::ScopedGlslProg scp(mRoomShader);
	mRoomShader->uniform( "mvpMatrix", mSpringCam.mMvpMatrix );
	mRoomShader->uniform( "mMatrix", m );
	mRoomShader->uniform( "eyePos", mSpringCam.mEye );
	mRoomShader->uniform( "roomDims", mRoom.getDims() );
	mRoomShader->uniform( "power", mRoom.getPower() );
	mRoomShader->uniform( "lightPower", mRoom.getLightPower() );
	mRoomShader->uniform( "timePer", mRoom.getTimePer() * 1.5f + 0.5f );
	mRoom.draw();
	
	mRoomFbo->unbindFramebuffer();
	glDisable( GL_CULL_FACE );
}

void ProtoApp::draw()
{
	gl::clear( ColorA( 0.1f, 0.1f, 0.1f, 0.0f ), true );
	
	// SET MATRICES TO WINDOW
	gl::setMatricesWindow( getWindowSize(), false );
	gl::viewport(getWindowSize() );

	gl::disableDepthRead();
	gl::disableDepthWrite();
	gl::enableAlphaBlending();
	gl::enable( GL_TEXTURE_2D );
	gl::color( ColorA( 1.0f, 1.0f, 1.0f, 1.0f ) );
	
	// DRAW ROOM FBO
	mRoomFbo->bindTexture();
	gl::drawSolidRect( getWindowBounds() );
	
	// SET MATRICES TO SPRING CAM
	gl::setMatrices( mSpringCam.getCam() );
	
	// DRAW INFO PANEL
	drawInfoPanel();
	
	// SAVE FRAMES
	if( mSaveFrames && mNumSavedFrames < 5000 ){
		writeImage( getHomeDirectory() / "Room" / toString( mNumSavedFrames ) / ".png", copyWindowSurface() );
		mNumSavedFrames ++;
	}

	// DRAW PARAMS WINDOW
//	if( mShowParams ){
//		gl::setMatricesWindow( getWindowSize() );
//		params::InterfaceGl::draw();
//	}
}

void ProtoApp::drawInfoPanel()
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

CINDER_APP( ProtoApp, RendererGl )
