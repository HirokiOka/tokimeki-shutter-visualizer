#pragma once

#include "ofMain.h"
#include "ofxDatGui.h"

class ofApp : public ofBaseApp{
	public:
		void setup();
		void update();
		void draw();
		
		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y);
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
    void onButtonEvent(ofxDatGuiButtonEvent e);

    const int logo_width = 640;
    const int logo_height = 160;
    const int x_offset = logo_width / 2;
    const int y_offset = logo_height / 2;
    const int button_y_padding = 100;
    int app_state = 0;


    ofImage logo_image;
    ofImage kobe_image;
    ofTrueTypeFont verdana;
    ofxDatGuiComponent* component;
    vector<ofxDatGuiComponent*> components;
    vector<ofxDatGuiComponent*> plotter_components;
    ofxDatGuiValuePlotter* plotters[8];
    ofxDatGuiButton* myButton;
    ofxDatGuiButton* returnButton;

    ofSoundPlayer button_sound;
    ofSoundPlayer ok_sound;

    ofVideoGrabber vidGrabber;
    int camWidth = 640;
    int camHeight = 480;
};
