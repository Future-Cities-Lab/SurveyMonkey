#pragma once

#include "ofMain.h"
#include "ofxUDPManager.h"
#include "ofxJSON.h"

class ofApp : public ofBaseApp {
public:
	void setup();
	void update();
	void draw();
	void exit();
private:
	ofTexture receiver;
	ofxJSONElement result;
	ofxUDPManager udpConnectionBroadcast;
	
	ofxUDPManager moduleConnections[24];
	
	/*TEST*/
	//vector<ofxUDPManager> moduleConnections;
	
	vector<vector <char>> pixelsToSend;
	vector<vector <int> > panels;
};
