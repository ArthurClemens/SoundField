#pragma once

#ifndef _LAYOUTSETTINGS
#define _LAYOUTSETTINGS

#define		SETTINGS_APP_FILE_PATH		"settings/AppSettings.xml"

#include <string>
#include <map>
#include "ofxXmlSettings.h"

class sfAppSettings {
	
public:
	static void init();
	
	// getters
	static string getStringValue(string key, string defaultValue = "");
	static float getFloatValue(string key, float defaultValue = 0.0);
	static int getIntValue(string key, int defaultValue = 0);
	static unsigned int getColorValue(string key, string defaultValue = "");
	
	// setters
	static void setIntValue(string key, int value);
	static void setFloatValue(string key, float value);
	static void setStringValue(string key, string value);
	static void setColorValue(string key, string value);
	
private:
	static map<string, const void*> mSettings;
	static ofxXmlSettings* mXML;
	
	// VALUES READ FROM CONFIG FILE
	
	// sound
//	int SOUND_MIN_FILESIZE_FOR_STREAMING
	
	// center dot
//	float RADIUS_CENTER_DOT;
//	float WIDTH_CENTER_GRAB_HELPER;
//	float PADDING_CENTER_OUTSIDE_GRAB;
//	float WIDTH_CENTER_DOT_LINE;
//	float RESOLUTION_CENTER_DOT_LINE;
	
	// outer circle
//	float WIDTH_OUTER_CIRCLE_LINE;
//	float RESOLUTION_OUTER_CIRCLE_LINE;
//	float MIN_RADIUS_OUTER_CIRCLE;
//	float PADDING_OUTER_CIRCLE_INSIDE_GRAB;
//	float PADDING_OUTER_CIRCLE_OUTSIDE_GRAB;

	// grab helper
//	float WIDTH_OUTER_CIRCLE_GRAB_HELPER;
//	float WIDTH_OUTER_CIRCLE_GRAB_HELPER_LINE;
//	float RESOLUTION_OUTER_CIRCLE_GRAB_HELPER_LINE;
//	int COLOR_HELPER_CONTROL_FILL;
//	int COLOR_HELPER_CONTROL_OUTLINE;	

	// circle colors	
//	int COLOR_BACKGROUND;
//	int COLOR_OUTER_CIRCLE_DEFAULT;
//	int COLOR_OUTER_CIRCLE_ACTIVE;
//  int COLOR_OUTER_CIRCLE_INACTIVE;
//	int COLOR_SOUND_NAME;
	
	// walker
//	float RADIUS_WALKER;
//	float RADIUS_WALKER_CENTER;
//	int COLOR_WALKER_OUTER_CIRCLE;
//	int COLOR_WALKER_OUTER_CIRCLE_INACTIVE;
//	int COLOR_WALKER_CENTER;
//	int COLOR_WALKER_CENTER_INACTIVE;
	
	// interface
//	string APP_FONT_PATH;
//	float APP_FONT_SIZE;
//	static int APP_INTERVAL_DOUBLE_CLICK;
//	static int APP_FRAMERATE;
	
};

#endif