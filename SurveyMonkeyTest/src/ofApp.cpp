#include "ofApp.h"
#include "ofxSpout.h"

void ofApp::setup()
{
	ofSetFrameRate(120);

	// OPEN THE JSON FILE
	std::string file = "newLeds.json";
	bool parsingSuccessful = result.open(file);
	if (parsingSuccessful) {
		cout << "LED CONFIG SUCCESSFULLY PARSED" << endl;
	} else {
		cout << "LED CONFIG FAILED" << endl;
	}

	// CREATE THE PANELS
	panels.resize(result["panels"].size());
	for (auto panel : result["panels"]) {
		vector<int> newPanel;
		newPanel.resize(panel["data"].size());

		for (int led = 0; led < panel["data"].size(); led++) {
			newPanel[led] = panel["data"][led].asInt();
		}
		cout << panel["position"].asInt() << endl;
		panels[panel["position"].asInt()-1] = newPanel;
	}

	for (int i = 0; i < panels.size(); i++) {
		vector<char> newPix;
		for (int j = 0; j < panels[i].size()*3; j++) {
			newPix.push_back('a');
		}
		pixelsToSend.push_back(newPix);
	}


	udpConnectionBroadcast.Create();
	udpConnectionBroadcast.SetNonBlocking(true);
	udpConnectionBroadcast.Bind(6000);
	udpConnectionBroadcast.Connect("192.168.1.255", 8888);
	udpConnectionBroadcast.SetEnableBroadcast(true);
	std::string messageSent = "**";
	udpConnectionBroadcast.Send(messageSent.c_str(), messageSent.size());
	ofSleepMillis(100);
	char udpMessage[1000];
	auto ret = udpConnectionBroadcast.Receive(udpMessage, 1000);
	//moduleConnections.reserve(panels.size());
	cout << "" << endl;
	cout << "LOOKING FOR MODULES" << endl;
	while (udpMessage[0] != '\0') {
		cout << "response" << endl;
		int num = ((int)udpMessage[0]) - 101;
		ofxUDPManager udpConnectionRx;
		moduleConnections[((int)udpMessage[0]) - 101] = udpConnectionRx;
		moduleConnections[((int)udpMessage[0]) - 101].Create();
		moduleConnections[((int)udpMessage[0]) - 101].SetNonBlocking(true);
		moduleConnections[((int)udpMessage[0]) - 101].Bind(6000);
		string ip = "192.168.1." + ofToString((int)udpMessage[0]);
		cout << ip << endl;
		moduleConnections[((int)udpMessage[0]) - 101].Connect(ip.c_str(), 8888);
		moduleConnections[((int)udpMessage[0]) - 101].SetEnableBroadcast(false);
		std::fill_n(udpMessage, 1000, 0);
		auto ret = udpConnectionBroadcast.Receive(udpMessage, 1000);
		cout << "" << endl;
	}

	ofSleepMillis(1000);

	ofxSpout::init("", receiver, ofGetWidth(), ofGetHeight(), false);
}

void ofApp::update()
{
	ofxSpout::initReceiver(receiver);
	ofxSpout::receiveTexture(receiver);
	if (receiver.isAllocated()) {
		ofPixels pix;
		receiver.readToPixels(pix);
		pix.setNumChannels(3);

		for (int panel = 0; panel < panels.size(); panel++) {
			vector<int> curPanel = panels[panel];
			for (int i = 0; i < curPanel.size(); i++) {
				pixelsToSend[panel][3 * i] = pix[3*curPanel[i] + 0];
				pixelsToSend[panel][3 * i + 1] = pix[3*curPanel[i] + 1];
				pixelsToSend[panel][3 * i + 2] = pix[3*curPanel[i] + 2];
			}
		}
		for (int panel = 0; panel < panels.size(); panel++) {
			char* pix = &pixelsToSend[panel][0];
			moduleConnections[panel].Send(pix, pixelsToSend[panel].size());
			ofSleepMillis(1);
		}
		for (int panel = 0; panel < panels.size(); panel++) {
			moduleConnections[panel].Send("*", 1);
		}
	}
}

//--------------------------------------------------------------
void ofApp::draw()
{
	receiver.draw(0, 0);
}

//--------------------------------------------------------------
void ofApp::exit()
{
	ofxSpout::exit();
}
