
#include "sfHelperControl.h"

void sfHelperControl::setLoc(ofPoint inLoc) {
	loc = inLoc;
}

void sfHelperControl::setClickLoc(ofPoint inClickLoc) {
	offset = inClickLoc - loc;
}
