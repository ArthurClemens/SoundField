#pragma once

#ifndef _SOUND
#define _SOUND

#include "ofSoundPlayer.h"
#include "ofxXmlSettings.h"
#include "ofxXmlSerializable.h"

class sfSoundPlayer : public ofSoundPlayer, public ofxXmlSerializable {
	
public:
	string filePath;
	string printFileName;
	bool doStream;
	bool isPaused;
	
	typedef ofSoundPlayer super;
	
	sfSoundPlayer(string filePath = "");
	~sfSoundPlayer();
	sfSoundPlayer* init(string filePath = "");
	void setFilePath(string filePath);
	void loadSound();
	void pauseSound(bool state);
	string getPrintFileName();
	
private:
	void _xmlWrite(ofxXmlSettings& xml);
	void _xmlRead(ofxXmlSettings& xml);
	string _generatePrintFileName();

};

#endif