#include "sfAppSettings.h"

/**
 
*/

void sfAppSettings::init() {
	mXML = new ofxXmlSettings();
	bool success = mXML->loadFile( SETTINGS_APP_FILE_PATH );
	if (!success) {
		cout << "unable to load " << SETTINGS_APP_FILE_PATH << " -- check data/ folder\n";
	}
}

string sfAppSettings::getStringValue(string inKey, string inDefaultValue) {
	if (!mSettings[inKey]) {
		string value = mXML->getValue(inKey, inDefaultValue);
		setStringValue(inKey, value);
	}
	string* ptr = (string*)mSettings[inKey];
	return *ptr;
}

float sfAppSettings::getFloatValue(string inKey, float inDefaultValue) {
	if (!mSettings[inKey]) {
		float value = mXML->getValue(inKey, inDefaultValue);
		setFloatValue(inKey, value);
	}
	float* ptr = (float*)mSettings[inKey];
	return *ptr;
}

int sfAppSettings::getIntValue(string inKey, int inDefaultValue) {
	if (!mSettings[inKey]) {
		int value = mXML->getValue(inKey, (int)inDefaultValue);
		setIntValue(inKey, value);
	}
	int* ptr = (int*)mSettings[inKey];
	return *ptr;
}

unsigned int sfAppSettings::getColorValue(string inKey, string inDefaultValue) {
	if (!mSettings[inKey]) {
		string value = mXML->getValue(inKey, inDefaultValue);
		setColorValue(inKey, value);
	}
	unsigned int* ptr = (unsigned int*)mSettings[inKey];
	return *ptr;
}

void sfAppSettings::setIntValue(string inKey, int inValue) {
	mSettings[inKey] = new int(inValue);
}

void sfAppSettings::setFloatValue(string inKey, float inValue) {
	mSettings[inKey] = new float(inValue);
}

void sfAppSettings::setStringValue(string inKey, string inValue) {
	mSettings[inKey] = new string(inValue);
}

void sfAppSettings::setColorValue(string inKey, string inValue) {
	unsigned int x;   
	std::stringstream ss;
	ss << std::hex << inValue;
	ss >> x;
	// output it as a signed type
	unsigned int value = static_cast<unsigned int>(x);
	unsigned int* ptr = new unsigned int(value);
	mSettings[inKey] = ptr;
}

map<string, const void*> sfAppSettings::mSettings;
ofxXmlSettings* sfAppSettings::mXML;

