#pragma once

#ifndef _SOUNDSHAPE
#define _SOUNDSHAPE

#include <cmath>
#include "ofMain.h"
#include "ofTypes.h"
#include "sfHelperControl.h"
#include "sfColor.h"
#include "sfAppSettings.h"
#include "sfPointUtils.h"
#include "sfSoundPlayer.h"
#include "sfInteractiveObject.h"
#include "ofxXmlSettings.h"
#include "ofxXmlSerializable.h"

class sfSound : public sfInteractiveObject, public ofxXmlSerializable {
	
public:
	
	sfSound(string filePath = "", int zIndex = 0);
	sfSound* init(string filePath, int zIndex);

	void setup();
	void exit();
	void update();
	void draw(float scale = 1.0);

	void updateWithMouseLoc(ofPoint mouseLoc, bool isDragging);
	void updateWithWalkerLoc(ofPoint walkerLoc, bool isDragging);
	sfSound* handleMousePressed(ofPoint mouseLoc, bool isDragging);
	sfSound* handleMouseReleased();
	void notifyWillDrag();
	int getZIndex() const;
	void setZIndex(int index);
	void debugDrawDistance(ofPoint vector);
	
private:

	typedef sfInteractiveObject super;
	
	float mRadius;
	int mZIndex;
	
	sfHelperControl* mHelperControlCenter;
	sfHelperControl* mHelperControlOuterCirle;
	sfSoundPlayer* mSoundPlayer;
	ofTrueTypeFont TTF;
	
	unsigned int mState;
	static const unsigned int STATE_MOUSE_OVER_OUTER_LINE = 0x03;
	static const unsigned int STATE_MOUSE_OVER_CENTER = 0x04;
	static const unsigned int STATE_MOUSE_OVER_PAN_CONTROL = 0x05;
	static const unsigned int STATE_DRAG_CENTER_DOT = 0x06;
	static const unsigned int STATE_DRAG_OUTER_CIRCLE = 0x07;
	
	void _setState(int state);
	void _setActiveState(int state);
	void _setRadius(float radius);
	void _drawOuterCircle(float scale);
	void _drawCenter(float scale);
	void _drawCenterGrabHelper(float scale);
	void _drawOuterCircleGrabHelper(float scale);
	void _setCircleColor();
	void _dragCenter(ofPoint loc);
	void _updateControlOuterCircle (ofPoint loc);
	void _dragOuterCircle(ofPoint mouseLoc);
	void _drawFilename(float scale);
	void _xmlWrite(ofxXmlSettings& xml);
	void _xmlRead(ofxXmlSettings& xml);
};

#endif
