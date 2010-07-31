#include "sfSoundPlayer.h"

sfSoundPlayer::sfSoundPlayer(string inFilePath) {
	ofxXmlSerializable::setup("sfSoundPlayer");
	init(inFilePath);
}

sfSoundPlayer* sfSoundPlayer::init(string inFilePath) {
	printFileName = "";
	doStream = 1;
	isPaused = 0;
	setMultiPlay(0);
	setVolume(0);
	setLoop(1);
	setFilePath(inFilePath);
	return this;
}

void sfSoundPlayer::setFilePath(string inFilePath) {
	if (inFilePath != "") {
		filePath = inFilePath;
		loadSound();
		play();
	}
}

sfSoundPlayer::~sfSoundPlayer() {
	super::unloadSound();
}

void sfSoundPlayer::loadSound() {
	super::loadSound(filePath, doStream);
}

void sfSoundPlayer::pauseSound(bool inState) {
	super::setPaused(inState);
	isPaused = inState;
}

void sfSoundPlayer::_xmlWrite(ofxXmlSettings& xml) {		
	xml.addValue("filePath", filePath);
	xml.addValue("streaming", doStream ? 1 : 0);
	xml.addValue("paused", isPaused ? 1 : 0);
	xml.addValue("position", getPosition());
}

void sfSoundPlayer::_xmlRead(ofxXmlSettings& xml) {
	doStream = xml.getValue("streaming", doStream);
	isPaused = xml.getValue("paused", isPaused);
	setPosition(xml.getValue("position", 0));
	setFilePath( xml.getValue("filePath", filePath) );
}

string sfSoundPlayer::getPrintFileName() {
	if (printFileName == "") {
		printFileName = _generatePrintFileName();
	}
	return printFileName;
}

string sfSoundPlayer::_generatePrintFileName() {
	string s = filePath;
    std::string::size_type prev_pos = 0, pos = 0;
    while ( (pos = s.find('/', pos)) != std::string::npos ) {
        std::string substring( s.substr(prev_pos, pos-prev_pos) );
        prev_pos = ++pos;
    }
    std::string substring( s.substr(prev_pos, pos-prev_pos) ); // Last word
	return substring;
}
