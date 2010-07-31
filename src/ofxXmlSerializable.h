#pragma once

#ifndef _ofxXmlSerializable
#define _ofxXmlSerializable

#include "ofxXmlSettings.h"

class ofxXmlSerializable {
public:
	void xmlSave(string filename);
	void xmlWrite(ofxXmlSettings& xml);
	virtual void _xmlWrite(ofxXmlSettings& xml) = 0;
	
	void xmlLoad(string filename);
	void xmlRead(ofxXmlSettings& xml);
	virtual void _xmlRead(ofxXmlSettings& xml) = 0;
protected:
	string name;
	void setup(string name);
};

#endif