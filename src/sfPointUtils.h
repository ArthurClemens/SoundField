#pragma once

#ifndef _SFPOINTUTILS
#define _SFPOINTUTILS

#include "ofTypes.h"

class sfPointUtils {
	
public:
	static float distance (ofPoint p) {
		return sqrt( p.x*p.x + p.y*p.y + p.z*p.z );
	}
};

#endif