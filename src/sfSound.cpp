
#include "sfSound.h"

sfSound::sfSound(string inFilePath, int inZIndex) {
	ofxXmlSerializable::setup("sfSound");
	init(inFilePath, inZIndex);
}

sfSound* sfSound::init(string inFilePath, int inZIndex) {
	super::init();

	TTF.loadFont(sfAppSettings::getStringValue("APP_FONT_PATH"), sfAppSettings::getFloatValue("APP_FONT_SIZE"));
	
	mHelperControlOuterCirle = new sfHelperControl();
	mHelperControlCenter = new sfHelperControl();
	mTimeLastClicked = 0;
	mSoundPlayer = NULL;
	
	if (inFilePath != "") {
		mSoundPlayer = new sfSoundPlayer(inFilePath);
	}
	mZIndex = inZIndex;
	
	// set default values
	float x = 40;
	float y = 30 + inZIndex * 50;
	mLoc = ofPoint(x, y);
	_setRadius(20);
	
	_setState(STATE_NEUTRAL);
	_setActiveState(STATE_NEUTRAL);
	
	return this;
}

void sfSound::exit() {
	delete mHelperControlCenter;
	delete mHelperControlOuterCirle;
}

void sfSound::draw(float inScale) {	
	_drawFilename(inScale);
	_drawCenter(inScale);
	if (mState == STATE_MOUSE_OVER_CENTER || mState == STATE_DRAG_CENTER_DOT) {
		_drawCenterGrabHelper(inScale);
    }
	_drawOuterCircle(inScale);
	if (mState == STATE_MOUSE_OVER_OUTER_LINE || mState == STATE_DRAG_OUTER_CIRCLE) {
		_drawOuterCircleGrabHelper(inScale);
    }
}

void sfSound::updateWithMouseLoc(ofPoint inMouseLoc, bool isDragging) {
	
	if (mState == STATE_DRAG_CENTER_DOT) {
		_dragCenter(inMouseLoc);
		return;
	}

	if (mState == STATE_DRAG_OUTER_CIRCLE) {
		_dragOuterCircle(inMouseLoc);
		return;
	}
	
	// reset
	_setState(STATE_NEUTRAL);
	
	ofPoint loc = inMouseLoc - mLoc;

	float distance = sfPointUtils::distance(loc);
	
	// Outer circle
    float outerCircleMinDist = mRadius - sfAppSettings::getFloatValue("PADDING_OUTER_CIRCLE_INSIDE_GRAB");
    float outerCircleMaxDist = mRadius + sfAppSettings::getFloatValue("PADDING_OUTER_CIRCLE_OUTSIDE_GRAB");
	
	if ( (distance > outerCircleMinDist) && (distance < outerCircleMaxDist) ) { 
		_updateControlOuterCircle(loc);
		_setState(STATE_MOUSE_OVER_OUTER_LINE);
	}
	
	// Center
    float centerCircleMaxDist = sfAppSettings::getFloatValue("PADDING_CENTER_OUTSIDE_GRAB");
    if (distance < centerCircleMaxDist) {
		_setState(STATE_MOUSE_OVER_CENTER);
    }
}

void sfSound::updateWithWalkerLoc(ofPoint inWalkerLoc, bool isDragging) {
	if (mActiveState == STATE_INACTIVE) return;
	
	ofPoint vector = inWalkerLoc - mLoc;
	float distance = sfPointUtils::distance( vector );
	if (distance > mRadius) {
		mSoundPlayer->setVolume(0);
		return;
	}
	float volume = (mRadius - distance) / mRadius;
	mSoundPlayer->setVolume( volume );
	
	float panAngle = HALF_PI + atan2(vector.x, vector.y);
	float pan = sfAppSettings::getFloatValue("SOUND_PAN_FACTOR") * distance/mRadius * cos(panAngle);
	mSoundPlayer->setPan(pan);	
}

sfSound* sfSound::handleMousePressed(ofPoint inMouseLoc, bool isDragging) {
	
	if (mState == STATE_MOUSE_OVER_CENTER) {
		this->_handleDoubleClick();
		_setState(STATE_DRAG_CENTER_DOT);
		mHelperControlCenter->setLoc( mLoc );
		mHelperControlCenter->setClickLoc( inMouseLoc );
		return this;
	}
	if (mState == STATE_MOUSE_OVER_OUTER_LINE) {
		_setState(STATE_DRAG_OUTER_CIRCLE);
		mHelperControlOuterCirle->setClickLoc( inMouseLoc );
		return this;
	}
	return NULL;
}	
	
void sfSound::notifyWillDrag() {
	if (mState != STATE_DRAG_CENTER_DOT && mState != STATE_DRAG_OUTER_CIRCLE) {
		_setState(STATE_NEUTRAL);
	}
}

sfSound* sfSound::handleMouseReleased() {
	if (mState == STATE_DRAG_CENTER_DOT || mState == STATE_DRAG_OUTER_CIRCLE) {
		// stop dragging
		_setState(STATE_NEUTRAL);
		return this;
	}
	return NULL;
}

/**
Used for drawing order. 
*/

int sfSound::getZIndex() const {
	return mZIndex;
}

void sfSound::setZIndex(int inIndex) {
	mZIndex = inIndex;
}

void sfSound::_xmlWrite(ofxXmlSettings& xml) {	
	xml.addValue("x", mLoc.x);
	xml.addValue("y", mLoc.y);
	xml.addValue("zIndex", mZIndex);
	xml.addValue("radius", mRadius);
	xml.addValue("state", (int)mState);
	xml.addValue("activeState", getActiveState());	
	
	mSoundPlayer->xmlWrite(xml);
}

void sfSound::_xmlRead(ofxXmlSettings& xml) {
	if (!mSoundPlayer) {
		mSoundPlayer = new sfSoundPlayer();
	}
	mSoundPlayer->xmlRead(xml);
	
	mLoc.x = xml.getValue("x", mLoc.x);
	mLoc.y = xml.getValue("y", mLoc.y);
	mZIndex = xml.getValue("zIndex", mZIndex);
	
	_setRadius( xml.getValue("radius", mRadius) );
	mState = xml.getValue("state", (int)mState);
		
	_setActiveState( xml.getValue("activeState", mActiveState) );
}

void sfSound::_dragCenter(ofPoint inLoc) {
	mLoc.set( inLoc.x - mHelperControlCenter->offset.x, inLoc.y - mHelperControlCenter->offset.y, 0 );
}

/**

Calculates and sets the new radius based on the dragging mouse position.

*/

void sfSound::_dragOuterCircle(ofPoint inMouseLoc) { 

    ofPoint mouseLoc = inMouseLoc - mLoc - mHelperControlOuterCirle->offset;
    float distance = sfPointUtils::distance(mouseLoc);
	_setRadius(distance);
    
    _updateControlOuterCircle(mouseLoc);
}

void sfSound::_setState(int inState) {
	mState = inState;
}

void sfSound::_setActiveState(int inState) {
	super::_setActiveState(inState);
	if (mSoundPlayer) {
		if (mActiveState == STATE_INACTIVE) {
			mSoundPlayer->pauseSound(1);	
		} else {
			mSoundPlayer->pauseSound(0);	
		}
	}
}

void sfSound::_setRadius(float inRadius) {
	mRadius = inRadius;
	if (mRadius < sfAppSettings::getFloatValue("MIN_RADIUS_OUTER_CIRCLE")) mRadius = sfAppSettings::getFloatValue("MIN_RADIUS_OUTER_CIRCLE");
}

/**
Draws the outer circle. 
*/

void sfSound::_drawOuterCircle(float inScale) {
	ofSetCircleResolution(sfAppSettings::getFloatValue("RESOLUTION_OUTER_CIRCLE_LINE"));
	ofSetLineWidth(sfAppSettings::getFloatValue("WIDTH_OUTER_CIRCLE_LINE"));
	
	float x = inScale * mLoc.x;
	float y = inScale * mLoc.y;
	float r = inScale * mRadius;
	
	// fill color based on volume
	if (mActiveState != STATE_INACTIVE) {
		sfColor *c;
		c = new sfColor( sfAppSettings::getColorValue("COLOR_OUTER_CIRCLE_FILL"));
		// adjust alpha
		c->a *= mSoundPlayer->volume;
		ofSetColor( c->r, c->g, c->b, c->a );
		ofFill();
		ofCircle(x, y, r);
	}

	_setCircleColor();
	ofNoFill();
	ofCircle(x, y, r);
}

void sfSound::_setCircleColor() {
	
	sfColor *c;
	if (mActiveState == STATE_INACTIVE) {
		c = new sfColor( sfAppSettings::getColorValue("COLOR_OUTER_CIRCLE_INACTIVE"));
	} else if (mState == STATE_DRAG_CENTER_DOT || mState == STATE_DRAG_OUTER_CIRCLE) {
		c = new sfColor( sfAppSettings::getColorValue("COLOR_OUTER_CIRCLE_ACTIVE"));
	} else if (mState == STATE_MOUSE_OVER_OUTER_LINE) {
		c = new sfColor( sfAppSettings::getColorValue("COLOR_OUTER_CIRCLE_ACTIVE"));
	} else if (mState == STATE_MOUSE_OVER_CENTER) {
		c = new sfColor( sfAppSettings::getColorValue("COLOR_OUTER_CIRCLE_ACTIVE"));
	} else {
		c = new sfColor( sfAppSettings::getColorValue("COLOR_OUTER_CIRCLE_DEFAULT"));
    }
	ofSetColor( c->r, c->g, c->b, c->a );
}

void sfSound::_drawCenter(float inScale) {

	ofSetCircleResolution(sfAppSettings::getFloatValue("RESOLUTION_CENTER_DOT_LINE"));
	ofSetLineWidth(sfAppSettings::getFloatValue("WIDTH_CENTER_DOT_LINE"));
	
	_setCircleColor();

	ofFill();
	ofCircle(inScale * mLoc.x, inScale * mLoc.y, sfAppSettings::getFloatValue("RADIUS_CENTER_DOT") - .5);
	
	ofNoFill();
	ofCircle(inScale * mLoc.x, inScale * mLoc.y, sfAppSettings::getFloatValue("RADIUS_CENTER_DOT"));
}
	
void sfSound::_drawCenterGrabHelper(float inScale) {
	ofSetCircleResolution(sfAppSettings::getFloatValue("RESOLUTION_CENTER_DOT_LINE"));
	ofSetLineWidth(sfAppSettings::getFloatValue("WIDTH_CENTER_DOT_LINE"));
	
	ofSetColor(sfAppSettings::getColorValue("COLOR_HELPER_CONTROL_FILL"));
	
	ofFill();
	ofCircle(inScale * mLoc.x, inScale * mLoc.y, sfAppSettings::getFloatValue("WIDTH_CENTER_GRAB_HELPER") - 1);
	
	ofNoFill();
	ofSetColor(sfAppSettings::getColorValue("COLOR_HELPER_CONTROL_OUTLINE"));

	ofCircle(inScale * mLoc.x, inScale * mLoc.y, sfAppSettings::getFloatValue("WIDTH_CENTER_GRAB_HELPER"));
}

void sfSound::_drawOuterCircleGrabHelper(float inScale) {
	ofSetCircleResolution(sfAppSettings::getFloatValue("RESOLUTION_OUTER_CIRCLE_GRAB_HELPER_LINE"));
	ofSetLineWidth(sfAppSettings::getFloatValue("WIDTH_OUTER_CIRCLE_GRAB_HELPER_LINE"));
	
	ofSetColor(sfAppSettings::getColorValue("COLOR_HELPER_CONTROL_FILL"));
	
	ofFill();
	ofCircle(inScale * mHelperControlOuterCirle->loc.x, inScale * mHelperControlOuterCirle->loc.y, sfAppSettings::getFloatValue("WIDTH_OUTER_CIRCLE_GRAB_HELPER") - 1);
	
	ofNoFill();
	ofSetColor(sfAppSettings::getColorValue("COLOR_HELPER_CONTROL_OUTLINE"));
	ofCircle(inScale * mHelperControlOuterCirle->loc.x, inScale * mHelperControlOuterCirle->loc.y, sfAppSettings::getFloatValue("WIDTH_OUTER_CIRCLE_GRAB_HELPER"));
}

void sfSound::_drawFilename(float inScale) {
	sfColor *c = new sfColor( sfAppSettings::getColorValue("COLOR_SOUND_NAME"));
	ofSetColor( c->r, c->g, c->b, c->a );	
	
	// round position to prevent blurry pixel font
	int x = round(inScale * (mLoc.x + sfAppSettings::getFloatValue("RADIUS_CENTER_DOT") + 6));
	int y = round(inScale * (mLoc.y + 1));
	TTF.drawString(mSoundPlayer->getPrintFileName(), x, y);
}

void sfSound::_updateControlOuterCircle (ofPoint inLoc) {
	float angle = HALF_PI - atan2(inLoc.x, inLoc.y);
	ofPoint loc = ofPoint(mLoc.x + mRadius * cos(angle), mLoc.y + mRadius * sin(angle));
	mHelperControlOuterCirle->setLoc( loc );
}

void sfSound::debugDrawDistance(ofPoint inWalkerLoc) {
	if (mSoundPlayer->volume == 0) {
		return;	
	}
	sfColor *c = new sfColor( sfAppSettings::getColorValue("APP_DEBUG_COLOR"));
	c->a *= (.5 + (.5 * mSoundPlayer->volume));
	ofSetLineWidth(.5 + mSoundPlayer->volume * 3);
	ofSetColor( c->r, c->g, c->b, c->a );
	ofLine(inWalkerLoc.x, inWalkerLoc.y, mLoc.x, mLoc.y);
	
	c = new sfColor( sfAppSettings::getColorValue("APP_DEBUG_COLOR"));
	ofSetLineWidth(4);
	ofLine(mLoc.x, mLoc.y, mLoc.x + mSoundPlayer->pan * mRadius, mLoc.y);
	
}