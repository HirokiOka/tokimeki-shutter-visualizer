#pragma once

#include "ofMain.h"
#include "ofxDatGui.h"
#include "ofxNetwork.h"

class ofApp : public ofBaseApp{
	public:
		void setup();
		void update();
		void draw();

    void drawStar(int x, int y, int r);
		
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
    const int button_width = 300;
    const int button_height = 80;
    const int button_y_padding = 100;
    const int camera_device_id = 1;
    const int camera_width = 240;
    const int camera_height = 360;
    const int plotter_width = 240;
    const int plotter_label_width = 80;
    const int threshold = 900;
    int app_state = 0;
    bool is_tokimeki = false;

    int button_labels_size, sensor_labels_size;

    ofImage logo_image;
    ofImage kobe_image;
    ofImage boy_image;
    vector<ofImage> photos;
    ofTrueTypeFont text_font;
    ofxDatGuiComponent* component;
    vector<ofxDatGuiComponent*> components;
    vector<ofxDatGuiComponent*> plotter_components;
    ofxDatGuiValuePlotter* plotters[8];
    ofxDatGuiButton* myButton;
    ofxDatGuiButton* returnButton;

    ofSoundPlayer button_sound;
    ofSoundPlayer ok_sound;
    ofSoundPlayer tokimeki_sound;
    ofVideoGrabber vidGrabber;

    ofxUDPManager udpConnection;
    ofSerial serial;
};
