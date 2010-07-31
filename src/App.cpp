#include "App.h"

bool sortSoundShapeByZIndex(sfSound* a, sfSound* b) {
	return a->getZIndex() < b->getZIndex();
}

App::App() {
	ofxXmlSerializable::setup("App");
	sfAppSettings::init();
}

void App::setup(){
	ofSoundSetVolume(1.0);
	ofSetFrameRate(sfAppSettings::getIntValue("APP_FRAMERATE"));
	ofEnableSmoothing();
	ofEnableAlphaBlending();
	if (sfAppSettings::getIntValue("APP_DEBUG")) {
		TTF.loadFont(sfAppSettings::getStringValue("APP_FONT_PATH"), sfAppSettings::getFloatValue("APP_FONT_SIZE"));
	}
	
	mWalker = new sfWalker();
	mDraggedSound = NULL;
	mIsDragging = 0;
	mIsFullScreen = 0;
	mWindowScale = 1.0;
	
	bool success = _readSettings();
	if (!success) {
		_setScreenSize(sfAppSettings::getIntValue("APP_DEFAULT_WINDOW_WIDTH"), sfAppSettings::getIntValue("APP_DEFAULT_WINDOW_HEIGHT"));
		_loadSoundsFromDirectory();
	}
	_setSettingsBackgroundColor();
}

void App::_setSettingsBackgroundColor() {
	sfColor* c = new sfColor( sfAppSettings::getColorValue("APP_COLOR_BACKGROUND") );
	ofBackground(c->r, c->g, c->b);
}

/**
Sets the screen size based on input params, but not larger than the current screen.
*/
void App::_setScreenSize(int w, int h) {
	int windowWidth = (w > ofGetScreenWidth()) ? ofGetScreenWidth() : w;
	int windowHeight = (h > ofGetScreenHeight()) ? ofGetScreenHeight() : h;
	ofSetWindowShape(windowWidth, windowHeight);
	windowResized(windowWidth, windowHeight);
}

void App::_loadSoundsFromDirectory() {
	ofxDirList DIR;
	
	DIR.allowExt("mp3");
	DIR.allowExt("mp2");
	DIR.allowExt("wav");
	DIR.allowExt("ogg");
	DIR.allowExt("raw");
	DIR.allowExt("aif");
	DIR.allowExt("aiff");
	
    int fileCount = DIR.listDir("sounds");
	
	vector <string> paths;
    for(int i = 0; i < fileCount; i++){
		paths.push_back(DIR.getPath(i));
    }
	
	sort(paths.begin(), paths.end());
	fileCount = paths.size();
	for(int i = 0; i < fileCount; i++){
		mSounds.push_back( new sfSound(paths[i], i) );
    }
}
	
void App::draw() {
	int numSounds = mSounds.size();
	for (int i=0; i<numSounds; ++i) {
		mSounds[i]->draw(mWindowScale);
	}
	mWalker->draw(mWindowScale);
	
	if (sfAppSettings::getIntValue("APP_DEBUG")) {
		sfColor *c = new sfColor( sfAppSettings::getColorValue("APP_DEBUG_COLOR"));
		ofSetColor( c->r, c->g, c->b, c->a );
		string fpsStr = "frame rate: " + ofToString(ofGetFrameRate(), 2);
		TTF.drawString(fpsStr, 20, 10);
		for (int i=0; i<numSounds; ++i) {
			mSounds[i]->debugDrawDistance(mWalker->getLoc());
		}
	}
}

void App::update() {
	_updateWalkerWithMouseLoc();
}

/*
void App::keyPressed(int key){

}
void App::keyReleased(int key){

}
*/

void App::mouseMoved(int inX, int inY ) {
	_setMouseLoc(inX, inY);
	_updateWithMouseLoc();
}

void App::_setMouseLoc(int inX, int inY ) {
	mMouseLoc = ofPoint((float)inX, (float)inY);
	mMouseLoc /= mWindowScale;
}

void App::mouseDragged(int inX, int inY, int button) {
	if (button == 0) {
		mIsDragging = 1;
		_setMouseLoc(inX, inY);
		if (mDraggedSound) {
			_updateWalkerWithMouseLoc();
			mDraggedSound->updateWithMouseLoc(mMouseLoc, mIsDragging);
		} else {
			mousePressed(inX, inY, button);
			_updateWalkerWithMouseLoc();
			_updateWithMouseLoc();
		}
	}
}

void App::mousePressed(int inX, int inY, int button) {
	if (button == 0) {
		_setMouseLoc(inX, inY);
		int numSounds = mSounds.size();
		sfSound* ss = NULL;
		for (int i=0; i<numSounds; ++i) {
			ss = mSounds[i]->handleMousePressed(mMouseLoc, mIsDragging);
			if (ss) {
				// search no longer
				break;	
			}
		}
		if (ss) {
			// let circles know drag will happen
			for (int i=0; i<numSounds; ++i) {
				mSounds[i]->notifyWillDrag();
			}
		}
		if (ss) {
			mDraggedSound = ss;
			
			// move item last so that drawing occurs last and the pressed sfSound appears on top
			sfSound* lastItem = mSounds.back();
			int lastZIndex = lastItem->getZIndex();
			lastItem->setZIndex( ss->getZIndex() );
			ss->setZIndex(lastZIndex);

			sort (mSounds.begin(), mSounds.end(), sortSoundShapeByZIndex);
		}
		if (!ss) {
			// not clicked on a sound, so anywhere else
			mWalker->handleMousePressed(mMouseLoc, mIsDragging);
		}
	}
}

void App::mouseReleased(int inX, int inY, int button) {
	if (button == 0) {
		mIsDragging = 0;
		_setMouseLoc(inX, inY);
		// stop dragging
		int numSounds = mSounds.size();
		sfSound* ss = NULL;
		for (int i=0; i<numSounds; ++i) {
			ss = mSounds[i]->handleMouseReleased();
			if (ss) {
				break;	
			}
		}
		if (ss) {
			mDraggedSound = NULL;
		}
		_updateWalkerWithMouseLoc();
		_updateWithMouseLoc();
	}
}

void App::keyPressed(int key) {
	if (key == 's' || key == 'S') {
		_saveSettings();
	} else if (key == 'f' || key == 'F') {
		_setFullScreen(!mIsFullScreen);
	}
	
}
void App::_setFullScreen(bool inState) {
	mIsFullScreen = inState;
	ofSetFullscreen(mIsFullScreen);
}
	
bool App::_readSettings() {
	ofxXmlSettings XML;
	bool success = XML.loadFile(SETTINGS_FILE_PATH);
	if (!success) return 0;
	xmlRead(XML);

	int numSounds = XML.getNumTags("sfSound");
	for (int i = 0; i < numSounds; i++) {
		sfSound* sound = new sfSound();
		sound->xmlRead(XML);
		mSounds.push_back(sound);
	}
	mWalker->xmlRead(XML);
	_setMouseLoc(mWalker->getLoc().x, mWalker->getLoc().y);
	return 1;	
}

void App::_saveSettings() {
	ofxXmlSettings XML;
	this->xmlWrite(XML);
	mWalker->xmlWrite(XML);
	
	int numSounds = mSounds.size();
	for (int i=0; i<numSounds; ++i) {
		mSounds[i]->xmlWrite(XML);
	}
	
	XML.saveFile(SETTINGS_FILE_PATH);
}

void App::_xmlWrite(ofxXmlSettings& xml) {
	xml.addValue("fullscreen", mIsFullScreen ? 1 : 0);
	xml.addValue("windowWidth", sfAppSettings::getIntValue("windowWidth", ofGetWidth()));
	xml.addValue("windowHeight", sfAppSettings::getIntValue("windowHeight", ofGetHeight()));
}

void App::_xmlRead(ofxXmlSettings& xml) {
	int isFullScreen = xml.getValue("fullscreen", mIsFullScreen);
	_setFullScreen( (bool)isFullScreen );
	int w = xml.getValue("windowWidth", sfAppSettings::getIntValue("APP_DEFAULT_WINDOW_WIDTH", ofGetWidth()));
	int h = xml.getValue("windowHeight", sfAppSettings::getIntValue("APP_DEFAULT_WINDOW_HEIGHT", ofGetHeight()));
	_setScreenSize(w, h);
}

void App::windowResized(int w, int h) {
	sfAppSettings::setIntValue("windowWidth", w);
	sfAppSettings::setIntValue("windowHeight", h);
	
	float currentRatio = (float)w / (float)h;
	float defaultRatio = (float)sfAppSettings::getIntValue("APP_DEFAULT_WINDOW_WIDTH") / (float)sfAppSettings::getIntValue("APP_DEFAULT_WINDOW_HEIGHT");
	if (currentRatio > defaultRatio) {
		mWindowScale = (float)h / sfAppSettings::getIntValue("APP_DEFAULT_WINDOW_HEIGHT");
	} else {
		mWindowScale = (float)w / sfAppSettings::getIntValue("APP_DEFAULT_WINDOW_WIDTH");
	}
	update();
}

void App::_updateWithMouseLoc() {
	int numSounds = mSounds.size();
	for (int i=0; i<numSounds; ++i) {
		mSounds[i]->updateWithMouseLoc(mMouseLoc, mIsDragging);
	}
}

void App::_updateWalkerWithMouseLoc() {
	mWalker->updateWithMouseLoc(mMouseLoc, mIsDragging);
	
	int numSounds = mSounds.size();
	for (int i=0; i<numSounds; ++i) {
		mSounds[i]->updateWithWalkerLoc(mWalker->getLoc(), mIsDragging);
	}
}
