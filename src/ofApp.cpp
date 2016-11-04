#include "ofApp.h"
#include "ofxSpout.h"
#include "ofxUDPManager.h"
//--------------------------------------------------------------
void ofApp::setup()
{
    ofSetFrameRate(120);
    
    /* Module Communication */
    udpConnectionBroadcast.Create();
    udpConnectionBroadcast.SetNonBlocking(true);
    udpConnectionBroadcast.Bind(6000);
    udpConnectionBroadcast.Connect("192.168.2.255", 8888);
    udpConnectionBroadcast.SetEnableBroadcast(true);
    std::string messageSent = "**";
    udpConnectionBroadcast.Send(messageSent.c_str(), messageSent.size());
    ofSleepMillis(100);
    char udpMessage[1000];
    auto ret = udpConnectionBroadcast.Receive(udpMessage, 1000);
    while (udpMessage[0] != '\0') {
        int num = ((int)udpMessage[0]) - 101;
        if (num < 7) {
            ofxUDPManager udpConnectionRx;
            moduleConnectionsZone2[((int)udpMessage[0]) - 101] = udpConnectionRx;
            moduleConnectionsZone2[((int)udpMessage[0]) - 101].Create();
            moduleConnectionsZone2[((int)udpMessage[0]) - 101].SetNonBlocking(true);
            moduleConnectionsZone2[((int)udpMessage[0]) - 101].Bind(6000);
            string ip = "192.168.2." + ofToString((int)udpMessage[0]);
            moduleConnectionsZone2[((int)udpMessage[0]) - 101].Connect(ip.c_str(), 8888);
            moduleConnectionsZone2[((int)udpMessage[0]) - 101].SetEnableBroadcast(false);
            std::fill_n(udpMessage, 1000, 0);
            auto ret = udpConnectionBroadcast.Receive(udpMessage, 1000);
        } 
        cout << "" << endl;
    }
    ofSleepMillis(1000);

    /* SPOUT Stuff */
	ofxSpout::init("", receiver, ofGetWidth(), ofGetHeight(), false);
}

void ofApp::update() 
{
	ofxSpout::initReceiver(receiver);
	ofxSpout::receiveTexture(receiver);

	/* Module Communciation */
	// if ( testImage2.isAllocated()) {
 //        ofPixels pix;
 //        testImage2.draw(0, 24);
 //        pix = testImage2.getPixels();
 //        for (int i = 0; i < 288; i++) {
 //            pixelsToSendZone2[0][3*i] = pix[panel1Z2[i]+0];
 //            pixelsToSendZone2[0][3*i+1] = pix[panel1Z2[i]+1];
 //            pixelsToSendZone2[0][3*i+2] = pix[panel1Z2[i]+2];
 //        }
 //        for (int i = 0; i < 288; i++) {
 //            pixelsToSendZone2[1][3*i] = pix[panel2Z2[i]+0];
 //            pixelsToSendZone2[1][3*i+1] = pix[panel2Z2[i]+1];
 //            pixelsToSendZone2[1][3*i+2] = pix[panel2Z2[i]+2];

 //        }
 //        for (int i = 0; i < 288; i++) {
 //            pixelsToSendZone2[2][3*i] = pix[panel3Z2[i]+0];
 //            pixelsToSendZone2[2][3*i+1] = pix[panel3Z2[i]+1];
 //            pixelsToSendZone2[2][3*i+2] = pix[panel3Z2[i]+2];

 //        }
 //        for (int i = 0; i < 288; i++) {
 //            pixelsToSendZone2[3][3*i] = pix[panel4Z2[i]+0];
 //            pixelsToSendZone2[3][3*i+1] = pix[panel4Z2[i]+1];
 //            pixelsToSendZone2[3][3*i+2] = pix[panel4Z2[i]+2];

 //        }
 //        for (int i = 0; i < 288; i++) {

 //            pixelsToSendZone2[4][3*i] = pix[panel5Z2[i]+0];
 //            pixelsToSendZone2[4][3*i+1] = pix[panel5Z2[i]+1];
 //            pixelsToSendZone2[4][3*i+2] = pix[panel5Z2[i]+2];

 //        }
 //        for (int i = 0; i < 288; i++) {
 //            pixelsToSendZone2[5][3*i] = pix[panel6Z2[i]+0];
 //            pixelsToSendZone2[5][3*i+1] = pix[panel6Z2[i]+1];
 //            pixelsToSendZone2[5][3*i+2] = pix[panel6Z2[i]+2];
 //        }
 //        for (int i = 0; i < 288; i++) {
 //            pixelsToSendZone2[6][3*i] = pix[panel7Z2[i]+0];
 //            pixelsToSendZone2[6][3*i+1] = pix[panel7Z2[i]+1];
 //            pixelsToSendZone2[6][3*i+2] = pix[panel7Z2[i]+2];
 //        }
 //        for (int panel = 0; panel < 7; panel++) {
 //            moduleConnectionsZone2[panel].Send(pixelsToSendZone2[panel], 864);
 //            ofSleepMillis(1);
 //        }
 //        for (int panel = 0; panel < 7; panel++) {
 //            moduleConnectionsZone2[panel].Send("*", 1);
 //        }
 //    }
}

//--------------------------------------------------------------
void ofApp::draw()
{

	receiver.draw(0, 0);

	ofDrawBitmapStringHighlight("ofxSpout receiver example", ofGetWidth() - 220, ofGetHeight() - 20);
}

//--------------------------------------------------------------
void ofApp::exit()
{
	// exit spout
	ofxSpout::exit();
}
