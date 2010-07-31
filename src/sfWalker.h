#pragma once

#ifndef _WALKER
#define _WALKER

#define MASS	100
#define DAMPING .5

#include "ofTypes.h";
#include "sfAppSettings.h"
#include "sfInteractiveObject.h"
#include "sfPointUtils.h"
#include "ofxXmlSerializable.h"
#include "ofxXmlSettings.h"
#include "sfColor.h"

class sfWalker : public sfInteractiveObject, public ofxXmlSerializable {

public:

	sfWalker();
	sfWalker* init();
	void draw(float scale = 1.0);
	void updateWithMouseLoc(ofPoint mouseLoc, bool isDragging);
	sfWalker* handleMousePressed(ofPoint mouseLoc, bool isDragging);
	
private:
	typedef sfInteractiveObject super;

	ofPoint mMomentum;
	bool mPaused;
	
	void _xmlWrite(ofxXmlSettings& xml);
	void _xmlRead(ofxXmlSettings& xml);
};

#endif