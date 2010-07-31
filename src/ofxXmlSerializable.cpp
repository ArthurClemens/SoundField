#include "ofxXmlSerializable.h"

void ofxXmlSerializable::setup(string name) {
	this->name = name;
}

void ofxXmlSerializable::xmlWrite(ofxXmlSettings& xml) {
	int nextWhich = xml.getNumTags(name);
	xml.addTag(name);
	xml.pushTag(name, nextWhich);
	_xmlWrite(xml);
	xml.popTag();
}

void ofxXmlSerializable::xmlSave(string filename) {
	ofxXmlSettings file;
	xmlWrite(file);
	file.saveFile(ofToDataPath(filename));
}

void ofxXmlSerializable::xmlRead(ofxXmlSettings& xml) {
	xml.pushTag(name);
	_xmlRead(xml);
	xml.popTag();
	xml.removeTag(name);
}

void ofxXmlSerializable::xmlLoad(string filename) {
	ofxXmlSettings file;
	file.loadFile(ofToDataPath(filename));
	xmlRead(file);
}