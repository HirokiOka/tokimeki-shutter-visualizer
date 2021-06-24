#pragma once

#include "ofMain.h"
#include "ofxDatGui.h"
#include "ofxNetwork.h"

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

    const string photo_dir_path = "/Users/Hiroki/Downloads";
    const string path_to_logo = "tokimeki_shutter_logo.png";
    const string path_to_kobe = "kobe_univ.jpeg";
    const string path_to_boy = "camera_man_boy.png";
    const string path_to_button_sound = "button_sound.mp3";
    const string path_to_tokimeki_sound = "tokimeki.mp3";
    const string path_to_cancel_sound = "cancel.mp3";
    const string path_to_font = "ofxbraitsch/fonts/Verdana.ttf";

    const int logo_width = 640;
    const int logo_height = 160;
    const int x_offset = logo_width / 2;
    const int y_offset = logo_height / 2;

    const int button_width = 360;
    const int button_height = 80;
    const int button_y_padding = 100;

    const int photo_width = 240;
    const int photo_height = 240;

    const int camera_device_id = 0;
    const int camera_width = 240;
    const int camera_height = 360;

    const int plotter_width = 240;
    const int plotter_label_width = 80;
    const int sensor_min_value = 0;
    const int sensor_max_value = 250;
    const int threshold = 200;

    const int R_port = 50004;


    int app_state = 0;
    bool is_tokimeki = false;

    bool is_read_dir = false;

    int button_labels_size, sensor_labels_size;

    ofImage logo_image;
    ofImage kobe_image;
    ofImage boy_image;
    vector<ofImage> photos;
    ofTrueTypeFont text_font;
    ofxDatGuiComponent* component;
    vector<ofxDatGuiComponent*> components;
    vector<ofxDatGuiComponent*> plotter_components;
    vector<ofxDatGuiComponent*> no_cam_plotter_components;
    ofxDatGuiValuePlotter* plotters[8];
    ofxDatGuiValuePlotter* no_cam_plotters[8];
    ofxDatGuiButton* myButton;

    ofSoundPlayer button_sound;
    ofSoundPlayer ok_sound;
    ofSoundPlayer tokimeki_sound;
    ofSoundPlayer cancel_sound;
    ofVideoGrabber vidGrabber;

    ofxUDPManager udpConnection[7];

    void drawStar(int x, int y, int r);
    void drawTokimeki();
};
