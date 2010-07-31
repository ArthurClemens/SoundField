
#include "sfInteractiveObject.h"

sfInteractiveObject* sfInteractiveObject::init() {
	return this;
}

ofPoint sfInteractiveObject::getLoc() {
	return mLoc;	
}

int sfInteractiveObject::getActiveState() {
	return mActiveState;	
}

void sfInteractiveObject::updateWithMouseLoc(ofPoint inMouseLoc, bool isDragging) {}

sfInteractiveObject* sfInteractiveObject::handleMousePressed(ofPoint inMouseLoc, bool isDragging) {
	return this;
}

void sfInteractiveObject::_handleDoubleClick() {
	if (mTimeLastClicked) {
		int interval = ofGetElapsedTimeMillis() - mTimeLastClicked;
		if (interval < sfAppSettings::getIntValue("APP_INTERVAL_DOUBLE_CLICK")) {
			if (mActiveState != STATE_INACTIVE) {
				this->_setActiveState(STATE_INACTIVE);
			} else {
				this->_setActiveState(STATE_NEUTRAL);	
			}
			mTimeLastClicked = 0;
			return;
		}
	}
	mTimeLastClicked = ofGetElapsedTimeMillis();
}

void sfInteractiveObject::_setActiveState(int inState) {
	mActiveState = inState;
}
