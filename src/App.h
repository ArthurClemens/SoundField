#pragma once

#ifndef _TEST_APP
#define _TEST_APP

#include <algorithm>
#include <string>
#include <map>
#include "ofMain.h"
#include "ofTypes.h"
#include "ofxDirList.h"
#include "sfSound.h"
#include "sfWalker.h"
#include "sfColor.h"
#include "ofxXmlSerializable.h"
#include "ofxXmlSettings.h"

#define		SETTINGS_FILE_PATH		"settings/SoundFieldSettings.xml"


class App : public ofBaseApp, public ofxXmlSerializable {

public:

	App();
	void setup();
	void draw();
	void update();

	// events
	void mouseMoved(int x, int y );
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);
	void windowResized(int w, int h);
	void keyPressed(int key);
	
/*

	void keyReleased(int key);
*/
	
private:
	void _setSettingsBackgroundColor();
	void _setScreenSize(int w, int h);
	void _updateWithMouseLoc();
	void _updateWalkerWithMouseLoc();
	void _loadSoundsFromDirectory();
	void _saveSettings();
	void _setFullScreen(bool state);
	bool _readSettings();
	void _xmlWrite(ofxXmlSettings& xml);
	void _xmlRead(ofxXmlSettings& xml);
	void _setMouseLoc(int x, int y);

	vector <sfSound*> mSounds;
	sfSound* mDraggedSound;
	sfWalker* mWalker;
	ofPoint mMouseLoc;
	bool mIsDragging;
	bool mIsFullScreen;
	ofTrueTypeFont TTF;
	float mWindowScale;
};

#endif

