#pragma once

#ifndef _HELPERCONTROL
#define _HELPERCONTROL

#include "ofTypes.h"

class sfHelperControl {
	
public:
	
	void setLoc(ofPoint loc);
	void setClickLoc(ofPoint loc);

	ofPoint loc;
	ofPoint offset;
};

#endif