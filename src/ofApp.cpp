#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
  //左足ポート：50001 右足ポート：50002
  ofSetWindowTitle("TOKIMEKI Visualizer");
  ofSetFrameRate(30);

  ofDirectory dir("photos");
  dir.allowExt("jpg");
  dir.listDir();

  ofImage current_image;
  for (int i = 0; i < dir.size(); ++i) {
    current_image.load(dir.getPath(i));
    current_image.resize(200, 200);
    photos.push_back(current_image);
  }

  logo_image.load("tokimeki_shutter_logo.png");
  kobe_image.load("kobe_univ.jpeg");
  boy_image.load("camera_man_boy.png");
  button_sound.load("button_sound.mp3");
  tokimeki_sound.load("tokimeki.mp3");
  text_font.load("ofxbraitsch/fonts/Verdana.ttf", 32);

  udpConnection.Create();
  udpConnection.SetEnableBroadcast(true);
  //udpConnection.Bind(50008);
  udpConnection.BindMcast("10.32.130.100", 50008);
  udpConnection.SetNonBlocking(true);

  ofBackground(188, 226, 232);

  //vidGrabber.setVerbose(true);
  vidGrabber.setDeviceID(camera_device_id);
  vidGrabber.listDevices();
  vidGrabber.initGrabber(camera_width, camera_height);

  string button_labels[] = {"START", "GALLERY", "EXIT"};
  button_labels_size = sizeof(button_labels) / sizeof(button_labels[0]);
  for (int i = 0; i < button_labels_size; ++i) {
    string button_label = button_labels[i];
    myButton = new ofxDatGuiButton(button_label);
    myButton->onButtonEvent(this, &ofApp::onButtonEvent);
    myButton->setHeight(button_height);
    component = myButton;
    component->setLabelAlignment(ofxDatGuiAlignment::CENTER);
    component->setPosition(ofGetWidth() / 2 - 150, ofGetHeight() / 2 + button_y_padding * i);
    component->setLabelColor(ofColor(0));
    component->setBackgroundColor(ofColor(0, 200, 0));
    component->setWidth(button_width, 0);
    component->setBorderVisible(false);
    components.push_back(component);
  }

  returnButton = new ofxDatGuiButton("RETURN");
  returnButton->onButtonEvent(this, &ofApp::onButtonEvent);
  returnButton->setHeight(button_height);
  component = returnButton;
  component->setLabelAlignment(ofxDatGuiAlignment::CENTER);
  component->setPosition(ofGetWidth()*2/3, ofGetHeight()- 100);
  component->setLabelColor(ofColor(0));
  component->setBackgroundColor(ofColor(0, 200, 0));
  component->setWidth(button_width, 0);
  component->setBorderVisible(false);

  string sensor_labels[] = {"LFP1", "LFP2", "RFP1", "RFP2", "VERTICAL", "HORIZONTAL"};
  sensor_labels_size = sizeof(sensor_labels) / sizeof(sensor_labels[0]);
  for (int i = 0; i < sensor_labels_size; ++i) {
    string label = sensor_labels[i];
    plotters[i] = new ofxDatGuiValuePlotter(label, 0, 250);
    plotters[i]->setDrawMode(ofxDatGuiGraph::LINES);
    plotters[i]->setSpeed(3);
    component = plotters[i];
    component->setBackgroundColor(ofColor(0, 100, 0));
    int pos_x = 0;
    int pos_y = 0;

    if (i < 2) {
      //Left Foot Pressure
      pos_x = 320;
      pos_y = (i + 9) * 70;
      ofSetColor(0);
      ofDrawLine(pos_x, pos_y, ofGetWidth() / 3 - boy_image.getWidth() / 2, ofGetHeight()/ 2 - boy_image.getHeight() / 4);

    } else if (i < 4){
      //Right Foot Pressue
      pos_x = 20;
      pos_y = (i + 7) * 70;
      ofDrawLine(pos_x, pos_y, ofGetWidth() / 3 - boy_image.getWidth() / 2, ofGetHeight()/ 2 - boy_image.getHeight() / 4);

    } else {
      //Head
      pos_x = 20;
      pos_y = (i % 2) * 70;
      ofDrawLine(pos_x, pos_y, ofGetWidth() / 3 - boy_image.getWidth() / 2, ofGetHeight()/ 2 - boy_image.getHeight() / 4);
    }
    component->setPosition(pos_x, pos_y);
    component->setWidth(plotter_width, plotter_label_width);
    plotter_components.push_back(component);
  }

  /*
  ofTrueTypeFontSettings font_settings("HackGen-Regular.ttf", 32);
  font_settings.antialiased = true;
	font_settings.addRanges(ofAlphabet::Emoji);
  text_font.load(font_settings);
  */
}

//--------------------------------------------------------------
void ofApp::update(){

  if (app_state == 0) {
    //Menu
    for (int i = 0; i < components.size(); ++i) components[i]->update();

  } else if (app_state == 1) {
    //Visualizer
    int current_right_foot_r = 0;
    int current_right_foot_l = 0;
    int current_right_foot_b = 0;
    char udpMessage[100000];
    udpConnection.Receive(udpMessage,100000);
    string message=udpMessage;
    if(message!=""){
      for (int i = 0; i < 3; ++i) {
        if (i == 0){
          int row_r = ofBinaryToInt(ofToBinary(message[i]));
          current_right_foot_r = ofMap(row_r, 250, 0, 0, 250); 
        } else if (i == 1) { 
          int row_l = ofBinaryToInt(ofToBinary(message[i]));
          current_right_foot_l = ofMap(row_l, 250, 0, 0, 250); 
        } else if(i == 2){
          int row_b = ofBinaryToInt(ofToBinary(message[i]));
          current_right_foot_b = ofMap(row_b, 250, 0, 0, 250); 
        }
      }
    }
    vidGrabber.update();
    for (int i = 0; i < plotter_components.size(); ++i) {
      if (i != 2 && i != 3) {
        float current_value = ofRandom(0, 1000);
        plotters[i]->setValue(current_value);
      } else {
        if (i == 2) {
          plotters[i]->setValue(current_right_foot_r);
        } else if (i == 3) {
          plotters[i]->setValue(current_right_foot_b);
        }
      }
      plotter_components[i]->update();
    } 
    returnButton->update();

  } else if (app_state == 2) {
    //GALLERY
    returnButton->update();
  }
}

//--------------------------------------------------------------
void ofApp::draw(){
  int window_width = ofGetWidth();
  int window_height = ofGetHeight();

  //Menu
  if (app_state == 0) {
    ofSetColor(255);
    kobe_image.draw(0, 0, window_width, window_height);
    logo_image.draw(window_width / 2 - x_offset, window_height / 4 - y_offset, logo_width, logo_height);
    for (int i = 0; i < components.size(); ++i) components[i]->draw();
    
  //Visualizer
  } else if(app_state == 1) {
    ofBackground(188, 226, 232);
    if (is_tokimeki) {
      ofBackground(234, 145, 152);
      drawStar(100, 243,30);
      drawStar(220, 200, 60);
      drawStar(400, 20, 20);
      drawStar(500, 500, 30);
      drawStar(400, 200, 60);
      drawStar(500, 30, 40);
      drawStar(800, 50, 60);
      drawStar(800, 600, 60);
      if (ofGetElapsedTimeMillis() > 3000) is_tokimeki = false;
    }
    ofSetColor(255);
    boy_image.draw(window_width / 3 - boy_image.getWidth() / 2, window_height / 2 - boy_image.getHeight() / 4, boy_image.getWidth()/2, boy_image.getHeight()/2);
    ofSetColor(0);
    text_font.drawString("TOKIMEKI VISION", window_width*2/3 - 320 / 2, window_height / 6);
    ofSetColor(255);
    vidGrabber.draw(window_width/3, window_height/5);

    ofSetColor(100, 0, 0);
    ofDrawLine(20, 20, 280, 320);
    ofDrawLine(20, ofGetHeight(), 240, 480);
    ofDrawLine(400, ofGetHeight(), 310, 470);

    for (int i = 0; i < plotter_components.size(); ++i) {
      if (plotters[i]->getValue() > threshold) {
        plotter_components[i]->setBackgroundColor(ofColor(234, 145, 152));
       } else {
        plotter_components[i]->setBackgroundColor(ofColor(0, 100, 0));
       } 
        plotter_components[i]->draw();
    }
    returnButton->draw();


  //GALLERY
  } else if (app_state == 2) {
    ofSetColor(0);
    text_font.drawString("GALLERY MODE", 60, 60);
    ofSetColor(255);
    for (int i = 0; i < photos.size(); ++i) {
      if (i < 4) {
        photos[i].draw(i * 220 + 20, 80);
      } else {
        photos[i].draw((i%4) * 220 + 20, 300);
      }
    }
    returnButton->draw();
  }
}

void ofApp::drawStar(int x, int y, int r) {
  int vertex_num = 10;
  ofSetColor(255, 217, 0);
  ofPushMatrix();
  ofTranslate(x, y);
  //ofRotateRad(60);
  ofRotateRad(ofGetFrameNum() * 0.04);
  ofBeginShape();
  for (int i = 0; i < vertex_num; ++i) {
    int radius = (i % 2 == 0) ? r : r / 2;
    ofVertex(radius * cos(ofDegToRad(360 * i / vertex_num)), radius * sin(ofDegToRad(360 * i / vertex_num)));
  }
  ofEndShape(true);
  ofPopMatrix();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
  if (app_state == 1) {
    tokimeki_sound.play();
    is_tokimeki = !is_tokimeki;
    ofResetElapsedTimeCounter();
  }
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}

void ofApp::onButtonEvent(ofxDatGuiButtonEvent e) {
  button_sound.play();
  string label = e.target->getLabel();
  if (label == "START") {
    app_state = 1;
  } else if(label == "GALLERY") {
    app_state = 2;
  } else if (label == "RETURN") {
    app_state = 0;
  } else if (label == "EXIT") {
    //ofApp::exit();
  }
}
