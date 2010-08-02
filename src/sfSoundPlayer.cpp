#include "sfSoundPlayer.h"

sfSoundPlayer::sfSoundPlayer(string inFilePath) {
	ofxXmlSerializable::setup("sfSoundPlayer");
	init(inFilePath);
}

sfSoundPlayer* sfSoundPlayer::init(string inFilePath) {
	printFileName = "";
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
		// do not stream small files: they won't play
		int fileSize = _getFileSize(inFilePath);
		doStream = (fileSize < 1024 * sfAppSettings::getIntValue("SOUNDFILE_MIN_KBSIZE_FOR_STREAMING") ) ? 0 : 1;
		loadSound(filePath, doStream);
		play();
	}
}

int sfSoundPlayer::_getFileSize(string inFilePath) {
	struct stat S;
#ifdef WIN32 
	if ( -1 == _stat(inFilePath.c_str(), &S)) {
		printf("Error in _getFileSize!\n");
		return -1;
	}
#else
	if ( -1 == stat(inFilePath.c_str(), &S)) {
		printf("Error in _getFileSize!\n");
		return -1;
	}
#endif
	return S.st_size;
}

sfSoundPlayer::~sfSoundPlayer() {
	super::unloadSound();
}

void sfSoundPlayer::pauseSound(bool inState) {
	super::setPaused(inState);
	isPaused = inState;
}

void sfSoundPlayer::_xmlWrite(ofxXmlSettings& xml) {		
	xml.addValue("filePath", filePath);
	xml.addValue("paused", isPaused ? 1 : 0);
	xml.addValue("position", getPosition());
}

void sfSoundPlayer::_xmlRead(ofxXmlSettings& xml) {
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
