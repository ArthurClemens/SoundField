
#include "sfWalker.h"

sfWalker::sfWalker() {
	ofxXmlSerializable::setup("sfWalker");
	init();
}

sfWalker* sfWalker::init() {
	super::init();
	mLoc = NULL; // to prevent drawing if loc is not set
	return this;
}

void sfWalker::updateWithMouseLoc(ofPoint inMouseLoc, bool isDragging) {
	
	if (mActiveState == STATE_INACTIVE) return;

	ofPoint loc = inMouseLoc - mLoc;
	float dt = .2; // for framerate of 60

	ofPoint g = loc;
	ofPoint force = g * MASS;
	ofPoint velocity = mMomentum / MASS;
	velocity *= DAMPING;
	mLoc += g*dt*dt/2 + velocity * dt;
	velocity += g*dt;
	mMomentum = velocity * MASS;
}

void sfWalker::_xmlWrite(ofxXmlSettings& xml) {
	xml.addValue("x", mLoc.x);
	xml.addValue("y", mLoc.y);
	xml.addValue("activeState", getActiveState());
}

void sfWalker::_xmlRead(ofxXmlSettings& xml) {
	mLoc.x = xml.getValue("x", mLoc.x);
	mLoc.y = xml.getValue("y", mLoc.y);
	_setActiveState( xml.getValue("activeState", getActiveState()));
}

sfWalker* sfWalker::handleMousePressed(ofPoint inMouseLoc, bool isDragging) {
	/*
	 // only if we want to toggle when clicking on Walker itself
	ofPoint p = inMouseLoc - mLoc;
	float distance = sfPointUtils::distance(p);
	if (distance < sfAppSettings::getFloatValue("RADIUS_WALKER")) { 
		_handleDoubleClick();
	}
	 */
	if (!isDragging) {
		_handleDoubleClick();
	}
	return this;
}	

void sfWalker::draw(float inScale) {
	
	if (mLoc == NULL) return;
	
	ofEnableSmoothing();
	ofEnableAlphaBlending();
	
	sfColor *c;
	float x = inScale * mLoc.x;
	float y = inScale * mLoc.y;
	float r = sfAppSettings::getFloatValue("RADIUS_WALKER");
	float rc = sfAppSettings::getFloatValue("RADIUS_WALKER_CENTER");
	
	// outer circle
	c = new sfColor((mActiveState == STATE_INACTIVE) ? sfAppSettings::getColorValue("COLOR_WALKER_OUTER_CIRCLE_INACTIVE"): sfAppSettings::getColorValue("COLOR_WALKER_OUTER_CIRCLE"));
	ofSetColor( c->r, c->g, c->b, c->a );
	// smooth edges
	if (mActiveState != STATE_INACTIVE) {
		ofNoFill();
		ofSetLineWidth(1);
		ofCircle(x, y, r);
	}
	// fill
	ofFill();
	ofEnableAlphaBlending();
	ofCircle(x, y, r);

	
	// center
	ofFill();
	c = new sfColor((mActiveState == STATE_INACTIVE) ? sfAppSettings::getColorValue("COLOR_WALKER_CENTER_INACTIVE"): sfAppSettings::getColorValue("COLOR_WALKER_CENTER"));
	ofSetColor( c->r, c->g, c->b, c->a );
	ofCircle(x, y, rc);
	// smooth edges
	if (mActiveState != STATE_INACTIVE) {
		ofNoFill();
		ofSetLineWidth(1);
		ofCircle(x, y, rc);
	}
}