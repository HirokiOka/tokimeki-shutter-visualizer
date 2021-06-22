#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
  logo_image.load("tokimeki_shutter_logo.png");
  kobe_image.load("kobe_univ.jpeg");
  button_sound.load("button_sound.mp3");
  ok_sound.load("ok.mp3");
  verdana.load("ofxbraitsch/fonts/Verdana.ttf", 32);

  for (int i = 0; i < 3; ++i) {
    string button_label;
    if (i == 0) {
      button_label = "START";
    } else if (i == 1) {
      button_label = "GALLERY";
    } else {
      button_label = "EXIT";
    }
    myButton = new ofxDatGuiButton(button_label);
    myButton->onButtonEvent(this, &ofApp::onButtonEvent);
    myButton->setHeight(80);
    component = myButton;
    component->setLabelAlignment(ofxDatGuiAlignment::CENTER);
    component->setPosition(ofGetWidth() / 2 - 150, ofGetHeight() / 2 + button_y_padding * i);
    component->setLabelColor(ofColor(0));
    component->setBackgroundColor(ofColor(0, 200, 0));
    component->setWidth(300, 200);
    component->setBorderVisible(false);
    components.push_back(component);
  }

  for (int i = 0; i < 8; ++i) {
    plotters[i] = new ofxDatGuiValuePlotter("label", 0, 100);
    plotters[i]->setDrawMode(ofxDatGuiGraph::LINES);
    plotters[i]->setSpeed(3);
    component = plotters[i];
    if (i < 4) {
      component->setPosition(0, i * 80 + 100);
    } else {
      component->setPosition(420, (i % 4) * 80 + 100);
    }
    component->setWidth(400, 100);
    plotter_components.push_back(component);
  }
}

//--------------------------------------------------------------
void ofApp::update(){
  if (app_state == 0) {
    for (int i = 0; i < 3; ++i) components[i]->update();
  } else if (app_state == 1){
    for (int i = 0; i < 8; ++i){
      float current_value = ofRandom(0, 100);
      plotters[i]->setValue(current_value);
      plotter_components[i]->update();
    } 
  }
}

//--------------------------------------------------------------
void ofApp::draw(){
  if (app_state == 0) {
    ofBackground(188, 226, 232);
    kobe_image.draw(0, 0, ofGetWidth(), ofGetHeight());
    ofSetColor(255);
    logo_image.draw(ofGetWidth() / 2 - x_offset, ofGetHeight() / 4 - y_offset, logo_width, logo_height);
    for (int i = 0; i < 3; ++i) components[i]->draw();
  } else if(app_state == 1) {
    ofSetColor(0);
    verdana.drawString("TOKIMEKI VISUALIZER", 80, 80);
    for (int i = 0; i < 8; ++i) plotter_components[i]->draw();
  }
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

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
  ok_sound.play();
  app_state++;
}
