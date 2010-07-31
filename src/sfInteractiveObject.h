#pragma once

#ifndef _INTERACTIVE_OBJECT
#define _INTERACTIVE_OBJECT

#include "ofMain.h"
#include "ofTypes.h"
#include "sfAppSettings.h"

class sfInteractiveObject {

public:
	virtual sfInteractiveObject* init();
	virtual ofPoint getLoc();
	virtual int getActiveState();
	virtual void updateWithMouseLoc(ofPoint mouseLoc, bool isDragging);
	virtual sfInteractiveObject* handleMousePressed(ofPoint mouseLoc, bool isDragging);

protected:
	ofPoint mLoc;
	int mTimeLastClicked;
	
	int mActiveState;
	static const int STATE_NONE = 0x00;
	static const int STATE_INACTIVE = 0x01;
	static const int STATE_NEUTRAL = 0x02;
	
	virtual void _handleDoubleClick();
	virtual void _setActiveState(int inState);
	
};

#endif